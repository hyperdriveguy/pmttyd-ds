#include "native_harbor_finishing.hpp"
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t SequenceBase = 0x07000000, BattleFlags = 0xef4,
                   SpecialEnding = 0x00400000, Information = 0x2738,
                   Setup = 0xc, SetupFlags = 4, ReliefFirstFlag = 0x20000000,
                   MarioFlags = 0x138, ReliefVoiceMarioMask = 0x30000000,
                   UnitKind = 8, PartyInvisibleStatus = 0x12b, NpcCurrent = 8,
                   NpcFlags = 0x140, NpcEndingFlag = 0x100,
                   ReliefFirst = 0x802feafc, ReliefSecond = 0x802feb14,
                   VictoryPoseKey = 0x80427298, FadeColor = 0x80427144,
                   DefaultVolume = 0xf1194d80;
constexpr uint32_t Mario = 0x800f87a8, Party = 0x800f8728,
                   CheckStatus = 0x80127814, UnitSound = 0x8012619c,
                   BodyName = 0x801275f4, BodyType = 0x80127640,
                   AudienceVolume = 0x8019fddc, NpcWork = 0x8004829c,
                   FadeEntry = 0x80013368, BgmOff = 0x800daefc;
constexpr uint32_t EndingFadeType = 10, VictoryMusicOffFlags = 0x201,
                   AudienceEndVolume = 0, AudienceFadeArgument = 30,
                   UnitSoundBelonging = 0;
constexpr int32_t DeadStatus = 27, RestPoseType = 42, InvisiblePartyKind = 225,
                  VictoryPoseTicks = 60, FadeMilliseconds = 200;
int32_t signedByte(uint8_t v) {
  return v < 128 ? int32_t(v) : int32_t(v) - 256;
}
int32_t decrement(int32_t v) {
  const auto n = uint32_t(v) - 1;
  return n <= 0x7fffffff ? int32_t(n) : int32_t(int64_t(n) - 0x100000000ll);
}
} // namespace
NativeHarborFinishing::NativeHarborFinishing(uint32_t b,
                                             NativeHarborFinishingServices s)
    : battle_(b), services_(std::move(s)) {}
bool NativeHarborFinishing::ready() const {
  return !invoking_ && phase_ == Phase::Ready && current();
}
bool NativeHarborFinishing::current() const {
  if (checking_)
    return false;
  struct Check {
    bool &value;
    explicit Check(bool &v) : value(v) { value = true; }
    ~Check() { value = false; }
  } check(checking_);
  return battle_ && services_.lifetime && services_.current &&
         services_.current(services_.user) && services_.ordinaryRewardDone &&
         services_.ordinaryRewardDone(services_.user);
}
std::optional<uint32_t> NativeHarborFinishing::call(
    uint32_t function, uint32_t site, std::initializer_list<uint32_t> args,
    SourceBattleVoiceCursor *voice, const uint32_t *color) {
  pending_ = function;
  if (!services_.call)
    return {};
  auto &q = request_;
  q = {};
  q.function = function;
  q.callsite = site;
  q.count = uint8_t(args.size());
  unsigned i = 0;
  for (auto v : args)
    q.arguments[i++] = v;
  q.voice = voice;
  q.fadeColor = color;
  return services_.call(services_.user, q);
}
bool NativeHarborFinishing::advance(uint64_t visit) {
  if (invoking_)
    return false;
  struct Guard {
    NativeHarborFinishing &c;
    explicit Guard(NativeHarborFinishing &v) : c(v) { c.invoking_ = true; }
    ~Guard() {
      c.row_.sequence = c.sequence_;
      c.row_.pending = c.pending_;
      c.row_.timer = c.timer_;
      c.invoking_ = false;
    }
  } guard(*this);
  if (!current())
    return false;
  if (hasVisit_) {
    if (visit < visit_ || (!visitDone_ && visit != visit_))
      return false;
    if (visit == visit_ && visitDone_)
      return true;
  }
  if (!hasVisit_ || visit != visit_) {
    hasVisit_ = true;
    visit_ = visit;
    visitDone_ = false;
    visitPhase_ = phase_;
    step_ = 0;
    row_ = {visit, sequence_, sequence_, 0, timer_};
  }
  auto complete = [&] {
    visitDone_ = true;
    pending_ = 0;
    return true;
  };
  auto word = [&](uint32_t at) -> std::optional<uint32_t> {
    pending_ = at;
    return services_.word ? services_.word(services_.user, at) : std::nullopt;
  };
  for (;;) {
    if (!current())
      return false;
    switch (visitPhase_) {
    case Phase::OrdinaryBranch:
      if (step_ == 0) {
        auto v = word(battle_ + BattleFlags);
        if (!v)
          return false;
        flags_ = *v;
        step_ = 1;
        break;
      }
      if (flags_ & SpecialEnding) {
        pending_ = 0x800374ac;
        return false;
      } // Explicit unsupported actual event branch.
      sequence_ = SequenceBase + 0x10;
      phase_ = Phase::UpgradeGate;
      return complete();
    case Phase::UpgradeGate:
      // Source E30&~1 is private/native presentation state here; no guest E
      // allocation exists. ordinaryRewardDone proves the no-level-up E0c
      // domain.
      sequence_ = SequenceBase + 0x19;
      phase_ = Phase::VictoryPose;
      return complete();
    case Phase::VictoryPose:
      switch (step_) {
      case 0: {
        auto v = word(battle_ + Information);
        if (!v)
          return false;
        info_ = *v;
        ++step_;
        break;
      }
      case 1: {
        auto v = word(info_ + Setup);
        if (!v)
          return false;
        setup_ = *v;
        ++step_;
        break;
      }
      case 2: {
        auto v = call(Mario, 0x802163b4, {battle_});
        if (!v)
          return false;
        mario_ = *v;
        ++step_;
        break;
      }
      case 3: {
        auto v = call(Party, 0x802163c4, {battle_});
        if (!v)
          return false;
        party_ = *v;
        ++step_;
        break;
      }
      case 4: {
        auto v = word(mario_ + MarioFlags);
        if (!v)
          return false;
        flags_ = *v;
        relief_ = (flags_ & ReliefVoiceMarioMask) != 0;
        step_ = relief_ || !party_ ? 6 : 5;
        break;
      }
      case 5: {
        auto v = call(CheckStatus, 0x80216400, {party_, uint32_t(DeadStatus)});
        if (!v)
          return false;
        relief_ = *v != 0;
        step_ = 6;
        break;
      }
      case 6:
        if (!relief_) {
          step_ = 8;
          break;
        } else {
          auto v = word(setup_ + SetupFlags);
          if (!v)
            return false;
          key_ = (*v & ReliefFirstFlag) ? ReliefFirst : ReliefSecond;
          step_ = 7;
          break;
        }
      case 7: {
        auto v =
            call(UnitSound, key_ == ReliefFirst ? 0x80216438 : 0x80216454,
                 {mario_, key_, DefaultVolume, UnitSoundBelonging}, &voice_);
        if (!v)
          return false;
        step_ = 8;
        break;
      }
      case 8: {
        auto v = call(Mario, 0x8021645c, {battle_});
        if (!v)
          return false;
        mario_ = *v;
        ++step_;
        break;
      }
      case 9: {
        auto v = call(BodyName, 0x80216464, {mario_, VictoryPoseKey});
        if (!v)
          return false;
        ++step_;
        break;
      }
      default:
        timer_ = VictoryPoseTicks;
        sequence_ = SequenceBase + 0x1a;
        phase_ = Phase::PoseWait;
        return complete();
      }
      break;
    case Phase::PoseWait:
      switch (step_) {
      case 0:
        timer_ = decrement(timer_);
        step_ = 1;
        break;
      case 1:
        if (timer_ > 0)
          return complete();
        step_ = 2;
        break;
      case 2: {
        auto v = call(Mario, 0x8021649c, {battle_});
        if (!v)
          return false;
        mario_ = *v;
        ++step_;
        break;
      }
      case 3: {
        auto v = call(BodyType, 0x802164a4, {mario_, uint32_t(RestPoseType)});
        if (!v)
          return false;
        ++step_;
        break;
      }
      case 4: {
        auto v = call(Party, 0x802164ac, {battle_});
        if (!v)
          return false;
        party_ = *v;
        step_ = party_ ? 5 : 10;
        break;
      }
      case 5: {
        auto v = call(Party, 0x802164bc, {battle_});
        if (!v)
          return false;
        party_ = *v;
        ++step_;
        break;
      }
      case 6: {
        auto v = word(party_ + UnitKind);
        if (!v)
          return false;
        step_ = *v == uint32_t(InvisiblePartyKind) ? 7 : 8;
        break;
      }
      case 7: {
        pending_ = party_ + PartyInvisibleStatus;
        auto v = services_.byte ? services_.byte(services_.user, pending_)
                                : std::nullopt;
        if (!v)
          return false;
        step_ = signedByte(*v) >= 1 ? 10 : 8;
        break;
      }
      case 8: {
        auto v = call(Party, 0x802164e0, {battle_});
        if (!v)
          return false;
        party_ = *v;
        ++step_;
        break;
      }
      case 9: {
        auto v = call(BodyType, 0x802164e8, {party_, uint32_t(RestPoseType)});
        if (!v)
          return false;
        ++step_;
        break;
      }
      default:
        sequence_ = SequenceBase + 0x1b;
        phase_ = Phase::AfterPose;
        return complete();
      }
      break;
    case Phase::AfterPose:
      sequence_ = SequenceBase + 0x1c;
      phase_ = Phase::AudienceAndNpc;
      return complete();
    case Phase::AudienceAndNpc:
      switch (step_) {
      case 0:
        sequence_ = SequenceBase + 0x1d;
        phase_ = Phase::FadeEntry;
        ++step_;
        break;
      case 1: {
        auto v = call(AudienceVolume, 0x80216538,
                      {AudienceEndVolume, AudienceFadeArgument});
        if (!v)
          return false;
        ++step_;
        break;
      }
      case 2: {
        auto v = call(NpcWork, 0x8021653c, {});
        if (!v)
          return false;
        npcWork_ = *v;
        ++step_;
        break;
      }
      case 3: {
        auto v = word(npcWork_ + NpcCurrent);
        if (!v)
          return false;
        npc_ = *v;
        if (!npc_)
          return complete();
        ++step_;
        break;
      }
      case 4: {
        auto v = word(npc_ + NpcFlags);
        if (!v)
          return false;
        flags_ = *v;
        ++step_;
        break;
      }
      default:
        pending_ = 0x80216554;
        if (!services_.writeWord ||
            !services_.writeWord(services_.user, npc_ + NpcFlags,
                                 flags_ | NpcEndingFlag))
          return false;
        return complete();
      }
      break;
    case Phase::FadeEntry:
      switch (step_) {
      case 0: {
        auto v = word(FadeColor);
        if (!v)
          return false;
        color_ = *v;
        ++step_;
        break;
      }
      case 1: {
        auto v = call(FadeEntry, 0x80216570,
                      {EndingFadeType, uint32_t(FadeMilliseconds)}, nullptr,
                      &color_);
        if (!v)
          return false;
        fade_.enter(FadeMilliseconds);
        fadeVisit_ = visit_;
        ++step_;
        break;
      }
      case 2:
        sequence_ = SequenceBase + 0x1e;
        phase_ = Phase::FadeWait;
        ++step_;
        break;
      default: {
        auto v = call(BgmOff, 0x8021658c, {VictoryMusicOffFlags});
        if (!v)
          return false;
        return complete();
      }
      }
      break;
    case Phase::FadeWait:
      if (step_ == 0) {
        pending_ = 0x80012058;
        auto rate = services_.refreshRate
                        ? services_.refreshRate(services_.user)
                        : std::nullopt;
        if (!rate)
          return false;
        rate_ = *rate;
        ++step_;
        break;
      }
      if (step_ == 1) {
        pending_ = 0x80012058;
        if (rate_ <= 0)
          return false; // Captured unsupported finite domain.
        fade_.tick(rate_);
        ++step_;
        break;
      }
      if (!fade_.finished)
        return complete();
      sequence_ = SequenceBase + 0x1f;
      phase_ = Phase::OpaqueWait;
      return complete();
    case Phase::OpaqueWait: {
      pending_ = 0;
      auto shown = services_.opaquePresented
                       ? services_.opaquePresented(services_.user, fadeVisit_)
                       : std::nullopt;
      if (!shown)
        return false;
      if (*shown)
        phase_ = Phase::Ready;
      return complete();
    }
    case Phase::Ready:
      return complete();
    }
  }
}
} // namespace ttyd

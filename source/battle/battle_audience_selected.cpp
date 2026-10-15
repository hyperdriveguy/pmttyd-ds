#include "battle_audience_selected.hpp"
#include "battle_audience_item_on.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
bool identity(const BattleAudienceSelectedInput &a,
              const BattleAudienceSelectedInput &b) {
  return a.owner && a.owner == b.owner && a.audience == b.audience &&
         a.target == b.target;
}
bool valid(const BattleAudienceSelectedInput &a) {
  return a.owner && a.audience && a.target.slot < 200 && a.target.item < 100 &&
         a.target.actorGeneration && a.target.itemGeneration;
}
bool lifetime(const BattleAudienceSelectedInput &input,
              const BattleAudienceSelectedLive &live) {
  return valid(input) && input.owner == live.owner &&
         input.audience == live.audience &&
         input.target == live.candidate.target;
}
bool same(const BattleAudienceSelectedServices &a,
          const BattleAudienceSelectedServices &b) {
  const auto &x = a.memory.memory;
  const auto &y = b.memory.memory;
  return a.user == b.user && a.memory.user == b.memory.user &&
         a.memory.byte == b.memory.byte &&
         a.memory.audienceLifecycle.user == b.memory.audienceLifecycle.user &&
         a.memory.audienceLifecycle.event == b.memory.audienceLifecycle.event && x.user == y.user &&
         x.word == y.word && x.half == y.half && x.storeWord == y.storeWord &&
         x.storeHalf == y.storeHalf && x.storeByte == y.storeByte &&
         x.flushBlock == y.flushBlock && x.systemCall == y.systemCall &&
         a.mario == b.mario && a.endActMenu == b.endActMenu &&
         a.lightOff == b.lightOff;
}
} // namespace
std::optional<BattleAudienceSelectedHolder>
inspectSelectedBattleAudienceHolder(const BattleAudienceSelectedInput &input,
                                    const BattleAudienceSelectedLive &live,
                                    const BattleActorServices &a) {
  if (!lifetime(input, live) || !live.candidate.visible ||
      !live.candidate.dangerous || !live.candidate.interruptible)
    return {};
  const auto &m = a.memory;
  if (!m.word || !m.half)
    return {};
  const auto base = currentBattleAddress(a) + 0x2858;
  const auto memberBase = currentBattleAddress(a) + 0x2858;
  if (base != input.audience || memberBase != base)
    return {};
  const auto member = memberBase + 0x1bc + input.target.slot * 0x134;
  const auto flags = m.word(m.user, member);
  if (!(flags & 1) || (flags & 0x102))
    return {};
  const auto held = m.half(m.user, member + 0x1c);
  if (held == 0xffff || uint32_t(held) != input.target.item)
    return {};
  const auto itemBase = currentBattleAddress(a) + 0x2858;
  if (itemBase != base)
    return {};
  const auto item = itemBase + 0xf25c + uint32_t(held) * 0x48;
  const auto state = integer(m.word(m.user, item + 4));
  if (state > 4 && state != 7)
    return {};
  const auto kind = m.word(m.user, item + 0x10);
  if (integer(kind) < 236 || integer(kind) >= 240)
    return {};
  return BattleAudienceSelectedHolder{input, member, item, kind};
}
bool cacheSelectedBattleAudienceHolder(const BattleAudienceSelectedInput &input,
                                       const BattleAudienceSelectedLive &live,
                                       const BattleActorServices &a) {
  const auto inspected = inspectSelectedBattleAudienceHolder(input, live, a);
  if (!inspected)
    return false;
  const auto &holder = *inspected;
  const auto &m = a.memory;
  if (!m.storeWord || currentBattleAddress(a) + 0x2858 != input.audience)
    return false;
  const auto base = holder.input.audience, r = holder.member;
  m.storeWord(m.user, base + 0x138bc, holder.input.target.slot);
  m.storeWord(m.user, base + 0x138c0, m.word(m.user, r + 0x48));
  m.storeWord(m.user, base + 0x138c4, m.word(m.user, r + 0x4c));
  const auto raw = m.word(m.user, r + 0x50),
             offset = m.word(m.user, 0x804249cc);
  float z, d;
  std::memcpy(&z, &raw, 4);
  std::memcpy(&d, &offset, 4);
  const float sum = z + d;
  uint32_t bits;
  std::memcpy(&bits, &sum, 4);
  m.storeWord(m.user, base + 0x138c8, bits);
  return true;
}
struct BattleAudienceSelectedCursor::State {
  BattleAudienceSelectedServices owner;
  BattleAudienceSelectedInput input;
  uint32_t battle = 0;
  enum class Phase {
    BattleGuard,
    Mario,
    Query,
    Target,
    End,
    Type,
    Light,
    Flags
  } phase = Phase::BattleGuard;
};
BattleAudienceSelectedCursor::BattleAudienceSelectedCursor() = default;
BattleAudienceSelectedCursor::~BattleAudienceSelectedCursor() = default;
bool BattleAudienceSelectedCursor::active() const { return bool(state_); }
void BattleAudienceSelectedCursor::reset() { state_.reset(); }
std::optional<bool> resumeSelectedBattleAudienceInterruption(
    const BattleAudienceSelectedInput &input,
    const BattleAudienceSelectedLive &live,
    const BattleAudienceSelectedServices &s,
    BattleAudienceSelectedCursor &cursor) {
  if (cursor.state_ && (!identity(cursor.state_->input, input) ||
                        !same(cursor.state_->owner, s)))
    return {};
  const auto finish = [&](bool result) -> std::optional<bool> {
    cursor.reset();
    return result;
  };
  if (!lifetime(input, live))
    return finish(false);
  const auto &m = s.memory.memory;
  if (!m.word || !m.half || !m.storeWord || !m.storeHalf || !m.storeByte ||
      !s.memory.byte)
    return {};
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleAudienceSelectedCursor::State>();
    auto &c = *cursor.state_;
    c.owner = s;
    c.input = input;
    c.battle = currentBattleAddress(s.memory);
    if (c.battle + 0x2858 != input.audience)
      return finish(false);
  }
  auto &c = *cursor.state_;
  using P = BattleAudienceSelectedCursor::State::Phase;
  const auto word = [&](uint32_t p) { return m.word(m.user, p); };
  const auto put = [&](uint32_t p, uint32_t n) { m.storeWord(m.user, p, n); };
  for (;;)
    switch (c.phase) {
    case P::BattleGuard:
      if (word(c.battle + 0xef4) & 0x40000000u)
        return finish(false);
      c.phase = P::Mario;
      break;
    case P::Mario: {
      if (!s.mario)
        return {};
      const auto mario = s.mario(s.user, c.battle);
      if (!mario)
        return {};
      if (word(*mario + 0x104) & 0x10)
        return finish(false);
      c.phase = P::Query;
      break;
    }
    case P::Query: {
      if (!cacheSelectedBattleAudienceHolder(input, live, s.memory))
        return finish(false);
      c.phase = P::Target;
      break;
    }
    case P::Target:
      if (currentBattleAddress(s.memory) + 0x2858 != input.audience)
        return finish(false);
      packedBattleAudienceSetTarget(int32_t(input.target.slot), s.memory);
      c.phase = P::End;
      break;
    case P::End:
      if (!s.endActMenu || !s.endActMenu(s.user, c.battle))
        return {};
      c.phase = P::Type;
      break;
    case P::Type: {
      const auto previous = word(c.battle + 0x1c5c);
      put(c.battle + 0x1c60, previous);
      put(c.battle + 0x1c5c, 11);
      put(c.battle + 0x171c, 29);
      c.phase = P::Light;
      break;
    }
    case P::Light:
      if (!s.lightOff || !s.lightOff(s.user, 0, 1))
        return {};
      c.phase = P::Flags;
      break;
    case P::Flags:
      put(c.battle + 0xef4, word(c.battle + 0xef4) | 4u);
      return finish(true);
    }
}
} // namespace ttyd

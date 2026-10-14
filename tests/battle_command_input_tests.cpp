#include "battle_command_input.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
using F = BattleCommandService;
struct Fixture {
  static constexpr uint32_t B = 0x10000, U = 0x40000, M = B + 0x171c,
                            C = 0x80000, P = 0x90000, W = 0xa0000, E = 0xb0000;
  std::map<uint32_t, uint32_t> words{
      {B + 0x420, 1},       {P, C},         {C, 0}, {M + 0x80, W},
      {W + 0x70, 0x123456}, {E + 0x15c, 99}};
  std::map<uint32_t, uint8_t> bytes;
  std::vector<BattleCommandInputRequest> calls;
  std::vector<uint32_t> byteReads;
  std::function<std::optional<uint32_t>(const BattleCommandInputRequest &,
                                        uint32_t)>
      onRequest;
  std::function<void()> onTarget;
  std::function<void(uint32_t, uint32_t)> onStore;
  bool live = true;
  unsigned targets = 0;
  BattleCommandInputServices services() {
    BattleCommandInputServices s;
    s.user = this;
    s.privateCursor = P;
    s.memory.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.words[a] = v;
      if (f.onStore)
        f.onStore(a, v);
    };
    s.memory.memory.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[a] = v;
    };
    s.memory.byte = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.byteReads.push_back(a);
      return f.bytes[a];
    };
    s.request =
        [](void *p,
           const BattleCommandInputRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      f.calls.push_back(r);
      uint32_t result = 0;
      if (r.function == F::Unit)
        result = U;
      if (r.function == F::Cursor && r.arguments[1] != 99)
        f.words[r.arguments[2]] = C;
      if (r.function == F::Entry)
        result = E;
      if (r.function == F::EventLive)
        result = f.live;
      return f.onRequest ? f.onRequest(r, result)
                         : std::optional<uint32_t>(result);
    };
    s.target = [](void *p, uint32_t b, uint32_t u, uint32_t out) {
      auto &f = *static_cast<Fixture *>(p);
      assert(b == B && u == U && out == P);
      ++f.targets;
      if (f.onTarget)
        f.onTarget();
      return true;
    };
    return s;
  }
  std::optional<int32_t> run(uint32_t state) {
    words[M] = state;
    return packedBattleCommandInput(B, services());
  }
  unsigned count(F fn) const {
    unsigned n = 0;
    for (auto &r : calls)
      if (r.function == fn)
        ++n;
    return n;
  }
  const BattleCommandInputRequest &last(F fn) const {
    for (auto i = calls.rbegin(); i != calls.rend(); ++i)
      if (i->function == fn)
        return *i;
    assert(false);
    return calls[0];
  }
};
int main() {
  constexpr auto B = Fixture::B, U = Fixture::U, M = Fixture::M, C = Fixture::C,
                 W = Fixture::W;
  for (auto state : {30u, 0xffffffffu}) {
    Fixture f;
    assert(f.run(state) == 0 && f.calls.size() == 4);
    assert(f.calls[0].function == F::Unit && f.calls[1].function == F::Cursor &&
           f.calls[2].function == F::PresentItem &&
           f.calls[3].function == F::Restore);
  }
  Fixture wrapped;
  assert(packedBattleCommandInput(0u - 0x171c, wrapped.services()) == 1 &&
         wrapped.calls.size() == 1 && wrapped.calls[0].function == F::Unit);
  Fixture first;
  first.words[M + 0x540] = 11;
  first.words[M + 0x544] = 2;
  first.bytes[B + 0x1c74] = 2;
  first.bytes[M + 0x551] = 255;
  first.words[B + 0xef8] = 0x80;
  assert(first.run(0) == 0 && first.words[M] == 2 &&
         first.words[M + 0x540] == 2 && first.bytes[M + 0x551] == 0 &&
         first.bytes[B + 0x1c74] == 0 && first.words[B + 0xef8] == 0x81);
  assert(first.count(F::ActSetup) == 1 &&
         first.last(F::ActSetup).arguments[1] == 0 && !first.count(F::ActMain));
  Fixture bypass;
  bypass.onRequest = [&](const auto &r, uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::CommandPose)
      bypass.bytes[U + 0x20] = 5;
    return v;
  };
  assert(bypass.run(0) == 0 && bypass.words[M] == 5 &&
         !bypass.count(F::ActSetup) && bypass.count(F::LightOn) == 1);
  Fixture resume;
  assert(resume.run(1) == 0 && resume.words[M] == 2 &&
         resume.last(F::ActSetup).arguments[1] == 1);
  // A nonzero Main progresses inside one call; the next dispatch clears P.
  Fixture progress;
  progress.onRequest = [&](const auto &r,
                           uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::ActMain) {
      progress.words[M] = 5;
      return 1;
    }
    return v;
  };
  assert(progress.run(2) == 0 && progress.words[M] == 7 &&
         progress.count(F::WeaponMain) == 1);
  for (auto state : {3u, 8u, 17u, 22u}) {
    Fixture reason;
    assert(reason.run(state) == 0 && reason.words[M] == state + 1 &&
           reason.words[M + 0x568] == 99 && reason.count(F::Entry) == 1 &&
           reason.count(F::EventLive) == 1);
    const auto &entry = reason.last(F::Entry);
    assert(entry.count == 3 && entry.arguments[0] == 0x80416dc4 &&
           entry.arguments[1] == 0 && entry.arguments[2] == 32);
    assert(reason.last(F::EventLive).arguments[0] == 99);
  }
  for (auto state : {4u, 9u, 18u, 23u}) {
    Fixture wait;
    wait.words[M + 0x568] = 72;
    assert(wait.run(state) == 0 && !wait.count(F::Entry) &&
           wait.last(F::EventLive).arguments[0] == 72);
    Fixture done;
    done.live = false;
    done.words[M + 0x568] = 72;
    assert(done.run(state) == 0 && done.words[M + 0x568] == 0 &&
           done.words[M] == (state == 4    ? 2u
                             : state == 9  ? 7u
                             : state == 18 ? 16u
                                           : 21u));
  }
  Fixture changedId;
  changedId.onStore = [&](uint32_t a, uint32_t) {
    if (a == M + 0x568)
      changedId.words[a] = 321;
  };
  assert(changedId.run(3) == 0 &&
         changedId.last(F::EventLive).arguments[0] == 321);
  for (auto state : {5u, 6u})
    for (auto type : {0u, 2u})
      for (auto badge : {0u, 1u}) {
        Fixture f;
        f.words[M + 0x540] = type;
        f.bytes[U + 0x2f3] = uint8_t(badge);
        assert(f.run(state) == 0 && f.words[M] == 7 &&
               f.count(F::WeaponMain) == 1);
        assert(f.last(F::WeaponSetup).arguments[2] ==
               (state == 5 && !(type == 2 && badge) ? 1u : 0u));
        if (type != 2 || state == 6 || badge)
          for (auto a : f.byteReads)
            assert(a != U + 0x2f4);
      }
  Fixture secondBadge;
  secondBadge.words[M + 0x540] = 2;
  secondBadge.bytes[U + 0x2f4] = 1;
  assert(secondBadge.run(5) == 0 &&
         secondBadge.last(F::WeaponSetup).arguments[2] == 0);
  for (auto type : {0u, 1u}) {
    Fixture immediate;
    immediate.words[M + 0x540] = type;
    immediate.words[B + 0xef4] = type == 0 ? 0x200 : 0x400;
    assert(immediate.run(5) == 0 && immediate.words[M] == 10 &&
           immediate.count(F::TargetSetup) == 1 &&
           !immediate.count(F::WeaponSetup) &&
           immediate.last(F::WeaponDecide).arguments[1] == type &&
           immediate.last(F::WeaponDecide).arguments[2] == 0);
  }
  Fixture setupMutation;
  setupMutation.words[M + 0x540] = 2;
  setupMutation.onRequest = [&](const auto &r,
                                uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::WeaponSetup)
      setupMutation.words[M + 0x540] = 4;
    return v;
  };
  assert(setupMutation.run(5) == 0 &&
         setupMutation.last(F::WeaponMain).arguments[1] == 4);
  Fixture help;
  help.words[M + 0x540] = 99;
  help.words[C] = 1;
  help.words[M + 0x80 + 28] = W + 0x100;
  help.words[W + 0x100 + 0x70] = 0xabcdef;
  assert(help.run(10) == 0 && help.words[M] == 11 &&
         help.count(F::Cursor) == 2 &&
         help.last(F::HelpKey).arguments[0] == 0xabcdef &&
         help.last(F::HelpDisplay).arguments[0] == 1 && !help.targets);
  Fixture target;
  assert(target.run(11) == 0 && target.targets == 1);
  Fixture multiCancel;
  assert(multiCancel.run(12) == 0 && multiCancel.words[M] == 13 &&
         !multiCancel.count(F::MultiCancelMain));
  Fixture multiMain;
  assert(multiMain.run(13) == 0 && multiMain.count(F::MultiCancelMain) == 1);
  for (auto state : {14u, 15u, 19u, 20u, 24u, 25u}) {
    Fixture pair;
    const auto setup = state <= 15   ? F::MultiSetup
                       : state <= 20 ? F::OperationSetup
                                     : F::PartySetup;
    const auto main = state <= 15   ? F::MultiMain
                      : state <= 20 ? F::OperationMain
                                    : F::PartyMain;
    pair.onRequest = [&](const auto &r, uint32_t v) -> std::optional<uint32_t> {
      if (r.function == setup)
        pair.words[M] = 99;
      return v;
    };
    assert(pair.run(state) == 0 &&
           pair.words[M] == (state <= 15   ? 16u
                             : state <= 20 ? 21u
                                           : 26u) &&
           pair.count(main) == 1 &&
           pair.last(setup).arguments[1] ==
               (state == 14 || state == 19 || state == 24 ? 1u : 0u));
  }
  for (auto state : {16u, 21u, 26u}) {
    Fixture direct;
    assert(direct.run(state) == 0 &&
           direct.count(state == 16   ? F::MultiMain
                        : state == 21 ? F::OperationMain
                                      : F::PartyMain) == 1);
  }
  Fixture protect;
  assert(protect.run(27) == 0 && protect.words[M] == 28 &&
         protect.count(F::ProtectSetup) == 1 && protect.count(F::Trigger) == 2);
  Fixture cancel;
  cancel.onRequest = [](const auto &r, uint32_t v) -> std::optional<uint32_t> {
    return r.function == F::Trigger ? std::optional<uint32_t>(r.arguments[0])
                                    : std::optional<uint32_t>(v);
  };
  assert(cancel.run(28) == 0 && cancel.words[M] == 20 &&
         cancel.count(F::Trigger) == 1 &&
         cancel.last(F::Sound).arguments[0] == 2521);
  Fixture confirm;
  confirm.onRequest = [&](const auto &r,
                          uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::Trigger)
      return r.arguments[0] == 0x100 ? 1 : 0;
    if (r.function == F::ProtectEnd)
      assert(confirm.words[M] == 29);
    return v;
  };
  assert(confirm.run(28) == 0 && confirm.words[M] == 29 &&
         !confirm.count(F::Sound) && !confirm.count(F::HelpDisplay));
  Fixture finish;
  finish.bytes[B + 0x1c74] = 1;
  finish.onRequest = [&](const auto &r, uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::SelectAttack) {
      assert(r.arguments[0] == U && r.arguments[1] == M);
      finish.words[M + 4] = 0x01000081;
      finish.words[B + 0xef8] = 0x101;
    }
    return v;
  };
  assert(finish.run(29) == 1 && finish.words[M + 4] == 0x81 &&
         finish.words[B + 0xef8] == 0x100 && finish.count(F::Restore) == 1 &&
         finish.last(F::LightOff).arguments[0] == 0 &&
         finish.last(F::LightOff).arguments[1] == 0);
  // Pending re-entry must preserve P1, while the subsequent normal loop clears
  // it.
  Fixture pending;
  unsigned restores = 0;
  pending.onRequest = [&](const auto &r,
                          uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::Restore && ++restores == 2)
      pending.words[M] = 11;
    if (r.function == F::ActMain) {
      pending.words[M] = 7;
      pending.bytes[B + 0x1c74] = 1;
      return 1;
    }
    return v;
  };
  pending.onTarget = [&] {
    pending.bytes[B + 0x1c74] = 0;
    pending.words[M] = 29;
  };
  assert(pending.run(2) == 1 && pending.targets == 1 &&
         pending.count(F::SelectAttack) == 1 && !pending.count(F::WeaponMain));
  Fixture missing;
  missing.onRequest = [](const auto &r, uint32_t v) -> std::optional<uint32_t> {
    return r.function == F::CommandPose ? std::nullopt
                                        : std::optional<uint32_t>(v);
  };
  assert(!missing.run(0) && missing.words[B + 0xef8] == 1 &&
         !missing.count(F::ActSetup));
  Fixture retained;
  retained.onRequest = [&](const auto &r,
                           uint32_t v) -> std::optional<uint32_t> {
    if (r.function == F::PresentItem) {
      retained.words[B + 0x420] = 9;
      return 1;
    }
    if (r.function == F::CommandPose)
      assert(r.arguments[0] == U);
    return v;
  };
  assert(retained.run(0) == 0 && retained.count(F::Unit) == 1 &&
         retained.count(F::HelpDisplay) == 1 &&
         retained.last(F::HelpDisplay).arguments[0] == 0);
}

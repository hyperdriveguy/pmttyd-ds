#include "battle_act_menu.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
using F = BattleActService;
struct Fixture {
  static constexpr uint32_t B = 0x10000, M = B + 0x171c, U = 0x30000,
                            V = 0x40000, C = 0x50000, D = 0x60000, P = 0x70000;
  std::map<uint32_t, uint32_t> w{
      {B + 0x420, 1}, {P, C}, {C + 8, 1}, {M + 0xc, 1}, {M + 0x1720, 1}};
  std::map<uint32_t, uint8_t> bytes{
      {U + 0x22, 1}, {U + 0x23, 1}, {V + 0x22, 1}, {V + 0x23, 1}};
  std::vector<BattleActRequest> calls;
  std::function<std::optional<uint32_t>(const BattleActRequest &, uint32_t)>
      onCall;
  uint32_t buttons = 0, repeats = 0;
  bool allowed = true, position = false;
  BattleActMenuServices services() {
    BattleActMenuServices s;
    s.user = this;
    s.privateCursor = P;
    s.memory.memory.user = this;
    s.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->w[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->w[a] = v;
    };
    s.memory.memory.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[a] = v;
    };
    s.memory.byte = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->bytes[a];
    };
    s.request = [](void *p,
                   const BattleActRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      f.calls.push_back(r);
      uint32_t n = 0;
      switch (r.function) {
      case F::Unit:
      case F::Mario:
        n = U;
        break;
      case F::Party:
        n = V;
        break;
      case F::Cursor:
        f.w[r.arguments[2]] = C;
        break;
      case F::Allowed:
        n = f.allowed;
        break;
      case F::Position:
        n = f.position;
        break;
      case F::Trigger:
        n = f.buttons & r.arguments[0];
        break;
      case F::Repeat:
        n = f.repeats & r.arguments[0];
        break;
      default:
        break;
      }
      return f.onCall ? f.onCall(r, n) : std::optional<uint32_t>(n);
    };
    return s;
  }
  unsigned count(F fn) const {
    unsigned n = 0;
    for (auto &r : calls)
      if (r.function == fn)
        ++n;
    return n;
  }
  std::optional<int32_t> run() {
    return mainPackedBattleActMenu(B, services());
  }
};
int main() {
  constexpr auto B = Fixture::B, M = Fixture::M, U = Fixture::U, V = Fixture::V,
                 C = Fixture::C, D = Fixture::D, P = Fixture::P;
  Fixture setup;
  setup.w[M + 0x510] = 77;
  setup.w[M + 0x498] = 99;
  for (auto o : {0x4b0u, 0x4bcu, 0x4c8u, 0x4d4u, 0x4e0u, 0x4ecu, 0x4f8u, 0x504u,
                 0x51cu}) {
    setup.w[M + o] = 4;
    setup.w[M + o + 4] = 5;
    setup.w[M + o + 8] = 6;
  }
  setup.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::Init) {
      assert(r.arguments[1] == C && r.arguments[2] == M + 8 &&
             r.arguments[3] == 0);
      setup.w[M + 4] = 0x40;
    }
    return n;
  };
  assert(setupPackedBattleActMenu(B, 0, setup.services()) &&
         setup.w[M + 4] == 0x01000041 && setup.w[M + 0x510] == 77 &&
         setup.w[M + 0x498] == 99);
  for (auto o :
       {0x4b0u, 0x4bcu, 0x4c8u, 0x4d4u, 0x4e0u, 0x4ecu, 0x4f8u, 0x504u, 0x51cu})
    assert(setup.w[M + o] == 0 && setup.w[M + o + 4] == 0 &&
           setup.w[M + o + 8] == 6);
  Fixture skip;
  skip.w[M + 0x4b0] = 9;
  skip.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::Init)
      skip.bytes[M + 0x558] = 1;
    return n;
  };
  assert(setupPackedBattleActMenu(B, 0, skip.services()) &&
         skip.w[M + 0x4b0] == 9);
  Fixture end;
  end.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::End)
      end.w[M + 4] = 0x61000003;
    return n;
  };
  assert(endPackedBattleActMenu(B, end.services()) &&
         end.w[M + 4] == 0x61000002);
  Fixture gate;
  gate.w[V + 0x104] = 0x10;
  assert(checkPackedBattleChangePosition(B, gate.services()) == 1 &&
         gate.count(F::Status) ==
             8); // Party has no Mario-only word restriction.
  Fixture flags;
  flags.w[B + 0xef4] = 0x00800000;
  assert(checkPackedBattleChangePosition(B, flags.services()) == 0 &&
         flags.calls.empty());
  Fixture shortStatus;
  shortStatus.onCall = [](const auto &r,
                          uint32_t n) -> std::optional<uint32_t> {
    return r.function == F::Status && r.arguments[1] == 9 ? 1 : n;
  };
  assert(checkPackedBattleChangePosition(B, shortStatus.services()) == 0 &&
         shortStatus.count(F::Status) == 3 && !shortStatus.count(F::Party));
  Fixture signedMoves;
  signedMoves.bytes[U + 0x23] = 255;
  signedMoves.bytes[U + 0x22] = 255;
  assert(checkPackedBattleChangePosition(B, signedMoves.services()) == 0 &&
         !signedMoves.count(F::Status));
  Fixture admission;
  admission.allowed = false;
  assert(admission.run() == 0 && admission.count(F::Display) == 1 &&
         !admission.count(F::Now));
  Fixture early;
  early.buttons = 0x800;
  early.position = true;
  assert(early.run() == 1 && early.w[M] == 29 && early.w[M + 0x540] == 9 &&
         !early.count(F::Allowed) && early.count(F::Display) == 1);
  for (auto type : {0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u,
                    14u, 0xffffffffu}) {
    Fixture f;
    f.buttons = 0x100;
    f.w[M + 8] = type;
    assert(f.run() ==
           (type == 0 || type == 1 || (type >= 4 && type <= 8) || type == 13
                ? 1
                : 0));
    assert(f.w[M + 0x540] == type && f.count(F::End) == 1 &&
           f.count(F::Display) == 1);
    if (type == 2)
      assert(f.w[M] == 5 && f.bytes[M + 0x550] == 1);
  }
  Fixture special;
  special.buttons = 0x100;
  special.w[M + 8] = 2;
  special.bytes[U + 0x2f4] = 1;
  assert(special.run() == 1 && special.w[M] == 14);
  Fixture changed;
  changed.buttons = 0x100;
  changed.w[M + 8] = 0;
  changed.w[M + 28] = 13;
  changed.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::Sound && r.arguments[0] == 2520)
      changed.w[C] = 1;
    return n;
  };
  assert(changed.run() == 1 && changed.w[M + 0x540] == 13 &&
         changed.w[M] == 29);
  Fixture reason;
  reason.buttons = 0x100;
  reason.w[M + 0xc] = 0;
  reason.w[M + 0x18] = 4;
  reason.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::End)
      reason.w[M + 0x18] = 77;
    return n;
  };
  assert(reason.run() == 1 && reason.w[M] == 3 && reason.w[M + 0x564] == 77);
  Fixture check;
  check.w[B + 0xef4] = 0x200;
  check.w[M + 8] = 0;
  check.w[M + 0x18] = 17;
  assert(check.run() == 0 && check.w[M + 0xc] == 0 && check.w[M + 0x18] == 17 &&
         check.count(F::WeaponTable) == 1 && check.count(F::TargetCount) == 1);
  Fixture timer;
  timer.w[M + 0x560] = 899;
  assert(timer.run() == 0 && timer.w[M + 0x560] == 900 &&
         timer.count(F::CameraSpeed) == 1);
  Fixture wrap;
  wrap.w[M + 0x560] = 0x7fffffff;
  assert(wrap.run() == 0 && wrap.w[M + 0x560] == 0x80000000 &&
         !wrap.count(F::CameraMode));
  Fixture forward;
  forward.repeats = 0xc0000;
  forward.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::Sound) {
      assert(r.arguments[0] == 0x802efef4);
      forward.w[P] = D;
      forward.w[D] = 8;
    }
    return n;
  };
  assert(forward.run() == 0 && forward.w[D] == 0 &&
         forward.count(F::Repeat) == 1 && !forward.count(F::Audience));
  Fixture reverse;
  reverse.repeats = 0x40000;
  reverse.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::Sound) {
      reverse.w[P] = D;
      reverse.w[D + 8] = 0;
    }
    return n;
  };
  assert(reverse.run() == 0 && reverse.w[D] == 0xffffffff);
  Fixture late;
  unsigned triggers = 0, positions = 0;
  late.onCall = [&](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    if (r.function == F::Trigger && r.arguments[0] == 0x800) {
      ++triggers;
      return 1;
    }
    if (r.function == F::Position)
      return ++positions == 2 ? 1 : 0;
    return n;
  };
  assert(late.run() == 1 && triggers == 2 && positions == 2 &&
         late.w[M + 0x540] == 9);
  Fixture pending;
  pending.bytes[B + 0x1c74] = 1;
  pending.w[B + 0xef4] = 0x200;
  pending.w[M + 8] = 0;
  assert(pending.run() == 0 && pending.bytes[B + 0x1c74] == 0 &&
         pending.w[B + 0x1c70] == 0);
  Fixture missing;
  missing.onCall = [](const auto &r, uint32_t n) -> std::optional<uint32_t> {
    return r.function == F::Init ? std::nullopt : std::optional<uint32_t>(n);
  };
  assert(!setupPackedBattleActMenu(B, 0, missing.services()) &&
         missing.w[M + 4] == 0);
}

#include "battle_act_menu_continuation.hpp"
#include <cassert>
#include <map>
#include <set>
#include <vector>
using namespace ttyd;
using F = BattleActService;
namespace {
constexpr uint32_t B = 0x10000, M = B + 0x171c, U = 0x30000, V = 0x40000,
                   C = 0x50000, D = 0x60000, P = 0x70000, W = 0x80000,
                   Old = 0x90000;
struct Fixture {
  std::map<uint32_t, uint32_t> w{
      {B + 0x420, 7},        {C + 8, 1},   {M + 12, 1}, {M + 4, 1}, {P, C},
      {0x80415fd0, 0xa0000}, {0xa0004, 60}};
  std::map<uint32_t, uint8_t> bytes{
      {U + 0x22, 1}, {U + 0x23, 1}, {V + 0x22, 1}, {V + 0x23, 1}};
  std::vector<std::array<uint32_t, 7>> trace;
  std::map<F, uint32_t> returns{{F::Allowed, 1}};
  std::set<uint32_t> statuses;
  uint32_t buttons = 0, repeats = 0, now = 0, mutation = 0, allocationWord = W;
  unsigned accepted = 0, hold = 0, positionGets = 0;
  bool unknown = false, reenter = false;
  std::optional<uint32_t> *slot = nullptr;
  BattleActMenuContinuationCursor *menuCursor = nullptr;
  BattleActUiContinuationCursor *uiCursor = nullptr;
  uint32_t word(uint32_t a) {
    const auto v = w[a];
    if (a != P)
      trace.push_back({0, a, v});
    return v;
  }
  void put(uint32_t a, uint32_t v) {
    w[a] = v;
    if (a != P)
      trace.push_back({1, a, v});
    if (mutation == 8 && a == B + 0x1c78)
      w[a] = D;
    if (mutation == 9 && a == W + 0x38) {
      w[C + 8] = 2;
      w[0xa0004] = 30;
    }
  }
  uint8_t byte(uint32_t a) {
    const auto v = bytes[a];
    trace.push_back({2, a, v});
    return v;
  }
  void pb(uint32_t a, uint8_t v) {
    bytes[a] = v;
    trace.push_back({3, a, v});
  }
  void cursor(uint32_t c) {
    w[P] = c;
    if (slot)
      *slot = c;
  }
  BattleActorServices memory() {
    BattleActorServices s;
    s.user = this;
    s.memory.user = this;
    s.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->word(a);
    };
    s.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->put(a, v);
    };
    s.memory.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(p)->pb(a, v);
    };
    s.byte = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->byte(a);
    };
    return s;
  }
  std::optional<uint32_t> invoke(F fn, std::array<uint32_t, 4> a,
                                 uint8_t count) {
    if (reenter && menuCursor) {
      assert(!resumePackedBattleActMenuMain(B, native(), *menuCursor));
      assert(!menuCursor->reset());
    }
    if (hold && accepted + 1 == hold)
      return {};
    ++accepted;
    trace.push_back({4, uint32_t(fn), a[0], a[1], a[2], a[3], count});
    uint32_t n = returns[fn];
    switch (fn) {
    case F::Unit:
      n = U;
      break;
    case F::Mario:
      n = (mutation == 7 && ++positionGets == 1) ? 0 : U;
      break;
    case F::Party:
      n = V;
      break;
    case F::Status:
      n = statuses.count(a[1]) ? 1 : 0;
      break;
    case F::Cursor:
      if (!unknown)
        cursor(C);
      break;
    case F::Trigger:
      n = buttons & a[0];
      break;
    case F::Repeat:
      n = repeats & a[0];
      break;
    case F::Now:
      n = now;
      break;
    default:
      break;
    }
    if (mutation == 1 && fn == F::Sound && a[0] == 2520)
      w[C] = 1;
    if (mutation == 2 && fn == F::End) {
      w[M + 24] = 77;
      w[M + 4] = 0x41000003;
    }
    if (mutation == 3 && fn == F::Sound) {
      cursor(D);
      w[D] = 8;
      w[D + 8] = 0;
    }
    if (mutation == 4 && fn == F::TargetCount) {
      w[C + 8] = 1;
      w[M + 8] = 1;
      w[M + 24] = 19;
    }
    if (mutation == 5 && fn == F::Init) {
      w[M + 4] = 0x40;
      bytes[M + 0x558] = 1;
    }
    if (mutation == 6 && fn == F::CameraMode) {
      bytes[B + 0x1c74] = 1;
      w[B + 0xef4] = 0x200;
    }
    return n;
  }
  BattleActMenuServices immediate() {
    BattleActMenuServices s;
    s.memory = memory();
    s.user = this;
    s.privateCursor = P;
    s.request = [](void *p,
                   const BattleActRequest &r) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (r.function == F::Position) {
        auto result =
            checkPackedBattleChangePosition(r.arguments[0], f.immediate());
        assert(result);
        return uint32_t(*result);
      }
      auto args = r.arguments;
      auto count = r.count;
      if (r.function == F::Cursor) {
        args[2] = 0;
        count = 2;
      }
      return f.invoke(r.function, args, count);
    };
    return s;
  }
  BattleActMenuContinuationServices native() {
    BattleActMenuContinuationServices s;
    s.memory = memory();
    s.user = this;
    s.request = [](void *p, const BattleActMenuContinuationRequest &r)
        -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (r.function == F::Cursor) {
        assert(r.privateCursorOutput);
        if (f.slot)
          assert(f.slot == r.privateCursorOutput);
        f.slot = r.privateCursorOutput;
      } else
        assert(!r.privateCursorOutput);
      return f.invoke(r.function, r.arguments, r.count);
    };
    return s;
  }
  BattleActUiServices ui() {
    BattleActUiServices s;
    s.memory = memory();
    s.user = this;
    s.release = [](void *p, uint32_t old) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter) {
        assert(f.uiCursor);
        assert(!resumePackedBattleActUiEnd(B, f.ui(), *f.uiCursor));
        assert(!f.uiCursor->reset());
      }
      if (f.hold && f.accepted + 1 == f.hold)
        return false;
      ++f.accepted;
      f.trace.push_back({5, old});
      return true;
    };
    s.allocate = [](void *p, uint32_t n) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold && f.accepted + 1 == f.hold)
        return {};
      ++f.accepted;
      f.trace.push_back({6, n});
      return f.allocationWord;
    };
    return s;
  }
};
enum class Entry { Main, Setup, End, Position };
std::optional<int32_t> run(Fixture &f, Entry e, uint32_t flag,
                           BattleActMenuContinuationCursor &c) {
  switch (e) {
  case Entry::Main:
    return resumePackedBattleActMenuMain(B, f.native(), c);
  case Entry::Setup:
    return resumePackedBattleActMenuSetup(B, flag, f.native(), c)
               ? std::optional<int32_t>(0)
               : std::nullopt;
  case Entry::End:
    return resumePackedBattleActMenuEnd(B, f.native(), c)
               ? std::optional<int32_t>(0)
               : std::nullopt;
  case Entry::Position:
    return resumePackedBattleChangePosition(B, f.native(), c);
  }
  return {};
}
void compare(Fixture initial, Entry e = Entry::Main, uint32_t flag = 0) {
  auto reference = initial;
  std::optional<int32_t> result;
  switch (e) {
  case Entry::Main:
    result = mainPackedBattleActMenu(B, reference.immediate());
    break;
  case Entry::Setup:
    assert(setupPackedBattleActMenu(B, flag, reference.immediate()));
    result = 0;
    break;
  case Entry::End:
    assert(endPackedBattleActMenu(B, reference.immediate()));
    result = 0;
    break;
  case Entry::Position:
    result = checkPackedBattleChangePosition(B, reference.immediate());
    break;
  }
  assert(result);
  const auto count = reference.accepted;
  for (unsigned hold = 0; hold <= count; ++hold) {
    auto f = initial;
    f.hold = hold;
    BattleActMenuContinuationCursor c;
    if (hold) {
      assert(!run(f, e, flag, c));
      const auto prefix = f.trace;
      const auto words = f.w;
      const auto bytes = f.bytes;
      assert(c.active() && c.pending());
      assert(!run(f, e, flag, c) && prefix == f.trace && words == f.w &&
             bytes == f.bytes);
      auto wrong = f.native();
      wrong.memory.audienceLifecycle.user = &reference;
      assert(!resumePackedBattleActMenuMain(B, wrong, c) && prefix == f.trace);
      assert(!resumePackedBattleActMenuMain(B + 4, f.native(), c) &&
             prefix == f.trace);
    }
    f.hold = 0;
    assert(run(f, e, flag, c) == result);
    assert(!c.active() && !c.pending());
    assert(f.trace == reference.trace && f.w == reference.w &&
           f.bytes == reference.bytes);
  }
}
void compareUi(Fixture initial, bool ending, uint32_t flag) {
  auto reference = initial;
  assert(ending
             ? endPackedBattleActUi(B, reference.ui())
             : initializePackedBattleActUi(B, C, M + 8, flag, reference.ui()));
  for (unsigned hold = 0; hold <= reference.accepted; ++hold) {
    auto f = initial;
    f.hold = hold;
    BattleActUiContinuationCursor c;
    auto run = [&]() {
      return ending ? resumePackedBattleActUiEnd(B, f.ui(), c)
                    : resumePackedBattleActUiInit(B, C, M + 8, flag, f.ui(), c);
    };
    if (hold) {
      assert(!run());
      const auto trace = f.trace;
      const auto words = f.w;
      assert(!run() && trace == f.trace && words == f.w);
      auto wrong = f.ui();
      wrong.memory.audienceLifecycle.user = &reference;
      assert(!resumePackedBattleActUiInit(B, C, M + 8, flag, wrong, c));
    }
    f.hold = 0;
    assert(run());
    assert(!c.active() && !c.pending());
    assert(f.trace == reference.trace && f.w == reference.w &&
           f.bytes == reference.bytes);
  }
}
struct AudienceFixture {
  Fixture memory;
  BattleActMenuAudienceAdmissionCursor *cursor = nullptr;
  bool holdItem = false, holdEnd = false, recurse = false;
  unsigned triggers = 0, marioGets = 0, items = 0, targets = 0, ends = 0,
           lights = 0;
  AudienceFixture() { memory.w[0x8041eb90] = B; }
  BattleMenuAudienceServices services() {
    BattleMenuAudienceServices s;
    s.memory = memory.memory();
    s.user = this;
    s.privateItem = P;
    s.trigger = [](void *p, uint32_t mask) -> uint32_t {
      auto &f = *static_cast<AudienceFixture *>(p);
      ++f.triggers;
      assert(mask == 0x400);
      return 0x400;
    };
    s.mario = [](void *p, uint32_t b) -> uint32_t {
      auto &f = *static_cast<AudienceFixture *>(p);
      ++f.marioGets;
      assert(b == B);
      return U;
    };
    s.itemOn = [](void *p, uint32_t output, int32_t a, int32_t b, int32_t c,
                  int32_t d) {
      auto &f = *static_cast<AudienceFixture *>(p);
      assert(output == P && a == 0 && b == 0 && c == 0 && d == 0);
      if (f.recurse) {
        assert(!resumeAdmittedBattleActMenuAudience(f.services(), *f.cursor));
        assert(!f.cursor->reset());
      }
      if (f.holdItem)
        return false;
      ++f.items;
      f.memory.w[output] = 17;
      return true;
    };
    s.setTarget = [](void *p, int32_t slot) {
      ++static_cast<AudienceFixture *>(p)->targets;
      assert(slot == 17);
    };
    s.endActMenuReady = [](void *p, uint32_t b) {
      auto &f = *static_cast<AudienceFixture *>(p);
      assert(b == B);
      if (f.recurse) {
        assert(!resumeAdmittedBattleActMenuAudience(f.services(), *f.cursor));
        assert(!f.cursor->reset());
      }
      if (f.holdEnd)
        return false;
      ++f.ends;
      f.memory.w[B + 0x1c5c] = 8;
      return true;
    };
    s.lightOff = [](void *p, int32_t a, int32_t b) {
      ++static_cast<AudienceFixture *>(p)->lights;
      assert(a == 0 && b == 1);
    };
    return s;
  }
};
} // namespace
int main() {
  compare(Fixture{});
  for (uint32_t flag : {0u, 1u, 2u, 0xffffffffu}) {
    Fixture f;
    for (uint32_t o = 0x498; o <= 0x528; o += 4)
      f.w[M + o] = 7;
    compare(f, Entry::Setup, flag);
    f.mutation = 5;
    compare(f, Entry::Setup, flag);
  }
  Fixture end;
  end.mutation = 2;
  compare(end, Entry::End);
  for (uint32_t type = 0; type < 16; ++type)
    for (uint32_t special : {0u, 1u, 2u, 3u}) {
      Fixture f;
      f.buttons = 0x100;
      f.w[M + 8] = type;
      if (special == 1)
        f.bytes[U + 0x20] = 255;
      if (special == 2)
        f.bytes[U + 0x2f3] = 1;
      if (special == 3)
        f.bytes[U + 0x2f4] = 1;
      compare(f);
    }
  for (uint32_t kind : {0u, 1u, 0xffffffffu})
    for (uint32_t target : {0u, 1u, 0xffffffffu})
      for (uint32_t reason : {0u, 9u}) {
        Fixture f;
        f.w[M + 8] = kind;
        f.w[B + 0xef4] = 0x600;
        f.returns[F::TargetCount] = target;
        f.w[M + 24] = reason;
        compare(f);
      }
  for (uint32_t timer : {0u, 898u, 899u, 900u, 0x7fffffffu, 0xffffffffu})
    for (uint32_t now : {0u, 1u}) {
      Fixture f;
      f.w[M + 0x560] = timer;
      f.now = now;
      compare(f);
    }
  for (uint32_t enabled : {0u, 1u})
    for (uint32_t pending : {0u, 1u})
      for (uint32_t reason : {0u, 4u}) {
        Fixture f;
        f.buttons = 0x100;
        f.w[M + 12] = enabled;
        f.bytes[B + 0x1c74] = uint8_t(pending);
        f.w[M + 24] = reason;
        compare(f);
      }
  for (uint32_t count : {0u, 1u, 2u, 0xffffffffu})
    for (uint32_t index : {0u, 1u, 0xffffffffu})
      for (uint32_t repeat : {0u, 0x40000u, 0x80000u, 0xc0000u}) {
        Fixture f;
        f.w[C + 8] = count;
        f.w[C] = index;
        f.repeats = repeat;
        compare(f);
      }
  for (uint32_t mutation = 1; mutation <= 7; ++mutation) {
    Fixture f;
    f.mutation = mutation;
    if (mutation == 1) {
      f.buttons = 0x100;
      f.w[M + 28] = 13;
    }
    if (mutation == 2) {
      f.buttons = 0x100;
      f.w[M + 12] = 0;
      f.w[M + 24] = 4;
    }
    if (mutation == 3)
      f.repeats = 0xc0000;
    if (mutation == 4) {
      f.w[B + 0xef4] = 0x200;
      f.w[C + 8] = 3;
    }
    if (mutation == 6) {
      f.now = 1;
      f.w[M + 8] = 0;
    }
    if (mutation == 7)
      f.buttons = 0x800;
    compare(f);
  }
  for (uint32_t flag : {0u, 0x00800000u, 0x40000000u})
    for (uint32_t status : {0u, 1u, 2u, 9u, 27u}) {
      Fixture f;
      f.w[B + 0xef4] = flag;
      if (status)
        f.statuses.insert(status);
      compare(f, Entry::Position);
    }
  for (uint32_t a : {0u, 1u, 127u, 128u, 255u}) {
    Fixture f;
    f.bytes[U + 0x23] = uint8_t(a);
    f.bytes[U + 0x22] = uint8_t(a);
    compare(f, Entry::Position);
  }
  Fixture forward;
  forward.repeats = 0xc0000;
  forward.mutation = 3;
  compare(forward);
  Fixture reverse;
  reverse.repeats = 0x40000;
  reverse.mutation = 3;
  compare(reverse);
  Fixture audience;
  audience.returns[F::Audience] = 1;
  compare(audience);
  Fixture locked;
  locked.returns[F::Allowed] = 0;
  compare(locked);
  for (uint32_t old : std::array<uint32_t, 2>{0, Old})
    for (uint32_t flag : {0u, 1u})
      for (uint32_t count : {0u, 1u, 3u})
        for (uint32_t rate : {60u, 1u, 0xffffffffu}) {
          Fixture f;
          f.w[B + 0x1c78] = old;
          f.w[C + 8] = count;
          f.w[0xa0004] = rate;
          compareUi(f, false, flag);
          compareUi(f, true, flag);
        }
  // Allocate may wait after the old work was already freed: never free it
  // twice.
  Fixture allocation;
  allocation.w[B + 0x1c78] = Old;
  allocation.hold = 2;
  BattleActUiContinuationCursor ui;
  assert(!resumePackedBattleActUiInit(B, C, M + 8, 0, allocation.ui(), ui));
  assert(ui.pending() == 0x800f8934);
  allocation.w[B + 0x1c78] = 0xdead;
  allocation.hold = 0;
  assert(resumePackedBattleActUiInit(B, C, M + 8, 0, allocation.ui(), ui));
  unsigned frees = 0;
  for (const auto &r : allocation.trace)
    if (r[0] == 5) {
      ++frees;
      assert(r[1] == Old);
    }
  assert(frees == 1);
  for (uint32_t mutation : {8u, 9u}) {
    Fixture alias;
    alias.mutation = mutation;
    alias.w[C + 8] = 3;
    compareUi(alias, false, 0);
    compareUi(alias, false, 1);
  }
  // The original caller has no null-allocation guard. This map fixture models
  // guest address0 explicitly; it does not claim address0 is a valid native UI.
  Fixture nullAllocation;
  nullAllocation.allocationWord = 0;
  compareUi(nullAllocation, false, 0);
  // Unknown stack output is not fabricated. Source early position can avoid it.
  Fixture unknown;
  unknown.unknown = true;
  BattleActMenuContinuationCursor c;
  assert(!resumePackedBattleActMenuMain(B, unknown.native(), c));
  assert(c.pending() == uint32_t(F::Cursor));
  const auto prefix = unknown.trace;
  assert(!resumePackedBattleActMenuMain(B, unknown.native(), c) &&
         unknown.trace == prefix);
  assert(unknown.slot);
  *unknown.slot = C;
  assert(resumePackedBattleActMenuMain(B, unknown.native(), c) == 0);
  Fixture bypass;
  bypass.unknown = true;
  bypass.buttons = 0x800;
  assert(resumePackedBattleActMenuMain(B, bypass.native(), c) == 1);
  Fixture noUse;
  noUse.unknown = true;
  noUse.returns[F::Allowed] = 0;
  assert(resumePackedBattleActMenuMain(B, noUse.native(), c) == 0);
  Fixture setupUnknown;
  setupUnknown.unknown = true;
  assert(!resumePackedBattleActMenuSetup(B, 0, setupUnknown.native(), c));
  assert(c.pending() == uint32_t(F::Cursor) && setupUnknown.w[M + 4] == 1);
  assert(setupUnknown.slot);
  *setupUnknown.slot = C;
  assert(resumePackedBattleActMenuSetup(B, 0, setupUnknown.native(), c));
  Fixture re;
  re.reenter = true;
  re.menuCursor = &c;
  assert(resumePackedBattleActMenuMain(B, re.native(), c) == 0);
  Fixture uiRe;
  uiRe.reenter = true;
  uiRe.uiCursor = &ui;
  uiRe.w[B + 0x1c78] = Old;
  assert(resumePackedBattleActUiInit(B, C, M + 8, 0, uiRe.ui(), ui));
  for (bool item : {false, true}) {
    AudienceFixture audience, other;
    BattleActMenuAudienceAdmissionCursor admitted;
    audience.cursor = &admitted;
    audience.holdItem = item;
    audience.holdEnd = !item;
    audience.recurse = true;
    assert(!resumeAdmittedBattleActMenuAudience(audience.services(), admitted));
    assert(admitted.pending() == (item ? 0x801a2d58u : 0x801220d4u));
    const auto trace = audience.memory.trace;
    const auto words = audience.memory.w;
    assert(!resumeAdmittedBattleActMenuAudience(audience.services(), admitted));
    assert(trace == audience.memory.trace && words == audience.memory.w);
    auto wrong = audience.services();
    wrong.user = &other;
    assert(!resumeAdmittedBattleActMenuAudience(wrong, admitted));
    wrong = audience.services();
    wrong.itemOn = [](void *, uint32_t, int32_t, int32_t, int32_t, int32_t) {
      return true;
    };
    assert(!resumeAdmittedBattleActMenuAudience(wrong, admitted));
    wrong = audience.services();
    wrong.endActMenuReady = [](void *, uint32_t) { return true; };
    assert(!resumeAdmittedBattleActMenuAudience(wrong, admitted));
    wrong = audience.services();
    wrong.memory.audienceLifecycle.user = &other;
    assert(!resumeAdmittedBattleActMenuAudience(wrong, admitted));
    wrong = audience.services();
    wrong.privateItem += 4;
    assert(!resumeAdmittedBattleActMenuAudience(wrong, admitted));
    assert(trace == audience.memory.trace && words == audience.memory.w);
    audience.holdItem = audience.holdEnd = false;
    assert(resumeAdmittedBattleActMenuAudience(audience.services(), admitted) ==
           true);
    assert(!admitted.active() && !admitted.pending());
    assert(audience.triggers == 1 && audience.marioGets == 1 &&
           audience.items == 1 && audience.targets == 1 && audience.ends == 1 &&
           audience.lights == 1);
    assert(audience.memory.w[B + 0x1c60] == 8 &&
           audience.memory.w[B + 0x1c5c] == 11 && audience.memory.w[M] == 29 &&
           (audience.memory.w[B + 0xef4] & 4));
  }
  return 0;
}

#include "battle_act_continuation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t B = 0x10000, U = 0x40000, M = 0x50000, E = 0x60000;
uint32_t bits(float f) {
  uint32_t n;
  std::memcpy(&n, &f, 4);
  return n;
}
int sb(uint32_t n) {
  return uint8_t(n) < 128 ? int(uint8_t(n)) : int(uint8_t(n)) - 256;
}
int32_t sw(uint32_t n) {
  return n < 0x80000000u ? int32_t(n) : int32_t(int64_t(n) - 0x100000000LL);
}
BattleActChild child(uint32_t fn) {
  switch (fn) {
  case 0x800f88cc:
    return BattleActChild::Unit;
  case 0x800f87a8:
    return BattleActChild::Mario;
  case 0x80144d44:
    return BattleActChild::NiceAsync;
  case 0x801a82d4:
    return BattleActChild::AudienceInit;
  case 0x80127814:
    return BattleActChild::Status;
  case 0x80125380:
    return BattleActChild::CommandInit;
  case 0x8012c0e8:
    return BattleActChild::CameraSpeed;
  case 0x801229d8:
    return BattleActChild::CommandInput;
  case 0x8012733c:
    return BattleActChild::UnitData;
  case 0x800f75cc:
    return BattleActChild::WindowOff;
  case 0x800374ac:
    return BattleActChild::CreateEvent;
  case 0x80005da0:
    return BattleActChild::Random;
  case 0x80036664:
    return BattleActChild::DeleteEvent;
  case 0x8011ace8:
    return BattleActChild::WaitEvents;
  case 0x801958bc:
    return BattleActChild::AdjustMessages;
  case 0x801956f8:
    return BattleActChild::Messages;
  case 0x80195af8:
    return BattleActChild::Announcements;
  case 0x800f7624:
    return BattleActChild::WindowOn;
  case 0x80127bac:
    return BattleActChild::SetStatus;
  case 0x801277f8:
    return BattleActChild::CheckStatusFlag;
  case 0x801277d8:
    return BattleActChild::ClearStatusFlag;
  case 0x80128144:
    return BattleActChild::Belong;
  case 0x8011bb88:
    return BattleActChild::PhaseEnd;
  case 0x800f74b4:
    return BattleActChild::ConsumeItems;
  case 0x801a7e94:
    return BattleActChild::AudiencePerAct;
  case 0x801a7e30:
    return BattleActChild::AudienceReaction;
  case 0x8011aa34:
    return BattleActChild::Concluded;
  case 0x80203394:
    return BattleActChild::BreakSlotReaction;
  case 0x80036604:
    return BattleActChild::EventExists;
  default:
    assert(false);
    return BattleActChild::Unit;
  }
}
struct Fixture {
  std::map<uint32_t, uint32_t> mem;
  std::vector<std::array<uint32_t, 8>> trace;
  std::set<uint32_t> statuses;
  std::map<uint32_t, uint32_t> returns;
  float speed = 0;
  unsigned accepted = 0, hold = 0, draw = 0;
  std::vector<uint32_t> random{90, 90};
  bool absent = false, reenter = false;
  BattleActCursor *cursor = nullptr;
  void seed(unsigned state) {
    mem[B + 0xf20] = 0x06000000 + state;
    mem[B + 0xf1c] = 0x05000009;
    mem[B + 0x420] = 7;
    mem[U] = 7;
    mem[U + 8] = 222;
    mem[U + 0x22] = 1;
    mem[U + 0x24] = 9;
    mem[U + 0x2a0] = 0x71000;
    mem[U + 0x2a4] = 0x72000;
    mem[U + 0x2c8] = 0x73000;
    mem[U + 0x290] = 82;
    mem[U + 0x2c] = 0x04000002;
    mem[U + 0x2cc] = 83;
    mem[E + 0x15c] = 84;
    mem[0x80422584] = bits(2.0f);
    returns[0x801229d8] = 1;
    returns[0x8011ace8] = 1;
    returns[0x800374ac] = E;
  }
  uint32_t word(uint32_t a) {
    auto v = mem[a];
    trace.push_back({0, a, v});
    return v;
  }
  uint8_t byte(uint32_t a) {
    auto v = uint8_t(mem[a]);
    trace.push_back({1, a, v});
    return v;
  }
  void put(uint32_t a, uint32_t v) {
    mem[a] = v;
    trace.push_back({2, a, v});
  }
  void pb(uint32_t a, uint8_t v) {
    mem[a] = v;
    trace.push_back({3, a, v});
  }
  BattleActServices services() {
    BattleActServices s;
    s.user = this;
    s.actors.user = this;
    s.actors.memory.user = this;
    s.actors.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->word(a);
    };
    s.actors.byte = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->byte(a);
    };
    s.actors.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->put(a, v);
    };
    s.actors.memory.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(p)->pb(a, v);
    };
    s.call = [](void *p,
                const BattleActSequenceRequest &r) -> std::optional<BattleActOutput> {
      return static_cast<Fixture *>(p)->invoke(r);
    };
    return s;
  }
  std::optional<BattleActOutput> invoke(const BattleActSequenceRequest &r) {
    assert(r.child == child(r.function));
    if (reenter) {
      assert(cursor);
      assert(!resumePackedBattleAct(B, services(), *cursor));
      assert(!cursor->reset());
    }
    if (hold && accepted + 1 == hold)
      return {};
    ++accepted;
    trace.push_back({4, r.function, r.callsite, r.arguments[0], r.arguments[1],
                     r.arguments[2], r.arguments[3]});
    BattleActOutput out;
    switch (r.child) {
    case BattleActChild::Unit:
      out.value = absent ? 0 : U;
      break;
    case BattleActChild::Mario:
      out.value = M;
      break;
    case BattleActChild::CameraSpeed:
      out.real = speed;
      break;
    case BattleActChild::Status:
      out.value = statuses.count(r.arguments[1]) ? 1 : 0;
      break;
    case BattleActChild::Random:
      out.value = random[draw++ % random.size()];
      break;
    case BattleActChild::UnitData:
      out.value = 0x74000 + 4 * r.arguments[1];
      break;
    default:
      out.value = returns[r.function];
      break;
    }
    return out;
  }
  uint32_t call(uint32_t fn, uint32_t site, std::array<uint32_t, 4> a = {}) {
    BattleActSequenceRequest r;
    r.child = child(fn);
    r.function = fn;
    r.callsite = site;
    r.arguments = a;
    return invoke(r)->value;
  }
  uint32_t actor(uint32_t site) {
    const auto id = word(B + 0x420);
    return call(0x800f88cc, site, {B, id});
  }
  void set(unsigned state) { put(B + 0xf20, 0x06000000 + state); }
  void inc(unsigned channel = 6) {
    const auto a = B + (channel == 6 ? 0xf20 : 0xf1c);
    put(a, word(a) + 1);
  }
  // Independent immediate transcription: direct control flow, no continuation
  // state, no held requests, no production parent functions.
  void original() {
    const auto state = word(B + 0xf20) - 0x06000000u;
    uint32_t u = 0, e = 0, script = 0, flag = 0, first = 0, id = 0;
    switch (state) {
    case 0: {
      u = actor(0x8011b324);
      call(0x80144d44, 0x8011b334, {1});
      put(u + 0x284, 0);
      put(u + 0x2d0, 0);
      call(0x801a82d4, 0x8011b344);
      const auto kind = sw(word(u + 8));
      bool inhibited = true;
      if (kind == 222 || (kind >= 224 && kind < 231)) {
        const auto start = kind == 222 ? 0x8011b364u : 0x8011b3ccu;
        if (!call(0x80127814, start, {u, 1}) &&
            !call(0x80127814, start + 20, {u, 2}) &&
            !call(0x80127814, start + 40, {u, 9}) &&
            !call(0x80127814, start + 60, {u, 27}))
          inhibited = false;
      }
      if (inhibited) {
        set(3);
        return;
      }
      if (word(u + 0x104) & 16) {
        set(4);
        return;
      }
      if (word(u + 8) == 228) {
        auto mario = call(0x800f87a8, 0x8011b46c, {B});
        if (word(mario + 0x104) & 16) {
          set(4);
          return;
        }
      }
      if (sb(byte(u + 0x12b))) {
        set(5);
        return;
      }
      call(0x80125380, 0x8011b4bc, {B});
      set(1);
      return;
    }
    case 1: {
      BattleActSequenceRequest r;
      r.child = BattleActChild::CameraSpeed;
      r.function = 0x8012c0e8;
      r.callsite = 0x8011b4d8;
      const auto f = invoke(r)->real;
      uint32_t raw = word(0x80422584);
      float bound;
      std::memcpy(&bound, &raw, 4);
      if (f > bound)
        return;
      set(2);
    }
      [[fallthrough]];
    case 2:
      if (call(0x801229d8, 0x8011b500, {B})) {
        put(B + 0xef4, word(B + 0xef4) & ~128u);
        set(6);
      }
      return;
    case 3:
      u = actor(0x8011b574);
      put(u + 0x2ac, 1);
      put(u + 0x1c, word(u + 0x1c) | 0x04000000);
      set(6);
      return;
    case 4:
    case 5:
      u = actor(state == 4 ? 0x8011b5ac : 0x8011b538);
      script = call(0x8012733c, state == 4 ? 0x8011b5b8 : 0x8011b544,
                    {u, state == 4 ? 3u : 2u});
      put(u + 0x2a0, script);
      put(u + 0x2ac, state == 4 ? 27u : 1u);
      set(6);
      return;
    case 6: {
      auto before = actor(0x8011b5e8);
      if (!(word(before + 0x2ac) & 1))
        call(0x800f75cc, 0x8011b5f8);
      u = actor(0x8011b604);
      if (word(B + 0xef4) & 0x40000000)
        if (!(word(u + 0x104) & 0x2000)) {
          flag = word(u + 0x2ac);
          if (!(flag & 8) && !(flag & 16)) {
            e = call(0x800374ac, 0x8011b64c, {0x8034f234, 10, 0});
            goto bind;
          }
        }
      flag = word(u + 0x2ac);
      if (flag & 16) {
        script = word(u + 0x2a0);
        if (script)
          e = call(0x800374ac, 0x8011b678, {script, 10, 0});
        goto bind;
      }
      if (flag & 2) {
        script = word(u + 0x2a0);
        e = call(0x800374ac, 0x8011b698, {script, 10, 0});
        goto bind;
      }
      if (call(0x80127814, 0x8011b6ac, {u, 1}) ||
          call(0x80127814, 0x8011b6c0, {u, 2}) ||
          call(0x80127814, 0x8011b6d4, {u, 9}))
        goto bind;
      if (call(0x80127814, 0x8011b6e8, {u, 27}) && !(word(u + 0x104) & 0x10000))
        goto bind;
      if (call(0x80127814, 0x8011b708, {u, 6})) {
        if (sw(call(0x80005da0, 0x8011b718, {100})) < 0) {
          e = call(0x800374ac, 0x8011b734, {0x8034e0cc, 10, 0});
          goto bind;
        }
      }
      if (call(0x80127814, 0x8011b748, {u, 5}) &&
          sw(call(0x80005da0, 0x8011b758, {100})) < 50) {
        put(u + 0x27c, word(u + 0x27c) | 16);
        script = word(u + 0x2a4);
        e = call(0x800374ac, 0x8011b77c, {script, 10, 0});
      } else {
        script = word(u + 0x2a0);
        e = call(0x800374ac, 0x8011b794, {script, 10, 0});
      }
    bind:
      if (e) {
        put(u + 0x2a8, word(e + 0x15c));
        put(e + 0x160, word(u));
        id = word(u + 0x290);
        if (id) {
          call(0x80036664, 0x8011b7c0, {id});
          put(u + 0x290, 0);
        }
      }
      inc();
      return;
    }
    case 7:
      if (!call(0x8011ace8, 0x8011b7e0, {B}))
        return;
      inc();
      call(0x801958bc, 0x8011b7fc, {B});
      [[fallthrough]];
    case 8:
      if (!call(0x8011ace8, 0x8011b804, {B}))
        return;
      first = call(0x801956f8, 0x8011b814, {B});
      if (call(0x80195af8, 0x8011b820, {B}) | first)
        return;
      inc();
      [[fallthrough]];
    case 9:
      if (!(word(B + 0xef4) & 48))
        call(0x800f7624, 0x8011b844);
      u = actor(0x8011b850);
      if (u) {
        put(u + 0x1c, word(u + 0x1c) & ~0x04000000u);
        if (word(u + 0x27c) & 8) {
          call(0x80127bac, 0x8011b880, {u, 16, 1, uint32_t(-99)});
          put(u + 0x27c, word(u + 0x27c) & ~8u);
        }
        if (byte(B + 0x18ff9) == 1)
          pb(B + 0x18ff9, 0);
        put(u + 0x25c, 0);
        switch (sb(byte(u + 0x20))) {
        case 5:
          pb(u + 0x20, 6);
          break;
        case 6:
          pb(u + 0x20, 7);
          break;
        case 7:
        case 9:
          pb(u + 0x20, 0);
          break;
        case 8:
          pb(u + 0x20, 9);
          break;
        }
        const auto normalized = sb(byte(u + 0x20));
        if (!normalized) {
          (void)word(B + 0xef4);
          if (call(0x801277f8, 0x8011b940, {u, 0x02000000}))
            call(0x801277d8, 0x8011b954, {u, 0x02000000});
          else
            pb(u + 0x22, uint8_t(byte(u + 0x22) - 1));
          if (word(B + 0xef4) & 0x2000)
            pb(u + 0x22, 0);
          if (sb(byte(u + 0x22)) <= 0) {
            pb(u + 0x20, 10);
            flag = word(u + 0x27c);
            if (flag & 0x2000)
              put(u + 0x27c, flag & ~0x2000u);
            else
              pb(u + 0x24, 0);
            if (sb(call(0x80128144, 0x8011b9b4, {u})) == 0 &&
                word(u + 0x2c) == 0x04000002)
              if (!call(0x8011bb88, 0x8011b9d0) && !(word(B + 0xef4) & 0x2000))
                pb(u + 0x311, 96);
          }
        } else if (normalized == 3 || normalized == 4)
          pb(u + 0x20, 0);
      }
      call(0x800f74b4, 0x8011ba1c);
      put(B + 0xef4, word(B + 0xef4) & ~4u);
      call(0x801a7e94, 0x8011ba2c);
      inc();
      [[fallthrough]];
    case 10:
      if (uint8_t(call(0x801a7e30, 0x8011ba3c)))
        return;
      if (!call(0x8011aa34, 0x8011ba4c, {B}))
        if (uint8_t(call(0x80203394, 0x8011ba58)))
          return;
      inc();
      [[fallthrough]];
    case 11:
      first = call(0x801956f8, 0x8011ba74, {B});
      if (call(0x80195af8, 0x8011ba80, {B}) | first)
        return;
      inc();
      [[fallthrough]];
    case 12:
      if (!call(0x8011ace8, 0x8011ba9c, {B}))
        return;
      u = actor(0x8011bab0);
      if (u) {
        put(u + 0x2cc, 0);
        script = word(u + 0x2c8);
        if (script) {
          e = call(0x800374ac, 0x8011bad8, {script, 10, 0});
          if (e) {
            put(u + 0x2cc, word(e + 0x15c));
            put(e + 0x160, word(u));
            id = word(u + 0x290);
            if (id) {
              call(0x80036664, 0x8011bb00, {id});
              put(u + 0x290, 0);
            }
          }
        }
      }
      inc();
      [[fallthrough]];
    case 13:
      u = actor(0x8011bb20);
      if (u) {
        id = word(u + 0x2cc);
        if (id) {
          if (call(0x80036604, 0x8011bb38, {id}))
            return;
          put(u + 0x2cc, 0);
        }
      }
      inc();
      [[fallthrough]];
    case 14:
      if (call(0x8011ace8, 0x8011bb5c, {B}))
        inc(5);
      return;
    default:
      return;
    }
  }
};
void compare(Fixture input) {
  auto reference = input;
  reference.original();
  const auto count = reference.accepted;
  for (unsigned hold = 0; hold <= count; ++hold) {
    auto f = input;
    f.hold = hold;
    BattleActCursor cursor;
    if (hold) {
      assert(!resumePackedBattleAct(B, f.services(), cursor));
      const auto trace = f.trace;
      const auto mem = f.mem;
      const auto draws = f.draw;
      assert(cursor.active() && cursor.pending());
      assert(!resumePackedBattleAct(B, f.services(), cursor) &&
             trace == f.trace && mem == f.mem && draws == f.draw);
      auto wrong = f.services();
      wrong.actors.audienceLifecycle.user = &reference;
      assert(!resumePackedBattleAct(B, wrong, cursor) && trace == f.trace);
      assert(!resumePackedBattleAct(B + 4, f.services(), cursor) &&
             trace == f.trace);
    }
    f.hold = 0;
    assert(resumePackedBattleAct(B, f.services(), cursor));
    assert(!cursor.active() && !cursor.pending());
    assert(f.mem == reference.mem && f.trace == reference.trace &&
           f.draw == reference.draw);
  }
}
} // namespace
int main() {
  for (unsigned state = 0; state < 17; ++state) {
    Fixture f;
    f.seed(state);
    compare(f);
  }
  for (uint32_t kind :
       {221u, 222u, 223u, 224u, 228u, 230u, 231u, 0xffffffffu}) {
    for (uint32_t status : {0u, 1u, 2u, 9u, 27u}) {
      Fixture f;
      f.seed(0);
      f.mem[U + 8] = kind;
      if (status)
        f.statuses.insert(status);
      compare(f);
    }
  }
  for (unsigned route = 0; route < 5; ++route) {
    Fixture f;
    f.seed(0);
    f.mem[U + 8] = 228;
    if (route == 0)
      f.mem[U + 0x104] = 16;
    if (route == 1)
      f.mem[M + 0x104] = 16;
    if (route == 2)
      f.mem[U + 0x12b] = 255;
    if (route == 3)
      f.mem[U + 0x104] = 16, f.mem[U + 0x12b] = 255;
    compare(f);
  }
  for (float speed : {1.0f, 2.0f, 3.0f, std::nanf("")}) {
    Fixture f;
    f.seed(1);
    f.speed = speed;
    compare(f);
  }
  for (uint32_t ready : {0u, 1u}) {
    Fixture f;
    f.seed(2);
    f.returns[0x801229d8] = ready;
    f.mem[B + 0xef4] = 0x180;
    compare(f);
  }
  for (uint32_t flags : {0u, 1u, 2u, 8u, 16u, 18u, 24u})
    for (uint32_t gravity : {0u, 0x40000000u})
      for (uint32_t attribute : {0u, 0x2000u})
        for (bool failed : {false, true}) {
          Fixture f;
          f.seed(6);
          f.mem[U + 0x2ac] = flags;
          f.mem[B + 0xef4] = gravity;
          f.mem[U + 0x104] = attribute;
          if (failed)
            f.returns[0x800374ac] = 0;
          compare(f);
        }
  for (uint32_t status : {1u, 2u, 9u, 27u, 6u, 5u})
    for (uint32_t draw : {0u, 49u, 50u, 99u, 0xffffffffu}) {
      Fixture f;
      f.seed(6);
      f.statuses.insert(status);
      f.random = {draw, draw};
      compare(f);
      if (status == 27) {
        f.mem[U + 0x104] = 0x10000;
        compare(f);
      }
    }
  for (uint32_t flags : {2u, 16u}) {
    Fixture f;
    f.seed(6);
    f.mem[U + 0x2ac] = flags;
    f.mem[U + 0x2a0] = 0;
    compare(f);
  }
  for (unsigned state : {7u, 8u, 10u, 11u, 12u, 13u, 14u})
    for (uint32_t fn : {0x8011ace8u, 0x801956f8u, 0x80195af8u, 0x801a7e30u,
                        0x80203394u, 0x8011aa34u, 0x80036604u})
      for (uint32_t result : {0u, 1u, 0x100u}) {
        Fixture f;
        f.seed(state);
        f.returns[fn] = result;
        compare(f);
      }
  for (uint32_t state : {0u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 255u})
    for (uint32_t remaining : {0u, 1u, 2u, 128u, 255u})
      for (uint32_t flags : {0u, 8u, 0x2000u}) {
        Fixture f;
        f.seed(9);
        f.mem[U + 0x20] = state;
        f.mem[U + 0x22] = remaining;
        f.mem[U + 0x27c] = flags;
        f.mem[B + 0x18ff9] = 1;
        compare(f);
      }
  for (uint32_t fn : {0x801277f8u, 0x8011bb88u, 0x80128144u}) {
    Fixture f;
    f.seed(9);
    f.returns[fn] = 1;
    compare(f);
  }
  for (unsigned state : {9u, 12u, 13u}) {
    Fixture f;
    f.seed(state);
    f.absent = true;
    compare(f);
  }
  for (uint32_t bflags : {0x10u, 0x20u, 0x2000u}) {
    Fixture f;
    f.seed(9);
    f.mem[B + 0xef4] = bflags;
    compare(f);
  }
  for (bool noScript : {false, true}) {
    Fixture f;
    f.seed(12);
    if (noScript)
      f.mem[U + 0x2c8] = 0;
    else
      f.returns[0x800374ac] = 0;
    compare(f);
  }
  Fixture re;
  re.seed(6);
  BattleActCursor cursor;
  re.cursor = &cursor;
  re.reenter = true;
  assert(resumePackedBattleAct(B, re.services(), cursor));
  // The held WindowOff precedes the second independent actor-ID read.
  Fixture fresh;
  fresh.seed(6);
  fresh.hold = 2;
  assert(!resumePackedBattleAct(B, fresh.services(), cursor));
  fresh.mem[B + 0x420] = 19;
  fresh.hold = 0;
  assert(resumePackedBattleAct(B, fresh.services(), cursor));
  bool freshActor = false;
  for (const auto &r : fresh.trace)
    if (r[0] == 4 && r[2] == 0x8011b604 && r[4] == 19)
      freshActor = true;
  assert(freshActor);
  // A held actual actor call owns its earlier ID operand instead.
  Fixture captured;
  captured.seed(6);
  captured.hold = 3;
  assert(!resumePackedBattleAct(B, captured.services(), cursor));
  captured.mem[B + 0x420] = 19;
  captured.hold = 0;
  assert(resumePackedBattleAct(B, captured.services(), cursor));
  for (const auto &r : captured.trace)
    if (r[0] == 4 && r[2] == 0x8011b604)
      assert(r[4] == 7);
  // Script/ID operands read before an await remain captured; reads after
  // successful allocation retain the source freshness and ordering.
  Fixture allocation;
  allocation.seed(6);
  allocation.mem[U + 0x2ac] = 16;
  allocation.hold = 4;
  assert(!resumePackedBattleAct(B, allocation.services(), cursor));
  assert(cursor.pending() == 0x800374ac);
  allocation.mem[U + 0x2a0] = 0x99999;
  allocation.mem[E + 0x15c] = 543;
  allocation.mem[U] = 37;
  allocation.mem[U + 0x290] = 100;
  allocation.hold = 5;
  assert(!resumePackedBattleAct(B, allocation.services(), cursor));
  assert(cursor.pending() == 0x80036664);
  assert(allocation.mem[U + 0x2a8] == 543 && allocation.mem[E + 0x160] == 37);
  allocation.mem[U + 0x290] = 200;
  allocation.hold = 0;
  assert(resumePackedBattleAct(B, allocation.services(), cursor));
  bool oldScript = false, oldEvent = false;
  for (const auto &r : allocation.trace) {
    if (r[0] == 4 && r[1] == 0x800374ac)
      oldScript = r[3] == 0x71000;
    if (r[0] == 4 && r[1] == 0x80036664)
      oldEvent = r[3] == 100;
  }
  assert(oldScript && oldEvent && allocation.mem[U + 0x290] == 0);
  Fixture set;
  set.seed(9);
  set.mem[U + 0x27c] = 8;
  set.hold = 3;
  assert(!resumePackedBattleAct(B, set.services(), cursor));
  assert(cursor.pending() == 0x80127bac);
  set.mem[U + 0x27c] |= 0x80;
  set.mem[U + 0x22] = 3;
  set.hold = 0;
  assert(resumePackedBattleAct(B, set.services(), cursor));
  assert(set.mem[U + 0x27c] == 0x80 && set.mem[U + 0x22] == 2);
  Fixture missing;
  missing.seed(0);
  auto noProvider = missing.services();
  noProvider.call = nullptr;
  assert(!resumePackedBattleAct(B, noProvider, cursor));
  assert(missing.trace.size() == 2 && cursor.pending() == 0x800f88cc);
  const auto prefix = missing.trace;
  assert(!resumePackedBattleAct(B, noProvider, cursor) &&
         missing.trace == prefix);
  assert(cursor.reset());
  return 0;
}

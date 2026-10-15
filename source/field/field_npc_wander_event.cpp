#include "field_npc_wander_event.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Init = 0x80237878, Main = 0x802372a8, Random = 0x80005da0,
                   Revise = 0x80007328, Angle = 0x8000703c,
                   Compare = 0x800071f0, Ready = 0x80040750;
int32_t sw(uint32_t v) {
  return v <= INT32_MAX ? int32_t(v) : int32_t(int64_t(v) - 0x100000000LL);
}
uint32_t bits(float f) {
  uint32_t v;
  std::memcpy(&v, &f, 4);
  return v;
}
float real(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
} // namespace
bool fieldNpcWanderEventFunction(uint32_t fn) {
  return fn == Init || fn == Main;
}
int32_t fieldNpcWanderEvent(uint32_t fn, EventEntry &e, bool first,
                            uint32_t address, FieldNpcWanderEventCursor &c,
                            const FieldNpcWanderEventServices &s) {
  const auto &a = s.actor;
  if (!fieldNpcWanderEventFunction(fn) || !a.word || !a.value || !a.real ||
      !a.resolve || !a.store || !s.request || !s.work || !s.half ||
      !s.storeHalf)
    return 0;
  if (first)
    c = {};
  if (!c.initialized) {
    c.initialized = true;
    c.owner = a.user;
    c.entry = &e;
    c.id = e.id;
    c.function = fn;
    c.address = address;
    c.args = e.arguments;
  }
  if (c.owner != a.user || c.entry != &e || c.id != e.id || c.function != fn ||
      c.address != address)
    return 0;
  auto word = [&](uint32_t p) {
    uint32_t v = 0;
    const bool ok = a.word(a.user, p, v);
    assert(ok);
    (void)ok;
    return v;
  };
  auto load = [&](uint32_t p) { return real(word(p)); };
  auto store = [&](uint32_t p, uint32_t v) { a.store(a.user, p, v); };
  auto save = [&](uint32_t p, float v) { store(p, bits(v)); };
  auto half = [&](uint32_t p) {
    uint16_t v = 0;
    const bool ok = s.half(a.user, p, v);
    assert(ok);
    (void)ok;
    return v;
  };
  auto begin = [&](uint32_t function) {
    c.call = {};
    c.call.function = function;
    c.pending = true;
  };
  auto request = [&]() {
    if (!s.request(a.user, c.call, c.response))
      return false;
    c.pending = false;
    return true;
  };
  auto done = [&]() {
    c = {};
    return fn == Init ? 2 : 0;
  };
  auto turn = [&]() {
    save(c.actor + 0x144, float(c.response.number));
    save(c.actor + 0x1ac, load(c.actor + 0x144));
  };
  // No callbacks in local arithmetic/store stretches; each external request has
  // its own retained stage and captured input. Actor/work mappings stay stable.
  for (;;)
    switch (c.stage) {
    case 0: {
      uint32_t raw = 0;
      int32_t v = 0;
      if (!a.word(a.user, c.args, raw) || !a.value(a.user, e, sw(raw), v))
        return 0;
      c.name = uint32_t(v);
      c.stage = 1;
      break;
    }
    case 1: {
      const auto actor = a.resolve(a.user, e, c.name, true);
      if (!actor)
        return 0;
      c.actor = *actor;
      c.stage = fn == Init ? 2 : 20;
      break;
    }
    case 2:
      for (; c.argument < 6; ++c.argument) {
        uint32_t raw = 0;
        if (uint64_t(c.args) + 4 * c.argument > UINT32_MAX ||
            !a.word(a.user, c.args + 4 * c.argument, raw))
          return 0;
        if (c.argument < 5) {
          if (!a.real(a.user, e, sw(raw), c.values[c.argument - 1]))
            return 0;
        } else {
          int32_t v = 0;
          if (!a.value(a.user, e, sw(raw), v))
            return 0;
          c.flags = uint32_t(v);
        }
      }
      c.stage = 3;
      break;
    case 3:
      if (!c.pending) {
        uint32_t root = 0;
        if (!a.word(a.user, 0x8041e808, root))
          return 0;
        begin(0x8002ff58);
        c.call.words[0] = root;
        c.call.words[1] = 44;
      }
      if (!request())
        return 0;
      store(c.address + 0xc4, c.response.word);
      c.work = word(c.address + 0xc4);
      c.stage = 4;
      break;
    case 4:
      if (!s.work(a.user, c.work))
        return 0;
      store(c.work, c.flags);
      store(c.work + 4, 0);
      store(c.work + 8, 0);
      store(c.work + 12, 0);
      c.stage = 5;
      break;
    case 5:
    case 6:
    case 7: {
      if (!c.pending) {
        begin(0x800059cc);
        c.call.words[0] = c.stage == 5 ? 6000 : c.stage == 6 ? 9000 : 1500;
      }
      if (!request())
        return 0;
      store(c.work + 0x10 + 4 * (c.stage - 5), c.response.word);
      ++c.stage;
      break;
    }
    case 8:
      save(c.work + 0x1c, c.values[0]);
      save(c.work + 0x20, load(c.actor + 0x90));
      save(c.work + 0x24, c.values[1]);
      save(c.work + 0x28, c.values[2]);
      save(c.actor + 0x1b0, c.values[3]);
      store(c.actor, word(c.actor) | 0x00400000);
      return done();
    case 20:
      if (!c.pending)
        begin(0x8005e0c0);
      if (!request())
        return 0;
      c.mario = c.response.word;
      c.work = word(c.address + 0xc4);
      c.stage = 21;
      break;
    case 21: {
      if (!s.work(a.user, c.work))
        return 0;
      const auto phase = word(c.work + 4);
      c.stage = phase == 0   ? 30
                : phase == 1 ? 40
                : phase == 2 ? 80
                : phase == 3 ? 90
                             : 100;
      break;
    }
    case 30:
    case 80:
    case 91: {
      const uint32_t offset = c.stage == 30   ? 0x10
                              : c.stage == 80 ? 0x14
                                              : 0x18;
      if (!c.pending) {
        begin(Random);
        c.call.words[0] = uint32_t(sw(word(c.work + offset)) / 2) + 1;
      }
      if (!request())
        return 0;
      store(c.work + 8,
            uint32_t(sw(word(c.work + offset)) / 2) + c.response.word);
      if (c.stage == 30) {
        store(c.work + 8, uint32_t((sw(word(c.work + 8)) / 20) * 20));
        store(c.work + 12, 0);
        c.stage = 31;
      } else
        c.stage = c.stage == 80 ? 81 : 92;
      break;
    }
    case 31:
    case 81:
      if (!c.pending) {
        begin(Random);
        c.call.words[0] = 60;
      }
      if (!request())
        return 0;
      c.values[0] = (load(c.actor + 0x144) + float(sw(c.response.word))) - 30.f;
      ++c.stage;
      break;
    case 32:
    case 82:
      if (!c.pending) {
        begin(Revise);
        c.call.floats[0] = c.values[0];
      }
      if (!request())
        return 0;
      turn();
      if (c.stage == 32) {
        store(c.work + 4, 1);
        return done();
      }
      store(c.work + 12, 0);
      c.stage = (word(c.work) & 0x10000) ? 83 : 84;
      break;
    case 83:
      if (!c.pending) {
        begin(Random);
        c.call.words[0] = 1000;
      }
      if (!request())
        return 0;
      store(c.work + 12, uint32_t(sw(c.response.word) % 3) + 2);
      c.stage = 84;
      break;
    case 84:
      store(c.work + 4, 3);
      return done();
    case 90:
      store(c.work + 8, word(c.work + 8) - 1);
      c.stage = word(c.work + 8) == 0 ? 91 : 100;
      break;
    case 92:
      if (!word(c.work + 12)) {
        store(c.work + 4, 0);
        return done();
      }
      store(c.work + 12, word(c.work + 12) - 1);
      c.values[0] = 180.f + load(c.actor + 0x144);
      c.stage = 93;
      break;
    case 93:
      if (!c.pending) {
        begin(Revise);
        c.call.floats[0] = c.values[0];
      }
      if (!request())
        return 0;
      save(c.actor + 0x144, float(c.response.number));
      return done();
    case 40:
      if (!c.pending) {
        begin(0x8000722c);
        c.call.floats = {load(c.actor + 0x8c), load(c.actor + 0x94),
                         load(c.work + 0x1c), load(c.work + 0x24)};
      }
      if (!request())
        return 0;
      c.distance = float(c.response.number);
      c.stage = 41;
      break;
    case 41:
    case 45:
    case 51:
    case 55: {
      if (!c.pending) {
        begin(Ready);
        c.call.words[0] = word(c.actor + 0x104);
      }
      if (!request())
        return 0;
      const bool ready = c.response.word != 0;
      if (c.stage == 41) {
        if (ready) {
          if (c.distance > load(c.work + 0x28)) {
            if (!(word(c.work) & 1))
              store(c.work, word(c.work) | 3);
            if (word(c.work) & 2) {
              c.stage = 42;
              break;
            }
          } else if (word(c.work) & 1)
            store(c.work, word(c.work) & ~3u);
        }
        c.stage = 45;
      } else if (c.stage == 45)
        c.stage = ready ? 46 : 51;
      else if (c.stage == 51)
        c.stage = ready && word(c.actor + 0x2fc) ? 52 : 54;
      else {
        if (!ready)
          return done();
        const float speed = load(c.actor + 0x1b0), dt = load(c.actor + 0x1a4);
        const float product = 20.f * speed, cap = product / 1000.f;
        c.step = speed * dt;
        if (c.step >= cap)
          c.step = cap;
        c.stage = 56;
      }
      break;
    }
    case 42:
    case 52:
      if (!c.pending) {
        begin(Angle);
        c.call.floats = {load(c.actor + 0x8c), load(c.actor + 0x94),
                         load(c.work + 0x1c), load(c.work + 0x24)};
      }
      if (!request())
        return 0;
      c.values[0] = float(c.response.number);
      ++c.stage;
      break;
    case 43:
    case 50:
    case 53:
      if (!c.pending) {
        begin(Revise);
        c.call.floats[0] = c.values[0];
      }
      if (!request())
        return 0;
      turn();
      store(c.work, word(c.work) & (c.stage == 43 ? ~2u : ~3u));
      c.stage = c.stage == 43 ? 45 : c.stage == 50 ? 51 : 54;
      break;
    case 46:
      if (!c.pending)
        begin(0x80096948);
      if (!request())
        return 0;
      c.stage = c.response.word == c.actor ? 47 : 51;
      break;
    case 47:
      if (!c.pending) {
        begin(Compare);
        c.call.floats[0] = load(c.actor + 0x1ac);
        c.call.floats[1] = load(c.mario + 0x1a4);
      }
      if (!request())
        return 0;
      c.stage = std::fabs(float(c.response.number)) < 90.f ? 48 : 51;
      break;
    case 48:
      if (!c.pending) {
        begin(Angle);
        c.call.floats = {load(c.actor + 0x8c), load(c.actor + 0x94),
                         load(c.mario + 0x8c), load(c.mario + 0x94)};
      }
      if (!request())
        return 0;
      c.values[0] = float(c.response.number);
      c.stage = 49;
      break;
    case 49:
      if (!c.pending) {
        begin(Compare);
        c.call.floats[0] = c.values[0];
        c.call.floats[1] = load(c.mario + 0x1a4);
      }
      if (!request())
        return 0;
      if (std::fabs(float(c.response.number)) < 90.f &&
          c.distance < load(c.work + 0x28)) {
        c.values[0] = load(c.actor + 0x1ac) - 180.f;
        c.stage = 50;
      } else
        c.stage = 51;
      break;
    case 54:
      store(c.work + 8, word(c.work + 8) - 1);
      if (!word(c.work + 8)) {
        store(c.work + 4, 2);
        return done();
      }
      c.stage = 55;
      break;
    case 56:
    case 57:
      if (!c.pending) {
        begin(c.stage == 56 ? 0x80272410 : 0x80271ea8);
        const float product = 6.2831854820251465f * load(c.actor + 0x1ac);
        c.call.floats[0] = product / 360.f;
      }
      if (!request())
        return 0;
      save(c.actor + (c.stage == 56 ? 0x1b4 : 0x1bc),
           c.step * (c.stage == 56 ? float(c.response.number)
                                   : -float(c.response.number)));
      ++c.stage;
      break;
    case 58:
      if (!c.pending)
        begin(0x800dc0e8);
      if (!request())
        return 0;
      c.mask = 0;
      c.stage = (c.response.word & 0x100) ? 59 : 60;
      break;
    case 59:
      if (!c.pending) {
        begin(0x8004069c);
        c.call.words[0] = word(c.actor + 0x104);
      }
      if (!request())
        return 0;
      if (!(c.response.word & 0x1800))
        c.mask = 0x10000;
      c.stage = 60;
      break;
    case 60: {
      const float period = float(half(c.actor + 0x1e8)),
                  speed = load(c.actor + 0x1b0),
                  elapsed = load(c.actor + 0x1dc);
      const float threshold = period / speed;
      if (elapsed > threshold) {
        const int16_t toggle = int16_t(half(c.actor + 0x1d8));
        c.flags = word(c.actor + (toggle ? 0x1e0 : 0x1e4));
        c.stage = c.flags ? 61 : 63;
      } else
        c.stage = 64;
      break;
    }
    case 61:
      if (!c.pending) {
        begin(0x800dc650);
        c.call.words[0] = c.flags;
      }
      if (!request())
        return 0;
      c.flags = c.response.word | c.mask;
      c.stage = 62;
      break;
    case 62:
      if (!c.pending) {
        begin(0x800d9808);
        c.call.words = {c.flags, c.actor + 0x8c, 255, 255, 0, 8};
      }
      if (!request())
        return 0;
      c.stage = 63;
      break;
    case 63: {
      save(c.actor + 0x1dc, 0.f);
      const auto toggle = int16_t(half(c.actor + 0x1d8));
      const uint8_t v = uint8_t(1 - int32_t(toggle));
      const int32_t sign = v < 128 ? int32_t(v) : int32_t(v) - 256;
      s.storeHalf(a.user, c.actor + 0x1d8, uint16_t(sign));
      c.stage = 64;
      break;
    }
    case 64:
      if (word(c.actor + 0x1e4) || int16_t(half(c.actor + 0x1d8)) != 0) {
        const float dt = load(c.actor + 0x1a4), elapsed = load(c.actor + 0x1dc);
        save(c.actor + 0x1dc, std::fma(1000.f, dt, elapsed));
      }
      return done();
    case 100:
      return done();
    default:
      return 0;
    }
}
} // namespace ttyd

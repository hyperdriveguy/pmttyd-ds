#include "field_npc_wander_event.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t A = 0x1000, W = 0x2000, M = 0x3000, E = 0x4000,
                   Args = 0x5000, Init = 0x80237878, Main = 0x802372a8;
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
struct Fixture {
  EventEntry event;
  FieldNpcWanderEventCursor cursor;
  std::map<uint32_t, uint32_t> memory;
  std::vector<FieldNpcWanderRequest> calls;
  std::vector<uint32_t> stores;
  uint32_t blocked = 0;
  unsigned allocated = 0, draws = 0, outputs = 0;
  bool mutate = false, ready = true;
  uint32_t touch = 0;
  Fixture() {
    event.id = 4;
    event.arguments = Args;
    for (uint32_t base : {A, W, M, E, Args})
      for (unsigned n = 0; n < 0x340; n += 4)
        memory[base + n] = 0;
    memory[0x8041e808] = 0x6000;
    memory[Args] = 99;
    memory[E + 0xc4] = W;
    memory[A + 0x104] = 7;
    memory[W + 0x28] = bits(70);
    memory[W + 0x10] = 360;
    memory[W + 0x14] = 540;
    memory[W + 0x18] = 90;
  }
  void set(uint32_t p, float f) { memory[p] = bits(f); }
  float get(uint32_t p) { return real(memory[p]); }
  FieldNpcWanderEventServices services() {
    FieldNpcWanderEventServices s;
    auto &a = s.actor;
    a.user = this;
    a.word = [](void *p, uint32_t address, uint32_t &v) {
      auto &m = static_cast<Fixture *>(p)->memory;
      auto i = m.find(address);
      if (i == m.end())
        return false;
      v = i->second;
      return true;
    };
    a.value = [](void *, EventEntry &, int32_t raw, int32_t &v) {
      v = raw;
      return true;
    };
    a.real = [](void *, EventEntry &, int32_t raw, float &v) {
      v = float(raw);
      return true;
    };
    a.resolve = [](void *p, EventEntry &, uint32_t name,
                   bool access) -> std::optional<uint32_t> {
      assert(name == 99 && access);
      auto &f = *static_cast<Fixture *>(p);
      if (f.mutate)
        f.memory[Args + 4] = 12;
      return A;
    };
    a.store = [](void *p, uint32_t address, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.memory[address] = v;
      f.stores.push_back(address);
    };
    s.half = [](void *p, uint32_t address, uint16_t &v) {
      auto &m = static_cast<Fixture *>(p)->memory;
      auto i = m.find(address & ~3u);
      if (i == m.end())
        return false;
      v = uint16_t(i->second >> (address & 2 ? 0 : 16));
      return true;
    };
    s.storeHalf = [](void *p, uint32_t address, uint16_t v) {
      auto &f = *static_cast<Fixture *>(p);
      const auto shift = address & 2 ? 0 : 16;
      auto &w = f.memory[address & ~3u];
      w = (w & ~(0xffffu << shift)) | (uint32_t(v) << shift);
    };
    s.work = [](void *, uint32_t w) { return w == W; };
    s.request = [](void *p, const FieldNpcWanderRequest &q,
                   FieldNpcWanderResponse &out) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.blocked == q.function)
        return false;
      f.calls.push_back(q);
      switch (q.function) {
      case 0x8002ff58:
        assert(q.words[0] == 0x6000 && q.words[1] == 44);
        ++f.allocated;
        out.word = W;
        return true;
      case 0x800059cc:
        out.word = q.words[0] * 60 / 1000;
        if (f.mutate && q.words[0] == 1500)
          f.set(A + 0x90, 23);
        return true;
      case 0x8005e0c0:
        out.word = M;
        return true;
      case 0x80096948:
        out.word = f.touch;
        return true;
      case 0x80005da0:
        ++f.draws;
        out.word = 5;
        if (f.mutate && q.words[0] != 60)
          f.memory[W + 0x10] = 400;
        if (f.mutate && q.words[0] == 60)
          f.set(A + 0x144, 42);
        return true;
      case 0x80007328: {
        float r = float(std::fmod(double(q.floats[0]), 360.));
        if (std::isnan(r))
          r = 0;
        if (r < 0) {
          r += 360;
          if (r >= 360)
            r = 0;
        }
        out.number = r;
        return true;
      }
      case 0x80040750:
        out.word = f.ready;
        return true;
      case 0x8000722c:
        out.number =
            std::hypot(q.floats[2] - q.floats[0], q.floats[3] - q.floats[1]);
        return true;
      case 0x8000703c:
        out.number = 90;
        return true; // Explicit test numeric provider, not source atan
                     // fidelity.
      case 0x800071f0:
        out.number = 0;
        return true;
      case 0x80272410:
        out.number = std::sin(double(q.floats[0]));
        if (f.mutate)
          f.set(A + 0x1ac, 90);
        return true;
      case 0x80271ea8:
        out.number = std::cos(double(q.floats[0]));
        return true;
      case 0x800dc0e8:
        out.word = 0x100;
        return true;
      case 0x8004069c:
        out.word = 0;
        return true;
      case 0x800dc650:
        assert(q.words[0] == 0x9000);
        out.word = 77;
        return true;
      case 0x800d9808:
        assert((q.words == std::array<uint32_t, 6>{77 | 0x10000, A + 0x8c, 255,
                                                   255, 0, 8}));
        ++f.outputs;
        if (f.mutate) {
          f.memory[A + 0x1d8] = 2u << 16;
          f.set(A + 0x1a4, .03f);
        }
        return true;
      default:
        assert(false);
        return false;
      }
    };
    return s;
  }
  int run(uint32_t fn, bool first = false) {
    return fieldNpcWanderEvent(fn, event, first, E, cursor, services());
  }
};
void initialization() {
  Fixture f;
  f.mutate = true;
  f.memory[Args + 4] = 1;
  f.memory[Args + 8] = 2;
  f.memory[Args + 12] = 70;
  f.memory[Args + 16] = 30;
  f.memory[Args + 20] = 0x10000;
  f.blocked = 0x8002ff58;
  assert(f.run(Init, true) == 0 && f.allocated == 0 && f.stores.empty());
  f.blocked = 0x800059cc;
  assert(f.run(Init) == 0 && f.allocated == 1 && f.memory[E + 0xc4] == W);
  assert(f.run(Init) == 0 && f.allocated == 1);
  f.blocked = 0;
  assert(f.run(Init) == 2 && !f.cursor.initialized && f.allocated == 1);
  assert(f.memory[W] == 0x10000 && f.memory[W + 0x10] == 360 &&
         f.memory[W + 0x14] == 540 && f.memory[W + 0x18] == 90);
  assert(f.get(W + 0x1c) == 12 && f.get(W + 0x20) == 23 &&
         f.get(W + 0x24) == 2 && f.get(W + 0x28) == 70 &&
         f.get(A + 0x1b0) == 30 && (f.memory[A] & 0x400000));
}
void phasesAndDrawHolds() {
  Fixture f;
  f.mutate = true;
  f.blocked = 0x80005da0;
  assert(f.run(Main, true) == 0 && f.draws == 0 && f.cursor.pending);
  f.memory[W + 0x10] = 800;
  f.blocked = 0x80007328;
  assert(f.run(Main) == 0 && f.draws == 2);
  assert(f.calls[1].words[0] == 181 && f.memory[W + 8] == 200);
  assert(f.run(Main) == 0 && f.draws == 2);
  f.blocked = 0;
  assert(f.run(Main) == 0 && !f.cursor.initialized && f.memory[W + 4] == 1 &&
         f.get(A + 0x144) == 17 && f.get(A + 0x1ac) == 17);
  f.memory[W + 4] = 2;
  f.memory[W] = 0x10000;
  assert(f.run(Main) == 0 && f.memory[W + 4] == 3 && f.memory[W + 12] == 4);
  f.memory[W + 8] = 1;
  const auto heading = f.memory[A + 0x1ac];
  assert(f.run(Main) == 0 && f.memory[W + 12] == 3 && f.get(A + 0x144) == 197 &&
         f.memory[A + 0x1ac] == heading);
  f.memory[W + 8] = 1;
  f.memory[W + 12] = 0;
  assert(f.run(Main) == 0 && f.memory[W + 4] == 0);
}
void movementAndAudioHolds() {
  Fixture f;
  f.mutate = true;
  f.memory[W + 4] = 1;
  f.memory[W + 8] = 3;
  f.set(A + 0x1b0, 30);
  f.set(A + 0x1a4, .01f);
  f.set(A + 0x1dc, 1);
  f.memory[A + 0x1e4] = 0x9000;
  f.blocked = 0x800d9808;
  assert(f.run(Main, true) == 0 && f.cursor.pending && f.memory[W + 8] == 2 &&
         f.outputs == 0);
  assert(f.get(A + 0x1b4) == 0 && std::fabs(f.get(A + 0x1bc)) < 1e-6f);
  const auto stores = f.stores.size();
  assert(f.run(Main) == 0 && f.stores.size() == stores && f.memory[W + 8] == 2);
  f.blocked = 0;
  assert(f.run(Main) == 0 && !f.cursor.initialized && f.outputs == 1 &&
         f.memory[W + 8] == 2 && f.memory[A + 0x1d8] == 0xffff0000 &&
         std::fabs(f.get(A + 0x1dc) - 30) < 1e-5f);
  f.ready = false;
  assert(f.run(Main) == 0 && f.memory[W + 8] == 1);
  assert(f.run(Main) == 0 && f.memory[W + 4] == 2 && f.memory[W + 8] == 0);
}
void originalTurnGates() {
  Fixture f;
  f.memory[W + 4] = 1;
  f.memory[W + 8] = 8;
  f.set(A + 0x1b0, 30);
  f.set(A + 0x1a4, .01f);
  f.set(A + 0x8c, 100);
  assert(f.run(Main, true) == 0 && f.memory[W] == 1 && f.get(A + 0x144) == 90);
  f.set(A + 0x8c, 0);
  assert(f.run(Main) == 0 && f.memory[W] == 0);
  f.touch = A;
  f.set(A + 0x1ac, 0);
  assert(f.run(Main) == 0 && f.get(A + 0x144) == 180 &&
         f.get(A + 0x1ac) == 180);
  f.touch = 0;
  f.memory[A + 0x2fc] = 1;
  f.memory[W] = 3;
  assert(f.run(Main) == 0 && f.get(A + 0x144) == 90 && f.memory[W] == 0);
  f.memory[W + 4] = 0xffffffff;
  const auto before = f.stores.size();
  assert(f.run(Main) == 0 && !f.cursor.initialized &&
         f.stores.size() == before);
}

} // namespace
int main() {
  originalTurnGates();
  initialization();
  phasesAndDrawHolds();
  movementAndAudioHolds();
}

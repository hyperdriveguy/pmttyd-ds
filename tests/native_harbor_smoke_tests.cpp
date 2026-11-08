#include "event_manager.hpp"
#include "native_harbor_smoke.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
using namespace ttyd;
namespace {
struct EventFixture {
  EventEntry event;
  NativeHarborSmokeCursor cursor;
  NativeHarborSmokeServices services;
  std::map<uint32_t, uint32_t> memory;
  std::vector<uint32_t> trace;
  unsigned calls = 0, spawns = 0;
  int hold = -1, remaining = 0;
  bool alive = true, reenter = false;
  EventFixture() {
    event.flags = 1;
    event.id = 7;
    event.currentCommand = 0x95544ed0;
    event.arguments = 0x95000000;
    event.argumentCount = 14;
    for (unsigned i = 0; i < 15; ++i)
      memory[event.arguments + 4 * i] = i;
    memory[event.arguments] = 0;
    memory[event.arguments + 4] = 0x9540a84c;
    memory[event.arguments + 8] = 4;
    services.user = this;
    services.lifetime = std::make_shared<int>(0);
    services.current = [](void *p) {
      auto &f = *static_cast<EventFixture *>(p);
      if (f.reenter)
        assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor));
      return f.alive;
    };
    services.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<EventFixture *>(p);
      if (!f.enter(a))
        return {};
      auto it = f.memory.find(a);
      return it == f.memory.end() ? std::optional<uint32_t>{} : it->second;
    };
    services.value = [](void *p, EventEntry &,
                        int32_t raw) -> std::optional<int32_t> {
      auto &f = *static_cast<EventFixture *>(p);
      if (!f.enter(0x80031260))
        return {};
      return raw;
    };
    services.floating = [](void *p, EventEntry &,
                           int32_t raw) -> std::optional<float> {
      auto &f = *static_cast<EventFixture *>(p);
      if (!f.enter(0x80030840))
        return {};
      return float(raw);
    };
    services.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto &f = *static_cast<EventFixture *>(p);
      if (!f.enter(a))
        return {};
      constexpr char name[] = "kemuri_test";
      if (a < 0x9540a84c || a >= 0x9540a84c + sizeof(name))
        return {};
      return uint8_t(name[a - 0x9540a84c]);
    };
    services.spawn =
        [](void *p,
           const NativeHarborSmokeRequest &q) -> std::optional<uint64_t> {
      auto &f = *static_cast<EventFixture *>(p);
      if (!f.enter(0x8020de74))
        return {};
      assert((q.position == std::array<float, 3>{3, 4, 5}) && q.scale == 6);
      ++f.spawns;
      return 1;
    };
  }
  bool enter(uint32_t operation) {
    if (int(trace.size()) == hold && remaining) {
      --remaining;
      return false;
    }
    trace.push_back(operation);
    ++calls;
    return true;
  }
};
struct VisualFixture {
  std::shared_ptr<const void> life = std::make_shared<int>(0);
  bool alive = true, reenter = false;
  unsigned randoms = 0, sines = 0, cosines = 0;
  int boundary = -1, remaining = 0;
  unsigned calls = 0;
  NativeHarborSmokeSource source;
  NativeHarborSmokeData data;
  std::unique_ptr<NativeHarborSmokeVisual> visual;
  explicit VisualFixture(const char *path) {
    assert(data.load(path));
    source.lifetime = life;
    source.user = this;
    source.current = [](void *p) {
      return static_cast<VisualFixture *>(p)->alive;
    };
    source.random = [](void *p, uint32_t bound) -> std::optional<uint32_t> {
      auto &f = *static_cast<VisualFixture *>(p);
      assert(bound == 30);
      if (!f.enter())
        return {};
      return f.randoms++ % 30;
    };
    source.sin = [](void *p, float a) -> std::optional<float> {
      auto &f = *static_cast<VisualFixture *>(p);
      if (!f.enter())
        return {};
      ++f.sines;
      return float(std::sin(double(a)));
    };
    source.cos = [](void *p, float a) -> std::optional<float> {
      auto &f = *static_cast<VisualFixture *>(p);
      if (!f.enter())
        return {};
      ++f.cosines;
      return float(std::cos(double(a)));
    };
    visual = std::make_unique<NativeHarborSmokeVisual>(data, life, [this] {
      if (reenter) {
        assert(!visual->current());
        assert(!visual->advance(1));
        assert(!visual->spawn({}, source));
      }
      return alive;
    });
  }
  bool enter() {
    if (int(calls) == boundary && remaining) {
      --remaining;
      return false;
    }
    ++calls;
    return true;
  }
};
} // namespace
int main(int argc, char **argv) {
  assert(argc == 2);
  EventFixture baseline;
  assert(resumeNativeHarborSmoke(baseline.event, baseline.services,
                                 baseline.cursor));
  assert(baseline.spawns == 1);
  // Exact getter-call count: nineteen integer calls and thirteen float calls.
  unsigned values = 0, floats = 0;
  for (auto q : baseline.trace) {
    values += q == 0x80031260;
    floats += q == 0x80030840;
  }
  assert(values == 19 && floats == 13);
  assert(resumeNativeHarborSmoke(baseline.event, baseline.services,
                                 baseline.cursor) &&
         baseline.spawns == 1);
  for (unsigned at = 0; at < baseline.trace.size(); ++at) {
    EventFixture f;
    f.hold = int(at);
    f.remaining = 2;
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor));
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor));
    assert(resumeNativeHarborSmoke(f.event, f.services, f.cursor));
    assert(f.trace == baseline.trace && f.spawns == 1);
  }
  {
    EventFixture f;
    f.reenter = true;
    assert(resumeNativeHarborSmoke(f.event, f.services, f.cursor));
  }
  {
    EventFixture f;
    f.hold = 10;
    f.remaining = 1;
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor));
    auto changed = f.services;
    changed.user = nullptr;
    assert(!resumeNativeHarborSmoke(f.event, changed, f.cursor));
    f.alive = false;
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor));
    f.alive = true;
    assert(resumeNativeHarborSmoke(f.event, f.services, f.cursor));
  }
  {
    EventFixture f;
    f.memory[f.event.arguments] = 1;
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor) &&
           !f.spawns);
  }
  {
    EventFixture f;
    f.memory[f.event.arguments + 8] = 5;
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor) &&
           !f.spawns);
  }
  // Mutation of an already consumed argument does not replace the captured
  // source first value; later repeated getter calls still observe fresh loads.
  {
    EventFixture f;
    f.hold = 2;
    f.remaining = 1;
    assert(!resumeNativeHarborSmoke(f.event, f.services, f.cursor));
    f.memory[f.event.arguments] = 123;
    assert(resumeNativeHarborSmoke(f.event, f.services, f.cursor) &&
           f.spawns == 1);
  }
  NativeHarborSmokeRequest request;
  request.position = {40, 50, 0};
  request.scale = 1;
  for (int boundary = -1; boundary < 30; ++boundary) {
    VisualFixture f(argv[1]);
    if (boundary >= 0) {
      f.boundary = boundary;
      f.remaining = 2;
      assert(!f.visual->spawn(request, f.source));
      assert(!f.visual->spawn(request, f.source));
    }
    const auto id = f.visual->spawn(request, f.source);
    assert(id && f.randoms == 10 && f.sines == 10 && f.cosines == 10);
    assert(f.visual->advance(100));
    const auto initial = f.visual->instances();
    assert(f.visual->advance(100));
    assert(!f.visual->advance(102));
    for (uint64_t tick = 101; tick <= 164; ++tick)
      assert(f.visual->advance(tick));
    for (const auto &i : f.visual->instances())
      assert(!i.id);
    assert(f.randoms == 10);
    (void)initial;
  }
  {
    VisualFixture f(argv[1]);
    assert(f.visual->spawn(request, f.source));
    assert(f.visual->advance(1));
    auto expected = f.visual->instances()[0].particles;
    // Independent source subtype4 reference: delay branch precedes every
    // frame/position update, source binary32 stores round each operation.
    auto rounded = [](float value) {
      volatile float stored = value;
      return stored;
    };
    for (uint64_t tick = 2; tick <= 42; ++tick) {
      for (auto &p : expected) {
        if (p.delay) {
          --p.delay;
          continue;
        }
        p.frame = uint8_t(int(rounded(rounded(16.f * float(p.age)) / 32.f)));
        p.nextFrame = uint8_t(std::min(
            15, int(rounded(rounded(16.f * float(p.age + 1)) / 32.f))));
        ++p.age;
        p.x = rounded(p.x + p.vx);
        p.y = rounded(p.y + p.vy);
        p.vx = rounded(p.vx * f.data.decay);
        p.vy = rounded(p.vy * f.data.decay);
      }
      assert(f.visual->advance(tick));
      const auto &actual = f.visual->instances()[0];
      if (tick < 42) {
        assert(actual.id);
        for (unsigned n = 0; n < 10; ++n) {
          const auto &a = actual.particles[n];
          const auto &e = expected[n];
          assert(a.age == e.age && a.delay == e.delay && a.frame == e.frame &&
                 a.nextFrame == e.nextFrame && a.x == e.x && a.y == e.y &&
                 a.vx == e.vx && a.vy == e.vy);
        }
      } else
        assert(!actual.id);
    }
  }
  {
    VisualFixture f(argv[1]);
    f.reenter = true;
    assert(f.visual->spawn(request, f.source));
    f.visual->retire();
    assert(!f.visual->current() && !f.visual->spawn(request, f.source));
  }
  {
    VisualFixture f(argv[1]);
    f.boundary = 2;
    f.remaining = 1;
    assert(!f.visual->spawn(request, f.source));
    auto wrong = f.source;
    wrong.lifetime = std::make_shared<int>(0);
    assert(!f.visual->spawn(request, wrong));
    wrong = f.source;
    wrong.user = nullptr;
    assert(!f.visual->spawn(request, wrong));
    assert(f.visual->spawn(request, f.source));
  }
  {
    VisualFixture f(argv[1]);
    for (unsigned n = 0; n < 4; ++n)
      assert(f.visual->spawn(request, f.source));
    assert(!f.visual->spawn(request, f.source) && f.randoms == 40);
  }
  std::cout << "Smoke finite source prefix/every hold, real data,10particle "
               "simulation lifetime and ownership pass\n";
}

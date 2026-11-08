#include "event_manager.hpp"
#include "native_harbor_star_point.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
using namespace ttyd;
namespace {
struct EventFixture {
  EventEntry event;
  NativeHarborStarPointCursor cursor;
  NativeHarborStarPointServices services;
  std::map<uint32_t, uint32_t> memory;
  std::vector<uint32_t> trace;
  unsigned calls = 0, spawns = 0;
  int hold = -1, remaining = 0;
  bool alive = true, reenter = false;
  EventFixture() {
    event.flags = 1;
    event.id = 7;
    event.currentCommand = 0x95544f90;
    event.arguments = 0x95000000;
    event.argumentCount = 14;
    for (unsigned i = 0; i < 15; ++i)
      memory[event.arguments + 4 * i] = i;
    memory[event.arguments] = 0x9540a858;
    memory[event.arguments + 4] = 0x9540a85c;
    memory[event.arguments + 8] = 0;
    services.user = this;
    services.lifetime = std::make_shared<int>(0);
    services.current = [](void *p) {
      auto &f = *static_cast<EventFixture *>(p);
      if (f.reenter)
        assert(!resumeNativeHarborStarPoint(f.event, f.services, f.cursor));
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
      if (a == 0x9540a858 || a == 0x8042033c)
        return 0;
      constexpr char name[] = "star_point";
      if (a < 0x9540a85c || a >= 0x9540a85c + sizeof(name))
        return {};
      return uint8_t(name[a - 0x9540a85c]);
    };
    services.spawn =
        [](void *p,
           const NativeHarborStarPointRequest &q) -> std::optional<uint64_t> {
      auto &f = *static_cast<EventFixture *>(p);
      if (!f.enter(0x8020b0f4))
        return {};
      assert((q.position == std::array<float, 3>{3, 4, 5}) && q.amount == 6 &&
             q.subtype == 0);
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
struct SourceFixture {
  NativeHarborStarPointSource services;
  std::vector<std::array<uint32_t, 5>> trace;
  unsigned calls = 0, randoms = 0;
  int hold = -1, remaining = 0;
  int32_t raw = 0;
  bool alive = true, reenter = false;
  NativeHarborStarPointVisual *visual = nullptr;
  SourceFixture() {
    services.user = this;
    services.lifetime = std::make_shared<int>(0);
    services.current = [](void *p) {
      auto &f = *static_cast<SourceFixture *>(p);
      if (f.reenter && f.visual)
        assert(!f.visual->advance(1));
      return f.alive;
    };
    services.randomRaw = [](void *p) -> std::optional<int32_t> {
      auto &f = *static_cast<SourceFixture *>(p);
      if (!f.enter({0, 0, 0, 0, 0}))
        return {};
      ++f.randoms;
      return f.raw;
    };
    services.math = [](void *p, NativeHarborStarMath op, float a, float b,
                       float c) -> std::optional<float> {
      auto &f = *static_cast<SourceFixture *>(p);
      std::array<uint32_t, 5> q{uint32_t(op) + 1, 0, 0, 0, 0};
      std::memcpy(&q[1], &a, 4);
      std::memcpy(&q[2], &b, 4);
      std::memcpy(&q[3], &c, 4);
      if (!f.enter(q))
        return {};
      volatile float r = 0;
      switch (op) {
      case NativeHarborStarMath::Add:
        r = a + b;
        break;
      case NativeHarborStarMath::Subtract:
        r = a - b;
        break;
      case NativeHarborStarMath::Multiply:
        r = a * b;
        break;
      case NativeHarborStarMath::Divide:
        r = a / b;
        break;
      case NativeHarborStarMath::MultiplyAdd:
        r = std::fma(a, b, c);
        break;
      case NativeHarborStarMath::MultiplySubtract:
        r = std::fma(a, b, -c);
        break;
      }
      return r;
    };
  }
  bool enter(std::array<uint32_t, 5> q) {
    if (int(calls) == hold && remaining) {
      --remaining;
      return false;
    }
    ++calls;
    trace.push_back(q);
    return true;
  }
};
uint32_t bits(float x) {
  uint32_t w;
  std::memcpy(&w, &x, 4);
  return w;
}
float mul(float a, float b) {
  volatile float v = a * b;
  return v;
}
float sub(float a, float b) {
  volatile float v = a - b;
  return v;
}
// Independent scalar reference of the pinned original four angular sectors.
std::array<float, 2> reference(const NativeHarborStarPointData &d,
                               int32_t raw) {
  const auto &c = d.scalars;
  volatile float angle = mul(c[2], float(raw % 360)) / c[3];
  float u = 0;
  bool negSin = false, negCos = false;
  if (angle <= c[7]) {
    if (angle <= c[8])
      u = angle;
    else {
      u = sub(c[8], sub(angle, c[8]));
      negCos = true;
    }
  } else if (angle < c[9]) {
    u = sub(angle, c[7]);
    negSin = true;
    negCos = true;
  } else {
    u = sub(c[8], sub(angle, c[9]));
    negSin = true;
  }
  const float square = mul(u, u);
  float sn = mul(std::fma(std::fma(c[4], square, -c[5]), square, c[6]), u);
  float cs = std::fma(std::fma(c[11], square, -c[12]), square, c[6]);
  if (negSin)
    sn = -sn;
  if (negCos)
    cs = -cs;
  return {mul(c[10], sn), mul(c[10], cs)};
}
} // namespace
int main(int argc, char **argv) {
  assert(argc == 2);
  NativeHarborStarPointData data;
  assert(data.load(argv[1]) && data.valid());
  auto resource = std::make_shared<int>(0);
  bool live = true;
  for (int angle = 0; angle < 360; ++angle) {
    NativeHarborStarPointVisual v(data, resource, [&] { return live; });
    SourceFixture f;
    f.raw = angle;
    NativeHarborStarPointRequest q;
    q.amount = 1;
    auto id = v.spawn(q, f.services);
    assert(id && f.randoms == 1);
    auto expected = reference(data, angle);
    const auto &p = v.instances()[0].particles[0];
    assert(bits(p.velocity[0]) == bits(expected[0]) &&
           bits(p.velocity[2]) == bits(expected[1]));
  }
  NativeHarborStarPointRequest request;
  request.amount = 9;
  request.position = {12, 25, -4};
  SourceFixture baseline;
  baseline.raw = 219;
  NativeHarborStarPointVisual full(data, resource, [&] { return live; });
  assert(full.spawn(request, baseline.services));
  const auto expectedTrace = baseline.trace;
  assert(baseline.randoms == 9);
  for (unsigned boundary = 0; boundary < expectedTrace.size(); ++boundary) {
    SourceFixture f;
    f.raw = 219;
    f.hold = int(boundary);
    f.remaining = 2;
    NativeHarborStarPointVisual v(data, resource, [&] { return live; });
    assert(!v.spawn(request, f.services));
    assert(!v.spawn(request, f.services));
    auto replaced = f.services;
    replaced.user = nullptr;
    assert(!v.spawn(request, replaced));
    auto wrong = request;
    ++wrong.amount;
    assert(!v.spawn(wrong, f.services));
    assert(v.spawn(request, f.services));
    assert(f.trace == expectedTrace && f.randoms == 9);
    assert(v.instances()[0].particles.size() == 9);
  }
  // The actual Harbor scheduler services existing effect visits BEFORE
  // retrying a held constructor. Unpublished candidates must not deadlock it.
  {
    SourceFixture held;
    held.hold = 1;
    held.remaining = 2;
    NativeHarborStarPointVisual pending(data, resource, [] { return true; });
    assert(!pending.spawn(request, held.services));
    assert(pending.advance(1));
    assert(!pending.spawn(request, held.services));
    assert(pending.advance(2));
    assert(pending.spawn(request, held.services));
    assert(held.randoms == 9);
  }
  SourceFixture f;
  f.reenter = true;
  NativeHarborStarPointVisual v(data, resource, [&] { return live; });
  f.visual = &v;
  assert(v.spawn(request, f.services));
  assert(v.advance(1) && v.advance(1));
  assert(v.instances()[0].particles[0].delay == 3 && v.instances()[0].age == 1);
  for (uint64_t visit = 2; visit <= 4; ++visit)
    assert(v.advance(visit));
  assert(!v.instances()[0].particles[0].alpha);
  assert(v.advance(5) && v.instances()[0].particles[0].alpha == 255 &&
         v.instances()[0].particles[0].position[1] == 12);
  const auto callsBefore = f.calls;
  assert(v.advance(5) && f.calls == callsBefore);
  f.hold = int(f.calls);
  f.remaining = 2;
  assert(!v.advance(6) && !v.advance(6));
  assert(v.instances()[0].age == 5);
  assert(!v.advance(7));
  assert(v.advance(6) && v.instances()[0].age == 6);
  for (uint64_t visit = 7; visit <= 61; ++visit)
    assert(v.advance(visit));
  assert(!v.instances()[0].id);
  live = false;
  auto before = f.calls;
  assert(!v.spawn(request, f.services) && !v.advance(62) && f.calls == before);
  assert(v.retire());
  // Every actual Main arithmetic operation held twice after all nine delays
  // expire. Staged native geometry is published only when the visit completes.
  SourceFixture updateReference;
  updateReference.raw = 219;
  NativeHarborStarPointVisual updateFull(data, resource, [] { return true; });
  assert(updateFull.spawn(request, updateReference.services));
  for (uint64_t tick = 1; tick <= 32; ++tick)
    assert(updateFull.advance(tick));
  const auto updateStart = updateReference.calls;
  assert(updateFull.advance(33));
  const auto updateEnd = updateReference.calls;
  const auto referenceParticles = updateFull.instances()[0].particles;
  for (unsigned boundary = updateStart; boundary < updateEnd; ++boundary) {
    SourceFixture held;
    held.raw = 219;
    NativeHarborStarPointVisual attempt(data, resource, [] { return true; });
    assert(attempt.spawn(request, held.services));
    for (uint64_t tick = 1; tick <= 32; ++tick)
      assert(attempt.advance(tick));
    held.hold = int(boundary);
    held.remaining = 2;
    assert(!attempt.advance(33) && !attempt.advance(33));
    assert(attempt.instances()[0].age == 32);
    assert(attempt.advance(33));
    assert(held.trace == updateReference.trace);
    const auto &actual = attempt.instances()[0].particles;
    for (size_t n = 0; n < actual.size(); ++n) {
      assert(actual[n].alpha == referenceParticles[n].alpha);
      for (unsigned axis = 0; axis < 3; ++axis)
        assert(bits(actual[n].position[axis]) ==
               bits(referenceParticles[n].position[axis]));
    }
  }
  for (uint32_t raw = 0; raw <= 32767; ++raw) {
    // Original signed mulhw/div360 lowering in8020b210..23c.
    const int64_t product = int64_t(int32_t(0xb60b60b7u)) * int32_t(raw);
    const int32_t high = int32_t(uint64_t(product) >> 32);
    const int32_t sum = int32_t(uint32_t(high) + raw);
    const int32_t shift =
        sum >= 0 ? sum / 256 : -int32_t((uint32_t(-int64_t(sum)) + 255) / 256);
    const int32_t quotient = int32_t(uint32_t(shift) + (uint32_t(shift) >> 31));
    assert(int32_t(raw) - 360 * quotient == int32_t(raw % 360));
  }
  EventFixture direct;
  assert(resumeNativeHarborStarPoint(direct.event, direct.services,
                                     direct.cursor));
  auto trace = direct.trace;
  assert(direct.spawns == 1);
  for (unsigned i = 0; i < trace.size(); ++i) {
    EventFixture held;
    held.hold = int(i);
    held.remaining = 2;
    assert(
        !resumeNativeHarborStarPoint(held.event, held.services, held.cursor));
    assert(
        !resumeNativeHarborStarPoint(held.event, held.services, held.cursor));
    assert(resumeNativeHarborStarPoint(held.event, held.services, held.cursor));
    assert(held.trace == trace && held.spawns == 1);
  }
  std::cout << "Original StarPoint angular sectors, every constructor/event "
               "boundary and simulation lifetime pass\n";
}

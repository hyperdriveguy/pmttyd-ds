#include "native_harbor_smoke.hpp"
#include "event_manager.hpp"
#include "event_script_assets.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
struct Guard {
  bool &v;
  explicit Guard(bool &b) : v(b) { v = true; }
  ~Guard() { v = false; }
};
float real(uint32_t w) {
  float f;
  std::memcpy(&f, &w, 4);
  return f;
}
uint32_t bits(float f) {
  uint32_t w;
  std::memcpy(&w, &f, 4);
  return w;
}
float add(float a, float b) {
  volatile float f = a + b;
  return f;
}
float mul(float a, float b) {
  volatile float f = a * b;
  return f;
}
float div(float a, float b) {
  volatile float f = a / b;
  return f;
}
bool same(const NativeHarborSmokeServices &a,
          const NativeHarborSmokeServices &b) {
  return a.user == b.user && a.lifetime == b.lifetime &&
         !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) && a.current == b.current &&
         a.word == b.word && a.value == b.value && a.floating == b.floating &&
         a.byte == b.byte && a.spawn == b.spawn;
}
bool same(const NativeHarborSmokeRequest &a,
          const NativeHarborSmokeRequest &b) {
  for (unsigned i = 0; i < 3; ++i)
    if (bits(a.position[i]) != bits(b.position[i]))
      return false;
  return bits(a.scale) == bits(b.scale);
}
} // namespace
bool resumeNativeHarborSmoke(EventEntry &e, const NativeHarborSmokeServices &s,
                             NativeHarborSmokeCursor &c) {
  if (c.invoking_ || !s.lifetime || !s.current || !(e.flags & 1) ||
      e.argumentCount < 14)
    return false;
  Guard guard(c.invoking_);
  if (c.event_ &&
      (c.event_ != &e || c.id_ != e.id || c.command_ != e.currentCommand ||
       c.args_ != e.arguments || !same(s, c.services_)))
    return false;
  if (!s.current(s.user))
    return false;
  if (!c.event_) {
    c.event_ = &e;
    c.id_ = e.id;
    c.command_ = e.currentCommand;
    c.args_ = e.arguments;
    c.services_ = s;
  }
  if (c.event_ != &e || c.id_ != e.id || c.command_ != e.currentCommand ||
      c.args_ != e.arguments || !same(s, c.services_))
    return false;
  if (c.complete_)
    return true;
  if (!c.prepared_) {
    size_t read = 0;
    auto load = [&](uint32_t kind, uint32_t address, uint32_t &out) {
      if (read < c.reads_.size()) {
        const auto &r = c.reads_[read++];
        if (r.kind != kind || r.address != address)
          return false;
        out = r.value;
        return true;
      }
      c.pending_ = kind == 0   ? address
                   : kind == 1 ? 0x80031260
                   : kind == 2 ? 0x80030840
                               : address;
      if (!s.current(s.user))
        return false;
      std::optional<uint32_t> v;
      if (kind == 0 && s.word)
        v = s.word(s.user, address);
      else if (kind == 1 && s.value) {
        auto q = s.value(s.user, e, int32_t(address));
        if (q)
          v = uint32_t(*q);
      } else if (kind == 2 && s.floating) {
        auto q = s.floating(s.user, e, int32_t(address));
        if (q)
          v = bits(*q);
      } else if (kind == 3 && s.byte) {
        auto q = s.byte(s.user, address);
        if (q)
          v = *q;
      }
      if (!v || !s.current(s.user))
        return false;
      c.reads_.push_back({kind, address, *v});
      ++read;
      out = *v;
      return true;
    };
    auto argument = [&](uint32_t index, uint32_t kind, uint32_t &out) {
      uint32_t raw;
      return load(0, c.args_ + 4 * index, raw) && load(kind, raw, out);
    };
    std::array<uint32_t, 15> integers{}, floating{};
    // Original getter calls are repeated, not deduplicated. Index14 reads the
    // next mapped script word even when this command declared14 operands.
    for (unsigned n = 0; n < 2; ++n)
      for (unsigned k = 0; k < 3; ++k) {
        uint32_t captured;
        if (!argument(n, 1, captured))
          return false;
        if (!k)
          integers[n] = captured;
      }
    for (unsigned n = 2; n < 15; ++n)
      if (!argument(n, 1, integers[n]))
        return false;
    for (unsigned n = 2; n < 15; ++n)
      if (!argument(n, 2, floating[n]))
        return false;
    c.pending_ = 0x8005e8a0;
    constexpr char SupportedName[] = "kemuri_test";
    for (unsigned n = 0; n < sizeof(SupportedName); ++n) {
      uint32_t ch;
      if (!load(3, integers[1] + n, ch) || ch != uint8_t(SupportedName[n]))
        return false;
    }
    // Untyped naming writes require an actual guest entry, never native IDs.
    if (integers[0] || integers[2] != 4)
      return false;
    for (unsigned n = 0; n < 3; ++n)
      c.request_.position[n] = real(floating[n + 3]);
    c.request_.scale = real(floating[6]);
    c.prepared_ = true;
  }
  c.pending_ = 0x8020de74;
  if (!s.current(s.user) || !s.spawn)
    return false;
  auto result = s.spawn(s.user, c.request_);
  if (!result || !*result)
    return false;
  c.complete_ = true;
  c.pending_ = 0;
  return true;
}
bool NativeHarborSmokeData::load(const char *path) {
  EventScriptAssets source;
  if (!path || !source.load(path))
    return false;
  auto word = [&](uint32_t at) -> std::optional<uint32_t> {
    uint32_t value;
    if (!source.readWord(at, value))
      return {};
    return value;
  };
  NativeHarborSmokeData next;
  for (unsigned i = 0; i < 10; ++i) {
    auto a = word(0x8041a090 + 4 * i), r = word(0x8041a0b8 + 4 * i);
    if (!a || !r)
      return false;
    next.angles[i] = real(*a);
    next.radii[i] = real(*r);
  }
  struct Scalar {
    uint32_t address;
    float *value;
  };
  for (const auto &p : std::array<Scalar, 5>{{{0x8042701c, &next.quarterTurn},
                                              {0x80427018, &next.tau},
                                              {0x80427024, &next.fullTurn},
                                              {0x8042700c, &next.radiusScale},
                                              {0x80427028, &next.decay}}}) {
    auto w = word(p.address);
    if (!w)
      return false;
    *p.value = real(*w);
  }
  if (!next.valid())
    return false;
  *this = next;
  return true;
}
bool NativeHarborSmokeData::valid() const {
  for (float a : angles)
    if (!std::isfinite(a))
      return false;
  for (float r : radii)
    if (!std::isfinite(r))
      return false;
  return quarterTurn == 90 && tau > 6.28f && tau < 6.29f && fullTurn == 360 &&
         radiusScale == 2 && decay > .89f && decay < .91f;
}
NativeHarborSmokeVisual::NativeHarborSmokeVisual(
    NativeHarborSmokeData d, std::shared_ptr<const void> life,
    std::function<bool()> current)
    : data_(d), gpuLifetime_(std::move(life)), gpuCurrent_(std::move(current)) {
}
bool NativeHarborSmokeVisual::current() const {
  if (checking_)
    return false;
  checking_ = true;
  struct Check {
    bool &v;
    ~Check() { v = false; }
  } check{checking_};
  return !retired_ && data_.valid() && gpuLifetime_ && gpuCurrent_ &&
         gpuCurrent_();
}
std::optional<uint64_t>
NativeHarborSmokeVisual::spawn(const NativeHarborSmokeRequest &q,
                               const NativeHarborSmokeSource &s) {
  if (invoking_)
    return {};
  Guard guard(invoking_);
  if (candidateSlot_ != Capacity &&
      (!same(candidate_.request, q) || services_.user != s.user ||
       services_.lifetime != s.lifetime ||
       services_.lifetime.owner_before(s.lifetime) ||
       s.lifetime.owner_before(services_.lifetime) ||
       services_.current != s.current || services_.random != s.random ||
       services_.sin != s.sin || services_.cos != s.cos))
    return {};
  if (!current() || !s.lifetime || !s.current || !s.current(s.user) ||
      !s.random || !s.sin || !s.cos || !next_ || next_ == UINT64_MAX ||
      !std::isfinite(q.scale) || q.scale <= 0)
    return {};
  for (float x : q.position)
    if (!std::isfinite(x))
      return {};
  if (candidateSlot_ == Capacity) {
    size_t i = 0;
    while (i < Capacity && instances_[i].id)
      ++i;
    if (i == Capacity)
      return {};
    candidateSlot_ = i;
    candidate_ = {};
    candidate_.request = q;
    particle_ = 0;
    stage_ = 0;
    services_ = s;
  }
  if (!same(candidate_.request, q) || services_.user != s.user ||
      services_.lifetime != s.lifetime ||
      services_.lifetime.owner_before(s.lifetime) ||
      s.lifetime.owner_before(services_.lifetime) ||
      services_.current != s.current || services_.random != s.random ||
      services_.sin != s.sin || services_.cos != s.cos)
    return {};
  while (particle_ < Particles) {
    auto &p = candidate_.particles[particle_];
    if (stage_ == 0) {
      angle_ =
          div(mul(data_.tau, add(data_.quarterTurn, data_.angles[particle_])),
              data_.fullTurn);
      radius_ = mul(data_.radiusScale, data_.radii[particle_]);
      stage_ = 1;
    }
    if (stage_ == 1) {
      if (!current() || !services_.current(services_.user))
        return {};
      auto v = services_.sin(services_.user, angle_);
      if (!v || !std::isfinite(*v))
        return {};
      p.vx = mul(radius_, *v);
      stage_ = 2;
    }
    if (stage_ == 2) {
      if (!current() || !services_.current(services_.user))
        return {};
      auto v = services_.cos(services_.user, angle_);
      if (!v || !std::isfinite(*v))
        return {};
      p.vy = mul(-radius_, *v);
      stage_ = 3;
    }
    if (stage_ == 3) {
      if (!current() || !services_.current(services_.user))
        return {};
      auto v = services_.random(services_.user, DelayBound);
      if (!v || *v >= DelayBound)
        return {};
      p.delay = *v;
      stage_ = 0;
      ++particle_;
    }
  }
  if (!current() || !services_.current(services_.user))
    return {};
  candidate_.id = next_++;
  const auto id = candidate_.id;
  instances_[candidateSlot_] = candidate_;
  candidateSlot_ = Capacity;
  services_ = {};
  return id;
}
bool NativeHarborSmokeVisual::advance(uint64_t visit) {
  if (invoking_)
    return false;
  Guard guard(invoking_);
  if (!current() || (visited_ && visit < last_))
    return false;
  if (visited_ && visit == last_)
    return true;
  // Exactly one native simulation visit, irrespective of rendering/import pace.
  // First binding establishes the current clock and never fast-forwards old
  // RNG.
  if (!visited_) {
    visited_ = true;
    last_ = visit;
    return true;
  }
  if (visit != last_ + 1)
    return false;
  for (auto &i : instances_)
    if (i.id) {
      unsigned expired = 0;
      for (auto &p : i.particles) {
        if (p.delay) {
          --p.delay;
          continue;
        }
        p.frame = uint8_t((AnimationFrames * p.age) / LifetimeTicks);
        p.nextFrame = uint8_t(std::min<uint32_t>(
            AnimationFrames - 1,
            (AnimationFrames * (p.age + 1)) / LifetimeTicks));
        ++p.age;
        p.x = add(p.x, p.vx);
        p.y = add(p.y, p.vy);
        p.vx = mul(p.vx, data_.decay);
        p.vy = mul(p.vy, data_.decay);
        expired += p.age >= LifetimeTicks;
      }
      if (expired == Particles)
        i = {};
    }
  last_ = visit;
  return true;
}
void NativeHarborSmokeVisual::retire() {
  if (invoking_)
    return;
  retired_ = true;
  instances_ = {};
  candidate_ = {};
  candidateSlot_ = Capacity;
  services_ = {};
}
} // namespace ttyd

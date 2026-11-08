#include "native_harbor_star_point.hpp"
#include "event_script_assets.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
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
struct Guard {
  bool &v;
  explicit Guard(bool &b) : v(b) { v = true; }
  ~Guard() { v = false; }
};
bool sameOwner(const std::shared_ptr<const void> &a,
               const std::shared_ptr<const void> &b) {
  return a == b && !a.owner_before(b) && !b.owner_before(a);
}
bool same(const NativeHarborStarPointSource &a,
          const NativeHarborStarPointSource &b) {
  return a.user == b.user && sameOwner(a.lifetime, b.lifetime) &&
         a.current == b.current && a.randomRaw == b.randomRaw &&
         a.math == b.math;
}
bool same(const NativeHarborStarPointRequest &a,
          const NativeHarborStarPointRequest &b) {
  return a.subtype == b.subtype && a.amount == b.amount &&
         bits(a.position[0]) == bits(b.position[0]) &&
         bits(a.position[1]) == bits(b.position[1]) &&
         bits(a.position[2]) == bits(b.position[2]);
}
constexpr size_t Zero = 1, Tau = 2, FullTurn = 3, SinFirst = 4, SinSecond = 5,
                 One = 6, Pi = 7, HalfPi = 8, ThreeHalfPi = 9, Speed = 10,
                 CosFirst = 11, CosSecond = 12, InitialUp = 13;
struct Operation {
  uint32_t kind;
  std::array<uint32_t, 3> arguments;
  uint32_t result;
};
struct Tape {
  std::vector<Operation> operations;
  size_t index = 0;
  void replay() { index = 0; }
  std::optional<uint32_t> call(const NativeHarborStarPointSource &s,
                               uint32_t kind,
                               const std::array<uint32_t, 3> &args) {
    if (index < operations.size()) {
      const auto &q = operations[index++];
      if (q.kind != kind || q.arguments != args)
        return {};
      return q.result;
    }
    if (!s.lifetime || !s.current || !s.current(s.user))
      return {};
    std::optional<uint32_t> value;
    if (!kind && s.randomRaw) {
      auto r = s.randomRaw(s.user);
      if (r)
        value = uint32_t(*r);
    } else if (kind && s.math) {
      auto r = s.math(s.user, NativeHarborStarMath(kind - 1), real(args[0]),
                      real(args[1]), real(args[2]));
      if (r)
        value = bits(*r);
    }
    if (!value)
      return {};
    operations.push_back({kind, args, *value});
    ++index;
    return value;
  }
  std::optional<float> math(const NativeHarborStarPointSource &s,
                            NativeHarborStarMath op, float a, float b,
                            float c = 0) {
    auto r = call(s, uint32_t(op) + 1, {bits(a), bits(b), bits(c)});
    if (!r)
      return {};
    return real(*r);
  }
};
} // namespace
bool NativeHarborStarPointData::load(const char *path) {
  EventScriptAssets image;
  if (!path || !image.load(path))
    return false;
  NativeHarborStarPointData next;
  for (size_t i = 0; i < next.scalars.size(); ++i) {
    uint32_t v;
    if (!image.readWord(0x80426f08 + 4 * uint32_t(i), v))
      return false;
    next.scalars[i] = real(v);
  }
  uint32_t high, low;
  if (!image.readWord(0x802fe540, high) || !image.readWord(0x802fe544, low))
    return false;
  next.signedBias = (uint64_t(high) << 32) | low;
  if (!next.valid())
    return false;
  *this = next;
  return true;
}
bool NativeHarborStarPointData::valid() const {
  for (float f : scalars)
    if (!std::isfinite(f))
      return false;
  return signedBias == 0x4330000080000000ull && scalars[Zero] == 0 &&
         scalars[One] == 1 && scalars[FullTurn] == 360 && scalars[Speed] == 2 &&
         scalars[InitialUp] == 14 && scalars[Tau] > 6.28f &&
         scalars[Tau] < 6.29f;
}
struct NativeHarborStarPointVisual::State {
  NativeHarborStarPointData data;
  std::shared_ptr<const void> resource;
  std::function<bool()> resourceCurrent;
  std::array<Instance, Capacity> live;
  std::array<NativeHarborStarPointSource, Capacity> sources;
  bool invoking = false, retired = false;
  mutable bool checking = false;
  uint64_t next = 1, lastVisit = 0, pendingVisit = 0;
  size_t candidateSlot = Capacity;
  Instance candidate;
  NativeHarborStarPointSource candidateSource;
  Tape constructor, update;
};
NativeHarborStarPointVisual::NativeHarborStarPointVisual(
    NativeHarborStarPointData d, std::shared_ptr<const void> r,
    std::function<bool()> c)
    : state_(new State) {
  state_->data = d;
  state_->resource = std::move(r);
  state_->resourceCurrent = std::move(c);
}
NativeHarborStarPointVisual::~NativeHarborStarPointVisual() = default;
bool NativeHarborStarPointVisual::current() const {
  auto &s = *state_;
  if (s.checking)
    return false;
  Guard g(s.checking);
  return !s.retired && s.data.valid() && s.resource && s.resourceCurrent &&
         s.resourceCurrent();
}
std::optional<uint64_t>
NativeHarborStarPointVisual::spawn(const NativeHarborStarPointRequest &q,
                                   const NativeHarborStarPointSource &source) {
  auto &s = *state_;
  if (s.invoking)
    return {};
  Guard guard(s.invoking);
  if (s.candidateSlot != Capacity &&
      (!same(q, s.candidate.request) || !same(source, s.candidateSource)))
    return {};
  if (!current() || s.pendingVisit || !source.lifetime || !source.current ||
      !source.current(source.user) || !source.randomRaw || !source.math ||
      !s.next || s.next == UINT64_MAX || q.subtype != 0 || q.amount < 0 ||
      uint32_t(q.amount) > MaximumParticles)
    return {};
  for (float f : q.position)
    if (!std::isfinite(f))
      return {};
  if (s.candidateSlot == Capacity) {
    size_t slot = 0;
    while (slot < Capacity && s.live[slot].id)
      ++slot;
    if (slot == Capacity)
      return {};
    s.candidate = Instance{};
    s.candidate.request = q;
    s.candidate.particles.resize(size_t(q.amount));
    s.candidateSlot = slot;
    s.candidateSource = source;
    s.constructor.operations.clear();
  }
  s.constructor.replay();
  const auto &d = s.data.scalars;
  auto math = [&](NativeHarborStarMath op, float a, float b, float c = 0) {
    return s.constructor.math(source, op, a, b, c);
  };
  using M = NativeHarborStarMath;
  for (size_t i = 0; i < s.candidate.particles.size(); ++i) {
    auto raw = s.constructor.call(source, 0, {0, 0, 0});
    if (!raw || *raw > 32767)
      return {};
    // The actual source rand domain0..32767 makes the original mulhw remainder
    // exactly this unsigned remainder and the private signed conversion exact.
    auto scaled = math(M::Multiply, d[Tau], float(*raw % 360));
    if (!scaled)
      return {};
    auto angle = math(M::Divide, *scaled, d[FullTurn]);
    if (!angle)
      return {};
    auto reduced = [&]() -> std::optional<float> {
      if (*angle <= d[Pi]) {
        if (*angle <= d[HalfPi])
          return *angle;
        auto a = math(M::Subtract, *angle, d[HalfPi]);
        if (!a)
          return {};
        return math(M::Subtract, d[HalfPi], *a);
      }
      if (*angle < d[ThreeHalfPi])
        return math(M::Subtract, *angle, d[Pi]);
      auto a = math(M::Subtract, *angle, d[ThreeHalfPi]);
      if (!a)
        return {};
      return math(M::Subtract, d[HalfPi], *a);
    };
    auto u = reduced();
    if (!u)
      return {};
    auto square = math(M::Multiply, *u, *u);
    if (!square)
      return {};
    auto first = math(M::MultiplySubtract, d[SinFirst], *square, d[SinSecond]);
    if (!first)
      return {};
    auto second = math(M::MultiplyAdd, *first, *square, d[One]);
    if (!second)
      return {};
    auto sine = math(M::Multiply, *second, *u);
    if (!sine)
      return {};
    if (*angle > d[Pi])
      *sine = -*sine;
    auto vx = math(M::Multiply, d[Speed], *sine);
    if (!vx)
      return {};
    // Source independently recomputes the cosine reduction and square after vx.
    auto v = reduced();
    if (!v)
      return {};
    auto squareCos = math(M::Multiply, *v, *v);
    if (!squareCos)
      return {};
    auto firstCos =
        math(M::MultiplySubtract, d[CosFirst], *squareCos, d[CosSecond]);
    if (!firstCos)
      return {};
    auto cosine = math(M::MultiplyAdd, *firstCos, *squareCos, d[One]);
    if (!cosine)
      return {};
    if ((*angle > d[HalfPi] && *angle <= d[Pi]) ||
        (*angle > d[Pi] && *angle < d[ThreeHalfPi]))
      *cosine = -*cosine;
    auto vz = math(M::Multiply, d[Speed], *cosine);
    if (!vz)
      return {};
    auto &p = s.candidate.particles[i];
    p.position = {d[Zero], d[Zero], d[Zero]};
    p.velocity = {*vx, d[InitialUp], *vz};
    p.gravity = d[Speed];
    p.alpha = 0;
    p.delay = FirstParticleDelay + ParticleDelayStride * int32_t(i);
  }
  if (!current() || !source.current(source.user))
    return {};
  const uint64_t id = s.next++;
  s.candidate.id = id;
  s.live[s.candidateSlot] = std::move(s.candidate);
  s.sources[s.candidateSlot] = source;
  s.candidateSlot = Capacity;
  s.candidateSource = {};
  s.constructor.operations.clear();
  return id;
}
bool NativeHarborStarPointVisual::advance(uint64_t visit) {
  auto &s = *state_;
  if (s.invoking)
    return false;
  Guard guard(s.invoking);
  if (!current() || !visit || visit < s.lastVisit ||
      (s.pendingVisit && visit != s.pendingVisit))
    return false;
  if (visit == s.lastVisit)
    return true;
  if (!s.pendingVisit) {
    s.pendingVisit = visit;
    s.update.operations.clear();
  }
  s.update.replay();
  auto next = s.live;
  using M = NativeHarborStarMath;
  for (size_t i = 0; i < Capacity; ++i) {
    auto &v = next[i];
    if (!v.id)
      continue;
    auto &source = s.sources[i];
    if (!source.lifetime || !source.current || !source.current(source.user))
      return false;
    --v.lifetime;
    ++v.age;
    if (v.lifetime < 0) {
      v = Instance{};
      continue;
    }
    for (auto &p : v.particles) {
      if (p.delay) {
        --p.delay;
        p.alpha = 0;
        continue;
      }
      auto vy = s.update.math(source, M::Subtract, p.velocity[1], p.gravity);
      if (!vy)
        return false;
      p.velocity[1] = *vy;
      for (size_t axis = 0; axis < 3; ++axis) {
        auto value =
            s.update.math(source, M::Add, p.position[axis], p.velocity[axis]);
        if (!value)
          return false;
        p.position[axis] = *value;
      }
      p.alpha =
          *vy < s.data.scalars[Zero] ? ((p.alpha < 2) ? 0 : p.alpha - 2) : 255;
    }
  }
  if (!current())
    return false;
  s.live = std::move(next);
  for (size_t i = 0; i < Capacity; ++i)
    if (!s.live[i].id)
      s.sources[i] = {};
  s.lastVisit = visit;
  s.pendingVisit = 0;
  s.update.operations.clear();
  return true;
}
const std::array<NativeHarborStarPointVisual::Instance,
                 NativeHarborStarPointVisual::Capacity> &
NativeHarborStarPointVisual::instances() const {
  return state_->live;
}
bool NativeHarborStarPointVisual::retire() {
  auto &s = *state_;
  if (s.invoking || s.checking)
    return false;
  s.retired = true;
  s.live = {};
  s.sources = {};
  s.candidate = Instance{};
  s.candidateSlot = Capacity;
  s.candidateSource = {};
  s.constructor.operations.clear();
  s.update.operations.clear();
  return true;
}
} // namespace ttyd

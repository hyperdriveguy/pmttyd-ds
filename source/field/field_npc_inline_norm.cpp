#include "field_npc_inline_norm.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
// memcpy widths4/8 preserve IEEE float/double payloads. wide forces each
// source binary64 multiply rounding boundary before the following operation.
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
double realDouble(uint64_t w) {
  double f;
  std::memcpy(&f, &w, 8);
  return f;
}
double wide(double f) {
  volatile double v = f;
  return v;
}
} // namespace
// Opt-in native seed1/sqrt(double); this is not Gekko frsqrte's estimate.
// Core still performs all three source Newton steps; no implicit fallback.
bool nativeFieldNpcReciprocalSqrtEstimate(void *, double q, double &out) {
  out = 1.0 / std::sqrt(q);
  return true;
}
// State1..4 positive graph;10/11 nonpositive/unordered classification;
//20 terminal publication without reset. Reject changed input bits/providers
// across holds (including signed zero/NaN payload differences), never silently
// replace the caller's already captured squared-norm operation graph.
bool advanceFieldNpcInlineNorm(float squared, float zero,
                               const FieldNpcNormServices &s,
                               FieldNpcNormCursor &c, float &out) {
  if (!s.memory.word)
    return false;
  if (!c.active) {
    c = {};
    c.active = true;
    c.owner = s;
    c.squared = squared;
    c.zero = zero;
    c.result = squared;
    c.phase = squared > zero ? 1 : 10;
  }
  if (bits(c.squared) != bits(squared) || bits(c.zero) != bits(zero) ||
      c.owner.memory.user != s.memory.user ||
      c.owner.memory.word != s.memory.word ||
      c.owner.doubleWord != s.doubleWord || c.owner.estimate != s.estimate)
    return false;
  auto loadDouble = [&](uint32_t a, double &v) {
    auto w = s.doubleWord ? s.doubleWord(s.memory.user, a) : std::nullopt;
    if (!w)
      return false;
    v = realDouble(*w);
    return true;
  };
  for (;;)
    switch (c.phase) {
    // q>capturedZero only, including+infinity: request seed, then original
    // double HALF802c13c8 and THREE802c13d0. Three refinements retain double
    // rounded square/scaled products and NEGATED fused(q*square-THREE).
    // Final q*seed rounds to double then float; direct sqrtf changes this graph.
    case 1:
      if (!s.estimate || !s.estimate(s.memory.user, double(c.squared), c.seed))
        return false;
      ++c.phase;
      break;
    case 2:
      if (!loadDouble(0x802c13c8, c.half))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!loadDouble(0x802c13d0, c.three))
        return false;
      ++c.phase;
      break;
    case 4:
      for (unsigned i = 0; i < 3; ++i) {
        const double square = wide(c.seed * c.seed),
                     scaled = wide(c.half * c.seed);
        const double correction =
            -std::fma(double(c.squared), square, -c.three);
        c.seed = wide(scaled * correction);
      }
      c.result = float(wide(double(c.squared) * c.seed));
      c.phase = 20;
      break;
    // Load original double ZERO802c13d8 before classifying. Negative or NaN
    // uses canonical float word8041e3c8(original7fffffff). Exponent mask7f800000
    // plus nonzero fraction007fffff detects NaN; other nonpositive classes keep
    // input q, including signed zero. No added finite/epsilon rescue branch.
    case 10: {
      double zeroDouble;
      if (!loadDouble(0x802c13d8, zeroDouble))
        return false;
      const auto w = bits(c.squared);
      c.phase = double(c.squared) < zeroDouble ||
                        ((w & 0x7f800000u) == 0x7f800000u && (w & 0x007fffffu))
                    ? 11
                    : 20;
      break;
    }
    case 11: {
      auto w = s.memory.word(s.memory.user, 0x8041e3c8);
      if (!w)
        return false;
      c.result = real(*w);
      c.phase = 20;
      break;
    }
    case 20:
      out = c.result;
      return true;
    default:
      return false;
    }
}
} // namespace ttyd

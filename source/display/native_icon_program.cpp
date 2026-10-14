#include "native_icon_program.hpp"
namespace ttyd {
namespace {
using Args = std::array<uint32_t, 8>;
Args args(std::initializer_list<uint32_t> values) {
  Args out{};
  unsigned i = 0;
  for (auto v : values)
    out[i++] = v;
  return out;
}
bool equals(const std::optional<Args> &a,
            std::initializer_list<uint32_t> values) {
  return a && *a == args(values);
}
} // namespace
// Original __GXInitGX802b30bc..30d0 establishes identity table0.
NativeIconProgram::NativeIconProgram() { swaps_[0] = Args{0, 0, 1, 2, 3}; }
bool NativeIconProgram::request(const SourceIconGXCall &r) {
  const auto &a = r.integers;
  const auto stage = a[0];
  auto exact = [&](std::initializer_list<uint32_t> v) { return a == args(v); };
  auto setter = [&](std::optional<Args> Stage::*member) {
    if (stage >= 3)
      return false;
    stages_[stage].*member = a;
    return true;
  };
  switch (r.function) {
  case 0x802b6f3c:
    if (!exact({1}))
      return false;
    chans_ = 1;
    return true;
  case 0x802b6d6c:
  case 0x802b6e54:
    if (a != args({4, a[1]}))
      return false;
    (r.function == 0x802b6d6c ? ambient_ : material_) = a[1];
    return true;
  case 0x802b6f78: {
    if ((stage != 0 && stage != 2 && stage != 4) || a[1] != 0 || a[2] != 0 ||
        a[3] != 0 || a[4] != 0 || a[5] > 2 || a[6] > 2 || a[7])
      return false;
    if (stage == 0 || stage == 4)
      channels_[0] = a;
    if (stage == 2 || stage == 4)
      channels_[1] = a;
    return true;
  }
  case 0x802b8fcc:
    if (a != args({stage}) || stage < 1 || stage > 3)
      return false;
    count_ = stage;
    return true;
  case 0x802b8ab8:
    if (stage > 2 || a != args({stage, a[1]}))
      return false;
    colors_[stage] = a[1];
    return true;
  case 0x802b8e30:
    return setter(&Stage::order);
  case 0x802b89e8:
    if (a != args({stage, 0, 0, 0, 1, 0}))
      return false;
    return setter(&Stage::colorOp);
  case 0x802b8a50:
    if (a != args({stage, 0, 0, 0, 1, 0}))
      return false;
    return setter(&Stage::alphaOp);
  case 0x802b8960:
    return setter(&Stage::colorIn);
  case 0x802b89a4:
    return setter(&Stage::alphaIn);
  case 0x802b8c98:
    if (a != args({stage, a[1], a[2]}) || a[1] > 3 || a[2] > 3)
      return false;
    return setter(&Stage::swap);
  case 0x802b8ce0:
    if (stage > 3 || a != args({stage, a[1], a[2], a[3], a[4]}) || a[1] > 3 ||
        a[2] > 3 || a[3] > 3 || a[4] > 3)
      return false;
    swaps_[stage] = a;
    return true;
  case 0x802b88d4:
    if (!exact({0, 0}))
      return false;
    stages_[0].colorOp = args({0, 0, 0, 0, 1, 0});
    stages_[0].alphaOp = args({0, 0, 0, 0, 1, 0});
    stages_[0].colorIn = args({0, 15, 8, 10, 15});
    stages_[0].alphaIn = args({0, 7, 4, 5, 7});
    return true;
  case 0x802b5084:
    if (!exact({1}))
      return false;
    generators_ = 1;
    return true;
  case 0x802b4e04:
    if (!exact({0, 1, 4, 60, 0, 125}))
      return false;
    coord_ = true;
    return true;
  case 0x802b8ff4:
    if (a != args({0, a[1]}) || r.reals != std::array<float, 4>{})
      return false;
    fogOff_ = true;
    fog_ = a[1];
    return true;
  case 0x802b5c8c:
    if (a != args({stage}) || stage > 2)
      return false;
    cull_ = stage;
    return true;
  case 0x802b4520:
    if (!exact({}))
      return false;
    descriptors_ = {};
    return true;
  case 0x802b3c08: {
    int i = stage == 9 ? 0 : stage == 11 ? 1 : stage == 13 ? 2 : -1;
    if (i < 0 || !exact({stage, 1}))
      return false;
    descriptors_[i] = true;
    return true;
  }
  case 0x802b4558: {
    int i = a[1] == 9 ? 0 : a[1] == 13 ? 2 : -1;
    if (i < 0 || !exact({0, a[1], 1, 4, 0}))
      return false;
    formats_[i] = true;
    return true;
  }
  default:
    return false;
  }
}
std::optional<NativeIconPlan> NativeIconProgram::plan() const {
  if (chans_ != 1 || !material_ || !channels_[0] || !channels_[1] || !count_ ||
      !fogOff_ || !fog_ || !cull_ || generators_ != 1 || !coord_ ||
      descriptors_ != std::array<bool, 3>{true, false, true} || !formats_[0] ||
      !formats_[2])
    return {};
  NativeIconPlan p;
  p.material = *material_;
  p.fog = *fog_;
  p.cull = *cull_;
  auto ops = [&](unsigned i) {
    return equals(stages_[i].colorOp, {i, 0, 0, 0, 1, 0}) &&
           equals(stages_[i].alphaOp, {i, 0, 0, 0, 1, 0});
  };
  bool gray = *count_ == 3 && colors_[1] && colors_[2];
  for (unsigned i = 0; i < 3 && gray; ++i) {
    const auto &s = stages_[i];
    gray = ops(i) && equals(s.order, {i, 0, 0, 255}) &&
           equals(s.swap, {i, 0, i + 1}) &&
           equals(swaps_[i + 1], {i + 1, i, i, i, 3}) &&
           equals(s.colorIn, {i, 15, 2, 8, i ? 0u : 15u}) &&
           equals(s.alphaIn,
                  i == 2 ? std::initializer_list<uint32_t>{2, 7, 2, 4, 7}
                         : std::initializer_list<uint32_t>{i, 7, 7, 7, 4});
  }
  if (gray) {
    p.mode = NativeIconPlan::Grayscale;
    p.tint = *colors_[1];
    p.blend = *colors_[2];
    return p;
  }
  if (!equals(swaps_[0], {0, 0, 1, 2, 3}))
    return {};
  const auto &a = stages_[0];
  const auto &b = stages_[1];
  if ((*count_ != 2 && *count_ != 3) || !colors_[1] || !ops(0) || !ops(1) ||
      !equals(a.order, {0, 0, 0, 4}) || !equals(a.swap, {0, 0, 0}) ||
      !equals(a.colorIn, {0, 15, 8, 10, 15}) ||
      !equals(a.alphaIn, {0, 7, 4, 5, 7}) ||
      !equals(b.order, {1, 255, 255, 255}) ||
      !equals(b.colorIn, {1, 15, 2, 0, 15}) ||
      !equals(b.alphaIn, {1, 7, 1, 0, 7}))
    return {};
  p.tint = *colors_[1];
  if (*count_ == 3) {
    const auto &s = stages_[2];
    if (!colors_[2] || !ops(2) || !equals(s.order, {2, 255, 255, 255}) ||
        !equals(s.colorIn, {2, 15, 0, 4, 15}) ||
        !equals(s.alphaIn, {2, 7, 0, 2, 7}))
      return {};
    p.mode = NativeIconPlan::Blended;
    p.blend = *colors_[2];
  }
  return p;
}
} // namespace ttyd

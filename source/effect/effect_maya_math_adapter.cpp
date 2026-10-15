#include "effect_maya_math_adapter.hpp"
#include "battle_audience_aux_callback.hpp"
namespace ttyd {
namespace {
// Reuse the established affine provider with bounded copied payloads. It may
// reject outside its finite domain;do not acknowledge that as identity output.
bool calculate(BattleAudienceMatrixRequest &request, EffectMayaMatrix &output) {
  BattleAudienceAuxServices math;
  bindAudienceAuxMathAdapter(math);
  if (!math.matrix(nullptr, request))
    return false;
  output = request.output;
  return true;
}
} // namespace
// SDK tokens:Translate29e230,Scale29e2b0,RotRad29dff0,Concat29dd64.
// left/right order stays source order;native host/libm rotations are policy,
// not measured Gekko/FPSCR/raster parity. Memory and user remain caller-owned.
void bindEffectMayaMatrixMathAdapter(EffectMayaMatrixServices &s) {
  s.binary = effectMayaBinary32;
  s.fused = effectMayaFused32;
  s.translate = [](void *, const std::array<float, 3> &xyz,
                   EffectMayaMatrix &out) {
    BattleAudienceMatrixRequest r;
    r.function = 0x8029e230;
    r.xyz = xyz;
    return calculate(r, out);
  };
  s.scale = [](void *, const std::array<float, 3> &xyz, EffectMayaMatrix &out) {
    BattleAudienceMatrixRequest r;
    r.function = 0x8029e2b0;
    r.xyz = xyz;
    return calculate(r, out);
  };
  s.rotate = [](void *, uint32_t axis, float radians, EffectMayaMatrix &out) {
    BattleAudienceMatrixRequest r;
    r.function = 0x8029dff0;
    r.axis = axis;
    r.radians = radians;
    return calculate(r, out);
  };
  s.concat = [](void *, const EffectMayaMatrix &left,
                const EffectMayaMatrix &right, EffectMayaMatrix &out) {
    BattleAudienceMatrixRequest r;
    r.function = 0x8029dd64;
    r.left = left;
    r.right = right;
    return calculate(r, out);
  };
}
} // namespace ttyd

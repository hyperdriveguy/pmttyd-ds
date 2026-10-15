#include "effect_maya_math_adapter.hpp"
#include "battle_audience_aux_callback.hpp"
namespace ttyd {
namespace {
bool calculate(BattleAudienceMatrixRequest &request, EffectMayaMatrix &output) {
  BattleAudienceAuxServices math;
  bindAudienceAuxMathAdapter(math);
  if (!math.matrix(nullptr, request))
    return false;
  output = request.output;
  return true;
}
} // namespace
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

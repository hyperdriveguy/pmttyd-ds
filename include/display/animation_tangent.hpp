#pragma once
#include <array>
#include <cmath>
// Shared legacy table adaptation, not a serialized original tangent asset.
//178 rows span signed degrees-89..88, index89 is zero; factor3c8efa33 is
// original binary32 0.017453288659453392, not an exact pi/180 substitute.
// Static local initialization generates once per process and need not follow
// live source animInit cadence or alias guest tangent storage.
namespace ttyd {
// Native libm adaptation of animInit's runtime table; no Gekko tan parity claim.
inline float animationTangentRounded(float value){volatile float result=value;return result;}
inline const std::array<float,178>& nativeAnimationTangents(){
 static const auto table=[](){std::array<float,178> values{};
  for(int i=0;i<178;++i){const float angle=animationTangentRounded(float(i-89)*.017453288659453392f);values[unsigned(i)]=float(std::tan(double(angle)));}
  return values;}();return table;
}
}

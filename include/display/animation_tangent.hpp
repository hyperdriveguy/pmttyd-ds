#pragma once
#include <array>
#include <cmath>
namespace ttyd {
// Native libm adaptation of animInit's runtime table; no Gekko tan parity claim.
inline float animationTangentRounded(float value){volatile float result=value;return result;}
inline const std::array<float,178>& nativeAnimationTangents(){
 static const auto table=[](){std::array<float,178> values{};
  for(int i=0;i<178;++i){const float angle=animationTangentRounded(float(i-89)*.017453288659453392f);values[unsigned(i)]=float(std::tan(double(angle)));}
  return values;}();return table;
}
}

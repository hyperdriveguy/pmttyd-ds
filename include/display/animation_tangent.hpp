#pragma once
#include <array>
#include <cmath>
#include <cassert>
#include <cstdint>
#include <cstring>
// Legacy fallback below is a native adaptation, not a serialized tangent asset.
// AnimationTangentTable instead owns actual live source VM bytes.
//178 rows span signed degrees-89..88, index89 is zero; factor3c8efa33 is
// original binary32 0.017453288659453392, not an exact pi/180 substitute.
// Static local initialization generates once per process and need not follow
// live source animInit cadence or alias guest tangent storage.
namespace ttyd {
// Retained original runtime table bytes, shared directly with the source VM.
// Values are decoded as BE binary32; no cached float copy shadows VM writes.
struct AnimationTangentTable {
 static constexpr unsigned Count=178;
 std::array<uint8_t,Count*4> state{};
 uint32_t word(unsigned i) const{assert(i<Count);const auto* p=state.data()+4*i;return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];}
 void word(unsigned i,uint32_t v){assert(i<Count);for(unsigned b=0;b<4;++b)state[4*i+b]=uint8_t(v>>(24-8*b));}
 float at(unsigned i) const{const uint32_t v=word(i);float f;std::memcpy(&f,&v,4);return f;}
};
// Native libm adaptation of animInit's runtime table; no Gekko tan parity claim.
inline float animationTangentRounded(float value){volatile float result=value;return result;}
inline const std::array<float,178>& nativeAnimationTangents(){
 static const auto table=[](){std::array<float,178> values{};
  for(int i=0;i<178;++i){const float angle=animationTangentRounded(float(i-89)*.017453288659453392f);values[unsigned(i)]=float(std::tan(double(angle)));}
  return values;}();return table;
}
}

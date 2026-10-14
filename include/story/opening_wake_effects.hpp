#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
namespace ttyd {
struct OpeningWakeTap {float u=0,v=0,weight=.125f;};
inline OpeningWakeTap openingWakeTap(unsigned index,float depth) {
    constexpr float pi=3.1415927410125732f,half=1.5707963705062866f,threeHalf=4.71238899230957f;
    const float angle=(6.2831854820251465f*float(index))*.125f;
    float s=angle,c=angle;bool negS=false,negC=false;
    if(angle<=pi){if(angle>half){s=half-(angle-half);c=s;negC=true;}}
    else if(angle<threeHalf){s=angle-pi;c=s;negS=negC=true;}
    else {s=half-(angle-threeHalf);c=s;negS=true;}
    const float ss=s*s,cc=c*c;
    float sinValue=std::fma(std::fma(.00761000020429492f,ss,-.1660500019788742f),ss,1.f)*s;
    float cosValue=std::fma(std::fma(.037050001323223114f,cc,-.4966999888420105f),cc,1.f);
    if(negS)sinValue=-sinValue;
    if(negC)cosValue=-cosValue;
    const float amplitude=depth/100.f;
    return {amplitude*sinValue/100.f,amplitude*cosValue/100.f,.125f};
}
}

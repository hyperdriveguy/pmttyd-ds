#pragma once
#include "animation_main_replay.hpp"
namespace ttyd {
// Retained source addresses/numeric operands, with no independent VM ownership.
// copy shares replay's exact overlap/counter semantics; captured pins sample
// and counts for the full producer pass, including signed-zero/NaN fraction bits.
struct AnimationMainInterpolationCursor {
 bool active=false;uint32_t phase=0,channel=0,key=0,table=0,stream=0,offset=0,count=0,source=0,end=0,destination=0,cache=0,remaining=0,lane=0,index=0,angle=0;
 float scale=0,scaledFraction=0,oldValue=0,newValue=0,nextTime=0,duration=0,square=0,cube=0,delta=0,two=0,three=0,negativeTwo=0,tangent=0,otherTangent=0,h10=0,h01=0,term=0;
 std::array<float,3> deltas{};AnimationReplayCopy copy;
 AnimationMainEvaluation input;AnimationMainReplayResult captured;AnimationMainReplayServices owner;
};
// Entire original80040084..80040540: captured current/next/fraction/counts
// come from successful replay, not fresh live fields. Reads/copies/updates and
// finalR3c thenR40 stores retain progress across unavailable primitives and
// cooperative per-copy/entry yields. Keep the same producer pass and owners.
// Uses the actual replay VM/GQR services, including real tangent-table aliases.
// Completion does not acknowledge the optional facing tail or full Main.
bool advanceAnimationMainInterpolation(const AnimationMainEvaluation&,const AnimationMainReplayResult&,const AnimationMainReplayServices&,AnimationMainInterpolationCursor&);
struct AnimationMainGeometryCursor {
 bool active=false;uint32_t phase=0;AnimationMainEvaluation input;
 AnimationMainTimeServices timeOwner;AnimationMainReplayServices memoryOwner;
 AnimationMainCacheCursor cache;AnimationMainReplayResult captured;AnimationMainInterpolationCursor interpolation;
};
// Contiguous nonnull Main8003ee08..80040540. Later interpolation holds never
// repeat scalar/cache work. This owner is suitable for Main's evaluation child;
// complete native platform/linked/facing ownership remains caller responsibility.
bool advanceAnimationMainGeometry(const AnimationMainEvaluation&,const AnimationMainTimeServices&,const AnimationMainReplayServices&,AnimationMainGeometryCursor&);
}

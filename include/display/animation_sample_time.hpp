#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct AnimationSampleTimePlan {
    float frame=0,span=0,ratio=0,selected=0;
    bool loopConversion=false;
};
// Exact source single-operation span/ratio branches. The caller publishes ratio
// before the loop conversion and selected time afterward.
AnimationSampleTimePlan animationSampleTimePlan(float frame,uint32_t loop,float start,float end);
using AnimationLoopConversion=std::optional<int32_t>(*)(void*,float);
std::optional<int32_t> nativeAnimationLoopConversion(void*,float);
std::optional<float> finishAnimationSampleTime(const AnimationSampleTimePlan&,float publishedRatio,
    void* user=nullptr,AnimationLoopConversion=nativeAnimationLoopConversion);
}

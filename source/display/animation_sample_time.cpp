#include "animation_sample_time.hpp"
#include <cmath>
namespace ttyd {
namespace {float single(float value){volatile float result=value;return result;}}
// Zero span publishes offset as ratio and selects start. Looping negative
// offset also selects start without conversion; nonloop clamps start/end.
// Plan construction is separate because ratio is published before fctiwz.
AnimationSampleTimePlan animationSampleTimePlan(float frame,uint32_t loop,float start,float end){
    AnimationSampleTimePlan plan;plan.frame=frame;plan.span=single(end-start);plan.selected=start;
    const auto offset=single(frame-start);
    if(plan.span==0){plan.ratio=offset;return plan;}
    if(loop){
        if(offset<0)return plan;
        plan.ratio=single(offset/plan.span);plan.loopConversion=true;return plan;
    }
    if(frame<start)return plan;
    plan.ratio=single(offset/plan.span);plan.selected=frame>end?end:frame;return plan;
}
// Signed32 finite conversion domain[-2^31,2^31). Upper float bound is
// exclusive:2147483647 is not exactly representable as binary32.
std::optional<int32_t> nativeAnimationLoopConversion(void*,float value){
    // Native finite fctiwz adapter; FPSCR/NaN/overflow conversion domains remain
    // explicit providers instead of invoking an undefined C++ integer cast.
    if(!std::isfinite(value) || value<-2147483648.f || value>=2147483648.f)return {};
    return int32_t(value);
}
// Remove truncated loop count * span with one FMA from original frame;
// using fmod or start-relative reconstruction can change rounding/publication.
std::optional<float> finishAnimationSampleTime(const AnimationSampleTimePlan& plan,float publishedRatio,void* user,AnimationLoopConversion convert){
    if(!plan.loopConversion)return plan.selected;
    if(!convert)return {};
    const auto index=convert(user,publishedRatio);if(!index)return {};
    return std::fma(-float(*index),plan.span,plan.frame);
}
}

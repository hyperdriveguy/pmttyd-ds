#include "animation_sample_time.hpp"
#include "actor_animation.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
int main(){
    auto p=animationSampleTimePlan(26,1,5,15);
    assert(p.span==10 && p.ratio==2.1f && p.loopConversion);
    assert(*finishAnimationSampleTime(p,p.ratio)==6);
    // The source reloads the published ratio before integer conversion.
    assert(*finishAnimationSampleTime(p,1)==16);
    p=animationSampleTimePlan(15,1,5,15);
    assert(*finishAnimationSampleTime(p,p.ratio)==5);
    p=animationSampleTimePlan(4,1,5,15);
    assert(p.ratio==0 && !std::signbit(p.ratio) && !p.loopConversion && *finishAnimationSampleTime(p,p.ratio)==5);
    p=animationSampleTimePlan(26,0,5,15);
    assert(p.ratio==2.1f && *finishAnimationSampleTime(p,p.ratio)==15);
    p=animationSampleTimePlan(4,0,5,15);
    assert(p.ratio==0 && *finishAnimationSampleTime(p,p.ratio)==5);
    p=animationSampleTimePlan(2,1,5,5);
    assert(p.ratio==-3 && !p.loopConversion && *finishAnimationSampleTime(p,p.ratio)==5);
    const float nan=std::numeric_limits<float>::quiet_NaN();
    p=animationSampleTimePlan(nan,0,5,15);
    assert(std::isnan(p.ratio) && std::isnan(*finishAnimationSampleTime(p,p.ratio)));
    p=animationSampleTimePlan(nan,1,5,15);
    assert(p.loopConversion && !finishAnimationSampleTime(p,p.ratio));
    assert(!nativeAnimationLoopConversion(nullptr,2147483648.f));
    assert(*nativeAnimationLoopConversion(nullptr,-2147483648.f)==INT32_MIN);
    assert(*nativeAnimationLoopConversion(nullptr,-2.9f)==-2);
    assert(!nativeAnimationLoopConversion(nullptr,std::numeric_limits<float>::infinity()));
    // A fused subtract preserves the residual lost by separate rounding.
    p.frame=16777216.f;p.span=1.00000011920928955078125f;p.loopConversion=true;
    const auto fused=finishAnimationSampleTime(p,16777215.f);
    assert(fused && *fused==std::fma(-16777215.f,p.span,p.frame));
    volatile float product=16777215.f*p.span;
    assert(*fused!=p.frame-product);
    ActorAnimationPose pose;pose.start=5;pose.end=15;pose.loop=1;
    pose.groups.push_back(ActorGroup{-1,-1,0,0,0});pose.nodes.resize(24);
    pose.nodes[3]=pose.nodes[4]=pose.nodes[5]=1;
    pose.keys.resize(2);pose.keys[0].time=5;pose.keys[1].time=15;
    ActorAnimationSample sample;
    assert(pose.evaluateSelected(16,sample) && sample.selectedTime==16 && sample.last==1 && sample.next==0);
    assert(pose.evaluate(16,sample) && sample.selectedTime==6 && sample.last==0);
    assert(!pose.evaluateSelected(4,sample));
}

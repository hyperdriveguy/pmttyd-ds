#pragma once
#include "animation_main_time.hpp"
namespace ttyd {
struct AnimationMainTimelineServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
};
struct AnimationMainTimelineCursor {
 bool active=false;uint32_t phase=0,loop=0,count=0,index=0,pointer=0,previous=0,next=0,keys=0,state=0,current=0,control=0;
 AnimationMainEvaluation input;AnimationMainTimelineServices owner;
 float selected=0,keyTime=0,numerator=0,denominator=0,end=0,zero=0,fraction=0;
};
// Complete Main8003ef80..f0f4: both scans, indices/clamps, discrete state and
// interpolation fraction. loop is captured by the earlier time child, not
// reloaded here. No count/index/finite/pre-table guard or fallback is invented.
// False may follow acknowledged stores or one source scan backedge. Retain
// this cursor, request, service identities and producer pass; never replay a
// completed parent. Primitive unavailable/false has no effects. One backedge
// per resume is native scheduling policy, not an original total iteration cap.
bool advanceAnimationMainTimeline(const AnimationMainEvaluation&,uint32_t loop,const AnimationMainTimelineServices&,AnimationMainTimelineCursor&);
struct AnimationMainScalarCursor {
 bool active=false;uint32_t phase=0;
 AnimationMainEvaluation input;AnimationMainTimeServices owner;
 AnimationMainTimeResult timeResult;AnimationMainTimeCursor time;AnimationMainTimelineCursor timeline;
};
// Complete contiguous scalar region8003ee08..8003f0f4. Successful time work
// is retained while scans/state/fraction suspend. This does not acknowledge
// the later cache/output evaluator or full Main.
bool advanceAnimationMainScalar(const AnimationMainEvaluation&,const AnimationMainTimeServices&,AnimationMainScalarCursor&);

}

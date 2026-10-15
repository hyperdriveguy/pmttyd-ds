#pragma once
#include "animation_main.hpp"
namespace ttyd {
struct AnimationMainTimeServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 // One acknowledged original lfd, not independently suspended word halves.
 std::optional<uint64_t> (*doubleWord)(void*,uint32_t)=nullptr;
 bool (*divide)(void*,uint64_t,uint32_t,uint64_t&)=nullptr; // signed __div2i raw bits
 bool (*convertUnsigned)(void*,uint64_t,float&)=nullptr;
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
};
struct AnimationMainTimeResult {uint32_t loop=0;};
struct AnimationMainTimeCursor {
 bool active=false;uint32_t phase=0,mode=0,control=0,loop=0,gp=0,bus=0,hi=0,lo=0,epochLo=0,epochHi=0,divisor=0;
 AnimationMainEvaluation input;AnimationMainTimeServices owner;
 uint64_t quotient=0,bias=0;int32_t integer=0;
 float converted=0,sixty=0,thousand=0,zero=0,frame=0,rate=0,start=0,end=0,span=0,ratio=0,selected=0;
};
// Original nonnull Main8003ee08..8003ef80, ending after R24 publication.
// Source word/store/numeric unavailable operations have no effects. Earlier
// acknowledged operations may have published state; resume this SAME cursor,
// input, service identities and producer pass. Output publishes at completion.
// No validity clamps or finite conversion defaults are invented. Full remaining
// timeline/cache/output evaluation and outer Main are separate retained owners.
bool advanceAnimationMainTime(const AnimationMainEvaluation&,const AnimationMainTimeServices&,AnimationMainTimeCursor&,AnimationMainTimeResult&);
}

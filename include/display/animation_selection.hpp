#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct AnimationSelectionServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 // Original strcmp child, left directory slot/right raw selector. Provider
 // retains any partial child; acknowledged result is captured only once.
 bool (*compare)(void*,uint32_t,uint32_t,int32_t&)=nullptr;
 bool (*divide)(void*,uint64_t,uint32_t,uint64_t&)=nullptr;
};
struct AnimationSelectionCursor {
 bool active=false;uint32_t phase=0,handle=0,selector=0,force=0;
 uint32_t work=0,pool=0,models=0,pose=0,index=0,entry=0,holder=0,model=0,count=0,selected=0,directory=0,linked=0,flags=0,mode=0,bus=0,gp=0,hi=0,lo=0,zero=0;
 uint64_t quotient=0;AnimationSelectionServices owner;
};
// Complete local animPoseSetAnim80041170/17c. Raw low32 address arithmetic,
// signed selector/count, first matching name or row0, original force/clock/
// linked/reset order. No ID/resource/finite clamps. False can follow stores;
// resume SAME cursor, inputs, services and exclusive producer lifetime. Child
// unavailability has no newly acknowledged effects. Budget is native scheduling.
bool selectAnimation(uint32_t handle,uint32_t selector,uint32_t force,const AnimationSelectionServices&,AnimationSelectionCursor&,unsigned visits=64);
}

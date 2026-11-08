#pragma once
#include "animation_draw_wrapper.hpp"
#include "animation_facing.hpp"
#include "battle_audience_draw.hpp"
namespace ttyd {
// Original dispProc ABI. Pass belongs to callback/work state, not this request.
struct AnimationInnerTraversal {
 uint32_t parent=0xffffffff,group=0,current=0,next=0;float fraction=0;
};
struct AnimationInnerServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*gx)(void*,const BattleAudienceGXRequest&)=nullptr;
 bool (*callback)(void*,uint32_t,uint32_t)=nullptr;
 AnimationFacingServices facing;
 bool (*main)(void*,uint32_t)=nullptr;
 bool (*copy)(void*,const BattleActMatrix&,uint32_t destination)=nullptr;
 bool (*flush)(void*,uint32_t address,uint32_t bytes)=nullptr;
 bool (*invalidate)(void*)=nullptr;
 bool (*traverse)(void*,const AnimationInnerTraversal&)=nullptr;
};
// Bounded retained state; matrix is the copied caller input/effective output.
// pending/gxStep pin one GX call. Original sample captures survive late Main.
// Providers/producer lifetime remain caller-owned through every budget yield.
struct AnimationInnerCursor {
 bool active=false,ring=false,pending=false;uint32_t phase=0,mapPhase=0,gxStep=0;
 AnimationInnerDrawRequest input;AnimationInnerServices owner;
 uint32_t work=0,index=0,models=0,animation=0,entry=0,holder=0,model=0,current=0,next=0,fraction=0,directory=0,descriptor=0,mode=0,value=0,flags=0,primary=0,secondary=0,effective=0,callback=0;
 uint32_t mapIndex=0,mapBase=0,mapGroup=0,handle=0,pool=0,capacity=0,positions=0,auxiliary=0,total=0,cursor=0,base=0,selector=0,output=0,bytes=0;
 BattleActMatrix matrix{};AnimationFacingCursor facing;BattleAudienceGXRequest gx;
 AnimationInnerTraversal traversal;
};
// Complete local _animPoseDrawMtx8003bc68 parent + mapTestXLU leaf. Child
// effects may precede false; retain exact input/cursor/providers/pass. Word
// arithmetic wraps32; no source ID/count/pass/finite clamps. Source GPU/SDK/
// Main/traversal children require actual owners. Budget is native scheduling.
// Default 128 limits native work per pump; it does not cap total source work.
bool resumeAnimationInner(const AnimationInnerDrawRequest&,const AnimationInnerServices&,AnimationInnerCursor&,unsigned visits=128);
}

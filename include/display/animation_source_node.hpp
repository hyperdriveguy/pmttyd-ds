#pragma once
#include "animation_group.hpp"
namespace ttyd {
struct AnimationNodeRef {bool guest=false;uint32_t value=0;};
struct AnimationSourceMatrixCall {
 uint32_t function=0,axis=122;AnimationNodeRef left,right,output;
 std::array<float,3> xyz{};float radians=0;
};
using AnimationNodeLocals=std::array<BattleActMatrix,10>;
enum class AnimationNodeNumber {Multiply,Divide};
struct AnimationSourceNodeServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 std::optional<uint32_t> (*number)(void*,AnimationNodeNumber,uint32_t,uint32_t)=nullptr;
 bool (*matrix)(void*,const AnimationSourceMatrixCall&,AnimationNodeLocals&)=nullptr;
};
struct AnimationSourceNodeCursor {
 bool active=false,pending=false;AnimationGroupNode input;AnimationSourceNodeServices owner;
 uint32_t phase=0,capture=0,mask=0,rotation=0,literal=0,zero=0,two=0,value=0,angle=0,radians=0,level=0,target=0,selected=0,step=0,count=0;
 std::array<uint32_t,24> node{};std::array<uint32_t,3> compensation{};
 AnimationNodeLocals locals{};AnimationSourceMatrixCall request;
 std::array<AnimationSourceMatrixCall,3> dispatch{};
};
// Complete Joint8003e2a8 / Transform8003e7ec local bodies. Exact 12/21 field
// captures, live literals, original table targets, stack publication/reload.
// Local SDK scratch has durable identities; guest references remain VM addresses.
bool resumeAnimationSourceNode(const AnimationGroupNode&,const AnimationSourceNodeServices&,AnimationSourceNodeCursor&,unsigned visits=128);
}

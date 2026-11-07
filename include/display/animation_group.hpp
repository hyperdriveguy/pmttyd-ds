#pragma once
#include "animation_inner.hpp"
#include <memory>
#include <vector>
namespace ttyd {
// Guest node/matrix addresses stay tokens; the children own SDK/numeric effects.
// Parent is a scale triple for Joint; Transform ignores that triple.
struct AnimationGroupNode {
 uint32_t node=0;bool joint=false;std::array<float,3> parent{};
};
struct AnimationGroupVivian {
 uint32_t matrix=0,saved=0;std::array<float,4> parameters{};
};
struct AnimationGroupServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*node)(void*,const AnimationGroupNode&)=nullptr;
 bool (*copy)(void*,uint32_t source,uint32_t destination)=nullptr;
 bool (*material)(void*,uint32_t shape,uint32_t current)=nullptr;
 bool (*vivian)(void*,const AnimationGroupVivian&)=nullptr;
 bool (*callback)(void*,uint32_t identity,uint32_t data,uint32_t group,uint32_t matrix)=nullptr;
 bool (*render)(void*,uint32_t shape,uint32_t current)=nullptr;
 // Original single fsubs followed by fmadds; explicit numeric provider.
 std::optional<uint32_t> (*blend)(void*,uint32_t coefficient,uint32_t source,uint32_t old)=nullptr;
};
// One retained recursive invocation. Heap frame addresses remain stable when
// the vector grows; saved parent references survive push(). Fields are captured
// source operands, not a validated model tree or a matrix snapshot.
struct AnimationGroupFrame {
 AnimationInnerTraversal call;uint32_t phase=0;
 uint32_t work=0,model=0,pose=0,groups=0,group=0,visibility=0,index=0,nodes=0,parentGroup=0,kind=0,value=0;
 uint32_t matrix=0,pointer=0,destination=0,x=0,y=0,z=0,seed=0,shape=0,callback=0,data=0;
 uint32_t pair=0,animation=0,coefficient=0,source=0,old=0,result=0,lane=0,level=0;
 AnimationGroupNode node;AnimationGroupVivian vivian;
};
struct AnimationGroupCursor {
 bool active=false;AnimationInnerTraversal input;AnimationGroupServices owner;
 // Stable frames are reused at their observed high-water depth. There is no
 // source recursion/depth/index cap or successful cycle skipping.
 std::vector<std::unique_ptr<AnimationGroupFrame>> frames;size_t depth=0;
};
// Complete dispProc8003c4bc local parent. SDK node/copy, numeric, material,
// Vivian, callback and render children retain their own effects on false.
// Preserve this cursor and identical request/providers until completion.
// Default 128 is a native cooperative-work budget, not a source recursion limit.
bool resumeAnimationGroup(const AnimationInnerTraversal&,const AnimationGroupServices&,AnimationGroupCursor&,unsigned visits=128);
}

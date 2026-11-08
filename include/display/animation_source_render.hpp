#pragma once
#include "battle_act_draw.hpp"
#include <array>
#include <optional>
namespace ttyd {
// Durable references to original local arguments, without invented guest stack RAM.
enum class AnimationRenderKind {Scalar,Guest,Matrix,Vector,Color};
struct AnimationRenderArgument {AnimationRenderKind kind=AnimationRenderKind::Scalar;uint32_t value=0;};
struct AnimationRenderCall {
 uint32_t function=0,count=0;std::array<AnimationRenderArgument,8> arguments{};
};
// Local matrix/vector/color slots preserve source stack identities across holds.
// Slot IDs are not guest pointers; vectors/colors retain raw float/color words.
struct AnimationRenderLocals {
 // InvXpose can return without writing; an unavailable normal stays unavailable.
 std::array<std::optional<BattleActMatrix>,2> matrices;
 std::array<std::array<uint32_t,3>,2> vectors{};std::array<uint32_t,2> colors{};
};
struct AnimationSourceRenderChildren {
 void* user=nullptr;
 // Camera getters return semantic guest tokens; smartTexObj returns its pointer.
 // SDK/material/GX children own any partial effects while returning false.
 bool (*call)(void*,const AnimationRenderCall&,AnimationRenderLocals&,uint32_t& result)=nullptr;
 bool (*fifo)(void*,uint32_t address,uint16_t)=nullptr;
};
struct AnimationSourceRenderServices {
 void* user=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
 AnimationSourceRenderChildren children;
};
// Fixed retained parent state. found flags mean first matching texture kind,
// not successful binding;pending pins one child request. fifoValue pins one
// loaded index through a held FIFO write. No per-vertex heap storage is needed.
struct AnimationSourceRenderCursor {
 bool active=false,pending=false,found1=false,found2=false;
 uint32_t input=0,phase=0,result=0,w=0,d=0,p=0,h=0,b=0,draw=0,drawCount=0;
 uint32_t camera=0,modeling=0,slots=0,defaultSlots=0,mode=0,v=0,t=0,base=0,index=0;
 uint32_t textureTable=0,textures=0,states=0,textureCount=0,textureData=0;
 uint32_t scan=0,scanKind=0,scanReturn=0,object=0,matrix=0,objectValue=0,frame=0;
 uint32_t extra=0,selected=0,attribute=0,selector=0,run=0,runCount=0,runIndex=0;
 uint32_t first=0,count=0,vertex=0,coordinate=0,lane=0,postBeginWork=0,uvFirst=0;
 uint16_t fifoValue=0;
 std::array<uint32_t,3> byteBases{},arrayBases{},offsets{};
 std::array<uint32_t,8> uv{};std::array<uint32_t,4> streams{};
 AnimationSourceRenderServices owner;AnimationRenderLocals locals;AnimationRenderCall request;
};
// Complete renderProc8003c884 local body. Input is only the raw shape index;
// camera/matrices/material/GX/FIFO are explicit retained children, never receipts.
// Default 128 budgets native switch visits,not original iterations or frames.
bool resumeAnimationSourceRender(uint32_t shape,const AnimationSourceRenderServices&,
 AnimationSourceRenderCursor&,unsigned visits=128);
}

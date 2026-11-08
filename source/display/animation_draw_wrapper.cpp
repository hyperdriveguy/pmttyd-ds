#include "animation_draw_wrapper.hpp"
#include <cstring>
namespace ttyd {
namespace {
bool same(const AnimationDrawWrapperInput& a,const AnimationDrawWrapperInput& b){
 return a.handle==b.handle && a.pass==b.pass &&
  !std::memcmp(a.matrix.data(),b.matrix.data(),sizeof(float)*12) &&
  !std::memcmp(&a.angle,&b.angle,sizeof(float)) && !std::memcmp(&a.scale,&b.scale,sizeof(float));
}
}
// Only the original initially-unlinked route is implemented. Phase/frontier
// values retain captured source branches,not a retryable global link query.
// Compare float payload bits (48-byte affine matrix,angle,scale) across holds.
AnimationDrawWrapperFrontier resumeAnimationDrawWrapper(
 const AnimationDrawWrapperInput& input,const AnimationDrawWrapperServices& s,
 AnimationDrawWrapperCursor& c,unsigned visits){
 using F=AnimationDrawWrapperFrontier;
 if(!visits || !s.word)return F::Pending;
 if(!c.active){c={};c.active=true;c.input=input;c.owner=s;}
 if(!same(input,c.input) || s.user!=c.owner.user || s.word!=c.owner.word || s.inner!=c.owner.inner)return F::Pending;
 auto read=[&](uint32_t a,uint32_t& out){auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;};
 auto complete=[&]{c.active=false;return F::Complete;};
 for(unsigned i=0;i<visits;++i)switch(c.phase){
 // Root80415dc0->W,W10->pose pool;stride170hex bytes,wrapping handle.
 // P90=-1 admits this route. Cache stride16,file+8,FileEntry+a0 holder
 // resolves MODEL; the wrapper adds no source ID/active/null/index guard.
 case 0:if(!read(0x80415dc0,c.work))return F::Pending;++c.phase;break;
 case 1:if(!read(c.work+0x10,c.pool))return F::Pending;c.pose=c.pool+c.input.handle*0x170u;++c.phase;break;
 case 2:if(!read(c.pose+0x90,c.link))return F::Pending;c.phase=c.link==0xffffffffu?3:100;break;
 case 3:if(!read(c.pose+0x10,c.index))return F::Pending;++c.phase;break;
 // Source unlinked lookup reuses INITIAL W, not a fresh global load.
 case 4:if(!read(c.work,c.models))return F::Pending;++c.phase;break;
 case 5:if(!read(c.models+(c.index<<4)+8,c.entry))return F::Pending;++c.phase;break;
 case 6:if(!read(c.entry+0xa0,c.holder))return F::Pending;++c.phase;break;
 case 7:if(!read(c.holder,c.model))return F::Pending;c.phase=c.input.pass==2?8:9;break;
 case 8:if(!read(c.model+0xc4,c.flags))return F::Pending;c.phase=c.flags&1?11:9;break;
 // V2 fallback RELOADS flags. PPC slw ignores high26 shift bits, but low6>=32
 // produces0. Thus raw64/-64 shift0, raw32/-1 produce0; no enum/clamp.
 case 9:if(!read(c.model+0xc4,c.flags))return F::Pending;++c.phase;break;
 case 10:{const uint32_t shift=c.input.pass&63u;if(!(c.flags&(shift<32?(1u<<shift):0u)))return complete();++c.phase;break;}
 // Reread link only AFTER admitted pass mask. Rejecting masks complete
 // before this query. A changed final link reaches incoming-GPR30 frontier;
 // restoring memory cannot undo that already captured branch.
 case 11:
  if(!read(c.pose+0x90,c.link))return F::Pending;
  if(c.link!=0xffffffffu){c.phase=101;break;}
  c.request={c.pose,c.input.pass,c.input.matrix,c.input.angle,c.input.scale};++c.phase;break;
 // Inner receives captured POSE POINTER and copied caller matrix,never a
 // fabricated guest stack. Its partial effects belong to its retained owner.
 case 12:if(!s.inner || !s.inner(s.user,c.request))return F::Pending;return complete();
 case 100:return F::LinkedPreparation;
 // Initial unlinked path never initialized r30. Final non--1 dispatch uses
 // caller r30, not a new Q derived from this link. No such context is invented.
 case 101:return F::CallerRegister;
 default:return F::Pending;
 }
 return F::Pending;
}
}

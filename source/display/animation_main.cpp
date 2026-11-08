#include "animation_main.hpp"
namespace ttyd {
namespace {
bool same(const AnimationMainServices& a,const AnimationMainServices& b){
 return a.user==b.user && a.word==b.word && a.gqr==b.gqr && a.linked==b.linked && a.evaluate==b.evaluate &&
 a.facing.user==b.facing.user && a.facing.word==b.facing.word && a.facing.store==b.facing.store && a.facing.call==b.facing.call && a.facing.matrix==b.facing.matrix;
}
}
// Fixed source Main parent: handle is a guest pool index, not a host pointer.
// Field offsets/guest addresses below are hex. Cursor phases retain source
// operations through holds; callback/provider identity includes facing services.
bool advanceAnimationMain(uint32_t handle,const AnimationMainServices& s,AnimationMainCursor& c){
 if(!c.active){c={};c.active=true;c.handle=handle;c.owner=s;}
 else if(c.handle!=handle || !same(c.owner,s))return false;
 auto load=[&](uint32_t a,uint32_t& out){if(!s.word)return false;auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;};
 for(;;)switch(c.phase){
 // Root80415dc0 -> W; W10 pose pool (stride170hex), W0 MODEL cache
 // (16-byte rows,file+8). Resolve P10 resource/P14 animation via actual
 // FileEntry+a0 holder -> MODEL ->1ac directory,64-byte descriptors+3c.
 // No native ID/active/size/null check is attributed to these source reads.
 case 0:if(!load(0x80415dc0,c.captured.work))return false;++c.phase;break;
 case 1:if(!load(c.captured.work+0x10,c.captured.pool))return false;++c.phase;break;
 case 2:if(!load(c.captured.work,c.models))return false;c.captured.pose=c.captured.pool+handle*0x170u;++c.phase;break;
 case 3:if(!load(c.captured.pose+0x10,c.resource))return false;++c.phase;break;
 case 4:if(!load(c.captured.pose+0x14,c.index))return false;++c.phase;break;
 case 5:if(!load(c.models+(c.resource<<4)+8,c.entry))return false;++c.phase;break;
 case 6:if(!load(c.entry+0xa0,c.holder))return false;++c.phase;break;
 case 7:if(!load(c.holder,c.captured.model))return false;++c.phase;break;
 case 8:if(!load(c.captured.model+0x1ac,c.directory))return false;++c.phase;break;
 case 9:if(!load(c.directory+(c.index<<6)+0x3c,c.captured.timeline))return false;++c.phase;break;
 // Decimal SPR914..917 are GQR2..5. Mirrored load/store words00040004,
 // 00050005,00060006,00070007 install unsigned8/unsigned16/signed8/signed16
 // with scale 0. Main does not save/restore earlier GQR values; ARM providers
 // must honor the resulting quantized-load semantics, not merely accept writes.
 case 10:case 11:case 12:case 13:{const uint32_t i=c.phase-10;if(!s.gqr || !s.gqr(s.user,914+i,0x00040004+i*0x00010001))return false;++c.phase;break;}
 // Capture output pointers BEFORE linked recursion: We8 selects shared
 // Wf8/Wfc, otherwise private P4c/P54. Link P90=-1 skips recursion;
 // linked P4 mask2 also skips. The same CAPTURED pool resolves linked flags.
 case 14:if(!load(c.captured.work+0xe8,c.selector))return false;++c.phase;break;
 case 15:if(!load(c.selector?c.captured.work+0xf8:c.captured.pose+0x4c,c.captured.positions))return false;++c.phase;break;
 case 16:if(!load(c.selector?c.captured.work+0xfc:c.captured.pose+0x54,c.captured.normals))return false;++c.phase;break;
 case 17:if(!load(c.captured.pose+0x90,c.linked))return false;c.phase=c.linked==0xffffffff?20:18;break;
 case 18:if(!load(c.captured.pool+c.linked*0x170u+4,c.flags))return false;c.phase=(c.flags&2)?20:19;break;
 case 19:if(!s.linked || !s.linked(s.user,c.linked))return false;++c.phase;break;
 // Null captured timeline exits only AFTER platform setup and linked work.
 // Nonnull evaluation receives original W/pool/pose/MODEL/timeline/outputs;
 // linked mutations never refresh these. Facing follows successful evaluation
 // and holds there must not rerun sampling. No once-per-tick or cycle filter.
 case 20:if(!c.captured.timeline){c={};return true;}++c.phase;break;
 case 21:if(!s.evaluate || !s.evaluate(s.user,c.captured))return false;++c.phase;break;
 case 22:if(!advanceAnimationMainFacing(c.captured.pose,s.facing,c.facing))return false;c={};return true;
 default:return false;
 }
}
}

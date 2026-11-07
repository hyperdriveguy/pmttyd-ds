#include "animation_pose_release.hpp"
namespace ttyd {
namespace {
// Original function identity used for pending diagnostics (not a VM call).
constexpr uint32_t Parent=0x8003b2d0;
// Signed32 >0 predicate over a raw word AFTER wrapping decrement/reload.
bool positive(uint32_t v){return v && v<0x80000000u;}
struct Operations {
 const BattleSequenceEntryServices& s;AnimationReleaseCursor& c;
 bool load(uint32_t a,uint32_t& v){auto n=s.word?s.word(s.user,a):std::nullopt;if(!n)return false;v=*n;return true;}
 bool store(uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);}
 bool call(uint32_t function){
  if(!c.queued){c.call={};c.call.function=function;c.call.args[0]=c.value;c.queued=true;}
  c.pending=function;if(!s.call || !s.call(s.user,c.call))return false;
  c.queued=false;c.pending=Parent;return true;
 }
};
}
bool releaseAnimationPose(int32_t handle,const BattleSequenceEntryServices& s,AnimationReleaseCursor& c){
 if(!c.active){c={};c.active=true;c.handle=uint32_t(handle);c.owner=s;c.pending=Parent;}
 else if(c.handle!=uint32_t(handle) || c.owner.user!=s.user || c.owner.word!=s.word || c.owner.store!=s.store || c.owner.call!=s.call)return false;
 Operations m{c.owner,c};
 for(;;)switch(c.phase){
 // Root80415dc0 -> W; W10 is the 256-row pose array, stride 0x170.
 // Raw handle multiplication wraps; this parent invents no ID/active guard.
 // Pose+4 nonzero chooses borrowed/group refs at+8; nonnegative after
 // decrement returns early. Only a negative count reaches resource teardown.
 case 0:if(!m.load(0x80415dc0,c.work))return false;++c.phase;break;
 case 1:if(!m.load(c.work+0x10,c.value))return false;c.pose=c.value+c.handle*0x170;++c.phase;break;
 case 2:if(!m.load(c.pose+4,c.value))return false;c.phase=c.value?3:7;break;
 // Borrowed/group branch rereads the decremented counter before testing sign.
 case 3:if(!m.load(c.pose+8,c.value))return false;--c.value;++c.phase;break;
 case 4:if(!m.store(c.pose+8,c.value))return false;++c.phase;break;
 case 5:if(!m.load(c.pose+8,c.value))return false;c.phase=c.value<0x80000000u?40:16;break;
 // Ordinary branch reloads flags after the first smartFree child.
 // Ordinary flags 40/80 own smart allocations referenced by P15c/P164.
 // smartFree8002f878 is a real child; unavailable ownership must hold.
 case 7:if(!m.load(c.pose,c.value))return false;c.phase=(c.value&0x40)?8:11;break;
 case 8:if(!m.load(c.pose+0x15c,c.value))return false;++c.phase;break;
 case 9:if(!m.call(0x8002f878))return false;c.phase=11;break;
 case 11:if(!m.load(c.pose,c.value))return false;c.phase=(c.value&0x80)?12:16;break;
 case 12:if(!m.load(c.pose+0x164,c.value))return false;++c.phase;break;
 case 13:if(!m.call(0x8002f878))return false;c.phase=16;break;
 // Both branches share this ordered resource suffix. Pose/model addresses are
 // retained; W and indices reload exactly where the original loads them.
 // P10=model index; W0=model rows 16 bytes: flags0, refs4, file8, texture index c.
 // Clear model flags before its texture dependency when reloaded refs <= 0.
 // W8=texture rows 12 bytes: flags0,refs4,file8. Capture each actual row once.
 case 16:if(!m.load(0x80415dc0,c.work))return false;++c.phase;break;
 case 17:if(!m.load(c.pose+0x10,c.index))return false;++c.phase;break;
 case 18:if(!m.load(c.work,c.value))return false;c.model=c.value+c.index*16;++c.phase;break;
 case 19:if(!m.load(c.model+4,c.value))return false;--c.value;++c.phase;break;
 case 20:if(!m.store(c.model+4,c.value))return false;++c.phase;break;
 case 21:if(!m.load(c.model+4,c.value))return false;c.phase=positive(c.value)?36:22;break;
 case 22:if(!m.store(c.model,0))return false;++c.phase;break;
 case 23:if(!m.load(c.model+0xc,c.index))return false;++c.phase;break;
 case 24:if(!m.load(0x80415dc0,c.work))return false;++c.phase;break;
 case 25:if(!m.load(c.work+8,c.value))return false;c.texture=c.value+c.index*12;++c.phase;break;
 case 26:if(!m.load(c.texture+4,c.value))return false;--c.value;++c.phase;break;
 case 27:if(!m.store(c.texture+4,c.value))return false;++c.phase;break;
 case 28:if(!m.load(c.texture+4,c.value))return false;c.phase=positive(c.value)?33:29;break;
 // fileFree8018a260(texture.file) precedes texture flag clear; model.file
 // is read only afterward. Native file release need not free loaded payloads.
 case 29:if(!m.load(c.texture+8,c.value))return false;++c.phase;break;
 case 30:if(!m.call(0x8018a260))return false;++c.phase;break;
 case 31:if(!m.store(c.texture,0))return false;c.phase=33;break;
 // Read model.file after texture release/flag clear, including child effects.
 case 33:if(!m.load(c.model+8,c.value))return false;++c.phase;break;
 case 34:if(!m.call(0x8018a260))return false;c.phase=36;break;
 // Pose flags clear LAST. Other words/handles remain stale source bytes;
 // complete local Release is not whole-heap collection or GPU retirement.
 case 36:if(!m.store(c.pose,0))return false;c.phase=40;break;
 case 40:c={};return true;
 default:return false;
 }
}
}

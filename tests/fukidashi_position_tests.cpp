#include "fukidashi_position.hpp"
#include "fukidashi_entry.hpp"
#include "event_script_assets.hpp"
#include "battle_stage_display.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Payload=0x1000,Camera=0x2000,Mario=0x3000,Party=0x4000,Npc=0x5000;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float value(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct Fixture {
 EventScriptAssets assets;std::map<uint32_t,uint8_t> bytes;std::vector<uint32_t> writes;
 FukidashiPositionServices s;FukidashiPositionCursor cursor;
 bool cameraReady=true,sineReady=true,cosineReady=true,rotationReady=true,marioReady=true,idReady=true,pointerReady=true,nativeMath=false;
 bool mutateAfterX=false,extra=false;unsigned cameras=0,sines=0,cosines=0,rotations=0,marios=0,ids=0,pointers=0,converts=0;
 uint32_t partyId=7,partyPointer=Party,lastId=0;float sineInput=0,cosineInput=0,angle=0;std::array<float,3> axis{};
 std::array<float,12> matrix{1,2,0,0,0,3,0,0,0,4,1,0};
 uint8_t byte(uint32_t a){auto i=bytes.find(a);if(i!=bytes.end())return i->second;uint8_t n;assert(assets.readByte(a,n));return n;}
 uint32_t word(uint32_t a){uint32_t n=0;for(unsigned i=0;i<4;++i)n=n<<8|byte(a+i);return n;}
 void put(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(n>>(24-8*i));}
 void real(uint32_t a,float f){put(a,bits(f));}
 float real(uint32_t a){return value(word(a));}
 void xyz(uint32_t a,float x,float y,float z){real(a,x);real(a+4,y);real(a+8,z);}
 Fixture(){
  assert(assets.load("romfs/fukidashi-position.pmevt"));
  for(const auto* name:{"romfs/fukidashi-entry.pmevt","romfs/fukidashi-effect.pmevt"}){EventScriptAssets a;assert(a.load(name) && assets.merge(a));}
  for(unsigned i=0;i<88;i+=4)put(Payload+i,0xa5a5a5a5);
  xyz(Payload+4,91,92,93);xyz(Payload+0x10,1,2,3);real(Payload+0x1c,9);real(Payload+0x20,30);put(Payload+0x44,0);
  real(Camera+0x114,10);xyz(Mario+0x8c,10,20,30);xyz(Npc+0x8c,40,50,60);xyz(Party+0x58,70,80,90);
  s.user=this;s.memory.user=this;
  s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
  s.memory.storeWord=[](void* p,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.put(a,n);
   if(f.mutateAfterX && a==Payload+4){f.real(Payload+0x20,4);f.real(Npc+0x90,100);f.real(Payload+0x14,8);}
  };
  s.binary=[](void*,FukidashiPositionBinary op,float a,float b){switch(op){case FukidashiPositionBinary::Add:return a+b;case FukidashiPositionBinary::Subtract:return a-b;case FukidashiPositionBinary::Multiply:return a*b;case FukidashiPositionBinary::Divide:return a/b;}assert(false);return 0.f;};
  s.fused=[](void*,float a,float b,float c){return std::fma(a,b,c);};
  s.biasConvert=[](void* p,uint32_t n,uint64_t rawBias){auto& f=*static_cast<Fixture*>(p);++f.converts;
   assert(rawBias==0x4330000080000000ULL);const uint64_t raw=0x4330000000000000ULL|(n^0x80000000u);double a,b;std::memcpy(&a,&raw,8);std::memcpy(&b,&rawBias,8);return float(a-b);};
  s.camera=[](void* p,int32_t id)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(id==4);++f.cameras;if(!f.cameraReady)return {};return Camera;};
  s.sine=[](void* p,float input)->std::optional<double>{auto& f=*static_cast<Fixture*>(p);++f.sines;f.sineInput=input;
   assert(!f.cursor.reset());assert(!resumeFukidashiPosition(Payload,1,f.s,f.cursor));
   if(!f.sineReady)return {};return f.nativeMath?std::sin(double(input)):0.5;};
  s.cosine=[](void* p,float input)->std::optional<double>{auto& f=*static_cast<Fixture*>(p);++f.cosines;f.cosineInput=input;if(!f.cosineReady)return {};return f.nativeMath?std::cos(double(input)):0.25;};
  s.rotate=[](void* p,const std::array<float,3>& axis,float angle)->std::optional<std::array<float,12>>{auto& f=*static_cast<Fixture*>(p);++f.rotations;f.axis=axis;f.angle=angle;if(!f.rotationReady)return {};
   if(!f.nativeMath)return f.matrix;
   // Existing finite native SDK adapter. This proves composition, not original
   // Gekko frsqrte/libm/FPSCR equivalence or native GPU presentation.
   BattleStageMath r;r.function=0x8029e1c0;r.axis=axis;r.angle=angle;assert(nativeBattleStageMath(r));return r.matrix;};
  s.mario=[](void* p)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.marios;if(!f.marioReady)return {};return Mario;};
  s.partyId=[](void* p,bool extra)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.ids;f.extra=extra;if(!f.idReady)return {};return f.partyId;};
  s.partyPointer=[](void* p,uint32_t id)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.pointers;f.lastId=id;if(!f.pointerReady)return {};return f.partyPointer;};
 }
 bool run(int32_t argument=1){return resumeFukidashiPosition(Payload,argument,s,cursor);}
};
void capturedMathAndLiveTail(){
 Fixture f;f.sineReady=false;assert(!f.run() && f.cursor.pending()==0x80272410);
 const float first=f.sineInput;f.real(Camera+0x114,20);assert(!f.run() && f.sineInput==first && f.cameras==1 && f.writes.empty());
 auto changed=f.s;changed.rotate=nullptr;assert(!resumeFukidashiPosition(Payload,1,changed,f.cursor));assert(f.sines==2);
 assert(!f.run(2) && f.sines==2);
 f.sineReady=true;f.cosineReady=false;assert(!f.run() && f.cursor.pending()==0x80271ea8);
 const auto second=f.cosineInput;assert(second!=first);f.real(Camera+0x114,30);assert(!f.run() && f.cosineInput==second);
 f.cosineReady=true;f.rotationReady=false;assert(!f.run() && f.cursor.pending()==0x8029e1c0);
 assert((f.axis==std::array<float,3>{.5f,0,-.25f}));const auto angle=f.angle;const auto converts=f.converts;
 f.real(Payload+0x1c,40);f.put(Payload+0x44,Npc);assert(!f.run() && f.angle==angle && f.converts==converts && f.writes.empty());
 f.rotationReady=true;assert(f.run() && !f.cursor.active());
 assert(f.real(Payload+4)==133 && f.real(Payload+8)==190 && f.real(Payload+12)==247);
 assert(f.real(Payload+0x28)==40 && f.real(Payload+0x2c)==1.75f && f.converts==2);
 assert((f.writes==std::vector<uint32_t>{Payload+4,Payload+8,Payload+12,Payload+0x28,Payload+0x2c}));
}
void actorBranchesAndLiveComponents(){
 for(uint32_t attachment:{0u,0xffffffffu,Npc,0xfffffffeu,0xfffffffdu}){Fixture f;f.put(Payload+0x44,attachment);assert(f.run());
  const float x=attachment==0?0:attachment==0xffffffffu?10:attachment==Npc?40:70;
  const float y=attachment==0?0:attachment==0xffffffffu?20:attachment==Npc?50:80;
  const float z=attachment==0?0:attachment==0xffffffffu?30:attachment==Npc?60:90;
  assert(f.real(Payload+4)==x+93 && f.real(Payload+8)==y+140 && f.real(Payload+12)==z+187);
  assert(f.real(Payload+0x28)==9 && f.real(Payload+0x2c)==1.75f);
 }
 Fixture held;held.put(Payload+0x44,0xfffffffdu);held.pointerReady=false;
 assert(!held.run() && held.cursor.pending()==0x800bf1b4 && held.ids==1 && held.extra);
 held.partyId=9;held.put(Payload+0x44,0);held.pointerReady=true;assert(held.run() && held.lastId==7 && held.ids==1);
 assert(held.real(Payload+4)==163);
 Fixture missing;missing.put(Payload+0x44,0xfffffffeu);missing.partyPointer=0;assert(missing.run());
 assert(missing.real(Payload+4)==91 && missing.real(Payload+8)==92 && missing.real(Payload+12)==93);
 assert((missing.writes==std::vector<uint32_t>{Payload+0x28,Payload+0x2c}));
 Fixture live;live.put(Payload+0x44,Npc);live.mutateAfterX=true;assert(live.run());
 assert(live.real(Payload+4)==133 && live.real(Payload+8)==168 && live.real(Payload+12)==143 && live.real(Payload+0x2c)==float(.5+float(4.f/12)*.5));
}
void numericBoundaries(){
 Fixture fused;fused.matrix[1]=value(0x3f800008);fused.real(Payload+0x20,value(0xc16fffff));fused.real(Payload+0x10,value(0xbf800010));
 assert(fused.run() && fused.word(Payload+4)==0x2b800000); // FMA residue2^-40; split multiply/add yields0.
 Fixture wrap;wrap.real(Payload+0x1c,20);assert(wrap.run(0x40000000));assert(wrap.real(Payload+0x28)==0); // low32(arg*20)==0.
 Fixture signedWrap;signedWrap.real(Payload+0x1c,20);assert(signedWrap.run(0x20000000));assert(signedWrap.real(Payload+0x28)==-2147483648.f);
 // Do not simplify (angle-20)+20 to angle: the source rounded subtraction
 // loses this tiny input, then the addition cancels to +0; negation is -0.
 Fixture round;round.real(Payload+0x1c,0x1p-30f);assert(round.run());assert(bits(round.angle)==0x80000000 && round.word(Payload+0x28)==0);
}
void actualConstructorComposition(){
 Fixture f;f.nativeMath=true;f.real(Camera+0x114,0);
 constexpr uint32_t Manager=0x6000,GP=0x7000,Pool=0x8000;
 f.put(0x80415ee0,Manager);f.put(0x80415fd0,GP);f.put(Manager,1);f.put(Manager+4,Pool);f.put(GP+0x14,0);
 for(unsigned i=0;i<40;i+=4)f.put(Pool+i,0);
 FukidashiEntryServices entry;entry.user=&f;entry.pool.user=&f;entry.pool.memory=f.s.memory;
 entry.pool.memory.storeByte=[](void* p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->bytes[a]=n;};
 entry.pool.heapAllocate=[](void*,int32_t heap,uint32_t bytes)->std::optional<uint32_t>{assert(heap==3 && bytes==88);return Payload;};
 entry.binary=[](void*,FukidashiBinary op,float a,float b){assert(op==FukidashiBinary::Add);return a+b;};
 entry.position=[](void* p,uint32_t payload,int32_t argument){auto& f=*static_cast<Fixture*>(p);return resumeFukidashiPosition(payload,argument,f.s,f.cursor);};
 FukidashiEntryRequest request;request.position={1,2,3};request.radius=30;request.duration=30;FukidashiEntryCursor cursor;
 assert(resumeFukidashiEntry(request,entry,cursor)==Pool && !f.cursor.active());
 assert(f.real(Payload+4)==1 && std::abs(f.real(Payload+8)-51)<0.00002f && f.real(Payload+12)==3);
 assert(f.real(Payload+0x28)==0 && f.real(Payload+0x2c)==1.75f && f.cameras==1 && f.sines==1 && f.cosines==1 && f.rotations==1);
}
}
int main(){capturedMathAndLiveTail();actorBranchesAndLiveComponents();numericBoundaries();actualConstructorComposition();}

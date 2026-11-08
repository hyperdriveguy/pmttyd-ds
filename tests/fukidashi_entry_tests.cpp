#include "fukidashi_entry.hpp"
#include "effect_deletion.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
namespace {
constexpr uint32_t Manager=0x1000,GP=0x2000,Pool=0x3000,Effect=Pool+40,Payload=0x4000;
constexpr uint32_t Npc=0x5000,Party=0x6000,Mario=0x7000,Poison=0xa5a5a5a5;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
 EventScriptAssets assets;std::map<uint32_t,uint8_t> bytes;
 FukidashiEntryServices s;FukidashiEntryCursor cursor;FukidashiEntryRequest request;
 FukidashiServices mainServices;FukidashiMainCursor main;EffectMainCursor poolMain;
 unsigned allocations=0,positions=0,marios=0,ids=0,pointers=0,sounds=0,frees=0,displays=0;
 bool heapReady=true,positionReady=true,marioReady=true,idReady=true,pointerReady=true,soundReady=true;
 bool mutateRequest=false;uint32_t liveAttachment=Npc,partyId=7,partyPointer=Party,lastId=0,key=0;
 bool extra=false;std::array<float,3> soundPosition{};
 uint8_t byte(uint32_t a){auto i=bytes.find(a);if(i!=bytes.end())return i->second;uint8_t n;assert(assets.readByte(a,n));return n;}
 uint32_t word(uint32_t a){uint32_t n=0;for(unsigned i=0;i<4;++i)n=n<<8|byte(a+i);return n;}
 void put(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(n>>(24-8*i));}
 float real(uint32_t a){auto n=word(a);float f;std::memcpy(&f,&n,4);return f;}
 void xyz(uint32_t a,float x,float y,float z){put(a,bits(x));put(a+4,bits(y));put(a+8,bits(z));}
 Fixture(){
  assert(assets.load("romfs/fukidashi-entry.pmevt"));EventScriptAssets update;
  assert(update.load("romfs/fukidashi-effect.pmevt") && assets.merge(update));
  put(0x80415ee0,Manager);put(0x80415fd0,GP);put(Manager,2);put(Manager+4,Pool);put(Manager+0x14,0);put(GP+0x14,1);put(GP+0x16c,0);
  for(unsigned i=0;i<80;i+=4)put(Pool+i,Poison);put(Pool,2);put(Effect,0);
  for(unsigned i=0;i<88;i+=4)put(Payload+i,Poison);
  xyz(Npc+0x8c,100,200,300);xyz(Party+0x58,10,20,30);xyz(Mario+0x8c,40,50,60);
  request.kind=0;request.variant=11;request.duration=30;request.attachment=Npc;request.position={1,2,3};request.radius=30;request.angle=9;
  s.user=this;s.pool.user=this;s.pool.memory.user=this;
  s.pool.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
  s.pool.memory.storeWord=[](void* p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->put(a,n);};
  s.pool.memory.storeByte=[](void* p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->bytes[a]=n;};
  s.pool.heapAllocate=[](void* p,int32_t heap,uint32_t size)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.allocations;assert(heap==3 && size==88);if(!f.heapReady)return {};return Payload;};
  s.binary=[](void*,FukidashiBinary op,float a,float b){return op==FukidashiBinary::Add?a+b:a-b;};
  // Fixture child tests constructor boundaries; it is not original calc_pos.
  s.position=[](void* p,uint32_t payload,int32_t arg){auto& f=*static_cast<Fixture*>(p);assert(payload==Payload && arg==1);++f.positions;
   assert(!f.cursor.reset());assert(!resumeFukidashiEntry(f.request,f.s,f.cursor));
   if(!f.positionReady)return false;
   f.xyz(Payload+4,900,901,902);f.put(Payload+0x28,bits(19));f.put(Payload+0x2c,bits(20));f.put(Payload+0x44,f.liveAttachment);
   if(f.mutateRequest){f.request.kind=6;f.request.attachment=0xffffffff;f.request.position={77,88,99};}
   return true;};
  s.mario=[](void* p)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.marios;if(!f.marioReady)return {};return Mario;};
  s.partyId=[](void* p,bool extra)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.ids;f.extra=extra;if(!f.idReady)return {};return f.partyId;};
  s.partyPointer=[](void* p,uint32_t id)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.pointers;f.lastId=id;if(!f.pointerReady)return {};return f.partyPointer;};
  s.sound=[](void* p,uint32_t key,const std::array<float,3>& xyz){auto& f=*static_cast<Fixture*>(p);++f.sounds;f.key=key;f.soundPosition=xyz;return f.soundReady;};
 }
 std::optional<uint32_t> enter(){return resumeFukidashiEntry(request,s,cursor);}
};
void allocationAndPositionHolds(){
 Fixture f;f.heapReady=false;assert(!f.enter() && f.cursor.pending()==0x800300f0);
 assert(f.word(Pool)==2 && f.word(Effect)==1 && f.word(Effect+4)==1 && f.word(Effect+8)==1 && f.word(Effect+0x14)==0x802f36b0);
 // Changing request or provider cannot turn a pending invocation into another.
 auto changed=f.request;changed.position[0]=-0.f;assert(!resumeFukidashiEntry(changed,f.s,f.cursor));assert(f.allocations==1);
 auto services=f.s;services.sound=nullptr;assert(!resumeFukidashiEntry(f.request,services,f.cursor));assert(f.allocations==1);
 assert(!f.enter() && f.allocations==2 && f.word(Effect)==1);
 f.heapReady=true;f.positionReady=false;assert(!f.enter() && f.cursor.pending()==0x80133480);
 assert(f.word(Effect+0xc)==Payload && f.word(Effect+0x10)==0x80132ea8);
 assert(f.word(Payload)==0 && f.word(Payload+0x3c)==11 && f.word(Payload+0x30)==30 && f.word(Payload+0x40)==30);
 assert(f.real(Payload+0x10)==1 && f.real(Payload+0x14)==5 && f.real(Payload+0x18)==3);
 assert(f.real(Payload+0x1c)==9 && f.real(Payload+0x20)==30 && f.word(Payload+0x24)==0);
 assert(f.word(Payload+0x34)==0 && f.word(Payload+0x48)==255 && f.word(Payload+0x4c)==4 && f.word(Payload+0x54)==0);
 assert(f.word(Payload+4)==Poison && f.word(Payload+0x28)==Poison && f.word(Payload+0x2c)==Poison && f.word(Payload+0x38)==Poison && f.word(Payload+0x50)==Poison);
 const auto allocations=f.allocations;assert(!f.enter() && f.allocations==allocations && f.positions==2);
 f.positionReady=true;f.soundReady=false;assert(!f.enter() && f.cursor.pending()==0x800d9848);
 assert(f.key==0x802f36bc && (f.soundPosition==std::array<float,3>{100,200,300}));
 f.xyz(Npc+0x8c,7,8,9);f.put(Payload+0x44,0);f.soundReady=true;
 assert(f.enter()==Effect && f.sounds==2 && f.positions==3 && !f.cursor.active());
 assert((f.soundPosition==std::array<float,3>{100,200,300}));
}
void sourceSoundBranches(){
 Fixture question;question.request.kind=1;assert(question.enter()==Effect && question.key==0x802f36d8);
 for(uint32_t attachment:{0u,0xffffffffu}){Fixture f;f.request.attachment=attachment;f.liveAttachment=attachment;assert(f.enter()==Effect && f.sounds==0);assert(f.marios==(attachment==0xffffffffu?1u:0u));}
 for(uint32_t attachment:{0xfffffffeu,0xfffffffdu}){Fixture f;f.request.attachment=attachment;f.liveAttachment=attachment;f.request.kind=1;f.pointerReady=false;
  assert(!f.enter() && f.cursor.pending()==0x800bf1b4 && f.ids==1);f.partyId=19;f.pointerReady=true;
  assert(f.enter()==Effect && f.ids==1 && f.lastId==7 && f.extra==(attachment==0xfffffffdu));
  assert(f.key==94 && (f.soundPosition==std::array<float,3>{11,22,33}));
 }
 Fixture missing;missing.request.attachment=0xfffffffe;missing.liveAttachment=0xfffffffe;missing.partyId=0;missing.partyPointer=0;
 assert(missing.enter()==Effect && missing.lastId==0 && missing.key==93 && (missing.soundPosition==std::array<float,3>{1,2,3}));
 Fixture fresh;fresh.liveAttachment=0xffffffff;fresh.marioReady=false;assert(!fresh.enter() && fresh.cursor.pending()==0x8005e0c0);
 fresh.liveAttachment=0;fresh.put(Payload+0x44,0);fresh.marioReady=true;
 assert(fresh.enter()==Effect && fresh.key==0x802f36bc && (fresh.soundPosition==std::array<float,3>{41,52,63}));
 Fixture captured;captured.mutateRequest=true;assert(captured.enter()==Effect && captured.key==0x802f36bc && captured.real(Payload+0x24)==0);
 Fixture spin;spin.request.kind=6;spin.request.duration=-1;spin.liveAttachment=0;
 assert(spin.enter()==Effect && spin.real(Payload+0x24)==-450 && spin.word(Payload+0x30)==10000 && spin.word(Payload+0x40)==10000 && spin.sounds==0);
}
void realPoolLifetime(){
 Fixture f;f.request.duration=1;f.request.attachment=0;f.liveAttachment=0;assert(f.enter()==Effect);
 f.mainServices.memory=f.s.pool.memory;f.mainServices.user=&f;f.mainServices.binary=f.s.binary;
 f.mainServices.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};
 f.mainServices.depth=[](void*,const std::array<float,3>& xyz)->std::optional<float>{assert((xyz==std::array<float,3>{900,901,902}));return 12;};
 f.mainServices.display=[](void* p,int32_t camera,int32_t mode,uint32_t callback,uint32_t effect,float depth){assert(camera==4 && mode==8 && callback==0x8013174c && effect==Effect && depth==12);++static_cast<Fixture*>(p)->displays;return true;};
 f.mainServices.effectDelete=[](void* p,uint32_t effect){auto& f=*static_cast<Fixture*>(p);EffectDeletionServices d;d.user=p;d.memory=f.s.pool.memory;
  d.freeHeap=[](void* p,int32_t heap,uint32_t payload){assert(heap==3 && payload==Payload);++static_cast<Fixture*>(p)->frees;};hardDeleteEffect(effect,d);return true;};
 f.s.pool.update=[](void* p,uint32_t callback,uint32_t effect){auto& f=*static_cast<Fixture*>(p);assert(callback==0x80132ea8 && effect==Effect);return resumeFukidashiMain(effect,f.mainServices,f.main);};
 assert(resumeEffectMain(f.s.pool,f.poolMain) && f.displays==1 && f.word(Effect)==1);
 assert(resumeEffectMain(f.s.pool,f.poolMain) && f.displays==1 && f.frees==1 && f.word(Effect)==0);
 assert(resumeEffectMain(f.s.pool,f.poolMain) && f.frees==1);
}
}
int main(){allocationAndPositionHolds();sourceSoundBranches();realPoolLifetime();}

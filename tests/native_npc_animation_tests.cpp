#include "native_field_animation.hpp"
#include "field_npc_main_entry_binding.hpp"
#include "field_npc_registry_owner.hpp"
#include "field_party.hpp"
#include "npc_presentation.hpp"
#include "event_script_assets.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> native;
 bool current=true;int token=0;uint64_t pass=91;int32_t handle=0;
 Fixture(){
  assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform p;p.clock=[](void*)->uint32_t{return 162000000;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};
  native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),p);assert(native);
  const auto h=native->pool()->enter(fieldPlayerPoseResources()[0],2,{});assert(h);handle=*h;
  assert(native->pool()->selectName(handle,"M_W_1",true,{}));
  for(unsigned i=0;i<17;++i)assert(native->tick(false));
 }
 EventAddressSpace& memory(){return native->process().vm().memory;}
 BattlePose& pose(){return *native->pool()->pose(handle);}
 uint32_t word(uint32_t a){uint32_t w;assert(memory().readWord(a,w));return w;}
 void store(uint32_t a,uint32_t w){assert(memory().writeWord(a,w));}
 bool begin(){return native->beginNpcPass(&token,pass,{},[this]{return current;});}
 bool call(uint32_t fn,uint32_t selector=0,uint32_t force=0){FieldNpcDrawRequest q;q.function=fn;q.args={uint32_t(handle),selector,force};FieldNpcDrawResponse r;auto s=native->npcAnimationServices();return s.request(s.user,memory(),q,r);}
 void finish(uint32_t fn,uint32_t selector=0,uint32_t force=0){for(unsigned i=0;i<100000;++i)if(call(fn,selector,force))return;assert(false);}
};
void leaseAndSourceHolds(){
 Fixture f;auto& n=*f.native;auto& p=n.process();auto s=n.npcAnimationServices();FieldNpcDrawRequest q;q.function=0x80041170;q.args={uint32_t(f.handle),0,0};FieldNpcDrawResponse r;
 assert(!s.request(s.user,f.memory(),q,r));
 assert(!n.beginNpcPass(nullptr,f.pass,{},[]{return true;}));
 assert(!n.beginNpcPass(&f.token,f.pass,{},{}));f.current=false;assert(!f.begin());f.current=true;assert(f.begin());
 const auto config=p.mainConfiguration();int other=0;
 assert(n.producerPending() && n.npcPassPending() && !f.begin() && !n.tick(false) && !n.release(f.handle));
 assert(!n.pool()->enter(fieldPlayerPoseResources()[0],2,{}) && !n.configurePresentation({}));
 assert(!n.main(uint32_t(f.handle),f.pass,1));
 assert(!n.endNpcPass(&other,f.pass) && !n.endNpcPass(&f.token,f.pass+1));
 EventNativePlatform platform;EventNativeRuntime foreign(0x81000000,platform);
 assert(!s.request(s.user,foreign.memory,q,r));
 // Equal selector force0 returns before the source clock. Changed/forced
 // selection stores its prefix before holding at that same missing clock.
 std::vector<uint8_t> bus;assert(f.memory().unregisterRam(0x800000f8,&bus));
 f.finish(0x80041170,f.pose().word(0x14),0);
 assert(!f.call(0x80041170,1,1) && p.selectionPending() && f.pose().word(0x14)==1);
 const auto selected=f.pose().state;f.current=false;
 assert(!f.call(0x80041170,1,1) && f.pose().state==selected && !n.endNpcPass(&f.token,f.pass));f.current=true;
 assert(!f.call(0x80041170,0,1) && !f.call(0x8003ed2c) && f.pose().state==selected);
 assert(!n.endNpcPass(&f.token,f.pass));
 assert(f.memory().registerRam(0x800000f8,std::move(bus)));f.finish(0x80041170,1,1);
 n.publicationBudget(1);assert(!f.call(0x8003ed2c) && p.mainPending());
 const auto writes=p.quantization().writes();f.current=false;
 assert(!f.call(0x8003ed2c) && p.quantization().writes()==writes);f.current=true;
 assert(!f.call(0x80041170,0,1) && !n.endNpcPass(&f.token,f.pass));
 n.publicationBudget(8192);f.finish(0x8003ed2c);assert(p.quantization().writes()>=writes && p.quantization().writes()>=4);
 assert(n.producerPending() && !p.producerPending() && n.endNpcPass(&f.token,f.pass));
 // Same executor across NPC passes, then Mario can resume its own predicate.
 const auto frames=p.mainOwner()->frameAllocations();++f.pass;assert(f.begin());f.finish(0x8003ed2c);
 assert(p.mainConfiguration()==config && p.mainOwner()->frameAllocations()==frames && n.endNpcPass(&f.token,f.pass));
 MarioPresentation mario;mario.supported=true;mario.poseUploaded=true;mario.selected="M_W_1";mario.poseUploadSerial=9;
 const std::array<int32_t,3> handles{{f.handle,f.handle,f.handle}};
 assert(!n.mario(mario,handles,1) && !f.begin());for(unsigned i=0;i<10000 && n.presentationPending();++i)n.resumeMario(8192);
 assert(!n.producerPending() && n.marioPose() && f.begin() && n.endNpcPass(&f.token,f.pass));
 // A quiescent external reconfiguration invalidates services until readmitted.
 AnimationFacingAddressPolicy addresses;addresses.base=0x81900000;addresses.unmapped=[&f](uint32_t a,uint32_t b){return f.memory().unmappedRange(a,b);};
 assert(p.configureMain({},addresses,[](uint64_t){return true;}));assert(f.begin() && p.mainConfiguration()>config);
 assert(n.endNpcPass(&f.token,f.pass));
}
void retainedDrawBinding(){
 Fixture f;auto& n=*f.native;auto& m=f.memory();
 // Add only missing original literal words, read from the immutable ISO pack.
 // No source startup/work bytes are replaced by this test setup.
 EventScriptAssets source;assert(source.load("romfs/door-data.pmevt"));
 for(uint32_t a=0x8041fc68;a<0x8041fd80;a+=4){uint32_t w;if(!m.readWord(a,w) && source.readWord(a,w)){assert(m.registerRam(a,std::vector<uint8_t>(4)));f.store(a,w);}}
 auto owner=FieldNpcRegistryOwner::cold(0x85000000,n.pool());auto registry=FieldNpcRegistryBinding::bind(owner,m);assert(registry && registry->initialize());
 const auto actor=f.word(0x8041e8c0);assert(owner->ownsFieldActor(actor));
 // Owned raw actor fixture; this does not stand in for source actor creation.
 f.store(actor,0x02000001);f.store(actor+0x104,uint32_t(f.handle));f.store(actor+0x114,0xff7f03ab);
 f.store(actor+0xe0,bits(1));f.store(actor+0xe4,bits(1));f.store(actor+0xe8,bits(1));f.store(actor+0x144,bits(90));
 struct Probe {bool gpu=false;unsigned draw=0,legacyAnimation=0,legacyClock=0;} probe;
 FieldNpcMainEntryNativeServices services;services.user=&probe;services.numbers.divide=nativeFieldNpcClockDivide;services.numbers.convert=nativeFieldNpcClockConvert;
 services.poseClock=[](void* u,BattlePoseClock&){++static_cast<Probe*>(u)->legacyClock;return false;};
 services.movement=[](void*,EventAddressSpace&,const FieldNpcMainMoveRequest& q,FieldNpcMainMoveResponse&){assert(q.function==0x80048d9c);return true;}; // Explicit no-gravity fixture for automatic-pose routing only.
 services.admitMario=[](void*,EventAddressSpace&,uint32_t){return true;};services.animation=n.npcAnimationServices();
 services.drawNumber=[](void*,EventAddressSpace&,FieldNpcMatrixNumber& q){assert(q.function==0x80007328);q.result=npcReviseAngle(q.args[0]);return true;};
 services.drawMatrix=[](void*,EventAddressSpace&,BattleAudienceMatrixRequest& q){BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
 services.draw=[](void* u,EventAddressSpace&,const FieldNpcDrawRequest& q,FieldNpcDrawResponse& r){auto& p=*static_cast<Probe*>(u);
  if(q.function==0x80041170 || q.function==0x8003ed2c){++p.legacyAnimation;return true;}
  if(q.function==0x8003b4d8){if(!p.gpu)return false;++p.draw;return true;}
  if(q.function==0x80007328){r.number=npcReviseAngle(q.scalars[0]);return true;}
  assert(false);return false;
 }; // Explicit GPU receipt fixture, never a production draw implementation.
 auto binding=FieldNpcMainEntryBinding::bind(owner,*registry,n.process().vm(),*const_cast<AnimationPoolMemory*>(n.process().animationMemory()),services);assert(binding);
 assert(f.begin());n.publicationBudget(1);FieldNpcDrawCursor c;
 assert(!binding->advanceDraw(0x8004c7e0,actor,c) && c.active);
 n.publicationBudget(8192);for(unsigned i=0;i<10000 && n.process().mainPending();++i)assert(!binding->advanceDraw(0,actor,c));
 assert(!n.process().mainPending() && !probe.legacyAnimation && !probe.draw);
 const auto writes=n.process().quantization().writes();assert(writes);
 f.store(n.process().animationMemory()->poseAddress(f.handle)+0xf0,0x12345678);
 assert(!binding->advanceDraw(0,actor,c) && n.process().quantization().writes()==writes);
 probe.gpu=true;assert(binding->advanceDraw(0,actor,c) && !c.active && probe.draw==3 && !probe.legacyAnimation);
 assert(f.pose().word(0xf0)==0x12345678 && n.process().quantization().writes()==writes && n.endNpcPass(&f.token,f.pass));
 // Installed animation provider is exclusive even when no pass admits it.
 assert(!binding->advanceDraw(0x8004c7e0,actor,c) && c.active && !probe.legacyAnimation);
 ++f.pass;assert(f.begin());for(unsigned i=0;i<10000 && c.active;++i)binding->advanceDraw(0,actor,c);
 assert(!c.active && probe.draw==6 && !probe.legacyAnimation && n.endNpcPass(&f.token,f.pass));
 // Actual Main entry and movement prefixes, then original idle-pose selection.
 // The legacy clock is deliberately unavailable: installed native selection
 // must receive raw args and retain its source prefix before phase108 commits.
 for(uint32_t a:{0x802c1394u,0x802c1398u,0x802c13d8u,0x802c13dcu}){uint32_t w;assert(source.readWord(a,w));if(!m.readWord(a,w)){assert(source.readWord(a,w) && m.registerRam(a,std::vector<uint8_t>(4)));f.store(a,w);}}
 constexpr uint32_t marioAddress=0x8d000000,descriptor=0x8d001000;
 assert(m.registerRam(0x8041e900,std::vector<uint8_t>(4)) && m.registerRam(marioAddress,std::vector<uint8_t>(0x300)) && m.registerRam(descriptor,std::vector<uint8_t>(64)));
 f.store(0x8041e900,marioAddress);f.store(0x8041e8b8,1);f.store(actor,0x40004403);f.store(actor+0x28,descriptor);f.store(descriptor+0x10,1);
 FieldNpcMainEntryCursor entry;FieldNpcMainMoveCursor move;FieldNpcAutoPoseCursor automatic;
 ++f.pass;assert(f.begin());
 assert(binding->advanceMovement(entry,move)==FieldNpcMoveFrontier::AutomaticPose);
 std::vector<uint8_t> bus;assert(m.unregisterRam(0x800000f8,&bus));
 assert(binding->advanceAutomaticPose(entry,move,automatic)==FieldNpcMoveFrontier::Pending && automatic.phase==42 && n.process().selectionPending());
 assert(f.word(actor+0x108)==0 && f.pose().word(0x14)==1 && !probe.legacyClock);
 assert(m.registerRam(0x800000f8,std::move(bus)));
 assert(binding->advanceAutomaticPose(entry,move,automatic)==FieldNpcMoveFrontier::Detection && f.word(actor+0x108)==1 && !probe.legacyClock && !probe.legacyAnimation);
 // This test stops at the explicitly named Detection frontier, not full Main.
 assert(n.endNpcPass(&f.token,f.pass));

}
}
int main(){leaseAndSourceHolds();retainedDrawBinding();}

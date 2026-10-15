#include "field_npc_setup_binding.hpp"
#include "field_area_event.hpp"
#include "field_map_npc_assets.hpp"
#include "field_battle_information.hpp"
#include "battle_event_callbacks.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include "event_controls.hpp"
#include "game.hpp"
#include "battle_timing_a.hpp"
#include "camera_manager.hpp"
#include "battle_sequence_binding.hpp"
#include <map>
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Inputs=0x87000000,Area=0x8041fa70,Table=0x90524f10,CallerScript=0x80415e18;
struct Fixture {
 std::unique_ptr<Game> game=std::make_unique<Game>();std::unique_ptr<EventNativeRuntime> vm;
 std::shared_ptr<EventScriptAssets> source=std::make_shared<EventScriptAssets>();std::unique_ptr<FieldNpcRegistryBinding> registry;std::unique_ptr<FieldNpcSetupBinding> setup;
 std::unique_ptr<BattleSequenceBinding> sequence;bool marioReady=false;std::map<uint32_t,FieldNpcWanderEventCursor> wander;
 CameraManager cameras;std::map<uint32_t,FieldNpcActorEventCursor> leaves;
 BattleRandom random{0x12345678};unsigned draws=0,created=0,pending=0;bool admitted=true,blocked=false;EventEntry* caller=nullptr;
 Fixture(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.clock=[](void*)->uint32_t{return 243000000;};p.truncate=[](void*,float v){return int32_t(v);};p.user=this;p.call=[](void* p,EventEntry& e,uint32_t fn,int32_t first)->int32_t{auto& f=*static_cast<Fixture*>(p);if(f.setup && fieldNpcWanderEventFunction(fn)){auto result=f.setup->invokeWanderEvent(fn,e,first!=0,f.wander[e.id]);if(!f.wander[e.id].initialized)f.wander.erase(e.id);return result;}if(f.setup && fieldNpcActorEventFunction(fn)){auto result=f.setup->invokeActorEvent(fn,e,first!=0,f.leaves[e.id]);if(result==2)f.leaves.erase(e.id);return result;}++f.pending;return 0;};vm=std::make_unique<EventNativeRuntime>(0x81000000,p);
  assert(source->load("romfs/door-data.pmevt") && vm->memory.importScripts(*source));assert(vm->memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) && vm->memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) && vm->memory.registerRam(0x82000000,std::vector<uint8_t>(0x80000)) && vm->memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) && vm->memory.registerRam(0x88000000,std::vector<uint8_t>(8)));
  const auto m=vm->memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82080000,1,m);assert(eventSdkCreateHeap(start,0x82080000,m)==0);store(0x8041e844,0);vm->gp.refreshRate=60;vm->initializeContexts();
  registry=game->initializeOriginalFieldNpcs(vm->memory,Base);assert(registry);registerPackedBattleArea("gor",0x90540da8,0x90540f84,nativeBattleActorServices(*vm));for(unsigned n=0;n<4;++n)assert(vm->memory.writeByte(0x8100012c+n,n<3?uint8_t("gor"[n]):0));
  for(const char* name:{"c_deale","c_kuri_h","c_joho_b","c_monban","c_binbo","c_bomt_n","c_kuribo_n","c_tyusan","c_roten_g","c_borodo_n","c_f_mura","c_luigi"}){const auto base=std::string("romfs/field-native-")+name;assert(game->originalFieldNpcs->poses().load((base+".pose-directory.bin").c_str(),(base+".pmanim").c_str()));}
  FieldMapNpcAssets tags;assert(tags.load("romfs/field-npc-map-gor_02.bin") && tags.mapTo(vm->memory,Inputs));for(unsigned n=0;n<15;++n)assert(registry->registerMapNpc(*tags.input(n,Inputs,Area),{123,234}));
  setup=FieldNpcSetupBinding::bind(game->originalFieldNpcs,*registry,*vm,source,services());assert(setup);caller=&createRootEvent(vm->schedule,CallerScript,0,0,vm->creationServices());caller->arguments=0x88000000;store(0x88000000,Table);
 }
 ~Fixture(){vm->reinitializeContext();} // Retire referencing entries before actor unmapping.
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}
 void store(uint32_t a,uint32_t v){assert(vm->memory.writeWord(a,v));}
 FieldNpcSetupNativeServices services(){FieldNpcSetupNativeServices s;s.user=this;s.cameras=&cameras;s.mapHeap=sequence.get();s.soundFlags=game->soundFlags.get();
  s.admitMario=[](void* p,EventAddressSpace&,uint32_t mario){return static_cast<Fixture*>(p)->marioReady && mario==0x8c000000;};
  s.wanderRequest=[](void*,EventAddressSpace&,const FieldNpcWanderRequest& q,FieldNpcWanderResponse& out){switch(q.function){
   case 0x80007328:{float r=float(std::fmod(double(q.floats[0]),360.));if(std::isnan(r))r=0;if(r<0){r+=360;if(r>=360)r=0;}out.number=r;return true;}
   case 0x8000722c:out.number=std::hypot(q.floats[2]-q.floats[0],q.floats[3]-q.floats[1]);return true;
   case 0x8000703c:out.number=0;return true; // Explicit finite math fixture; no heading-fidelity claim.
   case 0x800071f0:out.number=0;return true;
   case 0x80272410:out.number=std::sin(double(q.floats[0]));return true;
   case 0x80271ea8:out.number=std::cos(double(q.floats[0]));return true;
   default:return false;
  }};s.clock=[](void*){return BattlePoseClock{777,888};};
  s.admitBattle=[](void* raw,EventAddressSpace& memory,uint32_t a,int32_t index){auto& f=*static_cast<Fixture*>(raw);auto read=[&](uint32_t address){uint32_t v=0;const bool ok=memory.readWord(address,v);assert(ok);return v;};if(!f.admitted || index!=0 || a<Base || (a-Base)%0x340 || a>=Base+36*0x340 || read(0x8030536c)!=0x90540da8)return false;
   for(const auto span:{std::array<uint32_t,2>{0x90540da8,0x44},{0x90540d18,0x18},{0x90540cf8,0x20},{0x90540cc8,0x30},{0x90541bb8,4},{0x80417880,8}})for(unsigned n=0;n<span[1];n+=4){uint32_t v=0;if(!f.source->readWord(span[0]+n,v) || read(span[0]+n)!=v)return false;}
   return true;};
  s.random=[](void* raw,int32_t bound){auto& f=*static_cast<Fixture*>(raw);assert(bound>=0);++f.draws;return f.random.bounded(bound);};return s;
 }
 FieldNpcSetupOperands operands(){return {this,[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);},[](void*,EventEntry&,int32_t raw){return raw;}};}
 FieldNpcSetupEvents events(){return {this,[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);deleteEventId(id,f.vm->creationServices());},[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);return checkEventId(id,f.vm->creationServices())!=0;},[](void* p,uint32_t script,uint8_t priority,uint8_t flags)->EventEntry*{auto& f=*static_cast<Fixture*>(p);if(f.blocked || f.vm->schedule.count>=256)return nullptr;auto& context=f.vm->gp.battleContext?f.vm->battle:f.vm->field;for(int n=0;n<context.capacity;++n)if(!(context.entries[n].flags&1)){++f.created;return &createRootEvent(f.vm->schedule,script,priority,flags,f.vm->creationServices());}return nullptr;}};}
 uint32_t actor(unsigned row){const auto a=registry->lookup(word(Table+0x5c*row));assert(a && *a);return *a;}
 int invoke(bool first,FieldNpcSetupCursor& c){caller->arguments=0x88000000;return setup->invoke(*caller,first,c,operands(),events());}
};
void actualSetup(){
 Fixture f;FieldNpcSetupCursor c;const auto initial=f.random.state;f.admitted=false;const auto actor=f.actor(0),flags=f.word(actor);assert(f.invoke(true,c)==0 && f.created==0 && f.draws==0 && f.word(actor)==flags && f.random.state==initial);
 f.admitted=true;assert(f.invoke(false,c)==0 && f.created==15 && f.draws==255);BattleRandom expected{initial};for(unsigned n=0;n<15;++n){for(unsigned k=0;k<16;++k)expected.bounded(1);expected.bounded(50);}assert(expected.state==f.random.state);
 uint16_t mode=0;assert(f.vm->memory.readHalf(f.game->originalFieldNpcs->fieldWorkAddress(),mode) && mode==1);
 for(unsigned n=0;n<15;++n){const auto a=f.actor(n),row=Table+0x5c*n;assert(f.word(a)&0x10000000);assert(f.word(a+0x124)==f.word(row+0x10));assert(f.word(a+0x248)==0x90540cf8 && f.word(a+0x230)==0x90540870 && f.word(a+0x238)==0);const auto id=f.word(a+0x118);EventEntry* child=nullptr;for(int k=0;k<f.vm->field.capacity;++k)if(f.vm->field.entries[k].id==id)child=&f.vm->field.entries[k];assert(child && child->field170==a && child->rawBytes[0]==0 && !child->waitingParent && child->restartScript==f.word(row+12));const auto* pose=f.game->originalFieldNpcs->poses().poses().pose(int32_t(f.word(a+0x104)));assert(pose && pose->timestamp()==777);}
 assert(f.invoke(false,c)==0 && f.created==15 && f.draws==255); // Live actual independent init roots hold polling.
 const auto chu=f.word(f.actor(9)+0x118),roten=f.word(f.actor(10)+0x118),info=f.word(f.actor(2)+0x118);f.vm->gp.timestamp=17*243000ULL;f.vm->runManager();
 assert(!checkEventId(info,f.vm->creationServices())); // Original DOL npc_init_evt flag leaf genuinely completes.
 assert(!checkEventId(chu,f.vm->creationServices()) && !checkEventId(roten,f.vm->creationServices()) && f.pending && f.invoke(false,c)==0 && f.draws==255);
 // Test lifecycle transition: retire pending init roots without pretending
 // their still-unbound world callbacks executed. Exercise real regular creation.
 for(unsigned n=0;n<15;++n)deleteEventId(f.word(f.actor(n)+0x118),f.vm->creationServices());
 assert(f.invoke(false,c)==2 && f.created==17 && f.draws==255);unsigned regular=0;for(unsigned n=0;n<15;++n){const auto a=f.actor(n);if(!f.word(a+0x124))continue;const auto id=f.word(a+0x11c);for(int k=0;k<f.vm->field.capacity;++k){const auto& e=f.vm->field.entries[k];if(e.id==id){assert(e.field170==a && e.rawBytes[0]==1 && e.flags==0x21 && !e.waitingParent);++regular;}}}assert(regular==2);
 // Install the same real bridge in the actual retained area facade. Execution
 // still waits for original earlier Bero/Mario services, not a skipped parent.
 assert(loadFieldAreaEventAssets("romfs"));auto game=std::make_unique<Game>();FieldAreaEvent area;auto state=std::make_unique<FieldAreaGpState>();
 std::memcpy(state->bytes.data()+0x10c-state->Begin,"gor_02",7);std::memcpy(state->bytes.data()+0x12c-state->Begin,"gor",4);
 FieldMapNpcAssets tags;assert(tags.load("romfs/field-npc-map-gor_02.bin") && area.begin(2,0) && !area.bindNpcSetup(f.services()) && area.prepareMapNpcs(*game,*state,tags,Inputs,Area,0x8100010c,Base));
 for(unsigned n=0;n<15;++n){const auto* pose=f.game->originalFieldNpcs->poses().poses().pose(int32_t(f.word(f.actor(n)+0x104)));assert(pose && game->originalFieldNpcs->poses().install(pose->resource));}
 assert(area.advanceMapNpcs({123,234}) && !area.bindNpcSetup({}) && area.bindNpcSetup(f.services()) && !area.bindNpcSetup(f.services()));

}
void holdsAndDeletion(){
 Fixture f;FieldNpcSetupCursor c;const auto a=f.actor(0),oldFlags=f.word(a),id=f.caller->id;f.store(0x81000014,1);assert(f.invoke(true,c)==0 && c.sourceFirst);f.store(0x81000014,0);f.store(a+0x118,id);assert(f.invoke(false,c)==0 && f.word(a)==oldFlags && f.draws==0 && f.created==0);
 f.store(a+0x118,0);f.blocked=true;assert(f.invoke(false,c)==0 && c.stage==FieldNpcSetupCursor::Stage::InitCreate && f.created==0 && f.draws==0);f.store(a,0x40000001);assert(f.invoke(false,c)==0 && f.word(a)==0x40000001 && !f.created);
 f.store(0x81000014,1);assert(f.invoke(false,c)==0 && !f.created);f.store(0x81000014,0);f.blocked=false;assert(f.invoke(false,c)==0 && f.created==15 && f.draws==255 && f.word(a)==0x50000001);
 // Source constructor scanner reads the operand even on END. Corrupting any
 // immutable instruction lane or requesting an actor RAM address is rejected.
 assert(admitOriginalNpcSetupScript(*f.source,f.vm->memory,CallerScript));const auto original=f.word(CallerScript);f.store(CallerScript,original^1);assert(!admitOriginalNpcSetupScript(*f.source,f.vm->memory,CallerScript));f.store(CallerScript,original);assert(!admitOriginalNpcSetupScript(*f.source,f.vm->memory,Base));
}
void ancestorAdmission(){
 Fixture f;FieldNpcSetupCursor c;const auto a=f.actor(0),before=f.word(a);auto& child=createChildEvent(f.vm->schedule,*f.caller,CallerScript,0,f.vm->creationServices());child.arguments=0x88000000;f.store(a+0x118,f.caller->id);assert(f.setup->invoke(child,true,c,f.operands(),f.events())==0 && f.word(a)==before && f.draws==0 && f.created==0);
}
void actualActorLeaves(){
 Fixture f;const auto a=f.actor(0),other=f.actor(1);auto& e=*f.caller;e.field170=a;
 assert(f.vm->memory.registerRam(0x89000000,std::vector<uint8_t>(256)));
 auto text=[&](uint32_t p,const char* t){do{assert(f.vm->memory.writeByte(p++,uint8_t(*t)));}while(*t++);};
 const auto args=0x89000000u,name=args+0x40;auto invoke=[&](uint32_t fn,std::initializer_list<int32_t> values){unsigned n=0;for(auto v:values)f.store(args+4*n++,uint32_t(v));e.arguments=args;FieldNpcActorEventCursor c;return f.setup->invokeActorEvent(fn,e,true,c);};
 text(name,"me");const auto flags=f.word(a);assert(invoke(0x800500b0,{0,int32_t(name),1})==2 && f.word(a)==(flags&~1u));
 assert(invoke(0x80053158,{int32_t(name),12,-34,56})==2 && f.word(a+0x8c)==0x41400000 && f.word(a+0x90)==0xc2080000);
 assert(invoke(0x80050988,{int32_t(name),-725})==2 && f.word(a+0x144)==0xc4354000);
 assert(invoke(0x80052e98,{int32_t(name),-30000000,-30000001,-30000002})==2);
 const auto ops=f.vm->interpreterServices().operands;assert(getEventFloat(e,-30000000,ops)==12 && getEventFloat(e,-30000001,ops)==-34 && getEventFloat(e,-30000002,ops)==56);
 f.store(a+0x32c,other);text(name,"master");assert(invoke(0x80050988,{int32_t(name),42})==2 && f.word(other+0x144)==0x42280000);
 text(name,"mario");assert(invoke(0x80050988,{int32_t(name),0})==0 && invoke(0x800500b0,{2,int32_t(name),-1})==2);
 text(name,"me");f.cameras.initializeBoot(608,480);assert(invoke(0x8005088c,{int32_t(name),0})==0);f.cameras.camera(4)->yaw=0;f.cameras.camera(4)->viewPublished=true;assert(invoke(0x8005088c,{int32_t(name),0})==2 && f.word(a+0x144)==0xc2b40000);
 // Flag/no-store outputs preserve evtSetFloat's source NaN route; work
 // outputs alone require the finite fctiwz domain.
 f.store(a+0x8c,0x7fc00000);assert(invoke(0x80052e98,{int32_t(name),-90000000,0,0})==2 && (f.vm->field.flags[0]&1));
 const auto yaw=f.word(a+0x144);assert(invoke(0x8005088c,{int32_t(name),INT32_MAX})==0 && f.word(a+0x144)==yaw);
 // UW destination aliases actor Y; the subsequent fresh Y output observes it.
 f.store(a+0x8c,0x41400000);f.store(a+0x90,0x40000000);e.userWork=a+0x90;
 assert(invoke(0x80052e98,{int32_t(name),-190000000,-30000001,-30000002})==0); // Second output's fctiwz domain holds instead of overflow.
 // A real aliased output may invalidate GP. The next output must hold after
 // the first store, then resume exactly once after the real owner is restored.
 f.store(a+0x8c,0x41400000);f.store(a+0x90,0x40000000);e.userWork=0x80415fd0;
 f.store(args,name);f.store(args+4,uint32_t(-190000000));f.store(args+8,uint32_t(-30000001));f.store(args+12,uint32_t(-30000002));e.arguments=args;FieldNpcActorEventCursor held;
 assert(f.setup->invokeActorEvent(0x80052e98,e,true,held)==0 && held.output==1 && f.word(0x80415fd0)!=0x81000000);
 f.store(0x80415fd0,0x81000000);assert(f.setup->invokeActorEvent(0x80052e98,e,false,held)==2 && f.word(0x80415fd0)==0x81000000);
 e.userWork=a+0x90;
 // No arbitrary malformed operand read can assert or publish an invented zero.
 assert(invoke(0x80050988,{-190000000,0})==0);
}

void actualWanderRoots(){
 Fixture f;assert(loadProloguePouchAssets("romfs"));f.game->progress.originalPouch=createProloguePouch();assert(f.game->progress.originalPouch);f.sequence=f.game->bindOriginalBattleSequence(*f.vm,"romfs");assert(f.sequence);
 auto storage=BattleSequenceStorage::cold(f.game->progress.originalPouch,0x8a000000,0x80000,0x8b000000);assert(storage && f.sequence->attachStorage(storage));
 f.setup=FieldNpcSetupBinding::bind(f.game->originalFieldNpcs,*f.registry,*f.vm,f.source,f.services());assert(f.setup);
 FieldNpcSetupCursor setup;assert(f.invoke(true,setup)==0);for(unsigned n=0;n<15;++n)deleteEventId(f.word(f.actor(n)+0x118),f.vm->creationServices());assert(f.invoke(false,setup)==2);
 const auto goomba=f.actor(8),borodo=f.actor(11),id=f.word(goomba+0x11c),guarded=f.word(borodo+0x11c);
 writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,215,f.vm->interpreterServices().operands);
 f.vm->gp.timestamp=17*243000ULL;f.vm->runManager();EventEntry* root=nullptr;for(int n=0;n<f.vm->field.capacity;++n)if(f.vm->field.entries[n].id==id)root=&f.vm->field.entries[n];
 assert(root && !checkEventId(guarded,f.vm->creationServices()));const uint32_t work=uint32_t(root->locals[10]);
 assert(f.sequence->ownsMapAllocation(work,44,f.vm->memory) && f.word(work+4)==0 && f.word(work+0x10)==360 && f.word(work+0x14)==540 && f.word(work+0x18)==90 && f.word(work+0x28)==0x428c0000 && f.word(goomba+0x1b0)==0x41f00000 && (f.word(goomba)&0x400000) && f.draws==255);
 assert(f.wander.at(id).pending && f.wander.at(id).call.function==0x8005e0c0);
 // Explicit packed Mario fixture; production semantic Mario fragments are not
 // silently used as this original root/radial-hint owner.
 assert(f.vm->memory.registerRam(0x8041e900,std::vector<uint8_t>(4)) && f.vm->memory.registerRam(0x8c000000,std::vector<uint8_t>(0x200)));f.store(0x8041e900,0x8c000000);f.marioReady=true;
 f.vm->gp.timestamp+=17*243000ULL;f.vm->runManager();assert(f.word(work+4)==1 && f.draws==257 && uint32_t(root->locals[10])==work);
 const auto xyz=std::array<uint32_t,3>{f.word(goomba+0x8c),f.word(goomba+0x90),f.word(goomba+0x94)};
 f.store(work+8,3);f.store(goomba+0x1a4,0x3c23d70a);f.store(goomba+0x1ac,0);f.store(goomba+0x2fc,0);
 f.vm->gp.timestamp+=17*243000ULL;f.vm->runManager();assert(f.word(work+8)==2 && uint32_t(root->locals[10])==work && f.word(goomba+0x1bc)!=0);
 assert((xyz==std::array<uint32_t,3>{f.word(goomba+0x8c),f.word(goomba+0x90),f.word(goomba+0x94)})); // Main consumes components separately.
 stopEventId(id,f.vm->creationServices());const auto count=f.word(work+8);f.vm->gp.timestamp+=17*243000ULL;f.vm->runManager();assert(f.word(work+8)==count && uint32_t(root->locals[10])==work);
 startEventId(id,f.vm->creationServices());f.vm->gp.timestamp+=17*243000ULL;f.vm->runManager();assert(f.word(work+8)==count-1 && uint32_t(root->locals[10])==work);
 EventNativeRuntime foreign(0x81000000,{});assert(!f.sequence->allocateMap(0x8a000000,44,foreign.memory) && !f.sequence->ownsMapAllocation(work,44,foreign.memory));
 deleteEventId(id,f.vm->creationServices());assert(f.sequence->ownsMapAllocation(work,44,f.vm->memory)); // No invented per-entry map free.
 // Outside the original inclusive guard, the actual Borodo wrapper allocates
 // and initializes a different retained work block through the same map heap.
 writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,262,f.vm->interpreterServices().operands);
 auto& resumed=createRootEvent(f.vm->schedule,f.word(borodo+0x124),0,0x20,f.vm->creationServices());resumed.field170=borodo;resumed.rawBytes[0]=1;
 f.vm->gp.timestamp+=17*243000ULL;f.vm->runManager();const auto second=uint32_t(resumed.locals[10]);assert(second!=work && f.sequence->ownsMapAllocation(second,44,f.vm->memory) && f.word(second+4)==1 && f.word(second+0x28)==0x428c0000 && f.draws==259);
 deleteEventId(resumed.id,f.vm->creationServices());assert(f.sequence->ownsMapAllocation(second,44,f.vm->memory));
 f.sequence.reset(); // Explicitly detach before the VM/facade dies.
}

}
int main(){actualSetup();holdsAndDeletion();ancestorAdmission();actualActorLeaves();actualWanderRoots();}

#include "field_npc_setup_binding.hpp"
#include "field_area_event.hpp"
#include "field_map_npc_assets.hpp"
#include "field_battle_information.hpp"
#include "battle_event_callbacks.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include "game.hpp"
#include "battle_timing_a.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Base=0x85000000,Inputs=0x87000000,Area=0x8041fa70,Table=0x90524f10,CallerScript=0x80415e18;
struct Fixture {
 std::unique_ptr<Game> game=std::make_unique<Game>();std::unique_ptr<EventNativeRuntime> vm;
 std::shared_ptr<EventScriptAssets> source=std::make_shared<EventScriptAssets>();std::unique_ptr<FieldNpcRegistryBinding> registry;std::unique_ptr<FieldNpcSetupBinding> setup;
 BattleRandom random{0x12345678};unsigned draws=0,created=0,pending=0;bool admitted=true,blocked=false;EventEntry* caller=nullptr;
 Fixture(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.clock=[](void*)->uint32_t{return 243000000;};p.truncate=[](void*,float v){return int32_t(v);};p.user=this;p.call=[](void* p,EventEntry&,uint32_t,int32_t)->int32_t{++static_cast<Fixture*>(p)->pending;return 0;};vm=std::make_unique<EventNativeRuntime>(0x81000000,p);
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
 FieldNpcSetupNativeServices services(){FieldNpcSetupNativeServices s;s.user=this;s.clock=[](void*){return BattlePoseClock{777,888};};
  s.admitBattle=[](void* raw,EventAddressSpace& memory,uint32_t a,int32_t index){auto& f=*static_cast<Fixture*>(raw);auto read=[&](uint32_t address){uint32_t v=0;const bool ok=memory.readWord(address,v);assert(ok);return v;};if(!f.admitted || index!=0 || a<Base || (a-Base)%0x340 || a>=Base+36*0x340 || read(0x8030536c)!=0x90540da8)return false;
   for(const auto span:{std::array<uint32_t,2>{0x90540da8,0x44},{0x90540d18,0x18},{0x90540cf8,0x20},{0x90540cc8,0x30},{0x90541bb8,4},{0x80417880,8}})for(unsigned n=0;n<span[1];n+=4){uint32_t v=0;if(!f.source->readWord(span[0]+n,v) || read(span[0]+n)!=v)return false;}
   return true;};
  s.random=[](void* raw,int32_t bound){auto& f=*static_cast<Fixture*>(raw);assert(bound==1 || bound==50);++f.draws;return f.random.bounded(bound);};return s;
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
 const auto chu=f.word(f.actor(9)+0x118),roten=f.word(f.actor(10)+0x118);f.vm->gp.timestamp=17*243000ULL;f.vm->runManager();
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
 for(unsigned n=0;n<15;++n){const auto* pose=f.game->originalFieldNpcs->poses().poses().pose(int32_t(n));assert(pose && game->originalFieldNpcs->poses().install(pose->resource));}
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
}
int main(){actualSetup();holdsAndDeletion();ancestorAdmission();}

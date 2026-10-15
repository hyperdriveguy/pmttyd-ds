#include "field_party_script_call.hpp"
#include "event_native_runtime.hpp"
#include "professor_story.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
struct ProfessorWorld {
 FieldPartyRegistry parties;FieldMarioBattleState mario;
 std::shared_ptr<ProloguePouch> pouch=std::make_shared<ProloguePouch>();
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();std::shared_ptr<NativeFieldAnimation> animation;
 std::optional<int32_t> current=0;EventNativeRuntime story;EventEntry entry;
 FieldPartyScriptBindings binding;static constexpr uint32_t E=0x81818000,A=0x81819000;
 static EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f)->int32_t{assert(std::isfinite(f));return int32_t(f);};return p;}
 explicit ProfessorWorld(const char* root):story(0x81000000,platform()){
  assert(loadFieldPlayerPoseAssets(root));animation=NativeFieldAnimation::create(root,files,platform());assert(animation);
  assert(parties.configureNativeAnimation(animation,fieldPlayerPoseResources()) && parties.initializeMario(mario,{},0));pouch->half(20,10);
  entry.id=31;entry.arguments=A;assert(story.entryAddresses.registerEntries(E,&entry,1));assert(story.memory.registerRam(A,std::vector<uint8_t>(64)));
  // Same guest addresses deliberately contain different operands in the native
  // producer VM. A bridge accidentally reading that VM cannot pass these tests.
  assert(animation->process().vm().entryAddresses.registerEntries(E,&nativeEntry,1));
  nativeEntry.arguments=A;assert(animation->process().vm().memory.registerRam(A,std::vector<uint8_t>(64,0xff)));
  binding.parties=&parties;binding.mario=&mario;binding.currentIndex=&current;binding.pouch=pouch;binding.root=root;
  binding.appearance.display=[](void*){return 0.f;};
 }
 EventEntry nativeEntry;
 void args(std::initializer_list<uint32_t> words){unsigned i=0;entry.arguments=A;for(auto v:words)assert(story.memory.writeWord(A+4*i++,v));}
 int32_t step(NativeFieldPartyScriptCall& c,uint32_t fn,bool first=true){return c.step(entry,first,fn,story,binding,1000);}
 FieldPartyActor* seed(bool auxiliary=false){auto c=NativeFieldPartyEntry::create(binding.root.c_str(),parties,mario,pouch);int32_t result=-1;assert(c && c->position(auxiliary?13:1,auxiliary,{{25,0,625}},{},1000,binding.appearance,result));assert(result==(auxiliary?1:0));mario.partyIndices[auxiliary?1:0]=uint8_t(result);return parties.pointer(result);}
 void manager(bool ready){assert(animation->process().vm().memory.writeWord(NativeFileCache::Global,ready?NativeFileCache::Manager:0));}
};

void actualProfessorConstruction(const char* root){
 ProfessorWorld w(root);std::vector<uint32_t> calls;
 auto ctor=NativeFieldPartyEntry::create(root,w.parties,w.mario,{},[&](uint32_t f,std::initializer_list<int32_t>){calls.push_back(f);});assert(ctor);
 int32_t result=-1;const std::array<float,3> xyz{{-140,0,-225}};
 w.manager(false);assert(!ctor->position(11,true,xyz,{},1000,{},result) && ctor->pending()==0x8004176c);
 auto* actor=w.parties.pointer(1);assert(actor && !w.parties.pointer(0) && actor->bytes[0x31]==11 && w.mario.partyIndices[1]==255);
 w.manager(true);assert(ctor->position(11,true,xyz,{},1000,{},result) && result==1);
 auto* pose=w.parties.pose(int32_t(actor->word(0xc)));assert(pose && pose->sourceResource==0x802cb4bc && pose->original()->resource->modelName=="c_kuri_h" && !(uint32_t(pose->flags)&8));
 assert(actor->word(0)==13 && actor->bytes[0x34]==2 && actor->bytes[0x35]==0 && actor->word(0x18)==0x804213f4 && actor->scalar(0xf0)==24 && actor->scalar(0xf4)==26);
 assert(actor->scalar(0x58)==-140 && actor->word(0x58)==actor->word(0xb8) && actor->word(0x60)==actor->word(0xc0) && actor->scalar(0x70)==1 && w.mario.savedPartyKinds[1]==0);
 for(auto fn:calls)assert(fn!=0x80140ea4 && fn!=0x800c6718 && fn!=0x800bbc48);
 w.mario.partyIndices[1]=1;auto lifetime=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(lifetime && lifetime->kill(1,false) && w.mario.savedPartyKinds[1]==11 && !w.parties.pointer(1));
}
void sharedProfessorModel(const char* root){
 ProfessorWorld w(root);auto ctor=NativeFieldPartyEntry::create(root,w.parties,w.mario,{});assert(ctor);
 auto resource=loadBattlePoseResource((std::string(root)+"/field-native-c_kuri_h.pose-directory.bin").c_str(),(std::string(root)+"/field-native-c_kuri_h.pmanim").c_str());assert(resource);
 auto npc=w.parties.originalPool->enter(resource,2,{},"c_kuri_h");assert(npc);
 int32_t result=-1;assert(ctor->position(11,true,{{1,2,3}},{},0,{},result));auto* actor=w.parties.pointer(1);const auto handle=int32_t(actor->word(0xc));assert(handle!=*npc);
 assert(w.parties.originalPool->pose(handle)->resource==w.parties.originalPool->pose(*npc)->resource);
 w.mario.partyIndices[1]=1;w.mario.savedPartyKinds[1]=77;auto lifetime=NativeFieldPartyLifetime::create(root,w.parties,w.mario);
 assert(lifetime && lifetime->kill(1,true) && w.mario.savedPartyKinds[1]==0 && w.parties.originalPool->pose(*npc) && !w.parties.originalPool->pose(handle));
 assert(w.parties.originalPool->release(*npc));
}
void positionOutputOrder(const char* root){
 ProfessorWorld w(root);w.args({0,uint32_t(-30000000),uint32_t(-29999999),uint32_t(-29999998)});w.current=-1;
 struct Mutation {ProfessorWorld* w;unsigned writes=0;FieldPartyActor* actor=nullptr;} mutation{&w};
 w.story.platform.user=&mutation;w.story.platform.truncate=[](void* p,float value)->int32_t{
  auto& m=*static_cast<Mutation*>(p);
  if(++m.writes==1){if(m.actor){m.actor->scalar(0x5c,99);assert(m.w->story.memory.writeWord(ProfessorWorld::A+8,uint32_t(-29999997)));}
   else assert(m.w->animation->process().vm().memory.writeWord(0x80421d60,0xc57a0000));}
  assert(std::isfinite(value));return int32_t(value);
 };
 NativeFieldPartyScriptCall empty;assert(w.step(empty,0x800eb3f8,false)==2);
 auto operands=w.story.interpreterServices().operands;
 assert(getEventFloat(w.entry,-30000000,operands)==-5000 && getEventFloat(w.entry,-29999999,operands)==-4000 && getEventFloat(w.entry,-29999998,operands)==-4000);
 auto ctor=NativeFieldPartyEntry::create(root,w.parties,w.mario,{});int32_t result=-1;assert(ctor && ctor->position(11,true,{{12.5f,20,30}},{},0,{},result));
 mutation.actor=w.parties.pointer(1);mutation.writes=0;w.entry.locals[1]=42;w.args({7,uint32_t(-30000000),uint32_t(-29999999),uint32_t(-29999998)});
 NativeFieldPartyScriptCall live;assert(w.step(live,0x800eb3f8)==2);
 assert(getEventFloat(w.entry,-30000000,operands)==12.5f && w.entry.locals[1]==42 && getEventFloat(w.entry,-29999997,operands)==99 && getEventFloat(w.entry,-29999998,operands)==30);
 // Inaccessible second destination holds AFTER output0. Retry must not replay it.
 mutation.actor=nullptr;w.story.platform=ProfessorWorld::platform();w.args({1,uint32_t(-30000000)});w.entry.arguments=ProfessorWorld::A+56;
 assert(w.story.memory.writeWord(ProfessorWorld::A+56,1) && w.story.memory.writeWord(ProfessorWorld::A+60,uint32_t(-30000000)));
 NativeFieldPartyScriptCall held;assert(w.step(held,0x800eb3f8)==0 && !w.parties.nativeEntryOwner.expired());
 w.entry.locals[0]=777;assert(w.story.memory.registerRam(ProfessorWorld::A+64,std::vector<uint8_t>(8)));
 assert(w.story.memory.writeWord(ProfessorWorld::A+64,uint32_t(-29999999)) && w.story.memory.writeWord(ProfessorWorld::A+68,uint32_t(-29999998)));
 assert(w.step(held,0x800eb3f8,false)==2 && w.entry.locals[0]==777 && w.parties.nativeEntryOwner.expired());
}
void originalTalkAndFence(const char* root){
 ProfessorWorld w(root);auto* primary=w.seed();assert(primary);assert(loadProfessorStoryAssets(root));ProfessorStory scene;assert(scene.begin(0,{-215,0,-95},{-245,0,-95}));scene.fieldParty=w.binding;w.manager(false);
 for(unsigned i=0;i<30000 && !scene.nativePartyPending();++i)scene.tick(scene.now+17,0x100,false);
 assert(!scene.failed && scene.nativePartyPending() && !scene.temporaryProfessor && scene.messageOrder.size()==29 && w.mario.partyIndices[1]==255);
 auto* actor=w.parties.pointer(1);assert(actor && actor->bytes[0x31]==11);
 w.manager(true);for(unsigned i=0;i<1000 && !scene.finished;++i)scene.tick(scene.now+17,0x100,false);
 assert(scene.finished && scene.story==9 && scene.temporaryProfessor && w.mario.partyIndices[1]==1 && primary->bytes[0x34]==1);
 actor->scalar(0x58,-220);actor->scalar(0x5c,0);actor->scalar(0x60,305);scene.actors[2].position={-900,10,-900};w.mario.savedPartyKinds[1]=77;
 assert(scene.beginFence(scene.now+17,{-215,0,-15},{-245,0,-15}));scene.fieldParty=w.binding;w.manager(false);
 for(unsigned i=0;i<10000 && !scene.nativePartyPending();++i)scene.tick(scene.now+17,0x100,false);
 assert(!scene.failed && scene.temporaryProfessor && scene.messageOrder.empty() && scene.nativePartyPending() && w.mario.partyIndices[1]==255);
 w.manager(true);scene.tick(scene.now+17,0x100,false);
 assert(!scene.failed && !scene.temporaryProfessor && !w.parties.pointer(1) && w.mario.savedPartyKinds[1]==0 && w.parties.pointer(0)==primary);
 assert((scene.actors[2].position==std::array<float,3>{{-220,0,305}}));
 for(unsigned i=0;i<10000 && !scene.captureRequested;++i)scene.tick(scene.now+17,0x100,false);
 assert(!scene.failed && scene.captureRequested && scene.messageOrder.size()==1 && scene.messageOrder[0]=="mac_0_083");
 scene.captureReady=true;scene.captureBounds={100,100,80,60};
 for(unsigned i=0;i<10000 && scene.story==9;++i)scene.tick(scene.now+17,0x100,false);
 assert(!scene.failed && scene.story==10 && scene.fenceCollisionOff && scene.messageOrder.size()==2 && scene.messageOrder[1]=="mac_0_084");
}
}
void runNativeFieldProfessorFixtures(const char* root){actualProfessorConstruction(root);sharedProfessorModel(root);positionOutputOrder(root);originalTalkAndFence(root);}
int main(){runNativeFieldProfessorFixtures("romfs");std::cout<<"native temporary Professor and original fence handoff: PASS\n";}

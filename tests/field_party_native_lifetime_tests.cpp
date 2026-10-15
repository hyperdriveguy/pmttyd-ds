#include "field_party_native_lifetime.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
struct World {
 FieldPartyRegistry parties;FieldMarioBattleState mario;
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();
 std::shared_ptr<NativeFieldAnimation> animation;
 std::unique_ptr<NativeFieldPartyLifetime> lifetime;
 explicit World(const char* root){
  assert(loadFieldPlayerPoseAssets(root));EventNativePlatform platform;
  platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  animation=NativeFieldAnimation::create(root,files,platform);assert(animation);
  assert(parties.configureNativeAnimation(animation,fieldPlayerPoseResources()) && parties.initializeMario(mario,{},0));
  lifetime=NativeFieldPartyLifetime::create(root,parties,mario);assert(lifetime);
  for(const uint32_t a:std::array<uint32_t,3>{{0x80314410,0x80314528,0x803145f0}})assert(word(memory(),a)==(a==0x80314528?0x802cb47cu:0x804213e0u) && word(memory(),a+0x24)==0);
 }
 EventAddressSpace& memory(){return animation->process().vm().memory;}
 int32_t spawn(unsigned slot,uint8_t kind){
  // Explicit actor-memory fixture, not an implementation/claim of Entry2Pos.
  // Raw kind changes are independent of the retained pose in the removal graph;
  // kind8 uses a c_babyyoshi model in a complete source constructor.
  assert(!parties.actors[slot] && parties.nativeRemovalOwner.expired());
  parties.actors[slot]=std::make_unique<FieldPartyActor>();auto& p=*parties.actors[slot];
  p.word(0,kind==13?13:slot?5:1);p.word(0x160,FieldMarioOwnerAddress);
  p.bytes[0x31]=kind;p.bytes[0x34]=1;p.bytes[0x2f]=uint8_t(slot);
  auto handle=parties.enterPose(0x804213e0,2,{},0);assert(handle>=0);
  p.word(0xc,uint32_t(handle));mario.partyIndices[slot]=uint8_t(slot);mario.savedPartyKinds[slot]=77;return handle;
 }
 void marioSurvives(){for(auto handle:mario.poseHandles)assert(parties.pose(handle) && parties.originalPool->pose(handle));}
};
void realSharedFiles(const char* root){
 World w(root);const auto p=w.spawn(0,1),q=w.spawn(1,13);assert(p==3 && q==4);
 auto pool=w.animation->pool();auto* pose=pool->pose(p);auto old=pose->sourceBuffers;
 const auto model=pose->word(0x10),texture=pool->models()[model].texture();
 const auto modelFile=pool->models()[model].file(),textureFile=pool->textures()[texture].file();
 assert(pool->models()[model].references()==2 && pool->textures()[texture].references()==1);
 assert(w.lifetime->kill(0) && !w.lifetime->active() && !w.lifetime->retiring());
 assert(!w.parties.pointer(0) && w.parties.pointer(1) && !w.parties.pose(p) && w.parties.pose(q));
 assert(w.mario.partyIndices[0]==255 && w.mario.partyIndices[1]==1 && w.mario.savedPartyKinds[0]==1 && w.mario.savedPartyKinds[1]==77);
 assert(w.parties.currentDefinition==0x80314410 && !pool->pose(p) && pool->pose(q) && !old->node(*pool->slot(p),0));
 assert(pool->models()[model].references()==1 && half(w.memory(),modelFile+2)==1 && half(w.memory(),textureFile+2)==1);
 assert(w.lifetime->kill(1,true) && !w.parties.pointer(1) && !w.parties.pose(q) && !pool->pose(q));
 assert(w.mario.savedPartyKinds[1]==0 && w.mario.partyIndices[1]==255 && w.parties.currentDefinition==0x803145f0);
 assert(!pool->models()[model].flags() && !pool->textures()[texture].flags() && half(w.memory(),modelFile+2)==0 && half(w.memory(),textureFile+2)==0);
 w.marioSurvives();
}
void fileHoldAndWarmReuse(const char* root){
 World w(root);const auto h=w.spawn(0,1);auto pool=w.animation->pool();auto* pose=pool->pose(h);auto* actor=w.parties.pointer(0);
 auto old=pose->sourceBuffers;const auto generation=pose->nativeGeneration,model=uint64_t(pose->word(0x10));
 const auto texture=pool->models()[unsigned(model)].texture(),file=pool->models()[unsigned(model)].file();
 const auto payload=word(w.memory(),word(w.memory(),file+0xa0)),relocated=word(w.memory(),payload+0x1ac);
 assert(w.memory().writeWord(NativeFileCache::Global,0));
 assert(!w.lifetime->kill(0,true) && w.lifetime->active() && w.lifetime->retiring() && w.lifetime->pending()==0x8003b2d0);
 assert(!w.parties.pointer(0) && !w.parties.pose(h) && w.mario.partyIndices[0]==255 && w.mario.savedPartyKinds[0]==0);
 assert(actor->word(0xc)==uint32_t(h) && actor->bytes[0x31]==1 && pose->sourceBuffers==old && !pool->pose(h));
 assert(w.animation->releasePending()==0x8018a260 && pool->models()[unsigned(model)].references()==0 && pool->textures()[texture].references()==0);
 assert(w.parties.allocateFirst()==-1 && !pool->enter(fieldPlayerPoseResources()[3],2,{}));
 auto other=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(other && !other->kill(0) && !other->removeAll());
 const auto state=pose->state;assert(!w.lifetime->kill(1,true) && !w.lifetime->kill(0,false) && !w.lifetime->removeAll());
 w.mario.savedPartyKinds[0]=88;w.mario.partyIndices[0]=1;w.parties.currentDefinition=0x12345678;
 assert(!w.lifetime->kill(0,true) && pose->state==state && actor->word(0xc)==uint32_t(h));
 assert(w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager));
 assert(w.lifetime->kill(0,true) && !w.lifetime->active() && !w.lifetime->retiring() && w.parties.nativeRemovalOwner.expired());
 assert(w.mario.savedPartyKinds[0]==88 && w.mario.partyIndices[0]==1 && w.parties.currentDefinition==0x12345678 && !pose->sourceBuffers);
 w.mario.partyIndices[0]=255;
 for(unsigned i=0;i<20;++i){
  const auto reused=w.spawn(0,1);assert(reused==h);auto* fresh=pool->pose(reused);
  assert(fresh->nativeGeneration>generation && !old->node(*fresh,0));
  assert(pool->models()[fresh->word(0x10)].file()==file && word(w.memory(),payload+0x1ac)==relocated);
  assert(pool->selectName(reused,"PKR_W_1",true,{}) && w.animation->tick(false) && pool->main(reused,{}));
  assert(w.lifetime->kill(0) && w.mario.savedPartyKinds[0]==1 && half(w.memory(),file+2)==0);
 }
 w.marioSurvives();
}
void borrowedPoseLifetime(const char* root){
 World w(root);const auto h=w.spawn(0,1);auto pool=w.animation->pool();auto* pose=pool->pose(h);auto* view=w.parties.pose(h);auto source=pose->sourceBuffers;
 pose->word(4,1);pose->word(8,1); // Explicit source borrowed/group history.
 assert(w.lifetime->kill(0) && !w.parties.pointer(0) && pool->pose(h)==pose);
 assert(pose->word(8)==0 && pose->sourceBuffers==source && w.parties.pose(h)==view);
 assert(view->original()==pose && w.mario.savedPartyKinds[0]==1);
 // A second actor explicitly retains the same live borrowed handle. Its next
 // source decrement becomes negative and tears down; no invented extra Enter.
 w.parties.actors[0]=std::make_unique<FieldPartyActor>();auto* actor=w.parties.pointer(0);
 actor->word(0x160,FieldMarioOwnerAddress);actor->word(0xc,uint32_t(h));actor->bytes[0x31]=1;w.mario.partyIndices[0]=0;
 assert(w.memory().writeWord(NativeFileCache::Global,0));
 assert(!w.lifetime->kill(0) && w.lifetime->retiring() && !w.parties.pose(h) && !pool->pose(h));
 assert(pose->word(8)==0xffffffff && pose->sourceBuffers==source);
 assert(w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager));
 assert(w.lifetime->kill(0) && !w.parties.pose(h) && !pool->pose(h) && !pose->sourceBuffers);
 w.marioSurvives();
}
void anotherPoolRelease(const char* root){
 World w(root);w.spawn(0,1);auto* actor=w.parties.pointer(0);auto pool=w.animation->pool();
 const auto prefix=std::string(root)+"/trio-enemy-kuribo";
 auto resource=loadBattlePoseResource((prefix+".pose-directory.bin").c_str(),(prefix+".pmanim").c_str());assert(resource);
 const auto other=pool->enter(resource,1,{});assert(other && *other==4);
 assert(w.memory().writeWord(NativeFileCache::Global,0) && !pool->release(*other));
 assert(w.animation->releasePending() && !w.lifetime->kill(0) && !w.lifetime->active());
 assert(w.parties.pointer(0)==actor && w.parties.pose(3) && w.mario.partyIndices[0]==0 && w.mario.savedPartyKinds[0]==77);
 assert(w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager) && pool->release(*other));
 assert(w.lifetime->kill(0) && !w.parties.pointer(0) && !w.parties.pose(3));w.marioSurvives();
}
void originalKindsAndCallbacks(const char* root){
 for(const uint8_t kind:std::array<uint8_t,3>{{1,8,13}}){World w(root);w.spawn(1,kind);assert(w.lifetime->kill(1));assert(w.mario.savedPartyKinds[1]==kind && w.parties.currentDefinition==0x803143e8+uint32_t(kind)*0x28);}
 {World w(root);w.spawn(0,1);assert(w.memory().writeWord(0x80314434,0x80001234));
  assert(!w.lifetime->kill(0) && w.lifetime->pending()==0x80001234 && w.parties.pointer(0) && !w.lifetime->retiring());
  assert(w.mario.partyIndices[0]==0 && w.mario.savedPartyKinds[0]==77 && w.parties.pose(3));
  auto next=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(next && word(w.memory(),0x80314434)==0x80001234 && !next->kill(0));
  // Native adapter disposal expires its exclusion lease without inventing
  // original Kill completion or freeing the still-published actor. The real
  // hook stays unavailable; normal field callers must retain the invocation.
  assert(!w.parties.nativeRemovalOwner.expired());w.lifetime.reset();
  assert(w.parties.nativeRemovalOwner.expired() && w.parties.pointer(0) && w.parties.pose(3));
  assert(w.mario.partyIndices[0]==0 && w.mario.savedPartyKinds[0]==77 && !w.animation->releasePending());
 }
 {World w(root);w.spawn(0,1);assert(w.memory().writeWord(0x80314434,0x80001234));w.parties.pointer(0)->bytes[0x34]=11;
  assert(w.lifetime->kill(0) && !w.parties.pointer(0));}
}
void originalOuterCaller(const char* root){
 World w(root);w.spawn(0,1);w.spawn(1,13);assert(w.memory().writeWord(NativeFileCache::Global,0));
 assert(!w.lifetime->removeAll() && w.lifetime->active() && w.lifetime->retiring());
 assert(!w.parties.pointer(0) && !w.parties.pointer(1) && !w.parties.pose(3) && !w.parties.pose(4));
 assert(w.mario.partyIndices[0]==255 && w.mario.partyIndices[1]==255 && w.mario.savedPartyKinds[0]==1 && w.mario.savedPartyKinds[1]==13);
 assert(w.parties.currentDefinition==0x803145f0 && !w.lifetime->kill(1) && !w.lifetime->removeAll());
 assert(w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager));
 assert(w.lifetime->removeAll() && !w.lifetime->active() && !w.lifetime->retiring());
 assert(w.lifetime->removeAll());w.marioSurvives();
}
void actualEventEntries(const char* root){
 constexpr uint32_t E=0x81801000,A=0x81802000;
 for(const bool clear:{false,true})for(const unsigned slot:std::array<unsigned,2>{{0,1}}){
  World w(root);const auto h=w.spawn(slot,slot?13:1);EventEntry entry;entry.arguments=A;
  assert(w.animation->process().vm().entryAddresses.registerEntries(E,&entry,1));
  assert(w.memory().registerRam(A,std::vector<uint8_t>(4)) && w.memory().writeWord(A,slot));
  int32_t result=99;assert(w.lifetime->event(E,clear,w.memory(),result) && result==2 && !w.parties.pointer(int32_t(slot)) && !w.parties.pose(h));
  assert(w.mario.savedPartyKinds[slot]==(slot && clear?0:slot?13:1));
  assert(w.animation->process().vm().entryAddresses.unregisterEntries(E));
 }
 {World w(root);w.spawn(1,13);EventEntry entry;entry.arguments=A;
  assert(w.animation->process().vm().entryAddresses.registerEntries(E,&entry,1) && w.memory().registerRam(A,std::vector<uint8_t>(4)) && w.memory().writeWord(A,1));
  assert(w.memory().writeWord(NativeFileCache::Global,0));int32_t result=99;
  assert(!w.lifetime->event(E,true,w.memory(),result) && result==99 && w.lifetime->retiring());
  assert(!w.lifetime->kill(1,true) && !w.lifetime->removeAll() && !w.lifetime->event(E,false,w.memory(),result));
  assert(w.memory().writeWord(A,0) && w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager));
  assert(w.lifetime->event(E,true,w.memory(),result) && result==2 && w.mario.savedPartyKinds[1]==0);
  assert(w.animation->process().vm().entryAddresses.unregisterEntries(E));
 }
 {World w(root);EventEntry entry;entry.arguments=A;
  assert(w.animation->process().vm().entryAddresses.registerEntries(E,&entry,1) && w.memory().registerRam(A,std::vector<uint8_t>(4)) && w.memory().writeWord(A,2));
  int32_t result=99;assert(!w.lifetime->event(E,true,w.memory(),result) && result==99 && !w.lifetime->active());
  assert(w.memory().writeWord(A,0) && w.lifetime->event(E,true,w.memory(),result) && result==2);
  assert(w.animation->process().vm().entryAddresses.unregisterEntries(E));
 }
}
void admissionAndDetachedCopies(const char* root){
 World w(root);assert(w.lifetime->kill(-1) && w.lifetime->kill(0) && !w.lifetime->kill(2) && !w.lifetime->active());
 w.spawn(0,1);auto copy=w.parties;assert(copy.nativeRemovalOwner.expired() && !copy.nativeAnimation && copy.pointer(0));
 auto detached=NativeFieldPartyLifetime::create(root,copy,w.mario);assert(detached && !detached->kill(0) && !detached->active() && copy.pointer(0));
 assert(w.lifetime->kill(0) && !w.parties.pointer(0) && copy.pointer(0));
 FieldPartyRegistry empty;FieldMarioBattleState mario;auto plain=NativeFieldPartyLifetime::create(root,empty,mario);assert(plain && plain->removeAll());
}
}
void runNativeFieldPartyLifetimeFixtures(const char* root){realSharedFiles(root);fileHoldAndWarmReuse(root);borrowedPoseLifetime(root);anotherPoolRelease(root);originalKindsAndCallbacks(root);originalOuterCaller(root);actualEventEntries(root);admissionAndDetachedCopies(root);}
int main(){runNativeFieldPartyLifetimeFixtures("romfs");std::cout<<"native field party lifetime passed\n";}

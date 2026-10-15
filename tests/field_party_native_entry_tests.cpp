#include "field_party_native_entry.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace ttyd;
namespace {
struct Call {uint32_t fn;std::vector<int32_t> args;};
struct World {
 FieldPartyRegistry parties;FieldMarioBattleState mario;std::shared_ptr<ProloguePouch> pouch=std::make_shared<ProloguePouch>();
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();std::shared_ptr<NativeFieldAnimation> animation;
 std::unique_ptr<NativeFieldPartyEntry> entry;std::vector<Call> calls;std::function<void(uint32_t)> onCall;
 FieldPartyAppearanceServices appearance;unsigned roots=0,displays=0,rays=0;float facing=60;FieldWallVector position{100,20,300};
 int32_t result=99;const char* root;
 explicit World(const char* r):root(r){
  assert(loadFieldPlayerPoseAssets(root));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  animation=NativeFieldAnimation::create(root,files,platform);assert(animation);
  assert(parties.configureNativeAnimation(animation,fieldPlayerPoseResources()) && parties.initializeMario(mario,{},0));
  pouch->half(14,1);pouch->half(20,10);
  for(unsigned i=0;i<3;++i){parties.pose(mario.poseHandles[i])->materialFlags=0x100+i;parties.pose(mario.poseHandles[i])->materialLightFlags=0x40+i;}
  entry=NativeFieldPartyEntry::create(root,parties,mario,pouch,[this](uint32_t f,std::initializer_list<int32_t> a){calls.push_back({f,{a}});if(onCall)onCall(f);});assert(entry);
  appearance.user=this;
  appearance.root=[](void* p){auto& w=*static_cast<World*>(p);++w.roots;return w.position;};
  appearance.display=[](void* p){auto& w=*static_cast<World*>(p);++w.displays;return w.facing;};
  appearance.auxiliary=[](void*)->int32_t{return -1;};appearance.revise=[](void*,float f){return f;};
  appearance.heading=[](void*,float d,float o,bool second){return fieldPartyHeading(d,0,0,0,o,second);};
  appearance.sincos=[](void*,float f){return fieldPartySinCos(f);};appearance.move=[](void*,FieldWallVector& v,float d,float a){fieldPartyMove(v,d,a);};
  appearance.ray=[](void* p,FieldWallVector,FieldWallVector d,float){++static_cast<World*>(p)->rays;return FieldPartyAppearanceHit{d.y==-1,20};};
 }
 EventAddressSpace& memory(){return animation->process().vm().memory;}
 bool step(){return entry->step(1,{},1000,appearance,result);}
 unsigned count(uint32_t f) const{unsigned n=0;for(auto& c:calls)n+=c.fn==f;return n;}
 void done(){
  assert(result==0 && entry->complete() && entry->pending()==0 && mario.partyIndices[0]==0 && mario.savedPartyKinds[0]==1);
  auto* p=parties.pointer(0);assert(p && !parties.pointer(1) && p->bytes[0x34]==1 && p->bytes[0x38]==10);
  assert(p->word(0x70)==0x3f800000 && p->word(0xf4)==0x41d00000 && p->word(0xb8)==p->word(0x58) && p->word(0xbc)==p->word(0x5c) && p->word(0xc0)==p->word(0x60));
  assert(count(0x801412fc)==1 && count(0x800bea68)==1 && count(0x800be144)==1 && count(0x8004176c)==1 && count(0x800c6718)==2 && count(0x80140ea4)==3);
  assert(parties.nativeEntryOwner.expired() && parties.nativeRemovalOwner.expired());
  const auto n=calls.size();assert(step() && calls.size()==n);
 }
};
void normalAndLiveHp(const char* root){
 World w(root);unsigned placements=0;
 w.onCall=[&](uint32_t f){if(f==0x800c6718){++placements;if(placements==1)assert(w.mario.partyIndices[0]==255);else {assert(w.mario.partyIndices[0]==0);w.pouch->half(20,5);w.position={500,20,600};}}};
 assert(w.step());w.done();auto* actor=w.parties.pointer(0);auto* p=w.parties.pose(int32_t(actor->word(0xc)));
 assert(p && p->original() && p->original()->sourceBuffers && uint32_t(p->materialFlags)==0x100 && uint32_t(p->materialLightFlags)==0x40);
 assert(actor->word(0x18)==0x802cb52c && w.rays==4 && actor->scalar(0x58)>450);
 assert(w.count(0x8026d464)==3); // Main repeat, first appearance, changed HP at Hello.
}
void heldEntryAndExclusion(const char* root){
 World w(root);assert(w.memory().writeWord(NativeFileCache::Global,0));
 assert(!w.step() && w.entry->pending()==0x8004176c && w.result==99);
 auto* actor=w.parties.pointer(0);assert(actor && !w.parties.pointer(1) && actor->word(0xc)==0xffffffff && w.mario.partyIndices[0]==255 && w.mario.savedPartyKinds[0]==1);
 auto other=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(other && !other->kill(0) && !other->removeAll());
 assert(w.parties.allocateFirst()==-1);
 for(unsigned i=0;i<10;++i)assert(!w.step() && w.parties.pointer(0)==actor && w.count(0x8004176c)==1);
 assert(w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager));assert(w.step());w.done();
}
void heldMaterialAndCapturedOwner(const char* root){
 World w(root);std::unique_ptr<FieldPartyPose> owner;int32_t original=w.mario.poseHandles[0];bool captured=false;
 w.onCall=[&](uint32_t f){
  if(f==0x8004069c && !captured){captured=true;owner=std::move(w.parties.poses[unsigned(original)]);w.mario.poseHandles[0]=w.mario.poseHandles[1];w.parties.pointer(0)->word(0x160,0x12345678);}
  if(f==0x800406f0)w.parties.pointer(0)->word(0x160,FieldMarioOwnerAddress);
 };
 assert(!w.step() && w.entry->pending()==0x8004069c);auto* actor=w.parties.pointer(0);
 for(unsigned i=0;i<5;++i)assert(!w.step() && w.parties.pointer(0)==actor && w.count(0x8004069c)==1);
 w.parties.poses[unsigned(original)]=std::move(owner);assert(w.step());w.done();
 const auto* pose=w.parties.pose(int32_t(actor->word(0xc)));assert(uint32_t(pose->materialFlags)==0x100 && uint32_t(pose->materialLightFlags)==0x41);
}
void heldSetterRetainsMask(const char* root){
 World w(root);std::unique_ptr<FieldPartyPose> target;int32_t handle=-1;bool held=false;
 w.onCall=[&](uint32_t f){if(f==0x80040730 && !held){held=true;handle=int32_t(w.parties.pointer(0)->word(0xc));target=std::move(w.parties.poses[unsigned(handle)]);w.parties.pose(w.mario.poseHandles[0])->materialFlags=0x1234;}};
 assert(!w.step() && w.entry->pending()==0x80040730 && w.count(0x8004069c)==1);
 for(unsigned i=0;i<5;++i)assert(!w.step());
 w.parties.poses[unsigned(handle)]=std::move(target);assert(w.step());w.done();assert(uint32_t(w.parties.pose(handle)->materialFlags)==0x100 && w.count(0x80040730)==1);
}
void heldPlacementAndString(const char* root){
 World w(root);const auto ready=w.appearance;w.appearance.ray=nullptr;
 assert(!w.step() && w.entry->pending()==0x800c6718 && w.roots==0 && w.displays==2);
 auto* p=w.parties.pointer(0);const auto handle=p->word(0xc);assert(p->bytes[0x33]==1 && p->word(0x70)==0x3f800000 && w.mario.partyIndices[0]==255);
 assert(!w.step() && w.parties.pointer(0)==p && p->word(0xc)==handle && w.displays==2);
 p->word(0x18,0x81805000);w.appearance=ready;
 assert(!w.step() && w.entry->pending()==0x8026d464 && w.roots>0 && w.mario.partyIndices[0]==255);
 const auto roots=w.roots,rays=w.rays;w.position={900,20,900};w.pouch->half(20,1);
 for(unsigned i=0;i<5;++i)assert(!w.step() && w.roots==roots && w.rays==rays);
 assert(w.memory().registerRam(0x81805000,{'P','K','R','_','S','_','1',0}));assert(w.step());w.done();
 assert(p->word(0x18)==0x802cb52c && w.count(0x800c6718)==2); // HP change belongs to fresh Hello, not held first selector.
}
void existingReplacement(const char* root){
 World w(root);const auto slot=w.parties.allocateFirst();assert(slot==0);w.parties.initializeBeforePose(slot,1,w.mario);
 auto oldHandle=w.parties.enterPose(0x804213e0,2,{},0);assert(oldHandle==3);w.parties.pointer(0)->word(0xc,uint32_t(oldHandle));w.mario.partyIndices[0]=0;
 auto old=w.parties.originalPool->pose(oldHandle)->sourceBuffers;
 assert(w.memory().writeWord(NativeFileCache::Global,0));assert(!w.step() && w.entry->pending()==0x8003b2d0);
 assert(!w.parties.pointer(0) && !w.parties.pointer(1) && !w.parties.pose(oldHandle) && w.count(0x800bea68)==0);
 w.mario.partyIndices[0]=1;for(unsigned i=0;i<5;++i)assert(!w.step() && w.count(0x800be69c)==1);
 assert(w.memory().writeWord(NativeFileCache::Global,NativeFileCache::Manager));assert(w.step());w.done();
 auto* fresh=w.parties.originalPool->pose(int32_t(w.parties.pointer(0)->word(0xc)));assert(fresh && fresh->sourceBuffers!=old && w.count(0x800be69c)==1);
}
void retainedDefinitionAndSignedHp(const char* root){
 for(const int16_t hp:std::array<int16_t,4>{{-1,-32768,5,6}}){World w(root);w.pouch->half(20,hp);assert(w.step());w.done();assert(w.parties.pointer(0)->word(0x18)==(hp<=5?0x802cb52cu:0x802cb4dcu));}
 World w(root);bool redirected=false;
 w.onCall=[&](uint32_t f){if(f==0x8026d464 && !redirected){redirected=true;w.parties.currentDefinition=0x81806000;}};
 assert(!w.step() && w.entry->pending()==0x800be144);auto* actor=w.parties.pointer(0);assert(actor->word(0x1c)==0);
 actor->word(0x1c,0x12345678);w.parties.currentDefinition=0x80314410;
 assert(!w.step() && actor->word(0x1c)==0x12345678);
 assert(w.memory().registerRam(0x81806004,{0x80,0x0c,0x4f,0xa0}));assert(w.step());w.done();assert(actor->word(0x1c)==0x12345678);
}
void originalGatesAndCallback(const char* root){
 {World w(root);w.pouch->half(14,0);w.mario.savedPartyKinds[0]=77;assert(w.step() && w.result==-1 && !w.parties.pointer(0) && w.mario.savedPartyKinds[0]==77 && w.count(0x800bea68)==0);}
 {World w(root);assert(w.memory().writeWord(0x80314414,0x80001234));assert(!w.step() && w.entry->pending()==0x80001234);
  assert(w.memory().writeWord(0x80314414,0x800c4fa0));for(unsigned i=0;i<3;++i)assert(!w.step() && w.entry->pending()==0x80001234 && w.count(0x80001234)==1);assert(w.mario.partyIndices[0]==255);}
 {World w(root);unsigned comparisons=0;w.onCall=[&](uint32_t f){if(f==0x8026d464 && ++comparisons==1)w.parties.pointer(0)->word(4,0x8000);};
  assert(!w.step() && w.entry->pending()==0x800be144 && comparisons==1); // Main second comparison ran despite fresh8000; ordinary placement refuses dot domain.
  assert(w.parties.pointer(0)->bytes[0x33]==1 && w.mario.partyIndices[0]==255);}
}
}
void runNativeFieldPartyEntryFixtures(const char* root){normalAndLiveHp(root);heldEntryAndExclusion(root);heldMaterialAndCapturedOwner(root);heldSetterRetainsMask(root);heldPlacementAndString(root);existingReplacement(root);retainedDefinitionAndSignedHp(root);originalGatesAndCallback(root);}
int main(){runNativeFieldPartyEntryFixtures("romfs");std::cout<<"retained native Goombella entry: PASS\n";}

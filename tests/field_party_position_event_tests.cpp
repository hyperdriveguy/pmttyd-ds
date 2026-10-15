#include "field_party_position_event.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace ttyd;
namespace {
struct PositionWorld {
 FieldPartyRegistry parties;FieldMarioBattleState mario;std::shared_ptr<ProloguePouch> pouch=std::make_shared<ProloguePouch>();
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();std::shared_ptr<NativeFieldAnimation> animation;
 std::vector<uint32_t> calls;std::function<void(uint32_t)> onCall;FieldPartyAppearanceServices appearance;
 EventEntry event;std::unique_ptr<NativeFieldPartyPositionEvent> binding;const char* root;unsigned displays=0;
 static constexpr uint32_t E=0x81818000,A=0x81819000;
 explicit PositionWorld(const char* r):root(r){
  assert(loadFieldPlayerPoseAssets(root));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  platform.truncate=[](void*,float f)->int32_t{assert(std::isfinite(f));return int32_t(f);};
  animation=NativeFieldAnimation::create(root,files,platform);assert(animation);
  assert(parties.configureNativeAnimation(animation,fieldPlayerPoseResources()) && parties.initializeMario(mario,{},0));pouch->half(20,10);
  for(unsigned i=0;i<3;++i){parties.pose(mario.poseHandles[i])->materialFlags=0x100+i;parties.pose(mario.poseHandles[i])->materialLightFlags=0x40+i;}
  event.arguments=A;assert(vm().entryAddresses.registerEntries(E,&event,1) && vm().memory.registerRam(A,std::vector<uint8_t>(64)));
  appearance.user=this;appearance.display=[](void* p){auto& w=*static_cast<PositionWorld*>(p);return float(++w.displays)*30;};
 }
 EventNativeRuntime& vm(){return animation->process().vm();}
 NativeFieldPartyEntry::Trace trace(){return [this](uint32_t fn,std::initializer_list<int32_t>){calls.push_back(fn);if(onCall)onCall(fn);};}
 unsigned count(uint32_t fn) const{unsigned n=0;for(auto f:calls)n+=f==fn;return n;}
 std::unique_ptr<NativeFieldPartyEntry> ctor(bool withPouch=true){return NativeFieldPartyEntry::create(root,parties,mario,withPouch?pouch:std::shared_ptr<ProloguePouch>{},trace());}
 void args(std::initializer_list<uint32_t> words){unsigned i=0;for(auto v:words)assert(vm().memory.writeWord(A+4*i++,v));}
 void originalArgs(uint32_t site){
  // Read actual authored REL operand bytes, not a rewritten test event. Native
  // probe privately stages the same hash-audited source REL; production does not.
  const auto path=std::string(root)=="romfs"?"local-assets/door-data/gor.rel":std::string(root)+"/field-party-position-gor.rel";
  std::ifstream file(path,std::ios::binary);assert(file);
  auto read=[&](){uint8_t b[4];file.read(reinterpret_cast<char*>(b),4);assert(file);return uint32_t(b[0])<<24|uint32_t(b[1])<<16|uint32_t(b[2])<<8|b[3];};
  file.seekg(site);const uint32_t command=read(),function=read();(void)function;assert((command&0xffff)==91 && command>>16>=2);
  for(unsigned i=0;i<(command>>16)-1;++i)assert(vm().memory.writeWord(A+4*i,read()));
 }
 int32_t step(bool first=true,bool previous=false){
  if(!binding)binding=std::make_unique<NativeFieldPartyPositionEvent>(root,parties,mario,pouch,trace());
  return binding->step(event,first,previous,vm().memory,vm().interpreterServices().operands,{},1000,appearance);
 }
};
void directAuxiliary(const char* root){
 PositionWorld w(root);w.mario.flags=0x80000000;w.mario.savedPartyKinds={{77,88}};auto entry=w.ctor(false);assert(entry);
 const std::array<float,3> xyz{{1.25f,-0.f,-300.75f}};int32_t result=99;
 assert(entry->position(13,true,xyz,{},1000,{},result) && result==1 && entry->complete());
 auto* p=w.parties.pointer(1);assert(p && !w.parties.pointer(0) && p->word(0)==13 && p->word(4)==0 && p->word(8)==0x2008 && p->bytes[0x31]==13 && p->bytes[0x34]==2 && p->bytes[0x35]==0);
 assert(p->word(0x18)==0x802cb4dc && p->scalar(0x58)==xyz[0] && p->word(0x5c)==0x80000000 && p->scalar(0x60)==xyz[2]);
 assert(p->word(0x58)==p->word(0xb8) && p->word(0x5c)==p->word(0xbc) && p->word(0x60)==p->word(0xc0) && p->word(0x70)==0x3f800000 && p->word(0x78)==0x3f800000);
 assert(w.mario.partyIndices[0]==255 && w.mario.partyIndices[1]==255 && w.mario.savedPartyKinds[0]==77 && w.mario.savedPartyKinds[1]==88 && w.pouch->half(14)==0);
 assert(w.count(0x80140ea4)==0 && w.count(0x800c6718)==0 && w.count(0x800bbc48)==0 && w.count(0x800bcb2c)==1 && w.count(0x800bcb98)==1 && w.displays==0);
 const auto* pose=w.parties.pose(int32_t(p->word(0xc)));assert(pose && pose->original() && uint32_t(pose->materialFlags)==0x100 && uint32_t(pose->materialLightFlags)==0x40);
 auto second=w.ctor(false);auto* same=p;assert(second->position(13,true,xyz,{},1000,{},result) && result==-1 && w.parties.pointer(1)==same && !w.parties.pointer(0));
}
void ordinaryPosition(const char* root){
 PositionWorld w(root);auto entry=w.ctor();int32_t result=99;const std::array<float,3> xyz{{25,0,625}};
 assert(entry->position(1,false,xyz,{},1000,w.appearance,result) && result==0);
 auto* p=w.parties.pointer(0);assert(p && p->bytes[0x34]==0 && p->bytes[0x38]==0 && p->scalar(0x58)==25 && p->scalar(0x60)==625 && p->word(0xb8)==0 && p->word(0xc0)==0);
 assert(p->scalar(0x110)==30 && p->scalar(0x10c)==60 && w.displays==2 && w.count(0x80140ea4)==2 && w.count(0x800c6718)==0 && w.count(0x800bbc48)==0);
 assert(w.mario.partyIndices[0]==255 && w.mario.savedPartyKinds[0]==1 && w.pouch->half(14)==0);
 const auto before=w.calls.size();assert(entry->position(1,false,xyz,{},1000,w.appearance,result) && w.calls.size()==before);
 assert(!entry->position(1,false,{{26,0,625}},{},1000,w.appearance,result) && w.parties.pointer(0)==p);
}
void authoredHarborAndPlaza(const char* root){
 PositionWorld w(root);w.originalArgs(0x275b4);w.event.locals[0]=-230000000+int32_t(12.75f*1024);w.event.locals[1]=-230000000+int32_t(-2.75f*1024);w.event.locals[2]=-230000000+int32_t(301.5f*1024);
 assert(w.vm().memory.writeWord(NativeFileCache::Global,0));assert(w.step()==0 && w.binding->pending()==0x8004176c);
 auto* p=w.parties.pointer(1);assert(p && !w.parties.pointer(0) && w.mario.partyIndices[1]==255 && w.count(0x80031260)==4);
 // Source arguments/locals change after capture. Native retry must complete the
 // original first=true invocation, even though the VM now passes first=false.
 w.args({0,1,900,900,900});w.event.locals.fill(0);w.event.arguments=PositionWorld::A+32;
 w.event.id=88;assert(w.step(false)==0 && w.binding->pending()==0x800e3120 && w.parties.pointer(1)==p);w.event.id=0;
 for(unsigned i=0;i<3;++i)assert(w.step(false)==0 && w.parties.pointer(1)==p && w.count(0x80031260)==4);
 assert(w.vm().memory.writeWord(NativeFileCache::Global,NativeFileCache::Manager));assert(w.step(false)==2 && w.binding->complete() && w.mario.partyIndices[1]==1);
 assert(p->scalar(0x58)==12 && p->scalar(0x5c)==-2 && p->scalar(0x60)==301 && p->bytes[0x31]==13);
 auto lifetime=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(lifetime);w.event.arguments=PositionWorld::A;w.args({1});int32_t removed=99;
 assert(lifetime->event(PositionWorld::E,true,w.vm().memory,removed) && removed==2 && !w.parties.pointer(1) && w.mario.partyIndices[1]==255 && w.mario.savedPartyKinds[1]==0);
 // Plaza original set(0,1,25,0,625) is separate from the preceding source pouch
 // join. The constructor itself never changes the canonical join bit.
 w.binding.reset();w.originalArgs(0x374bc);assert(w.step()==2 && w.mario.partyIndices[0]==0 && w.pouch->half(14)==0);
 p=w.parties.pointer(0);assert(p && !w.parties.pointer(1) && p->bytes[0x31]==1 && p->scalar(0x58)==25 && p->scalar(0x60)==625 && p->bytes[0x34]==0);
}
void originalEventGatesAndSavedKind(const char* root){
 {PositionWorld w(root);w.args({1});assert(w.step(false)==2 && w.count(0x80031260)==0 && !w.parties.pointer(1));}
 {PositionWorld w(root);w.mario.partyIndices[1]=127;w.args({1});assert(w.step()==2 && w.count(0x80031260)==0 && !w.parties.pointer(1));}
 {PositionWorld w(root);w.args({1,12,13,14});w.mario.savedPartyKinds[1]=0;assert(w.step(true,true)==2 && w.count(0x80031260)==3 && !w.parties.pointer(1));}
 {PositionWorld w(root);w.args({1,12,13,14});w.mario.savedPartyKinds[1]=13;assert(w.step(true,true)==2 && w.mario.partyIndices[1]==1 && w.mario.savedPartyKinds[1]==13);auto* p=w.parties.pointer(1);assert(p->scalar(0x58)==12 && p->scalar(0x60)==14 && w.count(0x80140ea4)==0);}
 {PositionWorld w(root);w.originalArgs(0x498e8);w.mario.savedPartyKinds[0]=1;w.event.locals[0]=25;w.event.locals[1]=0;w.event.locals[2]=625;assert(w.step(true,true)==2 && w.mario.partyIndices[0]==0 && w.parties.pointer(0)->bytes[0x34]==0);}
}
void heldModeAndDisplay(const char* root){
 {PositionWorld w(root);auto entry=w.ctor(false);w.onCall=[&](uint32_t fn){if(fn==0x800bcb2c)w.parties.pointer(1)->bytes[0x34]=1;};int32_t result=99;
  assert(!entry->position(13,true,{{1,2,3}},{},1000,{},result) && entry->pending()==0x800bcb98);auto* p=w.parties.pointer(1);assert(p->bytes[0x35]==1 && p->word(0x58)==0);
  p->bytes[0x34]=0;for(unsigned i=0;i<3;++i)assert(!entry->position(13,true,{{1,2,3}},{},1000,{},result) && entry->pending()==0x800bcb98 && w.count(0x800bcb2c)==1 && w.count(0x800bcb98)==1);}
 {PositionWorld w(root);auto entry=w.ctor();int32_t result=99;const std::array<float,3> xyz{{25,0,625}};
  assert(!entry->position(1,false,xyz,{},1000,{},result));auto* p=w.parties.pointer(0);assert(p && p->scalar(0x58)==25 && w.mario.partyIndices[0]==255);
  for(unsigned i=0;i<3;++i)assert(!entry->position(1,false,xyz,{},1000,{},result) && w.parties.pointer(0)==p && w.count(0x8004176c)==1);
  assert(entry->position(1,false,xyz,{},1000,w.appearance,result) && result==0 && w.displays==2 && w.mario.partyIndices[0]==255);}
}
}
void runNativeFieldPartyPositionFixtures(const char* root){directAuxiliary(root);ordinaryPosition(root);authoredHarborAndPlaza(root);originalEventGatesAndSavedKind(root);heldModeAndDisplay(root);}
int main(){runNativeFieldPartyPositionFixtures("romfs");std::cout<<"original party position constructors and event publication: PASS\n";}

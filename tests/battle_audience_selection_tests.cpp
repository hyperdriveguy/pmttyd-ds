#include "battle_audience_item_on.hpp"
#include "battle_weapon_menu_main.hpp"
#include "harbor_tutorial_packed.hpp"
#include <cassert>
#include <map>
#include <functional>
#include <vector>
#include <memory>
using namespace ttyd;
struct Fixture {
 static constexpr uint32_t B=0x100000,B2=0x200000,U=0x300000,Out=0x400000;
 std::map<uint32_t,uint32_t> w{{0x8041eb90,B}};
 std::map<uint32_t,uint16_t> h;std::map<uint32_t,uint8_t> b;
 std::vector<uint32_t> stores;std::function<void(uint32_t,uint32_t)> onStore;
 unsigned triggers=0,queries=0,targets=0,ends=0,lights=0;bool queryReady=true,endReady=false;
 static uint32_t member(uint32_t battle,int32_t slot){return battle+0x2858+0x1bc+uint32_t(slot)*0x134;}
 static uint32_t item(uint32_t battle,int32_t slot){return battle+0x2858+0xf25c+uint32_t(slot)*0x48;}
 void holder(int slot,int index,uint32_t flags=1,int state=7){const auto p=member(B,slot);w[p]=flags;h[p+0x1c]=uint16_t(index);b[p+0x19]=uint8_t(state);w[item(B,index)]=1;w[item(B,index)+4]=4;}
 BattleActorServices memory(){BattleActorServices a;a.user=this;a.memory.user=this;
  a.memory.word=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->w[at];};
  a.memory.half=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->h[at];};
  a.byte=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->b[at];};
  a.memory.storeWord=[](void* p,uint32_t at,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.w[at]=v;f.stores.push_back(at);if(f.onStore)f.onStore(at,v);};
  a.memory.storeHalf=[](void* p,uint32_t at,uint16_t v){static_cast<Fixture*>(p)->h[at]=v;};
  a.memory.storeByte=[](void* p,uint32_t at,uint8_t v){static_cast<Fixture*>(p)->b[at]=v;};return a;
 }
 BattleMenuAudienceServices services(){BattleMenuAudienceServices s;s.memory=memory();s.user=this;s.privateItem=Out;
  s.trigger=[](void* p,uint32_t mask){auto& f=*static_cast<Fixture*>(p);++f.triggers;assert(mask==0x400);return mask;};
  s.mario=[](void*,uint32_t battle){assert(battle==B);return U;};
  s.itemOn=[](void* p,uint32_t out,int32_t,int32_t,int32_t,int32_t){auto& f=*static_cast<Fixture*>(p);++f.queries;if(!f.queryReady)return false;packedBattleAudienceItemOn({out,0,0,0,0},f.memory());return true;};
  s.setTarget=[](void* p,int32_t target){auto& f=*static_cast<Fixture*>(p);++f.targets;packedBattleAudienceSetTarget(target,f.memory());};
  s.endActMenuReady=[](void* p,uint32_t battle){auto& f=*static_cast<Fixture*>(p);++f.ends;if(!f.endReady)return false;f.w[battle+0x1c5c]=6;return true;};
  s.lightOff=[](void* p,int32_t a,int32_t b){auto& f=*static_cast<Fixture*>(p);assert(!a && b==1);++f.lights;f.w[B+0xef4]=0x80;};return s;
 }
};
void targetCleanup(){
 Fixture f;f.holder(3,0);f.holder(8,1);f.holder(9,2,0x101);f.holder(10,3,3);f.holder(11,4,1,8);f.holder(199,5);f.holder(200,6);
 packedBattleAudienceSetTarget(3,f.memory());
 assert(f.w[Fixture::member(Fixture::B,3)]==0x21 && f.h[Fixture::member(Fixture::B,3)+0x1c]==0);
 for(int n:{8,199}){assert(f.w[Fixture::item(Fixture::B,n==8?1:5)]==0);assert(f.h[Fixture::member(Fixture::B,n)+0x1c]==0xffff && f.b[Fixture::member(Fixture::B,n)+0x19]==0);}
 for(int n:{9,10,11,200})assert(f.h[Fixture::member(Fixture::B,n)+0x1c]!=0xffff);
 assert(f.stores.front()==Fixture::member(Fixture::B,3));
 // Original signed held index and wrap arithmetic are not clamped to the item bank.
 Fixture negative;negative.holder(1,-2);packedBattleAudienceSetTarget(0,negative.memory());assert(negative.w[Fixture::item(Fixture::B,-2)]==0);
 // Item clear keeps the captured member, then ChangeStatus freshly resolves global B.
 Fixture live;live.holder(1,0);live.w[Fixture::member(Fixture::B2,1)]=1;live.b[Fixture::member(Fixture::B2,1)+0x19]=7;
 live.onStore=[&](uint32_t at,uint32_t){if(at==Fixture::item(Fixture::B,0))live.w[0x8041eb90]=Fixture::B2;};
 packedBattleAudienceSetTarget(0,live.memory());assert(live.h[Fixture::member(Fixture::B,1)+0x1c]==0xffff && live.b[Fixture::member(Fixture::B,1)+0x19]==7 && live.b[Fixture::member(Fixture::B2,1)+0x19]==0);
}
void retainedInterruption(){
 Fixture f;f.holder(3,0);f.holder(8,1);BattleMenuAudienceCursor c;auto s=f.services();
 f.queryReady=false;assert(!resumePackedBattleMenuAudienceCheck(s,c));assert(f.triggers==1 && f.queries==1 && !f.targets);
 f.queryReady=true;assert(!resumePackedBattleMenuAudienceCheck(s,c));assert(f.triggers==1 && f.queries==2 && f.targets==1 && f.ends==1 && !f.lights);
 assert(c.active && c.phase==3 && f.w[Fixture::member(Fixture::B,3)]==0x21);
 // Change global B, output and target flags during the held UI call. None may
 // retrigger the admitted query/target. Previous type is read only after End.
 f.w[0x8041eb90]=Fixture::B2;f.w[Fixture::Out]=99;f.w[Fixture::member(Fixture::B,3)]=1;
 Fixture other;auto wrong=other.services();assert(!resumePackedBattleMenuAudienceCheck(wrong,c));assert(!other.triggers);
 f.endReady=true;assert(resumePackedBattleMenuAudienceCheck(s,c)==true && !c.active);
 assert(f.targets==1 && f.queries==2 && f.triggers==1 && f.ends==2 && f.lights==1);
 assert(f.w[Fixture::member(Fixture::B,3)]==1 && f.w[Fixture::B+0x1c60]==6 && f.w[Fixture::B+0x1c5c]==11 && f.w[Fixture::B+0x171c]==29 && f.w[Fixture::B+0xef4]==0x84);
 Fixture empty;auto e=empty.services();assert(resumePackedBattleMenuAudienceCheck(e,c)==false && !c.active && !empty.targets);
 Fixture blocked;blocked.w[Fixture::B+0xef4]=0x40000000;assert(resumePackedBattleMenuAudienceCheck(blocked.services(),c)==false && !blocked.queries);
}
void nativePhysicalBridge(){
 auto ownedVM=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});auto& vm=*ownedVM;
 assert(vm.memory.registerRam(0x80422b44,{0x42,0xb4,0,0,0xc2,0x48,0,0}));
 assert(vm.memory.registerRam(0x80424988,std::vector<uint8_t>(0x48)));
 assert(vm.memory.registerRam(0x802fa104,std::vector<uint8_t>(48)));
 auto ownedPacked=std::make_unique<HarborTutorialPacked>(vm);auto& p=*ownedPacked;assert(p.provision());auto ownedScene=std::make_unique<HarborTutorial>();auto& scene=*ownedScene;
 const auto B=HarborTutorialPacked::Battle,U=HarborTutorialPacked::Units;
 auto put=[&](uint32_t at,uint32_t v){assert(vm.memory.writeWord(at,v));};
 auto read=[&](uint32_t at){uint32_t v;assert(vm.memory.readWord(at,v));return v;};
 put(B+0x20,U);put(U+8,222);put(U+0x10c,0x00070000);put(B+0x1c5c,2);put(B+0x1720,0x11);put(B+0x1c78,0x84600c00);
 const auto actor=Fixture::member(B,3);put(actor,1);assert(vm.memory.writeHalf(actor+0x1c,0));assert(vm.memory.writeByte(actor+0x19,7));put(Fixture::item(B,0)+4,4);
 scene.classMenuOpen=scene.weaponMenuOpen=true;BattlePadSample physical;physical.snapshot.buttons=physical.pressed=0x400;p.samplePhysical(17,&physical);
 assert(p.attackAudienceMenu(scene)==true && !scene.classMenuOpen && !scene.weaponMenuOpen);
 assert(read(B+0x1c5c)==11 && read(B+0x1c60)==2 && read(B+0x171c)==29 && read(B+0x1720)==0x10 && read(B+0x1c78)==0 && read(actor)==0x21);
 // Unknown native UI owner waits, preserving completed target effects. The
 // helper can resume when that owner becomes available; no fake free succeeds.
 BattlePadSample quiet;p.samplePhysical(34,&quiet);p.samplePhysical(51,&physical);
 put(B+0x1c78,0x84600100);put(actor,1);put(B+0x1c5c,4);assert(!p.attackAudienceMenu(scene));assert(p.audienceMenuCursor.active && read(actor)==0x21);
 put(actor,1);put(B+0x1c78,0);p.samplePhysical(68,&quiet);assert(p.attackAudienceMenu(scene)==true && read(actor)==1 && read(B+0x1c60)==4);
 assert(read(U+0x10c)==0x00070000);for(int32_t i=0;i<vm.battle.capacity;++i)assert(!(vm.battle.entries[i].flags&1));
}
int main(){targetCleanup();retainedInterruption();nativePhysicalBridge();}

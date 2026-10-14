#include "battle_weapon_menu_decision.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
 static constexpr uint32_t B=0x10000,M=B+0x171c,U=0x40000,V=0x40100,W=0x50000,Buf=B+0x428;
 std::map<uint32_t,uint32_t> words{{B+0x420,1},{M+0x80,W},{M+4,0x80}};std::map<uint32_t,uint16_t> halves;
 std::map<uint32_t,uint8_t> bytes{{U+0xc,255},{B+2,254}};std::vector<int> order;bool ready=true,mutateActor=false;
 BattleWeaponMenuDecisionServices services(){BattleWeaponMenuDecisionServices s;s.user=this;s.privateCursor=0x60000;s.memory.user=this;s.memory.memory.user=this;
 s.memory.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);return f.words[at];};s.memory.memory.half=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->halves[at];};s.memory.byte=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->bytes[at];};s.memory.memory.storeByte=[](void* p,uint32_t at,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);f.bytes[at]=v;};s.memory.memory.storeWord=[](void* p,uint32_t at,uint32_t v){static_cast<Fixture*>(p)->words[at]=v;};
 s.unit=[](void* p,uint32_t b,int32_t id){auto& f=*static_cast<Fixture*>(p);assert(b==B);f.order.push_back(1);return id==1?U:V+uint32_t(id)*0x100;};
 s.build=[](void* p,uint32_t b,uint32_t type){auto& f=*static_cast<Fixture*>(p);assert(b==B && type==0);f.order.push_back(2);if(f.mutateActor)f.words[B+0x420]=2;return f.ready;};
 s.enemyBelong=[](void*,uint32_t u){assert(u==U);return 257;};
 s.belong=[](void* p,uint32_t u){auto& f=*static_cast<Fixture*>(p);return u==U?255:int32_t(f.bytes[u+0xc]);};
 s.sample=[](void* p,uint32_t buffer,uint32_t weapon,int32_t actor,int32_t enemy,uint32_t,uint32_t,int32_t direction){auto& f=*static_cast<Fixture*>(p);assert(buffer==Buf && weapon==W && actor==(f.mutateActor?2:1) && enemy==1 && direction==-2);f.order.push_back(3);f.bytes[Buf+0xab7]=77;return true;};
 s.cursor=[](void* p,uint32_t menu,uint32_t type,uint32_t output){auto& f=*static_cast<Fixture*>(p);assert(menu==M && type==0);f.order.push_back(5);f.words[output]=0x70000;};
 s.initialize=[](void* p,uint32_t b,uint32_t cursor,uint32_t rows,uint32_t flags){auto& f=*static_cast<Fixture*>(p);assert(b==B && cursor==0x70000 && rows==M+0x80 && flags==9);f.order.push_back(6);f.words[M+4]=0x40;return true;};return s;}
 void candidates(){bytes[Buf+0xa6c]=3;for(unsigned j=0;j<3;++j){bytes[Buf+0xa6d+j]=j;halves[Buf+4+36*j]=uint16_t(j+2);bytes[V+(j+2)*0x100+0xc]=j==1?0:255;}}
};
int main(){
 Fixture f;f.mutateActor=true;assert(decidePackedBattleWeapon(Fixture::B,0,0,f.services())==0 && f.bytes[Fixture::Buf+0xab7]==0);assert(f.order==std::vector<int>({1,2,3,4}));
 Fixture same;same.candidates();same.words[Fixture::W+0x68]=0x30000000;assert(decidePackedBattleWeapon(Fixture::B,0,0,same.services())==0 && same.bytes[Fixture::Buf+0xab7]==2);
 Fixture different;different.candidates();different.words[Fixture::W+0x68]=0x20000000;assert(decidePackedBattleWeapon(Fixture::B,0,0,different.services())==0 && different.bytes[Fixture::Buf+0xab7]==1);
 Fixture unavailable;unavailable.ready=false;assert(!decidePackedBattleWeapon(Fixture::B,0,0,unavailable.services()).has_value() && unavailable.order==std::vector<int>({1,2}));
 Fixture negative;negative.bytes[Fixture::Buf+0xa6c]=255;negative.words[Fixture::W+0x68]=0x10000000;assert(decidePackedBattleWeapon(Fixture::B,0,0,negative.services())==0 && negative.bytes[Fixture::Buf+0xab7]==0);
 Fixture live;live.candidates();live.words[Fixture::W+0x68]=0x10000000;auto liveServices=live.services();liveServices.belong=[](void* p,uint32_t u){auto& f=*static_cast<Fixture*>(p);if(u==Fixture::U)return 255;if(u==Fixture::V+2*0x100){f.words[Fixture::W+0x68]=0x20000000;return 0;}return int32_t(f.bytes[u+0xc]);};assert(decidePackedBattleWeapon(Fixture::B,0,0,liveServices)==0 && live.bytes[Fixture::Buf+0xab7]==0 && live.order==std::vector<int>({1,2,3,4,1,4})); // Flags reloaded after belong service; first differing candidate wins.
 Fixture count;count.candidates();count.words[Fixture::W+0x68]=0x10000000;auto countServices=count.services();countServices.belong=[](void* p,uint32_t u){auto& f=*static_cast<Fixture*>(p);if(u==Fixture::U)return 255;f.bytes[Fixture::Buf+0xa6c]=1;return 255;};assert(decidePackedBattleWeapon(Fixture::B,0,0,countServices)==0 && count.bytes[Fixture::Buf+0xab7]==0 && count.order==std::vector<int>({1,2,3,4,1,4}));
 Fixture permutation;permutation.candidates();permutation.words[Fixture::W+0x68]=0x20000000;permutation.bytes[Fixture::Buf+0xa6d]=2;permutation.bytes[Fixture::Buf+0xa6e]=0;permutation.bytes[Fixture::Buf+0xa6f]=1;assert(decidePackedBattleWeapon(Fixture::B,0,0,permutation.services())==0 && permutation.bytes[Fixture::Buf+0xab7]==2);
 Fixture unavailableSample;auto stalled=unavailableSample.services();stalled.sample=[](void*,uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t){return false;};unavailableSample.bytes[Fixture::Buf+0xab7]=77;assert(!decidePackedBattleWeapon(Fixture::B,0,0,stalled) && unavailableSample.bytes[Fixture::Buf+0xab7]==77);
 Fixture setup;assert(setupPackedBattleWeaponMenu(Fixture::B,0,9,setup.services()) && setup.words[Fixture::M+4]==0x01000050 && setup.order==std::vector<int>({2,5,6}));
}

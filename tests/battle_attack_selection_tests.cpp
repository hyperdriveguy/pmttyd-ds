#include "battle_attack_selection.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
 static constexpr uint32_t B=0x10000,U=0x40000,M=0x50000,W=0x60000,C=0x70000;
 std::map<uint32_t,uint32_t> words{{0x8041eb90,B},{U,1},{U+8,222},{M+0x80,W},{C,0},{W+0xb0,0x1234},{W+0x1c,1},{B+0x24,U}};
 std::map<uint32_t,uint8_t> bytes;std::vector<uint32_t> reads;std::vector<std::pair<uint32_t,uint32_t>> writes;unsigned cursorCalls=0,messageCalls=0;bool cursorReady=true,mutateGlobal=false,mutateKind=false,resumeAfterCount=false,recapture=false;
 BattleAttackSelectionServices services(){BattleAttackSelectionServices s;s.user=this;s.memory.user=this;s.memory.memory.user=this;
  s.memory.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(at);return f.words[at];};
  s.memory.byte=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(at);return f.bytes[at];};
  s.memory.memory.storeWord=[](void* p,uint32_t at,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.writes.emplace_back(at,v);f.words[at]=v;if(f.mutateKind && at==U+0x2a0)f.words[U+8]=223;};
  s.memory.memory.storeByte=[](void* p,uint32_t at,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.writes.emplace_back(at,v);f.bytes[at]=v;if(f.recapture && at==U+0x20)f.words[0x8041eb90]=B+0x1000;if(f.resumeAfterCount && at==B+0x16f47){f.words[B+0x19090]=1;f.words[B+0x19094]=44;}};
  s.cursor=[](void* p,uint32_t menu,uint32_t,uint32_t& cursor){auto& f=*static_cast<Fixture*>(p);assert(menu==M);++f.cursorCalls;if(f.mutateGlobal)f.words[0x8041eb90]=B+0x1000;cursor=C;return f.cursorReady;};
  s.continueMessage=[](void* p,uint32_t handle){auto& f=*static_cast<Fixture*>(p);assert(handle==44 && f.bytes[B+0x16f47]==1);++f.messageCalls;};return s;}
 bool run(uint32_t type,uint32_t sub=0){words[M+0x540]=type;words[M+0x548]=sub;return packedSetBattleAttackEvent(U,M,services());}
};
int main(){
 for(uint32_t type=0;type<14;++type){Fixture f;f.bytes[Fixture::U+0x20]=9;assert(f.run(type));assert(f.words[Fixture::U+0x2a0]);if(type==0)assert(f.bytes[Fixture::B+0x16f38]==1);if(type==1)assert(f.bytes[Fixture::B+0x16f39]==1);if(type==2)assert(f.bytes[Fixture::B+0x16f41]==1);if(type==9)assert(f.bytes[Fixture::U+0x20]==1 && f.words[Fixture::U+0x2ac]==7);}
 for(uint32_t sub=0;sub<10;++sub){Fixture f;f.words[0x803550a8+0xb0]=101;f.words[0x80355228+0xb0]=102;f.words[Fixture::U+0x18]=0x80000;f.words[0x80000]=2;f.words[0x80004]=108;assert(f.run(5,sub));assert(f.words[Fixture::U+0x2a0]);if(sub==1 || sub==2)assert(f.words[Fixture::U+0x2a0]==100+sub);if(sub==8)assert(f.words[Fixture::U+0x2a0]==108 && f.words[Fixture::U+0x2ac]==3);}
 Fixture override;override.bytes[Fixture::M+0x551]=1;assert(override.run(0) && !override.cursorCalls && override.bytes[Fixture::U+0x20]==0 && override.words[Fixture::U+0x2ac]==1);assert((override.writes==std::vector<std::pair<uint32_t,uint32_t>>{{Fixture::U+0x20,0},{Fixture::U+0x2a0,0x80416b40},{Fixture::U+0x2ac,1}}));
 Fixture fallback;fallback.words[Fixture::W+0xb0]=0;fallback.mutateGlobal=true;assert(fallback.run(4) && fallback.words[Fixture::U+0x2a0]==0x80416b40 && fallback.bytes[Fixture::B+0x16f3a]==1);
 Fixture charge;charge.mutateKind=true;charge.words[0x803550a8+0xb0]=101;assert(charge.run(5,1) && charge.bytes[Fixture::B+0x16f4f]==1 && charge.bytes[Fixture::B+0x16f50]==0); // Retain branch-kind choice through event store.
 for(uint8_t n:{uint8_t(99),uint8_t(100),uint8_t(255)}){Fixture f;f.bytes[Fixture::B+0x16f38]=n;f.run(0);assert(f.bytes[Fixture::B+0x16f38]==(n<100?n+1:n));assert(f.writes.size()==(n<100?3u:2u));}
 Fixture unsupported;unsupported.cursorReady=false;assert(!unsupported.run(0) && unsupported.writes.empty());
 Fixture special;special.words[Fixture::M+0x80]=0x8036aa84;special.words[0x8036aa84+0xb0]=1;assert(special.run(2) && special.bytes[Fixture::B+0x16f41]==0); // Pointer exemption, before kind/classifier.
 for(int value:{-128,-1,0,6,7,8,9,127}){Fixture f;f.bytes[Fixture::U+0x20]=uint8_t(value);f.resumeAfterCount=true;assert(f.run(11));assert(f.bytes[Fixture::U+0x20]==(value==6?5:value==7?6:value==9?8:4) && f.messageCalls==1);}
 Fixture move;move.words[Fixture::B+0x16040]=1;assert(move.run(12) && move.words[Fixture::U+0x2a0]==0x80373d3c && move.messageCalls==0);move.words[Fixture::B+0x16040]=2;move.run(12);assert(move.words[Fixture::U+0x2a0]==0x80373d74);
 Fixture data;data.words[Fixture::U+0x18]=0x80000;data.words[0x80000]=2;data.words[0x80004]=0;data.words[0x80008]=2;data.words[0x8000c]=99;assert(packedUnitData(Fixture::U,2,data.services().memory)==0 && packedUnitData(Fixture::U,0,data.services().memory)==0);
 Fixture missing;assert(missing.run(5,8) && missing.words[Fixture::U+0x2a0]==0 && missing.words[Fixture::U+0x2ac]==3);
 for(auto mode:{uint32_t(14),uint32_t(-1)}){Fixture invalid;invalid.bytes[Fixture::U+0x20]=77;assert(invalid.run(mode) && invalid.words[Fixture::U+0x2a0]==0x80416b40 && invalid.words[Fixture::U+0x2ac]==0 && invalid.bytes[Fixture::U+0x20]==77);}
 Fixture other;other.words[Fixture::U+8]=223;assert(other.run(2) && other.bytes[Fixture::B+0x16f43]==1);other.run(5,6);assert(other.words[Fixture::U+0x2a0]==0x80416b88);
 Fixture recaptured;recaptured.recapture=true;recaptured.words[Fixture::B+0x1000+0x24]=Fixture::U;recaptured.words[Fixture::U+0x18]=0x80000;recaptured.words[0x80000]=6;recaptured.words[0x80004]=99;assert(recaptured.run(11) && recaptured.words[Fixture::U+0x2a0]==99 && recaptured.bytes[Fixture::B+0x16f47]==1 && recaptured.bytes[Fixture::B+0x1000+0x16f47]==0);
 Fixture classifier;auto memory=classifier.services().memory;assert(!packedWeaponIsAttack(0,memory));assert(packedWeaponIsAttack(Fixture::W,memory));classifier.words[Fixture::W+0x1c]=0;classifier.bytes[Fixture::W+0x93]=1;classifier.bytes[Fixture::W+0x94]=1;classifier.bytes[Fixture::W+0x95]=255;assert(packedWeaponIsAttack(Fixture::W,memory));classifier.bytes[Fixture::W+0x95]=1;assert(!packedWeaponIsAttack(Fixture::W,memory));classifier.bytes[Fixture::W+0xa2]=255;classifier.bytes[Fixture::W+0xa3]=1;assert(!packedWeaponIsAttack(Fixture::W,memory));
}

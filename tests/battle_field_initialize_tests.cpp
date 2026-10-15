#include "battle_field_initialize.hpp"
#include <cassert>
#include <map>
#include <functional>
#include <vector>
using namespace ttyd;
struct InitFixture {
 uint32_t gp=0x81000000,f=0x85000000,n=0x86000000,p=0x87000000,party=0x88000000,hold=0,debug=0,kind=7;
 std::map<uint32_t,uint32_t> words;
 std::vector<BattleSequenceCall> calls;
 std::function<void(BattleSequenceCall&)> hook;
 bool absentField=false;
 InitFixture(){words[0x80415fd0]=gp;words[gp+0x168]=f;words[f+8]=n;words[party+0x31]=1;words[n+0x2f4]=3;words[n+0x2f5]=4;words[n+0x2f6]=5;}
 BattleSequenceEntryServices services(){BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<InitFixture*>(p);if(f.absentField && a==f.gp+0x168)return {};return f.words[a];};
 s.byte=[](void* p,uint32_t a)->std::optional<uint8_t>{return uint8_t(static_cast<InitFixture*>(p)->words[a]);};
 s.half=[](void* p,uint32_t a)->std::optional<uint16_t>{return uint16_t(static_cast<InitFixture*>(p)->words[a]);};
 s.store=[](void* p,uint32_t a,uint32_t v){static_cast<InitFixture*>(p)->words[a]=v;return true;};
 s.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<InitFixture*>(p)->words[a]=v;return true;};
 s.call=[](void* p,BattleSequenceCall& c){auto& f=*static_cast<InitFixture*>(p);f.calls.push_back(c);if(c.function==f.hold)return false;
 switch(c.function){case 0x800d5eb0:c.result=f.p;break;case 0x80009d90:c.result=f.debug;break;case 0x80141050:c.result=0;break;case 0x800bf1b4:c.result=f.party;break;case 0x800f83e4:c.result=f.kind;break;default:break;}if(f.hook)f.hook(c);return true;};return s;}
 unsigned count(uint32_t fn){unsigned n=0;for(auto& c:calls)n+=c.function==fn;return n;}
 const BattleSequenceCall& last(uint32_t fn){for(auto it=calls.rbegin();it!=calls.rend();++it)if(it->function==fn)return *it;assert(false);return calls.front();}
};
int main(){
 {InitFixture f;BattleFieldInitializeCursor c;f.absentField=true;assert(!initializeFieldBattle(f.services(),c));f.words[0x80415fd0]=0x81100000;f.absentField=false;f.hold=0x80030078;
  assert(!initializeFieldBattle(f.services(),c) && c.field==f.f && c.npc==f.n && c.pouch==f.p && c.pending==f.hold && f.count(0x8000e95c)==1);}
 for(uint32_t debug:{0u,0x90000000u}){InitFixture f;BattleFieldInitializeCursor c;f.words[f.gp+0xc]=1;f.debug=debug;f.hold=0x8008465c;
  assert(!initializeFieldBattle(f.services(),c) && f.last(f.hold).args[0]==(debug?debug:0x802c2abc));f.debug=0xffffffff;f.words[f.f+8]=0xdead0000;f.hold=0x80041c58;
  assert(!initializeFieldBattle(f.services(),c) && c.npc==f.n && f.count(0x80009d90)==1 && f.words[0x8041eb90]==0 && f.count(0x80030078)==1);
  assert(f.count(0x8004ce58)==0 && f.words[f.gp+0x14]==0);}
 const uint32_t types[]={0,0x20000,0x40000,0x80000,0x100000,0x200000,0x400000,0x800000,0x1000000,0x10000000,0xffffffff};
 for(auto type:types)for(unsigned blocked=0;blocked<2;++blocked){InitFixture f;BattleFieldInitializeCursor c;f.words[f.f+0x10]=type;f.words[f.f+0x14]=123;f.words[f.f+0x1c]=blocked;f.hold=0x800f8f04;
  assert(!initializeFieldBattle(f.services(),c) && c.pending==f.hold && f.words[f.gp+0x14]==1 && f.count(0x800377b0)==1);
  const uint32_t expected=blocked?0:type==0x10000000?123:type>=0x20000 && type<=0x1000000?uint32_t(__builtin_ctz(type)-16):0;
  assert(f.last(0x80114588).args[1]==expected && f.last(0x80114590).args[1]==7 && f.last(0x80114510).args[1]==f.n+0x230);
  assert(f.words[f.f+0x38]==3 && f.words[f.f+0x3a]==4 && f.words[f.f+0x3b]==5 && f.last(0x800f8f04).args[0]==f.f+0x20);
  f.words[f.f+8]=0;f.hold=0;assert(initializeFieldBattle(f.services(),c) && !c.active && !c.pending && f.count(0x80041c58)==1 && f.count(0x80114518)==1);}
 for(unsigned mode=0;mode<3;++mode){InitFixture f;BattleFieldInitializeCursor c;if(mode==0)f.party=0;if(mode==1)f.words[f.p+14]=2;if(mode==2){f.words[f.party+0x31]=255;f.words[f.p-14]=0;}
  assert(initializeFieldBattle(f.services(),c));assert(f.count(0x80114590)==unsigned(mode==2));if(mode==2)assert(f.last(0x800f83e4).args[0]==0xffffffff);}
 {InitFixture f;BattleFieldInitializeCursor c;f.hold=0x800f83e4;assert(!initializeFieldBattle(f.services(),c));f.words[f.party+0x31]=2;f.words[f.p+14]=2;f.hold=0;
  assert(initializeFieldBattle(f.services(),c) && c.kind==1 && f.count(0x80114590)==0);}
}

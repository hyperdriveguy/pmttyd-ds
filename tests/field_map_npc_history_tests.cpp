#include "field_map_npc_history.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <memory>
#include <string>
using namespace ttyd;
namespace {
constexpr uint32_t F=0x85000000,H=F+0x4c,Names=0x87000000,Map=Names+32;
struct Fixture {
 std::unique_ptr<EventNativeRuntime> vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});
 Fixture(){EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));assert(vm->memory.registerRam(F,std::vector<uint8_t>(0x580)) && vm->memory.registerRam(Names,std::vector<uint8_t>(512)));text(Map,"gor_02");}
 void store(uint32_t a,uint32_t v){assert(vm->memory.writeWord(a,v));}
 uint32_t word(uint32_t a){uint32_t v=0;assert(vm->memory.readWord(a,v));return v;}
 void text(uint32_t a,const std::string& s){for(unsigned n=0;n<=s.size();++n)assert(vm->memory.writeByte(a+n,n==s.size()?0:uint8_t(s[n])));}
 std::string text(uint32_t a){std::string s;for(;;++a){uint8_t b=0;assert(vm->memory.readByte(a,b));if(!b)return s;s.push_back(char(b));}}
 void slot(unsigned n,const std::string& name,uint32_t mask){text(H+20*n,name);store(H+20*n+16,mask);}
 auto prepare(){return prepareFieldMapNpcHistory(vm->memory,F,Map);}
 void definition(uint32_t flags,uint32_t currentGroup=4,uint32_t previousGroup=4){
  // Runtime-copy fixture; original pack remains unchanged. First match wins.
  text(Names,"previous");store(0x8030f668,Map);store(0x8030f66c,currentGroup);store(0x8030f670,flags);
  store(0x8030f674,Names);store(0x8030f678,previousGroup);store(0x8030f67c,0);store(0x8030f680,0);
 }
};
void originalAndPromotion(){
 Fixture f;f.slot(0,"previous",0x55);f.slot(1,"gor_02",0x12345678);f.slot(2,"older",0x88);
 const auto gate=f.prepare();assert(gate && gate->head==H && !gate->enabled);assert(f.text(H)=="gor_02" && f.word(H+16)==0x12345678 && f.text(H+20)=="previous" && f.word(H+36)==0x55 && f.text(H+40)=="older");
 assert(skipFieldMapNpc(f.vm->memory,*gate,3)==false);f.store(H+16,0xffffffff);assert(skipFieldMapNpc(f.vm->memory,*gate,0)==false);
 const auto again=f.prepare();assert(again && !again->enabled && f.text(H+20)=="previous" && f.word(H+16)==0xffffffff);
 Fixture fresh;fresh.slot(0,"previous",1);fresh.slot(1,"older",2);fresh.store(H+2*20+16,0xdeadbeef);assert(fresh.prepare());assert(fresh.text(H)=="gor_02" && fresh.word(H+16)==0 && fresh.text(H+20)=="previous" && fresh.text(H+40)=="older");
}
void groupRules(){
 for(unsigned flags:{1u,3u,5u,7u}){Fixture f;f.definition(flags);f.slot(0,"previous",1);f.slot(1,"gor_02",2);f.slot(2,"older",3);const auto gate=f.prepare();assert(gate && gate->enabled && f.word(H+16)==2 && f.word(H+36)==1);assert(f.word(H+56)==(flags==5?0u:3u));assert(f.text(H+40)==(flags==5?"":"older"));}
 Fixture changed;changed.definition(1,4,5);changed.slot(0,"previous",1);changed.slot(1,"gor_02",2);changed.slot(2,"older",3);const auto gate=changed.prepare();assert(gate && gate->enabled && changed.text(H)=="gor_02");for(unsigned n=16;n<0x500;++n){uint8_t b=1;assert(changed.vm->memory.readByte(H+n,b) && b==0);}
 // Head selection compares the actual second record, not a host last-map cache.
 Fixture head;head.definition(1,4,5);head.slot(0,"gor_02",2);head.slot(1,"previous",1);assert(head.prepare() && head.word(H+16)==0);
}
void masksAndAdmission(){
 Fixture f;f.definition(1);f.slot(0,"gor_02",0x80000001);f.slot(1,"previous",0);const auto gate=f.prepare();assert(gate && gate->enabled);
 for(unsigned n:{0u,31u,64u,95u,256u,287u})assert(skipFieldMapNpc(f.vm->memory,*gate,n)==true);
 for(unsigned n:{1u,32u,63u,96u,255u})assert(skipFieldMapNpc(f.vm->memory,*gate,n)==false);
 f.store(H+16,2);assert(skipFieldMapNpc(f.vm->memory,*gate,1)==true && skipFieldMapNpc(f.vm->memory,*gate,0)==false);
 Fixture full;for(unsigned n=0;n<64;++n)full.slot(n,"room"+std::to_string(n),n+1);assert(!full.prepare() && full.text(H)=="room0" && full.word(H+16)==1 && full.text(H+63*20)=="room63");
 full.slot(63,"gor_02",0xa5);assert(full.prepare() && full.text(H)=="gor_02" && full.word(H+16)==0xa5 && full.text(H+63*20)=="room62");
 Fixture malformed;malformed.text(Map,"1234567890123456");assert(!malformed.prepare() && malformed.text(H).empty());assert(!prepareFieldMapNpcHistory(malformed.vm->memory,0xfffffff0,Map));
}
void orderedPromotion(){
 Fixture f;f.slot(0,"previous",1);f.slot(1,"older",2);f.slot(2,"gor_02",3);
 struct Probe {Fixture* f;unsigned mutations=0;} p{&f};PackedMapNpcHistoryServices s;s.user=s.memory.user=&p;
 s.memory.word=[](void* p,uint32_t a){return static_cast<Probe*>(p)->f->word(a);};
 s.memory.storeWord=[](void* raw,uint32_t a,uint32_t v){auto& p=*static_cast<Probe*>(raw);p.f->store(a,v);
  if(a==H+40){p.f->store(H+24,0x11111111);p.f->store(H+28,0x22222222);++p.mutations;}
  if(a==H+48){p.f->store(H+36,0x44444444);++p.mutations;}
 };
 s.compare=[](void* raw,uint32_t a,uint32_t b)->int32_t{auto& f=*static_cast<Probe*>(raw)->f;const auto x=f.text(a),y=f.text(b);return x<y?-1:x>y?1:0;};
 s.copyString=[](void* raw,uint32_t a,uint32_t b){auto& f=*static_cast<Probe*>(raw)->f;f.text(a,f.text(b));};
 s.clear=[](void* raw,uint32_t a,uint32_t size){auto& m=static_cast<Probe*>(raw)->f->vm->memory;for(unsigned n=0;n<size;++n)assert(m.writeByte(a+n,0));};
 const auto gate=preparePackedMapNpcHistory(F,Map,s);assert(gate && !gate->enabled && p.mutations==2);
 assert(f.text(H)=="gor_02" && f.word(H+16)==3); // Selected five words captured before any shift stores.
 assert(f.word(H+44)==0x72000000 && f.word(H+48)==0x22222222 && f.word(H+56)==0x44444444); // Pair then fresh pair then late word.
}

}
int main(){originalAndPromotion();groupRules();masksAndAdmission();orderedPromotion();}

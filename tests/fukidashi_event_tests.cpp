#include "fukidashi_event.hpp"
#include "event_native_runtime.hpp"
#include "event_script_assets.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t E=0x1000,A=0x2000,M=0x3000,N=0x4000,P=0x5000,Name=0x6000,NpcName=0x6100;
constexpr uint32_t Twice=0x80420328,Thirds=0x8042032c,Zero=0x80420330,Me=0x80420334,Special=0x80420338,Empty=0x8042033c;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct F {
 FukidashiEventServices s;FukidashiEventCursor c;
 std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
 std::vector<uint32_t> reads,decoded;std::vector<char> kinds;std::vector<std::string> trace;
 FukidashiEntryRequest request;unsigned constructs=0,names=0,marios=0,npcs=0,ids=0,pointers=0,compares=0,divides=0;
 bool ready=true,compareReady=true,nameReady=true,rawReady=true,mutateDivisor=false,reentrant=false,extra=false;
 uint32_t party=P,effect=0x7000;uint8_t kind=1;
 F(){
  words[E+0x18]=A;words[E+0x170]=N;
  EventScriptAssets original;assert(original.load("romfs/fukidashi-event.pmevt"));
  for(auto a:{Twice,Thirds,Zero,Special})assert(original.readWord(a,words[a]));
  uint32_t me=0,empty=1;assert(original.readWord(Me,me)&&me==0x6d650000&&original.readWord(Empty,empty)&&empty==0);
  words[M+0x1bc]=bits(27);words[N+0x150]=bits(18);words[P+0xf0]=bits(21);
  for(unsigned i=0;i<=10;++i)words[A+4*i]=i;
  words[A]=2;words[A+4]=0;words[A+8]=NpcName;words[A+12]=1;words[A+16]=17;words[A+40]=60;
  for(unsigned i=5;i<10;++i)words[A+4*i]=bits(float(i)+.25f);
  s.user=this;
  s.word=[](void* p,uint32_t a){auto& f=*static_cast<F*>(p);f.reads.push_back(a);f.trace.push_back("r"+std::to_string(a));assert(f.words.count(a));return f.words.at(a);};
  s.byte=[](void* p,uint32_t a){auto& f=*static_cast<F*>(p);assert(a==P+0x31);f.trace.push_back("kind");return f.kind;};
  s.value=[](void* p,uint32_t entry,uint32_t raw)->std::optional<int32_t>{auto& f=*static_cast<F*>(p);assert(entry==E);if(!f.rawReady)return {};f.decoded.push_back(raw);f.kinds.push_back('i');return raw<=0x7fffffff?int32_t(raw):int32_t(int64_t(raw)-0x100000000LL);};
  s.real=[](void* p,uint32_t entry,uint32_t raw)->std::optional<float>{auto& f=*static_cast<F*>(p);assert(entry==E);if(!f.rawReady)return {};f.decoded.push_back(raw);f.kinds.push_back('f');return real(raw);};
  s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& f=*static_cast<F*>(p);++f.compares;if(!f.compareReady)return {};if(b==Me)return a==Me?0:1;assert(b==Empty&&a==Name);return 1;};
  s.binary=[](void* p,FukidashiEventBinary op,float a,float b){auto& f=*static_cast<F*>(p);f.trace.push_back(op==FukidashiEventBinary::Multiply?"mul":"div");if(op==FukidashiEventBinary::Multiply)return a*b;++f.divides;if(f.mutateDivisor&&f.divides==1)f.words[Thirds]=bits(6);return a/b;};
  s.mario=[](void* p)->std::optional<uint32_t>{++static_cast<F*>(p)->marios;return M;};
  s.npc=[](void* p,uint32_t name)->std::optional<uint32_t>{auto& f=*static_cast<F*>(p);assert(name==NpcName);++f.npcs;return N;};
  s.partyId=[](void* p,bool extra)->std::optional<uint32_t>{auto& f=*static_cast<F*>(p);++f.ids;f.extra=extra;return 7;};
  s.partyPointer=[](void* p,uint32_t id)->std::optional<uint32_t>{auto& f=*static_cast<F*>(p);assert(id==7);++f.pointers;return f.party;};
  s.construct=[](void* p,const FukidashiEntryRequest& r)->std::optional<uint32_t>{auto& f=*static_cast<F*>(p);++f.constructs;f.request=r;
   if(f.reentrant){assert(!f.c.reset());assert(!resumeFukidashiEvent(E,f.s,f.c));}
   if(!f.ready)return {};return f.effect;};
  s.setName=[](void* p,uint32_t e,uint32_t n){auto& f=*static_cast<F*>(p);assert(e==f.effect&&n==Name);++f.names;return f.nameReady;};
 }
 std::optional<int32_t> run(){return resumeFukidashiEvent(E,s,c);}
 std::vector<std::string> calculation(){auto begin=std::find(trace.begin(),trace.end(),"r"+std::to_string(Twice));if(begin==trace.end())begin=std::find(trace.begin(),trace.end(),"r"+std::to_string(M+0x1bc));
  // Height loads precede Twice on Mario/extra; select first nonargument read.
  begin=std::find_if(trace.begin(),trace.end(),[](const std::string& t){return t=="r"+std::to_string(Twice)||t=="r"+std::to_string(M+0x1bc)||t=="r"+std::to_string(P+0xf0);});return {begin,trace.end()};}
};
std::string r(uint32_t a){return "r"+std::to_string(a);}
void fixedAndCapture(){F f;f.words[A+20]=0x80000000;f.words[A+24]=0x7fc12345;f.ready=false;f.reentrant=true;
 assert(!f.run()&&f.c.pending()==0x80133164);assert(f.kinds==std::vector<char>({'i','i','i','i','i','f','f','f','f','f','i'}));
 assert(bits(f.request.position[0])==0x80000000&&bits(f.request.position[1])==0x7fc12345&&f.request.attachment==0&&f.request.variant==17&&f.request.duration==60);
 auto reads=f.reads;f.words[E+0x18]=0xdead;f.words[A+20]=0;f.ready=true;assert(f.run()==2&&f.reads==reads&&f.decoded.size()==11&&f.constructs==2);assert(!f.c.active());}
void rawCaptureAndOwnership(){F f;f.rawReady=false;assert(!f.run()&&f.c.pending()==0x80031260);auto reads=f.reads;f.words[A]=99;
 auto other=f.s;other.user=nullptr;assert(!resumeFukidashiEvent(E,other,f.c)&&f.reads==reads);assert(!resumeFukidashiEvent(E+4,f.s,f.c));
 f.rawReady=true;assert(f.run()==2&&f.decoded[0]==2&&f.constructs==1);}
void actors(){
 F mario;mario.words[A]=0;assert(mario.run()==2&&mario.marios==1&&mario.request.attachment==0xffffffff&&mario.request.position[1]==18&&mario.request.radius==9);
 assert(mario.calculation()==std::vector<std::string>({r(M+0x1bc),r(Twice),r(Thirds),"mul","div",r(Zero),"div"}));
 F npc;npc.words[A]=1;npc.words[A+8]=Me;npc.compareReady=false;assert(!npc.run());npc.words[E+0x170]=M;npc.words[M+0x150]=bits(24);npc.compareReady=true;assert(npc.run()==2&&npc.npcs==0&&npc.request.attachment==M&&npc.request.position[1]==16&&npc.request.radius==8);
 F named;named.words[A]=1;assert(named.run()==2&&named.npcs==1&&named.request.attachment==N&&named.request.position[1]==12);
 assert(named.calculation()==std::vector<std::string>({r(Twice),r(N+0x150),r(Thirds),"mul","div",r(Zero),"div"}));
}
void parties(){
 F party;party.words[A]=3;party.mutateDivisor=true;assert(party.run()==2&&!party.extra&&party.request.attachment==0xfffffffe&&party.request.position[1]==14&&party.request.radius==3.5f);
 assert(party.calculation()==std::vector<std::string>({r(Twice),r(P+0xf0),"kind","mul",r(Thirds),"div",r(Thirds),r(Zero),"div"}));
 F special;special.words[A]=3;special.kind=5;assert(special.run()==2&&special.request.position[1]==30&&special.request.radius==7);
 F extra;extra.words[A]=4;extra.kind=5;extra.mutateDivisor=true;assert(extra.run()==2&&extra.extra&&extra.request.position[1]==14&&extra.request.radius==7&&extra.request.attachment==0xfffffffd);
 assert(extra.calculation()==std::vector<std::string>({r(P+0xf0),r(Twice),r(Thirds),"mul","div",r(Zero),"div"}));
 F missing;missing.words[A]=3;missing.party=0;missing.words[A+4]=Name;assert(missing.run()==2&&missing.constructs==0&&missing.compares==1);
}
void names(){F f;f.words[A+4]=Name;f.nameReady=false;assert(!f.run()&&f.c.pending()==0x8005ebec);assert(f.constructs==1&&f.compares==1&&f.names==1);f.words[A]=99;f.nameReady=true;assert(f.run()==2&&f.constructs==1&&f.compares==1&&f.names==2);
 F invalid;invalid.words[A]=uint32_t(-1);invalid.words[A+4]=Name;assert(invalid.run()==2&&invalid.decoded.size()==11&&invalid.constructs==0&&invalid.compares==1&&invalid.names==0);
 F null;null.effect=0;null.words[A+4]=Name;assert(null.run()==2&&null.constructs==1&&null.compares==1&&null.names==0);
}
void actualVm(){
 EventNativePlatform platform;platform.truncate=[](void*,float f){return int32_t(f);};EventNativeRuntime vm{0x81000000,platform};
 assert(vm.memory.registerRam(A,std::vector<uint8_t>(44)));EventEntry e;assert(vm.entryAddresses.registerEntries(0x85000000,&e,1));e.id=17;e.arguments=A;e.argumentCount=11;
 assert(vm.memory.writeWord(A,2));assert(vm.memory.writeWord(A+12,1));assert(vm.memory.writeWord(A+20,uint32_t(-229998464))); // Original encoded1.5.
 assert(vm.memory.writeWord(A+24,7));assert(vm.memory.writeWord(A+40,30));
 F f;f.ready=false;NativeFukidashiEventCall call;const auto world=static_cast<FukidashiEventWorldServices>(f.s);int32_t result=99;
 assert(!call.step(e,vm,world,result)&&result==99&&call.pending()==0x80133164);assert(f.request.position[0]==1.5f&&f.request.position[1]==7&&f.request.duration==30);
 const auto before=f.constructs;++e.currentCommand;assert(!call.step(e,vm,world,result)&&f.constructs==before);--e.currentCommand;
 assert(vm.memory.writeWord(A+20,9));f.ready=true;assert(call.step(e,vm,world,result)&&result==2&&f.request.position[0]==1.5f);const auto count=f.constructs;
 assert(call.step(e,vm,world,result)&&f.constructs==count);++e.id;assert(!call.step(e,vm,world,result));--e.id;
 assert(vm.entryAddresses.unregisterEntries(0x85000000));assert(vm.entryAddresses.registerEntries(0x85001000,&e,1));
 assert(!call.step(e,vm,world,result));
}
}
int main(){fixedAndCapture();rawCaptureAndOwnership();actors();parties();names();actualVm();}

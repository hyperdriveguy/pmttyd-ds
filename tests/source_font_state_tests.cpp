#include "source_font_state.hpp"
#include <cstdlib>
#include <iostream>
#include <map>
#include <functional>
#include <string>
using namespace ttyd;
namespace {
void checkImpl(bool b,int line){if(!b){std::cerr<<"font state check failed at "<<line<<"\n";std::abort();}}
#define check(...) checkImpl((__VA_ARGS__),__LINE__)
struct Fixture {
 std::map<uint32_t,uint8_t> bytes;std::vector<std::string> memory;
 std::vector<SourceFontStateCall> accepted;std::vector<const SourceFontStateCall*> attempted;
 std::shared_ptr<const void> lease=std::make_shared<const uint8_t>(1);
 SourceFontStateCursor cursor;SourceFontStateServices services;SourceFontStateInput input;
 bool current=true,reenter=false,matrixWait=false,matrixHeld=false;
 size_t hold=999;bool held=false;uint32_t retireRead=0;
 std::function<void(Fixture&)> onHold;
 Fixture(){
  services.user=this;services.lifetime=lease;services.actors.user=this;services.actors.memory.user=this;
  services.current=[](void*p){return static_cast<Fixture*>(p)->current;};
  services.actors.memory.word=[](void*p,uint32_t at){auto&f=*static_cast<Fixture*>(p);f.memory.push_back("r"+std::to_string(at));const auto n=f.word(at);if(at==f.retireRead)f.current=false;return n;};
  services.actors.byte=[](void*p,uint32_t at){auto&f=*static_cast<Fixture*>(p);f.memory.push_back("b"+std::to_string(at));return f.bytes.at(at);};
  services.actors.memory.storeWord=[](void*p,uint32_t at,uint32_t n){auto&f=*static_cast<Fixture*>(p);f.memory.push_back("w"+std::to_string(at));f.put(at,n);};
  services.actors.memory.storeByte=[](void*p,uint32_t at,uint8_t n){auto&f=*static_cast<Fixture*>(p);f.memory.push_back("s"+std::to_string(at));f.bytes[at]=n;};
  services.request=[](void*p,const SourceFontStateCall&q){auto&f=*static_cast<Fixture*>(p);f.attempted.push_back(&q);
   if(f.reenter){check(!f.cursor.reset());check(!resumeSourceFontState(f.input,f.services,f.cursor));}
   if(!f.held&&f.accepted.size()==f.hold){f.held=true;if(f.onHold)f.onHold(f);return false;}
   f.accepted.push_back(q);return true;
  };
  services.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&f=*static_cast<Fixture*>(p);check(q.function==0x8029e2b0);check(q.xyz==std::array<float,3>{1.f/64,1.f/64,1});
   if(f.matrixWait&&!f.matrixHeld){f.matrixHeld=true;f.put(0x80420438,0x40000000);return false;}
   q.output={};q.output[0]=q.xyz[0];q.output[5]=q.xyz[1];q.output[10]=q.xyz[2];return true;};
  put(0x8041e9a4,1);put(0x8041e9a8,0x80000000);put(0x8041e9ac,0x12345680);
  put(0x80416010,0x1e1414ff);put(0x80416014,0xdcdcdcff);put(0x80416018,0xb47814ff);put(0x8041600c,0xabcdefa0);
  for(uint32_t a: {0x802c2b6cu,0x802c2b70u,0x802c2b74u})put(a,0x3f800000);
  put(0x8041ea24,0x81701000);put(0x80420438,0x3c800000);put(0x804203e4,0x3f800000);
 }
 void put(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(n>>(24-8*i));}
 uint32_t word(uint32_t a)const{uint32_t n=0;for(unsigned i=0;i<4;++i)n=n<<8|bytes.at(a+i);return n;}
 std::optional<uint32_t> run(uint32_t function,uint32_t arg=0,uint32_t bits=0,uint32_t*color=nullptr){input={function,arg,bits,color};return resumeSourceFontState(input,services,cursor);}
};
void colorAndAlias(){
 Fixture f;check(f.run(0x80077774).has_value());check(f.word(0x8041e9ac)==0x12345680);check(f.accepted.back().color==0x12345640);
 check(f.run(0x80077844,1).has_value());check(f.word(0x8041e9ac)==0xdcdcdcff);check(f.accepted.back().color==0xdcdcdc80);
 check(f.run(0x80077844,0xffffffff).has_value());check(f.word(0x8041e9ac)==0xabcdefa0);check(f.accepted.back().color==0xabcdef50);
 f.put(0x1000,0x77889980);check(f.run(0x800777e0,0x1000).has_value());check(f.word(0x1000)==0x77889940&&f.word(0x8041e9ac)==0x77889980);
 check(f.run(0x800777e0,0x1000).has_value());check(f.word(0x1000)==0x77889920&&f.word(0x8041e9ac)==0x77889940);
 f.put(0x8041e9ac,0x11223380);check(f.run(0x800777e0,0x8041e9ac).has_value());check(f.word(0x8041e9ac)==0x11223340);
 uint32_t local=0xaabbcc80;check(f.run(0x800777e0,0,0,&local).has_value());check(local==0xaabbcc40&&f.word(0x8041e9ac)==0xaabbcc80);
 for(unsigned a=0;a<256;++a)for(unsigned b=0;b<256;++b){f.bytes[0x8041e9a8]=uint8_t(b);local=0x12345600|a;check(f.run(0x800777e0,0,0,&local).has_value());check((local&255)==a*b/255);f.accepted.clear();f.memory.clear();f.attempted.clear();}
}
void resetsAndBits(){
 Fixture f;check(f.run(0x80077758,0,0x7fc12345).has_value());for(auto a:{0x8041e9b0u,0x8041e9b4u,0x8041e9b8u})check(f.word(a)==0x7fc12345);
 f.put(0x8041e9ac,0x80000000);f.put(0x8041e9b0,0x3f123456);f.put(0x8041e9b4,0x7f800000);
 check(f.run(0x8007773c,0x8041e9ac).has_value());check(f.word(0x8041e9b0)==0x80000000&&f.word(0x8041e9b4)==0x3f123456&&f.word(0x8041e9b8)==0x7f800000);
 check(f.run(0x8007776c)==0x80000000);check(f.run(0x80077aa8).has_value()&&f.word(0x8041e9a4)==0);check(f.run(0x80077ab4).has_value()&&f.word(0x8041e9a4)==1);
 f.memory.clear();check(f.run(0x80077ac0,0x1234).has_value());check(f.bytes.at(0x8041e9a8)==0x34&&f.word(0x8041e9a4)==0&&f.word(0x8041e9ac)==0x1e1414ff);
 check(f.accepted.back().function==0x80074bfc&&f.accepted.back().color==0x1e141434);
 std::vector<std::string> expected;for(auto a:{0x80416010u,0x802c2b6cu,0x802c2b70u,0x802c2b74u})expected.push_back("r"+std::to_string(a));expected.push_back("s"+std::to_string(0x8041e9a8u));for(auto a:{0x8041e9b4u,0x8041e9b8u,0x8041e9a4u,0x8041e9acu,0x8041e9b0u})expected.push_back("w"+std::to_string(a));check(f.memory==expected);
 check(f.run(0x80077b4c).has_value());check(f.bytes.at(0x8041e9a8)==255&&f.accepted.back().color==0x1e1414ff);
}
void heldCalls(){
 const std::array<std::pair<uint32_t,size_t>,8> cases{{{0x80077774,1},{0x800777e0,1},{0x80077844,1},{0x80077ac0,1},{0x80077b4c,1},{0x800778b4,2},{0x800779e4,1},{0x80077a08,6}}};
 for(auto [function,count]:cases)for(size_t hold=0;hold<count;++hold){Fixture f;f.put(0x1000,0x55667780);f.hold=hold;f.reenter=true;check(!f.run(function,function==0x800777e0?0x1000:0));const auto mutations=f.memory.size();check(f.cursor.active());auto wrong=f.services;wrong.user=nullptr;check(!resumeSourceFontState(f.input,wrong,f.cursor));check(resumeSourceFontState(f.input,f.services,f.cursor).has_value());check(f.accepted.size()==count&&f.memory.size()==mutations&&!f.cursor.active());check(f.attempted[hold]==f.attempted[hold+1]);}
 for(size_t hold=0;hold<11;++hold){Fixture f;f.hold=hold;f.onHold=[](Fixture&x){if(x.accepted.size()==8)x.put(0x8041ea24,0x12340000);};check(!f.run(0x800778e0));check(resumeSourceFontState(f.input,f.services,f.cursor).has_value());check(f.accepted.size()==11);check(f.accepted[8].integers==std::vector<uint32_t>({0x81701640,2}));check(f.accepted[9].matrix&&(*f.accepted[9].matrix)[0]==1.f/64);check(f.accepted[10].integers==std::vector<uint32_t>({1,1,5,33,0,125}));}
 Fixture f;f.matrixWait=true;check(!f.run(0x800778e0));check(f.cursor.pending()==0x8029e2b0);check(resumeSourceFontState(f.input,f.services,f.cursor).has_value());check(f.accepted.size()==11);
 Fixture absent;absent.services.request=nullptr;check(!absent.run(0x800777e0,0x8041e9ac));check(absent.word(0x8041e9ac)==0x12345640);check(!resumeSourceFontState(absent.input,absent.services,absent.cursor));check(absent.word(0x8041e9ac)==0x12345640);
}
void stale(){Fixture f;f.hold=0;check(!f.run(0x80077b4c));f.current=false;check(!resumeSourceFontState(f.input,f.services,f.cursor));check(f.cursor.stale());f.current=true;check(!resumeSourceFontState(f.input,f.services,f.cursor));check(f.cursor.reset());check(f.run(0x80077b4c).has_value());
 Fixture mid;mid.retireRead=0x80416010;check(!mid.run(0x80077b4c));check(mid.cursor.stale()&&mid.accepted.empty());check(!mid.run(0xdeadbeef));}
}
int main(){colorAndAlias();resetsAndBits();heldCalls();stale();}

#include "file_acquisition.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
  static constexpr uint32_t M=0x1000,E=0x2000,Next=0x20b0,P=0x7000,
      Payload=0x8000,D=0x9000,H=0xa000,Destination=0xb000;
  std::map<uint32_t,uint8_t> memory;
  std::vector<std::string> calls;
  std::vector<std::pair<unsigned,uint32_t>> reads;
  std::function<void(uint32_t,uint32_t)> stored;
  std::string held;
  std::vector<uint32_t> archiveResults{Payload};
  unsigned archiveCalls=0,yields=0,collections=0;
  uint32_t disc=D,length=31,header=H,readResult=0,requestedLength=0,relocated=0;
  bool pendingClears=true,copyChangesHeader=false;
  void region(uint32_t a,unsigned n,uint8_t fill=0){for(unsigned i=0;i<n;++i)memory[a+i]=fill;}
  uint8_t byte(uint32_t a){assert(memory.count(a));return memory[a];}
  uint16_t half(uint32_t a){return uint16_t(byte(a)<<8|byte(a+1));}
  uint32_t word(uint32_t a){return (uint32_t(half(a))<<16)|half(a+2);}
  void half(uint32_t a,uint16_t v){assert(memory.count(a)&&memory.count(a+1));memory[a]=v>>8;memory[a+1]=v;}
  void word(uint32_t a,uint32_t v){half(a,uint16_t(v>>16));half(a+2,uint16_t(v));}
  void string(uint32_t a,const std::string& value){for(unsigned i=0;i<=value.size();++i){assert(memory.count(a+i));memory[a+i]=i==value.size()?0:uint8_t(value[i]);}}
  std::string string(uint32_t a){std::string result;for(;byte(a);++a)result+=char(byte(a));return result;}
  bool ready(const std::string& name){calls.push_back(name);return held!=name;}
  Fixture(){
    region(0x80418498,4);region(M,0x100);region(E,0x160,0xa5);region(P,128);
    region(H,16);region(D,0x100);region(0xa4,4);region(0,16);
    word(0x80418498,M);word(M+0x10,E);word(M+0x14,E);word(E+0xa4,Next);
    word(H,Destination);word(4,Destination);string(P,"./battle/common/battle_common.tpl");
  }
  FileAcquisitionServices services(){
    FileAcquisitionServices s;s.user=this;s.memory.user=this;
    s.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back({4,a});return f.word(a);};
    s.memory.half=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back({2,a});return f.half(a);};
    s.byte=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back({1,a});return f.byte(a);};
    s.memory.storeWord=[](void* p,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(p);f.word(a,n);if(f.stored)f.stored(a,n);};
    s.memory.storeHalf=[](void* p,uint32_t a,uint16_t n){static_cast<Fixture*>(p)->half(a,n);};
    s.memory.storeByte=[](void* p,uint32_t a,uint8_t n){auto& f=*static_cast<Fixture*>(p);assert(f.memory.count(a));f.memory[a]=n;};
    s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.ready("compare"))return {};return f.string(a)==f.string(b)?0:1;};
    s.copy=[](void* p,uint32_t a,uint32_t b){auto& f=*static_cast<Fixture*>(p);if(!f.ready("copy"))return false;f.string(a,f.string(b));if(f.copyChangesHeader)f.word(H,0xd000);return true;};
    s.yield=[](void* p){auto& f=*static_cast<Fixture*>(p);if(!f.ready("yield"))return false;++f.yields;if(f.pendingClears)f.word(E+0xac,0);return true;};
    s.garbage=[](void* p,int32_t mode){auto& f=*static_cast<Fixture*>(p);assert(mode==0 || mode==1);if(!f.ready(mode?"collect1":"collect0"))return false;++f.collections;if(!mode)f.word(M+0x10,E);return true;};
    s.archive=[](void* p,uint32_t a,uint32_t out,uint32_t length)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(a==P && !out && !length);if(!f.ready("archive"))return {};const auto i=f.archiveCalls++;return i<f.archiveResults.size()?f.archiveResults[i]:0;};
    s.relocate=[](void* p,uint32_t payload,uint32_t type){auto& f=*static_cast<Fixture*>(p);assert(type==0x104);if(!f.ready("relocate"))return false;f.relocated=payload;return true;};
    s.open=[](void* p,uint32_t a,int32_t mode,uint32_t zero)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(a==P && mode==2 && !zero);if(!f.ready("open"))return {};return f.disc;};
    s.length=[](void* p,uint32_t d)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(d==D);if(!f.ready("length"))return {};return f.length;};
    s.allocate=[](void* p,uint32_t n,uint32_t zero)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(!zero);if(!f.ready("allocate"))return {};f.requestedLength=n;return f.header;};
    s.read=[](void* p,uint32_t d,uint32_t destination,uint32_t n,uint32_t offset)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(d==D && destination==Destination && n==f.requestedLength && !offset);if(!f.ready("read"))return {};assert(f.word(f.header+8)==E);f.word(H,0xc000);return f.readResult;};
    s.close=[](void* p,uint32_t d){auto& f=*static_cast<Fixture*>(p);assert(d==D);return f.ready("close");};
    return s;
  }
  uint32_t run(){auto s=services();FileAcquisitionCursor c;uint32_t result=99;assert(acquirePackedFile(P,0x104,s,c,result));assert(!c.phase);return result;}
};
void cache(){
  for(unsigned state:{1u,2u,3u,255u})for(uint16_t count:{uint16_t(0),uint16_t(1),uint16_t(32767),uint16_t(65535)}){
    Fixture f;f.word(Fixture::M+8,Fixture::E);f.memory[Fixture::E]=state;
    f.string(Fixture::E+0x20,f.string(Fixture::P));f.word(Fixture::E+0xac,0);f.half(Fixture::E+2,count);
    assert(f.run()==Fixture::E && f.calls==std::vector<std::string>{"compare"});
    assert(f.half(Fixture::E+2)==(state==3?1:uint16_t(count+1)));
    assert(f.byte(Fixture::E)==(state==2 || state==3?1:state));
    assert(f.byte(Fixture::E+1)==0xa5); // Requested type does not replace cache type.
  }
  Fixture f;f.word(Fixture::M+8,Fixture::E);f.memory[Fixture::E]=0;
  f.region(Fixture::Next,0xb0);f.memory[Fixture::Next]=1;
  f.string(Fixture::Next+0x20,f.string(Fixture::P));f.word(Fixture::Next+0xa4,0);f.half(Fixture::Next+2,2);
  assert(f.run()==Fixture::Next && f.calls==std::vector<std::string>{"compare"});
  Fixture pending;pending.word(Fixture::M+8,Fixture::E);pending.memory[Fixture::E]=3;
  pending.string(Fixture::E+0x20,pending.string(Fixture::P));pending.word(Fixture::E+0xac,9);
  auto s=pending.services();FileAcquisitionCursor c;uint32_t out=17;pending.held="yield";
  assert(!acquirePackedFile(Fixture::P,0x104,s,c,out));const auto before=pending.memory;
  assert(!acquirePackedFile(Fixture::P,0x104,s,c,out) && before==pending.memory && out==17);
  pending.held.clear();assert(acquirePackedFile(Fixture::P,0x104,s,c,out) && out==Fixture::E && pending.yields==1);
}
void newEntries(){
  for(uint32_t mode:{0u,1u,2u,3u,0xffffffffu})for(unsigned success=0;success<4;++success){
    Fixture f;f.word(Fixture::M+4,mode);f.archiveResults.assign(success,0);f.archiveResults.push_back(Fixture::Payload);
    assert(f.run()==Fixture::E);const unsigned trials=mode==0?1:(mode==1 || mode==2?3:0);
    const bool archive=success<trials;assert(f.archiveCalls==(archive?success+1:trials));
    assert(f.word(Fixture::E+0xa0)==(archive?Fixture::E+4:Fixture::H));
    if(archive)assert(f.word(Fixture::E+4)==Fixture::Payload);
    assert(f.byte(Fixture::E)==1 && f.byte(Fixture::E+1)==4 && f.half(Fixture::E+2)==1);
    assert(f.word(Fixture::E+0xac)==0 && f.word(Fixture::E+0xa4)==0);
    assert(f.string(Fixture::E+0x20)==f.string(Fixture::P));
    assert(f.word(Fixture::M+0x10)==Fixture::Next && f.word(Fixture::M+8)==Fixture::E && f.word(Fixture::M+0xc)==Fixture::E);
    assert(f.byte(Fixture::E+0x18)==0xa5); // Unclassified bytes remain untouched.
    if(archive)assert(f.calls[f.calls.size()-2]=="relocate" && f.calls.back()=="copy");
    else assert(f.calls[f.calls.size()-2]=="copy" && f.calls.back()=="relocate" && f.relocated==0xc000);
  }
  Fixture fresh;fresh.word(Fixture::M+4,3);fresh.copyChangesHeader=true;fresh.header=0;
  fresh.word(0,Fixture::Destination);assert(fresh.run()==Fixture::E && fresh.relocated==Fixture::Destination);
  assert(fresh.word(8)==Fixture::E); // Original has no smartAlloc-null guard.
  for(uint32_t n:{0u,1u,31u,32u,0xffffffe0u,0xffffffe1u,0xffffffffu}){
    Fixture f;f.word(Fixture::M+4,3);f.length=n;const auto aligned=(n+31u)&~31u;
    const auto result=f.run();assert(result==(aligned?Fixture::E:0));
    if(aligned)assert(f.requestedLength==aligned);
    else assert(f.word(Fixture::M+0x10)==Fixture::E && f.calls.back()=="close");
  }
  Fixture fail;fail.word(Fixture::M+4,3);fail.disc=0;assert(!fail.run());
  assert(fail.word(Fixture::M+0x10)==Fixture::E && fail.calls==std::vector<std::string>{"open"});
  Fixture collect;collect.word(Fixture::M+0x10,0);assert(collect.run()==Fixture::E && collect.collections==2);
}
void waits(){
  for(const std::string hold:{"archive","relocate","copy","open","length","allocate","read","close","collect1","collect0","compare"}){
    Fixture f;if(hold=="compare"){f.word(Fixture::M+8,Fixture::E);f.memory[Fixture::E]=1;f.word(Fixture::E+0xac,0);f.string(Fixture::E+0x20,f.string(Fixture::P));}
    if(hold=="open" || hold=="length" || hold=="allocate" || hold=="read" || hold=="close")f.word(Fixture::M+4,3);
    if(hold=="collect1" || hold=="collect0")f.word(Fixture::M+0x10,0);
    f.held=hold;auto s=f.services();FileAcquisitionCursor c;uint32_t result=19;
    assert(!acquirePackedFile(Fixture::P,0x104,s,c,result));const auto bytes=f.memory;const auto reads=f.reads;
    assert(!acquirePackedFile(Fixture::P,0x104,s,c,result) && bytes==f.memory && reads==f.reads && result==19);
    auto changed=s;changed.user=nullptr;assert(!acquirePackedFile(Fixture::P,0x104,changed,c,result));
    assert(!acquirePackedFile(Fixture::P,7,s,c,result));
    f.held.clear();assert(acquirePackedFile(Fixture::P,0x104,s,c,result) && result==Fixture::E);
  }
}
void aliases(){
  Fixture f;for(uint32_t m:{0x3000u,0x4000u,0x5000u})f.region(m,24);
  f.region(0x6000,0xb0);f.word(0x4008,0x6000);f.word(0x400c,0x6000);
  f.stored=[&](uint32_t a,uint32_t){
    if(a==Fixture::M+0x10)f.word(0x80418498,0x3000);
    if(a==0x3014)f.word(0x80418498,0x4000);
    if(a==0x60a4)f.word(0x80418498,0x5000);
  };
  assert(f.run()==Fixture::E);
  assert(f.word(Fixture::M+0x10)==Fixture::Next && f.word(0x3014)==0);
  assert(f.word(0x60a4)==Fixture::E && f.word(0x500c)==Fixture::E && !f.word(Fixture::M+8));
}
}
int main(){cache();newEntries();waits();aliases();}

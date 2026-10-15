#include "file_async.hpp"
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
FileAsyncServices async(Fixture& f){FileAsyncServices s;s.file=f.services();s.submit=[](void* p,uint32_t d,uint32_t to,uint32_t n,uint32_t off,uint32_t cb){auto& f=*static_cast<Fixture*>(p);assert(d==Fixture::D && to==Fixture::Destination && n==32 && !off && cb==0x8018a1a0);assert(f.word(Fixture::M+8)==Fixture::E && f.word(Fixture::E+0xac)==d && f.half(Fixture::E+2)==0);if(!f.ready("submit"))return false;submitPackedDvdRead(d,to,n,off,cb,f.services().memory);return true;};return s;}
void hits(){
 for(unsigned state:{1u,2u,3u,255u})for(unsigned count:{0u,7u,65535u})for(bool pending:{false,true}){
  Fixture f;f.word(Fixture::M+8,Fixture::E);f.memory[Fixture::E]=state;f.word(Fixture::E+0xac,pending?Fixture::D:0);f.half(Fixture::E+2,count);f.string(Fixture::E+0x20,f.string(Fixture::P));const auto before=f.memory;
  auto s=async(f);FileAsyncCursor c;uint32_t result=99;assert(requestPackedFile(Fixture::P,999,0x777,s,c,result));assert(result==(pending?0:Fixture::E));assert(f.half(Fixture::E+2)==count && f.byte(Fixture::E+1)==0xa5 && f.word(Fixture::E+0xa8)==0xa5a5a5a5);auto expected=before;if(!pending && state==2)expected[Fixture::E]=3;assert(f.memory==expected && f.calls==std::vector<std::string>{"compare"});
 }
}
void misses(){
 {Fixture f;auto s=async(f);FileAsyncCursor c;uint32_t v=99;assert(requestPackedFile(Fixture::P,0x104,0x555,s,c,v) && v==Fixture::E);assert(f.byte(Fixture::E)==3 && !f.half(Fixture::E+2) && !f.word(Fixture::E+0xac) && f.word(Fixture::E+0xa8)==0xa5a5a5a5);assert(!f.word(Fixture::M+0x10) && !f.word(Fixture::M+0x14));} // Archive pops live cleared E.next, never captured Next.
 for(const char* held:{"archive","open","length","close","allocate","copy","submit"}){
  Fixture f;f.archiveResults={0};auto s=async(f);FileAsyncCursor c;uint32_t v=99;f.held=held;assert(!requestPackedFile(Fixture::P,0x104,0x555,s,c,v));const auto before=f.memory;assert(!requestPackedFile(Fixture::P,0x104,0x555,s,c,v) && f.memory==before && v==99);assert(!requestPackedFile(Fixture::P,0x104,0x556,s,c,v) && f.memory==before);f.held.clear();assert(requestPackedFile(Fixture::P,0x104,0x555,s,c,v) && v==0);assert(f.word(Fixture::M+0x10)==Fixture::Next && f.word(Fixture::H+8)==Fixture::E && f.byte(Fixture::E)==3 && !f.half(Fixture::E+2));assert(f.word(Fixture::E+0xa8)==0x555 && f.word(Fixture::D+0x7c)==Fixture::Destination && f.half(Fixture::D+0x90)==1);
 }
 {Fixture f;f.archiveResults={0};f.disc=0;auto s=async(f);FileAsyncCursor c;uint32_t v=99;assert(requestPackedFile(Fixture::P,0x104,0,s,c,v) && v==0xffffffff && f.word(Fixture::M+0x10)==Fixture::E && !f.word(Fixture::M+8));}
 {Fixture f;f.archiveResults={0};f.length=0;auto s=async(f);FileAsyncCursor c;uint32_t v=99;assert(requestPackedFile(Fixture::P,0x104,0,s,c,v) && v==0 && !f.word(Fixture::M+8));}
}
void retainedSubmitArguments(){
 Fixture f;f.archiveResults={0};auto s=async(f);
 s.submit=[](void* p,uint32_t d,uint32_t to,uint32_t n,uint32_t off,uint32_t cb){auto& f=*static_cast<Fixture*>(p);assert(d==Fixture::D && to==Fixture::Destination && n==32 && !off && cb==0x8018a1a0);if(!f.ready("submit"))return false;submitPackedDvdRead(d,to,n,off,cb,f.services().memory);return true;};
 FileAsyncCursor c;uint32_t v=99;f.held="submit";assert(!requestPackedFile(Fixture::P,0x104,0,s,c,v));f.word(Fixture::E+0xac,0x1234);f.word(Fixture::H,0x5678);f.held.clear();assert(requestPackedFile(Fixture::P,0x104,0,s,c,v) && v==0);assert(f.word(Fixture::D+0x7c)==Fixture::Destination && f.word(Fixture::E+0xac)==0x1234 && f.word(Fixture::H)==0x5678);
}
void completions(){
 for(uint32_t result:{0u,1u,0xffffffffu,0xfffffffeu,0xfffffffdu}){
  Fixture f;f.word(Fixture::M+8,Fixture::E);f.word(Fixture::E+0xac,Fixture::D);f.word(Fixture::E+0xa0,Fixture::H);f.word(Fixture::E+0xa8,0x111);f.memory[Fixture::E]=3;f.memory[Fixture::E+1]=0xff;f.half(Fixture::E+2,17);auto s=async(f);
  s.file.relocate=[](void* p,uint32_t a,uint32_t t){auto& f=*static_cast<Fixture*>(p);assert(a==Fixture::Destination && t==0xffffffff && !f.word(Fixture::E+0xac));if(!f.ready("relocate"))return false;f.word(Fixture::E+0xa8,0x222);return true;};
  s.callback=[](void* p,uint32_t fn,uint32_t e){auto& f=*static_cast<Fixture*>(p);assert(fn==0x222 && e==Fixture::E && !f.word(e+0xac));return f.ready("callback");};
  FileAsyncCompletionCursor c;f.held=result==0xfffffffd?"close":"relocate";assert(!completePackedFile(result,Fixture::D+0x40,s,c));assert(!f.word(Fixture::E+0xac));const auto before=f.memory;assert(!completePackedFile(result,Fixture::D+0x40,s,c) && f.memory==before);assert(!completePackedFile(result,Fixture::D+0x44,s,c));f.held.clear();assert(completePackedFile(result,Fixture::D+0x40,s,c));assert(f.half(Fixture::E+2)==17 && f.word(Fixture::E+0xa0)==Fixture::H);assert(f.byte(Fixture::E)==(result==0xfffffffd?2:3) && f.byte(Fixture::E+1)==(result==0xfffffffd?0xb:0xff));
 }
 // Separate flag reloads and preservation of deferred-close bit.
 Fixture f;auto m=f.services().memory;f.half(Fixture::D+0x90,0xa);unsigned stores=0;f.stored=[&](uint32_t a,uint32_t){if(a==Fixture::D+0x88)++stores;};submitPackedDvdRead(Fixture::D,1,2,3,4,m);assert(f.half(Fixture::D+0x90)==9 && f.word(Fixture::D+0x8c)==4 && stores==1);
}
}
int main(){hits();misses();retainedSubmitArguments();completions();}

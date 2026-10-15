#include "file_handle_release.hpp"
#include <array>
#include <cassert>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
  uint32_t admission=0x2000;uint8_t state=1;uint16_t count=1;
  std::vector<unsigned> calls;bool mutateReload=false;
  FileHandleReleaseServices services(){
    FileHandleReleaseServices s;s.memory.user=this;
    s.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);assert(a==0x10a0);f.calls.push_back(1);return f.admission;};
    s.byte=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);assert(a==0x1000);f.calls.push_back(2);return f.state;};
    s.memory.half=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);assert(a==0x1002);f.calls.push_back(3);return f.count;};
    s.memory.storeHalf=[](void* p,uint32_t a,uint16_t n){auto& f=*static_cast<Fixture*>(p);assert(a==0x1002);f.calls.push_back(4);f.count=f.mutateReload?0:n;};
    s.memory.storeByte=[](void* p,uint32_t a,uint8_t n){auto& f=*static_cast<Fixture*>(p);assert(a==0x1000 && n==2);f.calls.push_back(5);f.state=n;};
    return s;
  }
};
}
int main(){
  Fixture f;auto s=f.services();releasePackedFileHandle(0,s);assert(f.calls.empty());
  f.admission=0;releasePackedFileHandle(0x1000,s);assert((f.calls==std::vector<unsigned>{1}));
  f.admission=0x2000;
  for(unsigned state=0;state<256;++state)if(state!=1){f.state=state;f.calls.clear();releasePackedFileHandle(0x1000,s);assert((f.calls==std::vector<unsigned>{1,2}));}
  for(uint16_t count:std::array<uint16_t,6>{2,1,0,65535,32768,32767}){
    f.state=1;f.count=count;f.calls.clear();releasePackedFileHandle(0x1000,s);
    assert(f.count==uint16_t(count-1));assert(f.state==(count==1?2:1));
    auto expected=std::vector<unsigned>{1,2,3,4,3};if(count==1)expected.push_back(5);assert(f.calls==expected);
  }
  f.state=1;f.count=15;f.mutateReload=true;releasePackedFileHandle(0x1000,s);assert(f.state==2);
  f.calls.clear();releasePackedFileHandle(0x1000,s);assert((f.calls==std::vector<unsigned>{1,2}));
  assert(f.admission==0x2000); // No holder clear or deallocator exists here.
}

#include "file_pool.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
  static constexpr uint32_t M=0x1000,Pool=0x10000;
  std::map<uint32_t,uint8_t> bytes;
  std::vector<std::pair<uint32_t,uint32_t>> stores;
  std::function<void(uint32_t,uint32_t)> afterStore;
  bool allocateReady=false,clearReady=false;
  unsigned allocations=0,clears=0;
  uint32_t allocated=Pool,cleared=0;
  void region(uint32_t a,unsigned n,uint8_t v=0xa5){for(unsigned i=0;i<n;++i)bytes[a+i]=v;}
  uint32_t word(uint32_t a){uint32_t n=0;for(unsigned i=0;i<4;++i){assert(bytes.count(a+i));n=(n<<8)|bytes[a+i];}return n;}
  void word(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i){assert(bytes.count(a+i));bytes[a+i]=uint8_t(n>>(24-8*i));}}
  Fixture(){region(0x80418498,4);region(M,32);region(Pool,0x2c000);word(0x80418498,M);}
  FilePoolServices services(){
    FilePoolServices s;s.user=this;s.memory.user=this;
    s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
    s.memory.storeWord=[](void* p,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(p);f.stores.push_back({a,n});f.word(a,n);if(f.afterStore)f.afterStore(a,n);};
    s.allocate=[](void* p,int32_t heap,uint32_t n)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(heap==0 && n==0x2c000);if(!f.allocateReady)return {}; ++f.allocations;return f.allocated;};
    s.clear=[](void* p,uint32_t a,int32_t fill,uint32_t n){auto& f=*static_cast<Fixture*>(p);assert(!fill && n==0x2c000);if(!f.clearReady)return false;++f.clears;f.cleared=a;for(unsigned i=0;i<n;++i){assert(f.bytes.count(a+i));f.bytes[a+i]=0;}return true;};
    return s;
  }
};
void stable(){
  Fixture f;auto s=f.services();FilePoolCursor c;
  assert(!initializePackedFilePool(s,c) && f.stores.empty());
  f.allocateReady=true;assert(!initializePackedFilePool(s,c));
  assert(f.stores.size()==4 && f.allocations==1);
  const auto stored=f.stores;assert(!initializePackedFilePool(s,c) && f.stores==stored && f.allocations==1);
  auto changed=s;changed.user=nullptr;assert(!initializePackedFilePool(changed,c));
  f.clearReady=true;assert(initializePackedFilePool(s,c) && !c.phase && f.allocations==1 && f.clears==1);
  assert(f.word(Fixture::M)==Fixture::Pool && !f.word(Fixture::M+4) && !f.word(Fixture::M+8) && !f.word(Fixture::M+12));
  assert(f.word(Fixture::M+16)==Fixture::Pool && f.word(Fixture::M+20)==Fixture::Pool+0x2bf50);
  assert(f.stores.size()==4+1024+3);
  for(unsigned i=0;i<1024;++i){
    const auto a=Fixture::Pool+i*0xb0;
    assert(f.stores[4+i]==std::make_pair(a+0xa4,a+0xb0)); // Last temporary link is one-past.
    assert(f.word(a+0xa4)==(i==1023?0:a+0xb0));
    for(unsigned j=0;j<0xb0;++j)if(j<0xa4 || j>=0xa8)assert(f.bytes[a+j]==0);
  }
  assert(f.bytes[Fixture::M+24]==0xa5); // No manager-wide clear.
  for(uint32_t mode:{0u,1u,2u,3u,0xffffffffu}){setPackedFileArchiveType(mode,s.memory);assert(f.word(Fixture::M+4)==mode);}
  assert(initializePackedFilePool(s,c) && f.allocations==2 && f.clears==2); // No local old-pool free.
}
void aliases(){
  Fixture f;constexpr uint32_t OtherM=0x2000,OtherPool=0x50000;
  f.region(OtherM,32);f.region(OtherPool,0x2c000);f.word(OtherM,OtherPool);
  f.afterStore=[&](uint32_t a,uint32_t){if(a==Fixture::M)f.word(0x80418498,OtherM);};
  f.allocateReady=f.clearReady=true;auto s=f.services();FilePoolCursor c;
  assert(initializePackedFilePool(s,c));
  assert(f.word(Fixture::M)==Fixture::Pool && f.cleared==OtherPool);
  assert(f.bytes[Fixture::Pool]==0xa5 && !f.bytes[OtherPool]);
  assert(f.word(OtherM+16)==OtherPool && f.word(OtherM+20)==OtherPool+0x2bf50);
  assert(!f.word(OtherPool+0x2bf50+0xa4));
}
}
int main(){stable();aliases();}

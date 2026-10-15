#include "file_pool.hpp"
namespace ttyd {
bool initializePackedFilePool(const FilePoolServices& s,FilePoolCursor& c){
  const auto& m=s.memory;
  if(c.phase && (c.owner.user!=s.user || c.owner.allocate!=s.allocate ||
      c.owner.clear!=s.clear || c.owner.memory.user!=m.user ||
      c.owner.memory.word!=m.word || c.owner.memory.storeWord!=m.storeWord))return false;
  const auto word=[&](uint32_t a){return m.word(m.user,a);};
  const auto put=[&](uint32_t a,uint32_t n){m.storeWord(m.user,a,n);};
  const auto manager=[&](){return word(0x80418498);};
  if(!c.phase){c={};c.owner=s;c.phase=1;}
  // Heap0 requests0x2c000 =1024 entries *0xb0 bytes. Allocation0 is engaged
  // completion, not nullopt. Manager pool/mode/used head/tail publish before
  // clear can hold; retry must not repeat allocation or those stores.
  if(c.phase==1){
    if(!s.allocate)return false;
    const auto pool=s.allocate(s.user,0,0x2c000);if(!pool)return false;
    put(manager(),*pool);put(manager()+4,0);put(manager()+8,0);put(manager()+0xc,0);
    c.destination=word(manager());c.phase=2;
  }
  if(!s.clear || !s.clear(s.user,c.destination,0,0x2c000))return false;
  auto cursor=word(manager());
  // Link128 groups of8 entries, stride0xb0/group0x580, next word+0xa4.
  // Source briefly stores a one-past-pool final next, then clears it after
  // publishing available head/tail (last entry offset0x2bf50). Keep that order.
  for(unsigned group=0;group<128;++group){
    for(unsigned j=0;j<8;++j)put(cursor+j*0xb0+0xa4,cursor+(j+1)*0xb0);
    cursor+=0x580;
  }
  auto current=manager();put(current+0x10,word(current));
  current=manager();put(current+0x14,word(current)+0x2bf50);
  current=manager();put(word(current+0x14)+0xa4,0);
  c={};return true;
}
void setPackedFileArchiveType(uint32_t type,const EventMemoryServices& m){
  const auto manager=m.word(m.user,0x80418498);
  m.storeWord(m.user,manager+4,type);
}
} // namespace ttyd

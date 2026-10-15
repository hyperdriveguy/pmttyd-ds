#include "battle_sequence_storage.hpp"
namespace ttyd {
namespace {void word(uint8_t* p,uint32_t v){for(unsigned i=0;i<4;++i)p[i]=uint8_t(v>>(24-8*i));}}
BattleSequenceStorage::BattleSequenceStorage(std::shared_ptr<ProloguePouch> p,uint32_t a,uint32_t n,uint32_t q):pouch_(std::move(p)),arenaAddress_(a),pouchAddress_(q),arena_(n){
 word(globals_.data(),a);word(globals_.data()+16,n);word(arena_.data()+4,n-32);word(pouchPointer_.data(),q);
}
std::shared_ptr<BattleSequenceStorage> BattleSequenceStorage::cold(std::shared_ptr<ProloguePouch> p,uint32_t a,uint32_t n,uint32_t q){
 // Policy admission only: source allocation still runs its real best-fit,
 // split/zero/cache operations. Require a terminal header beyond battle payload.
 if(!p || (a&31) || (n&31) || n<0x64040 || uint64_t(a)+n>0x100000000ULL || uint64_t(q)+p->bytes.size()>0x100000000ULL)return {};
 return std::shared_ptr<BattleSequenceStorage>(new BattleSequenceStorage(std::move(p),a,n,q));
}
bool BattleSequenceStorage::canAllocate(uint32_t root,uint32_t bytes,const EventAddressSpace& m) const{
 const uint64_t payload=(uint64_t(bytes)+31)&~uint64_t(31);if(!payload || payload+32>0xffffffff)return false;
 const uint64_t end=uint64_t(arenaAddress_)+arena_.size();bool found=false;
 // Native supported allocator domain: ordered headers entirely in this owner.
 // Inspect every header before any original side effects; do not repair bytes.
 for(uint32_t h=root;;){
  uint32_t next=0,size=0;uint16_t used=0;
  if((h&31) || h<arenaAddress_ || uint64_t(h)+32>end || !m.readWord(h,next) || !m.readWord(h+4,size) || !m.readHalf(h+8,used) || uint64_t(h)+32+size>end)return false;
  if(!used && size>=payload+32)found=true;
  if(!next)return found;
  if(next<=h || uint64_t(next)<uint64_t(h)+32+size)return false;
  h=next;
 }
}
}

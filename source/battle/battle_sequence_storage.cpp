#include "battle_sequence_storage.hpp"
#include <algorithm>
#include "event_ram_access.hpp"
namespace ttyd {
namespace {void word(uint8_t* p,uint32_t v){for(unsigned i=0;i<4;++i)p[i]=uint8_t(v>>(24-8*i));}}
// BE header words: next0, size4, used half8; header32 bytes. Initialize one
// free arena header, publish primary root8041e808/length8041e818 and pouch
// pointer cell. Remaining retained globals/pointers start at native cold zero.
BattleSequenceStorage::BattleSequenceStorage(std::shared_ptr<ProloguePouch> p,uint32_t a,uint32_t n,uint32_t q):pouch_(std::move(p)),arenaAddress_(a),pouchAddress_(q),arena_(n){
 word(globals_.data(),a);word(globals_.data()+16,n);word(arena_.data()+4,n-32);word(pouchPointer_.data(),q);
}
// Native policy: 32-byte aligned arena/base, length>=64040(hex), 32-bit spans.
// Minimum leaves the original64000 battle payload plus allocation/terminal
// headers. This rejects unsupported arenas without claiming source heap identity.
std::shared_ptr<BattleSequenceStorage> BattleSequenceStorage::cold(std::shared_ptr<ProloguePouch> p,uint32_t a,uint32_t n,uint32_t q){
 // Policy admission only: source allocation still runs its real best-fit,
 // split/zero/cache operations. Require a terminal header beyond battle payload.
 if(!p || (a&31) || (n&31) || n<0x64040 || uint64_t(a)+n>0x100000000ULL || uint64_t(q)+p->bytes.size()>0x100000000ULL)return {};
 return std::shared_ptr<BattleSequenceStorage>(new BattleSequenceStorage(std::move(p),a,n,q));
}

namespace {
bool poolBacking(const EventAddressSpace&m,uint32_t address,const uint8_t*bytes,uint32_t size){
 // Only synchronous borrow metadata changes; the facade/source bytes do not.
 return const_cast<EventAddressSpace&>(m).withRawRamAccess([&](EventAddressSpace::RawRamAccess&r){return r.matchesBacking(address,size,bytes);});
}
}
bool BattleSequenceStorage::aliases(const EventAddressSpace&m)const{
 return poolBacking(m,0x8041e808,globals_.data(),20)&&poolBacking(m,0x8041eb00,pouchPointer_.data(),4)&&poolBacking(m,0x8041eb90,battlePointer_.data(),4)&&
  poolBacking(m,arenaAddress_,arena_.data(),uint32_t(arena_.size()))&&poolBacking(m,pouchAddress_,pouch_->bytes.data(),uint32_t(pouch_->bytes.size()));
}
void BattleSequenceStorage::unregisterOwnAliases(EventAddressSpace&m,const std::vector<uint32_t>&mapped)const{
 struct Span{uint32_t address;const uint8_t*bytes;uint32_t size;};
 const Span spans[]={{0x8041e808,globals_.data(),20},{0x8041eb00,pouchPointer_.data(),4},{0x8041eb90,battlePointer_.data(),4},{arenaAddress_,arena_.data(),uint32_t(arena_.size())},{pouchAddress_,pouch_->bytes.data(),uint32_t(pouch_->bytes.size())}};
 for(const auto&s:spans)if(std::find(mapped.begin(),mapped.end(),s.address)!=mapped.end()&&poolBacking(m,s.address,s.bytes,s.size))m.unregisterExternalRam(s.address);
}
bool BattleSequenceStorage::ownsPrimaryAllocation(uint32_t payload,uint32_t bytes,const EventAddressSpace&facade)const{
 if(!aliases(facade))return false;
 uint32_t root=0;if(!facade.readWord(0x8041e808,root)||root!=arenaAddress_)return false;
 const uint64_t end=uint64_t(arenaAddress_)+arena_.size();
 for(uint32_t h=root;;){uint32_t next=0,size=0;uint16_t used=0;
  if((h&31)||h<arenaAddress_||uint64_t(h)+32>end||!facade.readWord(h,next)||!facade.readWord(h+4,size)||!facade.readHalf(h+8,used)||uint64_t(h)+32+size>end)return false;
  if(uint64_t(h)+32==payload)return used!=0&&size>=bytes;
  if(!next||next<=h||uint64_t(next)<uint64_t(h)+32+size)return false;
  h=next;
 }
}
bool BattleSequenceStorage::canAllocate(uint32_t root,uint32_t bytes,const EventAddressSpace& m) const{
 // Round up32 in widened arithmetic; zero/too-large requests reject. A free
 // header needs payload+32 for source split/terminal space, not just payload.
 // Validate ALL ordered nonoverlapping headers, even after finding a fit; bad
 // suffix rejects before source mutation. Never repair corrupted history.
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

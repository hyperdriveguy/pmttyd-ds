#pragma once
#include "event_address_space.hpp"
#include <array>
#include <cstring>
namespace ttyd {
// A noncopyable, callback-local accelerator over explicitly admitted raw RAM.
// No host pointers escape. Callers must not remap semantic providers, invoke
// external children or yield while using it. Unbound/semantic accesses retain
// the ordinary facade. Storage mapping mutation and nested sessions are rejected.
class EventAddressSpace::RawRamAccess {
public:
 RawRamAccess(const RawRamAccess&)=delete;
 RawRamAccess& operator=(const RawRamAccess&)=delete;
 bool bind(uint32_t address,uint32_t size){
  if(!size)return false;
  if(at(address,size))return true;
  auto* bytes=owner_.rawRamSpan(address,size);if(!bytes)return false;
  // Stream validation extends prefixes as successive keys become reachable.
  for(unsigned i=0;i<count_;++i)if(spans_[i].address==address){spans_[i]={address,size,bytes};return true;}
  if(count_<spans_.size())spans_[count_++]={address,size,bytes};
  // Capacity limits acceleration, never the admitted address domain.
  return true;
 }
 // Prove a caller-owned backing identity inside this synchronous borrow.
 bool aliases(uint32_t address,const uint8_t* bytes,uint32_t size) const{return size && owner_.rawRamSpan(address,size)==bytes;}
 bool readWord(uint32_t address,uint32_t& value) const {
  if(const auto* b=at(address,4)){value=uint32_t(b[0])<<24|uint32_t(b[1])<<16|uint32_t(b[2])<<8|b[3];return true;}
  return owner_.readWord(address,value);
 }
 bool readByte(uint32_t address,uint8_t& value) const {
  if(const auto* b=at(address,1)){value=*b;return true;}return owner_.readByte(address,value);
 }
 bool writeWord(uint32_t address,uint32_t value){
  if(auto* b=at(address,4)){for(unsigned i=0;i<4;++i)b[i]=uint8_t(value>>(8*(3-i)));return true;}
  return owner_.writeWord(address,value);
 }
 bool writeByte(uint32_t address,uint8_t value){
  if(auto* b=at(address,1)){*b=value;return true;}return owner_.writeByte(address,value);
 }
 // Only raw, fully bound, physically AND virtually disjoint ranges. Rejecting
 // aliases preserves the caller's original forward word/byte copy semantics.
 bool copy(uint32_t destination,uint32_t source,uint32_t size){
  if(!size || (destination>=source?uint64_t(destination)-source:uint64_t(source)-destination)<size)return false;
  auto* to=at(destination,size);const auto* from=at(source,size);if(!to || !from)return false;
  const uintptr_t a=reinterpret_cast<uintptr_t>(to),b=reinterpret_cast<uintptr_t>(from);
  if((a>=b?a-b:b-a)<size)return false;
  std::memcpy(to,from,size);return true;
 }
private:
 friend class EventAddressSpace;
 explicit RawRamAccess(EventAddressSpace& owner):owner_(owner){}
 ~RawRamAccess()=default;
 struct Span {uint32_t address=0,size=0;uint8_t* bytes=nullptr;};
 uint8_t* at(uint32_t address,uint32_t size) const {
  for(unsigned i=0;i<count_;++i){const auto& s=spans_[i];if(address>=s.address && uint64_t(address-s.address)+size<=s.size)return s.bytes+(address-s.address);}return nullptr;
 }
 void remember(uint32_t a,uint32_t n,uint8_t* bytes){
  for(unsigned i=0;i<count_;++i)if(spans_[i].address==a){spans_[i]={a,n,bytes};return;}
  if(count_<spans_.size())spans_[count_++]={a,n,bytes};
  else{spans_[replacement_]={a,n,bytes};replacement_=(replacement_+1)%spans_.size();}
 }
 // Thirty-two spans is an accelerator cache size, not a guest memory limit.
 // bind may succeed without caching at capacity; uncached reads/writes still
 // use the facade. Same-start stream prefixes may extend an existing binding.
 uint32_t unmappedBase_=0;uint64_t unmappedSize_=0;bool unmappedValid_=false;
 EventAddressSpace& owner_;std::array<Span,32> spans_{};unsigned count_=0,replacement_=0;
};
// RAII releases borrow even on exception; returned bool is callback result,
// not an allocation or independent async lifetime. No cached spans survive it.
// Facade reads/writes also reuse whole-region backing proofs during this scope.
// Bytes stay live; independent semantic registries/backing owners must stay fixed.
template<typename Callback> bool EventAddressSpace::withRawRamAccess(Callback&& callback){
 if(rawRamBorrowed_)return false;
 rawRamBorrowed_=true;
 RawRamAccess access(*this);rawRamCache_=&access;
 struct Release {bool& borrowed;RawRamAccess*& cache;~Release(){cache=nullptr;borrowed=false;}} release{rawRamBorrowed_,rawRamCache_};
 return callback(access);
}
}

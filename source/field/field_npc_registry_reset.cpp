#include "field_npc_registry_owner.hpp"
#include "field_npc_reset.hpp"
#include "event_address_space.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
bool FieldNpcRegistryBinding::reset(int32_t selector){
 if(!owner_->initialized_)return false;
 const unsigned selected=selector?2:0;const uint32_t capacity=selector?16:36;
 const auto registry=0x8041e8b4+(selector?0x14u:0u);uint32_t actualCapacity=0,array=0,gp=0,field=0,unused=0;
 if(!memory_.readWord(registry+4,actualCapacity) || actualCapacity!=capacity || !memory_.readWord(registry+12,array) || array!=owner_->addresses_[selected] || !owner_->span(array,capacity*0x340u))return false;
 if(!memory_.readWord(registry,unused) || !memory_.readWord(0x8041e8ac,unused))return false;
 if(!selector){uint16_t mode=0;if(!memory_.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !memory_.readWord(gp+0x168,field) || field!=owner_->addresses_[3] || !memory_.readHalf(field,mode))return false;}
 PackedNpcResetServices s;s.user=this;s.memory.user=&memory_;
 s.memory.word=[](void* p,uint32_t a){uint32_t v=0;const bool ok=static_cast<EventAddressSpace*>(p)->readWord(a,v);assert(ok);(void)ok;return v;};
 s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<EventAddressSpace*>(p)->writeWord(a,v);assert(ok);(void)ok;};
 s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){const bool ok=static_cast<EventAddressSpace*>(p)->writeHalf(a,v);assert(ok);(void)ok;};
 // Native retained allocation is contiguous; use the platform libc's optimized
 // clear rather than dispatching every byte through original-address lookup.
 s.clear=[](void* p,uint32_t a,uint32_t size){auto* bytes=static_cast<FieldNpcRegistryBinding*>(p)->owner_->span(a,size);assert(bytes);std::memset(bytes,0,size);};
 resetPackedNpcRegistry(selector,s);return true;
}
}

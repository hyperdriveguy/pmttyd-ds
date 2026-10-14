#include "field_sfx_ndsp.hpp"
#include <3ds.h>
namespace ttyd {
bool FieldSfxNdsp::install(int32_t handle,int32_t key,uint32_t descriptor,int32_t channel,bool pending) {
 const unsigned index=uint32_t(handle)&255;
 if(handle<0 || index>=registry_.slots.size() || key<0 || registry_.slots[index].key!=-1)return false;
 if(!pending && ((descriptor&0xc0000000) || !(descriptor&0x20000000) ||
    channel<20 || channel>=24))return false;
 for(unsigned i=0;i<registry_.slots.size();++i) {
  const auto& slot=registry_.slots[i];if(slot.key==-1)continue;
  if(!pending && slot.engine==channel)return false;
  if((uint32_t(slot.key)&0x1fff)==(uint32_t(key)&0x1fff) && descriptors_[i]!=descriptor)return false;
 }
 auto& slot=registry_.slots[index];slot=PositionalSfxSlot{};slot.key=key;
 slot.generation=uint16_t(uint32_t(handle)>>8);slot.flags=pending?1:0;slot.engine=pending?-1:channel;
 descriptors_[index]=descriptor;return true;
}
bool FieldSfxNdsp::attach(int32_t h,int32_t k,uint32_t d,int32_t c){return install(h,k,d,c,false);}
bool FieldSfxNdsp::attachPending(int32_t h,int32_t k,uint32_t d){return install(h,k,d,-1,true);}
bool FieldSfxNdsp::canOff(int32_t handle,bool available) const {
 if(handle==-1)return true;
 const unsigned index=uint32_t(handle)&255;if(index>=registry_.slots.size())return false;
 const auto& slot=registry_.slots[index];
 if(slot.key==-1 || (slot.flags&1) || slot.engine==-1)return true;
 return available && !(descriptors_[index]&0xc0000000) && (descriptors_[index]&0x20000000) &&
        slot.engine>=20 && slot.engine<24;
}
PositionalSfxStopServices FieldSfxNdsp::services() {
 PositionalSfxStopServices s;s.user=this;
 s.descriptor=[](void* p,uint32_t key)->std::optional<uint32_t>{auto& b=*static_cast<FieldSfxNdsp*>(p);
  for(unsigned i=0;i<b.registry_.slots.size();++i)if(b.registry_.slots[i].key!=-1 && (uint32_t(b.registry_.slots[i].key)&0x1fff)==key)return b.descriptors_[i];
  return {};
 };
 s.stop=[](void* p,PositionalSfxStopKind kind,int32_t channel){auto& b=*static_cast<FieldSfxNdsp*>(p);
  if(kind!=PositionalSfxStopKind::Effect || channel<20 || channel>=24)return false;
  for(const auto& slot:b.registry_.slots)if(slot.key!=-1 && !(slot.flags&1) && slot.engine==channel){ndspChnWaveBufClear(channel);return true;}
  return false;
 };return s;
}
bool FieldSfxNdsp::clear() {
 for(unsigned i=0;i<registry_.slots.size();++i)if(registry_.slots[i].key!=-1 && !registry_.off(int32_t(i),services()))return false;
 return true;
}
}

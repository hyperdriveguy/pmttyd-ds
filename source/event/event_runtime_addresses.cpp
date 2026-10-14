#include "event_runtime_addresses.hpp"
#include <cstring>

// Original globals:256 schedule IDs at0x803d8530,256 slots at0x803d8930;
// 0x8041e858/5c/60=active/count/allocations,0x804151e8/ec=next ID/speed,
// 0x8041e940=requested system level. Arrays have4-byte stride.
// Big-endian byte lanes alias words; speed transfers binary32 bits via memcpy.
namespace ttyd {
bool EventRuntimeAddresses::mayMapRange(uint32_t base,uint64_t size) const {
    if(!size)return false;
    const auto overlaps=[&](uint32_t at,uint32_t count){return uint64_t(base)<uint64_t(at)+count && uint64_t(at)<uint64_t(base)+size;};
    if(overlaps(0x803d8530,0x800))return true;
    for(const auto at:{0x8041e858u,0x8041e85cu,0x8041e860u,0x804151e8u,0x804151ecu,0x8041e940u})if(overlaps(at,4))return true;
    return false;
}
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
}
bool EventRuntimeAddresses::readField(uint32_t a,uint32_t& v) const {
    if(a>=0x803d8530 && a<0x803d8930)v=schedule_.ids[(a-0x803d8530)/4];
    else if(a>=0x803d8930 && a<0x803d8d30)v=static_cast<uint32_t>(schedule_.slots[(a-0x803d8930)/4]);
    else switch(a) {
    case 0x8041e858:v=schedule_.managerActive;break;
    case 0x8041e85c:v=static_cast<uint32_t>(schedule_.count);break;
    case 0x8041e860:v=globals_.allocationCount;break;
    case 0x804151e8:v=globals_.nextId;break;
    case 0x804151ec:std::memcpy(&v,&globals_.speed,4);break;
    case 0x8041e940:v=static_cast<uint32_t>(globals_.systemLevel);break;
    default:return false;
    }
    return true;
}
bool EventRuntimeAddresses::writeField(uint32_t a,uint32_t v) {
    if(a>=0x803d8530 && a<0x803d8930)schedule_.ids[(a-0x803d8530)/4]=v;
    else if(a>=0x803d8930 && a<0x803d8d30)schedule_.slots[(a-0x803d8930)/4]=sw(v);
    else switch(a) {
    case 0x8041e858:schedule_.managerActive=v;break;
    case 0x8041e85c:schedule_.count=sw(v);break;
    case 0x8041e860:globals_.allocationCount=v;break;
    case 0x804151e8:globals_.nextId=v;break;
    case 0x804151ec:std::memcpy(&globals_.speed,&v,4);break;
    case 0x8041e940:globals_.systemLevel=sw(v);break;
    default:return false;
    }
    return true;
}
bool EventRuntimeAddresses::readByte(uint32_t a,uint8_t& v) const {uint32_t word;if(!readField(a&~3u,word))return false;v=uint8_t(word>>(24-8*(a&3)));return true;}
bool EventRuntimeAddresses::writeByte(uint32_t a,uint8_t v) {uint32_t word;if(!readField(a&~3u,word))return false;const unsigned shift=24-8*(a&3);return writeField(a&~3u,(word&~(0xffu<<shift))|(uint32_t(v)<<shift));}
bool EventRuntimeAddresses::readWord(uint32_t a,uint32_t& v) const {
    if((a&3)==0)return readField(a,v);
    if(uint64_t(a)+4>0x100000000ULL)return false;
    uint32_t word=0;for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(a+i,b))return false;word=word<<8|b;}v=word;return true;
}
bool EventRuntimeAddresses::writeWord(uint32_t a,uint32_t v) {
    if((a&3)==0)return writeField(a,v);
    if(uint64_t(a)+4>0x100000000ULL)return false;
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(a+i,b))return false;}
    for(unsigned i=0;i<4;++i)if(!writeByte(a+i,uint8_t(v>>(24-8*i))))return false;
    return true;
}
}

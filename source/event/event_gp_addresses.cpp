#include "event_gp_addresses.hpp"

// Live GP aliases: +4 refresh rate,+14 battle-context selector,+38/3c event
// timestamp high/low; all other bytes remain retained packed storage.
// Root pointer0x80415fd0..d3 is separately mapped big-endian state and may
// change via byte writes. GP retirement keeps that original word unresolved.
namespace ttyd {
bool EventGpAddresses::mayMapRange(uint32_t base,uint64_t size) const {
    if(!size)return false;
    if(uint64_t(base)<0x80415fd4ULL && 0x80415fd0ULL<uint64_t(base)+size)return true;
    for(const auto& r:regions_)if(uint64_t(base)<uint64_t(r.base)+EventGpAddresses::size && uint64_t(r.base)<uint64_t(base)+size)return true;
    return false;
}
namespace {
int32_t sw(uint32_t v) {return v<=0x7fffffff?static_cast<int32_t>(v):static_cast<int32_t>(int64_t(v)-0x100000000LL);}
}
EventGpAddresses::Position EventGpAddresses::find(uint32_t a) const {
    for(size_t i=0;i<regions_.size();++i)if(a>=regions_[i].base && uint64_t(a)<uint64_t(regions_[i].base)+size)return {i,a-regions_[i].base,true};
    return {};
}
bool EventGpAddresses::registerGp(uint32_t base,EventGlobals& globals) {
    if(!base || uint64_t(base)+size>0x100000000ULL || (uint64_t(base)<0x80415fd4ULL && 0x80415fd0ULL<uint64_t(base)+size))return false;
    for(const auto& r:regions_)if(r.globals==&globals || (uint64_t(base)<uint64_t(r.base)+size && uint64_t(r.base)<uint64_t(base)+size))return false;
    regions_.push_back({base,&globals,std::vector<uint8_t>(size,0)});return true;
}
bool EventGpAddresses::unregisterGp(uint32_t base,std::vector<uint8_t>* bytes) {
    size_t i=0;while(i<regions_.size() && regions_[i].base!=base)++i;if(i==regions_.size())return false;
    if(bytes) {std::vector<uint8_t> snapshot(size);for(uint32_t j=0;j<size;++j)readByte(base+j,snapshot[j]);*bytes=std::move(snapshot);}
    regions_.erase(regions_.begin()+i);return true; // Current ORIGINAL word is preserved.
}
EventGlobals* EventGpAddresses::currentGlobals() const {for(const auto& r:regions_)if(r.base==current_)return r.globals;return nullptr;}
bool EventGpAddresses::readByte(uint32_t a,uint8_t& value) const {
    if(a>=0x80415fd0 && a<0x80415fd4) {value=uint8_t(current_>>(24-8*(a&3)));return true;}
    auto p=find(a);if(!p.valid)return false;const auto& r=regions_[p.region];const auto& g=*r.globals;uint32_t word;
    switch(p.offset&~3u) {case 4:word=static_cast<uint32_t>(g.refreshRate);break;case 0x14:word=static_cast<uint32_t>(g.battleContext);break;case 0x38:word=static_cast<uint32_t>(g.timestamp>>32);break;case 0x3c:word=static_cast<uint32_t>(g.timestamp);break;default:value=r.bytes[p.offset];return true;}
    value=uint8_t(word>>(24-8*(p.offset&3)));return true;
}
bool EventGpAddresses::writeByte(uint32_t a,uint8_t value) {
    if(a>=0x80415fd0 && a<0x80415fd4) {const unsigned shift=24-8*(a&3);current_=(current_&~(0xffu<<shift))|(uint32_t(value)<<shift);return true;}
    auto p=find(a);if(!p.valid)return false;auto& r=regions_[p.region];auto& g=*r.globals;const uint32_t off=p.offset&~3u;uint32_t word;
    switch(off) {case 4:word=static_cast<uint32_t>(g.refreshRate);break;case 0x14:word=static_cast<uint32_t>(g.battleContext);break;case 0x38:word=static_cast<uint32_t>(g.timestamp>>32);break;case 0x3c:word=static_cast<uint32_t>(g.timestamp);break;default:r.bytes[p.offset]=value;return true;}
    const unsigned shift=24-8*(p.offset&3);word=(word&~(0xffu<<shift))|(uint32_t(value)<<shift);
    switch(off) {case 4:g.refreshRate=sw(word);break;case 0x14:g.battleContext=sw(word);break;case 0x38:g.timestamp=(uint64_t(word)<<32)|(g.timestamp&0xffffffffu);break;case 0x3c:g.timestamp=(g.timestamp&0xffffffff00000000ULL)|word;break;}
    return true;
}
bool EventGpAddresses::readWord(uint32_t a,uint32_t& value) const {
    if(uint64_t(a)+4>0x100000000ULL)return false;
    if(a==0x80415fd0) {value=current_;return true;}
    auto p=find(a);if(p.valid && (p.offset&3)==0) {
        const auto& g=*regions_[p.region].globals;
        switch(p.offset) {case 4:value=static_cast<uint32_t>(g.refreshRate);return true;case 0x14:value=static_cast<uint32_t>(g.battleContext);return true;case 0x38:value=static_cast<uint32_t>(g.timestamp>>32);return true;case 0x3c:value=static_cast<uint32_t>(g.timestamp);return true;default:break;}
    }
    uint32_t word=0;
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(a+i,b))return false;word=word<<8|b;}value=word;return true;
}
bool EventGpAddresses::writeWord(uint32_t a,uint32_t value) {
    if(uint64_t(a)+4>0x100000000ULL)return false;
    if(a==0x80415fd0) {current_=value;return true;}
    auto p=find(a);if(p.valid && (p.offset&3)==0) {
        auto& g=*regions_[p.region].globals;
        switch(p.offset) {case 4:g.refreshRate=sw(value);return true;case 0x14:g.battleContext=sw(value);return true;case 0x38:g.timestamp=(uint64_t(value)<<32)|(g.timestamp&0xffffffffu);return true;case 0x3c:g.timestamp=(g.timestamp&0xffffffff00000000ULL)|value;return true;default:break;}
    }
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(a+i,b))return false;}
    for(unsigned i=0;i<4;++i)if(!writeByte(a+i,uint8_t(value>>(24-8*i))))return false;
    return true;
}
}

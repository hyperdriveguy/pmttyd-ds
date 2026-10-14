#include "event_address_space.hpp"

namespace ttyd {
bool EventAddressSpace::readByte(uint32_t a,uint8_t& v) const {
    if(entries_.readByte(a,v) || contexts_.readByte(a,v) || runtime_.readByte(a,v) || gp_.readByte(a,v))return true;
    for(const auto& r:ram_)if(a>=r.base && uint64_t(a)-r.base<r.bytes.size()) {v=r.bytes[a-r.base];return true;}
    return false;
}
bool EventAddressSpace::writeByte(uint32_t a,uint8_t v) {
    if(entries_.writeByte(a,v) || contexts_.writeByte(a,v) || runtime_.writeByte(a,v) || gp_.writeByte(a,v))return true;
    for(auto& r:ram_)if(a>=r.base && uint64_t(a)-r.base<r.bytes.size()) {r.bytes[a-r.base]=v;return true;}
    return false;
}
bool EventAddressSpace::available(uint32_t base,uint64_t size) const {
    if(!size || uint64_t(base)+size>0x100000000ULL)return false;
    for(uint64_t i=0;i<size;++i) {uint8_t v;if(readByte(base+uint32_t(i),v))return false;}
    return true;
}
bool EventAddressSpace::registerRam(uint32_t base,std::vector<uint8_t> bytes) {
    if(!available(base,bytes.size()))return false;
    ram_.push_back({base,std::move(bytes)});return true;
}
bool EventAddressSpace::unregisterRam(uint32_t base,std::vector<uint8_t>* bytes) {
    for(size_t i=0;i<ram_.size();++i)if(ram_[i].base==base) {
        if(bytes)*bytes=std::move(ram_[i].bytes);
        ram_.erase(ram_.begin()+i);return true;
    }
    return false;
}
bool EventAddressSpace::extractRam(uint32_t base,uint32_t size,std::vector<uint8_t>& bytes) {
    const uint64_t end=uint64_t(base)+size;
    if(!size || end>0x100000000ULL)return false;
    for(size_t i=0;i<ram_.size();++i) {
        const auto& region=ram_[i];const uint64_t limit=uint64_t(region.base)+region.bytes.size();
        if(base<region.base || end>limit)continue;
        const auto begin=region.bytes.begin();const size_t offset=base-region.base;
        std::vector<uint8_t> removed(begin+offset,begin+offset+size);
        Region left{region.base,std::vector<uint8_t>(begin,begin+offset)};
        Region right{static_cast<uint32_t>(end),std::vector<uint8_t>(begin+offset+size,region.bytes.end())};
        if(left.bytes.empty()) {
            if(right.bytes.empty())ram_.erase(ram_.begin()+i);
            else ram_[i]=std::move(right);
        } else {
            ram_[i]=std::move(left);if(!right.bytes.empty())ram_.push_back(std::move(right));
        }
        bytes=std::move(removed);return true;
    }
    return false;
}
bool EventAddressSpace::importScripts(const EventScriptAssets& scripts) {
    std::vector<Region> imported;
    for(const auto& segment:scripts.segments()) {
        if(!available(segment.address,segment.size))return false;
        Region r{segment.address,std::vector<uint8_t>(segment.size)};
        for(uint32_t i=0;i<segment.size;++i)if(!scripts.readByte(segment.address+i,r.bytes[i]))return false;
        imported.push_back(std::move(r));
    }
    if(imported.empty())return false;
    for(auto& r:imported)ram_.push_back(std::move(r));
    return true;
}
bool EventAddressSpace::readSpan(uint32_t a,unsigned size,uint32_t& value) const {
    if(uint64_t(a)+size>0x100000000ULL)return false;
    uint32_t result=0;
    for(unsigned i=0;i<size;++i) {uint8_t b;if(!readByte(a+i,b))return false;result=result<<8|b;}
    value=result;return true;
}
bool EventAddressSpace::writeSpan(uint32_t a,unsigned size,uint32_t value) {
    if(uint64_t(a)+size>0x100000000ULL)return false;
    // Native unsupported-range policy:reject before changing any mapped byte.
    for(unsigned i=0;i<size;++i) {uint8_t b;if(!readByte(a+i,b))return false;}
    for(unsigned i=0;i<size;++i)if(!writeByte(a+i,uint8_t(value>>(8*(size-1-i)))))return false;
    return true;
}
bool EventAddressSpace::readHalf(uint32_t a,uint16_t& value) const {uint32_t v;if(!readSpan(a,2,v))return false;value=uint16_t(v);return true;}
bool EventAddressSpace::readWord(uint32_t a,uint32_t& value) const {
    if(entries_.readWord(a,value) || contexts_.readWord(a,value) || runtime_.readWord(a,value) || gp_.readWord(a,value))return true;
    return readSpan(a,4,value);
}
bool EventAddressSpace::writeHalf(uint32_t a,uint16_t value) {return writeSpan(a,2,value);}
bool EventAddressSpace::writeWord(uint32_t a,uint32_t value) {
    // Preserve the single-word store of aligned live fields where possible.
    if(entries_.writeWord(a,value) || contexts_.writeWord(a,value) || runtime_.writeWord(a,value) || gp_.writeWord(a,value))return true;
    return writeSpan(a,4,value);
}
}

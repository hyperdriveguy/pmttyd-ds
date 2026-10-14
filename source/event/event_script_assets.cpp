#include "event_script_assets.hpp"
#include <cstdio>
#include <cstring>
#include <limits>

namespace ttyd {
namespace {
uint32_t word(const std::vector<uint8_t>& bytes,size_t offset) {
    return uint32_t(bytes[offset])<<24|uint32_t(bytes[offset+1])<<16|uint32_t(bytes[offset+2])<<8|bytes[offset+3];
}
}
bool EventScriptAssets::load(const char* path) {
    if(!path)return false;
    auto* file=std::fopen(path,"rb");if(!file)return false;
    if(std::fseek(file,0,SEEK_END)!=0) {std::fclose(file);return false;}
    const long length=std::ftell(file);
    if(length<12 || uint64_t(length)>std::numeric_limits<uint32_t>::max() || std::fseek(file,0,SEEK_SET)!=0) {std::fclose(file);return false;}
    std::vector<uint8_t> bytes(static_cast<size_t>(length));const size_t read=std::fread(bytes.data(),1,bytes.size(),file);std::fclose(file);
    if(read!=bytes.size() || std::memcmp(bytes.data(),"PMEVT001",8)!=0)return false;
    const uint32_t count=word(bytes,8);if(count==0 || count>(bytes.size()-12)/12)return false;
    std::vector<EventScriptSegment> segments;segments.reserve(count);
    uint64_t nextOffset=12+12ULL*count,previousEnd=0;
    for(uint32_t i=0;i<count;++i) {
        const size_t at=12+12ULL*i;EventScriptSegment s{word(bytes,at),word(bytes,at+4),word(bytes,at+8)};
        const uint64_t end=uint64_t(s.address)+s.size;
        if((s.address&3) || s.size==0 || (s.size&3) || end>0x100000000ULL || s.address<previousEnd || s.offset!=nextOffset || nextOffset+s.size>bytes.size())return false;
        previousEnd=end;nextOffset+=s.size;segments.push_back(s);
    }
    if(nextOffset!=bytes.size())return false;
    data_=std::move(bytes);segments_=std::move(segments);return true;
}
const EventScriptSegment* EventScriptAssets::find(uint32_t address,uint32_t bytes) const {
    size_t low=0,high=segments_.size();
    while(low<high) {const size_t mid=low+(high-low)/2;if(segments_[mid].address<=address)low=mid+1;else high=mid;}
    if(low==0)return nullptr;
    const auto& s=segments_[low-1];
    return uint64_t(address)+bytes<=uint64_t(s.address)+s.size?&s:nullptr;
}
bool EventScriptAssets::readByte(uint32_t address,uint8_t& value) const {
    auto* s=find(address,1);if(!s)return false;value=data_[s->offset+address-s->address];return true;
}
bool EventScriptAssets::readWord(uint32_t address,uint32_t& value) const {
    if(auto* s=find(address,4)) {value=word(data_,s->offset+address-s->address);return true;}
    if(uint64_t(address)+4>0x100000000ULL)return false;
    uint32_t result=0;
    for(unsigned i=0;i<4;++i) {uint8_t b;if(!readByte(address+i,b))return false;result=result<<8|b;}
    value=result;return true;
}
}

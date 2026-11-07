#include "event_script_assets.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <limits>

// PMEVT001 pack:8-byte magic, big-endian count at8, then12-byte
// (original address,size,file offset) rows from12 and contiguous payload.
// Alignment4 is script word alignment; address ceiling is exclusive2^32.
// Validate into candidates and publish only after exact EOF/nonoverlap checks.
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
// Merge sorted spans without an address-sized map or per-byte allocation.
// Payload storage is private: merged offsets need no serialized PMEVT header.
bool EventScriptAssets::merge(const EventScriptAssets& other) {
    if(segments_.empty() || other.segments_.empty())return false;
    std::vector<uint8_t> bytes;
    std::vector<EventScriptSegment> segments;
    size_t left=0,right=0;
    while(left<segments_.size() || right<other.segments_.size()) {
        const bool own=right==other.segments_.size() ||
            (left<segments_.size() && segments_[left].address<=other.segments_[right].address);
        const auto& source=own?*this:other;
        const auto s=own?segments_[left++]:other.segments_[right++];
        uint32_t overlap=0;
        if(!segments.empty() && uint64_t(s.address)<=uint64_t(segments.back().address)+segments.back().size) {
            const auto& last=segments.back();
            overlap=uint32_t(std::min<uint64_t>(s.size,uint64_t(last.address)+last.size-s.address));
            for(uint32_t i=0;i<overlap;++i)
                if(bytes[last.offset+s.address-last.address+i]!=source.data_[s.offset+i])return false;
        } else {
            if(bytes.size()>UINT32_MAX)return false;
            segments.push_back({s.address,0,uint32_t(bytes.size())});
        }
        const auto extra=s.size-overlap;
        if(uint64_t(bytes.size())+extra>UINT32_MAX || uint64_t(segments.back().size)+extra>UINT32_MAX)return false;
        bytes.insert(bytes.end(),source.data_.begin()+s.offset+overlap,source.data_.begin()+s.offset+s.size);
        segments.back().size+=extra;
    }
    data_=std::move(bytes);segments_=std::move(segments);return true;
}
// Upper-bound search chooses the last segment starting at/before address.
// Cross-segment word reads fall back to bytes, retaining big-endian assembly.
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

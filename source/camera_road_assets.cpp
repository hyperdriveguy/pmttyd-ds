#include "camera_road_assets.hpp"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <limits>

#include <utility>
namespace ttyd {
namespace {
constexpr size_t MaxBytes=2*1024*1024;
std::array<CameraRoadAsset,5> assets;
bool loaded=false;
uint32_t beWord(const std::vector<uint8_t>& b,size_t o) {
    return uint32_t(b[o])<<24|uint32_t(b[o+1])<<16|uint32_t(b[o+2])<<8|b[o+3];
}
std::optional<std::string> text(const std::vector<uint8_t>& b,size_t o,size_t length) {
    if(o>b.size() || length>b.size()-o)return std::nullopt;
    size_t end=o;while(end<o+length && b[end])++end;
    if(end==o+length)return std::nullopt;
    return std::string(b.begin()+o,b.begin()+end);
}
}
std::optional<uint32_t> CameraRoadAsset::word(uint32_t offset) const {
    if(uint64_t(offset)+4>data.size())return std::nullopt;
    return beWord(data,offset);
}
std::optional<float> CameraRoadAsset::number(uint32_t offset) const {
    const auto bits=word(offset);if(!bits)return std::nullopt;
    float value;std::memcpy(&value,&*bits,4);return value;
}
std::optional<uint32_t> CameraRoadAsset::element(size_t collection,uint32_t index,uint32_t stride,uint32_t field) const {
    if(collection>=blocks.size() || stride<4 || field%4 || field>stride-4 || index>=blocks[collection].count)return std::nullopt;
    return blocks[collection].offset+index*stride+field;
}
bool CameraRoadAsset::decode(std::vector<uint8_t> bytes) {
    if(bytes.size()<0x10c || bytes.size()>MaxBytes || beWord(bytes,0)!=bytes.size())return false;
    if(text(bytes,4,64)!="MarioSt_CameraRoadExport" || text(bytes,0x44,64)!="1.01")return false;
    if(!text(bytes,0x84,64))return false;
    std::array<Collection,9> parsed;
    constexpr uint32_t strides[]={12,184,104,8,12,12,4,104,0};
    for(size_t n=0;n<parsed.size();++n) {
        parsed[n]={beWord(bytes,0xc4+n*4),beWord(bytes,0xe8+n*4)};
        if(parsed[n].count && (!strides[n] || parsed[n].offset<0x10c || parsed[n].offset%4 ||
            uint64_t(parsed[n].offset)+uint64_t(parsed[n].count)*strides[n]>bytes.size()))return false;
    }
    // Only the first lens triplet is a closed runtime reader; extra108's
    // stride is unresolved and all five shipped collections are empty.
    if(parsed[0].count!=1 || parsed[8].count!=0)return false;
    CameraRoadAsset candidate;candidate.data=std::move(bytes);candidate.blocks=parsed;candidate.versionText="1.01";
    auto span=[](uint32_t start,uint32_t count,uint32_t total){return uint64_t(start)+count<=total;};
    for(uint32_t n=0;n<parsed[1].count;++n) {
        if(!candidate.roadName(n))return false;
        if(!span(*candidate.roadWord(n,0xa0),*candidate.roadWord(n,0xa4),parsed[4].count) ||
           !span(*candidate.roadWord(n,0xa8),*candidate.roadWord(n,0xac),parsed[2].count) ||
           (*candidate.roadWord(n,0xb4) && !span(*candidate.roadWord(n,0xb0),*candidate.roadWord(n,0xb4),parsed[7].count)))return false;
    }
    for(uint32_t n=0;n<parsed[2].count;++n) {
        if(*candidate.zoneWord(n,0x58)>parsed[5].count ||
           !span(*candidate.zoneWord(n,0x60),*candidate.zoneWord(n,0x64),parsed[3].count))return false;
    }
    for(uint32_t n=0;n<parsed[3].count;++n) {
        const auto p=*candidate.polygon(n);if(!span(p[0],p[1],parsed[6].count))return false;
    }
    // Polygon indices are relative to the selected zone's vertex start.
    for(uint32_t n=0;n<parsed[2].count;++n) {
        const auto start=*candidate.zoneWord(n,0x58),first=*candidate.zoneWord(n,0x60),count=*candidate.zoneWord(n,0x64);
        for(uint32_t p=first;p<first+count;++p) {
            const auto polygon=*candidate.polygon(p);
            for(uint32_t i=polygon[0];i<polygon[0]+polygon[1];++i)
                if(uint64_t(start)+*candidate.vertexIndex(i)>=parsed[5].count)return false;
        }
    }
    *this=std::move(candidate);return true;
}
bool CameraRoadAsset::load(const char* path) {
    std::ifstream file(path,std::ios::binary|std::ios::ate);if(!file)return false;
    const auto size=file.tellg();if(size<0 || size>std::streamoff(MaxBytes))return false;
    std::vector<uint8_t> raw(static_cast<size_t>(size));file.seekg(0);
    if(!file.read(reinterpret_cast<char*>(raw.data()),raw.size()))return false;
    return decode(std::move(raw));
}
std::optional<std::array<float,3>> CameraRoadAsset::lensMetadata() const {
    if(data.empty())return std::nullopt;
    return std::array<float,3>{*number(blocks[0].offset),*number(blocks[0].offset+4),*number(blocks[0].offset+8)};
}
std::optional<std::string> CameraRoadAsset::roadName(uint32_t index) const {
    const auto o=element(1,index,184,0);return o?text(data,*o,32):std::nullopt;
}
std::optional<uint32_t> CameraRoadAsset::roadWord(uint32_t index,uint32_t field) const {
    const auto o=element(1,index,184,field);return o?word(*o):std::nullopt;
}
std::optional<float> CameraRoadAsset::roadFloat(uint32_t index,uint32_t field) const {
    const auto o=element(1,index,184,field);return o?number(*o):std::nullopt;
}
std::optional<CameraPoint> CameraRoadAsset::pathVector(uint32_t index) const {
    const auto o=element(4,index,12,0);if(!o)return std::nullopt;
    return CameraPoint{*number(*o),*number(*o+4),*number(*o+8)};
}
std::optional<CameraPoint> CameraRoadAsset::zoneVertex(uint32_t index) const {
    const auto o=element(5,index,12,0);if(!o)return std::nullopt;
    return CameraPoint{*number(*o),*number(*o+4),*number(*o+8)};
}
std::optional<uint32_t> CameraRoadAsset::zoneWord(uint32_t index,uint32_t field) const {
    const auto o=element(2,index,104,field);return o?word(*o):std::nullopt;
}
std::optional<std::array<uint32_t,2>> CameraRoadAsset::polygon(uint32_t index) const {
    const auto o=element(3,index,8,0);if(!o)return std::nullopt;
    return std::array<uint32_t,2>{*word(*o),*word(*o+4)};
}
std::optional<uint32_t> CameraRoadAsset::vertexIndex(uint32_t index) const {
    const auto o=element(6,index,4,0);return o?word(*o):std::nullopt;
}
bool loadCameraRoadAssets(const char* directory) {
    std::array<CameraRoadAsset,5> next;
    constexpr const char* names[]={"map","gor_00","gor_02","gor_03","gor_04"};
    for(size_t n=0;n<next.size();++n) {
        const std::string path=std::string(directory)+"/"+names[n]+".camera";
        if(!next[n].load(path.c_str()))return false;
    }
    assets=std::move(next);loaded=true;return true;
}
const CameraRoadAsset* cameraRoadAsset(uint32_t room) {return loaded && room<assets.size()?&assets[room]:nullptr;}
}

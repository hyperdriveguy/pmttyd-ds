#include "battle_audience_resources.hpp"
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const std::vector<uint8_t>& b,size_t o){return uint32_t(b[o])<<24|uint32_t(b[o+1])<<16|uint32_t(b[o+2])<<8|b[o+3];}
bool read(const std::string& path,std::vector<uint8_t>& b,size_t size,unsigned count) {
    FILE* f=std::fopen(path.c_str(),"rb");if(!f)return false;
    b.resize(size);const bool ok=std::fread(b.data(),1,size,f)==size && std::fgetc(f)==EOF;std::fclose(f);
    if(!ok || word(b,0)!=0x0020af30 || word(b,4)!=count)return false;
    const auto table=word(b,8);if(table>size || count>(size-table)/8)return false;
    for(unsigned i=0;i<count;++i) {
        const auto image=word(b,table+8*i),palette=word(b,table+8*i+4);
        if(image>size || size-image<36 || palette || word(b,image)!=0x00400040 || word(b,image+4)!=14)return false;
        const auto pixels=word(b,image+8);if(pixels>size || size-pixels<2048)return false;
    }
    return true;
}
}
bool BattleAudienceResources::load(const char* root) {
    if(!root)return false;
    std::vector<uint8_t> normal,luigi;
    if(!read(std::string(root)+"/audience-normal.tpl",normal,428896,205) ||
       !read(std::string(root)+"/audience-luigi.tpl",luigi,39776,19))return false;
    normal_=std::move(normal);luigi_=std::move(luigi);return true;
}
bool BattleAudienceResources::loadFrames(const char* root) {
    if(!root)return false;
    FILE* f=std::fopen((std::string(root)+"/audience-frames.bin").c_str(),"rb");
    if(!f)return false;
    std::array<uint8_t,20+224*32> data{};
    const bool ok=std::fread(data.data(),1,data.size(),f)==data.size() && std::fgetc(f)==EOF;
    std::fclose(f);
    if(!ok || std::memcmp(data.data(),"PMAUD001",8))return false;
    const auto le=[&](size_t p){return uint32_t(data[p]) | uint32_t(data[p+1])<<8 |
        uint32_t(data[p+2])<<16 | uint32_t(data[p+3])<<24;};
    if(le(8)!=1024 || le(12)!=1024 || le(16)!=224)return false;
    std::array<Frame,224> frames{};
    for(unsigned slot=0;slot<frames.size();++slot) {
        const size_t p=20+32*slot;
        if(le(p)!=(slot<205?1u:2u) || le(p+4)!=(slot<205?slot:slot-205) ||
           le(p+8)!=64 || le(p+12)!=64)return false;
        const float x=float((slot%15)*68+2),y=float((slot/15)*68+2);
        const std::array<float,4> expected{x/1024,1-y/1024,(x+64)/1024,1-(y+64)/1024};
        frames[slot].width=64;frames[slot].height=64;
        for(unsigned i=0;i<4;++i) {
            const uint32_t bits=le(p+16+4*i);float uv;
            std::memcpy(&uv,&bits,sizeof(uv));
            if(uv!=expected[i])return false; // Also rejects NaNs/Inf and cross-frame UVs.
            frames[slot].uv[i]=uv;
        }
    }
    frames_=frames;framesReady_=true;return true;
}
const BattleAudienceResources::Frame* BattleAudienceResources::frame(uint32_t handle,uint32_t index) const {
    if(!framesReady_)return nullptr;
    if(handle==1 && index<205)return &frames_[index];
    if(handle==2 && index<19)return &frames_[205+index];
    return nullptr;
}
std::optional<uint32_t> BattleAudienceResources::handle(const std::string& path) const {
    if(path=="battle/audience/audience_normal.tpl" && !normal_.empty())return 1;
    if(path=="battle/audience/audience_luigi.tpl" && !luigi_.empty())return 2;
    return std::nullopt;
}
const std::vector<uint8_t>* BattleAudienceResources::payload(uint32_t handle) const {
    if(handle==1 && !normal_.empty())return &normal_;
    if(handle==2 && !luigi_.empty())return &luigi_;
    return nullptr;
}
}

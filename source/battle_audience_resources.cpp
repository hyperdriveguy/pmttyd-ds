#include "battle_audience_resources.hpp"
#include <cstdio>
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

#include "pouch_revision.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <string>
namespace ttyd {
namespace {
std::array<int8_t,340> costs{};std::array<int16_t,32> hp{};bool ready=false;
int16_t narrow(int32_t value){const auto bits=uint32_t(value)&65535;return bits<32768?int16_t(bits):int16_t(int32_t(bits)-65536);}
}
bool loadPouchRevisionAssets(const char* root) {
    if(!root)return false;FILE* f=std::fopen((std::string(root)+"/pouch-revision.bin").c_str(),"rb");if(!f)return false;
    char magic[8];uint8_t count[4];std::array<int8_t,340> c{};std::array<uint8_t,64> h{};
    const bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMPRV001",8) && std::fread(count,4,1,f)==1 &&
        count[0]==84 && count[1]==1 && !count[2] && !count[3] && std::fread(c.data(),c.size(),1,f)==1 &&
        std::fread(h.data(),h.size(),1,f)==1 && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
    std::array<int16_t,32> values{};for(unsigned i=0;i<32;++i)values[i]=narrow(uint32_t(h[2*i])|(uint32_t(h[2*i+1])<<8));
    costs=c;hp=values;ready=true;return true;
}
bool pouchRevisionAvailable(const ProloguePouch& p) {
    if(!ready)return false;
    for(unsigned i=0;i<200;++i){const auto item=p.half(0x38a+2*i);if(item<0 || item>=340)return false;}
    for(unsigned id=1;id<=7;++id){const auto rank=p.half(14*id+8);if(rank<0 || rank>3)return false;}
    return true;
}
bool revisePouchParameters(ProloguePouch& p) {
    if(!pouchRevisionAvailable(p))return false;
    p.half(0x72,p.half(0x8e));p.half(0x76,p.half(0x90));
    for(unsigned i=0;i<200;++i){const auto item=p.half(0x38a+2*i);
        if(item==278)p.half(0x72,narrow(p.half(0x72)+5));
        if(item==280)p.half(0x76,narrow(p.half(0x76)+5));}
    if(p.half(0x70)>p.half(0x72))p.half(0x70,p.half(0x72));
    if(p.half(0x74)>p.half(0x76))p.half(0x74,p.half(0x76));
    p.half(0x92,p.half(0x94));
    for(unsigned i=0;i<200;++i)p.half(0x92,narrow(p.half(0x92)-costs[unsigned(p.half(0x38a+2*i))]));
    unsigned count=0;for(unsigned i=0;i<200;++i)if(p.half(0x38a+2*i)==279)++count;
    for(unsigned id=1;id<=7;++id){const unsigned base=14*id;
        p.half(base+4,hp[4*id+unsigned(p.half(base+8))]);
        p.half(base+2,narrow(p.half(base+4)+5*int32_t(count)));
        if(p.half(base+6)>p.half(base+2))p.half(base+6,p.half(base+2));}
    return true;
}
std::optional<int32_t> removeRevisedKeyItem(ProloguePouch& p,int32_t item) {
    if(!pouchRevisionAvailable(p) || item<1 || item>120)return std::nullopt;
    if(p.removeKeyItem(item)!=1)return -1;
    revisePouchParameters(p);return 0;
}
}

#include "field_map_npc_assets.hpp"
#include "event_address_space.hpp"
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {uint32_t little(const uint8_t* p){return p[0]|uint32_t(p[1])<<8|uint32_t(p[2])<<16|uint32_t(p[3])<<24;}}
bool FieldMapNpcAssets::load(const char* path){
 if(!path)return false;
 FILE* f=std::fopen(path,"rb");if(!f)return false;uint8_t header[16];bool ok=std::fread(header,16,1,f)==1 && !std::memcmp(header,"PMFMNPC1",8) && little(header+12)==392 && little(header+8)<=4096;std::vector<uint8_t> next;
 if(ok){next.resize(little(header+8)*392);ok=next.empty() || std::fread(next.data(),next.size(),1,f)==1;}
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 for(size_t n=0;n<next.size();n+=392){if(little(next.data()+n)!=n/392)return false;for(unsigned k=0;k<5;++k)if(!std::memchr(next.data()+n+72+64*k,0,64))return false;
  for(unsigned k=0;k<72;k+=4){const auto v=little(next.data()+n+k);for(unsigned b=0;b<4;++b)next[n+k+b]=uint8_t(v>>(24-8*b));}
 }
 bytes_=std::move(next);loaded_=true;return true;
}
bool FieldMapNpcAssets::mapTo(EventAddressSpace& m,uint32_t base) const{return loaded_ && (bytes_.empty() || m.registerRam(base,bytes_));}
std::optional<PackedMapNpcInput> FieldMapNpcAssets::input(unsigned index,uint32_t base,uint32_t area) const{
 if(index>=count() || uint64_t(base)+bytes_.size()>0x100000000ULL)return {};
 const auto a=base+392*index;PackedMapNpcInput i;i.ordinal=index;i.area=area;i.name=a+72;i.tribe=a+136;i.ai=a+200;i.territory=a+264;i.battle=a+328;i.position=a+4;i.direction=a+16;i.territoryBase=a+20;i.loiter=a+32;i.homing=a+44;for(unsigned n=0;n<4;++n)i.ranges[n]=a+56+4*n;return i;
}
}

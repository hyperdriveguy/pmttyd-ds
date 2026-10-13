#include "save.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>
namespace ttyd {
namespace {
void put(SaveBytes& b, int offset, uint32_t v) { for (int i=0;i<4;++i) b[offset+i]=(v>>(i*8))&255; }
uint32_t get(const SaveBytes& b,int offset) { uint32_t v=0; for (int i=0;i<4;++i) v|=uint32_t(b[offset+i])<<(i*8); return v; }
uint32_t checksum(const SaveBytes& b,int length) { uint32_t v=2166136261u; for(int i=0;i<length;++i) {v^=b[i];v*=16777619u;} return v; }
bool read(const std::string& path, Progress& p) {
    FILE* f=std::fopen(path.c_str(),"rb"); if(!f) return false;
    SaveBytes b{};const size_t size=std::fread(b.data(),1,b.size(),f);
    bool ok=(size==32 && (get(b,4)==1 || get(b,4)==2)) || (size==40 && get(b,4)>=3 && get(b,4)<=7);
    ok=ok && std::fgetc(f)==EOF; std::fclose(f);
    return ok && decodeSave(b,p);
}
}
SaveBytes encodeSave(const Progress& p) {
    SaveBytes b{}; if(p.mushrooms<0 || p.mushrooms>99 || p.hp<0 || p.hp>10 || !roomAsset(p.room) || !std::isfinite(p.elevation) || std::fabs(p.elevation)>12.7f ||
        (p.interiorMask && !((p.room==0 && p.interiorMask==1) || (p.room==2 && p.interiorMask==2))))return b;
    const auto& talk=p.originalNpcTalkState;
    const bool known=talk.quest1420 || talk.flag5405 || talk.flag5406;
    std::memcpy(b.data(),"PM2D",4); put(b,4,known?7:6);
    uint32_t x,z; std::memcpy(&x,&p.position.x,4); std::memcpy(&z,&p.position.z,4);
    put(b,8,x); put(b,12,z); put(b,16,p.hp); put(b,20,p.mushrooms);
    if(known)put(b,20,uint32_t(p.mushrooms) | (uint32_t(talk.quest1420.value_or(0))<<8) |
        (uint32_t(talk.quest1420.has_value())<<16) | (uint32_t(talk.flag5405.has_value())<<17) |
        (uint32_t(talk.flag5406.has_value())<<18) | (uint32_t(talk.flag5405.value_or(false))<<19) |
        (uint32_t(talk.flag5406.value_or(false))<<20));
    b[24]=p.crumpDefeated; b[25]=p.sound;b[26]=p.room;b[27]=int8_t(std::round(p.elevation*10));
    if(p.originalStory){put(b,28,uint32_t(*p.originalStory));b[32]=1;}
    b[33]=p.interiorMask;
    b[34]=uint8_t(p.originalFlag1207) | (uint8_t(p.originalFlag233)<<1) | (uint8_t(p.originalFlag236)<<2);
    b[35]=p.originalSaveMode;
    put(b,36,checksum(b,36));return b;
}
bool decodeSave(const SaveBytes& b, Progress& p) {
    const uint32_t version=get(b,4);
    if(std::memcmp(b.data(),"PM2D",4) || version<1 || version>7 ||
       (version>=3?get(b,36)!=checksum(b,36):get(b,28)!=checksum(b,28)))return false;
    if(version>=3 && (b[32]>1 || (version==3 && b[33]) ||
       (version<5?b[34]!=0:version==5?b[34]>1:(b[34]&~7u)!=0) ||
       (version<6 && b[35]) || (!b[32] && get(b,28))))return false;
    Progress out; uint32_t x=get(b,8),z=get(b,12);
    const uint32_t packed=get(b,20),mushrooms=version==7?packed&255u:packed;
    if(version==7) {
        if((packed&0xffe00000u) || (!(packed&0x10000u) && (packed&0xff00u)) ||
           (!(packed&0x20000u) && (packed&0x80000u)) || (!(packed&0x40000u) && (packed&0x100000u)))return false;
        if(packed&0x10000u)out.originalNpcTalkState.quest1420=uint8_t(packed>>8);
        if(packed&0x20000u)out.originalNpcTalkState.flag5405=(packed&0x80000u)!=0;
        if(packed&0x40000u)out.originalNpcTalkState.flag5406=(packed&0x100000u)!=0;
    }
    std::memcpy(&out.position.x,&x,4); std::memcpy(&out.position.z,&z,4);
    if(get(b,16)>10 || mushrooms>99 || b[24]>1 || b[25]>1 || (get(b,4)==1 && b[26]) || !roomAsset(b[26]) || (get(b,4)==1 && b[27]) || !std::isfinite(out.position.x) || !std::isfinite(out.position.z) || std::fabs(out.position.x)>100 || std::fabs(out.position.z)>100) return false;
    if(version>=3 && b[32]){const uint32_t word=get(b,28);out.originalStory=word<=0x7fffffff?int32_t(word):int32_t(int64_t(word)-0x100000000LL);}
    if(version>=4) {
        out.interiorMask=b[33];
        if(out.interiorMask && !((b[26]==0 && out.interiorMask==1) || (b[26]==2 && out.interiorMask==2)))return false;
    }
    if(version>=5)out.originalFlag1207=(b[34]&1)!=0;
    if(version>=6) {
        out.originalFlag233=(b[34]&2)!=0;out.originalFlag236=(b[34]&4)!=0;
        out.originalSaveMode=b[35];
    }
    out.room=b[26];out.elevation=int8_t(b[27])/10.f;out.hp=get(b,16); out.mushrooms=mushrooms; out.crumpDefeated=b[24];out.sound=b[25];p=out;return true;
}
LoadResult loadSave(const std::string& path,Progress& p) {
    if(read(path,p)) return LoadResult::Loaded;
    if(read(path+".bak",p)) return LoadResult::Recovered;
    return access(path.c_str(),F_OK)==0 || access((path+".bak").c_str(),F_OK)==0 ? LoadResult::Invalid : LoadResult::Missing;
}
bool writeSave(const std::string& path,const Progress& p) {
    const auto b=encodeSave(p); Progress checked;
    if(!decodeSave(b,checked)) return false;
    const std::string tmp=path+".tmp",bak=path+".bak";
    FILE* f=std::fopen(tmp.c_str(),"wb"); if(!f) return false;
    bool ok=std::fwrite(b.data(),1,b.size(),f)==b.size();
    if(std::fflush(f)!=0) ok=false;
    // libctru's filesystem backend closes/commits files; fsync is used on hosts.
#ifndef __3DS__
    if(fsync(fileno(f))!=0) ok=false;
#endif
    if(std::fclose(f)!=0) ok=false;
    if(!ok) {std::remove(tmp.c_str());return false;}
    Progress existing;
    if(read(path,existing)) {
        if(std::rename(path.c_str(),bak.c_str())!=0) {std::remove(tmp.c_str());return false;}
    }
    if(std::rename(tmp.c_str(),path.c_str())!=0) {std::remove(tmp.c_str());return false;}
    return true;
}
}

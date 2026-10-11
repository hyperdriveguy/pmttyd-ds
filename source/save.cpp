#include "save.hpp"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <unistd.h>
namespace ttyd {
namespace {
void put(SaveBytes& b, int offset, uint32_t v) { for (int i=0;i<4;++i) b[offset+i]=(v>>(i*8))&255; }
uint32_t get(const SaveBytes& b,int offset) { uint32_t v=0; for (int i=0;i<4;++i) v|=uint32_t(b[offset+i])<<(i*8); return v; }
uint32_t checksum(const SaveBytes& b) { uint32_t v=2166136261u; for(int i=0;i<28;++i) {v^=b[i];v*=16777619u;} return v; }
bool read(const std::string& path, Progress& p) {
    FILE* f=std::fopen(path.c_str(),"rb"); if(!f) return false;
    SaveBytes b{}; bool ok=std::fread(b.data(),1,b.size(),f)==b.size();
    ok=ok && std::fgetc(f)==EOF; std::fclose(f);
    return ok && decodeSave(b,p);
}
}
SaveBytes encodeSave(const Progress& p) {
    SaveBytes b{}; std::memcpy(b.data(),"PM2D",4); put(b,4,1);
    uint32_t x,z; std::memcpy(&x,&p.position.x,4); std::memcpy(&z,&p.position.z,4);
    put(b,8,x); put(b,12,z); put(b,16,p.hp); put(b,20,p.mushrooms);
    b[24]=p.crumpDefeated; b[25]=p.sound; put(b,28,checksum(b)); return b;
}
bool decodeSave(const SaveBytes& b, Progress& p) {
    if(std::memcmp(b.data(),"PM2D",4) || get(b,4)!=1 || get(b,28)!=checksum(b)) return false;
    Progress out; uint32_t x=get(b,8),z=get(b,12);
    std::memcpy(&out.position.x,&x,4); std::memcpy(&out.position.z,&z,4);
    if(get(b,16)>10 || get(b,20)>99 || b[24]>1 || b[25]>1 || b[26] || b[27] || !walkable(out.position)) return false;
    out.hp=get(b,16); out.mushrooms=get(b,20); out.crumpDefeated=b[24];out.sound=b[25];p=out;return true;
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

#include "uv_animation.hpp"
#include <cstdio>
#include <cmath>
#include <set>
namespace ttyd {
bool loadUVAnimation(const char* path,uint32_t vertices,std::vector<UVTrack>& result) {
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    char magic[4];uint32_t version=0,count=0;
    bool ok=std::fread(magic,1,4,f)==4 && magic[0]=='U' && magic[1]=='V' && magic[2]=='A' && magic[3]=='N' &&
        std::fread(&version,4,1,f)==1 && version==1 && std::fread(&count,4,1,f)==1 && count<=32;
    std::vector<UVTrack> tracks;std::set<uint32_t> bound;
    for(uint32_t n=0;ok && n<count;++n) {
        uint32_t frames=0,bindings=0;
        ok=std::fread(&frames,4,1,f)==1 && std::fread(&bindings,4,1,f)==1 && frames>0 && frames<=4096 && bindings>0 && bindings<=vertices;
        if(!ok)break;
        UVTrack track;track.frames.resize(frames);track.bindings.resize(bindings);
        ok=std::fread(track.frames.data(),sizeof(track.frames[0]),frames,f)==frames &&
            std::fread(track.bindings.data(),sizeof(UVBinding),bindings,f)==bindings;
        for(auto matrix:track.frames)for(float value:matrix)if(!std::isfinite(value))ok=false;
        for(auto b:track.bindings)if(b.vertex>=vertices || !std::isfinite(b.u) || !std::isfinite(b.v) || !bound.insert(b.vertex).second)ok=false;
        tracks.push_back(std::move(track));
    }
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);
    if(ok)result=std::move(tracks);
    return ok;
}
std::array<float,2> animatedUV(const UVTrack& track,const UVBinding& b,uint64_t ticks) {
    if(track.frames.empty())return {b.u,1-b.v};
    auto m=track.frames[ticks%track.frames.size()];
    return {m[0]*b.u+m[1]*b.v+m[2],1-(m[3]*b.u+m[4]*b.v+m[5])};
}
}

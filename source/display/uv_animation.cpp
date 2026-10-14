#include "uv_animation.hpp"
#include <cstdio>
#include <cmath>
#include <set>
#include <cstring>
namespace ttyd {
namespace {
template<class Track,class Valid>
bool loadTracks(const char* path,uint32_t vertices,std::vector<Track>& result,const char* expected,Valid valid) {
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    char magic[4];uint32_t version=0,count=0;
    bool ok=std::fread(magic,1,4,f)==4 && std::memcmp(magic,expected,4)==0 &&
        std::fread(&version,4,1,f)==1 && version==1 && std::fread(&count,4,1,f)==1 && count<=32;
    std::vector<Track> tracks;std::set<uint32_t> bound;
    for(uint32_t n=0;ok && n<count;++n) {
        uint32_t frames=0,bindings=0;
        ok=std::fread(&frames,4,1,f)==1 && std::fread(&bindings,4,1,f)==1 && frames>0 && frames<=4096 && bindings>0 && bindings<=vertices;
        if(!ok)break;
        Track track;track.frames.resize(frames);track.bindings.resize(bindings);
        ok=std::fread(track.frames.data(),sizeof(track.frames[0]),frames,f)==frames &&
            std::fread(track.bindings.data(),sizeof(track.bindings[0]),bindings,f)==bindings;
        for(auto matrix:track.frames)for(float value:matrix)if(!std::isfinite(value))ok=false;
        for(auto b:track.bindings)if(b.vertex>=vertices || !valid(b) || !bound.insert(b.vertex).second)ok=false;
        tracks.push_back(std::move(track));
    }
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);
    if(ok)result=std::move(tracks);
    return ok;
}
}
bool loadUVAnimation(const char* path,uint32_t vertices,std::vector<UVTrack>& result) {
    return loadTracks(path,vertices,result,"UVAN",[](UVBinding b){return std::isfinite(b.u) && std::isfinite(b.v);});
}
bool loadMeshAnimation(const char* path,uint32_t vertices,std::vector<MeshTrack>& result) {
    return loadTracks(path,vertices,result,"MSAN",[](MeshBinding b){return std::isfinite(b.x) && std::isfinite(b.y) && std::isfinite(b.z);});
}
std::array<float,3> animatedPosition(const MeshTrack& track,const MeshBinding& b,uint64_t ticks) {
    if(track.frames.empty())return {b.x,b.y,b.z};
    return transformedPosition(track.frames[ticks%track.frames.size()],b);
}
std::array<float,2> animatedUV(const UVTrack& track,const UVBinding& b,uint64_t ticks) {
    if(track.frames.empty())return {b.u,1-b.v};
    return transformedUV(track.frames[ticks%track.frames.size()],b);
}
}

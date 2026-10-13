#pragma once
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <utility>
#include <cstring>
#include <string>
#include <vector>
namespace ttyd {
struct ActorMeshVertex {float x,y,z,u,v;};
// Source ordinary scalar2/unit-scale/zero-pivot model matrix adaptation.
struct ActorRootTransform {
    std::array<float,3> root;
    float sine,cosine,scale,zScale;
    ActorRootTransform(std::array<float,3> position,float yaw,bool reflectZ,float modelScale=.02f):root(position),
        sine(std::sin(yaw*.01745329238474369f)),cosine(std::cos(yaw*.01745329238474369f)),scale(modelScale),zScale(reflectZ?-modelScale:modelScale) {}
    std::array<float,3> point(const ActorMeshVertex& v) const {
        const float x=v.x*scale,y=v.y*scale,z=v.z*zScale;
        return {root[0]+x*cosine+z*sine,root[1]+y,root[2]-x*sine+z*cosine};
    }
};
inline std::array<float,3> actorMeshPoint(const ActorMeshVertex& v,std::array<float,3> root,float yaw,bool reflectZ) {
    return ActorRootTransform(root,yaw,reflectZ).point(v);
}
struct ActorMeshPose {std::string name;std::vector<ActorMeshVertex> vertices;};
class ActorMeshPack {
public:
    const ActorMeshPose* pose(const char* name) const {
        for(const auto& p:poses)if(p.name==name)return &p;
        return nullptr;
    }
    bool load(const char* path) {
        FILE* file=std::fopen(path,"rb");if(!file)return false;
        const bool ok=read(file);std::fclose(file);return ok;
    }
    // Generated local little-endian pack on the supported native/host targets.
    bool read(FILE* file) {
        std::array<char,8> magic{};uint32_t count=0;
        if(std::fread(magic.data(),8,1,file)!=1 || std::memcmp(magic.data(),"PMCHAR1",8)!=0 ||
           std::fread(&count,4,1,file)!=1 || !count || count>64)return false;
        std::vector<ActorMeshPose> next;
        for(uint32_t n=0;n<count;++n) {
            std::array<char,32> name{};uint32_t size=0;
            if(std::fread(name.data(),32,1,file)!=1 || !name[0] || !std::memchr(name.data(),0,32) ||
               std::fread(&size,4,1,file)!=1 || !size || size%3 || size>8192)return false;
            ActorMeshPose p;p.name=name.data();
            for(const auto& previous:next)if(previous.name==p.name)return false;
            p.vertices.resize(size);
            if(std::fread(p.vertices.data(),sizeof(ActorMeshVertex),size,file)!=size)return false;
            for(const auto& v:p.vertices)for(float f:{v.x,v.y,v.z,v.u,v.v})if(!std::isfinite(f))return false;
            for(const auto& v:p.vertices)if(v.u<0 || v.u>1 || v.v<0 || v.v>1)return false;
            next.push_back(std::move(p));
        }
        if(std::fgetc(file)!=EOF)return false;
        poses=std::move(next);return true;
    }
private:
    std::vector<ActorMeshPose> poses;
};
static_assert(sizeof(ActorMeshVertex)==20,"Generated actor mesh vertex layout");
}

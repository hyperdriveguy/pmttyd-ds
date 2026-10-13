#include "opening_house_objects.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <set>
namespace ttyd {
bool OpeningHouseObjects::load(const char* path,uint32_t vertexCount){
    if(!path)return false;
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    char magic[8];uint32_t vertices=0,count=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMHOBJ01",8) &&
        std::fread(&vertices,4,1,f)==1 && vertices==vertexCount &&
        std::fread(&count,4,1,f)==1 && count==2;
    std::vector<Object> next;std::set<uint32_t> used;
    for(uint32_t n=0;ok && n<count;++n){
        Object o{};uint32_t bindings=0;
        ok=std::fread(&o.tag,4,1,f)==1 && o.tag==n && std::fread(&bindings,4,1,f)==1 && bindings>0 && bindings<=vertices &&
            std::fread(o.world.data(),4,12,f)==12;
        if(!ok)break;
        for(float v:o.world)if(!std::isfinite(v))ok=false;
        o.bindings.resize(bindings);
        for(auto& b:o.bindings){
            if(!ok)break;
            ok=std::fread(&b.index,4,1,f)==1 && b.index<vertices && used.insert(b.index).second &&
                std::fread(b.local.data(),4,3,f)==3 && std::fread(b.original.data(),4,3,f)==3;
            for(float v:b.local)if(!std::isfinite(v))ok=false;
            for(float v:b.original)if(!std::isfinite(v))ok=false;
        }
        next.push_back(std::move(o));
    }
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);
    if(ok)objects=std::move(next);
    return ok;
}
size_t OpeningHouseObjects::bindings() const {size_t n=0;for(const auto& o:objects)n+=o.bindings.size();return n;}
void OpeningHouseObjects::apply(float door,float postbox,const Writer& write) const {
    if(!write || !std::isfinite(door) || !std::isfinite(postbox))return;
    // evt_door multiplies its interpolation operand by Q1024(-568), then -1.
    const float angle=(door*.5546875f)*.01745329238474369f;
    const float c=std::cos(angle),s=std::sin(angle);
    for(const auto& o:objects)for(const auto& b:o.bindings){
        const float parameter=o.tag==0?door:postbox;
        if(parameter==0){write(b.index,b.original);continue;}
        Point p=b.local;
        if(o.tag==0){p[0]=c*b.local[0]+s*b.local[2];p[2]=-s*b.local[0]+c*b.local[2];}
        else p[0]+=postbox/10.f; // mapObjTranslate's source local-unit divisor.
        Point result{};
        for(unsigned r=0;r<3;++r)result[r]=o.world[r*4]*p[0]+o.world[r*4+1]*p[1]+o.world[r*4+2]*p[2]+o.world[r*4+3];
        write(b.index,result);
    }
}
}

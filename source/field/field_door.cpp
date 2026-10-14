#include "field_door.hpp"
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {std::vector<FieldDoorRecord> records;}
const std::vector<FieldDoorRecord>& fieldDoors(){return records;}
bool loadFieldDoors(const char* path) {
    FILE* f=std::fopen(path,"rb");if(!f)return false;
    char magic[8];uint32_t count=0;
    bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMDOOR1",8) &&
        std::fread(&count,4,1,f)==1 && count<=2;
    std::vector<FieldDoorRecord> next;
    for(uint32_t i=0;ok && i<count;++i) {
        FieldDoorRecord r;uint32_t nodes=0;
        ok=std::fread(&r.room,4,1,f)==1 && std::fread(&r.mask,4,1,f)==1 &&
            std::fread(&r.openNode,4,1,f)==1 && std::fread(&r.closeNode,4,1,f)==1 &&
            std::fread(r.outside.data(),12,1,f)==1 && std::fread(r.inside.data(),12,1,f)==1 &&
            std::fread(&nodes,4,1,f)==1 && nodes<=1024;
        if(!ok)break;
        if(!((r.room==0 && r.mask==1) || (r.room==2 && r.mask==2)) || !r.openNode || !r.closeNode || r.openNode==r.closeNode)ok=false;
        for(float p:r.outside)if(!std::isfinite(p) || std::fabs(p)>100)ok=false;
        for(float p:r.inside)if(!std::isfinite(p) || std::fabs(p)>100)ok=false;
        r.interiorNodes.resize(nodes);
        if(nodes && std::fread(r.interiorNodes.data(),4,nodes,f)!=nodes)ok=false;
        for(auto node:r.interiorNodes)if(!node)ok=false;
        for(const auto& other:next)if(r.mask==other.mask)ok=false;
        next.push_back(std::move(r));
    }
    ok=ok && std::fgetc(f)==EOF;std::fclose(f);
    if(ok)records.swap(next);
    return ok;
}
}

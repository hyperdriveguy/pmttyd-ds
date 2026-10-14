#include "stage_map_objects.hpp"
#include <cassert>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <unistd.h>
using namespace ttyd;
namespace {
using P=StageMapObjects::Point;using M=StageMapObjects::Matrix;
M matrix(float x=0,float y=0,float z=0){return {1,0,0,x,0,1,0,y,0,0,1,z};}
void close(float a,float b){assert(std::fabs(a-b)<.00003f);}
void fixture(const std::string& path) {
    auto* f=std::fopen(path.c_str(),"wb");assert(f);
    std::fwrite("PMMAP001",8,1,f);uint32_t vertices=6,count=3;std::fwrite(&vertices,4,1,f);std::fwrite(&count,4,1,f);
    for(unsigned i=0;i<3;++i) {
        int32_t parent=i?0:-1;char name[64]{};std::strcpy(name,i==0?"root":i==1?"prop":"sibling");const auto local=matrix(i==1?10:0);const uint32_t n=i?3:0;
        std::fwrite(&parent,4,1,f);std::fwrite(name,64,1,f);std::fwrite(local.data(),4,12,f);std::fwrite(&n,4,1,f);
        for(unsigned j=0;j<n;++j){uint32_t index=(i-1)*3+j;P p{float(j),1,2};std::fwrite(&index,4,1,f);std::fwrite(p.data(),4,3,f);}
    }
    std::fclose(f);
}
}
int main(int argc,char**) {
    StageMapObjects map;
    if(argc>1) {
        assert(map.load("romfs/stg_00_1.mapobj"));assert(map.vertices()==3393);
        assert(map.flag("A2",1,true));assert(*map.flags("isi_1")&1);assert(map.flag("isi_1",1,false));assert(!(*map.flags("isi_1")&1) && (*map.flags("isi_2")&1));
        assert(map.flag("A2",1,false));
        assert(map.translate("isi_1",{-202,0,-17}) && map.translate("isi_2",{200,0,-30}) && map.translate("A1",{0,0,-50}));map.recalculate();
        FILE* f=std::fopen("romfs/stg_00_1.vertices","rb");assert(f);unsigned checked=0;
        map.apply([&](uint32_t index,const P& p){float original[3];assert(std::fseek(f,index*36,SEEK_SET)==0 && std::fread(original,4,3,f)==3);for(unsigned a=0;a<3;++a)close(p[a],original[a]);++checked;});
        assert(checked==3393);std::fclose(f);
        assert(map.load("romfs/bti_01.mapobj") && map.vertices()==11721 && map.flags("spotlight") && map.flags("escape_wall"));return 0;
    }
    const std::string path="/tmp/ttyd-stage-map-"+std::to_string(getpid());fixture(path);assert(map.load(path.c_str()));std::remove(path.c_str());
    assert(std::string(map.root())=="root");close((*map.world("prop"))[3],1);const auto original=map.revision();
    assert(map.translate("prop",{20,0,0}) && map.rotate("prop",{0,0,90}));assert(*map.flags("prop")&0x200);map.recalculate();
    assert(!(*map.flags("prop")&0x200) && (*map.flags("prop")&0x200000));assert(map.revision()==original+1);close((*map.world("prop"))[3],1.2f);
    map.apply([](uint32_t index,const P& p){if(!index){close(p[0],1.1f);close(p[1],0);close(p[2],.2f);}});
    map.recalculate();assert(map.revision()==original+1);
    assert(map.translate("prop",{20,0,0}));map.recalculate();close((*map.world("prop"))[3],1.2f);close((*map.world("prop"))[0],.1f); // Reset after source latch clear, no accumulation.
    assert(map.translate("prop",{10,0,0}) && map.translate("prop",{20,0,0}));map.recalculate();close((*map.world("prop"))[3],1.3f);
    assert(map.flag("root",1,true) && (*map.flags("prop")&1) && (*map.flags("sibling")&1));assert(map.flag("prop",1,false) && !(*map.flags("prop")&1) && (*map.flags("root")&1));
    unsigned vertices=0;map.visibleRanges(0,6,[&](uint32_t first,uint32_t count){assert(first==0 && count==3);vertices+=count;});assert(vertices==3);
    assert(map.flag("root",1,false));assert(map.flag("prop",1,true) && !(*map.flags("sibling")&1)); // Target siblings excluded.
    vertices=0;map.visibleRanges(0,6,[&](uint32_t first,uint32_t count){assert(first==3 && count==3);vertices+=count;});assert(vertices==3);
    assert(!map.translate("missing",{}) && !map.flags("missing"));
}

#include "opening_house_objects.hpp"
#include "opening_ship_objects.hpp"
#include "vertex_upload_range.hpp"
#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

namespace {
using Vertex=std::array<float,9>;
std::vector<Vertex> readVertices(const char* name) {
    const auto path=std::string("romfs/")+name+".vertices";
    FILE* file=std::fopen(path.c_str(),"rb");assert(file);
    std::fseek(file,0,SEEK_END);const auto bytes=std::ftell(file);std::rewind(file);
    assert(bytes>0 && size_t(bytes)%sizeof(Vertex)==0);
    std::vector<Vertex> result(size_t(bytes)/sizeof(Vertex));
    assert(std::fread(result.data(),1,size_t(bytes),file)==size_t(bytes));std::fclose(file);
    return result;
}
template<class Apply> void check(const char* name,std::vector<Vertex>& cpu,const Apply& apply) {
    auto gpu=cpu;ttyd::VertexUploadRanges ranges;
    apply([&](uint32_t index,const std::array<float,3>& point) {
        assert(index<cpu.size());
        for(unsigned i=0;i<3;++i)cpu[index][i]=point[i];ranges.include(index);
    });
    size_t bytes=0,calls=0;
    ranges.flush([&](uint32_t first,uint32_t count) {
        assert(size_t(first)+count<=cpu.size());bytes+=count*sizeof(Vertex);++calls;
        std::memcpy(gpu.data()+first,cpu.data()+first,count*sizeof(Vertex));
    });
    // Match a full-buffer flush, including UV/color fields outside changed XYZ.
    assert(std::memcmp(cpu.data(),gpu.data(),cpu.size()*sizeof(Vertex))==0);
    assert(bytes<cpu.size()*sizeof(Vertex));
    std::printf("%s: %zu bytes / %zu full bytes, %zu ranges\n",name,bytes,cpu.size()*sizeof(Vertex),calls);
}
}
int main() {
    auto house=readVertices("opening-aaa_00");ttyd::OpeningHouseObjects objects;
    assert(objects.load("romfs/opening-aaa_00.houseobj",house.size()));
    for(float parameter:{0.f,20.f,-40.f,0.f})check("house",house,[&](const auto& writer){objects.apply(parameter,parameter*.5f,-parameter,writer);});
    for(const char* name:{"opening-gor_00","opening-muj_20"}) {
        auto vertices=readVertices(name);ttyd::OpeningShipObjects ship;
        assert(ship.load((std::string("romfs/")+name+".shipobj").c_str(),vertices.size()));
        for(float parameter:{0.f,20.f,-40.f,0.f}) {
            const std::array<float,4> parts{{parameter,parameter*.5f,-parameter,parameter*.25f}};
            check(name,vertices,[&](const auto& writer){
                if(std::strcmp(name,"opening-muj_20")==0)ship.applyVoyage({parameter,1,2},parameter,parameter*.1f,parts,writer);
                else ship.apply(parameter,2,parameter-3,parameter,parameter*.1f,parts,writer);
            });
        }
    }
}

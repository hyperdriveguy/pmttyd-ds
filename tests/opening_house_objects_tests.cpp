#include "opening_house_objects.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <unistd.h>
using namespace ttyd;
int main(int argc,char** argv){
    const auto path=std::filesystem::temp_directory_path()/("house-objects-"+std::to_string(getpid()));
    auto fixture=[&](bool duplicate=false){
        FILE* f=std::fopen(path.c_str(),"wb");assert(f);
        auto u=[&](uint32_t v){std::fwrite(&v,4,1,f);};auto fs=[&](const float* v,unsigned n){std::fwrite(v,4,n,f);};
        std::fwrite("PMHOBJ01",8,1,f);u(4);u(2);
        const float matrix[]={.1f,0,0,2,0,.1f,0,3,0,0,.1f,-4};
        const float local[]={10,0,0},original[]={3,3,-4};
        for(unsigned i=0;i<2;++i){u(i);u(1);fs(matrix,12);u(duplicate?0:i);fs(local,3);fs(original,3);}
        std::fclose(f);
    };
    fixture();OpeningHouseObjects pack;assert(pack.load(path.c_str(),4));assert(pack.size()==2 && pack.bindings()==2);
    std::array<OpeningHouseObjects::Point,4> vertices{};
    auto write=[&](uint32_t i,const auto& xyz){vertices[i]=xyz;};
    pack.apply(180,2,write);
    const float angle=99.84375f*.01745329238474369f;
    assert(std::fabs(vertices[0][0]-(2+std::cos(angle)))<1e-6f);
    assert(std::fabs(vertices[0][2]-(-4-std::sin(angle)))<1e-6f);
    assert(std::fabs(vertices[1][0]-3.02f)<1e-6f);
    const auto once=vertices;pack.apply(180,2,write);assert(vertices==once);
    pack.apply(0,0,write);assert((vertices[0]==OpeningHouseObjects::Point{3,3,-4}));assert(vertices[1]==vertices[0]);
    assert(!pack.load(path.c_str(),5));assert(pack.bindings()==2);
    fixture(true);assert(!pack.load(path.c_str(),4));assert(pack.bindings()==2);
    std::filesystem::remove(path);
    if(argc>1){
        OpeningHouseObjects original;assert(original.load(argv[1],25626));assert(original.bindings()==882);
        unsigned changed=0,reset=0;original.apply(180,2,[&](uint32_t i,const auto& p){assert(i<25626);for(float x:p)assert(std::isfinite(x));++changed;});
        original.apply(0,0,[&](uint32_t,const auto&){++reset;});assert(changed==reset && changed==original.bindings());
        std::printf("original door/postbox bindings:%u\n",changed);
    }
}

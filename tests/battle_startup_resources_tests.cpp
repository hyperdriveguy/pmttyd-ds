#include "battle_startup_resources.hpp"
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace ttyd;
int main(int argc,char**argv){
    BattleStartupResources missing;assert(!missing.load("/missing-original-startup") && !missing.ready());
    assert(!missing.paperReady(0x802efbe4,1) && !missing.niceReady(1));
    assert(!missing.admitTexture(0,[](unsigned,const auto&){return true;}));
    if(argc<2)return 0;
    BattleStartupResources resources;assert(resources.load(argv[1]) && resources.ready());
    unsigned descriptors=0,supported=0;
    for(unsigned i=0;i<9;++i){const auto* r=resources.resource(i);assert(r && !r->originalBank.empty());descriptors+=r->descriptors.size();
        assert(r->needsCapture==(i<3));for(auto& p:r->descriptors)supported+=p.supported;
        assert(!resources.resourceReady(r->sourceKey,1) && !resources.resourceReady(r->sourceKey,0));
    }
    assert(descriptors==25 && supported==23);
    const auto* accordion=resources.resource(1);assert(accordion->textureOwner=="p_slit");
    unsigned unsupported=0;for(auto& p:accordion->descriptors)if(!p.supported){++unsupported;assert(p.name=="PM_J_3A" || p.name=="PM_J_1F");assert(!accordion->animations.pose(p.name.c_str()));}assert(unsupported==2);
    assert(!resources.admitTexture(0,[](unsigned,const auto&){return false;}) && !resources.paperReady(0x802efbe4,1));
    unsigned calls=0;
    auto readTexture=[&](unsigned index,const std::string& path){assert(index==calls++);FILE* f=std::fopen(path.c_str(),"rb");assert(f);assert(std::fgetc(f)!=EOF);std::fclose(f);return true;};
    for(unsigned i=0;i<9;++i){assert(resources.admitTexture(i,readTexture));if(i<3)assert(resources.paperReady(resources.resource(i)->sourceKey,1));if(i<8)assert(!resources.niceReady(1));}
    assert(calls==9 && resources.niceReady(1) && !resources.niceReady(0) && !resources.niceReady(2));
    assert(!resources.paperReady(0x802f3d00,1) && !resources.paperReady(0x802efbe4,0) && !resources.resourceReady(0,1));
    assert(resources.admitTexture(0,readTexture) && calls==9);
    resources.releaseTexture(8);assert(!resources.niceReady(1) && resources.paperReady(0x802efbf8,1));
    assert(!resources.load("/missing-original-startup") && resources.paperReady(0x802efbf8,1));
    const auto bad=std::filesystem::temp_directory_path()/("battle-startup-bad-"+std::to_string(getpid()));std::filesystem::create_directory(bad);
    std::ifstream input(std::string(argv[1])+"/battle-startup-resources.bin",std::ios::binary);std::vector<char> bytes((std::istreambuf_iterator<char>(input)),{});assert(bytes.size()>200);
    for(unsigned i=0;i<9;++i){const auto file="battle-ready-"+resources.resource(i)->name+".pmanim";std::filesystem::create_symlink(std::filesystem::absolute(std::filesystem::path(argv[1])/file),bad/file);}
    bytes[bytes.size()-1]^=1;std::ofstream output(bad/"battle-startup-resources.bin",std::ios::binary);output.write(bytes.data(),bytes.size());output.close();
    assert(!resources.load(bad.c_str()) && resources.paperReady(0x802efbf8,1));std::filesystem::remove_all(bad);
    resources.clear();assert(!resources.ready() && !resources.resource(0));
    std::puts("Original9-bank/25-descriptor/23-pose admission; source keys/context/GPU callback gates passed");
}

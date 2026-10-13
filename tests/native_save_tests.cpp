#include "save.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <unistd.h>
using namespace ttyd;
static void checksum(SaveBytes& bytes,int length=36) {
    uint32_t sum=2166136261u;
    for(int i=0;i<length;++i){sum^=bytes[i];sum*=16777619u;}
    for(int i=0;i<4;++i)bytes[length+i]=uint8_t(sum>>(8*i));
}
int main() {
    Progress p;p.room=0;p.interiorMask=1;p.originalStory=8;p.position={1.06f,-1.24f};
    auto bytes=encodeSave(p);Progress out;assert(bytes[4]==4 && bytes[33]==1);
    assert(decodeSave(bytes,out) && out.room==0 && out.interiorMask==1 && out.originalStory==8);
    p.room=2;p.interiorMask=2;p.originalStory.reset();
    assert(decodeSave(encodeSave(p),out) && out.interiorMask==2 && !out.originalStory);
    p.room=1;assert(!decodeSave(encodeSave(p),out));
    p.room=0;p.interiorMask=2;assert(!decodeSave(encodeSave(p),out));
    p.interiorMask=3;assert(!decodeSave(encodeSave(p),out));
    const Progress previous=out;
    for(int field:{33,34,35}) {
        auto bad=bytes;bad[field]=255;checksum(bad);
        assert(!decodeSave(bad,out) && out.interiorMask==previous.interiorMask && out.room==previous.room);
    }
    p.room=0;p.interiorMask=0;p.originalStory=8;
    auto legacy=encodeSave(p);legacy[4]=3;checksum(legacy);
    assert(decodeSave(legacy,out) && out.interiorMask==0 && out.originalStory==8);
    auto invalidLegacy=legacy;invalidLegacy[33]=1;checksum(invalidLegacy);
    assert(!decodeSave(invalidLegacy,out));
    char temporary[]="/tmp/pm-door-save-XXXXXX";const int fd=mkstemp(temporary);assert(fd>=0);close(fd);
    const std::string path=temporary;
    {
        std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(legacy.data()),legacy.size());
    }
    assert(loadSave(path,out)==LoadResult::Loaded && out.interiorMask==0 && out.originalStory==8);
    p.interiorMask=1;assert(writeSave(path,p));
    assert(loadSave(path,out)==LoadResult::Loaded && out.interiorMask==1 && out.originalStory==8);
    {std::ofstream f(path,std::ios::binary|std::ios::trunc);f.write("broken",6);}
    assert(loadSave(path,out)==LoadResult::Recovered && out.interiorMask==0 && out.originalStory==8);
    for(unsigned version:{1u,2u}) {
        auto old=legacy;old[4]=version;checksum(old,28);
        assert(decodeSave(old,out) && out.interiorMask==0 && !out.originalStory);
    }
    std::remove(path.c_str());std::remove((path+".bak").c_str());
    std::puts("Native v4 interior state, legacy saves, validation and recovery passed");
}

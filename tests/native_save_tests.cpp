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
    auto bytes=encodeSave(p);Progress out;assert(bytes[4]==6 && bytes[33]==1);
    assert(decodeSave(bytes,out) && out.room==0 && out.interiorMask==1 && out.originalStory==8);
    for(int quest:{-1,0,2,13,255})for(int a:{-1,0,1})for(int b:{-1,0,1}) {
        auto state=p;if(quest>=0)state.originalNpcTalkState.quest1420=uint8_t(quest);
        if(a>=0)state.originalNpcTalkState.flag5405=a!=0;if(b>=0)state.originalNpcTalkState.flag5406=b!=0;
        const auto encoded=encodeSave(state);assert(decodeSave(encoded,out));
        assert(out.originalNpcTalkState.quest1420==state.originalNpcTalkState.quest1420 &&
               out.originalNpcTalkState.flag5405==state.originalNpcTalkState.flag5405 &&
               out.originalNpcTalkState.flag5406==state.originalNpcTalkState.flag5406 && out.mushrooms==p.mushrooms);
    }
    auto fresh=p;fresh.originalNpcTalkState={uint8_t(0),false,false};const auto v7=encodeSave(fresh);assert(v7[4]==7);
    for(unsigned bit:{8u,19u,20u,21u,31u}) {
        auto corrupt=bytes;corrupt[4]=7;corrupt[20+bit/8]|=uint8_t(1u<<(bit%8));checksum(corrupt);
        assert(!decodeSave(corrupt,out));
    }
    fresh.mushrooms=65536;assert(!decodeSave(encodeSave(fresh),out));
    for(unsigned version=1;version<=6;++version) {
        auto legacyState=bytes;legacyState[4]=uint8_t(version);legacyState[33]=0;
        checksum(legacyState,version<3?28:36);out.originalNpcTalkState={uint8_t(13),true,true};
        assert(decodeSave(legacyState,out) && !out.originalNpcTalkState.quest1420 &&
               !out.originalNpcTalkState.flag5405 && !out.originalNpcTalkState.flag5406);
    }
    p.originalFlag1207=true;
    const auto departed=encodeSave(p);assert(departed[34]==1 && decodeSave(departed,out) && out.originalFlag1207);
    for(unsigned bits=0;bits<8;++bits)for(unsigned mode:{0u,1u,9u,255u}) {
        p.originalFlag1207=bits&1;p.originalFlag233=bits&2;p.originalFlag236=bits&4;p.originalSaveMode=mode;
        const auto saved=encodeSave(p);
        assert(saved[34]==bits && saved[35]==mode && decodeSave(saved,out));
        assert(out.originalFlag1207==p.originalFlag1207 && out.originalFlag233==p.originalFlag233 &&
               out.originalFlag236==p.originalFlag236 && out.originalSaveMode==mode);
    }
    auto v5=departed;v5[4]=5;checksum(v5);
    assert(decodeSave(v5,out) && out.originalFlag1207 && !out.originalFlag233 && !out.originalFlag236 && !out.originalSaveMode);
    for(int field:{34,35}){auto invalid=v5;invalid[field]=2;checksum(invalid);assert(!decodeSave(invalid,out));}
    p.originalFlag233=p.originalFlag236=false;p.originalSaveMode=0;
    auto v4=bytes;v4[4]=4;checksum(v4);assert(decodeSave(v4,out) && !out.originalFlag1207 && out.interiorMask==1);
    v4[34]=1;checksum(v4);assert(!decodeSave(v4,out));
    p.originalFlag1207=false;
    p.room=2;p.interiorMask=2;p.originalStory.reset();
    assert(decodeSave(encodeSave(p),out) && out.interiorMask==2 && !out.originalStory);
    p.room=1;assert(!decodeSave(encodeSave(p),out));
    p.room=0;p.interiorMask=2;assert(!decodeSave(encodeSave(p),out));
    p.interiorMask=3;assert(!decodeSave(encodeSave(p),out));
    const Progress previous=out;
    for(int field:{33,34}) {
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
    std::puts("Native v6 source save switches, original flags/interior state, legacy saves and recovery passed");
}

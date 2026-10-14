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
        std::ofstream f(path,std::ios::binary);f.write(reinterpret_cast<const char*>(legacy.data()),40);
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
    p.originalPouch=std::make_shared<ProloguePouch>();p.hp=25;
    for(unsigned i=0;i<p.originalPouch->bytes.size();++i)p.originalPouch->bytes[i]=uint8_t(i*73+19);
    p.originalPouch->half(0x70,25);p.originalPouch->half(0x92,-12);
    auto frozen=snapshotSaveProgress(p);const auto captured=encodeSave(frozen);
    assert(frozen.originalPouch!=p.originalPouch && frozen.originalPouch->bytes==p.originalPouch->bytes);
    const auto item=p.originalPouch->half(0xa0);p.originalPouch->half(0xa0,107);
    assert(encodeSave(frozen)==captured);p.originalPouch->half(0xa0,item);
    for(int flag:{-1,0,1}) {
        p.originalNpcTalkState.flag5407.reset();if(flag>=0)p.originalNpcTalkState.flag5407=flag!=0;
        const auto saved=encodeSave(p);assert(saved[4]==8 && decodeSave(saved,out));
        assert(out.hp==25 && out.originalPouch && out.originalPouch!=p.originalPouch && out.originalPouch->bytes==p.originalPouch->bytes);
        assert(out.originalNpcTalkState.flag5407==p.originalNpcTalkState.flag5407);
        const auto retained=out.originalPouch;
        for(unsigned at:{0u,20u,40u,100u,1531u,1535u}) {
            auto bad=saved;bad[at]^=1;assert(!decodeSave(bad,out) && out.originalPouch==retained);
        }
    }
    auto flags=p;flags.originalPouch.reset();flags.hp=10;
    flags.originalNpcTalkState.flag5407=false;auto onlyFlags=encodeSave(flags);
    assert(decodeSave(onlyFlags,out) && !out.originalPouch && out.originalNpcTalkState.flag5407==false);
    auto bad=onlyFlags;bad[40]=1;checksum(bad,1532);assert(!decodeSave(bad,out));
    bad=onlyFlags;bad[22]&=uint8_t(~0x20u);bad[22]|=0x40;checksum(bad);checksum(bad,1532);assert(!decodeSave(bad,out));
    flags.originalNpcTalkState.flag5407.reset();const auto v7legacy=encodeSave(flags);assert(v7legacy[4]==6);
    out.originalPouch=p.originalPouch;out.originalNpcTalkState.flag5407=true;
    assert(decodeSave(v7legacy,out) && !out.originalPouch && !out.originalNpcTalkState.flag5407);
    assert(writeSave(path,p) && loadSave(path,out)==LoadResult::Loaded && out.originalPouch->bytes==p.originalPouch->bytes);
    {std::ifstream f(path,std::ios::binary|std::ios::ate);assert(f.tellg()==1536);}
    const auto first=p.originalPouch->bytes;p.originalPouch->half(0xa0,107);p.originalNpcTalkState.flag5407=true;
    assert(writeSave(path,p) && loadSave(path,out)==LoadResult::Loaded && out.originalNpcTalkState.flag5407==true && out.originalPouch->half(0xa0)==107);
    const auto extended=encodeSave(p);
    for(unsigned size:{40u,1535u,1537u}) {
        std::ofstream f(path,std::ios::binary|std::ios::trunc);
        f.write(reinterpret_cast<const char*>(extended.data()),size>1536?1536:size);if(size>1536)f.put(0);f.close();
        assert(loadSave(path,out)==LoadResult::Recovered && out.originalPouch->bytes==first);
    }
    std::remove(path.c_str());std::remove((path+".bak").c_str());
    // Completion5376 is independent of quest/progress and never inferred from
    // legacy saves. A new version preserves v8's reserved-bit contract.
    for(bool pouch:{false,true})for(int completion:{-1,0,1}) {
        auto state=flags;state.originalPouch=pouch?p.originalPouch:nullptr;
        state.originalNpcTalkState.quest1420=2;state.originalNpcTalkState.flag5406=true;
        if(completion>=0)state.originalNpcTalkState.flag5376=completion!=0;
        const auto saved=encodeSave(state);assert(saved[4]==(completion>=0?9:pouch?8:7));
        out.originalNpcTalkState.flag5376=true;
        assert(decodeSave(saved,out) && out.originalNpcTalkState.flag5376==state.originalNpcTalkState.flag5376);
        assert(out.originalNpcTalkState.quest1420==2 && out.originalNpcTalkState.flag5406==true);
        assert(bool(out.originalPouch)==pouch);
        if(pouch)assert(out.originalPouch->bytes==p.originalPouch->bytes);
        assert(writeSave(path,state) && loadSave(path,out)==LoadResult::Loaded);
        assert(out.originalNpcTalkState.flag5376==state.originalNpcTalkState.flag5376);
        {std::ifstream f(path,std::ios::binary|std::ios::ate);assert(f.tellg()==(completion>=0 || pouch?1536:40));}
    }
    auto completion=flags;completion.originalNpcTalkState.flag5376=true;
    const auto v9=encodeSave(completion);assert(v9[4]==9);
    for(unsigned version=1;version<=8;++version) {
        auto old=bytes;old[4]=version;old[33]=0;
        checksum(old,version<3?28:36);if(version==8)checksum(old,1532);
        out.originalNpcTalkState.flag5376=true;
        assert(decodeSave(old,out) && !out.originalNpcTalkState.flag5376.has_value());
    }
    for(unsigned bit:{24u,25u,26u,31u}) {
        auto old=onlyFlags;old[20+bit/8]|=uint8_t(1u<<(bit%8));checksum(old);checksum(old,1532);
        assert(!decodeSave(old,out)); // v8 rejects new bits even with valid checksums.
    }
    for(unsigned bit:{26u,27u,28u,29u,30u,31u}) {
        auto bad=v9;bad[20+bit/8]|=uint8_t(1u<<(bit%8));checksum(bad);checksum(bad,1532);
        assert(!decodeSave(bad,out));
    }
    auto valueOnly=v9;valueOnly[23]&=uint8_t(~1u);checksum(valueOnly);checksum(valueOnly,1532);
    assert(!decodeSave(valueOnly,out));
    auto payload=v9;payload[40]=1;checksum(payload,1532);assert(!decodeSave(payload,out));
    for(unsigned at:{20u,36u,1532u,1535u}) {
        auto bad=v9;bad[at]^=1;assert(!decodeSave(bad,out));
    }
    assert(writeSave(path,completion));completion.originalNpcTalkState.flag5376=false;
    assert(writeSave(path,completion));
    for(unsigned size:{40u,1535u,1537u}) {
        std::ofstream f(path,std::ios::binary|std::ios::trunc);
        f.write(reinterpret_cast<const char*>(v9.data()),size>1536?1536:size);if(size>1536)f.put(0);f.close();
        assert(loadSave(path,out)==LoadResult::Recovered && out.originalNpcTalkState.flag5376==true);
    }
    std::remove(path.c_str());std::remove((path+".bak").c_str());
    std::puts("Native v9 completion5376, v8 pouch/5407, legacy unknown migration, corruption and recovery passed");
}

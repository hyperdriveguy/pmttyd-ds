#include "battle_stage_objects.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
float number(uint32_t word) {float value;std::memcpy(&value,&word,4);return value;}
}
bool packedBattleStageObjectMain(const BattleStageObjectServices& s) {
    const auto& m=s.actors.memory;
    assert(m.word && m.half && m.storeWord && s.translate && s.rotate && s.groupFlags && s.groupFlag);
    const auto b=currentBattleAddress(s.actors);
    auto real=[&](uint32_t a){return number(m.word(m.user,a));};
    auto copy=[&](uint32_t a,uint32_t cache) {
        // Original loads X/Y before either store, then reloads Z.
        const auto x=m.word(m.user,a),y=m.word(m.user,a+4);
        m.storeWord(m.user,cache,x);m.storeWord(m.user,cache+4,y);
        m.storeWord(m.user,cache+8,m.word(m.user,a+8));
    };
    auto different=[&](uint32_t a,uint32_t cache) {
        for(unsigned axis=0;axis<3;++axis)if(real(a+4*axis)!=real(cache+4*axis))return true;
        return false;
    };
    auto point=[&](uint32_t a){return BattleStageObjectServices::Point{real(a),real(a+4),real(a+8)};};
    for(unsigned i=0;i<32;++i) {
        const auto r=b+0x1715c+0x7c*i,ordinal=m.word(m.user,r);
        if(ordinal==0 || ordinal>=0x80000000)continue;
        bool dirty=false;
        for(const auto offset:{4u,0x10u,0x28u,0x1cu})
            if(different(r+offset,r+offset+0x30)){copy(r+offset,r+offset+0x30);dirty=true;}
        const auto flags=m.word(m.user,r+0x68);
        if(flags&2){m.storeWord(m.user,r+0x68,flags&~uint32_t(2));dirty=true;}
        const auto descriptor=m.word(m.user,r+0x64);
        if(m.half(m.user,descriptor+6)==2 && different(b+0x180e0,b+0x180ec)) {
            copy(b+0x180e0,b+0x180ec);dirty=true;
        }
        const auto layer=m.half(m.user,m.word(m.user,r+0x64)+6);
        auto name=[&](){return m.word(m.user,m.word(m.user,r+0x64));};
        if(dirty && layer<8) {
            BattleStageObjectServices::Point total{};
            for(unsigned axis=0;axis<3;++axis) {
                const float first=real(r+4+4*axis)+real(r+0x10+4*axis);
                total[axis]=real(r+0x28+4*axis)+first;
            }
            if(!s.translate(s.user,name(),total))return false;
            if(layer==2 || layer==3) {
                const auto pivot=r+(layer==2?0x14:8);
                if(!s.translate(s.user,name(),{0,-real(pivot),0}))return false;
                if(!s.rotate(s.user,name(),point(layer==2?b+0x180e0:r+0x1c)))return false;
                if(!s.translate(s.user,name(),{0,real(pivot),0}))return false;
            } else if(!s.rotate(s.user,name(),point(r+0x1c)))return false;
        }
        const bool hidden=(m.word(m.user,r+0x68)&1)!=0;
        const auto actual=s.groupFlags(s.user,name());
        if(!actual)return false;
        if(((*actual&1)!=0)!=hidden && !s.groupFlag(s.user,name(),1,hidden))return false;
    }
    // Final BattleObjectConfig80149794 is a verified complete blr.
    return true;
}
}

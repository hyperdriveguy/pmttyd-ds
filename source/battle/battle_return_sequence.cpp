#include "battle_return_sequence.hpp"
#include "battle_main.hpp"
#include <cassert>

namespace ttyd {
// Guest work+4 state,+10 counter. State5 checks live global battle end bit;
// on success reset counter, increment freshly read state, then GP+c may select11.
// Always run state5 tail, even on ready0; state6 never falls through same call.
bool packedBattleReturnSequence(uint32_t w,const BattleReturnSequenceServices& s) {
    const auto& m=s.actors.memory;
    assert(m.word && m.storeWord);
    const auto state=m.word(m.user,w+4);
    if(state!=5 && state!=6)return false;
    if(state==5) {
        assert(s.tail);
        if(packedBattleSequenceEndCheck(s.actors)) {
            m.storeWord(m.user,w+0x10,0);
            m.storeWord(m.user,w+4,m.word(m.user,w+4)+uint32_t(1));
            const auto gp=m.word(m.user,0x80415fd0);
            if(m.word(m.user,gp+0xc))m.storeWord(m.user,w+4,11);
        }
        s.tail(s.user);
    // U32 increment then signed>30. GP flags1000+2000 BOTH set select
    // seq5, otherwise2; parameter1, data0. Do not replace with >=30 or milliseconds.
    } else {
        const auto counter=m.word(m.user,w+0x10)+uint32_t(1);
        m.storeWord(m.user,w+0x10,counter);
        // Signed compare without implementation-defined unsigned-to-int cast.
        if(counter>30 && counter<0x80000000) {
            assert(s.setSequence);
            const auto gp=m.word(m.user,0x80415fd0),flags=m.word(m.user,gp);
            s.setSequence(s.user,(flags&0x3000)==0x3000?5:2,1,0);
        }
    }
    return true;
}
// Field state0 only: GP mask10 requests fade18/900ms/000000ff and source
// event80414f28, then clears mask through a fresh GP. Otherwise GP118 selects
// fade type/duration at108/10c instead of f8/fc. Fresh GP per default store
// resets BOTH pairs to type9/duration300, then increments fresh state word.
// Calls may mutate GP/work; no state1 listener executes by fallthrough.
bool packedFieldSequenceIncomingFade(uint32_t w,const FieldSequenceFadeServices& s) {
    const auto& m=s.memory;
    assert(m.word && m.storeWord);
    if(m.word(m.user,w+4)!=0)return false;
    assert(s.fade);
    const auto gp=m.word(m.user,0x80415fd0);
    if(m.word(m.user,gp)&0x10) {
        assert(s.event);
        s.fade(s.user,18,900,0xff);
        s.event(s.user,0x80414f28,0,0,0);
        const auto current=m.word(m.user,0x80415fd0);
        m.storeWord(m.user,current,m.word(m.user,current)&~uint32_t(0x10));
    } else {
        const auto offset=m.word(m.user,gp+0x118)?0x108u:0xf8u;
        const auto type=m.word(m.user,gp+offset),duration=m.word(m.user,gp+offset+4);
        s.fade(s.user,type,duration,0xff);
    }
    for(const auto offset:{0xf8u,0xfcu,0x108u,0x10cu}) {
        const auto current=m.word(m.user,0x80415fd0);
        m.storeWord(m.user,current+offset,(offset==0xf8 || offset==0x108)?9:300);
    }
    m.storeWord(m.user,w+4,m.word(m.user,w+4)+uint32_t(1));
    return true;
}
}

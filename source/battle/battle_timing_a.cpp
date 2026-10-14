#include "battle_timing_a.hpp"
#include <cstddef>

namespace ttyd {
namespace {
// Original threshold/sample pairs indexed by difficulty0..6. A count equal
// to threshold still uses that row;99999 is the last-row threshold,100000
// the explicit exhausted-window cutoff. Samples mean newest history entries.
struct Window { int threshold, samples; };
constexpr Window windows0[]={{4,10},{5,9},{6,8},{7,7},{8,6},{9,5},{99999,1}};
constexpr Window windows1[]={{0,10},{1,9},{2,8},{3,7},{4,6},{5,5},{6,4},{7,4},{8,3},{9,2},{99999,1}};
constexpr Window windows2[]={{0,9},{1,8},{2,7},{3,6},{4,5},{5,4},{6,3},{7,2},{8,1},{9,1},{99999,1}};
constexpr Window windows3[]={{0,8},{1,7},{2,6},{3,5},{4,4},{5,3},{6,2},{8,1},{9,1},{99999,1}};
constexpr Window windows4[]={{0,7},{1,6},{2,5},{3,4},{4,3},{5,2},{8,1},{9,1},{99999,1}};
constexpr Window windows5[]={{0,6},{1,5},{2,4},{3,3},{4,2},{8,1},{9,1},{99999,1}};
constexpr Window windows6[]={{0,5},{1,4},{2,3},{3,2},{8,1},{9,1},{99999,1}};
constexpr const Window* windows[]={windows0,windows1,windows2,windows3,windows4,windows5,windows6};
// Preserve word wrap without C++ signed overflow or implementation-defined casts.
int32_t signedWord(uint32_t word) {
    return word<=0x7fffffff ? int32_t(word) : int32_t(int64_t(word)-0x100000000LL);
}
void increment(int32_t& word) { word=signedWord(uint32_t(word)+1); }
}

void BattleTriggerHistory::push(uint32_t triggers) {
    for(size_t i=entries.size()-1;i>0;--i)entries[i]=entries[i-1];
    entries[0]=triggers;
}
// Source LCG modulo2^32: multiplier1103515245, increment12345. Output
// bits16..30 produce0..32767; seed/lifetime is caller-owned, not inferred.
int32_t BattleRandom::next() {
    state=state*1103515245u+12345u;
    return int32_t((state>>16)&32767);
}
int32_t BattleRandom::bounded(int32_t bound) {
    return bound==0 ? 0 : next()%bound;
}
int TimingACommand::successWindow(int difficulty) const {
    if(difficulty<0 || difficulty>6)return -1;
    const Window* entry=windows[difficulty];
    // Flag bit1 chooses count-dependent windows; otherwise first row applies
    // regardless of prior successes. Invalid difficulty is a native boundary.
    if(!(flags&2))return entry->samples;
    if(count>=100000)return 0;
    while(count>entry->threshold)++entry;
    return entry->samples;
}
uint32_t TimingACommand::evaluate(const BattleTriggerHistory& history,
                                const TimingAContext& context,BattleRandom& random) {
    const int window=successWindow(context.difficulty);
    if(window<0)return result; // Native boundary: no mutation or RNG consumption.
    if((context.setupOptions&1) && (context.actorFlags&0x10)) {
        // Original negative-draw success branch is unreachable for this irand.
        result=random.bounded(100)<0 ? 2 : 0;
        if(result&2)increment(count);
        return result;
    }
    if(context.automatic) {
        result=2;increment(count);return result;
    }
    result=0;
    // Source battle bit0x01000000 forces ordinary success. Result bit1=success;
    // classification1=mash/pressed failure,2=no press,3=ordinary success.
    if(context.battleFlags&0x01000000) {
        classification=3;result=2;increment(count);return result;
    }
    // Scan20 general trigger samples for mash rejection before testing the
    // shorter success window. This is separate from private defense history.
    int presses=0;
    for(size_t i=0;i<20;++i)if(history.entries[i]&BattleButtonA)++presses;
    if(presses>=2) { classification=1;return result; }
    for(int i=0;i<window;++i) {
        if(history.entries[size_t(i)]&BattleButtonA) {
            classification=3;result=2;increment(count);return result;
        }
    }
    classification=presses ? 1 : 2;
    return result;
}
// Source main states:0 init,99 idle,1000 start finish,1001 countdown,
// 1002 retire callbacks.60 means callback invocations, not rendered frames.
// Scratch lanes5/8 correspond to original display float-300 and scalar20.
int TimingACommand::tick() {
    switch(state) {
    case 0:
        scratch.fill(0);scratch[5]=0xc3960000; // Original binary32 -300.
        scratch[8]=20;state=99;flags&=~1u;classification=0;
        break;
    case 1000:
        finishCountdown=60;state=1001;
        [[fallthrough]];
    case 1001:
        finishCountdown=signedWord(uint32_t(finishCountdown)-1);
        if(finishCountdown<=0)state=1002;
        break;
    case 1002:
        callbacksLive=false;return 0;
    default:break;
    }
    return 1;
}
}

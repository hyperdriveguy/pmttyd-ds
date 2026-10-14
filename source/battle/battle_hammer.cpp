#include "battle_hammer.hpp"
#include <algorithm>

namespace ttyd {
namespace {
int32_t signedWord(uint32_t value) {
    return value<=0x7fffffff ? int32_t(value) : int32_t(int64_t(value)-0x100000000LL);
}
void decrement(int32_t& value) {value=signedWord(uint32_t(value)-1);}
void increment(int32_t& value) {value=signedWord(uint32_t(value)+1);}
}
// Original weapon address overrides give25/30 invocation periods;40 is
// the source default. These are original tokens, not native object addresses.
int ordinaryHammerPeriod(uint32_t originalWeaponAddress) {
    switch(originalWeaponAddress) {
    case 0x80353de8:return 25;
    case 0x80354da8:case 0x80354ce8:return 30;
    default:return 40;
    }
}
int attackDifficulty(int base,uint8_t simplifiers,uint8_t unsimplifiers) {
    const auto adjusted=signedWord(uint32_t(base)-uint32_t(simplifiers)+uint32_t(unsimplifiers));
    return int(std::clamp<int32_t>(adjusted,0,6));
}
// Source allowance table by difficulty0..6. Compare period-countdown to
// this invocation window only at the final charge stage (N+1).
int HammerCommand::releaseAllowance(int difficulty) {
    constexpr int windows[]={80,70,60,40,20,10,5};
    return difficulty<0 || difficulty>6 ? -1 : windows[difficulty];
}
// Main states:0 init,99 idle,100 start,1000 wait-left,1001 charge,
// 1003 completion bridge,1004 idle,1005 stop request,1006 finish countdown,
// 1007 callback retirement. Result bits0/1=pending/success; classification
// 1 failed release,2 overrun,3 successful release. Numeric order is source ABI.
int HammerCommand::tick(const HammerContext& context,BattleRandom& random,HammerSoundSink sounds) {
    if(chargeSteps<1 || chargeSteps>12 || period<0 || releaseAllowance(context.difficulty)<0)return -1;
    soundRequest=nullptr;
    // Original captures automatic and the suppression latch once, before redispatch.
    const bool automatic=context.automatic;
    suppressionLatch=(state!=0 && suppression!=0) ? 1 : 0;
    if(suppressionLatch)return 1;
    bool repeat;
    do {
        repeat=false;
        if(suppressionLatch)break;
        switch(state) {
        // Scratch11 words are the original44-byte display block: lane5 holds
        // float bits0xc3960000 (-300), lane8 scalar20. Voice handle -1=unallocated.
        case 0:
            result=1;stage=0;scratch.fill(0);scratch[5]=0xc3960000;scratch[8]=20;
            enteredCharge=false;suppression=suppressionLatch=0;finalSound=false;
            classification=0;handles.fill(-1);state=99;
            break;
        //120 callback polls to acquire held-left; fallthrough processes the
        // newly installed countdown on this same invocation.
        case 100:
            countdown=120;state=1000;
            [[fallthrough]];
        case 1000:
            if((context.setupOptions&1) && (context.actorFlags&0x10)) {
                if(context.setupOptions&2) {
                    decrement(specialDelay);
                    if(specialDelay>=0)break;
                }
                result=random.bounded(100)<0 ? 2 : 0;
                if(result&2)increment(count);
                return 0; // Manager clears main; the callback does not clean state.
            }
            if((context.held&BattleHeldLeft) || forcedCounter>0 || automatic) {
                enteredCharge=true;state=1001;countdown=stage=1;stepCount=0;
            } else {
                decrement(countdown);
                if(countdown<=0) {result=0;state=1003;repeat=true;}
            }
            break;
        case 1001: {
            if(forcedCounter>0) {
                --forcedCounter;result=2;state=1003;repeat=true;
                // Deliberately continue through countdown/stage/release processing.
            }
            decrement(countdown);
            if(countdown<0) {
                increment(stage);increment(stepCount);
                if(stage>=chargeSteps+2) {
                    result&=~1u;state=1003;classification=2;
                    if(mode&2)result|=2;
                    repeat=true;
                } else {
                    const int k=stage-2;
                    // Retail stages produce0..N-1; reject invalid native work state
                    // rather than perform the original unchecked out-of-bounds read.
                    if(k<0 || k>=int(handles.size()))return -1;
                    if(handles[k]==-1) {
                        soundRequest=k<chargeSteps-1 ? "SFX_AC_PI1" : "SFX_AC_PONE1";
                        handles[k]=sounds.play ? sounds.play(sounds.user,soundRequest) : -1;
                        if(k>=chargeSteps-1)finalSound=true;
                    }
                    countdown=period;
                }
            } else if(!(context.held&BattleHeldLeft) || automatic) {
                bool success=false;
                if(stage==chargeSteps+1) {
                    const int32_t elapsed=signedWord(uint32_t(period)-uint32_t(countdown));
                    if(elapsed<=releaseAllowance(context.difficulty)) {
                        classification=3;success=true;
                    } else if(!(mode&2))classification=1;
                } else if(!(mode&2))classification=1;
                if(success) {result=(result|2)&~1u;state=1003;repeat=true;}
                else if(!automatic && !(mode&2)) {result&=~1u;state=1003;repeat=true;}
            }
            break;
        }
        case 1003:
            // The original bdnz busy loop has no observable service request/yield.
            state=1004;
            break;
        case 1005:
            finishCountdown=60;state=1006; // No same-call decrement (unlike TimingA).
            break;
        case 1006:
            decrement(finishCountdown);
            if(finishCountdown<=0)state=1007;
            break;
        case 1007:
            mainLive=resultLive=displayLive=deleteLive=false;
            break;
        default:break;
        }
    } while(repeat);
    return 1;
}
}

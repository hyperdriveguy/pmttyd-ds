#include "battle_defense.hpp"
#include <algorithm>

namespace ttyd {
namespace {
uint32_t virtualButtons(const BattlePadSnapshot& pad) {
    uint32_t mask=0;
    if(pad.mainY>=30)mask|=0x10000;
    if(pad.mainY<=-30)mask|=0x20000;
    if(pad.mainX<=-30)mask|=0x40000;
    if(pad.mainX>=30)mask|=0x80000;
    if(pad.subY>=30)mask|=0x100000;
    if(pad.subY<=-30)mask|=0x200000;
    if(pad.subX<=-30)mask|=0x400000;
    if(pad.subX>=30)mask|=0x800000;
    return mask;
}
int signedByte(uint8_t value) {return value<128 ? int(value) : int(value)-256;}
int pressCount(const BattlePadWork& pad) {
    int count=0;
    for(size_t i=0;i<15;++i) {
        if(pad.triggers.entries[i]&BattleButtonA)++count;
        if(pad.triggers.entries[i]&BattleButtonB)++count;
    }
    return count;
}
bool pressedWithin(const BattlePadWork& pad,uint32_t mask,int window) {
    for(int i=0;i<window;++i)if(pad.triggers.entries[size_t(i)]&mask)return true;
    return false;
}
}
void BattlePadWork::init() {
    const auto currentTail=current.retainedB,previousTail=previous.retainedB;
    current={};previous={};triggers={};repeats={};held={};
    // Original WorkInit writes bytes0..a, but does not write byteb.
    current.retainedB=currentTail;previous.retainedB=previousTail;
    repeatPhase=repeatCounter=0;
}
void BattlePadWork::update(const BattlePadSample& input) {
    previous=current;current=input.snapshot;current.analogA=current.analogB=0;
    current.retainedA=previous.retainedA;current.retainedB=previous.retainedB;
    const uint32_t now=current.buttons|virtualButtons(current);
    const uint32_t before=held.entries[0];
    const uint32_t virtualPress=now&~before&0xffff0000u;
    held.push(now);triggers.push(input.pressed|virtualPress);repeats.push(input.repeat);
    released[0]=input.released|(before&~now&0xffff0000u);
    if(now==0) {
        repeats.entries[0]=0;repeatPhase=repeatCounter=0;
    } else if(now!=before) {
        // Literal changed-mask operation: preserve physical repeat bits and only
        // the virtual bits present in the new trigger word.
        repeats.entries[0]=(input.repeat|(now&0xffff0000u))&
            ((triggers.entries[0]&0xffff0000u)|0xffffu);
        repeatPhase=1;repeatCounter=0;
    } else {
        repeatCounter=uint8_t(repeatCounter+1);
        if(signedByte(repeatCounter)>=(signedByte(repeatPhase)==1?16:8)) {
            repeats.entries[0]|=now&0xffff0000u;
            repeatPhase=2;repeatCounter=0;
        }
    }
}
int DefenseCommand::evaluate(BattlePadWork& pad,const DefenseContext& context) {
    result=0;
    const int difficulty=std::clamp(3+int(context.unsimplifiers)-int(context.simplifiers),0,6);
    constexpr int aWindows[]={10,9,8,8,7,6,5},bWindows[]={5,4,3,3,2,1,1};
    if(context.weaponGate!=0 && context.hp>0) {
        if(overrideCounter>0) {--overrideCounter;result=5;}
        else if(context.automatic)result=5;
        else if(pressedWithin(pad,BattleButtonB,bWindows[difficulty]) && pressCount(pad)<2)
            result=5;
    }
    if(result!=5) {
        if(overrideCounter>0) {--overrideCounter;result=4;}
        else {
            const int count=pressCount(pad);
            if(count>=2)result=1;
            else if(pressedWithin(pad,BattleButtonA,aWindows[difficulty]))result=4;
            else result=count>0 ? 1 : 2;
        }
    }
    pad.init(); // Every terminal route clears private input state, retaining release.
    return result;
}
}

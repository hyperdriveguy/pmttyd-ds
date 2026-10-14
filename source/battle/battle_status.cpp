#include "battle_status.hpp"

namespace ttyd {
namespace {
int signedByte(uint8_t value) {return value<128?int(value):int(value)-256;}
int narrowId(int32_t id) {return signedByte(uint8_t(id));}
// Original status IDs0..27 index unit byte offsets (hex), not a sequential
// duration struct. IDs10/11,12/13,14/15 share duration/strength pairs whose
// strength sign selects the active ID;25/26 have no stored status lanes.
// Full ID table: docs/behavior/battle/BATTLE_STATUS_STORAGE.md.
constexpr unsigned durationOffsets[]={
    0x12a,0x118,0x11a,0x11b,0x11c,0x11e,0x11f,0x120,
    0x121,0x122,0x123,0x123,0x125,0x125,0x127,0x127,
    0x129,0x12b,0x12c,0x12f,0x130,0x12d,0x12e,0x131,0x133,0,0,0x135};
unsigned strengthOffset(int id) {
    if(id==4 || id==23 || id==24 || (id>=10 && id<=15))return durationOffsets[id]+1;
    return durationOffsets[id];
}
// Move bytes at original20..24: mode,maximum,current,baseline,counter.
// Source flag0x1000 suppresses reset; compare signed counters after narrowing.
void checkMoveCount(BattleStatusUnit& unit) {
    if(!(unit.flags&0x1000) && signedByte(unit.moveBytes[3])>0 &&
       signedByte(unit.moveBytes[3])==signedByte(unit.moveBytes[2]))resetMoveStatus(unit);
}
int towardZero(int value) {return value>0?value-1:value+1;}
}
// ID narrows to signed8 before range check. Returned bytes stay unsigned;
// callers that decrement or compare strength explicitly sign-extend them.
StatusValue getStatus(const BattleStatusUnit& unit,int32_t rawId) {
    const int id=narrowId(rawId);
    if(id<0 || id>27 || !durationOffsets[id])return {};
    const uint8_t duration=unit.statusByte(durationOffsets[id]);
    const uint8_t strength=unit.statusByte(strengthOffset(id));
    if(id>=10 && id<=15) {
        if((id%2==0 && signedByte(strength)<=0) || (id%2!=0 && signedByte(strength)>=0))return {};
    }
    return {duration,strength};
}
bool checkStatus(const BattleStatusUnit* unit,int32_t id) {
    if(!unit)return false;
    return getStatus(*unit,id).duration!=0 || (narrowId(id)==6 && unit->zapTap!=0);
}
int setStatus(BattleStatusUnit& unit,int32_t rawId,int32_t rawDuration,int32_t rawStrength) {
    const int id=narrowId(rawId);
    const uint8_t duration=uint8_t(rawDuration),strength=uint8_t(rawStrength);
    if(id<0 || id>27 || !durationOffsets[id])return 1;
    auto& storedDuration=unit.statusByte(durationOffsets[id]);
    if(id==0 || id==21) {
        storedDuration=signedByte(duration)>0?duration:0;
        return signedByte(duration)>0?1:0;
    }
    // Sleep1 clears auxiliary byte119 on SET; recovery preserves it.
    // Opposite statuses8/9 cancel rather than overwrite when already active.
    if(id==1) {storedDuration=duration;unit.statusByte(0x119)=0;return 1;}
    if(id==8 || id==9) {
        auto& opposite=unit.statusByte(id==8?0x122:0x121);
        if(opposite!=0) {opposite=0;return 0;}
    }
    if(id>=10 && id<=15) {
        auto& storedStrength=unit.statusByte(strengthOffset(id));
        const int old=signedByte(storedStrength),next=signedByte(strength);
        if(next==0) {
            if((id%2==0 && old>=0) || (id%2!=0 && old<=0))storedDuration=storedStrength=0;
            return 0;
        }
        if(id>=14 && ((next>0 && old<0) || (next<0 && old>0))) {
            storedDuration=storedStrength=0;return 0;
        }
        storedDuration=duration;storedStrength=strength;return 1;
    }
    // Charge16 is additive strength in a single byte, duration ignored.
    // Wrap first, then signed negative clears; positive values upper-cap at99.
    if(id==16) {
        storedDuration=uint8_t(uint32_t(storedDuration)+uint32_t(rawStrength));
        if(signedByte(storedDuration)<0) {storedDuration=0;return 0;}
        if(storedDuration>99)storedDuration=99;
        return 1;
    }
    // Fast19/Slow20 cancel their opposite and may reset move counters.
    // Setter return0 can still mean a state mutation (cancellation).
    if(id==19 || id==20) {
        auto& opposite=unit.statusByte(id==19?0x130:0x12f);
        if(signedByte(opposite)>0 && signedByte(duration)>0) {
            opposite=0;checkMoveCount(unit);return 0;
        }
        storedDuration=duration;checkMoveCount(unit);return 1;
    }
    storedDuration=duration;
    if(id==4 || id==23 || id==24)unit.statusByte(strengthOffset(id))=strength;
    return 1;
}
// Recovery skips dead/HP<=0, Charge16 and status17; signed durations move
// toward zero. Setting a recovered value can invoke cancellation/move side effects.
int checkRecoveryStatus(BattleStatusUnit* unit,int32_t rawId) {
    if(!unit || checkStatus(unit,27) || unit->hp<=0)return 0;
    const StatusValue value=getStatus(*unit,rawId);
    if(value.duration==0)return 0;
    const int id=narrowId(rawId);
    if(id<0 || id>24 || id==16 || id==17)return 0;
    int duration=signedByte(value.duration),strength=signedByte(value.strength);
    if(id==1) {
        duration=duration>1?duration-1:0;
        unit->statusByte(0x118)=uint8_t(duration); // Preserve Sleep's auxiliary119.
    } else {
        duration=towardZero(duration);
        if(id>=10 && id<=13) {
            if(duration==0)strength=0;
            unit->statusByte(durationOffsets[id])=uint8_t(duration);
            unit->statusByte(strengthOffset(id))=uint8_t(strength);
        } else {
            if(id!=4 && id!=14 && id!=15 && id!=23 && id!=24)
                strength+=strength>=0?-1:1;
            if(duration==0)strength=0;
            setStatus(*unit,id,duration,strength); // Ignore setter return, retain side effects.
        }
    }
    return duration==0?1:0;
}
// Fast doubles move count with byte wrap. Slow halves when count>=2;
// otherwise cadence threshold2 (or4 for nonpositive maximum) selects a turn.
// Mode10 marks no move; this is source state, not a simulation tick delay.
void resetMoveStatus(BattleStatusUnit& unit) {
    auto& bytes=unit.moveBytes;
    bytes[0]=0;bytes[2]=bytes[1];
    if(checkStatus(&unit,19))bytes[2]=uint8_t(unsigned(bytes[2])*2);
    if(checkStatus(&unit,20)) {
        const int count=signedByte(bytes[2]);
        if(count>=2)bytes[2]=uint8_t(count/2);
        else {
            const int threshold=signedByte(bytes[1])<1?4:2;
            bytes[2]=signedByte(bytes[4])>=threshold?1:0;
            bytes[0]=bytes[2]?0:10;
        }
    }
    bytes[3]=bytes[2];
    if(signedByte(bytes[3])==0) {bytes[2]=0;bytes[0]=10;}
}
void setMaxMoveCount(BattleStatusUnit& unit,int32_t count) {
    unit.moveBytes[1]=uint8_t(count);
    if(signedByte(unit.moveBytes[2])>signedByte(unit.moveBytes[1]))unit.moveBytes[2]=unit.moveBytes[1];
}
// Action/guard deny Sleep1, Stop2, Freeze9, down27 or statusFlags bit0.
// Null unit follows the original permissive status checks; AC additionally
// requires belong0 and excludes kind223 (the source special actor kind).
bool canActStatus(const BattleStatusUnit* unit) {
    for(int id:{1,2,9,27})if(checkStatus(unit,id))return false;
    return !unit || !(unit->statusFlags&1);
}
bool canGuardStatus(const BattleStatusUnit* unit) {return canActStatus(unit);}
bool actionCommandPossible(const BattleStatusUnit& unit) {
    if(unit.belong!=0 || unit.kind==223)return false;
    return canActStatus(&unit);
}
}

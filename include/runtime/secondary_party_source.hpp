#pragma once
namespace ttyd {
// partyWalkMain's bank1 source decision. Distance is in original engine units;
// retain the ordered >= branch, including its unordered floating-point result.
inline bool secondaryPartyUsesOther(bool available,int kind,int mode,int status,float distance) {
    if(!available || (kind==3 && mode==3) || status==3)return false;
    return !(distance>=150.f);
}
}

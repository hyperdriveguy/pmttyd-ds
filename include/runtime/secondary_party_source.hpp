#pragma once
namespace ttyd {
// partyWalkMain's bank1 source decision. Distance is in original engine units;
// retain the ordered >= branch, including its unordered floating-point result.
// Source exclusions are kind3+mode3 or status3; leave these numeric IDs
// as source tokens. Below150 ENGINE units uses the other party; unordered
// distance also takes that branch because the comparison is negated.
inline bool secondaryPartyUsesOther(bool available,int kind,int mode,int status,float distance) {
    if(!available || (kind==3 && mode==3) || status==3)return false;
    return !(distance>=150.f);
}
}

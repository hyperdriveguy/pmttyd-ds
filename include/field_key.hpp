#pragma once
#include <cstdint>
#include <functional>
namespace ttyd {
struct FieldKeyPartner {uint32_t flags=0;int8_t motion=0;};
// Explicit service inputs; native defaults are adapters, not source init claims.
struct FieldKeyServices {
    std::function<int()> partyId;
    std::function<const FieldKeyPartner*(int)> party;
    std::function<int()> sequence,inputByte,shipChanging,rollEnabled,slitEnabled;
    std::function<int()> christineStatus,bomheiKeyCheck,yoshiStatus,holdItem;
};
inline bool fieldKeyAdmitted(const FieldKeyServices& s) {
    const auto* partner=s.party(s.partyId());
    if(s.sequence()!=2 || s.inputByte()!=0)return false;
    if(s.shipChanging()==1 || s.shipChanging()==2)return false;
    if(s.rollEnabled()==0 || s.slitEnabled()==0)return false;
    if(!partner)return true;
    if(s.christineStatus()==1 || s.bomheiKeyCheck()!=0 || s.yoshiStatus()!=0)return false;
    if((partner->flags&0x100) && partner->motion>=5 && partner->motion<=7)return false;
    return s.holdItem()==0;
}
}

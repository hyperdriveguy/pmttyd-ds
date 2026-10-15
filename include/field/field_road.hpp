#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
// Ordinary ROAD records only. Door/train and original scene execution are
// separate owners; unresolved source state must never become a generic route.
enum class RoadCallback {None,HarborOut,PartnerArrival,CoinArrival,DebugArrival};
enum class RoadPolicy {Generic,MissingStory,Custom};
// Source story triggers5 harbor exit, 6 partner arrival, 7 coin arrival need
// their custom owner. Unknown story stays MissingStory, not fabricated stage0.
inline RoadPolicy roadCallbackPolicy(RoadCallback callback,std::optional<int32_t> story) {
    if(callback==RoadCallback::None || callback==RoadCallback::DebugArrival)return RoadPolicy::Generic;
    if(!story)return RoadPolicy::MissingStory;
    const int32_t trigger=callback==RoadCallback::HarborOut?5:callback==RoadCallback::PartnerArrival?6:7;
    return *story==trigger?RoadPolicy::Custom:RoadPolicy::Generic;
}
struct RoadCaseAdmission {
    int32_t systemLevel=0;
    bool window=false,globalBlocked=false,preventMarioEvent=false,caseValid=true;
    bool permits() const{return systemLevel==0 && !window && !globalBlocked && !preventMarioEvent && caseValid;}
};
inline bool roadContactMatches(uint32_t contact,uint32_t registeredNode,bool enabled,const RoadCaseAdmission& gate) {
    return gate.permits() && enabled && contact!=0 && contact==registeredNode;
}
// AUTO prefix order n_,ne_,e_,se_,s_,sw_,w_,nw_. Host cardinal vectors avoid
// introducing source SDK trig into room data; native units are engine*.01.
// AUTO cardinal codes0 north, 2 east, 4 south, 6 west. Diagonal1/3/5/7
// remain outside this finite adapter rather than becoming a generic direction.
inline bool roadDirection(uint32_t direction,float& x,float& z) {
    switch(direction){case 0:x=0;z=-1;return true;case 2:x=1;z=0;return true;
        case 4:x=0;z=1;return true;case 6:x=-1;z=0;return true;default:return false;}
}
}

#include "native_display_state.hpp"
namespace ttyd {
bool NativeDisplayState::request(const DisplayStateRequest& r,const NativeDisplaySubmit& s){
    if(!state_ || !s.submit)return false;auto next=*state_;
    auto bit=[&](unsigned i){return r.values[i]==0 || r.values[i]==1;};
    switch(r.kind){
    case DisplayGXState::ColorUpdate:if(!bit(0))return false;next.color=r.values[0];break;
    case DisplayGXState::AlphaUpdate:if(!bit(0))return false;next.alpha=r.values[0];break;
    case DisplayGXState::ZScaleOffset:
        // Source identity leaves the admitted viewport depth range intact.
        if(r.floats[0]!=1 || r.floats[1]!=0)return false;break;
    case DisplayGXState::BlendMode:
        if(r.values[0]==0){next.sourceBlend=1;next.destinationBlend=0;next.logicOr=false;}
        else if(r.values[0]==1 && ((r.values[1]==4 && r.values[2]==5) || (r.values[1]==1 && r.values[2]==1) || (r.values[1]==6 && r.values[2]==7))){next.sourceBlend=r.values[1];next.destinationBlend=r.values[2];next.logicOr=false;}
        else if(r.values==std::array<int32_t,5>{2,1,1,7,0})next.logicOr=true;
        else return false;break;
    case DisplayGXState::ZCompareLocation:if(!bit(0))return false;next.beforeAlpha=r.values[0];break;
    case DisplayGXState::AlphaCompare:
        if(r.values==std::array<int32_t,5>{7,0,0,7,0})next.alphaReject=false;
        else if(r.values==std::array<int32_t,5>{6,128,1,0,0})next.alphaReject=true;
        else return false;break;
    case DisplayGXState::ZMode:
        if(!bit(0) || !bit(2) || (r.values[1]!=3 && r.values[1]!=7))return false;
        if(!r.values[0] && r.values[2])return false;
        next.depth=r.values[0];next.compare=r.values[1];next.depthWrite=r.values[2];break;
    default:return false;
    }
    if(!s.submit(s.user,next))return false;state_=next;return true;
}
bool NativeDisplayState::restore(const NativeDisplaySubmit& s) const{
    return !state_ || (s.submit && s.submit(s.user,*state_));
}
}

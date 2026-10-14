#pragma once
#include "display_queue.hpp"
namespace ttyd {
// Known state established by the real source dummy; masks are independent.
struct NativeDisplayPipeline {
    bool color=false,alpha=false,depth=false,depthWrite=false,beforeAlpha=false,alphaReject=true;
    int32_t compare=7,sourceBlend=0,destinationBlend=1;
    bool rasterAdmitted() const{return !(beforeAlpha && alphaReject && depth && depthWrite);}
};
struct NativeDisplaySubmit {void* user=nullptr;bool (*submit)(void*,const NativeDisplayPipeline&)=nullptr;};
class NativeDisplayState {
public:
    void afterDummy(){state_=NativeDisplayPipeline{};}
    bool request(const DisplayStateRequest&,const NativeDisplaySubmit&);
    bool restore(const NativeDisplaySubmit&) const;
    const NativeDisplayPipeline* state() const{return state_?&*state_:nullptr;}
private:
    std::optional<NativeDisplayPipeline> state_;
};
}

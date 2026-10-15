#pragma once
#include "display_queue.hpp"
namespace ttyd {
// Known state established by the real source dummy; masks are independent.
struct NativeDisplayPipeline {
    bool color=false,alpha=false,depth=false,depthWrite=false,beforeAlpha=false,alphaReject=true;
    // Dummy defaults: compare7 ALWAYS, blend ZERO/ONE(0/1), all write masks
    // disabled. The real source dummy, not C2D defaults, seeds this owner.
    int32_t compare=7,sourceBlend=0,destinationBlend=1;
    bool logicOr=false;
    // PICA late-depth adapter cannot acknowledge simultaneous before-alpha
    // testing, alpha rejection, depth test AND depth write. Keep that wait.
    bool rasterAdmitted() const{return !(beforeAlpha && alphaReject && depth && depthWrite);}
};
struct NativeDisplaySubmit {void* user=nullptr;bool (*submit)(void*,const NativeDisplayPipeline&)=nullptr;};
// Logical GX owner. restore is idempotent physical replay; afterDummy is a
// source side effect and must not be substituted for restoration.
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

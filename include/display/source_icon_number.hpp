#pragma once
#include "source_icon_matrix.hpp"
namespace ttyd {
struct SourceIconNumberServices {SourceIconMatrixServices icons;};
class SourceIconNumberCursor {
public:
 SourceIconNumberCursor();~SourceIconNumberCursor();
 SourceIconNumberCursor(const SourceIconNumberCursor&)=delete;
 SourceIconNumberCursor& operator=(const SourceIconNumberCursor&)=delete;
 bool active() const;void reset();
private:
 struct State;State* state_=nullptr;
 friend bool resumeSourceIconNumber(const CameraViewMatrix&,int32_t,uint32_t,
     const uint32_t&,const SourceIconNumberServices&,SourceIconNumberCursor&,int32_t&);
};
// Whole iconNumberDispGx80019120/284. Base/color objects are borrowed at stable
// addresses through completion: base is captured anew at each source Concat,
// color after that Concat. Mode0 selects table80415084; ALL other modes select
// table80415098. Signed quotient/remainder indices and max5 low digits are the
// source behavior, including negative indices: no minus/abs/clamp formatting.
// Delegates complete iconDispGxCol to retained SourceIconMatrix/GX. Provider
// owners/tables must remain stable; linked readers provide actual source data.
// Do not recursively resume/reset this same cursor from a provider callback.
// Returns source accumulated integer width only on whole completion. Native
// finite/representable fctiwz domain is admitted; other conversion domains hold.
bool resumeSourceIconNumber(const CameraViewMatrix& base,int32_t number,uint32_t mode,
    const uint32_t& rgba,const SourceIconNumberServices&,SourceIconNumberCursor&,int32_t& width);
}

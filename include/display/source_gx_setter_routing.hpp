#pragma once
#include "source_gx_inherited_state.hpp"
#include <functional>
namespace ttyd {
struct DisplayStateRequest;struct BattleTexturePlaneRequest;struct SourceFontStateCall;
struct SourceFontDrawStartFog;struct SourceIconGXCall;struct WindowPrimitiveRequest;
// Metadata result only. NONE of these results acknowledges an original child.
// Call accepted ONLY AFTER its real owner acknowledged the actual setter; a
// held or rejected source owner must never enter this observer.
enum class SourceGxObserved {Ignored,Recorded,Invalidated,Unavailable};
class SourceGxSetterRouting {
public:
 SourceGxSetterRouting(std::shared_ptr<SourceGxInheritedState>,std::shared_ptr<const void>actualScope,std::function<bool()>sourceCurrent);
 SourceGxObserved accepted(const SourceGxInheritedCall&);
 SourceGxObserved accepted(const DisplayStateRequest&);
 SourceGxObserved accepted(const BattleTexturePlaneRequest&);
 SourceGxObserved accepted(const SourceFontStateCall&);
 SourceGxObserved accepted(const SourceFontDrawStartFog&);
 SourceGxObserved accepted(const SourceIconGXCall&);
 SourceGxObserved accepted(const WindowPrimitiveRequest&);
 // Short actual-source admission. Long packet proof is the returned immutable
 // snapshot.current(), independently of this source scope/camera epoch.
 std::shared_ptr<const SourceGxInheritedSnapshot>capture(unsigned stages=1)const;
 bool compatibleNow(unsigned stages=1)const;
 void retire();
private:
 std::shared_ptr<SourceGxInheritedState>history_;std::shared_ptr<const void>scope_;
 std::function<bool()>sourceCurrent_;bool alive_=true;mutable bool invoking_=false;
 bool current()const;SourceGxObserved record(const SourceGxInheritedCall&);
};
}

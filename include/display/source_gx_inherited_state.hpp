#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
// Native semantic source-GX register history. A call is an actual reached source
// setter effect, not evidence that an atlas/native C3D state is ready.
struct SourceGxInheritedCall {
 uint32_t function=0,callsite=0,count=0;
 std::array<uint32_t,8> integers{};
 std::array<uint32_t,4> floatBits{};
 bool fog=false;
};
struct SourceGxInheritedBinding {
 std::shared_ptr<const void> process;
 void*user=nullptr;bool(*current)(void*)=nullptr;
 // Actual native startup admission, separate from long process currentness.
 bool(*coldCurrent)(void*)=nullptr;
};
class SourceGxInheritedState;
class SourceGxInheritedSnapshot {
public:
 bool current()const;
 unsigned stages()const{return stages_;}
 const std::array<SourceGxInheritedCall,96>& evidence()const{return evidence_;}
 unsigned size()const{return size_;}
private:
 struct Lifetime;
 std::shared_ptr<Lifetime> lifetime_;
 std::array<SourceGxInheritedCall,96> evidence_{};
 unsigned size_=0,stages_=0;
 friend class SourceGxInheritedState;
};
// Owns logical fields used by the finite UI raster paths. No GPU draw/FIFO,
// guest GXData adoption, constructor defaults, global ready bit, or whole SDK
// function acknowledgement is implied. Native packets still own local TEV,
// matrix, textures, blend/depth, source geometry and real residency separately.
class SourceGxInheritedState {
public:
 SourceGxInheritedState();~SourceGxInheritedState();
 SourceGxInheritedState(const SourceGxInheritedState&)=delete;
 SourceGxInheritedState&operator=(const SourceGxInheritedState&)=delete;
 bool bind(SourceGxInheritedBinding);
 bool current()const;
 // Exact bounded native cold contribution from pinned __GXInitGX source:
 // original private fog-color capture and all operations3088..3220. Executes
 // once per actual native cold process; NEVER proves reached/completed GXInit,
 // DEMOInit, Logo, or the omitted earlier/later operations of __GXInitGX.
 bool initializeCold();
 bool coldComplete()const;
 // Record only actual reached/owned setter effects. Callers route EVERY source
 // write of tracked fields; unknown effects invalidate future capture. This is
 // an atomic logical effect; it is not an acknowledgement of any external GPU.
 bool apply(const SourceGxInheritedCall&);
 static bool tracks(uint32_t function);
 // Unknown/unowned source GX work invalidates future known-field evidence,
 // while already copied packets remain valid under their long process proof.
 bool invalidate();
 std::shared_ptr<const SourceGxInheritedSnapshot> capture(unsigned stages=1)const;
 void retire();
private:
 struct State;std::unique_ptr<State>state_;
 mutable bool invoking_=false;
};
}

#pragma once
#include "event_native_runtime.hpp"
#include "native_facade_registration.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
constexpr uint32_t NativeFieldMarioGetPositionFunction=0x800e71f8;
struct NativeFieldMarioGetPositionBinding {
 std::shared_ptr<const void> lifetime;
 NativeFacadeRegistration::Proof host;
 void* user=nullptr;
 // Called only after intrinsic actual VM and host liveness proof. Must prove
 // the actual native field/Encounter admission before borrowing Game storage.
 bool(*current)(void*,const EventNativeRuntime&)=nullptr;
 // Actual engine-unit binary32 XYZ, same finite-view convention as Prepare8.
 std::optional<uint32_t>(*position)(void*,unsigned axis)=nullptr;
};
// Native preadmission for the actually reached (0,LW0,LW1,LW2) domain. It
// executes the existing whole original getter; no complete Mario RAM is made.
class NativeFieldMarioGetPosition {
public:
 static std::unique_ptr<NativeFieldMarioGetPosition> create(EventNativeRuntime&,NativeFieldMarioGetPositionBinding);
 std::optional<int32_t> invoke(uint32_t function,EventEntry&,bool firstCall);
 bool current();
private:
 NativeFieldMarioGetPosition(EventNativeRuntime&,NativeFieldMarioGetPositionBinding);
 EventNativeRuntime*vm_;NativeFacadeRegistration::Proof vmProof_;
 NativeFieldMarioGetPositionBinding binding_;
 std::array<uint32_t,3> sample_{};
 bool invoking_=false,checking_=false,stale_=false;
};
}

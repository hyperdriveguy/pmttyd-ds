#pragma once
#include "event_native_runtime.hpp"
#include "native_facade_registration.hpp"
#include "field_mario_preparation.hpp"
#include <memory>
#include <optional>
namespace ttyd {
constexpr uint32_t NativeFieldMarioKeyFunction=0x800e7474;
struct NativeFieldMarioKeyBinding {
 std::shared_ptr<const void>lifetime;
 NativeFacadeRegistration::Proof host;
 void*user=nullptr;
 bool(*current)(void*,const EventNativeRuntime&)=nullptr;
 // Actual typed native owners/callbacks, copied once. Callback userdata must
 // remain stationary and alive under the independent lifetime/current proof.
 FieldMarioPreparationWorld world;
};
// Finite native admission for original raw immediate key-on/off0/1. The whole
// existing source helper owns flags/key-depth order and source wrapping.
class NativeFieldMarioKey {
public:
 static std::unique_ptr<NativeFieldMarioKey>create(EventNativeRuntime&,NativeFieldMarioKeyBinding);
 std::optional<int32_t>invoke(uint32_t function,EventEntry&,bool firstCall);
 bool current();
private:
 NativeFieldMarioKey(EventNativeRuntime&,NativeFieldMarioKeyBinding);
 EventNativeRuntime*vm_;NativeFacadeRegistration::Proof vmProof_;
 NativeFieldMarioKeyBinding binding_;
 bool invoking_=false,checking_=false,stale_=false;
};
}

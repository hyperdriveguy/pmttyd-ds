#pragma once
#include "event_native_runtime.hpp"
#include "native_facade_registration.hpp"
#include <map>
#include <memory>
#include <optional>
namespace ttyd {
struct NativeEnvironmentSoundState;
class NativeEnvironmentSound;
class NativeEnvironmentColdProof {
public:NativeEnvironmentColdProof()=default;
private:
 std::weak_ptr<NativeEnvironmentSoundState>state_;
 explicit NativeEnvironmentColdProof(const std::shared_ptr<NativeEnvironmentSoundState>&s):state_(s){}
 friend class NativeEnvironmentSound;
};
struct NativeEnvironmentSoundBinding {
 std::shared_ptr<const void>lifetime;
 NativeFacadeRegistration::Proof host;
 void*user=nullptr;
 bool(*current)(void*)=nullptr;
 // Actual source-owned SDA bytes, not a guessed table constant. Read may hold.
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
};
class NativeEnvironmentOffCursor {
public:
 bool active()const{return phase_!=0&&phase_!=4;}
 uint32_t pending()const{return pending_;}
private:
 std::weak_ptr<NativeEnvironmentSoundState>owner_;
 uint32_t control_=0,table_=0,pending_=0;uint16_t handle_=0;uint8_t phase_=0;
 friend class NativeEnvironmentSound;
};
// Unknown until actual native Audio startup invokes the explicit cold
// constructor contribution. This is NOT a whole SoundInit/psndInit receipt.
class NativeEnvironmentSound {
public:
 static std::shared_ptr<NativeEnvironmentSound>prepare(NativeEnvironmentSoundBinding);
 std::optional<NativeEnvironmentColdProof>initializeNativeCold();
 bool current(const NativeEnvironmentColdProof&);
 std::optional<int32_t>off(const NativeEnvironmentColdProof&,uint32_t control,NativeEnvironmentOffCursor&);
 // An accepted unowned environment mutation invalidates empty-state evidence.
 // It never clears source/guest/native voices or permits a second cold init.
 void invalidate();
private:
 std::shared_ptr<NativeEnvironmentSoundState>state_;
 explicit NativeEnvironmentSound(std::shared_ptr<NativeEnvironmentSoundState>);
 friend struct NativeEnvironmentSoundFixture;
};
constexpr uint32_t NativeEnvironmentOffEventFunction=0x80151e0c;
// Whole original raw-argument wrapper, tied to the actual caller VM. The
// native source child above retains its own handle/table read across holds.
class NativeFieldEnvironmentOff {
public:
 static std::unique_ptr<NativeFieldEnvironmentOff>create(EventNativeRuntime&,std::shared_ptr<NativeEnvironmentSound>,NativeEnvironmentColdProof);
 std::optional<int32_t>invoke(uint32_t function,EventEntry&,bool firstCall);
 void retire(const EventEntry&);
 uint32_t pending()const{return pending_;}
private:
 struct Call{EventEntry*entry=nullptr;uint32_t address=0,id=0,command=0,args=0,raw=0;uint8_t phase=0;NativeEnvironmentOffCursor child;};
 EventNativeRuntime*vm_;NativeFacadeRegistration::Proof vmProof_;
 std::shared_ptr<NativeEnvironmentSound>environment_;NativeEnvironmentColdProof cold_;
 std::map<uint32_t,Call>calls_;bool invoking_=false,stale_=false;uint32_t pending_=0;
 NativeFieldEnvironmentOff(EventNativeRuntime&,std::shared_ptr<NativeEnvironmentSound>,NativeEnvironmentColdProof);
};
}

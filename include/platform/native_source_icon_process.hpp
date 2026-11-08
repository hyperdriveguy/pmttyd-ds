#pragma once
#include <memory>
#include <functional>
#include "native_source_icon_memory.hpp"
#include "native_field_animation.hpp"
namespace ttyd {
enum class NativeSourceIconBoundary {HeapTransfer,Delivery,Callback};
struct NativeSourceIconAdmission {
 std::shared_ptr<const void>lifetime;
 std::function<bool(NativeSourceIconBoundary,uint32_t,uint32_t)>admit;
};
// Supported native cold contribution: whole original IconInit and callbacks,
// actual existing process heap, finite DVD entries/deliveries. Does not complete
// whole Logo/GX/SDK/DVDMgr parents or provide GPU residency/readiness.
class NativeSourceIconProcess {
public:
 static std::shared_ptr<NativeSourceIconProcess>create(std::shared_ptr<NativeFieldAnimation>,const char*rawDirectory,const char*startupPack,NativeSourceIconAdmission={});
 ~NativeSourceIconProcess();
 NativeSourceIconProcess(const NativeSourceIconProcess&)=delete;
 NativeSourceIconProcess&operator=(const NativeSourceIconProcess&)=delete;
 bool begin(); // Whole source IconInit; admits actual two jobs, not callbacks.
 bool pump(); // Actual native delivery then retained original callbacks.
 bool ready()const;bool current()const;uint32_t pending()const;
 bool owns(uint32_t,uint32_t size=1)const;
 std::optional<uint8_t>byte(uint32_t)const;
 std::optional<uint16_t>half(uint32_t)const;
 std::optional<uint32_t>word(uint32_t)const;
 std::shared_ptr<NativeSourceIconMemory>memory()const;
 std::shared_ptr<const NativeSourceIconAssets>assets()const;
 struct Counts {uint32_t allocations=0,transfers=0,opens=0,jobs=0,reads=0,deliveries=0,callbacks=0,closes=0,clears=0;};
 Counts counts()const;
 // Native metadata only. Retire mappings while actual process VM is alive.
 void retire();
private:
 NativeSourceIconProcess();struct State;std::shared_ptr<State>state_;mutable bool invoking_=false;
};
}

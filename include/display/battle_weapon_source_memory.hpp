#pragma once
#include "battle_actor.hpp"
#include "event_native_runtime.hpp"
#include "event_gp_language_alias.hpp"
#include "native_ui_startup.hpp"
#include <functional>
namespace ttyd {
// Source-only read/store composition. It installs no VM aliases and supplies
// neither a VM execution receipt nor source UI readiness. The actual VM and
// address-space mappings must survive this owner unchanged through source use.
class BattleWeaponSourceMemory {
public:
 // One-shot per actual destination VM: installs the supplied genuine source
 // GP-language alias only after immutable/font/allocation preflight. The
 // runtime retains this owner; later renderer allocations reuse it. Retiring
 // a source process cannot authorize rebinding an extant destination GP.
 static std::shared_ptr<BattleWeaponSourceMemory>create(EventNativeRuntime&,
   std::shared_ptr<NativeUiStartup>,std::shared_ptr<EventGpLanguageAlias>,
   std::shared_ptr<const void> actualVmLifetime,std::function<bool()> actualVmCurrent,
   const char*actualFontStartupImage);
 BattleWeaponSourceMemory(const BattleWeaponSourceMemory&)=delete;
 BattleWeaponSourceMemory&operator=(const BattleWeaponSourceMemory&)=delete;
 bool current()const;
 bool owns(uint32_t address,uint32_t size=1)const;
 std::optional<uint8_t>byte(uint32_t)const;
 std::optional<uint16_t>half(uint32_t)const;
 std::optional<uint32_t>word(uint32_t)const;
 bool store(uint32_t address,uint32_t size,uint32_t value);
 BattleActorServices actors();
 std::shared_ptr<NativeUiStartup>startup()const{return startup_;}
private:
 BattleWeaponSourceMemory()=default;
 EventNativeRuntime*vm_=nullptr;uint32_t gp_=0;
 std::shared_ptr<NativeUiStartup>startup_;
 std::shared_ptr<EventGpLanguageAlias>language_;
 std::shared_ptr<NativeSourceFontMemory>font_;
 std::shared_ptr<const void>lifetime_;
 std::function<bool()>current_;
 EventScriptAssets constants_;
 mutable bool invoking_=false,invalid_=false;
 bool valid()const;
 bool constant(uint32_t,uint32_t)const;
 std::optional<uint32_t>read(uint32_t,uint32_t)const;
};
}

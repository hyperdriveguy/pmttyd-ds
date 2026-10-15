#pragma once
#include "field_npc_setup.hpp"
#include "field_npc_actor_event.hpp"
#include "field_npc_registry_owner.hpp"
namespace ttyd {
class EventNativeRuntime;
class EventScriptAssets;
class CameraManager;
// Validates the original constructor scanner's entire immutable input span;
// world primitives reached later remain separately admitted at execution.
bool admitOriginalNpcSetupScript(const EventScriptAssets&,EventAddressSpace&,uint32_t);
struct FieldNpcSetupNativeServices {
 void* user=nullptr;
 BattlePoseClock (*clock)(void*)=nullptr;
 bool (*admitBattle)(void*,EventAddressSpace&,uint32_t actor,int32_t index)=nullptr;
 int32_t (*random)(void*,int32_t bound)=nullptr;
 int32_t (*savedFlag)(void*,int32_t index)=nullptr;
 const CameraManager* cameras=nullptr; // Retained camera4 owner; unpublished yaw holds.
};
// Stable real actor/descriptor/pose bridge for evt_npc_setup. Bindings, facade,
// and synchronous providers outlive all referencing event cursors.
class FieldNpcSetupBinding {
public:
 static std::unique_ptr<FieldNpcSetupBinding> bind(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcRegistryBinding&,EventNativeRuntime&,std::shared_ptr<const EventScriptAssets>,const FieldNpcSetupNativeServices&);
 int32_t invokeActorEvent(uint32_t function,EventEntry&,bool first,FieldNpcActorEventCursor&);
 int32_t invoke(EventEntry&,bool first,FieldNpcSetupCursor&,const FieldNpcSetupOperands&,const FieldNpcSetupEvents&);
private:
 FieldNpcSetupBinding(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcRegistryBinding&,EventNativeRuntime&,std::shared_ptr<const EventScriptAssets>,const FieldNpcSetupNativeServices&,uint32_t mode);
 bool admit(const EventEntry&,uint32_t table,bool first);
 bool safeDelete(uint32_t,const EventEntry&) const;
 bool readable(uint32_t,unsigned) const;
 bool text(uint32_t,std::string&,unsigned) const;
 uint32_t word(uint32_t) const;
 FieldNpcSetupWorld world();
 std::shared_ptr<FieldNpcRegistryOwner> owner_;FieldNpcRegistryBinding& binding_;EventNativeRuntime& vm_;
 std::shared_ptr<const EventScriptAssets> source_;FieldNpcSetupNativeServices services_;uint32_t mode_;
};
}

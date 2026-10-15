#pragma once
#include "field_npc_setup.hpp"
#include "field_npc_actor_event.hpp"
#include "field_npc_wander_event.hpp"
#include "field_npc_registry_owner.hpp"
namespace ttyd {
class EventNativeRuntime;
class EventScriptAssets;
class CameraManager;
class BattleSequenceBinding;
class SourceSoundFlags;
// Validates the original constructor scanner's entire immutable input span;
// world primitives reached later remain separately admitted at execution.
// Native scanner bound65536 words/16 labels, source opcode1 END/3 LABEL.
// It admits immutable constructor input, not execution readiness of every callback.
bool admitOriginalNpcSetupScript(const EventScriptAssets&,EventAddressSpace&,uint32_t);
// Clock and session RNG are real owners. admitBattle must cover the exact
// requested configuration, including any savedFlag/RNG domain, without effects.
struct FieldNpcSetupNativeServices {
 void* user=nullptr;
 BattlePoseClock (*clock)(void*)=nullptr;
 bool (*admitBattle)(void*,EventAddressSpace&,uint32_t actor,int32_t index)=nullptr;
 int32_t (*random)(void*,int32_t bound)=nullptr;
 int32_t (*savedFlag)(void*,int32_t index)=nullptr;
 BattleSequenceBinding* mapHeap=nullptr; // Same-VM retained primary arena binding.
 const SourceSoundFlags* soundFlags=nullptr;
 bool (*admitMario)(void*,EventAddressSpace&,uint32_t)=nullptr;
 bool (*wanderRequest)(void*,EventAddressSpace&,const FieldNpcWanderRequest&,FieldNpcWanderResponse&)=nullptr;
 const CameraManager* cameras=nullptr; // Retained camera4 owner; unpublished yaw holds.
};
// Stable real actor/descriptor/pose bridge for evt_npc_setup. Bindings, facade,
// and synchronous providers outlive all referencing event cursors.
// Binding captures raw GP context word and retained F identity. Every invoke
// rechecks those exact owners; it must not silently normalize/rebind a context.
class FieldNpcSetupBinding {
public:
 static std::unique_ptr<FieldNpcSetupBinding> bind(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcRegistryBinding&,EventNativeRuntime&,std::shared_ptr<const EventScriptAssets>,const FieldNpcSetupNativeServices&);
 int32_t invokeWanderEvent(uint32_t,EventEntry&,bool first,FieldNpcWanderEventCursor&);
 // No-effects owner/caller admission and the shared original name resolver.
 bool bindsFieldOwner(const EventNativeRuntime&,const std::shared_ptr<FieldNpcRegistryOwner>&) const;
 bool ownsCaller(const EventEntry&) const;
 std::optional<uint32_t> resolveEventActor(EventEntry&,uint32_t name,bool dereference);
 // Checked actor operands defer at most one setter word, then publish only on
 // full per-output admission. Actor-name sentinels require separate handling.
 int32_t invokeActorEvent(uint32_t function,EventEntry&,bool first,FieldNpcActorEventCursor&);
 int32_t invoke(EventEntry&,bool first,FieldNpcSetupCursor&,const FieldNpcSetupOperands&,const FieldNpcSetupEvents&);
private:
 friend class FieldNpcCreationBinding;
 FieldNpcSetupBinding(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcRegistryBinding&,EventNativeRuntime&,std::shared_ptr<const EventScriptAssets>,const FieldNpcSetupNativeServices&,uint32_t mode);
 FieldNpcActorEventServices actorServices();
 bool caller(const EventEntry&) const;
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

#pragma once
#include "native_harbor_field_sequence.hpp"
#include "field_npc_setup_binding.hpp"
#include "field_npc_creation_binding.hpp"
#include "native_harbor_sailors.hpp"
namespace ttyd {
class HarborEncounter;
// Immutable ORIGINAL source snapshot. Loading does not create NPCs, events or
// GPU readiness; the actual native process owns the MODEL/TPL file providers.
struct NativeHarborEncounterFieldAssets {
 std::shared_ptr<const EventScriptAssets> source;
 FieldMapNpcAssets tags;
 std::optional<std::array<uint8_t,8>> originalHeldItemTable;
 std::vector<std::shared_ptr<const BattlePoseResource>> poses;
 static std::shared_ptr<const NativeHarborEncounterFieldAssets> load(const char* actualAssetRoot);
};
struct NativeHarborEncounterFieldConfig {
 std::shared_ptr<NativeFieldAnimation> animation;
 std::shared_ptr<FieldNpcRegistryOwner> registry;
 FieldNpcRegistryBinding* registryBinding=nullptr; // Optional actual same-VM binding, borrowed through teardown.
 std::shared_ptr<const NativeHarborEncounterFieldAssets> assets;
 // Real native host lifetime, separate from genuine original Sequence owner.
 NativeFacadeRegistration::Proof host;
 std::shared_ptr<const void> serviceLifetime;void* user=nullptr;
 bool(*current)(void*,const EventNativeRuntime&,const HarborEncounter*)=nullptr;
 // Actual clock is supplied by animation. Optional reached battle admission,
 // RNG and saved flags retain honest unavailable domains, never guessed data.
 // Exact unowned original callback. Null/empty holds; child owns retained work.
 std::optional<int32_t>(*call)(void*,EventNativeRuntime&,EventEntry&,uint32_t,bool)=nullptr;
 void(*retiring)(void*,EventNativeRuntime&,const EventEntry&)=nullptr;
 FieldNpcSetupNativeServices setup;
 BattleSequenceEntryServices battle;
 // Native allocation policy; occupied/semantic/unknown overlaps reject.
 uint32_t newRegistryBase=0x85000000,privateTagBase=0x87000000;
};
// One actual Encounter VM context. Own uniquely with the Runtime AFTER its VM
// member so bindings are destroyed while VM memory still exists. Tickets are
// safe to copy out; this borrowed-VM constructor itself must not outlive VM.
class NativeHarborEncounterField {
public:
 static std::unique_ptr<NativeHarborEncounterField> prepare(EventNativeRuntime&,
   const HarborEncounter* actualFacade,NativeFacadeRegistration::Proof actualFacadeProof,
   NativeHarborEncounterFieldConfig);
 ~NativeHarborEncounterField();
 bool current(const HarborEncounter*);
 // File/retained child work only. True admits the next actual VM manager
 // visit; initialized() separately means both original constructor roots ended.
 bool advance(const HarborEncounter*);
 bool initialized()const;
 bool handles(uint32_t function)const;
 std::optional<int32_t> invoke(const HarborEncounter*,uint32_t,EventEntry&,bool);
 std::optional<int32_t> external(const HarborEncounter*,uint32_t,EventEntry&,bool);
 void retire(const HarborEncounter*,const EventEntry&);
 uint32_t pending()const;
 std::optional<NativeHarborBattleTicket> request(const HarborEncounter*);
 bool entered(const HarborEncounter*,const NativeHarborBattleTicket&,const NativeHarborBattleIdentity&);
 bool returned(const HarborEncounter*,const NativeHarborBattleTicket&,const NativeHarborCompletionReceipt&);
 std::shared_ptr<FieldNpcRegistryOwner> registry()const;
 std::optional<uint32_t> endingFieldWork(const HarborEncounter*,const NativeHarborBattleTicket&);
 std::optional<uint32_t> endingFieldWord(const HarborEncounter*,const NativeHarborBattleTicket&,uint32_t);
 bool endingFieldStore(const HarborEncounter*,const NativeHarborBattleTicket&,uint32_t,uint32_t);
 std::optional<uint32_t> actor(const HarborEncounter*,uint32_t actualSourceName);
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 explicit NativeHarborEncounterField(std::unique_ptr<State>);
};
}

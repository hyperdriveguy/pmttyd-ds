#pragma once
#include "field_npc_registry.hpp"
#include "field_pose_pool.hpp"
#include "field_map_npc.hpp"
#include <array>
namespace ttyd {
class EventAddressSpace;
class FieldNpcRegistryBinding;
struct FieldNpcRegistryEntryState;
struct FieldNpcEntryCursor {
 std::shared_ptr<FieldNpcRegistryEntryState> state;
 bool pending() const;
 bool complete() const;
};
struct FieldMapNpcBattleAdmission {
 void* user=nullptr;
 // Required for non--1 requests; no effects, complete numeric/RNG admission.
 bool (*admit)(void*,uint32_t actor,int32_t index)=nullptr;
 int32_t (*random)(void*,int32_t)=nullptr;
 int32_t (*savedFlag)(void*,int32_t)=nullptr;
};
// One cold-boot NPC owner. Stable native allocations have explicit guest
// identities, not original __memAlloc addresses. Cold BSS counts are zero;
// npcInit itself does not clear them. Room reset/deletion remain separate.
class FieldNpcRegistryOwner {
public:
 // allocationBase is a native16-byte-aligned guest arena, not source heap
 // identity. Four stable buffers back field actors/release/battle actors/F.
 // Optional pose pool is injected unchanged, sharing Game's original global
 // namespace. Native RAM/BSS allocation remains a separate cold owner.
 static std::shared_ptr<FieldNpcRegistryOwner> cold(uint32_t allocationBase,std::shared_ptr<BattlePosePool> pool={});
 FieldNpcRegistryOwner(const FieldNpcRegistryOwner&)=delete;
 FieldNpcRegistryOwner& operator=(const FieldNpcRegistryOwner&)=delete;
 bool initialized() const{return initialized_;}
 bool entryPending() const{return bool(pendingEntry_);}
 bool ownsFieldActor(uint32_t address) const; // Admission only; active/name gates remain source reads.
 uint32_t fieldWorkAddress() const{return addresses_[3];}
 FieldPosePool& poses(){return poses_;}
 const FieldPosePool& poses() const{return poses_;}
private:
 friend class FieldNpcRegistryBinding;
 friend class FieldNpcSetupBinding;
 friend class FieldNpcMainEntryBinding;
 explicit FieldNpcRegistryOwner(uint32_t,std::shared_ptr<BattlePosePool>);
 uint8_t* span(uint32_t,uint32_t);
 std::array<uint8_t,0x30> registry_{}; // Source startup BSS8041e8ac..e8db.
 std::array<uint32_t,4> addresses_{};
 std::array<std::vector<uint8_t>,4> allocations_;
 FieldPosePool poses_;
 bool initialized_=false;
 // Preserve an unfinished source constructor even if its caller disappears.
 // Cancellation is unbound: never expose its partial actor as a ready record.
 std::shared_ptr<FieldNpcRegistryEntryState> pendingEntry_;
};
// Must retire before its facade. Retains the owner; unregisters only its own
// external spans. Multiple independent facades alias the same actor bytes.
// Binding retains owner and only unregisters its external aliases. Rebinding
// an existing owner must preserve bytes, pose handles and initialized state.
class FieldNpcRegistryBinding {
public:
 static std::unique_ptr<FieldNpcRegistryBinding> bind(std::shared_ptr<FieldNpcRegistryOwner>,EventAddressSpace&);
 ~FieldNpcRegistryBinding();
 FieldNpcRegistryBinding(const FieldNpcRegistryBinding&)=delete;
 FieldNpcRegistryBinding& operator=(const FieldNpcRegistryBinding&)=delete;
 bool initialize(); // Once, actual npcInit. Never a room reset.
 bool reset(int32_t selector); // Actual npcReset only; pose/event teardown stays separate.
 bool canEnter(uint32_t name,uint32_t model) const;
 // Caller supplies an already parsed, history-gate-admitted row. All row
 // services are admitted before Entry; no partial row is repeated on a hold.
 std::optional<uint32_t> registerMapNpc(const PackedMapNpcInput&,BattlePoseClock,const FieldMapNpcBattleAdmission& = {});
 std::optional<int32_t> enter(uint32_t name,uint32_t model,BattlePoseClock);
 std::optional<int32_t> resumeEnter(uint32_t name,uint32_t model,BattlePoseClock,FieldNpcEntryCursor&);
 std::optional<uint32_t> lookup(uint32_t name,PackedNpcLookup=PackedNpcLookup::NoAssert);
 std::optional<std::vector<uint32_t>> activeActors() const; // Active1, not query/display readiness.
private:
 friend class FieldNpcSetupBinding;
 friend class FieldNpcMainEntryBinding;
 friend class FieldNpcClockBinding;
 FieldNpcRegistryBinding(std::shared_ptr<FieldNpcRegistryOwner>,EventAddressSpace&);
 struct Call;
 PackedNpcRegistryServices services(Call&);
 bool text(uint32_t,std::string&,unsigned limit) const;
 bool registry(uint32_t& base,uint32_t& array,uint32_t& count) const;
 std::shared_ptr<FieldNpcRegistryOwner> owner_;
 EventAddressSpace& memory_;
 std::vector<uint32_t> mapped_;
 FieldNpcEntryCursor implicitEntry_;
};
}

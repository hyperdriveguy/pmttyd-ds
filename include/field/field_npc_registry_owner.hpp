#pragma once
#include "field_npc_registry.hpp"
#include "field_pose_pool.hpp"
#include <array>
namespace ttyd {
class EventAddressSpace;
class FieldNpcRegistryBinding;
// One cold-boot NPC owner. Stable native allocations have explicit guest
// identities, not original __memAlloc addresses. Cold BSS counts are zero;
// npcInit itself does not clear them. Room reset/deletion remain separate.
class FieldNpcRegistryOwner {
public:
 static std::shared_ptr<FieldNpcRegistryOwner> cold(uint32_t allocationBase);
 FieldNpcRegistryOwner(const FieldNpcRegistryOwner&)=delete;
 FieldNpcRegistryOwner& operator=(const FieldNpcRegistryOwner&)=delete;
 bool initialized() const{return initialized_;}
 FieldPosePool& poses(){return poses_;}
 const FieldPosePool& poses() const{return poses_;}
private:
 friend class FieldNpcRegistryBinding;
 explicit FieldNpcRegistryOwner(uint32_t);
 uint8_t* span(uint32_t,uint32_t);
 std::array<uint8_t,0x30> registry_{}; // Source startup BSS8041e8ac..e8db.
 std::array<uint32_t,4> addresses_{};
 std::array<std::vector<uint8_t>,4> allocations_;
 FieldPosePool poses_;
 bool initialized_=false;
};
// Must retire before its facade. Retains the owner; unregisters only its own
// external spans. Multiple independent facades alias the same actor bytes.
class FieldNpcRegistryBinding {
public:
 static std::unique_ptr<FieldNpcRegistryBinding> bind(std::shared_ptr<FieldNpcRegistryOwner>,EventAddressSpace&);
 ~FieldNpcRegistryBinding();
 FieldNpcRegistryBinding(const FieldNpcRegistryBinding&)=delete;
 FieldNpcRegistryBinding& operator=(const FieldNpcRegistryBinding&)=delete;
 bool initialize(); // Once, actual npcInit. Never a room reset.
 std::optional<int32_t> enter(uint32_t name,uint32_t model,BattlePoseClock);
 std::optional<uint32_t> lookup(uint32_t name,PackedNpcLookup=PackedNpcLookup::NoAssert);
 std::optional<std::vector<uint32_t>> activeActors() const; // Active1, not query/display readiness.
private:
 FieldNpcRegistryBinding(std::shared_ptr<FieldNpcRegistryOwner>,EventAddressSpace&);
 struct Call;
 PackedNpcRegistryServices services(Call&);
 bool text(uint32_t,std::string&,unsigned limit) const;
 bool registry(uint32_t& base,uint32_t& array,uint32_t& count) const;
 std::shared_ptr<FieldNpcRegistryOwner> owner_;
 EventAddressSpace& memory_;
 std::vector<uint32_t> mapped_;
};
}

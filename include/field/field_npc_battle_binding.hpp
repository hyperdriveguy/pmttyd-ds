#pragma once
#include "field_npc_battle_event.hpp"
#include "field_npc_setup_binding.hpp"
#include "original_sequence.hpp"
#include <map>
namespace ttyd {
// Native provenance only: actual retirement snapshots, never fabricated source
// IDs or writes. Create with map ownership before regular events can retire;
// retain through late battle binding, clear on source context ReInit/unload.
class FieldNpcRegularEventHistory {
public:
 static std::shared_ptr<FieldNpcRegularEventHistory> bind(std::shared_ptr<FieldNpcRegistryOwner>,EventNativeRuntime&);
 bool matches(const std::shared_ptr<FieldNpcRegistryOwner>&,const EventNativeRuntime&) const;
 void retire(const EventEntry&);
 bool owns(uint32_t id,uint32_t actor) const;
private:
 FieldNpcRegularEventHistory(std::shared_ptr<FieldNpcRegistryOwner>,EventNativeRuntime&);
 bool context() const;
 bool regular(const EventEntry&,uint32_t actor) const;
 std::shared_ptr<FieldNpcRegistryOwner> owner_;EventNativeRuntime& vm_;
 std::map<uint32_t,std::array<uint32_t,2>> retired_;
};
// Actual same-VM registry/F/result/sequence bridge. Backend owns the remaining
// Mario/party/audio children, including any held continuations. Borrowed VM,
// setup and sequence must outlive this facade and its pending/retired calls.
class FieldNpcBattleBinding {
public:
 static std::unique_ptr<FieldNpcBattleBinding> bind(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcSetupBinding&,EventNativeRuntime&,OriginalSequence&,const BattleSequenceEntryServices&,std::shared_ptr<FieldNpcRegularEventHistory> = {});
 ~FieldNpcBattleBinding();
 std::optional<int32_t> invoke(uint32_t,EventEntry&,bool first);
 void retire(const EventEntry&);
 bool pending() const;
 bool quarantined() const{return !retired_.empty();}
 uint32_t pendingFunction() const{return pending_;}
private:
 struct Call;
 FieldNpcBattleBinding(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcSetupBinding&,EventNativeRuntime&,OriginalSequence&,const BattleSequenceEntryServices&,uint32_t gp,std::shared_ptr<FieldNpcRegularEventHistory>);
 bool current() const;
 BattleSequenceEntryServices services(Call&);
 std::shared_ptr<FieldNpcRegistryOwner> owner_;FieldNpcSetupBinding& setup_;
 EventNativeRuntime& vm_;OriginalSequence& sequence_;BattleSequenceEntryServices backend_;
 std::shared_ptr<FieldNpcRegularEventHistory> regular_;
 uint32_t gp_=0,pending_=0;
 std::map<uint32_t,std::unique_ptr<Call>> calls_;
 std::vector<std::unique_ptr<Call>> retired_;
};
}

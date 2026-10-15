#pragma once
#include "field_npc_creation_event.hpp"
#include "field_npc_setup_binding.hpp"
#include "animation_group_request_binding.hpp"
#include "native_field_animation.hpp"
namespace ttyd {
// Original callback bridge over actual registry, live event operands and the
// same native animation/file process. Source VM/registry/setup outlive binding;
// source script/string RAM stays immovable until borrowed views retire.
class FieldNpcCreationBinding {
public:
 static std::unique_ptr<FieldNpcCreationBinding> bind(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcRegistryBinding&,FieldNpcSetupBinding&,EventNativeRuntime&,std::shared_ptr<NativeFieldAnimation>);
 ~FieldNpcCreationBinding();
 std::optional<int32_t> invoke(uint32_t function,EventEntry&);
 // Drive only the retained native invocation, without interpreter/input/clock
 // progress. Completed scalar is consumed by the next matching USER_FUNC call.
 bool resume();
 bool pending() const{return caller_ && !completed_;}
 bool retained() const{return caller_!=nullptr;}
 uint32_t pendingFunction() const{return caller_?function_:0;}
 uint32_t pendingEventId() const{return caller_?id_:0;}
 bool quarantined() const{return quarantined_;}
 void retire(const EventEntry&);
 bool pump(uint32_t bytes=4096){return animation_->files()->advanceAsync(bytes);}
 void budgets(unsigned group,unsigned selection){groupBudget_=group;selectionBudget_=selection;}
private:
 FieldNpcCreationBinding(std::shared_ptr<FieldNpcRegistryOwner>,FieldNpcRegistryBinding&,FieldNpcSetupBinding&,EventNativeRuntime&,std::shared_ptr<NativeFieldAnimation>);
 FieldNpcCreationEventServices services();
 bool caller(const EventEntry&) const;bool current() const;
 bool text(uint32_t,std::string&,unsigned=32) const;
 bool alias(uint32_t);std::optional<uint32_t> tribe(uint32_t);
 void finish();
 std::shared_ptr<FieldNpcRegistryOwner> owner_;FieldNpcRegistryBinding& registry_;FieldNpcSetupBinding& setup_;EventNativeRuntime& vm_;
 std::shared_ptr<NativeFieldAnimation> animation_;
 std::unique_ptr<FieldNpcRegistryBinding> animationRegistry_;
 std::unique_ptr<AnimationGroupRequestBinding> groups_;
 struct Alias {uint32_t address,size;std::string bytes;};std::vector<Alias> aliases_;
 FieldNpcCreationEventCursor cursor_;FieldNpcEntryCursor entry_;
 EventEntry* caller_=nullptr;uint32_t id_=0,function_=0,gp_=0;
 std::optional<int32_t> completed_;bool quarantined_=false;
 unsigned groupBudget_=8192,selectionBudget_=64;
};
}

#pragma once
#include "field_party_native_entry.hpp"
#include "event_operands.hpp"
#include "event_address_space.hpp"
namespace ttyd {
// One actual evt_mario_set_party_pos or set_prev_party_pos invocation. Native
// holds retain its first-call source execution; initial first=false is a no-op.
// EventEntry, memory, typed owners and resolved providers stay stationary.
class NativeFieldPartyPositionEvent {
public:
 NativeFieldPartyPositionEvent(std::string root,FieldPartyRegistry& p,FieldMarioBattleState& m,std::shared_ptr<ProloguePouch> q={},NativeFieldPartyEntry::Trace trace={}):root_(std::move(root)),parties_(p),mario_(m),pouch_(std::move(q)),trace_(std::move(trace)){}
 // Native return0 retains reached invocation;source return2 only at Done.
 // First is captured on entry:later VM first=false cannot discard a held load.
 // Direct constructor result>=0 publishes low byte only after full construction.
 int32_t step(EventEntry&,bool first,bool previous,EventAddressSpace&,const EventOperandServices&,std::shared_ptr<const ActorAnimationPack>,uint64_t,const FieldPartyAppearanceServices&);
 uint32_t pending() const{return pending_;}
 bool complete() const{return stage_==Stage::Done;}
 int32_t constructorResult() const{return result_;}
private:
 // Explicit:raw selector,kind,X,Y,Z. Previous:raw selector,X,Y,Z,saved kind.
 // Args address,entry pointer/id and memory identity stay captured;provider
 // identity pins once value resolution begins. Done remains terminal.
 enum class Stage {Start,Selector,Guard,Kind,X,Y,Z,SavedKind,Construct,Publish,Done};
 std::string root_;FieldPartyRegistry& parties_;FieldMarioBattleState& mario_;std::shared_ptr<ProloguePouch> pouch_;NativeFieldPartyEntry::Trace trace_;
 Stage stage_=Stage::Start;EventEntry* entry_=nullptr;EventAddressSpace* memory_=nullptr;EventOperandServices operands_{};
 bool first_=false,previous_=false,pinned_=false;uint32_t args_=0,selector_=0,pending_=0,entryId_=0;int32_t kind_=0,result_=-1;
 std::array<float,3> position_{};std::unique_ptr<NativeFieldPartyEntry> constructor_;
 uint32_t function() const{return previous_?0x800e2f00:0x800e3120;}
 bool providers(const EventOperandServices&);
 bool value(unsigned,EventEntry&,const EventOperandServices&,int32_t&);
};
}

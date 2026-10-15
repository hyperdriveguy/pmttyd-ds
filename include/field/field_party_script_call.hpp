#pragma once
#include "field_party_position_event.hpp"
namespace ttyd {
class EventNativeRuntime;
// Bind actual native field owners to an independently owned story/event VM.
// Each field owner remains stationary through a pending invocation. Display/math
// providers may become available before their reached use. Borrowed providers
// are never dereferenced by cursor destruction.
struct FieldPartyScriptBindings {
 FieldPartyRegistry* parties=nullptr;FieldMarioBattleState* mario=nullptr;
 std::optional<int32_t>* currentIndex=nullptr; // Original independent8041ead0 view.
 std::shared_ptr<ProloguePouch> pouch;std::string root;
 std::shared_ptr<const ActorAnimationPack> bank;
 FieldPartyAppearanceServices appearance;
 bool bound() const{return parties && mario;}
};
// One USER_FUNC invocation. Caller erases only after result2, and owns active
// entry/VM lifetime; disposal is not native cancellation or source completion.
class NativeFieldPartyScriptCall {
public:
 int32_t step(EventEntry&,bool first,uint32_t function,EventNativeRuntime&,const FieldPartyScriptBindings&,uint64_t);
 uint32_t pending() const{return pending_;}
 // Entry retirement can dispose pure leaves/mode prefixes. Opaque native
 // constructor/release children require quarantine until cancellation is bound.
 bool releaseForRetirement();
private:
 enum class Stage {Start,Bind,Address,Dispatch,JoinRaw,JoinValue,Join,ModeRaw,ModeLookup,ModeLease,ModeActor,ModeGate,ModeOuter,ModeInner,Slit,ModeFinish,Done};
 Stage stage_=Stage::Start;FieldPartyScriptBindings binding_;EventNativeRuntime* vm_=nullptr;EventEntry* entry_=nullptr;
 std::shared_ptr<NativeFieldAnimation> native_;std::shared_ptr<BattlePosePool> pool_;
 uint32_t function_=0,pending_=0,id_=0,address_=0,args_=0,raw_=0,selector_=0;
 bool first_=false,bound_=false,issued_=false,slit_=false;int32_t kind_=0,index_=-1,oldMode_=0,newMode_=0;
 FieldPartyActor* actor_=nullptr;
 std::shared_ptr<int> modeLease_=std::make_shared<int>(0);
 std::unique_ptr<NativeFieldPartyPositionEvent> position_;
 std::unique_ptr<NativeFieldPartyLifetime> lifetime_;
 FieldPartyRemovalEventCursor removal_;
 BattleSequenceEntryServices removalServices();
 bool call(BattleSequenceCall&);
 void next(Stage s){stage_=s;issued_=false;}
 bool same(const FieldPartyScriptBindings&) const;
};
}

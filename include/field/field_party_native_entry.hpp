#pragma once
#include "field_party_native_lifetime.hpp"
#include "field_party_appearance.hpp"
#include "prologue_pouch.hpp"
#include <functional>
namespace ttyd {
// Retained ordinary Goombella caller/Main/Entry/fresh Hello and positioned
// ordinary1/auxiliary13 construction. Other kinds, d_mario and nonordinary
// Hello dispatch remain explicit provider boundaries.
// Keep this object, registry, Mario and pouch stationary through completion.
// Disposal releases only native exclusion ownership; it never invents source
// teardown/completion or dereferences borrowed registry/Mario during destruction.
class NativeFieldPartyEntry {
public:
 // Trace reports first reached source request identities, not scheduling or
 // completion. Form/Phase are native continuations. Done stays terminal, so a
 // held outer caller cannot replay a completed constructor. No implicit reset.
 using Trace=std::function<void(uint32_t,std::initializer_list<int32_t>)>;
 // Position constructors do not perform the pouch join gate or publish Mario
 // slot indices. Ordinary kind1 uses Main; auxiliary kind13 uses only slot1.
 // xyz bits(including signed zero) bind this invocation. Ordinary Position
 // finishes mode0;Secondary kind13 uses mode2 and previousXYZ/unit scalars.
 // Direct APIs do not publish Mario slot bytes; the EVT wrapper owns that.
 bool position(int32_t kind,bool secondary,const std::array<float,3>&,std::shared_ptr<const ActorAnimationPack>,uint64_t,const FieldPartyAppearanceServices&,int32_t& result);
 static std::unique_ptr<NativeFieldPartyEntry> create(const char*,FieldPartyRegistry&,FieldMarioBattleState&,std::shared_ptr<ProloguePouch>,Trace={});
 bool step(int32_t kind,std::shared_ptr<const ActorAnimationPack>,uint64_t,const FieldPartyAppearanceServices&,int32_t& result);
 uint32_t pending() const{return pending_;}
 bool complete() const{return phase_==Phase::Done;}
 int32_t slot() const{return slot_;}
private:
 enum class Form {Ordinary,Position,Secondary};
 // Each next() retires only that stage's issued request latch. Grouped raw
 // source reads require complete mappings; this is not instruction-level replay
 // of arbitrary partial-RAM faults. Fixed owners/pool/actor survive all holds.
 enum class Phase {Caller,Kill,Allocate,AuxAllocate,Resource,ResourceRead,Release,Enter,Paper,Owner,MaterialSelect,MaterialGet,MaterialOn,LightSelect,LightGet,LightOn,
  MainSelect,MainCompare,MainAgain,MainFinish,InitRead,Init,InitWidth,InitHeight,AuxMode,AuxModeInner,AuxModeFinish,AuxUnit,AuxSeed,AuxPosition,PosVector,PosVectorZ,PosSeed,PosPosition,PosOwner,PosFinalVector,PosFinalZ,PosSelect,PosCompare,PosFinish,Scale,ScaleZ,ScaleOwner,DisplayFirst,DisplaySecond,Place,PlaceSelect,PlaceCompare,Publish,Hello,HelloMain,Done};
 NativeFieldPartyEntry(FieldPartyRegistry& p,FieldMarioBattleState& m,std::shared_ptr<ProloguePouch> q,Trace t):parties_(p),mario_(m),pouch_(std::move(q)),trace_(std::move(t)){}
 FieldPartyRegistry& parties_;FieldMarioBattleState& mario_;std::shared_ptr<ProloguePouch> pouch_;Trace trace_;
 std::shared_ptr<NativeFieldAnimation> native_;std::shared_ptr<BattlePosePool> pool_;
 EventScriptAssets data_,definitions_;std::unique_ptr<NativeFieldPartyLifetime> lifetime_;
 std::shared_ptr<int> lease_=std::make_shared<int>(0);
 Form form_=Form::Ordinary;bool started_=false;std::array<uint32_t,3> position_{};int32_t oldMode_=0;uint32_t unit_=0;
 Phase phase_=Phase::Caller;bool issued_=false,hello_=false;int32_t kind_=-1,slot_=-1,oldIndex_=-1,handle_=-1,childHandle_=-1,result_=-1;
 int32_t selector_=1;
 uint32_t pending_=0,resource_=0,mask_=0,selected_=0,oldName_=0,definition_=0,init_=0,width_=0,height_=0;
 FieldPartyActor* actor_=nullptr;FieldMarioBattleState* capturedMario_=nullptr;
 void next(Phase p){phase_=p;issued_=false;}
 void request(uint32_t,std::initializer_list<int32_t> ={});
 bool word(uint32_t,uint32_t&) const;
 bool byte(uint32_t,uint8_t&) const;
 FieldPartyPose* pose(int32_t) const;
 bool advance(int32_t,std::shared_ptr<const ActorAnimationPack>,uint64_t,const FieldPartyAppearanceServices&,int32_t&);
 uint32_t entryFunction() const{return form_==Form::Secondary?0x800bd6d8u:form_==Form::Position?0x800be84cu:0x801412fcu;}
 bool seed(uint32_t) const;
 bool select();bool compare(bool second=false);void updateName();
};
}

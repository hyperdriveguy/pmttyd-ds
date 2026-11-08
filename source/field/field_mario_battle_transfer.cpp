#include "field_mario_battle_transfer.hpp"
namespace ttyd {
namespace {
constexpr uint32_t MarioPointer=0x8041e900,SavedPosition=0x8041e8f4;
constexpr uint32_t PartyPointers=0x8041ead8,PartyDefinitions=0x803143e8;
constexpr uint32_t BattlePrepared=0x1000,PartyControlDisabled=0x20000000;
struct Guard{bool&value;explicit Guard(bool&v):value(v){value=true;}~Guard(){value=false;}};
bool same(const FieldMarioBattleTransferServices&a,const FieldMarioBattleTransferServices&b){
 return a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.user==b.user&&a.current==b.current&&a.word==b.word&&a.half==b.half&&a.byte==b.byte&&a.store==b.store&&a.call==b.call;
}
}
struct FieldMarioBattleTransfer::State {
 enum class Pc{Start,Mario,PrepareX,PrepareY,SaveX,SaveY,PrepareZ,SaveZ,Adjust,PrepareMotion,PrepareFlags,PrepareStore,
  PostFlags,PostStore,PostMotion,RestoreX,RestoreY,WriteX,WriteY,RestoreZ,WriteZ,Motion26,Action26,
  PartyId,PartyIdOutput,PartyPointer,PartyPointerOutput,UseMode,UseKind,UseCallback,UseCall,
  Appearance,AppearanceXY,AppearanceX,AppearanceY,AppearanceZRead,AppearanceZ,
  ControlPointer,ControlFlags,ControlStore,PostFinalMotion,PostMode,Done,Invalid};
 uint32_t function=0,pending=0,mario=0,party=0,value=0,x=0,y=0,z=0,index=0,ability=0;
 Pc pc=Pc::Start;bool bound=false,poisoned=false;FieldMarioBattleTransferServices services;
 FieldMarioBattleTransferRequest child;bool queued=false;
 std::array<std::optional<uint32_t>,3> appearance{};
 explicit State(uint32_t fn):function(fn){if(fn!=MarioFieldBattlePrepare&&fn!=MarioFieldBattlePost)pc=Pc::Invalid;}
 bool current(){if(poisoned)return false;if(!services.current(services.user)){poisoned=true;return false;}return true;}
 bool call(uint32_t fn,uint32_t site,uint32_t a=0,bool vector=false){
  if(!queued){child={};child.function=fn;child.callsite=site;child.arguments[0]=a;if(vector)child.privateVector=&appearance;queued=true;}
  pending=child.function;if(!services.call||!services.call(services.user,child)||!current())return false;
  queued=false;return true;
 }
};
FieldMarioBattleTransfer::FieldMarioBattleTransfer(uint32_t fn):state_(std::make_unique<State>(fn)){}
FieldMarioBattleTransfer::~FieldMarioBattleTransfer()=default;
bool FieldMarioBattleTransfer::complete()const{return state_->pc==State::Pc::Done;}
uint32_t FieldMarioBattleTransfer::pending()const{return state_->pending;}
bool FieldMarioBattleTransfer::resume(const FieldMarioBattleTransferServices&services){
 if(invoking_||!services.lifetime||!services.current)return false;
 Guard guard(invoking_);auto&s=*state_;using Pc=State::Pc;
 if(s.bound&&!same(s.services,services))return false;
 if(!s.bound){s.services=services;s.bound=true;}
 if(!s.current()||s.pc==Pc::Invalid)return false;
 auto word=[&](uint32_t a,uint32_t&v){s.pending=a;if(!services.word)return false;auto r=services.word(services.user,a);if(!r||!s.current())return false;v=*r;return true;};
 auto half=[&](uint32_t a,uint32_t&v){s.pending=a;if(!services.half)return false;auto r=services.half(services.user,a);if(!r||!s.current())return false;v=*r;return true;};
 auto byte=[&](uint32_t a,uint32_t&v){s.pending=a;if(!services.byte)return false;auto r=services.byte(services.user,a);if(!r||!s.current())return false;v=*r;return true;};
 auto store=[&](uint32_t a,uint32_t v){s.pending=a;return services.store&&services.store(services.user,a,v)&&s.current();};
 for(;;){if(!s.current())return false;switch(s.pc){
 case Pc::Start:s.pc=Pc::Mario;break;
 case Pc::Mario:if(!word(MarioPointer,s.mario))return false;s.pc=s.function==MarioFieldBattlePrepare?Pc::PrepareX:Pc::PostFlags;break;
 case Pc::PrepareX:if(!word(s.mario+0x8c,s.x))return false;s.pc=Pc::PrepareY;break;
 case Pc::PrepareY:if(!word(s.mario+0x90,s.y))return false;s.pc=Pc::SaveX;break;
 case Pc::SaveX:if(!store(SavedPosition,s.x))return false;s.pc=Pc::SaveY;break;
 case Pc::SaveY:if(!store(SavedPosition+4,s.y))return false;s.pc=Pc::PrepareZ;break;
 case Pc::PrepareZ:if(!word(s.mario+0x94,s.z))return false;s.pc=Pc::SaveZ;break;
 case Pc::SaveZ:if(!store(SavedPosition+8,s.z))return false;s.pc=Pc::Adjust;break;
 case Pc::Adjust:if(!s.call(0x800965fc,0x8005c3f8))return false;s.pc=Pc::PrepareMotion;break;
 case Pc::PrepareMotion:if(!half(s.mario+0x2e,s.value))return false;s.pc=s.value==20?Pc::PartyId:Pc::PrepareFlags;break;
 case Pc::PrepareFlags:if(!word(s.mario,s.value))return false;s.value|=BattlePrepared;s.pc=Pc::PrepareStore;break;
 case Pc::PrepareStore:if(!store(s.mario,s.value))return false;s.pc=Pc::PartyId;break;
 case Pc::PostFlags:if(!word(s.mario,s.value))return false;s.value&=~BattlePrepared;s.pc=Pc::PostStore;break;
 case Pc::PostStore:if(!store(s.mario,s.value))return false;s.pc=Pc::PostMotion;break;
 case Pc::PostMotion:if(!half(s.mario+0x2e,s.value))return false;s.pc=s.value==20?Pc::RestoreX:Pc::Motion26;break;
 case Pc::RestoreX:if(!word(SavedPosition,s.x))return false;s.pc=Pc::RestoreY;break;
 case Pc::RestoreY:if(!word(SavedPosition+4,s.y))return false;s.pc=Pc::WriteX;break;
 case Pc::WriteX:if(!store(s.mario+0x8c,s.x))return false;s.pc=Pc::WriteY;break;
 case Pc::WriteY:if(!store(s.mario+0x90,s.y))return false;s.pc=Pc::RestoreZ;break;
 case Pc::RestoreZ:if(!word(SavedPosition+8,s.z))return false;s.pc=Pc::WriteZ;break;
 case Pc::WriteZ:if(!store(s.mario+0x94,s.z))return false;s.pc=Pc::Motion26;break;
 case Pc::Motion26:if(!half(s.mario+0x2e,s.value))return false;s.pc=s.value==26?Pc::Action26:Pc::PartyId;break;
 case Pc::Action26:if(!s.call(0x800b6ad8,0x8005c360))return false;s.pc=Pc::PartyId;break;
 case Pc::PartyId:if(!s.call(0x80141050,s.function==MarioFieldBattlePrepare?0x8005c414:0x8005c364))return false;s.pc=Pc::PartyIdOutput;break;
 case Pc::PartyIdOutput:if(!s.child.result)return false;s.value=*s.child.result;s.pc=Pc::PartyPointer;break;
 case Pc::PartyPointer:if(!s.call(0x800bf1b4,s.function==MarioFieldBattlePrepare?0x8005c418:0x8005c368,s.value))return false;s.pc=Pc::PartyPointerOutput;break;
 case Pc::PartyPointerOutput:if(!s.child.result)return false;s.party=*s.child.result;s.index=0;s.pc=s.function==MarioFieldBattlePrepare?Pc::UseMode:(s.party?Pc::Appearance:Pc::ControlPointer);break;
 case Pc::UseMode:if(!s.party){s.pc=Pc::ControlPointer;break;}if(!byte(s.party+0x34,s.value))return false;s.pc=s.value==3?Pc::UseKind:Pc::ControlPointer;break;
 case Pc::UseKind:if(!byte(s.party+0x31,s.value))return false;{const int32_t kind=s.value<128?int32_t(s.value):int32_t(s.value)-256;s.value=PartyDefinitions+uint32_t(kind*40)+0x10;}s.pc=Pc::UseCallback;break;
 case Pc::UseCallback:if(!word(s.value,s.ability))return false;s.pc=s.ability?Pc::UseCall:Pc::ControlPointer;break;
 case Pc::UseCall:if(!s.call(s.ability,0x800bcf7c,s.party))return false;s.pc=Pc::ControlPointer;break;
 case Pc::Appearance:if(!s.call(0x800c6c9c,0x8005c378,s.party,true))return false;s.pc=Pc::AppearanceXY;break;
 case Pc::AppearanceXY:if(!s.appearance[0]||!s.appearance[1])return false;s.x=*s.appearance[0];s.y=*s.appearance[1];s.pc=Pc::AppearanceX;break;
 case Pc::AppearanceX:if(!store(s.party+0x58,s.x))return false;s.pc=Pc::AppearanceY;break;
 case Pc::AppearanceY:if(!store(s.party+0x5c,s.y))return false;s.pc=Pc::AppearanceZRead;break;
 case Pc::AppearanceZRead:if(!s.appearance[2])return false;s.z=*s.appearance[2];s.pc=Pc::AppearanceZ;break;
 case Pc::AppearanceZ:if(!store(s.party+0x60,s.z))return false;s.pc=Pc::ControlPointer;break;
 case Pc::ControlPointer:if(!word(PartyPointers+s.index*4,s.party))return false;s.pc=s.party?Pc::ControlFlags:Pc::ControlStore;if(!s.party){if(++s.index==2)s.pc=s.function==MarioFieldBattlePrepare?Pc::Done:Pc::PostFinalMotion;else s.pc=Pc::ControlPointer;}break;
 case Pc::ControlFlags:if(!word(s.party,s.value))return false;s.value=s.function==MarioFieldBattlePrepare?s.value|PartyControlDisabled:s.value&~PartyControlDisabled;s.pc=Pc::ControlStore;break;
 case Pc::ControlStore:if(!store(s.party,s.value))return false;if(++s.index==2)s.pc=s.function==MarioFieldBattlePrepare?Pc::Done:Pc::PostFinalMotion;else s.pc=Pc::ControlPointer;break;
 case Pc::PostFinalMotion:if(!half(s.mario+0x2e,s.value))return false;s.pc=s.value==18||s.value==20?Pc::PostMode:Pc::Done;break;
 case Pc::PostMode:if(!s.call(0x8009455c,0x8005c3b0,0))return false;s.pc=Pc::Done;break;
 case Pc::Done:s.pending=0;return true;
 case Pc::Invalid:return false;
 }}
}
}

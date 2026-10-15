#include "field_party_position_event.hpp"
namespace ttyd {
bool NativeFieldPartyPositionEvent::providers(const EventOperandServices& s){
 if(!s.contextAddress || !s.entryAddress || !s.globalAddress || !s.readWord || !s.readByte || !s.truncate)return false;
 if(!pinned_){operands_=s;pinned_=true;return true;}
 return s.user==operands_.user && s.contextAddress==operands_.contextAddress && s.entryAddress==operands_.entryAddress && s.globalAddress==operands_.globalAddress && s.readWord==operands_.readWord && s.readByte==operands_.readByte && s.truncate==operands_.truncate;
}
bool NativeFieldPartyPositionEvent::value(unsigned index,EventEntry& e,const EventOperandServices& s,int32_t& out){
 if(!providers(s))return false;
 uint32_t raw=0;if(!memory_->readWord(args_+4*index,raw))return false;
 if(trace_)trace_(0x80031260,{int32_t(raw)});
 out=getEventValue(e,int32_t(raw),operands_);return true;
}
int32_t NativeFieldPartyPositionEvent::step(EventEntry& e,bool first,bool previous,EventAddressSpace& memory,const EventOperandServices& operands,std::shared_ptr<const ActorAnimationPack> bank,uint64_t now,const FieldPartyAppearanceServices& appearance){
 pending_=0;
 if(stage_!=Stage::Start && (&e!=entry_ || uint32_t(e.id)!=entryId_ || &memory!=memory_ || previous!=previous_)){pending_=function();return 0;}
 for(;;)switch(stage_){
 case Stage::Start:
  entry_=&e;entryId_=uint32_t(e.id);memory_=&memory;first_=first;previous_=previous;args_=e.arguments;
  if(trace_){trace_(function(),{});trace_(0x8005e0c0,{});}stage_=Stage::Selector;break;
 case Stage::Selector:
  if(!memory_->readWord(args_,selector_)){pending_=function();return 0;}stage_=Stage::Guard;break;
 case Stage::Guard:
  if(!first_){stage_=Stage::Done;break;}
  if(selector_>1){pending_=function();return 0;} // Native typed byte-domain admission; source raw arithmetic is unchecked.
  if(int8_t(mario_.partyIndices[selector_])>=0){stage_=Stage::Done;break;}
  stage_=previous_?Stage::X:Stage::Kind;break;
 case Stage::Kind:
  if(!value(1,e,operands,kind_)){pending_=0x80031260;return 0;}stage_=Stage::X;break;
 case Stage::X:case Stage::Y:case Stage::Z:{
  const unsigned axis=stage_==Stage::X?0:stage_==Stage::Y?1:2;int32_t resolved=0;
  if(!value((previous_?1:2)+axis,e,operands,resolved)){pending_=0x80031260;return 0;}
  // Original signed-bias double conversion followed by single rounding is
  // equivalent for signed32 values under the supported round-to-nearest mode.
  position_[axis]=float(resolved);
  stage_=axis==0?Stage::Y:axis==1?Stage::Z:previous_?Stage::SavedKind:Stage::Construct;break;}
 case Stage::SavedKind:
  kind_=int8_t(mario_.savedPartyKinds[selector_]);if(!kind_){stage_=Stage::Done;break;}stage_=Stage::Construct;break;
 case Stage::Construct:
  if(!constructor_)constructor_=NativeFieldPartyEntry::create(root_.c_str(),parties_,mario_,pouch_,trace_);
  if(!constructor_){pending_=selector_?0x800bd6d8:0x800be84c;return 0;}
  if(!constructor_->position(kind_,selector_!=0,position_,bank,now,appearance,result_)){pending_=constructor_->pending();return 0;}
  stage_=Stage::Publish;break;
 case Stage::Publish:
  if(result_>=0)mario_.partyIndices[selector_]=uint8_t(result_);
  stage_=Stage::Done;break;
 case Stage::Done:return 2;
 }
}
}

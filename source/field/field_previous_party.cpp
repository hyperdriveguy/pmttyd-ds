#include "field_previous_party.hpp"
#include "event_manager.hpp"
namespace ttyd {
namespace {int32_t signedByte(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}}
int32_t fieldPreviousPartyEvent(EventEntry& e,bool first,FieldPreviousPartyCursor& c,
 void* operands,uint32_t (*word)(void*,uint32_t),const FieldPreviousPartyWorld& w) {
 if(first)c={};
 c.firstVisit(first);
 if(c.stage==FieldPreviousPartyCursor::Stage::Complete)return 2;
 if(c.stage==FieldPreviousPartyCursor::Stage::Unsupported)return 0;
 if(c.stage!=FieldPreviousPartyCursor::Stage::Start && w.user!=c.owner)return 0;
 if(c.stage==FieldPreviousPartyCursor::Stage::Start) {
  if(!word || !w.mario || !w.byte)return 0;
  const auto args=e.arguments;const auto mario=w.mario(w.user);
  // Missing owner is native admission, not a source successful null actor.
  if(!mario)return 0;
  const auto selector=word(operands,args);c.owner=w.user;c.mario=mario;c.selector=selector;
  if(selector>1){c.stage=FieldPreviousPartyCursor::Stage::Unsupported;return 0;}
  if(c.sourceFirst) {
   if(signedByte(w.byte(w.user,mario+0x245+selector))>=0){c.stage=FieldPreviousPartyCursor::Stage::Complete;return 2;}
   c.kind=signedByte(w.byte(w.user,mario+0x247+selector));
   if(c.kind==0){c.stage=FieldPreviousPartyCursor::Stage::Complete;return 2;}
   c.stage=FieldPreviousPartyCursor::Stage::Constructor;
  } else c.stage=FieldPreviousPartyCursor::Stage::Tail;
 }
 if(c.stage==FieldPreviousPartyCursor::Stage::Constructor) {
  if(!w.enter || !w.storeByte || !w.byte)return 0;
  const auto result=w.enter(w.user,c.kind,c.selector!=0);if(!result)return 0;
  if(*result<0){c.stage=FieldPreviousPartyCursor::Stage::Complete;return 2;}
  w.storeByte(w.user,c.mario+0x245+c.selector,uint8_t(*result));
  c.stage=FieldPreviousPartyCursor::Stage::Tail;
 }
 if(c.stage==FieldPreviousPartyCursor::Stage::Tail) {
  if(!w.byte)return 0;
  c.index=signedByte(w.byte(w.user,c.mario+0x245+c.selector));c.stage=FieldPreviousPartyCursor::Stage::Lookup;
 }
 if(c.stage==FieldPreviousPartyCursor::Stage::Lookup) {
  if(!w.party || !w.party(w.user,c.index))return 0; // optional distinguishes actual null0.
  c.stage=FieldPreviousPartyCursor::Stage::Complete;return 2;
 }
 return 0;
}
}

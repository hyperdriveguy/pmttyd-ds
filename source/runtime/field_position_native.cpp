#include "game.hpp"
#include "event_manager.hpp"
#include "event_operands.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
constexpr uintptr_t Mario=FieldMarioOwnerAddress,Camera=FieldMarioOwnerAddress+0x1000;
uint32_t bits(float value){uint32_t out;std::memcpy(&out,&value,4);return out;}
float scalar(uint32_t value){float out;std::memcpy(&out,&value,4);return out;}
float& lane(CameraPoint& p,unsigned n){assert(n<3);return n==0?p.x:n==1?p.y:p.z;}
bool triple(uintptr_t address,uintptr_t base,unsigned& n){if(address<base || address>=base+12 || ((address-base)&3))return false;n=unsigned((address-base)/4);return true;}
}
FieldPositionWorld Game::fieldPositionWorld() {
 FieldPositionWorld w;w.user=this;
 w.mario=[](void*)->uintptr_t{return Mario;};
 w.currentParty=[](void* p){const auto& g=*static_cast<Game*>(p);assert(g.fieldPartyCurrentIndex);return *g.fieldPartyCurrentIndex;};
 w.party=[](void* p,int32_t index)->uintptr_t{return reinterpret_cast<uintptr_t>(static_cast<Game*>(p)->fieldParties.pointer(index));};
 w.camera=[](void*,int32_t index)->uintptr_t{assert(index==4);return Camera;};
 w.read=[](void* p,uintptr_t address)->uint32_t {
  auto& g=*static_cast<Game*>(p);unsigned n=0;
  if(address==Mario)return g.fieldMarioBattle.flags;
  if(address==Mario+4)return g.fieldCameraDisplayFlags;
  if(address==Mario+0x184)return bits(g.fieldMarioBody.walkBase);
  if(address==Mario+0x188)return bits(g.fieldMarioBody.dashBase);
  if(address==Mario+0x1b8)return bits(g.fieldMarioBody.width);
  if(address==Mario+0x1bc)return bits(g.fieldMarioBody.height);
  if(triple(address,Mario+0x8c,n))return bits(n==0?g.progress.position.x*100:n==1?(g.groundHeight+g.jumpHeight)*100:g.progress.position.z*100);
  if(triple(address,Mario+0xec,n))return bits(lane(g.marioCamera.retained,n));
  if(triple(address,Mario+0xf8,n))return bits(lane(g.marioCameraCollision,n));
  if(triple(address,Mario+0x2e4,n)){assert(g.fieldRecoveryWords[n]);return *g.fieldRecoveryWords[n];}
  if(address==Mario+0x2e0){assert(g.fieldRecoveryReload);return *g.fieldRecoveryReload;}
  if(triple(address,Camera+0x94,n))return bits(lane(g.fieldCamera.point,n));
  if(triple(address,Camera+0xac,n))return bits(lane(g.fieldCameraCollisionTarget,n));
  for(const auto& actor:g.fieldParties.actors)if(actor) {
   const auto base=reinterpret_cast<uintptr_t>(actor.get());
   if(address>=base && address<base+actor->bytes.size())return actor->word(unsigned(address-base));
  }
  assert(false);return 0;
 };
 w.write=[](void* p,uintptr_t address,uint32_t word) {
  auto& g=*static_cast<Game*>(p);unsigned n=0;const auto value=scalar(word);
  if(address==Mario){g.fieldMarioBattle.flags=word;g.fieldKeysOff=(word&8) || g.goombaAcquisition.active() || g.goombaReward.active();return;}
  if(address==Mario+4){g.fieldCameraDisplayFlags=word;return;}
  if(address==Mario+0x184){g.fieldMarioBody.walkBase=value;return;}
  if(address==Mario+0x188){g.fieldMarioBody.dashBase=value;return;}
  if(address==Mario+0x1b8){g.fieldMarioBody.width=value;return;}
  if(address==Mario+0x1bc){g.fieldMarioBody.height=value;return;}
  if(triple(address,Mario+0x8c,n)) {
   if(n==0)g.progress.position.x=value*.01f;
   else if(n==1)g.jumpHeight=value*.01f-g.groundHeight; // Current root only; retain actual floor/ground metadata.
   else g.progress.position.z=value*.01f;
   return;
  }
  if(triple(address,Mario+0xec,n)){lane(g.marioCamera.retained,n)=value;return;}
  if(triple(address,Mario+0xf8,n)){lane(g.marioCameraCollision,n)=value;return;}
  if(triple(address,Mario+0x2e4,n)){g.fieldRecoveryWords[n]=word;return;}
  if(address==Mario+0x2e0){g.fieldRecoveryReload=word;return;}
  if(triple(address,Camera+0x94,n)){lane(g.fieldCamera.point,n)=value;return;}
  if(triple(address,Camera+0xac,n)){lane(g.fieldCameraCollisionTarget,n)=value;return;}
  for(auto& actor:g.fieldParties.actors)if(actor) {
   const auto base=reinterpret_cast<uintptr_t>(actor.get());
   if(address>=base && address<base+actor->bytes.size()){actor->word(unsigned(address-base),word);return;}
  }
  assert(false);
 };
 w.roadReset=[](void* p){auto& g=*static_cast<Game*>(p);g.cameraRoadWork={};g.fieldCameraCandidateActive=false;g.fieldCameraCandidateShiftRate=0;};
 w.shiftReset=[](void* p){static_cast<Game*>(p)->fieldCameraShift.reset();};
 w.half=[](void* p,uintptr_t address)->uint16_t{if(address==Camera)return uint16_t(static_cast<Game*>(p)->fieldCameraFlags);assert(address==Mario+0x2e);return uint16_t(static_cast<Game*>(p)->fieldJump.motion);};return w;
}
FieldMarioPreparationWorld Game::fieldMarioPreparationWorld() {
 FieldMarioPreparationWorld w;w.storage=fieldPositionWorld();
 w.admit=[](void* p,uint32_t fn){auto& g=*static_cast<Game*>(p);
  if(fn==0x800e7474)return true;
  return (fn==0x800e73ec || fn==0x800e7410 || fn==0x800e7314) && g.fieldMarioBody.ordinary(g.fieldMarioBattle.flags|uint32_t(g.fieldIdle.mainFlags));
 };
 w.byte=[](void* p,uintptr_t a)->uint8_t{auto& g=*static_cast<Game*>(p);if(a==Mario+0x3c)return g.fieldMarioBody.form;assert(a==Mario+0x39);return g.fieldMarioBattle.keyDepth;};
 w.storeByte=[](void* p,uintptr_t a,uint8_t v){assert(a==Mario+0x39);auto& g=*static_cast<Game*>(p);g.fieldMarioBattle.keyDepth=v;};
 w.storeHalf=[](void* p,uintptr_t a,uint16_t v){assert(a==Camera);auto& g=*static_cast<Game*>(p);g.fieldCameraFlags=(g.fieldCameraFlags&0xffff0000u)|v;};
 return w;
}
FieldPreviousPartyWorld Game::fieldPreviousPartyWorld() {
 FieldPreviousPartyWorld w;w.user=this;w.mario=[](void*)->uintptr_t{return Mario;};
 w.byte=[](void* p,uintptr_t a)->uint8_t{auto& g=*static_cast<Game*>(p);
  assert(a>=Mario+0x245 && a<=Mario+0x248);const auto offset=unsigned(a-Mario-0x245);
  return offset<2?g.fieldMarioBattle.partyIndices[offset]:g.fieldMarioBattle.savedPartyKinds[offset-2];
 };
 w.storeByte=[](void* p,uintptr_t a,uint8_t v){assert(a>=Mario+0x245 && a<Mario+0x247);static_cast<Game*>(p)->fieldMarioBattle.partyIndices[unsigned(a-Mario-0x245)]=v;};
 w.party=[](void* p,int32_t index)->std::optional<uintptr_t>{if(index>=2)return {};return reinterpret_cast<uintptr_t>(static_cast<Game*>(p)->fieldParties.pointer(index));};
 // Constructor-kind0/known-index branches need no Entry. Nonzero kinds hold
 // until genuine synchronous Entry/Entry2 resources and world effects are bound.
 return w;
}
bool Game::applyFieldBeroStart() {
 if(fieldBeroStartApplied)return true;
 if(!fieldBeroStart || !fieldPartyCurrentIndex || *fieldPartyCurrentIndex>=2)return false;
 struct Context {EventEntry event;uint32_t selector=0;};Context context;
 publishFieldBeroStart(context.event,*fieldBeroStart);context.event.arguments=0x100;
 FieldPositionOperands o;o.user=&context;
 o.word=[](void* p,uint32_t address){auto& c=*static_cast<Context*>(p);
  if(address>=0x100 && address<0x10c && !(address&3))return uint32_t(-30000000+10+(address-0x100)/4);
  if(address==0x200)return c.selector;
  assert(address>=0x204 && address<0x210 && !(address&3));return uint32_t(-30000000+10+(address-0x204)/4);
 };
 // Delegate actual fetched-word decoding to the shared source operand owner.
 o.scalar=[](void* p,EventEntry& e,int32_t raw) {
  auto& c=*static_cast<Context*>(p);EventOperandServices s;s.user=&c;
  s.contextAddress=[](void*)->uint32_t{return 0;};s.entryAddress=[](void*,EventEntry&)->uint32_t{return 0x1000;};
  s.readWord=[](void* p,uint32_t a){auto& c=*static_cast<Context*>(p);assert(a>=0x109c && a<0x10dc && !(a&3));return uint32_t(c.event.locals[(a-0x109c)/4]);};
  return getEventFloat(e,raw,s);
 };
 const auto world=fieldPositionWorld();
 fieldMarioSetPositionEvent(context.event,o,world);
 context.event.arguments=0x200;fieldPartySetPositionEvent(context.event,o,world);
 context.selector=1;fieldPartySetPositionEvent(context.event,o,world);
 fieldCameraRoadResetEvent(world);
 fieldBeroStartApplied=true;return true;
}
bool Game::executeFieldBeroInitial() {
 // A copied Game may observe completion, but cannot advance a shared pending VM.
 if(fieldBeroInitial && !fieldBeroInitial->finished && fieldBeroInitial.use_count()!=1)return false;
 if(fieldBeroInitial && fieldBeroInitial->finished){fieldBeroInitialPending=false;return true;}
 if(!fieldBeroInfo.known() || !fieldBeroInfo.registered() || !fieldBeroPosition || !fieldPartyCurrentIndex || *fieldPartyCurrentIndex>=2)return false;
 if(!fieldBeroInitial) {
  auto next=std::make_shared<FieldBeroInitialEvent>();EventEntry parentBank;
  // Ordinary root constructor and untouched residual locals are source-proven;
  // selected outputs are real, not invented case IDs or checkpoint history.
  if(!fieldBeroInfo.intoInfo(parentBank) || !next->begin(fieldBeroInfo,parentBank.locals,ticks*1000/60))return false;
  fieldBeroInitial=std::move(next);
 }
 FieldBeroPositionServices geometry;geometry.user=this;
 geometry.center=[](void* p,const std::string&,FieldWallVector& out){auto& g=*static_cast<Game*>(p);if(!g.fieldBeroPosition)return false;out=g.fieldBeroPosition->center;return true;};
 geometry.floor=[](void*,FieldWallVector origin,FieldWallVector direction,float length){const auto hit=fieldShadowRay(origin,direction,length);return FieldBeroFloor{!hit.unresolvedObject,hit.found,hit.point.y};};
 geometry.sine=[](void*,float a){return float(std::sin(double(a)));};geometry.cosine=[](void*,float a){return float(std::cos(double(a)));};
 geometry.negativeMultiplySubtract=[](void*,float a,float b,float c){return -std::fma(a,b,-c);};geometry.multiplyAdd=[](void*,float a,float b,float c){return std::fma(a,b,c);};
 fieldBeroInitial->tick(ticks*1000/60,geometry,fieldPositionWorld());
 if(!fieldBeroInitial->finished)return false;
 fieldBeroPosition=fieldBeroInitial->position;fieldBeroStart=fieldBeroStartPublication(fieldBeroPosition->start);
 assert(fieldBeroStart);fieldBeroStartApplied=true;fieldBeroInitialPending=false;
 arrivalStart={fieldBeroStart->position.x*.01f,fieldBeroStart->position.z*.01f};
 arrivalEnd={fieldBeroPosition->finish.x*.01f,fieldBeroPosition->finish.z*.01f};
 roadMotion.begin({arrivalStart.x,arrivalStart.z},{arrivalEnd.x,arrivalEnd.z});return true;
}
}

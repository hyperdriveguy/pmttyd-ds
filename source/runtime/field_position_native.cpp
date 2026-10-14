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
  if(triple(address,Mario+0x8c,n))return bits(n==0?g.progress.position.x*100:n==1?(g.groundHeight+g.jumpHeight)*100:g.progress.position.z*100);
  if(triple(address,Mario+0xec,n))return bits(lane(g.marioCamera.retained,n));
  if(triple(address,Mario+0xf8,n))return bits(lane(g.marioCameraCollision,n));
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
  if(triple(address,Mario+0x8c,n)) {
   if(n==0)g.progress.position.x=value*.01f;
   else if(n==1)g.jumpHeight=value*.01f-g.groundHeight; // Current root only; retain actual floor/ground metadata.
   else g.progress.position.z=value*.01f;
   return;
  }
  if(triple(address,Mario+0xec,n)){lane(g.marioCamera.retained,n)=value;return;}
  if(triple(address,Mario+0xf8,n)){lane(g.marioCameraCollision,n)=value;return;}
  if(triple(address,Camera+0x94,n)){lane(g.fieldCamera.point,n)=value;return;}
  if(triple(address,Camera+0xac,n)){lane(g.fieldCameraCollisionTarget,n)=value;return;}
  for(auto& actor:g.fieldParties.actors)if(actor) {
   const auto base=reinterpret_cast<uintptr_t>(actor.get());
   if(address>=base && address<base+actor->bytes.size()){actor->word(unsigned(address-base),word);return;}
  }
  assert(false);
 };
 w.roadReset=[](void* p){auto& g=*static_cast<Game*>(p);g.cameraRoadWork={};g.fieldCameraCandidateActive=false;g.fieldCameraCandidateShiftRate=0;};
 w.shiftReset=[](void* p){static_cast<Game*>(p)->fieldCameraShift.reset();};return w;
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
}

#include "field_mario_preparation.hpp"
#include "event_manager.hpp"
#include <cassert>
namespace ttyd {
namespace {
int32_t signedByte(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}
void follow(bool off,const FieldMarioPreparationWorld& w) {
 const auto& s=w.storage;const auto camera=s.camera(s.user,4);assert(camera);
 const auto flags=s.half(s.user,camera);
 w.storeHalf(s.user,camera,uint16_t(off?flags|0x10:flags&~0x10));
 const auto mario=s.mario(s.user);assert(mario);const auto display=s.read(s.user,mario+4);
 s.write(s.user,mario+4,off?display|0x20000:display&~0x20000u);
}
bool admitted(uint32_t fn,const FieldMarioPreparationWorld& w){return w.admit && w.admit(w.storage.user,fn);}
}
int32_t fieldMarioKeyOff(const FieldMarioPreparationWorld& w) {
 const auto& s=w.storage;const auto mario=s.mario(s.user);assert(mario);
 const auto main=s.read(s.user,mario);s.write(s.user,mario,main|8);
 const auto depth=w.byte(s.user,mario+0x39);w.storeByte(s.user,mario+0x39,uint8_t(depth+1));
 return signedByte(w.byte(s.user,mario+0x39));
}
int32_t fieldMarioKeyOn(const FieldMarioPreparationWorld& w) {
 const auto& s=w.storage;const auto mario=s.mario(s.user);assert(mario);
 const auto depth=w.byte(s.user,mario+0x39);w.storeByte(s.user,mario+0x39,uint8_t(depth-1));
 if(signedByte(w.byte(s.user,mario+0x39))==0){const auto main=s.read(s.user,mario);s.write(s.user,mario,main&~8u);}
 return signedByte(w.byte(s.user,mario+0x39));
}
int32_t fieldMarioKeyEvent(EventEntry& e,const FieldMarioPreparationOperands& o,const FieldMarioPreparationWorld& w) {
 if(!admitted(0x800e7474,w))return 0;
 assert(o.word && o.value && w.storage.mario && w.storage.read && w.storage.write && w.byte && w.storeByte);
 const auto args=e.arguments;w.storage.mario(w.storage.user); // Actual wrapper lookup, unused result.
 const auto raw=o.word(o.user,args);const auto on=o.value(o.user,e,int32_t(raw));
 if(on)fieldMarioKeyOn(w);else fieldMarioKeyOff(w);
 // Actual partyKeyOn/Off are return-only US bodies, with no actor lookup.
 return 2;
}
int32_t fieldMarioBackgroundEvent(bool enabled,const FieldMarioPreparationWorld& w) {
 if(!admitted(enabled?0x800e7410:0x800e73ec,w))return 0;
 const auto& s=w.storage;assert(s.mario && s.read && s.write && s.camera && s.half && w.byte && w.storeHalf);
 const auto captured=s.mario(s.user);assert(captured);const auto main=s.read(s.user,captured);
 s.write(s.user,captured,enabled?main|0x2000000:main&~0x2000000u);
 const auto mario=s.mario(s.user);assert(mario);const auto form=signedByte(w.byte(s.user,mario+0x3c));
 if(form==1) {
  s.write(s.user,mario+0x1b8,0x41a00000);s.write(s.user,mario+0x1bc,0x423c0000);
  s.write(s.user,mario+0x184,0x3f800000);s.write(s.user,mario+0x188,0x40000000);
 } else {
  if(form!=2){const bool small=(s.read(s.user,mario)&0x4000000)!=0;
   s.write(s.user,mario+0x1b8,small?0x41200000:0x41a00000);s.write(s.user,mario+0x1bc,small?0x41cf3333:0x42140000);
  }
  s.write(s.user,mario+0x184,0x3f800000);s.write(s.user,mario+0x188,0x40100000);
 }
 follow(enabled,w);return 2;
}
int32_t fieldMarioCameraYOffEvent(const FieldMarioPreparationWorld& w) {
 if(!admitted(0x800e7314,w))return 0;
 const auto& s=w.storage;assert(s.camera && s.half && s.mario && s.read && s.write && w.storeHalf);follow(true,w);return 2;
}
}

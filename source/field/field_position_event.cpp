#include "field_position_event.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float value){uint32_t out;std::memcpy(&out,&value,4);return out;}
float scalar(uint32_t value){float out;std::memcpy(&out,&value,4);return out;}
}
int32_t fieldMarioSetPositionEvent(EventEntry& e,const FieldPositionOperands& o,const FieldPositionWorld& w) {
 assert(o.word && o.scalar && w.mario && w.write);
 const auto args=e.arguments;const auto mario=w.mario(w.user);assert(mario);
 for(unsigned n=0;n<3;++n){const auto raw=int32_t(o.word(o.user,args+4*n));
  const auto value=o.scalar(o.user,e,raw);w.write(w.user,mario+0x8c+4*n,bits(value));}
 return 2;
}
int32_t fieldPartySetPositionEvent(EventEntry& e,const FieldPositionOperands& o,const FieldPositionWorld& w) {
 assert(o.word && w.party);
 const auto args=e.arguments;const auto selector=o.word(o.user,args);
 int32_t index=1;if(!selector){assert(w.currentParty);index=w.currentParty(w.user);}
 const auto party=w.party(w.user,index);if(!party)return 2;
 assert(o.scalar && w.write);
 for(unsigned n=0;n<3;++n){const auto raw=int32_t(o.word(o.user,args+4+4*n));
  const auto value=o.scalar(o.user,e,raw);w.write(w.user,party+0x58+4*n,bits(value));}
 return 2;
}
int32_t fieldCameraRoadResetEvent(const FieldPositionWorld& w) {
 assert(w.mario && w.camera && w.read && w.write && w.roadReset && w.shiftReset);
 const auto mario=w.mario(w.user),camera=w.camera(w.user,4);assert(mario && camera);
 w.write(w.user,camera+0x94,w.read(w.user,mario+0x8c));
 volatile float y=.009999999776482582f+scalar(w.read(w.user,mario+0x90));
 w.write(w.user,camera+0x98,bits(y));
 w.write(w.user,camera+0x9c,w.read(w.user,mario+0x94));
 for(unsigned n=0;n<3;++n)w.write(w.user,camera+0xac+4*n,w.read(w.user,camera+0x94+4*n));
 for(const auto offset:{0xecu,0xf8u}) {
  const auto x=w.read(w.user,mario+0x8c),yword=w.read(w.user,mario+0x90);
  w.write(w.user,mario+offset,x);w.write(w.user,mario+offset+4,yword);
  w.write(w.user,mario+offset+8,w.read(w.user,mario+0x94));
 }
 w.roadReset(w.user);w.shiftReset(w.user);return 2;
}
}

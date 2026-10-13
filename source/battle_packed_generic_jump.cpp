#include "battle_packed_generic_jump.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const EventMemoryServices& s,uint32_t a){return s.word(s.user,a);}void word(const EventMemoryServices& s,uint32_t a,uint32_t v){s.storeWord(s.user,a,v);}
float real(const EventMemoryServices& s,uint32_t a){const auto b=word(s,a);float v;std::memcpy(&v,&b,4);return v;}void real(const EventMemoryServices& s,uint32_t a,float v){uint32_t b;std::memcpy(&b,&v,4);word(s,a,b);}
std::array<float,3> point(const EventMemoryServices& s,uint32_t a){return {real(s,a),real(s,a+4),real(s,a+8)};}
void visible(const EventMemoryServices& s,uint32_t u,uint32_t p){for(unsigned i=0;i<3;++i)word(s,u+0x3c+4*i,word(s,p+4*i));}
void integrate(uint32_t u,const BattlePackedGenericJumpServices& s){const auto& c=s.movement.query.common;const auto& p=c.position;const auto& m=p.unit.memory;real(m,u+0x158,p.add(p.numericUser,real(m,u+0x158),real(m,u+0x178)));real(m,u+0x178,c.subtract(p.numericUser,real(m,u+0x178),real(m,u+0x174)));}
}
int32_t packedGenericJumpPositionEvent(EventEntry& e,int32_t first,const BattlePackedGenericJumpServices& s){
 const auto& c=s.movement.query.common;const auto& p=c.position;const auto& u=p.unit;const auto& m=u.memory;const auto args=e.arguments;
 auto value=[&](unsigned i){return u.values.resolve(u.values.user,e,int32_t(word(m,args+4*i)));};
 const auto selector=value(0),id=u.translate(u.user,e,selector);const float x=c.convert(p.numericUser,value(1)),y=c.convert(p.numericUser,value(2)),z=c.convert(p.numericUser,value(3));const auto count=value(4),mode=value(5);const auto battle=word(m,0x8041eb90),actor=u.unit(u.user,battle,id);
 if(first){e.initialized=1;if(count)word(m,actor+0x16c,uint32_t(count));setPackedMovePosition(actor,true,{x,y,z},m);const auto start=point(m,actor+0x3c);setPackedMovePosition(actor,false,start,m);
  real(m,actor+0x17c,s.movement.heading(s.movement.query.user,start[0],start[2],x,z));real(m,actor+0x180,s.movement.query.distance(s.movement.query.user,start[0],start[2],x,z));
  const auto dx=c.subtract(p.numericUser,x,start[0]);if(dx>0 || dx<0)m.storeByte(m.user,actor+0x188,dx>0?1:255);const auto dy=c.subtract(p.numericUser,y,start[1]);
  if(word(m,actor+0x16c)==0)word(m,actor+0x16c,uint32_t(u.truncateFloat(u.user,c.divide(p.numericUser,real(m,actor+0x180),real(m,actor+0x170)))));
  else real(m,actor+0x170,c.divide(p.numericUser,real(m,actor+0x180),c.convert(p.numericUser,int32_t(word(m,actor+0x16c)))));
  const auto n=c.convert(p.numericUser,int32_t(word(m,actor+0x16c)));real(m,actor+0x180,c.negativeMultiplySubtract(p.numericUser,n,real(m,actor+0x170),real(m,actor+0x180)));
  if(word(m,actor+0x16c)==0)return 2;
  real(m,actor+0x178,p.fma(p.numericUser,p.multiply(p.numericUser,real(m,actor+0x174),n),real(m,0x804222a0),c.divide(p.numericUser,dy,n)));
  real(m,actor+0x170,p.add(p.numericUser,real(m,actor+0x170),c.divide(p.numericUser,real(m,actor+0x180),n)));
  loadPackedUnitSoundMode(2,mode,actor+0x1c4,actor+0x18c,m);const auto key=word(m,actor+0x18c);if(key){assert(s.play);s.play(s.user,actor,key,point(m,actor+0x154));}
 }
 integrate(actor,s);if(real(m,actor+0x178)<0 && real(m,actor+0x158)<real(m,actor+0x164))real(m,actor+0x158,real(m,actor+0x164));
 integratePackedMoveHorizontal(actor+0x154,actor+0x15c,real(m,actor+0x170),real(m,actor+0x17c),s.movement);visible(m,actor,actor+0x154);word(m,actor+0x16c,word(m,actor+0x16c)-1);
 if(int32_t(word(m,actor+0x16c))>0)return 0;
 visible(m,actor,actor+0x160);const auto key=word(m,actor+0x190);if(key){assert(s.play);s.play(s.user,actor,key,point(m,actor+0x160));}return 1;
}
int32_t packedGenericJumpContinueEvent(EventEntry& e,int32_t first,const BattlePackedGenericJumpServices& s){
 const auto& c=s.movement.query.common;const auto& u=c.position.unit;const auto& m=u.memory;const auto selector=u.values.resolve(u.values.user,e,int32_t(word(m,e.arguments)));const auto id=u.translate(u.user,e,selector);const auto b=word(m,0x8041eb90),actor=u.unit(u.user,b,id);
 if(first){e.initialized=1;setPackedMovePosition(actor,false,point(m,actor+0x3c),m);}integrate(actor,s);integratePackedMoveHorizontal(actor+0x154,actor+0x15c,real(m,actor+0x170),real(m,actor+0x17c),s.movement);visible(m,actor,actor+0x154);
 const auto position=point(m,actor+0x154);assert(s.floor);const auto floor=s.floor(s.user,b,position[0],position[1],position[2]);if(position[1]<floor){real(m,actor+0x158,floor);visible(m,actor,actor+0x154);return 1;}return 0;
}
}

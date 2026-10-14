#include "battle_camera_requests.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
uint32_t word(const BattleCameraRequestServices& s,uint32_t a){return s.positions.memory.memory.word(s.positions.memory.memory.user,a);}
void word(const BattleCameraRequestServices& s,uint32_t a,uint32_t v){s.positions.memory.memory.storeWord(s.positions.memory.memory.user,a,v);}
int32_t raw(const BattleCameraRequestServices& s,EventEntry& e,unsigned i){return int32_t(word(s,e.arguments+4*i));}
int32_t value(const BattleCameraRequestServices& s,EventEntry& e,unsigned i){assert(s.values.resolve);return s.values.resolve(s.values.user,e,raw(s,e,i));}
uint32_t battle(const BattleCameraRequestServices& s){return word(s,0x8041eb90);}
void real(const BattleCameraRequestServices& s,uint32_t a,float v){uint32_t b;std::memcpy(&b,&v,4);word(s,a,b);}
void half(const BattleCameraRequestServices& s,uint32_t a,uint16_t v){s.positions.memory.memory.storeHalf(s.positions.memory.memory.user,a,v);}
bool admitted(const BattleCameraRequestServices& s,uint32_t b,int32_t priority){return priority>=int32_t(word(s,b+0x275c));}
}
int32_t battleCameraShakeHorizontalEvent(EventEntry& e,const BattleCameraRequestServices& s){
 const auto args=e.arguments;assert(s.values.resolve && s.signedWordFloat);
 auto resolve=[&](uint32_t token){return s.values.resolve(s.values.user,e,int32_t(token));};
 const auto priority=resolve(word(s,args));
 const auto x=resolve(word(s,args+4));const auto rawY=word(s,args+8);
 const auto amplitudeX=s.signedWordFloat(s.user,x);
 const auto y=resolve(rawY);const auto rawDuration=word(s,args+12);
 const auto amplitudeY=s.signedWordFloat(s.user,y);
 const auto duration=resolve(rawDuration),cycles=resolve(word(s,args+16));
 const auto b=battle(s);if(!admitted(s,b,priority))return 2;
 word(s,b+0x2754,word(s,b+0x2754)|2);
 real(s,b+0x2848,amplitudeX);real(s,b+0x284c,amplitudeY);
 word(s,b+0x282c,uint32_t(cycles));word(s,b+0x2834,uint32_t(duration));word(s,b+0x283c,uint32_t(duration));
 if(!(word(s,b+0x2754)&0x40)){assert(s.rumbleOn);s.rumbleOn(s.user,0);}
 return 2;
}
int32_t battleCameraModeEvent(EventEntry& e,const BattleCameraRequestServices& s){const auto priority=value(s,e,0),mode=value(s,e,1);const auto b=battle(s);if(!admitted(s,b,priority))return 2;
 word(s,b+0x2760,uint32_t(mode));real(s,b+0x2778,0);half(s,b+0x277c,1);half(s,b+0x277e,1);word(s,b+0x2758,0);
 // Each source internal axis request has its own priority0 admission gate.
 if(admitted(s,b,0))for(uint32_t mask:{4u,8u,16u})word(s,b+0x2754,word(s,b+0x2754)&~mask);
 word(s,b+0x2754,word(s,b+0x2754)&~0x20u);return 2;
}
int32_t battleCameraSpeedEvent(EventEntry& e,const BattleCameraRequestServices& s){const auto priority=value(s,e,0),level=value(s,e,1);const auto b=battle(s);if(!admitted(s,b,priority))return 2;
 half(s,b+0x277c,uint16_t(level>=0 && level<=5?level:1));const auto& m=s.positions.memory.memory;const auto selected=m.half(m.user,b+0x277c);half(s,b+0x277e,selected);return 2;
}
int32_t battleCameraZoomEvent(EventEntry& e,const BattleCameraRequestServices& s){const auto priority=value(s,e,0);assert(s.values.resolveFloat && s.positions.unit.truncateFloat);const auto zoom=s.values.resolveFloat(s.values.user,e,raw(s,e,1));const auto integer=s.positions.unit.truncateFloat(s.positions.unit.user,zoom);const auto b=battle(s);
 if(admitted(s,b,priority))real(s,b+0x2778,float(integer));
 return 2;
}
int32_t battleCameraHomingPartsEvent(EventEntry& e,const BattleCameraRequestServices& s){
 const auto priority=value(s,e,0),owner=value(s,e,1),ownerPart=value(s,e,2),target=value(s,e,3),targetPart=value(s,e,4);
 const auto b=battle(s);if(!admitted(s,b,priority))return 2;
 const auto& u=s.positions.unit;assert(u.translate && u.unit);
 const auto first=u.translate(u.user,e,owner);word(s,b+0x2764,uint32_t(first));word(s,b+0x276c,uint32_t(ownerPart));
 const auto actor=u.unit(u.user,b,int32_t(word(s,b+0x2764)));
 (void)packedRegistryPart(int32_t(word(s,b+0x2764)),int32_t(word(s,b+0x276c)),s.positions.memory);
 // GetHomePos writes stack locals before the camera vector publication.
 const std::array<uint32_t,3> home{word(s,actor+0x30),word(s,actor+0x34),word(s,actor+0x38)};
 for(unsigned axis=0;axis<3;++axis)word(s,b+0x2810+4*axis,home[axis]);
 if(target==-1)return 2;
 const auto second=u.translate(u.user,e,target);word(s,b+0x2768,uint32_t(second));word(s,b+0x2770,uint32_t(targetPart));
 const auto other=u.unit(u.user,b,int32_t(word(s,b+0x2768)));
 const auto part=packedRegistryPart(int32_t(word(s,b+0x2768)),int32_t(word(s,b+0x2770)),s.positions.memory);
 const auto hit=packedHitPosition(other,part,s.positions);
 for(unsigned axis=0;axis<3;++axis)real(s,b+0x281c+4*axis,hit[axis]);
 return 2;
}
int32_t battleCameraHomingEvent(EventEntry& e,const BattleCameraRequestServices& s){(void)value(s,e,0);const auto owner=value(s,e,1),target=value(s,e,2);const auto b=battle(s);const auto& u=s.positions.unit;assert(u.translate && u.unit);
 const auto first=u.translate(u.user,e,owner);word(s,b+0x2764,uint32_t(first));const auto actor=u.unit(u.user,b,first);const auto body=packedBodyPartId(actor,u.memory);word(s,b+0x276c,uint32_t(body));(void)packedUnitPart(actor,body,u.memory);
 for(unsigned axis=0;axis<3;++axis)word(s,b+0x2810+4*axis,word(s,actor+0x30+4*axis));
 if(target==-1)return 2;
 const auto second=u.translate(u.user,e,target);word(s,b+0x2768,uint32_t(second));const auto other=u.unit(u.user,b,second);const auto otherBody=packedBodyPartId(other,u.memory);word(s,b+0x2770,uint32_t(otherBody));const auto part=packedUnitPart(other,otherBody,u.memory);
 const auto hit=packedHitPosition(other,part,s.positions);for(unsigned axis=0;axis<3;++axis)real(s,b+0x281c+4*axis,hit[axis]);return 2;
}
}

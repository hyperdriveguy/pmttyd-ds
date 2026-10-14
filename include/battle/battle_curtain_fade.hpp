#pragma once
#include <cstdint>
namespace ttyd {
// Bounded original fadeEntry23/fadeMain branch. Pose time and asset admission
// belong to the actual presentation provider, never the raw duration field.
struct BattleCurtainPoseServices {
 void* user=nullptr;
 bool (*ready)(void*,const char*,int32_t)=nullptr;
 int32_t (*entry)(void*,const char*,int32_t)=nullptr;
 void (*release)(void*,int32_t)=nullptr;
 void (*flagsOn)(void*,int32_t,uint32_t)=nullptr;
 void (*flagsOff)(void*,int32_t,uint32_t)=nullptr;
 void (*animation)(void*,int32_t,const char*,int32_t)=nullptr;
 void (*color)(void*,int32_t,uint32_t)=nullptr;
 float (*loopTimes)(void*,int32_t)=nullptr;
 void (*main)(void*,int32_t)=nullptr;
};
class BattleCurtainFade {
public:
 bool enter(int32_t type,int32_t duration,uint32_t color);
 void tick(const BattleCurtainPoseServices&);
 bool finished() const{return !active || (flags&2);}
 bool active=false;
 uint16_t flags=0;
 int32_t type=0,rawDuration=0,step=0,pose=-1;
 uint32_t entryColor=0,poseColor=0xffffffff;
};
}

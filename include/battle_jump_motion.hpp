#pragma once
#include <array>
#include <cstdint>

namespace ttyd {
struct JumpVector {float x=0,y=0,z=0;};
// Separate semantic unit fields. Visible position never supplies the next
// baseline implicitly. Numeric/audio/VM services retain their own ownership.
struct MarioJumpUnit {
    JumpVector visible{},baseline{},target{},delta{};
    int32_t remaining=0;
    float speed=0,phaseIncrement=0,phase=0,heading=0,distance=0,height=0;
    float angle=0,angleStep=0,lastY=0;
    int8_t direction=0;
    std::array<uint8_t,0x36> soundSource{};
    std::array<uint8_t,0x1c> soundMode{};
};
struct MarioJumpEvent {int8_t phase=0;};
// Coordinates are the immediately converted integer evtGetValue results.
struct MarioJumpArguments {
    int32_t actor=0;
    float x=0,y=0,z=0;
    int32_t count=0,mode=0,soundMode=-1;
};
struct JumpMotionServices {
    void* user=nullptr;
    float (*heading)(void*,float,float,float,float)=nullptr;
    float (*distance)(void*,float,float,float,float)=nullptr;
    double (*sin)(void*,double)=nullptr;
    double (*fmod)(void*,double,double)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr; // Original fctiwz, including exceptional input.
    bool (*eligible)(void*,MarioJumpUnit&)=nullptr;
    JumpVector (*getPosition)(void*,MarioJumpUnit&)=nullptr;
    void (*setPosition)(void*,MarioJumpUnit&,JumpVector)=nullptr;
    void (*playSound)(void*,uint32_t,JumpVector)=nullptr;
};
// Source/destination may overlap; ordered partial writes are intentional.
void loadUnitSoundMode(int32_t type,int32_t mode,const uint8_t* source,uint8_t* destination);
float reviseJumpAngle(float,const JumpMotionServices&);
// Results are original event results0(block),1(advance/yield),2(advance).
int marioJumpMotion(MarioJumpUnit&,MarioJumpEvent&,const MarioJumpArguments&,
                    bool firstCall,const JumpMotionServices&);
struct JumpEventServices {
    void* user=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translateActor)(void*,int32_t)=nullptr;
    MarioJumpUnit* (*unit)(void*,int32_t)=nullptr;
    void (*store)(void*,int32_t destination,int32_t value)=nullptr;
};
// Re-resolves all seven raw operands on every invocation, including resumptions.
int marioJumpMotionEvent(MarioJumpEvent&,const std::array<int32_t,7>& operands,
                         bool firstCall,const JumpEventServices&,const JumpMotionServices&);
int32_t marioJumpCount(MarioJumpUnit&,float x,float z,int32_t requested,
                       const JumpMotionServices&);
int marioJumpParameterEvent(const std::array<int32_t,5>& operands,
                            const JumpEventServices&,const JumpMotionServices&);
}

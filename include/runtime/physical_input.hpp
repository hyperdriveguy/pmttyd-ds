#pragma once
#include "battle_defense.hpp"
namespace ttyd {
struct RawPad {
    uint16_t buttons=0;
    int8_t mainX=0,mainY=0,subX=0,subY=0;
    uint8_t leftTrigger=0,rightTrigger=0,analogA=0,analogB=0;
    int8_t error=0;uint8_t padding=0;
};
static_assert(sizeof(RawPad)==12,"Raw record has twelve semantic bytes");
struct DemoPad {
    RawPad raw;
    uint16_t pressed=0,released=0,direction=0,directionPressed=0,directionReleased=0;
    std::array<int16_t,4> delta{};
    void copy(const RawPad&);
};
struct GlobalInputChannel {
    uint32_t buttons=0,pressed=0,repeat=0,released=0,direction=0,directionPressed=0,directionRepeat=0;
    int32_t buttonCountdown=0,directionCountdown=0;
    BattlePadSnapshot analog;
    uint8_t motorMode=0,previousMotorMode=0,forceMotor=0;
    BattlePadSample battleSample() const;
};
struct InputHardwareRequests {
    uint32_t resetMask=0;
    std::array<int,4> motors{{-1,-1,-1,-1}};
};
struct InputHardwareServices {
    void* user=nullptr;
    void (*reset)(void*,uint32_t)=nullptr;
    void (*motor)(void*,int,int)=nullptr;
};
struct PhysicalInputProducer {
    std::array<DemoPad,4> demo;
    std::array<GlobalInputChannel,4> channels;
    uint32_t countedChannels=0,ready=0;
    // Caller supplies PADRead/PADClamp results. No SDK/hardware behavior is fabricated.
    InputHardwareRequests update(const std::array<RawPad,4>& clamped,int32_t frameRate,bool motorsEnabled,InputHardwareServices hardware={});
};
}

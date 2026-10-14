#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
struct PositionalSfxSlot {
    int32_t key=0;
    uint16_t generation=0,flags=0;
    uint8_t volume=0;
    uint8_t rowVolume=0,pan=0,rowMode=0;
    uint16_t priority=0,delay=0,retriggerDelay=0,variation=0;
    uint16_t distance=0,pitch=0;
    int32_t engine=0;
    std::array<float,3> position{};
};
enum class PositionalSfxStopKind {Stream,Song,Effect};
struct PositionalSfxStopServices {
    void* user=nullptr;
    std::optional<uint32_t> (*descriptor)(void*,uint32_t keyIndex)=nullptr;
    bool (*stop)(void*,PositionalSfxStopKind,int32_t engine)=nullptr;
};
// Retain this owner across audience/battle lifetimes. The typed representation
// is not a serialized GameCube pointer/record ABI. On publishes original pending slots;
// Main/backend provisioning is separate from allocation handles.
struct PositionalSfxRegistry {
    std::array<PositionalSfxSlot,40> slots{};
    uint16_t allocationGeneration=0;
    uint16_t retriggerOffset=0,globalFlags=0;
    int8_t cooldown=0; // Source G5c; decrement belongs to psndMain.
    PositionalSfxRegistry(){initializePublicSlots();}
    // Only source psndInit's public-bank writes, after backend SoundInit.
    void initializePublicSlots();
    // nullopt / false denotes the original out-of-bank address domain or an
    // unavailable backend, never an invented source invalid-handle result.
    std::optional<int32_t> check(int32_t handle)const;
    std::optional<uint8_t> getVolume(int32_t handle)const;
    bool setVolume(int32_t handle,int32_t value);
    bool setDistance(int32_t handle,int32_t value);
    bool off(int32_t handle,const PositionalSfxStopServices&);
};
}

#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
// Used evt_npc_jump_position_nohit: integer target/time/height operands,
// absolute native simulation-ms bridge to original entry timestamp delta.
class OpeningHarborJump {
public:
    using Point=std::array<float,3>;
    Point position{},start{},target{};
    uint32_t flags=0,savedGravityFlag=0;
    int32_t duration=0,result=0;
    float coefficient=0,linear=0;
    bool active=false,failed=false;
    bool begin(Point from,std::array<int32_t,3> to,int32_t milliseconds,int32_t height,uint32_t originalFlags=0);
    int32_t advance(uint32_t elapsedMilliseconds);
};
}

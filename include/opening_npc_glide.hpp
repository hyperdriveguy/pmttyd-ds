#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
// Used evt_npc_glide_position domain: duration0, speed80, height-30,
// selector11, flags0. Float elapsed/delta inputs belong to npcMain's clock;
// this owner has no rendering clock, collision shortcut or endpoint snap.
class OpeningNpcGlide {
public:
    using Point=std::array<float,3>;
    Point start{},target{},position{},pending{};
    float duration=0,elapsed=0,delta=0,height=-30,speed=80;
    uint32_t sourceCommand=0;
    int32_t result=0;
    bool active=false,failed=false;
    bool begin(Point from,Point to,uint32_t sourceCommand);
    // Callback result0 retains invocation; result1 clears/yields. The native
    // house consumes pending movement through its bounded NPC presentation.
    int32_t step(float elapsedSeconds,float deltaSeconds);
    static float curve11(float fraction);
};
}

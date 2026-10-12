#pragma once
#include <array>
#include <cmath>
namespace ttyd {
// Ordinary road evt_mario_mov_pos2:150 engine units/s, NTSC60Hz,
// native engine-unit scale0.01; tick count truncates(distance+1)/speed.
struct RoadMotion {
    std::array<float,2> position{},target{},step{};
    int remaining=0;
    bool active() const {return remaining>0;}
    bool begin(std::array<float,2> start,std::array<float,2> end) {
        for(float value:{start[0],start[1],end[0],end[1]})if(!std::isfinite(value))return false;
        float x=end[0]-start[0],z=end[1]-start[1],d=std::hypot(x,z);
        if(!std::isfinite(d) || d>8192)return false;
        position=start;target=end;
        remaining=int((d+.01f)/.025f);step=d>0?std::array<float,2>{x*.025f/d,z*.025f/d}:std::array<float,2>{};
        if(remaining==0)position=target;
        return true;
    }
    bool tick() {
        if(remaining>0) {position[0]+=step[0];position[1]+=step[1];if(--remaining==0)position=target;}
        return remaining==0;
    }
};
}

#pragma once
#include <array>
#include <cstdint>
#include <vector>
namespace ttyd {
struct UVBinding {uint32_t vertex;float u,v;};
struct UVTrack {
    std::vector<std::array<float,6>> frames;
    std::vector<UVBinding> bindings;
};
bool loadUVAnimation(const char* path,uint32_t vertices,std::vector<UVTrack>& result);
std::array<float,2> animatedUV(const UVTrack&,const UVBinding&,uint64_t ticks);
}

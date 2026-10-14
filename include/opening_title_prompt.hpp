#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
// Original pressStartGX identity + translateS=counter/-200 on texture6.
inline std::array<float,2> titlePromptPatternPoint(uint32_t counter,float u,float v){return {{u+float(counter)/-200.f,v}};}
// Original unit quad scaled by US mask288x32 and centered at source0,-122.
inline std::array<float,2> titlePromptSourcePoint(float u,float v){return {{(u-.5f)*288,-122+(0.5f-v)*32}};}
}

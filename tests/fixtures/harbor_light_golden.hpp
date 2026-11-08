#pragma once
#include <array>
#include <cstdint>
// Source-derived US m/gor_00/d golden, not runtime snapshots. Original map SHA:
// d942568687b82ede86c7ea5aab0a3aea6c0df23b538257fde8ae1632bd13089b.
// These assert the original reached scale-by10/Entry ABI, before map animation.
namespace harbor_light_fixture {
struct Golden {const char* name;std::array<uint32_t,3> position;uint32_t color,scalar1;};
constexpr std::array<Golden,5> Lights{{
 {"ambientLight1",{0x437f139du,0x41804869u,0x43bfe738u},0x7394ffffu,0x3f400000u},
 {"pointLight1",{0x43eda949u,0x42cccbdeu,0xc315c2f3u},0x335bffu,0x3f800000u},
 {"pointLight2",{0x428f01b2u,0x42449198u,0xc2f2adc8u},0x335bffu,0x3f800000u},
 {"pointLight3",{0x43cc4cd2u,0x42e91e18u,0x43a64bb2u},0x335bffu,0x3f800000u},
 {"pointLight4",{0x428f01b2u,0x42449198u,0x435a4048u},0x335bffu,0x3f800000u},
}};
constexpr uint32_t InputBase=0x89000000,TableFileOffset=0xe0d9c;
constexpr uint32_t Table=InputBase+TableFileOffset,MapEntry=0x88000000;
}

#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
inline constexpr std::array<uint32_t,33> gameOuterMainCalls{{
    0x80005ff8,0x8002e228,0x800b2508,0x8000a274,0x8002db8c,0x8004281c,
    0x800f904c,0x8000f704,0x8007f9ec,0x80026bb8,0x80018264,0x800368d0,
    0x80024b08,0x8005a7e0,0x8005f968,0x80049a7c,0x80012058,0x800714c0,
    0x8001c028,0x800730ac,0x800adfe0,0x8006e5ec,0x8001b2d8,0x800d67cc,
    0x800dc48c,0x8017df24,0x8013d654,0x8019dad0,0x80200af0,0x8020940c,
    0x8023f1b0,0x8005ea7c,0x8006edbc
}};
struct GameOuterMainServices {
    void* user=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
    bool (*store)(void*,uint32_t,uint32_t)=nullptr;
    bool (*request)(void*,uint32_t,uint32_t& pending)=nullptr;
    std::optional<uint64_t> (*time)(void*)=nullptr;
    std::optional<int64_t> (*divide)(void*,int64_t,uint32_t)=nullptr;
    std::optional<int32_t> (*sequence)(void*)=nullptr;
};
struct GameOuterMainCursor {
    bool active=false;
    uint32_t phase=0,index=0,gp=0,pending=0,channel=0,field=0,low=0,clock=0,high=0,flags=0;
    uint64_t time=0;
    int64_t quotient=0;
};
// Complete marioStMain8006f904 parent. Children retain their own cursors.
bool updateGameOuterMain(const GameOuterMainServices&,GameOuterMainCursor&);
}

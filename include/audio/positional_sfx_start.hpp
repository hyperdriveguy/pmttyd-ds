#pragma once
#include "positional_sfx_registry.hpp"
#include <array>
namespace ttyd {
struct PositionalSfxRow {
    // Exact original20-byte immutable table row, including name/descriptor words.
    std::array<uint8_t,20> bytes{};
    uint32_t word(unsigned offset)const;
    uint16_t half(unsigned offset)const;
};
struct PositionalSfxStartRequest {
    int32_t key=0,volume=255,pan=255,delay=0;
    std::optional<std::array<float,3>> position;
    int32_t positionMode=0,distance=16,muteOverride=0;
};
struct PositionalSfxStartServices {
    void* user=nullptr;
    std::optional<PositionalSfxRow> (*row)(void*,uint32_t index)=nullptr;
    std::optional<int32_t> (*compareName)(void*,uint32_t originalName,uint32_t requestedName)=nullptr;
    std::optional<int32_t> (*story)(void*)=nullptr;
    std::optional<int32_t> (*rate)(void*)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    float milliseconds=1000; // Source804218cc; caller supplies its verified data value.
    PositionalSfxStopServices stop;
};
// Whole __psndSFXOn local body: returns an original allocated PENDING slot handle,
// not a backend voice. Missing providers retain prior publications and return nullopt.
std::optional<int32_t> startPositionalSfx(PositionalSfxRegistry&,const PositionalSfxStartRequest&,const PositionalSfxStartServices&);
}

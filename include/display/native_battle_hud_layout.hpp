#pragma once
#include "native_battle_status.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
namespace ttyd {
// Immediate native presentation input, captured by the existing real owners.
// Negative values remain unavailable; the layout never produces resource data.
struct NativeBattleHudReadings {
 int hp=-1,maxHp=-1,fp=-1,maxFp=-1,coins=-1,starPoints=-1;
 const NativeBattleStatusSnapshot* sourceStatus=nullptr;
};
struct NativeBattleHudIconDraw {
 uint32_t icon=0;
 float centerX=0,bottomY=0,sourceScale=1;
};
constexpr unsigned NativeBattleHudDecimalDigits=6;
// Three resource pairs, optional AP marker/gauge, and two pouch counters.
constexpr std::size_t NativeBattleHudMaximumIcons=
 3*(3+2*NativeBattleHudDecimalDigits)+(2+9)+
 (1+2*(2+NativeBattleHudDecimalDigits));
struct NativeBattleHudPlan {
 std::array<NativeBattleHudIconDraw,NativeBattleHudMaximumIcons> icons{};
 std::size_t count=0;
};
// messageIconImage already performs source->native half scaling and accepts a
// BOTTOM anchor. sourceScale1 therefore preserves original table scale1.
// No source status-window callback, slide/blink or GPU completion is implied.
std::optional<NativeBattleHudPlan> planNativeBattleHud(const NativeBattleHudReadings&);
}

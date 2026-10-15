#pragma once
#include "battle_actor.hpp"
// Window work A+0x1375c may capture another A than the count owner. Stores
// retain separate binary32 operations. The15-step source timer is not measured
// frame timing, and draw presentation is a separate consumer.
namespace ttyd {
// WinCtrlProcess801a4570; uses the original live count, timer and constants.
void packedBattleAudienceWindowMain(const BattleActorServices&);
}

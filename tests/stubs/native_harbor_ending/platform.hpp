#pragma once
#include "game.hpp"
#include "native_harbor_reward_presentation.hpp"
#include "harbor_battle_music_owner.hpp"
namespace ttyd {
// TEST actual-backend boundary only; no hardware/PCM/GPU claim.
class Audio {
public:
 unsigned battles=0,victories=0,retired=0,updates=0;bool holdVictory=true,holdBattle=false;
 bool beginHarborBattleMusic(const HarborBattleMusicBinding&b,bool){assert(b.current(b.user));if(holdBattle){holdBattle=false;return false;}++battles;return true;}
 bool beginHarborVictoryMusic(const HarborBattleMusicBinding&b,bool){assert(b.current(b.user));if(holdVictory){holdVictory=false;return false;}++victories;return true;}
 bool updateHarborBattleMusic(const std::shared_ptr<const void>&,bool){++updates;return true;}
 bool retireHarborBattleAudio(const std::shared_ptr<const void>&r){assert(r);++retired;return true;}
};
class Renderer {
public:
 std::shared_ptr<NativeHarborRewardPresentation>model;
 std::shared_ptr<const void>runtime;
 bool available=false;
 std::shared_ptr<NativeHarborRewardPresentation>nativeHarborRewardPresentation(const std::shared_ptr<const void>&r)const{return available&&r==runtime?model:nullptr;}
};
}

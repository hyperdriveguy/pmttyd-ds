#pragma once
#include "battle_packed_move_position.hpp"
namespace ttyd {
struct BattlePackedMarioJumpServices {
    BattlePackedMovePositionServices motion;
    void* user=nullptr;
    int32_t (*eligible)(void*,uint32_t unit)=nullptr;
    uint32_t (*play)(void*,uint32_t key,const std::array<float,3>&)=nullptr;
    int32_t (*divideWord)(void*,int32_t,int32_t)=nullptr; // motion numericUser
    double (*add)(void*,double,double)=nullptr;
    double (*subtract)(void*,double,double)=nullptr;
    double (*multiply)(void*,double,double)=nullptr;
    double (*divide)(void*,double,double)=nullptr;
    double (*fma)(void*,double,double,double)=nullptr;
    double (*fmod)(void*,double,double)=nullptr;
};
int32_t packedACPossibility(uint32_t unit,const BattleActorServices&);
float revisePackedJumpAngle(float,const BattlePackedMarioJumpServices&);
int32_t packedMarioJumpPositionEvent(EventEntry&,int32_t firstCall,const BattlePackedMarioJumpServices&);
}

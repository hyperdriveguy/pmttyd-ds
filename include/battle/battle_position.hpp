#pragma once
#include "battle_jump_motion.hpp"

namespace ttyd {
struct BattlePositionPart;
// Views let motion/status/resource owners provide their actual shared fields.
struct BattlePositionUnit {
    JumpVector* home=nullptr;
    JumpVector* visible=nullptr;
    float* scale=nullptr;
    const int8_t* facing=nullptr; // Original189, distinct from motion188.
    const int8_t* belong=nullptr;
    BattlePositionPart* head=nullptr;
};
struct BattlePositionPart {
    uint32_t attributes=0;
    int32_t id=0;
    BattlePositionUnit* owner=nullptr;
    BattlePositionPart* next=nullptr;
    JumpVector absolute{},offset{};
    std::array<JumpVector,2> hitOffsets{};
};
struct BattleHitPositionServices {
    void* user=nullptr;
    int8_t (*sideFactor)(void*,int8_t belong)=nullptr;
};
void getHomePosition(const BattlePositionUnit&,float*,float*,float*);
void getVisiblePosition(const BattlePositionUnit&,float*,float*,float*);
void getPartOffsetPosition(const BattlePositionPart&,float*,float*,float*);
void getPartWorldPosition(const BattlePositionPart&,float*,float*,float*);
void getHitPosition(const BattlePositionUnit&,const BattlePositionPart&,float*,float*,float*,const BattleHitPositionServices&);
BattlePositionPart* getPositionPart(const BattlePositionUnit&,int32_t id);
struct BattlePositionEventServices {
    void* user=nullptr;
    int32_t (*resolve)(void*,int32_t)=nullptr;
    int32_t (*translate)(void*,int32_t)=nullptr;
    BattlePositionUnit* (*unit)(void*,int32_t)=nullptr;
    // Separate registry request; owns its own shared-work reload and unit lookup.
    BattlePositionPart* (*part)(void*,int32_t unit,int32_t part)=nullptr;
    int32_t (*truncate)(void*,float)=nullptr;
    void (*store)(void*,int32_t,int32_t)=nullptr;
};
int hitPositionEvent(const std::array<int32_t,5>& operands,const BattlePositionEventServices&,const BattleHitPositionServices&);
}

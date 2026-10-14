#pragma once
#include "camera_depth.hpp"
#include <optional>
namespace ttyd {
struct BattleMonosiriServices {
    void* user=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<int32_t> (*switchFlag)(void*,int32_t)=nullptr;
};
struct BattleMonosiriCursor {
    bool active=false;uint32_t phase=0,battle=0,unit=0,kind=0,flags=0,result=0,pending=0;
};
// Checker8012ee3c: global B, retained kind, fresh unit flags and independent bank.
std::optional<int32_t> checkBattleMonosiri(uint32_t unit,const BattleMonosiriServices&,BattleMonosiriCursor&);
struct BattleEnemyHpDrawServices {
    void* user=nullptr;
    std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
    std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
    bool (*camera)(void*,uint32_t)=nullptr;
    std::optional<int32_t> (*status)(void*,uint32_t,int32_t)=nullptr;
    std::optional<int32_t> (*monosiri)(void*,uint32_t)=nullptr;
    bool (*screenPoint)(void*,const std::array<float,3>&,std::array<float,3>&)=nullptr;
    std::optional<float> (*interpolate)(void*,int32_t,int32_t,int32_t,float,float)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    bool (*number)(void*,const CameraViewMatrix&,int32_t,uint32_t,uint32_t)=nullptr;
    bool (*bar)(void*,const std::array<float,3>&,uint32_t,uint32_t)=nullptr;
};
struct BattleEnemyHpDrawCursor {
    bool active=false;uint32_t phase=0,battle=0,slot=0,unit=0,reloaded=0,step=0,pending=0,barColor=0xffffffff;
    int32_t elapsed=0,number=0;float fade=0;std::array<float,3> world{},screen{};CameraViewMatrix matrix{};
};
CameraDepthPoint battleEnemyHpScreenPoint(const CameraDepthSnapshot&,CameraDepthPoint);
bool drawBattleEnemyHp(uint32_t battle,const BattleEnemyHpDrawServices&,BattleEnemyHpDrawCursor&);
}

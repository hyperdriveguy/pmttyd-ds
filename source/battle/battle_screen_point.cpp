#include "battle_screen_point.hpp"
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t word) {
    float result;std::memcpy(&result,&word,sizeof(result));return result;
}
// Camera4 view+11c then projection+15c both transform the copied local
// in-place. Only after BOTH acknowledge fetch original scale/zero constants
// and publish output; failure does not partially update output XYZ.
bool projectCapturedPoint(uint32_t camera,BattleScreenPoint point,BattleScreenPoint& output,
                          const BattleScreenPointServices& s) {
    const auto read=[&](uint32_t address) {
        return scalar(s.memory.word(s.memory.user,address));
    };
    if(!s.view || !s.view(s.user,camera+0x11c,point,point))return false;
    if(!s.project || !s.project(s.user,camera+0x15c,point,point))return false;
    // Source r2=80427440: original304,240,0. Fetch after both SDK calls so
    // mutations at those boundaries retain the original read ordering.
    const float xScale=read(0x80422244),x=point[0];
    const float yScale=read(0x80422248);
    const float screenX=xScale*x;
    const float zero=read(0x80422240);
    output[0]=screenX;
    output[1]=yScale*point[1];
    output[2]=zero;
    return true;
}
}
bool packedBattleScreenPoint(uint32_t position,BattleScreenPoint& output,
                             const BattleScreenPointServices& s) {
    if(!s.camera)return false;
    const auto camera=s.camera(s.user,4);
    if(!camera)return false;
    const auto read=[&](uint32_t address) {
        return scalar(s.memory.word(s.memory.user,address));
    };
    // Source copies all three position words only after camGetPtr(4).
    BattleScreenPoint point{read(position),read(position+4),read(position+8)};
    return projectCapturedPoint(*camera,point,output,s);
}
bool packedBattleScreenPoint(const BattleScreenPoint& position,BattleScreenPoint& output,
                             const BattleScreenPointServices& s) {
    if(!s.camera)return false;
    const auto camera=s.camera(s.user,4);
    if(!camera)return false;
    const BattleScreenPoint point=position;
    return projectCapturedPoint(*camera,point,output,s);
}
}

#include "battle_enemy_hp_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t bits){int32_t out;std::memcpy(&out,&bits,4);return out;}
int32_t signedHalf(uint16_t v){return v<0x8000?int32_t(v):int32_t(v)-0x10000;}
float real(uint32_t bits){float out;std::memcpy(&out,&bits,4);return out;}
float add(float a,float b){volatile float out=a+b;return out;}
uint32_t bankAddress(uint32_t battle,uint32_t kind){return battle+0x163d4+4*uint32_t(signedWord(kind)/32);}
uint32_t bankMask(uint32_t kind){const uint32_t shift=uint32_t(signedWord(kind)%32)&63;return shift<32?uint32_t(1)<<shift:0;}
}
std::optional<int32_t> checkBattleMonosiri(uint32_t unit,const BattleMonosiriServices& s,BattleMonosiriCursor& c){
    if(!c.active){c={};c.active=true;c.unit=unit;}
    auto word=[&](uint32_t a){return s.word?s.word(s.user,a):std::nullopt;};
    for(;;)switch(c.phase){
    case 0:{c.pending=0x8012ee3c;const auto v=word(0x8041eb90);if(!v)return {};c.battle=*v;++c.phase;break;}
    case 1:{const auto v=word(c.unit+8);if(!v)return {};c.kind=*v;++c.phase;break;}
    case 2:{const auto v=word(c.battle+0x163f4);if(!v)return {};if(*v&2){c.result=1;c.phase=7;}else ++c.phase;break;}
    case 3:{c.pending=0x800858b4;const auto v=s.switchFlag?s.switchFlag(s.user,signedWord(c.kind+4474)):std::nullopt;if(!v)return {};c.result=uint32_t(*v);c.pending=0x8012ee3c;++c.phase;break;}
    case 4:{const auto v=word(c.unit+0x1c);if(!v)return {};c.flags=*v;++c.phase;break;}
    case 5:{const auto v=word(bankAddress(c.battle,c.kind));if(!v)return {};c.result|=*v&bankMask(c.kind);c.phase=c.flags&4?6:7;break;}
    case 6:{const auto v=word(c.unit+4);if(!v)return {};c.kind=*v;c.phase=8;break;}
    case 8:{const auto v=word(bankAddress(c.battle,c.kind));if(!v)return {};c.result|=*v&bankMask(c.kind);c.phase=7;break;}
    case 7:c.active=false;c.pending=0;return c.result?1:0;
    default:return {};
    }
}
CameraDepthPoint battleEnemyHpScreenPoint(const CameraDepthSnapshot& camera,CameraDepthPoint point){
    const auto projected=cameraDepthProject(camera,cameraDepthView(camera,point));
    volatile float x=304.f*projected[0],y=240.f*projected[1];return {x,y,0};
}
bool drawBattleEnemyHp(uint32_t battle,const BattleEnemyHpDrawServices& s,BattleEnemyHpDrawCursor& c){
    if(!c.active){c={};c.active=true;c.battle=battle;}
    auto word=[&](uint32_t a){return s.word?s.word(s.user,a):std::nullopt;};
    auto half=[&](uint32_t a){return s.half?s.half(s.user,a):std::nullopt;};
    auto next=[&](uint32_t phase){c.phase=phase;c.step=0;c.pending=0x8011fde0;};
    auto slotAddress=[&](){return c.battle+0x20+4*c.slot;};
    for(;;)switch(c.phase){
    case 0:c.pending=0x8000f2fc;if(!s.camera || !s.camera(s.user,8))return false;next(1);break;
    case 1:if(c.slot==64){c.active=false;c.pending=0;return true;}else {const auto v=word(slotAddress());if(!v)return false;c.unit=*v;next(c.unit?2:30);}break;
    case 2:{c.pending=0x80127814;const auto v=s.status?s.status(s.user,c.unit,27):std::nullopt;if(!v)return false;next(*v?30:3);break;}
    case 3:{const auto v=word(slotAddress());if(!v)return false;c.reloaded=*v;next(4);break;}
    case 4:{c.pending=0x8012ee3c;const auto v=s.monosiri?s.monosiri(s.user,c.reloaded):std::nullopt;if(!v)return false;next(*v?5:30);break;}
    case 5:{const auto v=word(c.unit+0x104);if(!v)return false;next(*v&0x02000000?30:6);break;}
    case 6:{const auto v=word(c.battle+0x1720);if(!v)return false;next(*v&0x01000000?8:7);break;}
    case 7:{const auto v=word(c.unit+0x1c);if(!v)return false;next(*v&1?8:30);break;}
    case 8:case 10:{const uint32_t offset=c.phase==8?0x3c:0x30;
        while(c.step<3){const auto v=word(c.unit+offset+4*c.step);if(!v)return false;c.world[c.step++]=real(*v);}next(c.phase==8?9:11);break;}
    case 9:{const auto v=word(c.battle+0xef4);if(!v)return false;next(*v&0x80?10:11);break;}
    case 11:{const auto v=half(c.unit+0xd4);if(!v)return false;c.world[0]=add(c.world[0],float(signedHalf(*v)));next(12);break;}
    case 12:{const auto v=half(c.unit+0xd6);if(!v)return false;c.world[1]=add(c.world[1],float(signedHalf(*v)));next(13);break;}
    case 13:c.pending=0x801009b4;if(!s.screenPoint || !s.screenPoint(s.user,c.world,c.screen))return false;next(14);break;
    case 14:{const auto v=word(c.unit+0x1c);if(!v)return false;next(*v&1?15:19);break;}
    case 15:{const auto v=half(c.unit+0x1fe);if(!v)return false;next(*v&1?16:19);break;}
    case 16:{const auto v=half(c.unit+0x204);if(!v)return false;c.elapsed=signedHalf(*v);next(c.elapsed<20?17:19);break;}
    case 17:{c.pending=0x80006c8c;const auto v=s.interpolate?s.interpolate(s.user,0,c.elapsed,20,0,255):std::nullopt;if(!v)return false;c.fade=*v;next(18);break;}
    case 18:{c.pending=0x8011ff38;const auto v=s.truncate?s.truncate(s.user,c.fade):std::nullopt;if(!v)return false;c.barColor=(c.barColor&0xffffff00)|uint8_t(*v);next(19);break;}
    case 19:{const auto v=word(slotAddress());if(!v)return false;c.reloaded=*v;next(20);break;}
    case 20:{const auto v=half(c.reloaded+0x10c);if(!v)return false;const int32_t hp=signedHalf(*v),offset=hp>=100?12:hp>=10?16:20;
        c.matrix={};for(unsigned i=0;i<3;++i)c.matrix[i][i]=1;c.matrix[0][3]=add(c.screen[0],float(offset));c.matrix[1][3]=add(c.screen[1],-32);next(21);break;}
    case 21:{const auto v=word(slotAddress());if(!v)return false;c.reloaded=*v;next(22);break;}
    case 22:{const auto v=half(c.reloaded+0x10c);if(!v)return false;c.number=signedHalf(*v);next(23);break;}
    case 23:c.pending=0x80019120;if(!s.number || !s.number(s.user,c.matrix,c.number,1,0xffffffff))return false;next(24);break;
    case 24:c.pending=0x8011fc3c;if(!s.bar || !s.bar(s.user,c.screen,c.unit,c.barColor))return false;next(30);break;
    case 30:++c.slot;next(1);break;
    default:return false;
    }
}
}

#include "battle_stock_exp_draw.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {float real(uint32_t bits){float f;std::memcpy(&f,&bits,4);return f;}float add(float a,float b){volatile float f=a+b;return f;}}
bool drawBattleStockExp(const BattleStockExpDrawServices& s,BattleStockExpDrawCursor& c){
    if(!c.active){c={};c.active=true;}
    auto word=[&](uint32_t a){return s.word?s.word(s.user,a):std::nullopt;};
    for(;;)switch(c.phase){
    case 0:{c.pending=0x80100258;const auto v=word(0x8041eb90);if(!v)return false;c.battle=*v;++c.phase;break;}
    case 1:{const auto v=word(c.battle+0xf04);if(!v)return false;std::memcpy(&c.count,&*v,4);if(c.count<=0)c.phase=5;else {if(c.count>100)c.count=100;c.tens=c.count/10;c.ones=c.count%10;c.phase=2;}break;}
    case 2:if(c.index>=uint32_t(c.ones)){c.index=0;c.phase=4;break;}else {const auto v=word(0x8041eb98);if(!v)return false;c.x=real(*v);c.phase=3;}break;
    case 3:case 7:{const bool big=c.phase==7;const auto v=word(0x8041eb9c);if(!v)return false;c.icon={};c.icon.flags=16;c.icon.icon=404;c.icon.alpha=255;c.icon.scale=big?.5f:.35f;
        const float b=add(add(308,c.x),big?-10.f:-3.5f);const float x=-std::fma(big?20.f:13.f,float(c.index),-b);
        c.icon.position={big?add(4,x):x,big?add(16,add(real(*v),-220)):add(real(*v),-220),-100};c.phase=big?8:6;break;}
    case 6:case 8:c.pending=0x8001a918;if(!s.icon || !s.icon(s.user,c.icon))return false;++c.index;c.pending=0x80100258;c.phase=c.phase==6?2:4;break;
    case 4:if(c.index>=uint32_t(c.tens)){c.phase=5;break;}else {const auto v=word(0x8041eb98);if(!v)return false;c.x=real(*v);c.phase=7;}break;
    case 5:c.active=false;c.pending=0;return true;
    default:return false;
    }
}
}

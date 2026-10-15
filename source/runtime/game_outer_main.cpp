#include "game_outer_main.hpp"
#include <cstring>
namespace ttyd {
bool updateGameOuterMain(const GameOuterMainServices& s,GameOuterMainCursor& c){
    if(!c.active){c={};c.active=true;}
    auto word=[&](uint32_t a){return s.word?s.word(s.user,a):std::nullopt;};
    auto call=[&](uint32_t fn){c.pending=fn;return s.request && s.request(s.user,fn,c.pending);};
    auto done=[&](){c.active=false;c.pending=0;return true;};
    for(;;)switch(c.phase){
    case 0:{c.pending=0x8006f904;const auto gp=word(0x80415fd0);if(!gp)return false;c.gp=*gp;c.phase=1;break;}
    case 1:{const auto ready=word(c.gp+0x10);if(!ready)return false;c.phase=*ready?2:3;break;}
    case 2:if(!call(0x8029ccf4))return false;c.phase=0;break;
    case 3:if(c.index==gameOuterMainCalls.size()){c.phase=4;break;}
        if(!call(gameOuterMainCalls[c.index]))return false;
        ++c.index;break;
    case 4:{c.pending=0x8006f904;const auto gp=word(0x80415fd0);if(!gp)return false;c.gp=*gp;c.phase=5;break;}
    case 5:{const auto flags=word(c.gp);if(!flags)return false;if(!(*flags&0x1000))return done();c.phase=6;break;}
    case 6:{
        if(c.channel==4){c.phase=7;break;}
        static constexpr uint32_t offsets[]{0x1378,0x1328,0x13b8,0x13bc,0x13c0,0x13c4,0x13c8,0x13cc};
        uint32_t value;
        if(c.field<2){const auto v=word(c.gp+offsets[c.field]+4*c.channel);if(!v)return false;value=*v;}
        else {const auto v=s.byte?s.byte(s.user,c.gp+offsets[c.field]+c.channel):std::nullopt;if(!v)return false;value=*v;}
        if(value){c.phase=20;break;}
        if(++c.field==8){c.field=0;++c.channel;}break;}
    case 7:{c.pending=0x8029d84c;const auto t=s.time?s.time(s.user):std::nullopt;if(!t)return false;c.time=*t;c.phase=8;break;}
    case 8:{const auto v=word(0x8041e94c);if(!v)return false;c.low=*v;c.phase=9;break;}
    case 9:{const auto v=word(0x800000f8);if(!v)return false;c.clock=*v;c.phase=10;break;}
    case 10:{const auto v=word(0x8041e948);if(!v)return false;c.high=*v;c.phase=11;break;}
    case 11:{c.pending=0x80264ad8;const uint64_t bits=c.time-((uint64_t(c.high)<<32)|c.low);int64_t delta;std::memcpy(&delta,&bits,8);
        const auto q=s.divide?s.divide(s.user,delta,c.clock>>2):std::nullopt;if(!q)return false;c.quotient=*q;if(*q<=120)return done();c.phase=12;break;}
    case 12:case 13:{c.pending=0x8002e210;const auto seq=s.sequence?s.sequence(s.user):std::nullopt;if(!seq)return false;
        if(*seq==(c.phase==12?2:4))c.phase=14;else if(c.phase==12)c.phase=13;else return done();break;}
    case 14:{const auto gp=word(0x80415fd0);if(!gp)return false;c.gp=*gp;c.phase=15;break;}
    case 15:{const auto flags=word(c.gp);if(!flags)return false;c.flags=*flags|0x2000;c.phase=16;break;}
    case 16:if(!s.store || !s.store(s.user,c.gp,c.flags))return false;return done();
    case 20:{c.pending=0x8029d84c;const auto t=s.time?s.time(s.user):std::nullopt;if(!t)return false;c.time=*t;c.phase=21;break;}
    case 21:if(!s.store || !s.store(s.user,0x8041e94c,uint32_t(c.time)))return false;c.phase=22;break;
    case 22:if(!s.store || !s.store(s.user,0x8041e948,uint32_t(c.time>>32)))return false;return done();
    default:return false;
    }
}
}

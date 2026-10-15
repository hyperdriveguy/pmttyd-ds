#include "battle_break_slot_draw.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {float real(uint32_t v){float f;std::memcpy(&f,&v,4);return f;}int32_t signedWord(uint32_t v){int32_t n;std::memcpy(&n,&v,4);return n;}}
bool drawBattleBreakSlot(const BattleBreakSlotDrawServices& s,BattleBreakSlotDrawCursor& c){
    if(!c.active){c={};c.active=true;}
    auto word=[&](uint32_t a){return s.word?s.word(s.user,a):std::nullopt;};
    auto next=[&](uint32_t phase){c.phase=phase;c.step=0;c.queued=false;c.pending=0x802036d4;};
    auto getter=[&](uint32_t following){c.pending=0x80205210;const auto b=word(0x8041eb90);if(!b)return false;c.base=*b+0x16184;c.index=0;next(following);return true;};
    auto matrix=[&](){c.pending=c.matrix.function;return s.matrix && s.matrix(s.user,c.matrix);};
    for(;;)switch(c.phase){
    case 0:if(!getter(1))return false;break;
    case 1:if(c.index==3){next(6);break;}else {c.reel=c.base+0x5c*c.index;const auto f=word(c.reel+4);if(!f)return false;next(*f&1?2:5);}break;
    case 2:{const auto state=word(c.reel+8);if(!state)return false;c.state=signedWord(*state);next(c.state>=1 && c.state<=4?3:5);break;}
    case 3:if(!c.queued){c.matrix={};c.matrix.function=0x8029e230;c.matrix.xyz={std::fma(42.f,float(c.index),226.f),122,0};c.queued=true;}
        if(!matrix())return false;
        c.icon={};c.icon.function=0x8001a4bc;c.icon.flags=16;c.icon.icon=476;c.icon.matrix=c.matrix.output;next(4);break;
    case 4:if(!c.queued){const auto color=word(0x80426884);if(!color)return false;c.icon.rgba=*color;c.queued=true;}
        c.pending=0x8001a4bc;
        if(s.icon){if(!s.icon(s.user,c.icon))return false;}
        else if(!s.sourceIcon || !resumeSourceIconMatrix(c.icon,*s.sourceIcon,c.iconCursor)){if(c.iconCursor.active)c.pending=c.iconCursor.pending;return false;}
        next(5);break;
    case 5:++c.index;next(1);break;
    case 6:if(!getter(7))return false;break;
    case 7:{const auto state=word(c.base+8);if(!state)return false;c.state=signedWord(*state);
        if(c.state>=1 && c.state<=4){c.scissor={0,82,608,32};next(8);}else {c.table=0x80419f3c;c.scissor={0,176,608,128};next(c.state==11?9:8);}break;}
    case 8:c.pending=0x802ba644;if(!s.scissor || !s.scissor(s.user,c.scissor))return false;
        if(c.state>=1 && c.state<=4)c.table=0x80419f30;
        next(9);break;
    case 9:if(!getter(10))return false;break;
    case 10:if(c.index==3){next(35);break;}else {c.reel=c.base+0x5c*c.index;const auto flags=word(c.reel+4);if(!flags)return false;next(*flags&1?11:34);}break;
    case 11:{static const ShadowGXRequest setup[]={{ShadowGX::Blend,{1,4,5,0}},{ShadowGX::DepthLocation,{1}},{ShadowGX::Alpha,{7,0,0,7,0}},{ShadowGX::Depth,{1,3,0}}};
        while(c.step<4){c.pending=uint32_t(setup[c.step].kind);if(!s.gx || !s.gx(s.user,setup[c.step]))return false;++c.step;}next(12);break;}
    case 12:{
        // Preserve every SDK boundary, including the fresh negative-pivot read.
        static constexpr uint32_t functions[]{0x8029e230,0x8029e2b0,0x8029dff0,0x8029dff0,0x8029dff0,0x8029dd64,0x8029dd64,0x8029e230,0x8029e230,0x8029dd64,0x8029dd64,0x8029dd64,0x8029dd64};
        static constexpr unsigned output[]{0,1,2,3,4,5,5,6,7,5,5,5,5};
        if(c.category==13){next(13);break;}
        const auto op=c.category;
        if(!c.queued){c.matrix={};c.matrix.function=functions[op];c.queued=true;c.step=0;
            if(op==5){c.matrix.left=c.matrices[2];c.matrix.right=c.matrices[4];}
            if(op==6){c.matrix.left=c.matrices[3];c.matrix.right=c.matrices[5];}
            if(op==9){c.matrix.left=c.matrices[5];c.matrix.right=c.matrices[6];}
            if(op>=10){c.matrix.left=c.matrices[op==10?7:op==11?1:0];c.matrix.right=c.matrices[5];}}
        if(op==0 || op==1 || op==7 || op==8){const uint32_t offset=op==0?0x20:op==1?0x38:0x44;
            while(c.step<3){const auto v=word(c.reel+offset+4*c.step);if(!v)return false;c.matrix.xyz[c.step++]=op==8?-real(*v):real(*v);}}
        if(op>=2 && op<=4 && c.step==0){const auto v=word(0x804268a0);if(!v)return false;c.matrix.radians=real(*v);c.step=1;}
        if(op>=2 && op<=4 && c.step==1){const auto v=word(c.reel+0x2c+4*(op-2));if(!v)return false;volatile float radians=c.matrix.radians*real(*v);c.matrix.radians=radians;c.matrix.axis=120+op-2;c.step=2;}
        if(!matrix())return false;
        c.matrices[output[op]]=c.matrix.output;c.queued=false;c.step=0;++c.category;break;}
    case 13:{const auto color=word(0x80426888);if(!color)return false;c.color=*color;next(14);break;}
    case 14:{const auto category=word(c.reel+0x10);if(!category)return false;c.category=*category;next(15);break;}
    case 15:{const auto id=s.half?s.half(s.user,c.table+2*c.category):std::nullopt;if(!id)return false;c.plane={};c.plane.texture=*id;c.plane.matrix=c.matrices[5];next(16);break;}
    case 16:{const auto alpha=s.byte?s.byte(s.user,c.reel+0x58):std::nullopt;if(!alpha)return false;c.plane.rgba=(c.color&0xffffff00)|*alpha;next(17);break;}
    case 17:c.pending=0x8010323c;if(!s.plane || !s.plane(s.user,c.plane))return false;next(18);break;
    case 18:{const auto state=word(c.reel+8);if(!state)return false;c.state=signedWord(*state);next(c.state>10 || c.state==3 || c.state==4?34:19);break;}
    case 19:{const auto ordinal=word(c.reel+0x1c);if(!ordinal)return false;c.candidate=*ordinal+1;next(20);break;}
    case 20:{const auto count=word(c.reel+0x18);if(!count)return false;if(signedWord(c.candidate)>=signedWord(*count))c.candidate=0;next(21);break;}
    case 21:{const auto data=word(c.reel+0x14);if(!data)return false;c.data=*data;next(22);break;}
    case 22:{const auto category=word(c.data+4*c.candidate);if(!category)return false;c.category=*category;next(23);break;}
    case 23:{const auto id=s.half?s.half(s.user,c.table+2*c.category):std::nullopt;if(!id)return false;c.plane.texture=*id;next(24);break;}
    case 24:if(!c.queued){c.matrix={};c.matrix.function=0x8029e230;c.matrix.xyz={0,c.state>=1 && c.state<=4?-32.f:-128.f,0};c.queued=true;}
        if(!matrix())return false;
        c.matrices[6]=c.matrix.output;next(25);break;
    case 25:if(!c.queued){c.matrix={};c.matrix.function=0x8029dd64;c.matrix.left=c.matrices[6];c.matrix.right=c.matrices[5];c.queued=true;}
        if(!matrix())return false;
        c.matrices[5]=c.matrix.output;c.plane.matrix=c.matrix.output;next(26);break;
    case 26:{const auto color=word(0x8042688c);if(!color)return false;c.color=*color;next(27);break;}
    case 27:{const auto alpha=s.byte?s.byte(s.user,c.reel+0x58):std::nullopt;if(!alpha)return false;c.plane.rgba=(c.color&0xffffff00)|*alpha;next(28);break;}
    case 28:c.pending=0x8010323c;if(!s.plane || !s.plane(s.user,c.plane))return false;next(34);break;
    case 34:++c.index;c.category=0;next(10);break;
    case 35:{const auto gp=word(0x80415fd0);if(!gp)return false;c.gp=*gp;next(36);break;}
    case 36:{const auto width=s.half?s.half(s.user,c.gp+0x170):std::nullopt;if(!width)return false;c.scissor={0,0,*width,0};next(37);break;}
    case 37:{const auto height=s.half?s.half(s.user,c.gp+0x172):std::nullopt;if(!height)return false;c.scissor[3]=*height;next(38);break;}
    case 38:c.pending=0x802ba644;if(!s.scissor || !s.scissor(s.user,c.scissor))return false;next(39);break;
    case 39:c.active=false;c.pending=0;return true;
    default:return false;
    }
}
}

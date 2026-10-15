#include "source_icon_position.hpp"
#include <cstring>
namespace ttyd {
namespace {float real(uint32_t bits){float out;std::memcpy(&out,&bits,4);return out;}}
bool resumeSourceIconPosition(const SourceIconPositionRequest& request,const SourceIconMatrixServices& s,SourceIconPositionCursor& c){
    if(!c.active){c={};c.active=true;c.request=request;}
    auto word=[&](uint32_t a){return s.word?s.word(s.user,a):std::nullopt;};
    auto half=[&](uint32_t a){return s.half?s.half(s.user,a):std::nullopt;};
    auto next=[&](uint32_t phase){c.phase=phase;c.queued=false;c.step=0;c.pending=0x8001a93c;};
    auto matrix=[&](){c.pending=c.matrix.function;return s.matrix && s.matrix(s.user,c.matrix);};
    for(;;)switch(c.phase){
    case 0:{c.pending=0x8000f2e4;const auto v=s.currentCamera?s.currentCamera(s.user):std::nullopt;if(!v)return false;c.camera=*v;next(1);break;}
    case 1:{const auto v=word(0x8041e638);if(!v)return false;next(*v?2:20);break;}
    case 2:{const auto v=word(0x8041e63c);if(!v)return false;next(*v?3:20);break;}
    case 3:if(!c.queued){c.matrix={};c.matrix.function=0x8029e230;c.matrix.xyz=request.position;c.queued=true;}
        if(!matrix())return false;
        c.matrices[0]=c.matrix.output;next(4);break;
    case 4:if(!c.queued){c.matrix={};c.matrix.function=0x8029e2b0;c.matrix.xyz.fill(c.request.scale);c.queued=true;}
        if(!matrix())return false;
        c.matrices[1]=c.matrix.output;next(5);break;
    case 5:{const auto v=word(c.camera+0x114);if(!v)return false;c.matrix={};c.matrix.radians=-real(*v);next(6);break;}
    case 6:{const auto v=word(0x8041f8a0);if(!v)return false;volatile float radians=real(*v)*c.matrix.radians;c.matrix.radians=radians;c.matrix.function=0x8029dff0;c.matrix.axis=121;next(7);break;}
    case 7:if(!matrix())return false;
        c.matrices[2]=c.matrix.output;next(8);break;
    case 8:case 9:if(!c.queued){c.matrix={};c.matrix.function=0x8029dd64;c.matrix.left=c.matrices[0];c.matrix.right=c.matrices[c.phase==8?2:1];c.queued=true;}
        if(!matrix())return false;
        c.matrices[0]=c.matrix.output;next(c.phase==8?9:10);break;
    case 10:if(!c.queued){c.matrix={};c.matrix.function=0x8029dd64;c.matrix.right=c.matrices[0];c.queued=true;}
        while(c.step<12){const auto v=word(c.camera+0x11c+4*c.step);if(!v)return false;c.matrix.left[c.step++]=real(*v);}next(11);break;
    case 11:if(!matrix())return false;
        c.matrices[0]=c.matrix.output;next(12);break;
    case 12:{const auto v=word(0x8041f860);if(!v)return false;c.color=*v;next(13);break;}
    case 13:{const auto v=word(0x8041e638);if(!v)return false;c.descriptor=*v;next(14);break;}
    case 14:{const auto v=word(0x8041e634);if(!v)return false;c.metadata=*v;c.icon.flags=uint16_t(c.request.flags);c.icon.id=uint16_t(c.request.icon);c.icon.rgba=(c.color&0xffffff00)|uint8_t(c.request.alpha);next(c.descriptor?15:17);if(c.phase==17)c.descriptor=0;break;}
    case 15:{const auto v=word(0x8041e63c);if(!v)return false;if(!*v){c.descriptor=0;next(17);}else next(16);break;}
    case 16:c.selected=c.icon.id;next(c.selected==73 || c.selected==103?21:24);break;
    case 21:{const auto v=word(0x80415fd0);if(!v)return false;c.gp=*v;next(22);break;}
    case 22:{const auto v=word(c.gp+0x16c);if(!v)return false;c.gp=*v;next(23);break;}
    case 23:{const auto v=half((c.selected==73?0x80415040u:0x8041504cu)+(c.gp<<1));if(!v)return false;c.selected=*v;next(24);break;}
    case 24:{const auto v=half(c.metadata+((c.selected<<1)&0x1fffeu)+2);if(!v)return false;c.descriptor=c.metadata+*v;next(17);break;}
    case 17:{
        // The late unload path really reads address2; it is not a second empty-icon return.
        while(c.step<5){static constexpr uint32_t offsets[]{2,4,8,10,6};const auto v=half(c.descriptor+offsets[c.step]);if(!v)return false;
            switch(c.step){case 0:c.icon.width=*v;break;case 1:c.icon.height=*v;break;case 2:c.icon.texture=*v;break;case 3:c.icon.delay=*v;break;case 4:c.color=*v;break;}++c.step;}
        c.frame=0;c.total=0;next(c.color>1?25:18);break;}
    case 25:while(c.frame<c.color){const auto v=half(c.descriptor+10+4*c.frame);if(!v)return false;c.total+=*v;++c.frame;}
        if(!c.total || c.total>0x7fffffffu)return false;
        next(26);break;
    case 26:{const auto v=word(0x80415fd0);if(!v)return false;c.gp=*v;next(27);break;}
    case 27:{const auto v=word(c.gp+0x1c);if(!v)return false;int32_t tick;std::memcpy(&tick,&*v,4);c.remaining=tick%int32_t(c.total);c.frame=0;next(28);break;}
    case 28:if(c.frame==c.color){next(18);break;}else {const auto v=half(c.descriptor+10+4*c.frame);if(!v)return false;if(c.remaining<int32_t(*v))next(29);else {c.remaining-=*v;++c.frame;}}break;
    case 29:{const auto v=half(c.descriptor+8+4*c.frame);if(!v)return false;c.icon.texture=*v;next(18);break;}
    case 18:c.pending=0x800197f8;
        if(s.draw){if(!s.draw(s.user,c.matrices[0],c.icon))return false;}
        else if(!s.gx || !resumeSourceIconGX(c.matrices[0],c.icon,*s.gx,c.gxCursor)){if(c.gxCursor.active)c.pending=c.gxCursor.pending;return false;}
        next(20);break;
    case 20:c.active=false;c.pending=0;return true;
    default:return false;
    }
}
}

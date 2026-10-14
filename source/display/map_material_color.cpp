#include "map_material_color.hpp"
namespace ttyd {
uint32_t mapMaterialLastStageColor(uint32_t flags,uint32_t c1,uint32_t c2,const MapMaterialGroup& group){
    std::array<uint8_t,4> rgba{255,255,255,255};
    auto multiply=[&](uint32_t color){for(unsigned i=0;i<4;++i)rgba[i]=uint8_t(unsigned(rgba[i])*((color>>(24-8*i))&255)/255);};
    if(flags&0x40)multiply(c1);
    const auto alpha=c2>>24;if(alpha!=255)rgba[3]=uint8_t(unsigned(rgba[3])*alpha/255);
    const uint16_t mapFlags=group.flags;
    if((mapFlags&2) && !(flags&0x800))multiply(group.colors[0]);
    if((mapFlags&4) && !(flags&0x1000))multiply(group.colors[1]);
    if((mapFlags&8) && !(flags&0x800))multiply(group.colors[2]);
    return uint32_t(rgba[0])<<24|uint32_t(rgba[1])<<16|uint32_t(rgba[2])<<8|rgba[3];
}
bool resumeMapMaterialColor(uint32_t function,uint32_t flags,uint32_t c1,uint32_t c2,const MapMaterialColorServices& s,MapMaterialColorCursor& c){
    if(!c.active){if(function!=0x80021860 && function!=0x800217c8)return false;c={};c.active=true;c.function=function;c.flags=flags;c.c1=c1;c.c2=c2;}
    if(c.function!=function)return false;
    if(!c.ready){
        if(function==0x800217c8 && !c.camera){c.pending=0x8000f2e4;if(!s.currentCamera || !(c.camera=s.currentCamera(s.user)))return false;}
        c.pending=function;const auto* group=s.group?s.group(s.user):nullptr;if(!group)return false;
        c.request={};
        if(function==0x80021860){c.request.kind=ShadowGX::TevKColor;c.request.color=mapMaterialLastStageColor(c.flags,c.c1,c.c2,*group);}
        else {
            c.request.kind=ShadowGX::Fog;c.request.color=0xffffffff;
            if(group->flags&1){constexpr int32_t types[]{2,4,5,6,7};if(group->fogType>=5)return false; // Native resource admission, not a source bounds check.
                c.request.values[0]=types[group->fogType];c.request.color=group->fogColor;
                c.request.floats={group->fogStart,group->fogEnd,c.camera->nearPlane,c.camera->farPlane};}
        }
        c.ready=true;
    }
    c.pending=uint32_t(c.request.kind);if(!s.gx || !s.gx(s.user,c.request))return false;
    c.active=false;c.pending=0;return true;
}
}

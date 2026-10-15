#include "npc_map_move.hpp"
#include <cmath>
#include <algorithm>
namespace ttyd {
// Engine-space probes at min(.2*height, 10),.4*height,.75*height. First
// admitted hit wins; middle probe alone requires slope angle>=45. Extent is
// travel length+.75*width; flags 2000/10000(hex) need unbound other paths.
std::optional<NpcMapMove> checkNpcMapSide(FieldWallVector root,float width,float height,
    float dx,float dz,uint32_t flags,const NpcMapMoveServices& s) {
    NpcMapMove result{root,{}};
    if(dx==0 && dz==0)return result;
    if((flags&(0x2000|0x10000)) || !s.direction || !s.length || !s.normalLength || !s.angle || !s.ray)return {};
    const auto direction=s.direction(s.user,dx,dz);
    const float length=s.length(s.user,dx,dz);
    const float offsets[]={std::min(.2f*height,10.f),.4f*height,.75f*height};
    for(unsigned probe=0;probe<3;++probe) {
        auto origin=root;origin.y+=offsets[probe];
        const auto hit=s.ray(s.user,origin,direction,std::fma(.75f,width,length));
        if(!hit.token)continue;
        if(probe==1 && !(s.angle(s.user,s.normalLength(s.user,hit.normal.x,hit.normal.z),-hit.normal.y)>=45.f))continue;
        const float dot=std::fma(dx,hit.normal.x,dz*hit.normal.z);
        result.root.x+=.5f*std::fma(-dot,hit.normal.x,dx);
        result.root.z+=.5f*std::fma(-dot,hit.normal.z,dz);
        result.contact=hit;return result;
    }
    result.root.x+=dx;result.root.z+=dz;return result;
}
// Publish FIRST contact before re-querying tentative displacement. Second
// geometry changes root only; returned contact deliberately stays the first hit.
// nullopt is unsupported/unavailable, not an acknowledged null-contact miss.
std::optional<NpcMapMove> advanceNpcMapSide(FieldWallVector root,float width,float height,
    float dx,float dz,uint32_t flags,const NpcMapMoveServices& s) {
    auto first=checkNpcMapSide(root,width,height,dx,dz,flags,s);
    if(first && s.publishContact)s.publishContact(s.user,first->contact);
    if(!first || !first->contact.token)return first;
    auto second=checkNpcMapSide(root,width,height,first->root.x-root.x,first->root.z-root.z,flags,s);
    if(!second)return {};
    second->contact=first->contact;return second;
}
}

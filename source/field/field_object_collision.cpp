#include "field_object_collision.hpp"
#include <cmath>
namespace ttyd {
// Kind1 map objects/hitMode1 use original bbox geometry, not animated draw
// bounds. Source factor 10 precedes engine->native*.01. Unit box has8 points,
// six four-index strips producing12 triangles with preserved winding.
void buildFieldObjectCollision(const FieldActorAssets& assets,uint32_t room,std::optional<int32_t> story,
                              uint32_t interior,uint32_t system,std::vector<FieldObjectTriangle>& output) {
    output.clear();
    constexpr float unit[8][3]={{-.5f,0,.5f},{.5f,0,.5f},{-.5f,1,.5f},{.5f,1,.5f},
                              {-.5f,1,-.5f},{.5f,1,-.5f},{-.5f,0,-.5f},{.5f,0,-.5f}};
    constexpr unsigned strips[6][4]={{0,6,2,4},{7,1,5,3},{7,6,1,0},{5,4,7,6},{3,2,5,4},{1,0,3,2}};
    for(const auto& actor:assets.actors()) {
        if(!actor.visible(room,story,interior,system) || actor.kind!=1)continue;
        const auto& model=assets.models()[actor.model];if(model.hitMode!=1)continue;
        const auto& b=model.hitBounds;
        const float dimensions[3]={10*std::fabs(b[0]-b[3]),10*std::fabs(b[4]),10*std::fabs(b[2]-b[5])};
        // Original bbox center offsets are added after Y rotation, not rotated
        // with the unit box. Height uses abs(maxY) rather than Y span.
        const float offsetX=(b[0]+b[3])*.5f*10,offsetZ=(b[2]+b[5])*.5f*10;
        const float angle=actor.yaw*.017453292519943295f,c=std::cos(angle),s=std::sin(angle);
        std::array<std::array<float,3>,8> vertices;
        for(unsigned i=0;i<8;++i) {
            const float x=unit[i][0]*dimensions[0],y=unit[i][1]*dimensions[1],z=unit[i][2]*dimensions[2];
            vertices[i]={(std::fma(c,x,s*z)+actor.position[0]+offsetX)*.01f,
                         (y+actor.position[1])*.01f,
                         (std::fma(-s,x,c*z)+actor.position[2]+offsetZ)*.01f};
        }
        for(const auto& strip:strips)for(unsigned j=0;j<2;++j) {
            FieldObjectTriangle triangle;triangle.owner=actor.key;
            const auto& name=model.originalName;
            // Hidden block models retain geometry identity but fail ordinary
            // solid-name admission; their activation/event owner is separate.
            triangle.ordinarySolid=!name.empty() && name!="MOBJ_HiddenHatenaBlock" &&
                name!="MOBJ_HiddenBadgeBlock" && name!="MOBJ_Hidden10CountBlock";
            const unsigned indices[3]={strip[j],strip[j+(j%2?1:2)],strip[j+(j%2?2:1)]};
            for(unsigned k=0;k<3;++k)for(unsigned axis=0;axis<3;++axis)triangle.vertices[k*3+axis]=vertices[indices[k]][axis];
            output.push_back(triangle);
        }
    }
}
}

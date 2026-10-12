#include "field_wall.hpp"
#include <algorithm>
#include <cmath>

namespace ttyd {
float quantizeFieldWall(float value) {
    return std::trunc(std::fma(1000.f,value,value>=0?.5f:-.5f))/1000.f;
}
bool fieldWallPassThrough(uint32_t attributes,const FieldWallActor& actor) {
    if((attributes&0x00080000) && !(actor.flags&0x01000000))return false;
    if(!(attributes&0x80))return (attributes&0x00800000)!=0;
    return actor.motion==25;
}
namespace {
FieldWallVector rotate(FieldWallVector d,float angle) {
    const float radians=angle*3.14159265358979323846f/180.f;
    const float s=std::sin(radians),c=std::cos(radians);
    return {std::fma(d.x,c,-d.z*s),0,std::fma(d.x,s,d.z*c)};
}
FieldWallHit ray(const FieldWallServices& s,FieldWallVector origin,FieldWallVector heading,float length) {
    auto hit=s.ray(s.user,origin,heading,length);
    if(hit.token) {
        hit.distance=quantizeFieldWall(hit.distance);
    }
    return hit;
}
}
FieldWallMove searchFieldFrontWall(FieldWallVector position,FieldWallVector d,
                                  const FieldWallActor& actor,const FieldWallServices& s) {
    const float speed=std::hypot(d.x,d.z),radius=actor.width*.5f;
    FieldWallMove result;result.position=position;
    if(speed==0)return result; // Zero-speed staying has a separate native owner.
    const FieldWallVector heading{d.x/speed,0,d.z/speed};
    const float heights[]={26.64000129699707f,11,.1f};
    const int count=actor.counter!=0?3:2;
    for(int n=0;n<count;++n) {
        auto hit=ray(s,{position.x,position.y+heights[n],position.z},heading,speed+radius);
        if(hit.token) {
            hit.normal.x=quantizeFieldWall(hit.normal.x);
            hit.normal.y=quantizeFieldWall(hit.normal.y);
            hit.normal.z=quantizeFieldWall(hit.normal.z);
        }
        if(hit.token && (!result.forward.token || hit.distance<result.forward.distance))result.forward=hit;
    }
    // The +45 guard stops at first hit. Only if it hit does the -45 guard run;
    // that guard retains its LAST ray token, including a null final token.
    FieldWallHit side;
    auto sideHeading=rotate(heading,45);
    for(int n=0;n<count;++n) {
        side=ray(s,{position.x,position.y+heights[n],position.z},sideHeading,radius);
        if(side.token)break;
    }
    if(side.token) {
        sideHeading=rotate(heading,-45);
        for(int n=0;n<count;++n)side=ray(s,{position.x,position.y+heights[n],position.z},sideHeading,radius);
    }
    result.sideBlocked=side.token!=0;
    if(result.forward.token) {
        result.distanceWritten=true;
        result.contactDistance=result.sideBlocked?0:side.distance;
        if(!result.sideBlocked) {
            const float dot=std::fma(d.x,result.forward.normal.x,d.z*result.forward.normal.z);
            const float x=std::fma(-dot,result.forward.normal.x,d.x)*.5f;
            const float z=std::fma(-dot,result.forward.normal.z,d.z)*.5f;
            result.position.x+=x;result.position.z+=z;
        }
    } else if(result.sideBlocked) {
        // Side-only obstruction returns without a forward hit or displacement.
    } else {
        result.position.x+=d.x;result.position.z+=d.z;
    }
    return result;
}
bool correctFieldWallsAround(FieldWallVector& position,FieldWallVector heading,int32_t mode,
                            const FieldWallActor& actor,const FieldWallServices& s) {
    const float norm=std::hypot(heading.x,heading.z);
    if(norm==0)return false;
    heading={heading.x/norm,0,heading.z/norm};
    const int count=(actor.viewFlags&1)?2:8;
    const float radius=actor.width*.5f;
    float cap=actor.width*((mode==0 && (actor.motion==10 || actor.motion==11))?.25f:.125f);
    cap=std::max(1.f,cap);
    FieldWallVector first;
    for(int pass=0;pass<2;++pass) {
        FieldWallVector push;
        bool blocked=false;
        for(int n=0;n<count;++n) {
            const auto direction=rotate(heading,n*(360.f/count));
            FieldWallHit nearest;
            for(float height:{11.f,actor.height-1}) {
                auto hit=ray(s,{position.x,position.y+height,position.z},direction,radius);
                if(hit.token && !fieldWallPassThrough(hit.attributes,actor) &&
                   (!nearest.token || hit.distance<nearest.distance))nearest=hit;
            }
            if(nearest.token) {
                blocked=true;
                const float amount=std::fabs(radius-nearest.distance);
                push.x=std::fma(-direction.x,amount,push.x);
                push.z=std::fma(-direction.z,amount,push.z);
            }
        }
        if(!blocked)return pass!=0;
        if(pass==0) {
            const float length=std::hypot(push.x,push.z);
            if(length>cap){push.x*=cap/length;push.z*=cap/length;}
            first=push;position.x+=push.x;position.z+=push.z;
        } else if(std::fma(first.x,push.x,first.z*push.z)>0) {
            position.x+=push.x*.5f;position.z+=push.z*.5f;
        }
    }
    return true;
}
}

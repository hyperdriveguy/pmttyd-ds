#include "shadow_collision.hpp"

namespace ttyd {
namespace {
// Explicit binary32 boundaries prevent native contraction across the source's
// fadds/fmuls/fsubs instructions. NaN payload/exception behavior is unverified.
float add(float a,float b){volatile float value=a+b;return value;}
float mul(float a,float b){volatile float value=a*b;return value;}
float sub(float a,float b){volatile float value=a-b;return value;}
void hitMatrix(ShadowRecord& record,const ShadowCollisionHit& hit){
    const float radius=record.radius,height=record.height;
    const auto& p=hit.point;const auto& n=hit.normal;auto& m=record.matrix;
    // Literal source store/arithmetic order; do not normalize the normal,
    // reverse its sign or replace it with a vertical support normal.
    m[3]=add(p[0],n[0]);m[7]=add(p[1],n[1]);m[11]=add(p[2],n[2]);
    m[4]=mul(n[0],radius);m[5]=mul(n[1],height);m[6]=mul(n[2],radius);
    m[8]=0;m[9]=mul(n[2],radius);m[10]=mul(-n[1],radius);
    const float nz2=mul(n[2],n[2]),negativeNy2=mul(-n[1],n[1]);
    m[0]=mul(radius,sub(negativeNy2,nz2));
    m[1]=mul(radius,mul(n[0],n[1]));m[2]=mul(radius,mul(n[0],n[2]));
}
}
bool shadowPrepareCollision(ShadowWork& work,ShadowCollisionCursor& cursor,const ShadowCollisionServices& services){
    if(!cursor.active){
        cursor={};cursor.active=true;cursor.owner=&work;cursor.pool=work.record(0);
    }
    if(cursor.owner!=&work)return false; // Borrowed source owner must survive holds.
    if(!cursor.cameraQueried){
        if(!services.currentCamera || !services.currentCamera(services.user))return false;
        cursor.cameraQueried=true;
    }
    while(cursor.rayPending || cursor.index<work.count()){
        if(cursor.index>=64)return false; // Native pool admission, not a source skip.
        auto& record=cursor.pool[cursor.index];
        if(!cursor.rayPending){
            if(record.radius==0){++cursor.index;continue;}
            const auto flags=record.flags;
            if(flags&2){++cursor.index;continue;}
            if(record.type==3 || !(flags&1)){
                // PSMTXTrans leaf, followed by source diagonal stores10,0,5.
                const float y=record.position[1],x=record.position[0],translatedY=add(1.5f,y),z=record.position[2];
                auto& m=record.matrix;
                m[3]=x;m[7]=translatedY;m[1]=0;m[2]=0;m[8]=0;m[9]=0;
                m[4]=0;m[5]=1;m[6]=0;m[10]=1;m[11]=z;m[0]=1;
                const float radius=record.radius;m[10]=radius;m[0]=radius;m[5]=record.height;
                record.flags|=2;++cursor.index;continue;
            }
            cursor.ray={};cursor.ray.origin=record.position;
            cursor.ray.origin[1]=add(cursor.ray.origin[1],1.f);
            cursor.rayPending=true;
        }
        if(!services.ray)return false;
        ShadowCollisionHit hit;
        const auto result=services.ray(services.user,cursor.ray,hit);
        if(result==ShadowRayResult::Unavailable)return false;
        if(result==ShadowRayResult::Hit){hitMatrix(record,hit);record.flags|=2;}
        cursor.rayPending=false;++cursor.index;
    }
    cursor.active=false;return true;
}
}

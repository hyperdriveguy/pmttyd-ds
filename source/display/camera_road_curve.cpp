#include "camera_road_curve.hpp"
#include <array>
#include <cmath>
namespace ttyd {
namespace {
struct Path {
    std::array<CameraPoint,6> points,rows;
    float left=0,right=0;
};
std::optional<Path> readPath(const CameraRoadAsset& asset,uint32_t road) {
    const auto start=asset.roadWord(road,0xa0),count=asset.roadWord(road,0xa4);
    if(!start || count!=12 || asset.roadWord(road,0x40)!=2 || asset.roadWord(road,0x44)!=3 ||
       asset.roadWord(road,0x84)!=1 || asset.roadWord(road,0xb4)!=0)return std::nullopt;
    Path path;
    const auto left=asset.roadFloat(road,0x48),right=asset.roadFloat(road,0x4c);
    if(!left || !right || !std::isfinite(*left) || !std::isfinite(*right) || *left<0 || *right<0)return std::nullopt;
    path.left=*left;path.right=*right;
    for(uint32_t n=0;n<6;++n) {
        const auto point=asset.pathVector(*start+n),row=asset.pathVector(*start+6+n);
        if(!point || !row)return std::nullopt;
        for(float f:{point->x,point->y,point->z,row->x,row->y,row->z})if(!std::isfinite(f))return std::nullopt;
        path.points[n]=*point;path.rows[n]=*row;
    }
    for(size_t n=0;n<5;++n) {
        const auto a=path.rows[n],b=path.rows[n+1];
        // collisionCurve collapses near normals before its equality dispatch.
        // Keep the raw rows for admission and output interpolation.
        const bool straight=(a.x==b.x && a.z==b.z) ||
            (std::fabs(double(a.x)-b.x)<.009999999776482582 &&
             std::fabs(double(a.z)-b.z)<.009999999776482582);
        if(!straight)return std::nullopt;
        const auto p=path.points[n],q=path.points[n+1];
        // Zero chords leave the source fraction register unwritten. Exact
        // zero parameter/tangent deltas make any finite fraction equivalent.
        if(p.x==q.x && p.z==q.z && (a.x!=b.x || a.y!=b.y || a.z!=b.z))return std::nullopt;
    }
    return path;
}
}
bool supportsStraightCameraPath(const CameraRoadAsset& asset,uint32_t road) {return bool(readPath(asset,road));}
CameraPathStatus solveStraightCameraPath(const CameraRoadAsset& asset,uint32_t road,
                                        CameraPoint q,CameraCurveOutput& output) {
    const auto path=readPath(asset,road);
    if(!path || !std::isfinite(q.x) || !std::isfinite(q.z))return CameraPathStatus::Unsupported;
    auto result=output;
    float low=path->rows[2].y;const float high=path->rows[3].y;
    if(std::fabs(double(high)-low)<9.999999747e-5)low=high;
    float best=100000000;bool found=false;
    for(uint32_t i=0;i<5;++i) {
        const auto a=path->points[i],b=path->points[i+1];
        const auto ta=path->rows[i],tb=path->rows[i+1];
        if(ta.x*(q.x-a.x)+ta.z*(q.z-a.z)<0 ||
           tb.x*(q.x-b.x)+tb.z*(q.z-b.z)>0)continue;
        const float hx=b.x-a.x,hz=b.z-a.z;
        CameraPoint projected=a;projected.y=0;float fraction=0;
        if(hx!=0 || hz!=0) {
            // Portable infinite-line projection; source uses a dominant-axis
            // f32/FMA solve. Never clamp the fraction to the segment.
            const float coefficient=((q.x-a.x)*hx+(q.z-a.z)*hz)/(hx*hx+hz*hz);
            projected={a.x+coefficient*hx,0,a.z+coefficient*hz};
            fraction=std::fabs(hx)>std::fabs(hz)?(projected.x-a.x)/hx:(projected.z-a.z)/hz;
        }
        if(!std::isfinite(projected.x) || !std::isfinite(projected.z) || !std::isfinite(fraction))return CameraPathStatus::Unsupported;
        const float parameter=path->rows[i].y+fraction*(path->rows[i+1].y-path->rows[i].y);
        const float dx=q.x-projected.x,dz=q.z-projected.z,d=std::sqrt(dx*dx+dz*dz);
        if(d>=best)continue;
        best=d;found=true;
        const float side=hx*dz-hz*dx,cap=side<0?path->left:path->right;
        const bool sideCap=d>cap;
        const float signedDistance=(side<0?1.f:-1.f)*(sideCap?cap:d);
        result.parameter=parameter;result.segmentStartY=a.y;
        result.flags=sideCap?2:0;result.segment=i;
        result.tangent={ta.x+fraction*(tb.x-ta.x),0,ta.z+fraction*(tb.z-ta.z)};
        if(parameter<low || parameter>high) {
            result.parameter=parameter<low?low:high;result.segment=parameter<low?2:3;result.flags|=1;
            const auto endpoint=path->points[result.segment];
            const auto tangent=path->rows[result.segment];
            result.tangent={tangent.x,0,tangent.z};
            result.anchor={endpoint.x+tangent.z*signedDistance,0,endpoint.z-tangent.x*signedDistance};
        } else {
            const float scale=sideCap && d!=0?cap/d:1;
            result.anchor={projected.x+dx*scale,0,projected.z+dz*scale};
        }
    }
    if(found)output=result;
    return found?CameraPathStatus::Found:CameraPathStatus::NoHit;
}
}

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
    const auto tangent=path.rows[0];
    if(!((std::fabs(tangent.x)==1 && tangent.z==0) || (tangent.x==0 && std::fabs(tangent.z)==1)))return std::nullopt;
    for(size_t n=0;n<6;++n) {
        if(path.rows[n].x!=tangent.x || path.rows[n].z!=tangent.z ||
           (path.points[n].x-path.points[0].x)*tangent.z-(path.points[n].z-path.points[0].z)*tangent.x!=0)return std::nullopt;
        if(n && (path.points[n].x-path.points[n-1].x)*tangent.x+(path.points[n].z-path.points[n-1].z)*tangent.z<=0)return std::nullopt;
    }
    return path;
}
}
bool supportsStraightCameraPath(const CameraRoadAsset& asset,uint32_t road) {return bool(readPath(asset,road));}
CameraPathStatus solveStraightCameraPath(const CameraRoadAsset& asset,uint32_t road,
                                        CameraPoint q,CameraCurveOutput& output) {
    const auto path=readPath(asset,road);
    if(!path || !std::isfinite(q.x) || !std::isfinite(q.z))return CameraPathStatus::Unsupported;
    const auto tangent=path->rows[0];const float nx=-tangent.z,nz=tangent.x;
    float low=path->rows[2].y;const float high=path->rows[3].y;
    if(std::fabs(double(high)-low)<9.999999747e-5)low=high;
    float best=100000000;bool found=false;
    for(uint32_t i=0;i<5;++i) {
        const auto a=path->points[i],b=path->points[i+1];
        if(tangent.x*(q.x-a.x)+tangent.z*(q.z-a.z)<0 ||
           tangent.x*(q.x-b.x)+tangent.z*(q.z-b.z)>0)continue;
        CameraPoint projected;float fraction;
        if(tangent.x!=0){projected={q.x,0,a.z};fraction=(q.x-a.x)/(b.x-a.x);}
        else {projected={a.x,0,q.z};fraction=(q.z-a.z)/(b.z-a.z);}
        const float parameter=path->rows[i].y+fraction*(path->rows[i+1].y-path->rows[i].y);
        const float dx=q.x-projected.x,dz=q.z-projected.z,d=std::sqrt(dx*dx+dz*dz);
        if(d>=best)continue;
        best=d;found=true;
        const float side=nx*dx+nz*dz,cap=side<0?path->left:path->right;
        const bool sideCap=d>cap;
        const float signedDistance=(side<0?-1.f:1.f)*(sideCap?cap:d);
        output.parameter=parameter;output.segmentStartY=a.y;
        output.flags=sideCap?2:0;output.segment=i;output.tangent={tangent.x,0,tangent.z};
        if(parameter<low || parameter>high) {
            output.parameter=parameter<low?low:high;output.segment=parameter<low?2:3;output.flags|=1;
            const auto endpoint=path->points[output.segment];
            output.anchor={endpoint.x+nx*signedDistance,0,endpoint.z+nz*signedDistance};
        } else {
            const float scale=sideCap && d!=0?cap/d:1;
            output.anchor={projected.x+dx*scale,0,projected.z+dz*scale};
        }
    }
    return found?CameraPathStatus::Found:CameraPathStatus::NoHit;
}
}

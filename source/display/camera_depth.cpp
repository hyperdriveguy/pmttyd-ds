#include "camera_depth.hpp"
#include <cmath>
namespace ttyd {
namespace {
float round32(float v){volatile float single=v;return single;}
float row(const std::array<float,4>& m,CameraDepthPoint p){
    return round32(std::fma(m[2],p[2],round32(m[0]*p[0]))+std::fma(m[3],1.f,round32(m[1]*p[1])));
}
bool normalize(CameraDepthPoint& p){
    const float length=std::sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);if(!std::isfinite(length) || length==0)return false;
    for(auto& v:p)v/=length;return true;
}
CameraDepthPoint cross(CameraDepthPoint a,CameraDepthPoint b){return {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};}
}
CameraDepthPoint cameraDepthView(const CameraDepthSnapshot& c,CameraDepthPoint p){return {row(c.view[0],p),row(c.view[1],p),row(c.view[2],p)};}
CameraDepthPoint cameraDepthProject(const CameraDepthSnapshot& c,CameraDepthPoint p){
    const float inverse=round32(1.f/row(c.projection[3],p));
    return {round32(row(c.projection[0],p)*inverse),round32(row(c.projection[1],p)*inverse),round32(row(c.projection[2],p)*inverse)};
}
float cameraQueueDepth(const CameraDepthSnapshot& c,CameraDepthPoint p){
    const auto projected=cameraDepthProject(c,cameraDepthView(c,p));
    float depth=std::fma(5000.f,projected[2],5000.f);
    if(depth<0)depth=0;else if(depth>10000)depth=10000;
    return round32(-1.f*depth);
}
std::optional<CameraDepthSnapshot> makeCameraDepthSnapshot(CameraDepthPoint eye,CameraDepthPoint at,CameraDepthPoint up,float fov,float aspect,float nearPlane,float farPlane){
    for(float v:{eye[0],eye[1],eye[2],at[0],at[1],at[2],up[0],up[1],up[2],fov,aspect,nearPlane,farPlane})if(!std::isfinite(v))return {};
    if(fov<=0 || fov>=180 || aspect<=0 || nearPlane<=0 || farPlane<=nearPlane)return {};
    CameraDepthPoint back{eye[0]-at[0],eye[1]-at[1],eye[2]-at[2]};if(!normalize(back))return {};
    auto right=cross(up,back);if(!normalize(right))return {};up=cross(back,right);
    CameraDepthSnapshot c;const std::array<CameraDepthPoint,3> axes{right,up,back};
    for(size_t i=0;i<3;++i){const auto& a=axes[i];c.view[i]={a[0],a[1],a[2],-(a[0]*eye[0]+a[1]*eye[1]+a[2]*eye[2])};}
    const float cot=1.f/std::tan(fov*.008726646192371845f),inverse=1.f/(farPlane-nearPlane);
    c.projection[0][0]=cot/aspect;c.projection[1][1]=cot;
    c.projection[2][2]=-nearPlane*inverse;c.projection[2][3]=-farPlane*nearPlane*inverse;c.projection[3][2]=-1;
    return c;
}
}

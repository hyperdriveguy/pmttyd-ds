#include "camera_depth.hpp"
#include <cmath>
namespace ttyd {
namespace {
// Explicit binary32 stores retain recovered arithmetic boundaries. Host double
// trig/normalization still adapts Gekko numerics; this is not bit-parity proof.
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
// Affine view rows3x4: roll degrees*pi/180, then translation. Keep concat
// order and row-specific FMA/store boundaries before one-use projection extra.
CameraDepthSnapshot publishCameraMatrices(CameraDepthSnapshot c,CameraMatrixPublication& state){
    const float angle=round32(state.rollDegrees*0.01745329238474369f);
    const float sine=round32(float(std::sin(double(angle))));
    const float cosine=round32(float(std::cos(double(angle))));
    const std::array<std::array<float,4>,3> rotation{{{cosine,-sine,0,0},{sine,cosine,0,0},{0,0,1,0}}};
    const std::array<std::array<float,4>,3> translation{{{1,0,0,state.translation[0]},{0,1,0,state.translation[1]},{0,0,1,state.translation[2]}}};
    auto concat=[](const auto& a,const auto& b){
        std::array<std::array<float,4>,3> out{};
        for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j){
            float v=round32(a[i][0]*b[0][j]);
            v=std::fma(a[i][1],b[1][j],v);v=std::fma(a[i][2],b[2][j],v);
            if(j>=2)v=std::fma(a[i][3],j==3?1.f:0.f,v);
            out[i][j]=round32(v);
        }
        return out;
    };
    c.view=concat(rotation,c.view);c.view=concat(translation,c.view);
    publishCameraExtra(c.projection,state);
    return c;
}
// Flag mask8 consumes extra4x4 * existing projection once, then clears.
// Copy prior projection so in-place rows do not read already-updated elements.
void publishCameraExtra(CameraProjectionMatrix& projection,CameraMatrixPublication& state){
    if(state.flags&8){
        const auto previous=projection;
        for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j){
            float v=round32(state.extra[i][0]*previous[0][j]);
            for(unsigned k=1;k<4;++k)v=std::fma(state.extra[i][k],previous[k][j],v);
            projection[i][j]=round32(v);
        }
        state.flags=uint16_t(state.flags&~8u);
    }
}
CameraDepthPoint cameraDepthView(const CameraDepthSnapshot& c,CameraDepthPoint p){return {row(c.view[0],p),row(c.view[1],p),row(c.view[2],p)};}
// Homogeneous divide uses projection row3 W; no guessed W-zero clamp.
// Caller/domain providers must establish valid projection inputs.
CameraDepthPoint cameraDepthProject(const CameraDepthSnapshot& c,CameraDepthPoint p){
    const float inverse=round32(1.f/row(c.projection[3],p));
    return {round32(row(c.projection[0],p)*inverse),round32(row(c.projection[1],p)*inverse),round32(row(c.projection[2],p)*inverse)};
}
// Source normalized Z becomes5000*z+5000, clamps0..10000, then negates
// for queue depth. Do not use unprojected view Z or discard perspective divide.
float cameraQueueDepth(const CameraDepthSnapshot& c,CameraDepthPoint p){
    const auto projected=cameraDepthProject(c,cameraDepthView(c,p));
    float depth=std::fma(5000.f,projected[2],5000.f);
    if(depth<0)depth=0;else if(depth>10000)depth=10000;
    return round32(-1.f*depth);
}
// Degrees/2 -> radians; cotangent sets X/Y scale. Source GX depth terms
// use near/far span and W=-viewZ; do not substitute the usual OpenGL matrix.
CameraProjectionMatrix cameraPerspective(float fov,float aspect,float nearPlane,float farPlane){
    CameraProjectionMatrix p{};
    const float angle=round32(0.01745329238474369f*round32(.5f*fov));
    const float cot=round32(float(1./std::tan(double(angle))));
    const float inverse=round32(1.f/round32(farPlane-nearPlane));
    p[0][0]=round32(cot/aspect);p[1][1]=cot;
    p[2][2]=round32(-nearPlane*inverse);
    p[2][3]=round32(inverse*-round32(farPlane*nearPlane));p[3][2]=-1;
    return p;
}
// Bounds map width/height to normalized[-1,1] via2/span. Source depth
// is -1/(far-near), translation -far/(far-near), homogeneous W1.
CameraProjectionMatrix cameraOrthographic(float top,float bottom,float left,float right,float nearPlane,float farPlane){
    CameraProjectionMatrix p{};
    const float width=round32(right-left),height=round32(top-bottom),xSum=round32(right+left);
    const float xInverse=round32(1.f/width),yInverse=round32(1.f/height);
    p[0][0]=round32(2.f*xInverse);p[0][3]=round32(xInverse*-xSum);
    const float ySum=round32(top+bottom),zSpan=round32(farPlane-nearPlane),zInverse=round32(1.f/zSpan);
    p[1][1]=round32(2.f*yInverse);p[1][3]=round32(yInverse*-ySum);
    p[2][2]=round32(-1.f*zInverse);p[2][3]=round32(-farPlane*zInverse);p[3][3]=1;
    return p;
}
// Recovered C_MTXFrustum: single operations, GX near/span depth (not OpenGL).
CameraProjectionMatrix cameraFrustum(float top,float bottom,float left,float right,float nearPlane,float farPlane){
    CameraProjectionMatrix p{};
    const float xInverse=round32(1.f/round32(right-left));
    const float twiceNear=round32(2.f*nearPlane);
    const float yInverse=round32(1.f/round32(top-bottom));
    p[0][0]=round32(twiceNear*xInverse);p[0][2]=round32(xInverse*round32(right+left));
    p[1][1]=round32(twiceNear*yInverse);p[1][2]=round32(yInverse*round32(top+bottom));
    const float zInverse=round32(1.f/round32(farPlane-nearPlane));
    p[2][2]=round32(-nearPlane*zInverse);
    p[2][3]=round32(zInverse*-round32(farPlane*nearPlane));p[3][2]=-1.f;
    return p;
}
std::optional<CameraViewMatrix> cameraLookAtAdapter(CameraDepthPoint eye,CameraDepthPoint at,CameraDepthPoint up){
    for(float v:{eye[0],eye[1],eye[2],at[0],at[1],at[2],up[0],up[1],up[2]})if(!std::isfinite(v))return {};
    CameraDepthPoint back{eye[0]-at[0],eye[1]-at[1],eye[2]-at[2]};if(!normalize(back))return {};
    auto right=cross(up,back);if(!normalize(right))return {};up=cross(back,right);
    CameraViewMatrix view{};const std::array<CameraDepthPoint,3> axes{right,up,back};
    for(size_t i=0;i<3;++i){const auto& a=axes[i];view[i]={a[0],a[1],a[2],-(a[0]*eye[0]+a[1]*eye[1]+a[2]*eye[2])};}
    return view;
}
std::optional<CameraDepthSnapshot> makeCameraDepthSnapshot(CameraDepthPoint eye,CameraDepthPoint at,CameraDepthPoint up,float fov,float aspect,float nearPlane,float farPlane){
    for(float v:{fov,aspect,nearPlane,farPlane})if(!std::isfinite(v))return {};
    if(fov<=0 || fov>=180 || aspect<=0 || nearPlane<=0 || farPlane<=nearPlane)return {};
    const auto view=cameraLookAtAdapter(eye,at,up);if(!view)return {};
    CameraDepthSnapshot c{*view,cameraPerspective(fov,aspect,nearPlane,farPlane)};
    CameraMatrixPublication nativeUpright; // Explicit adapter; no inherited roll/extra provider.
    return publishCameraMatrices(c,nativeUpright);
}
}

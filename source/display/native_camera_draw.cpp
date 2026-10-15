#include "native_camera_draw.hpp"
#include <cmath>
namespace ttyd {
// Source raster is 608x480; rotated native target is 240x400. Convert all
// four endpoints with axis reversal, not just width/height. The 1e-7 epsilon
// admits only effectively integral physical endpoints; it is native policy.
std::optional<NativeCameraRect> nativeCameraRect(float x,float y,float w,float h){
    for(float v:{x,y,w,h})if(!std::isfinite(v))return {};
    if(x<0 || y<0 || w<=0 || h<=0 || double(x)+w>608 || double(y)+h>480)return {};
    const double edges[]{(480.-y-h)*240./480.,(608.-x-w)*400./608.,
        (480.-y)*240./480.,(608.-x)*400./608.};
    uint32_t n[4];
    for(unsigned i=0;i<4;++i){const double rounded=std::round(edges[i]);
        if(std::abs(edges[i]-rounded)>1.e-7)return {};n[i]=uint32_t(rounded);}
    if(n[2]<=n[0] || n[3]<=n[1])return {};
    return NativeCameraRect{n[0],n[1],n[2]-n[0],n[3]-n[1]};
}
// Separate nearest-endpoint adapter for source shadow rectangles. Unlike the
// exact converter, this accepts fractional physical edges if the rounded area
// stays nonempty. Both reject rectangles outside the source screen.
std::optional<NativeCameraRect> nativeCameraRoundedRect(float x,float y,float w,float h){
    for(float v:{x,y,w,h})if(!std::isfinite(v))return {};
    if(x<0 || y<0 || w<=0 || h<=0 || double(x)+w>608 || double(y)+h>480)return {};
    const uint32_t x0=uint32_t(std::round((480.-y-h)*.5)),x1=uint32_t(std::round((480.-y)*.5));
    const uint32_t y0=uint32_t(std::round((608.-x-w)*400./608.)),y1=uint32_t(std::round((608.-x)*400./608.));
    if(x1<=x0 || y1<=y0)return {};return NativeCameraRect{x0,y0,x1-x0,y1-y0};
}
// GX compact projection has seven floats: type, xx, x-offset, yy, y-offset, zz, zw.
// Kind 0 perspective uses column 2 offsets/W=-Z; kind 1 ortho column 3/W=1.
// Reconstruction intentionally discards components outside that source ABI.
std::array<float,7> cameraGxProjectionVector(const CameraProjectionMatrix& p,uint32_t kind){
    const bool ortho=kind==1;
    return {kind?1.f:0.f,p[0][0],p[0][ortho?3:2],p[1][1],p[1][ortho?3:2],p[2][2],p[2][3]};
}
CameraProjectionMatrix cameraGxProjectionMatrix(const std::array<float,7>& v){
    CameraProjectionMatrix p{};const bool ortho=v[0]!=0;
    p[0][0]=v[1];p[0][ortho?3:2]=v[2];p[1][1]=v[3];p[1][ortho?3:2]=v[4];p[2][2]=v[5];p[2][3]=v[6];
    if(ortho)p[3][3]=1;else p[3][2]=-1;return p;
}
std::optional<std::array<float,7>> NativeCameraDraw::projectionVector()const{
    return projection_?std::optional<std::array<float,7>>(cameraGxProjectionVector(*projection_,projectionKind_)):std::nullopt;
}
bool NativeCameraDraw::projectionVector(const std::array<float,7>& v,const NativeCameraSubmit& s){
    return projection(cameraGxProjectionMatrix(v),s,v[0]!=0);
}
// Multiply4x4 projection by affine3x4 view (implicit last row 0, 0, 0, 1),
// then rotate clip axes: native X=source Y, native Y=-source X. Depth/W
// retain the projection values; this helper does not convert world units.
CameraProjectionMatrix nativeCameraTransform(const CameraProjectionMatrix& p,const CameraViewMatrix& v){
    CameraProjectionMatrix combined{},tilted{};
    for(unsigned i=0;i<4;++i)for(unsigned j=0;j<4;++j){
        for(unsigned k=0;k<3;++k)combined[i][j]+=p[i][k]*v[k][j];
        if(j==3)combined[i][j]+=p[i][3];
    }
    for(unsigned j=0;j<4;++j){tilted[0][j]=combined[1][j];tilted[1][j]=-combined[0][j];
        tilted[2][j]=combined[2][j];tilted[3][j]=combined[3][j];}
    return tilted;
}
// Six source lanes are x, y, width, height, nearDepth, farDepth. PICA reversed
// depth maps with scale=near-far and offset=1-far. Publish logical and packet
// state only after the submit callback acknowledges the complete candidate.
bool NativeCameraDraw::viewport(std::array<float,6> v,const NativeCameraSubmit& s,bool rounded){
    const auto rect=rounded?nativeCameraRoundedRect(v[0],v[1],v[2],v[3]):nativeCameraRect(v[0],v[1],v[2],v[3]);
    if(!s.submit || !rect || !std::isfinite(v[4]) || !std::isfinite(v[5]) || v[4]<0 || v[5]>1 || v[4]>v[5])return false;
    NativeCameraPacket packet{};packet.kind=NativeCameraCommand::Viewport;packet.rect=*rect;
    packet.depthScale=v[4]-v[5];packet.depthOffset=1-v[5];
    if(!s.submit(s.user,packet))return false;viewport_=packet;logicalViewport_=v;return true;
}
bool NativeCameraDraw::scissor(std::array<uint32_t,4> v,const NativeCameraSubmit& s,bool rounded){
    // Avoid lossy uint32->float publication outside the source screen domain.
    if(v[0]>608 || v[1]>480 || v[2]>608 || v[3]>480)return false;
    const auto rect=rounded?nativeCameraRoundedRect(v[0],v[1],v[2],v[3]):nativeCameraRect(v[0],v[1],v[2],v[3]);
    if(!s.submit || !rect)return false;
    NativeCameraPacket packet{};packet.kind=NativeCameraCommand::Scissor;packet.rect=*rect;
    if(!s.submit(s.user,packet))return false;scissor_=packet;logicalScissor_=v;return true;
}
bool NativeCameraDraw::projection(const CameraProjectionMatrix& input,const NativeCameraSubmit& s,uint32_t kind){
    if(kind>1)return false; // Explicit native source projection-kind domain.
    const auto p=cameraGxProjectionMatrix(cameraGxProjectionVector(input,kind));
    if(!s.submit)return false;
    for(const auto& row:p)for(float v:row)if(!std::isfinite(v))return false;
    NativeCameraPacket packet{};packet.kind=NativeCameraCommand::Projection;packet.transform=nativeCameraTransform(p,view_);
    if(!s.submit(s.user,packet))return false;
    projection_=p;projectionKind_=kind!=0;return true;
}
bool NativeCameraDraw::position(const CameraViewMatrix& view,const NativeCameraSubmit& s){
    if(!s.submit || !projection_)return false;
    for(const auto& row:view)for(float v:row)if(!std::isfinite(v))return false;
    NativeCameraPacket packet{};packet.kind=NativeCameraCommand::Projection;packet.transform=nativeCameraTransform(*projection_,view);
    if(!s.submit(s.user,packet))return false;view_=view;return true;
}
bool NativeCameraDraw::request(const CameraDrawRequest& r,const NativeCameraSubmit& s){
    if(!r.camera || !s.submit)return false;
    const auto& c=*r.camera;NativeCameraPacket packet{};
    switch(r.kind){
    case CameraDrawRequestKind::Viewport:return viewport(c.viewport,s);
    case CameraDrawRequestKind::Scissor:return scissor({c.scissor[0],c.scissor[1],c.scissor[2],c.scissor[3]},s);
    case CameraDrawRequestKind::Projection:return projection(c.matrices.projection,s,c.projectionKind);
    case CameraDrawRequestKind::DummyDraw:
        if(!projection_ || !viewport_ || !scissor_)return false;
        for(const auto& row:c.matrices.view)for(float v:row)if(!std::isfinite(v))return false;
        packet.kind=NativeCameraCommand::DummyDraw;packet.transform=nativeCameraTransform(*projection_,c.matrices.view);
        if(!s.submit(s.user,packet))return false;view_=c.matrices.view;dummyState_=true;return true;
    }
    return false;
}
// Replay viewport, scissor, projection, then dummy STATE. Never replay source
// dummy geometry or reset other logical material/display owners here.
bool NativeCameraDraw::restore(const NativeCameraSubmit& s) const{
    if(!s.submit)return false;
    for(const auto* p:{&viewport_,&scissor_})if(*p && !s.submit(s.user,**p))return false;
    if(projection_){NativeCameraPacket p{};p.kind=NativeCameraCommand::Projection;
        p.transform=nativeCameraTransform(*projection_,view_);if(!s.submit(s.user,p))return false;
        if(dummyState_){p.kind=NativeCameraCommand::DummyStateRestore;if(!s.submit(s.user,p))return false;}}
    return true;
}
}

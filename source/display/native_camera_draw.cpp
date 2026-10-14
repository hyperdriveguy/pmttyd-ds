#include "native_camera_draw.hpp"
#include <cmath>
namespace ttyd {
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
std::optional<NativeCameraRect> nativeCameraRoundedRect(float x,float y,float w,float h){
    for(float v:{x,y,w,h})if(!std::isfinite(v))return {};
    if(x<0 || y<0 || w<=0 || h<=0 || double(x)+w>608 || double(y)+h>480)return {};
    const uint32_t x0=uint32_t(std::round((480.-y-h)*.5)),x1=uint32_t(std::round((480.-y)*.5));
    const uint32_t y0=uint32_t(std::round((608.-x-w)*400./608.)),y1=uint32_t(std::round((608.-x)*400./608.));
    if(x1<=x0 || y1<=y0)return {};return NativeCameraRect{x0,y0,x1-x0,y1-y0};
}
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
bool NativeCameraDraw::projection(const CameraProjectionMatrix& p,const NativeCameraSubmit& s){
    if(!s.submit)return false;
    for(const auto& row:p)for(float v:row)if(!std::isfinite(v))return false;
    NativeCameraPacket packet{};packet.kind=NativeCameraCommand::Projection;packet.transform=nativeCameraTransform(p,view_);
    if(!s.submit(s.user,packet))return false;projection_=p;return true;
}
bool NativeCameraDraw::request(const CameraDrawRequest& r,const NativeCameraSubmit& s){
    if(!r.camera || !s.submit)return false;
    const auto& c=*r.camera;NativeCameraPacket packet{};
    switch(r.kind){
    case CameraDrawRequestKind::Viewport:return viewport(c.viewport,s);
    case CameraDrawRequestKind::Scissor:return scissor({c.scissor[0],c.scissor[1],c.scissor[2],c.scissor[3]},s);
    case CameraDrawRequestKind::Projection:return projection(c.matrices.projection,s);
    case CameraDrawRequestKind::DummyDraw:
        if(!projection_ || !viewport_ || !scissor_)return false;
        for(const auto& row:c.matrices.view)for(float v:row)if(!std::isfinite(v))return false;
        packet.kind=NativeCameraCommand::DummyDraw;packet.transform=nativeCameraTransform(*projection_,c.matrices.view);
        if(!s.submit(s.user,packet))return false;view_=c.matrices.view;dummyState_=true;return true;
    }
    return false;
}
bool NativeCameraDraw::restore(const NativeCameraSubmit& s) const{
    if(!s.submit)return false;
    for(const auto* p:{&viewport_,&scissor_})if(*p && !s.submit(s.user,**p))return false;
    if(projection_){NativeCameraPacket p{};p.kind=NativeCameraCommand::Projection;
        p.transform=nativeCameraTransform(*projection_,view_);if(!s.submit(s.user,p))return false;
        if(dummyState_){p.kind=NativeCameraCommand::DummyStateRestore;if(!s.submit(s.user,p))return false;}}
    return true;
}
}

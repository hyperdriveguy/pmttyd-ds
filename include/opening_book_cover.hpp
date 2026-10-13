#pragma once
#include <array>
#include <cmath>
#include <optional>
#include <cstdint>
namespace ttyd {
struct BookCoverVertex {float x,y,z,u,v;};
struct BookCoverQuad {std::array<BookCoverVertex,4> vertices;unsigned palette=0,cull=0;bool blend=false;uint8_t alpha=255;};
// Source procedural quads and local matrix order. Runtime camera8 mutation,
// SDK/Gekko bit equality and final adjust/fade provider remain separate.
inline float openingBookCameraDistance(){return 240.f/float(std::tan(0.21816612780094147));}
inline std::optional<std::array<BookCoverQuad,3>> openingBookCover(float width,float height,float scale,float angle,float offsetX=0,float offsetY=0,float fade=0){
    if(!std::isfinite(width)||!std::isfinite(height)||!std::isfinite(scale)||!std::isfinite(angle)||!std::isfinite(offsetX)||!std::isfinite(offsetY)||!std::isfinite(fade)||width<=0||height<=0||scale<=0||fade<0||fade>1)return std::nullopt;
    const float radians=-angle*.01745329238474369f,c=std::cos(radians),s=std::sin(radians),hinge=std::fma(-width,.5f,8.f);
    const std::array<BookCoverVertex,4> local{{{0,height*.5f,0,0,0},{width,height*.5f,0,1,0},{width,-height*.5f,0,1,1},{0,-height*.5f,0,0,1}}};
    std::array<BookCoverQuad,3> quads;
    for(unsigned q=0;q<3;++q){
        auto& quad=quads[q];quad.palette=q==1?1:2;quad.cull=q==2?1:2;quad.blend=q==0;quad.alpha=q==0?uint8_t(int(255.f*(1-fade))):255;
        for(unsigned i=0;i<4;++i){auto p=local[i];if(q!=1)p.u=1-p.u;
            const float x=q?c*p.x+s*p.z:p.x,z=q?-s*p.x+c*p.z:p.z;
            p.x=offsetX+scale*(hinge+x);p.y=offsetY+scale*p.y;p.z=scale*z;quad.vertices[i]=p;
        }
    }
    return quads;
}
inline std::optional<BookCoverQuad> openingBookPage(float width,float height,float scale,unsigned page,float offsetX=0,float offsetY=0,float fade=0){
    const auto cover=openingBookCover(width,height,scale,0,offsetX,offsetY,fade);if(!cover||page>4)return std::nullopt;
    auto quad=(*cover)[0];quad.palette=3+page;
    const float hinge=offsetX+(-width*.5f+8)*scale;
    for(unsigned i=0;i<4;++i){quad.vertices[i].x=hinge+(i==1||i==2?width*.95f*scale:0);quad.vertices[i].y=offsetY+(i<2?1:-1)*height*.95f*.5f*scale;quad.vertices[i].u=(i==1||i==2)?1:0;}
    return quad;
}
inline std::optional<BookCoverQuad> openingBookIllustration(float scale,unsigned page,float offsetX=0,float offsetY=0,float fade=0){
    if(!std::isfinite(scale)||scale<=0||page>4||!std::isfinite(offsetX)||!std::isfinite(offsetY)||!std::isfinite(fade)||fade<0||fade>1)return std::nullopt;
    BookCoverQuad quad;quad.palette=8+page;quad.cull=2;quad.blend=true;quad.alpha=uint8_t(int(255.f*(1-fade)));
    constexpr float left=-202.6666717529296875f*.5f,right=101.33333587646484375f;
    quad.vertices={{{offsetX+left*scale,offsetY+115*scale,0,0,0},{offsetX+right*scale,offsetY+115*scale,0,1,0},{offsetX+right*scale,offsetY-45*scale,0,1,1},{offsetX+left*scale,offsetY-45*scale,0,0,1}}};
    return quad;
}
// Native 304x240 viewport adaptation of the recovered camera8 initial lens.
// Used for winding admission; actual vertices retain XYZ and hardware perspective.
inline std::array<float,2> openingBookScreen(const BookCoverVertex& p){const float d=openingBookCameraDistance(),q=d/(d-p.z);return {200+p.x*q*.5f,120-p.y*q*.5f};}
inline bool openingBookFaceVisible(const BookCoverQuad& quad){
    const auto a=openingBookScreen(quad.vertices[0]),b=openingBookScreen(quad.vertices[1]),c=openingBookScreen(quad.vertices[2]);
    const float area=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
    return quad.cull==2?area>0:quad.cull==1?area<0:true;
}
}

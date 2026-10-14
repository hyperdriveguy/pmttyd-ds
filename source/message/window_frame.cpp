#include "window_frame.hpp"
namespace ttyd {
WindowFrameMesh type0WindowFrame(float x,float y,float w,float h,float border,std::array<uint8_t,4> color) {
    const float columns[]={x,x+border,(x+w)-border,x+w};
    const float rows[]={y,y-border,border+(y-h),y-h};
    constexpr float uv[]={0,.3333333432674408f,.6666666865348816f,1};
    constexpr unsigned corners[4][2]={{0,0},{1,0},{1,1},{0,1}};
    WindowFrameMesh mesh{};mesh.color=color;unsigned index=0;
    for(unsigned row=0;row<3;++row)for(unsigned col=0;col<3;++col)
        for(const auto& corner:corners) {
            const auto a=col+corner[0],b=row+corner[1];
            mesh.quads[index++]={columns[a],rows[b],0,uv[a],uv[b]};
        }
    return mesh;
}
WindowFrameMesh transformedType0WindowFrame(float x,float y,float w,float h,float border,
        float rotation,float scale,std::array<uint8_t,4> color) {
    const float negativeHalfWidth=(-w)*.5f,halfHeight=h*.5f;
    auto mesh=type0WindowFrame(negativeHalfWidth,halfHeight,w,h,border,color);
    mesh.centerX=x-negativeHalfWidth;mesh.centerY=y-halfHeight;
    mesh.rotationDegrees=rotation;mesh.scale=scale;return mesh;
}
std::array<WindowFramePoint,54> windowFrameTriangles(const WindowFrameMesh& mesh) {
    std::array<WindowFramePoint,54> result;unsigned p=0;
    constexpr unsigned corners[]={0,1,2,0,2,3};
    for(unsigned quad=0;quad<9;++quad)for(unsigned corner:corners)result[p++]=mesh.quads[4*quad+corner];
    return result;
}
ItemSelectionFramePlan itemSelectionFramePlan(int32_t x,int32_t y,int32_t w,int32_t h,
        int32_t decoration,float rotation,float scale,std::array<uint8_t,3> rgb,uint32_t alpha) {
    ItemSelectionFramePlan plan{};
    plan.frames[plan.count++]=transformedType0WindowFrame(float(x),float(y),float(w),float(h),20,
        rotation,scale,{rgb[0],rgb[1],rgb[2],uint8_t(alpha)});
    if(decoration==1) {
        const int32_t difference=static_cast<int32_t>(uint32_t(w)-150u);
        const int32_t tabX=static_cast<int32_t>(uint32_t(x)+uint32_t(difference/2));
        const int32_t tabY=static_cast<int32_t>(uint32_t(y)+14u);
        plan.frames[plan.count++]=transformedType0WindowFrame(float(tabX),float(tabY),150,28,8,
            rotation,scale,{0x2b,0xa4,0xff,uint8_t(alpha)});
    }
    return plan;
}
}

#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
struct WindowFramePoint {float x,y,z,u,v;};
struct WindowFrameMesh {
    std::array<WindowFramePoint,36> quads;
    std::array<uint8_t,4> color;
    float centerX=0,centerY=0,rotationDegrees=0,scale=1;
};
// Original untransformed type0 FIFO. Valid finite geometry belongs to caller;
// no clamping or omitted middle cell is introduced for narrow rectangles.
WindowFrameMesh type0WindowFrame(float x,float y,float width,float height,
                                float border,std::array<uint8_t,4> color);
// Original transformed type0 FIFO, centered locally before T(center)*Rz*S.
WindowFrameMesh transformedType0WindowFrame(float x,float y,float width,float height,
    float border,float rotationDegrees,float scale,std::array<uint8_t,4> color);
std::array<WindowFramePoint,54> windowFrameTriangles(const WindowFrameMesh&);
struct ItemSelectionFramePlan {std::array<WindowFrameMesh,2> frames;unsigned count=0;};
// Used manager decoration0/1/2 only. Definition RGB and low8 manager alpha,
// source signed-word rectangle and separate tab transform centers are retained.
ItemSelectionFramePlan itemSelectionFramePlan(int32_t x,int32_t y,int32_t width,int32_t height,
    int32_t decoration,float rotationDegrees,float scale,std::array<uint8_t,3> rgb,uint32_t alpha);
}

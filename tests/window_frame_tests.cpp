#include "window_frame.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {uint32_t bits(float x){uint32_t b;std::memcpy(&b,&x,4);return b;}}
int main() {
    const auto frame=transformedType0WindowFrame(-24,116,300,72,20,30,.5f,{255,255,255,128});
    assert(frame.centerX==126 && frame.centerY==80 && frame.rotationDegrees==30 && frame.scale==.5f);
    // Independent source FIFO grid: all36 XYZUV writes, including middle quad.
    const float x[]={-150,-130,130,150},y[]={36,16,-16,-36};
    const uint32_t uv[]={0,0x3eaaaaab,0x3f2aaaab,0x3f800000};
    constexpr unsigned corners[4][2]={{0,0},{1,0},{1,1},{0,1}};
    for(unsigned cell=0;cell<9;++cell)for(unsigned corner=0;corner<4;++corner) {
        const auto col=cell%3+corners[corner][0],row=cell/3+corners[corner][1];const auto& p=frame.quads[4*cell+corner];
        assert(p.x==x[col] && p.y==y[row] && p.z==0 && bits(p.u)==uv[col] && bits(p.v)==uv[row]);
    }
    const auto triangles=windowFrameTriangles(frame);
    constexpr unsigned index[]={0,1,2,0,2,3};
    for(unsigned cell=0;cell<9;++cell)for(unsigned corner=0;corner<6;++corner) {
        const auto& q=frame.quads[4*cell+index[corner]];const auto& t=triangles[6*cell+corner];
        assert(t.x==q.x && t.y==q.y && t.z==q.z && t.u==q.u && t.v==q.v);
    }
    const auto plan=itemSelectionFramePlan(-24,116,300,72,1,30,.5f,{255,254,253},0x1234);
    assert(plan.count==2 && plan.frames[0].color[0]==255 && plan.frames[0].color[1]==254 && plan.frames[0].color[2]==253 && plan.frames[0].color[3]==0x34);
    const auto& tab=plan.frames[1];assert(tab.centerX==126 && tab.centerY==116);
    assert((tab.color==std::array<uint8_t,4>{0x2b,0xa4,0xff,0x34}) && tab.rotationDegrees==30 && tab.scale==.5f);
    assert(tab.quads[0].x==-75 && tab.quads[0].y==14 && tab.quads[1].x==-67);
    for(int decoration:{0,2})assert(itemSelectionFramePlan(0,0,300,72,decoration,0,1,{1,2,3},255).count==1);
    // Signed odd tab centering truncates toward zero, including negative deltas.
    assert(itemSelectionFramePlan(10,20,149,72,1,0,1,{255,255,255},255).frames[1].centerX==85);
    assert(itemSelectionFramePlan(10,20,147,72,1,0,1,{255,255,255},255).frames[1].centerX==84);
    assert(itemSelectionFramePlan(10,20,153,72,1,0,1,{255,255,255},255).frames[1].centerX==86);
    const auto absolute=type0WindowFrame(-76.8999939f,21,165.7999878f,24.1999989f,10,{255,255,255,128});
    assert(absolute.centerX==0 && absolute.centerY==0 && absolute.rotationDegrees==0 && absolute.scale==1);
    assert(absolute.quads[0].x==-76.8999939f && absolute.quads[0].y==21 && absolute.quads[35].y==21-24.1999989f);
    const auto narrow=type0WindowFrame(0,0,20,20,20,{255,255,255,255});
    assert(narrow.quads[1].x==20 && narrow.quads[5].x==0); // No native width/border clamp.
    std::cout<<"Original window FIFO, triangulation, separate frame/tab centers and low8 alpha passed\n";
}

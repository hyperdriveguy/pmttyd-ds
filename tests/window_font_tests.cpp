#include "window_font.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    MessageFont font;font.mapping.fill(0xffff);font.mapping['A']=0;font.mapping['B']=1;font.mapping[' ']=2;
    font.glyphs={{12,2,10,22,{0,0}},{40,2,7,22,{0,0}},{0,2,9,22,{0,0}}};
    WindowFontMeasurement measured;
    assert(measurePlainWindowFont(font,"AB\nA",2,measured));
    assert(measured.width==19 && measured.lineFeeds==1 && measured.glyphs==3);
    assert(measurePlainWindowFont(font,"",2,measured) && measured.width==0 && !measured.glyphs);
    measured.width=123;assert(!measurePlainWindowFont(font,"A<red>B",2,measured) && measured.width==123);
    font.mapping['C']=3;assert(!measurePlainWindowFont(font,"C",2,measured));
    struct Probe {std::vector<WindowFontQuad> quads;unsigned random=0;std::vector<char> order;} probe;
    WindowFontDrawServices service{&probe,[](void* p,const WindowFontQuad& q){auto& v=*static_cast<Probe*>(p);v.quads.push_back(q);v.order.push_back('q');},
        [](void* p,uint32_t bound){assert(bound==64);auto& v=*static_cast<Probe*>(p);++v.random;v.order.push_back('r');return 0u;}};
    WindowFontStyle style{0xffffffff,0x1e1414ff,2,128,true};
    assert(drawPlainWindowFont(font,"AB\nA",WindowFontConsumer::StringMatrix,style,service));
    assert(probe.quads.size()==15 && probe.random==30);
    assert(probe.quads[0].left==-2 && probe.quads[1].left==2 && probe.quads[2].top==-4 && probe.quads[3].top==0);
    assert(probe.quads[0].rgba==0x1e141480 && probe.quads[4].rgba==0xffffff80);
    assert(probe.quads[4].left==0 && probe.quads[4].right==10 && probe.quads[4].top==-2 && probe.quads[4].bottom==-24);
    assert(probe.quads[4].u0==12 && probe.quads[4].u1==22 && probe.quads[4].v0==2 && probe.quads[4].v1==24);
    assert(probe.quads[9].left==12 && probe.quads[14].left==0 && probe.quads[14].top==-30);
    for(size_t i=0;i<probe.order.size();i+=3)assert(probe.order[i]=='r' && probe.order[i+1]=='r' && probe.order[i+2]=='q');
    probe={};style.edge=false;
    assert(drawPlainWindowFont(font,"AB\nA",WindowFontConsumer::MessageMatrix,style,service));
    assert(probe.quads[1].left==0 && probe.quads[1].originX==12 && probe.quads[2].originY==-28);
    probe={};assert(drawPlainWindowFont(font,"AB\nA",WindowFontConsumer::MessageDirect,style,service,-210,-144));
    assert(probe.quads[0].left==-210 && probe.quads[0].top==-146 && probe.quads[1].left==-198 && probe.quads[2].top==-174);
    probe={};assert(!drawPlainWindowFont(font,"A<icon 1>",WindowFontConsumer::MessageDirect,style,service));assert(probe.random==0 && probe.quads.empty());
    // Original key-item capacity permits121 duplicate rows. Every source glyph
    // still executes; the native arena only needs the eight scissor-visible rows.
    unsigned visible=0;
    for(unsigned row=0;row<121;++row) {
        probe={};assert(drawPlainWindowFont(font,"AB",WindowFontConsumer::MessageMatrix,style,service));
        assert(probe.random==4 && probe.quads.size()==2);
        for(const auto& q:probe.quads)if(windowFontQuadIntersects(q,{36,84-24.f*row,1,1},{-24,82,276,-108}))++visible;
    }
    assert(visible==16);
    std::cout<<"Original US plain window-font widths, consumer pen domains, edge order, alpha and quad/RNG requests passed\n";
}

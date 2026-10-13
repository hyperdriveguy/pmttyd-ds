#include "message_selection.hpp"
#include "message_icons.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    MessageFont font;font.glyphs.resize(256);for(auto& g:font.glyphs){g.width=10;g.height=20;}
    for(unsigned i=0;i<256;++i)font.mapping[i]=i;
    MessageSelection s;assert(s.open("choice","<select 1 1 0 40>\nFirst\nSecond",font,0));
    assert(s.count==2 && s.current==1 && s.cancel==1);
    assert(s.message.choiceWidth==152 && s.message.geometry.x==-76 && s.message.geometry.y==40 && s.message.geometry.height==112);
    for(unsigned i=0;i<10;++i)s.tick({0x100,0,0,0});assert(s.state==5 && s.opacity==250);
    s.tick({0x100,0,0,0});assert(s.state==1 && s.opacity==255 && s.active());
    s.tick({4,0,0,0});assert(s.current==0); // Press wraps.
    s.tick({0,0,8,0});assert(s.current==0); // Repeat stops at endpoint.
    s.tick({12,0,0,0});assert(s.current==0); // Ordered opposing presses cancel.
    s.tick({0x300,0,0,0});assert(s.state==7 && s.current==1 && s.sounds.back()==0x20013);
    for(unsigned i=0;i<11;++i)s.tick({});assert(s.state==7 && s.opacity==0);
    s.tick({});assert(s.complete() && !s.message.active());
    assert(s.open("disabled","<select 0 -1 120 40>\nFirst\nSecond",font,0));
    for(unsigned i=0;i<11;++i)s.tick({});s.tick({0x300,0,0,0});assert(s.state==1 && s.current==0);
    s.tick({0x100,0,0,0});assert(s.state==7 && s.current==0 && s.sounds.back()==0x20012);
    assert(s.open("cancelminus","<select 0 -2 120 40>\nFirst\nSecond",font,0));
    for(unsigned i=0;i<11;++i)s.tick({});s.tick({0x200,0,0,0});assert(s.current==-1 && s.state==7);
    assert(loadMessageIcons("romfs/message-icons.bin"));
    const auto* cursor=messageIcon(504);assert(cursor && cursor->frames.size()==10 && cursor->loop==1);
    assert(cursor->frame(0).delay==2 && cursor->frame(18).delay==28 && &cursor->frame(46)==&cursor->frame(0));
    const auto* star=messageIcon(404);assert(star && star->loop==1 && star->frames.size()==4);
    for(const auto& frame:star->frames)assert(frame.delay==6 && frame.width==40 && frame.height==40);
    assert(&star->frame(6)==&star->frames[1] && &star->frame(24)==&star->frames[0]);
    const uint32_t widths[]={200,100,480,320,296,148,328,164},heights[]={48,24,48,32,48,24,48,24};
    for(unsigned n=0;n<8;++n){const auto* art=messageIcon(0x10000+124+n);assert(art && art->frames.size()==1 && art->loop==0 && art->frames[0].width==widths[n] && art->frames[0].height==heights[n]);}
    const auto* stick=messageIcon(129);assert(stick && stick->frames[0].width==80 && stick->frames[0].height==80);
    MessagePlayback p;assert(p.open("icon","<icon STICK 0.6 -30 45 -10>A",font,0));
    assert(p.records[0].code==0xfff8 && p.records[0].flags==129 && p.records[0].x==-6 && p.records[0].y==-48 && p.records[0].extra==45);
    assert(p.records[1].x==30 && p.records[1].schedule==25);
    std::cout<<"Original choice transitions, navigation, geometry and used icon records passed\n";
}

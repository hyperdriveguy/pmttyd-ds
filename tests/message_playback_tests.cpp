#include "message_playback.hpp"
#include "message_assets.hpp"
#include <cassert>
#include <cstdio>
#include <iostream>
using namespace ttyd;
int main(int argc,char**) {
    MessageFont font;
    if(argc>1){assert(font.load("romfs/font.pmfont"));MessageResource messages;assert(messages.load("romfs/messages/gor_00.txt"));
        for(int i=14;i<=33;++i){char key[20];std::snprintf(key,sizeof(key),"mac_0_%03d",i);MessagePlayback p;
            assert(p.open(key,messages.find(key),font,0));unsigned waits=0;
            for(uint64_t ms=0;ms<120000 && p.active();ms+=17){if(p.waiting())++waits;p.tick(ms,0x100);}
            assert(!p.active() && waits && p.rawText()==messages.find(key));
        }
        std::cout<<"20 original harbor payloads parse/play with original font and preserved bytes\n";return 0;
    }
    font.glyphs.resize(256,{0,0,10,22,{0,0}});for(unsigned i=0;i<256;++i)font.mapping[i]=i;
    MessagePlayback p;const char* raw="<dynamic 3>A\nB<k>\n<p>\n<wait 100>C<k>";
    assert(p.open("fixture",raw,font,0) && p.rawText()==raw && p.id()=="fixture");
    assert(p.records[0].code=='A' && p.records[0].flags==9 && p.records[0].value==3 && p.records[0].schedule==0);
    assert(p.records[1].code==0xfffb && p.records[1].schedule==25);
    assert(p.records[2].code=='B' && p.records[2].y==-42 && p.records[2].schedule==50);
    assert(p.records[3].code==0xfffe && p.records[3].x==0 && p.records[3].schedule==75);
    assert(p.records[4].code==0xfffd && p.records[4].y==-35 && p.records[4].schedule==175);
    assert(p.records[5].code=='C' && p.records[5].schedule==325);
    p.tick(0,0);p.tick(200,0);assert(p.state==6 && p.alpha==255);p.tick(201,0);assert(p.state==1);
    p.tick(226,0x100);assert(p.waiting() && p.elapsed==75 && !(p.flags&4));
    p.tick(227,0x100);assert(p.wait==2 && p.state==1);p.tick(228,0);assert(p.scrollTarget==35 && p.acceptedKey==1);
    for(uint64_t ms=245;ms<1000 && !p.waiting();ms+=17)p.tick(ms,0);
    assert(p.waiting() && p.acceptedKey==1 && p.scroll==35);
    const auto prior=p.id();assert(!p.open("bad","A<select 1>",font,1000) && p.id()==prior);
    assert(!p.open("bad","A<wait",font,1000));
    assert(p.open("scale","<scale 2>A\nB</scale>C",font,0));
    assert(p.records[0].scale==2 && p.records[2].y==-84 && p.records[3].scale==1);
    assert(p.open("controls","<col ff000080><scaleX 2>A\n<pos 12 999><wave>B</wave></scale></col>C",font,0));
    assert(p.records[0].code==0xfff7 && p.records[0].flags==0xff000080);
    assert(p.records[1].scale==2 && (p.records[1].flags&0x10000) && p.records[1].y==-10);
    assert(p.records[3].x==12 && p.records[3].y==-42 && (p.records[3].flags&4));
    assert(p.records[4].code==0xfff7 && p.records[4].flags==255);
    assert(p.records[5].scale==1 && !(p.records[5].flags&0x10004));
    assert(p.open("placement","<wpos -240 200 480 160>A\nB<k><p><pos 20 10>C",font,0));
    assert(p.geometry.x==-240 && p.geometry.y==200 && p.geometry.width==480 && p.geometry.height==160);
    assert(p.records[5].x==20 && p.records[5].y==-58);
    const auto geometry=p.geometry;
    assert(!p.open("bad","<wpos 0 0 -1 100>A",font,0) && p.geometry.width==geometry.width);
    assert(!p.open("bad","<col nope>A",font,0));
    assert(p.open("small","<small>A\nB",font,0) && p.windowType==1);
    assert(p.records[2].y==-32 && std::fabs(p.geometry.width-208.8f)<.001f);
    // Left/right, equality, offscreen and missing-speaker placement routes.
    assert(messageSpeakerMode(-100,0)==3 && messageSpeakerMode(100,0)==7);
    assert(messageSpeakerMode(0,0)==7 && messageSpeakerMode(300,0)==2 && messageSpeakerMode(-300,0)==2);
    const auto direct=messageWindowPlan({},0,0,0,128);
    assert(direct.textVisible && !direct.tail && direct.box.x==-280 && direct.box.y==225);
    const auto left=messageWindowPlan({},3,-100,-60,255),right=messageWindowPlan({},7,100,-60,255);
    assert(left.textVisible && left.tail && right.tail && left.tailX<right.tailX);
    assert(left.tipX>=-100 && right.tipX<=100 && left.tipY<left.tailY);
    const auto above=messageWindowPlan({},3,-100,100,255);
    assert(!above.tail); // Speaker above the source tail boundary suppresses it.
    const auto invisible=messageWindowPlan({},3,-100,-60,0);
    assert(!invisible.textVisible && invisible.box.width==10 && invisible.box.height==10);
    const auto halfway=messageWindowPlan({},3,-100,-60,128);
    assert(!halfway.textVisible && halfway.box.width>10 && halfway.box.width<560);
    const auto small=messageWindowPlan({0,0,208.8f,57.6f},7,100,20,255,1);
    assert(std::fabs(small.box.x+4.4f)<.001f && small.box.y==80 && small.textY==74);
    std::cout<<"source schedules, key waits, LF suppression, scrolling, fade and rollback passed\n";
}

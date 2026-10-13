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
    std::cout<<"source schedules, key waits, LF suppression, scrolling, fade and rollback passed\n";
}

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
        assert(messages.load("romfs/messages/gor_02.txt"));
        const char* professor[]={"066_kur","066_01","067","067_01","067_02","067_03","067_04","067_05","067_06","068_kur","069","069_01","069_02","069_03","070_kur","071","072_kur","073","073_01","074_kur","075","076_kur","077","078_kur","079","080","081","081_01_kur","082"};
        for(const auto* suffix:professor) {
            const std::string key=std::string("mac_0_")+suffix;MessagePlayback p;
            assert(p.open(key.c_str(),messages.find(key.c_str()),font,0));
            for(uint64_t ms=0;ms<120000 && p.active();ms+=17)p.tick(ms,0x100);
            assert(!p.active() && p.rawText()==messages.find(key.c_str()));
        }
        std::cout<<"20 harbor and 29 professor original payloads parse/play with original font and preserved bytes\n";return 0;
    }
    font.glyphs.resize(256,{0,0,10,22,{0,0}});for(unsigned i=0;i<256;++i)font.mapping[i]=i;
    MessagePlayback gated;assert(gated.open("gate","<dkey><wait 350>A</dkey>B",font,0));
    assert(gated.records[0].code==0xfff5 && gated.records[0].flags==1 && gated.records[0].schedule==0);
    assert(gated.records[2].code==0xfff5 && gated.records[2].flags==0 && gated.records[2].schedule==375);
    gated.tick(0,0);gated.tick(201,0);gated.tick(202,0x100);
    assert(gated.cursor==1 && gated.elapsed==0 && (gated.flags&0x80) && !(gated.flags&4));
    gated.tick(203,0x100);assert(gated.elapsed==1 && gated.cursor==1);
    gated.tick(552,0x100);assert(gated.elapsed==350 && gated.cursor==2 && (gated.flags&0x80));
    gated.tick(577,0x100);assert(gated.elapsed==375 && !(gated.flags&0x80));
    MessagePlayback talk;assert(talk.open("talk","A<wait 1000>B<o>",font,0));
    talk.tick(0,0);talk.tick(201,0);talk.tick(202,0);
    assert(talk.talkCountdown==15 && !talk.talking && !talk.talkLatched);
    talk.tick(203,0);assert(talk.talking && talk.talkLatched && talk.talkCountdown==14);
    assert(talk.poseRequest==MessagePlayback::PoseRequest::Talk && talk.poseRequestSerial==1 && talk.poseRequestedAt==203);
    for(uint64_t ms=204;ms<=217;++ms)talk.tick(ms,0);
    assert(!talk.talking && !talk.talkLatched && talk.state==1 && talk.cursor==1);
    assert(talk.poseRequest==MessagePlayback::PoseRequest::Stay && talk.poseRequestSerial==2 && talk.poseRequestedAt==217);
    talk.tick(1300,0);assert(talk.stopped() && !talk.talking && talk.talkCountdown==15);
    talk.tick(1301,0);assert(talk.talking && talk.talkCountdown==14);
    for(uint64_t ms=1302;ms<=1315;++ms)talk.tick(ms,0);
    assert(talk.stopped() && !talk.talking && !talk.talkCountdown);
    MessagePlayback p;const char* raw="<dynamic 3>A\nB<k>\n<p>\n<wait 100>C<k>";
    MessagePlayback once;assert(once.open("once","A<o>B",font,0));
    for(uint64_t ms=0;ms<1000 && !once.stopped();ms+=17)once.tick(ms,0x100);
    assert(once.stopped() && once.active() && once.cursor==2 && once.elapsed==25 && !(once.flags&4));
    assert(!once.pending() && (once.windowFlags&1));
    once.tick(2000,0x300);assert(once.stopped() && once.cursor==2);
    const auto onceSerial=once.serial;const auto onceCursor=once.cursor;
    assert(once.append("<p>C<k>",font,2000));
    assert(once.serial==onceSerial && once.cursor==onceCursor && once.elapsed==25 && once.rawText()=="A<o>B<p>C<k>");
    assert(once.records[2].code=='B' && once.records[2].schedule==75 && once.state==1);
    assert(once.pending());
    MessagePlayback retained;assert(retained.open("retained","A<o>B<k>",font,0));
    for(uint64_t ms=0;ms<1000 && !retained.stopped();ms+=17)retained.tick(ms,0x100);
    const auto cursor=retained.cursor,serial=retained.serial;
    const auto elapsed=retained.elapsed,dataFlags=retained.flags;
    assert(retained.resume(2000) && retained.pending() && retained.active());
    assert(retained.cursor==cursor && retained.serial==serial && retained.elapsed==elapsed && retained.flags==dataFlags);
    for(uint64_t ms=2001;ms<3000 && !retained.waiting();ms+=17)retained.tick(ms,0x100);
    assert(retained.waiting() && retained.pending());
    const auto waitCursor=retained.cursor;
    assert(retained.resume(4000) && retained.wait==1 && retained.cursor==waitCursor);
    retained.tick(4001,0);assert(retained.waiting() && retained.pending());
    for(uint64_t ms=4002;ms<5000 && retained.active();ms+=17) {
        retained.tick(ms,0x100);
        if(retained.state==7 || retained.state==8)assert(retained.pending());
    }
    assert(retained.complete() && !retained.pending() && !retained.resume(5000));
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
    assert(left.tipX>=-100 && left.tipX<=left.tailX && right.tipX>=100 && right.tipX<=right.tailX && left.tipY<left.tailY);
    const auto above=messageWindowPlan({},3,-100,100,255);
    assert(!above.tail); // Speaker above the source tail boundary suppresses it.
    const auto invisible=messageWindowPlan({},3,-100,-60,0);
    assert(!invisible.textVisible && invisible.box.width==10 && invisible.box.height==10);
    const auto halfway=messageWindowPlan({},3,-100,-60,128);
    assert(!halfway.textVisible && halfway.box.width>10 && halfway.box.width<560);
    const auto small=messageWindowPlan({0,0,208.8f,57.6f},7,100,20,255,1);
    assert(std::fabs(small.box.x+4.4f)<.001f && small.box.y==80 && small.textY==74);
    // Enlarged effects crossing the scissor retain their visible mask fraction.
    auto crop=messageGlyphCrop(50,15,30,33,25,75);
    assert(crop.visible && crop.y==25 && crop.height==23 && crop.v0>0 && crop.v1==1);
    crop=messageGlyphCrop(395,65,20,22,25,75);
    assert(crop.visible && crop.width==5 && crop.height==10 && crop.u1==.25f && crop.v1<1);
    assert(!messageGlyphCrop(50,100,20,22,25,75).visible);
    // Steep left/right and inverted tails preserve horizontal edges. Their
    // texture centerline must point along the base-to-speaker ray, rather
    // than the perpendicular axis produced by rotating a sprite rectangle.
    for(const auto& sample:std::array<std::array<float,3>,3>{{{7,-200,-180},{3,200,-180},{15,-100,400}}}) {
        const float ax=sample[1],ay=sample[2];
        const auto plan=messageWindowPlan({},unsigned(sample[0]),ax,ay,255);
        assert(plan.tail);
        const auto baseLeft=messageTailPoint(plan,0,0),baseRight=messageTailPoint(plan,1,0);
        const auto tipLeft=messageTailPoint(plan,0,1),tipRight=messageTailPoint(plan,1,1);
        assert(baseLeft.y==baseRight.y && tipLeft.y==tipRight.y);
        assert(std::fabs((baseRight.x-baseLeft.x)-plan.tailWidth)<.001f);
        const auto base=messageTailPoint(plan,.5f,0),tip=messageTailPoint(plan,.5f,1);
        const float cross=(tip.x-base.x)*(ay-base.y)-(tip.y-base.y)*(ax-base.x);
        const float dot=(tip.x-base.x)*(ax-base.x)+(tip.y-base.y)*(ay-base.y);
        assert(std::fabs(cross)<.01f && dot>0);
        const auto maskTip=messageTailPoint(plan,.5f,.75f);
        assert(std::fabs(maskTip.x-(base.x+(tip.x-base.x)*.75f))<.001f);
        auto stretched=plan;stretchMessageTail(stretched,ax,ay);
        assert(stretched.tail && stretched.tailStretched);
        assert(std::fabs(std::hypot(stretched.tipX-ax,stretched.tipY-ay)*MessageWindowScale-4)<.001f);
        for(bool shadow:{false,true}) {
            const auto visible=messageTailMaskPoint(stretched,22.5f/48,(shadow?59.5f:47.5f)/64,shadow);
            assert(std::fabs(visible.x-stretched.tipX)<.001f && std::fabs(visible.y-stretched.tipY)<.001f);
            const auto retainedBase=messageTailMaskPoint(stretched,.5f,0,shadow);
            assert(retainedBase.x==plan.tailX && retainedBase.y==plan.tailY);
        }
    }
    auto close=left;stretchMessageTail(close,close.tailX,close.tailY-1);assert(!close.tail);
    std::cout<<"source schedules, key waits, LF suppression, scrolling, fade and rollback passed\n";
}

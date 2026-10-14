#include "opening_title.hpp"
#include "actor_animation.hpp"
#include "opening_title_prompt.hpp"
#include <cassert>
#include <cstdio>
using namespace ttyd;
int main(){
    OpeningTitle title;title.begin();
    for(unsigned n=0;n<300;++n)title.tick(0x1100);
    assert(title.state==0 && !title.filesRequested);
    title.admitResources();title.tick();assert(title.state==0);
    title.admitMusic(true);title.tick();assert(title.state==1 && title.paperScale==3);
    for(unsigned n=0;n<100;++n)title.tick(0x1100);
    assert(title.state==1 && title.curtainCounter==0 && !title.filesRequested);
    title.admitCurtain(34);title.tick();assert(title.curtainCounter==1 && title.paperScale<1.5f);
    for(unsigned n=1;n<89;++n)title.tick();
    title.presented(34,0,1);assert(!title.curtainFinalPresented);
    title.tick();assert(title.curtainDriverFinished() && title.paperScale==1 && title.state==1);
    title.presented(34,0,1);title.tick();assert(title.state==2 && title.whiteAlpha()==255);
    for(unsigned n=0;n<61;++n)title.tick(0x1100);
    assert(title.state==3 && title.counter==0 && !title.filesRequested);
    assert(title.whiteDriverFinished && title.whiteAlpha()==0);
    assert(title.logoY==120 && title.copyrightY==-170);
    for(unsigned n=0;n<60;++n)title.tick(0x1100);
    assert(!title.inputReady() && !title.filesRequested);
    title.tick(0x200);assert(title.inputReady() && !title.filesRequested);
    title.tick(0x100);assert(title.filesRequested && title.state==10 && !title.complete);
    title.tick();assert(title.curtainType==63 && title.state==11);
    title.admitCurtain(63);for(unsigned n=0;n<80;++n)title.tick();
    assert(title.curtainDriverFinished() && !title.complete && title.state==11);
    title.presented(63,79,1);assert(!title.curtainFinalPresented);
    title.presented(63,80,1);title.tick();assert(title.curtainType==26 && title.state==12);
    title.admitCurtain(26);for(unsigned n=0;n<60;++n)title.tick();
    assert(!title.complete);title.presented(26,60,1);title.tick();assert(title.complete);
    // Source idle counter starts only after input eligibility; A wins before
    // counter increment at the return boundary. Return waits rendered black.
    OpeningTitle idle;idle.begin();idle.admitMusic(true);idle.admitResources();
    for(unsigned n=0;n<300 && !idle.inputReady();++n){idle.admitCurtain(idle.curtainType);idle.tick();idle.presented(idle.curtainType,idle.curtainFrame(),idle.paperScale);}
    assert(idle.inputReady());const auto initialIdle=idle.idleCounter;
    for(unsigned n=initialIdle;n<1140;++n)idle.tick();
    assert(idle.state==3 && idle.idleCounter==1140);
    auto press=idle;press.tick(0x1000);assert(press.filesRequested && !press.demoRequested);
    idle.tick();assert(idle.state==20 && idle.demoRequested && !idle.complete);
    idle.tick();idle.admitCurtain(33);assert(idle.state==21 && idle.blackAlpha()==0);
    for(unsigned n=0;n<61;++n)idle.tick();
    assert(idle.blackDriverFinished && !idle.complete && idle.curtainCounter<160);
    idle.presentedBlack(254);assert(!idle.complete);idle.presentedBlack(255);idle.tick();assert(idle.complete);
    assert(idle.logoY==400 && idle.copyrightY==-400 && idle.promptAmplitude==0);
    const auto source=titlePromptSourcePoint(0,0);assert(source[0]==-144 && source[1]==-106);
    const auto uv=titlePromptPatternPoint(100,1,1);assert(uv[0]==.5f && uv[1]==1);
        ActorAnimationPack center,sides;
    assert(center.load("romfs/title-curtain-center.pmanim") && sides.load("romfs/title-curtain-sides.pmanim"));
    for(const auto* pose:{center.pose("Z_2"),center.pose("A_2"),sides.pose("A_1"),sides.pose("Z_1")}){
        assert(pose && !pose->loop);ActorAnimationSample sample;
        assert(pose->evaluate(pose->end,sample) && !sample.vertices.empty() && sample.colors.size()==sample.vertices.size());
        for(const auto& batch:sample.batches)assert(batch.count && batch.start+batch.count<=sample.vertices.size() && batch.pass<=2 && batch.cull<=3);
        std::printf("%s %.0f..%.0f %zu keys, %zu original bindings, %zu rendered vertices\n",pose->name.c_str(),pose->start,pose->end,pose->keys.size(),pose->bindings.size(),sample.vertices.size());
    }
}

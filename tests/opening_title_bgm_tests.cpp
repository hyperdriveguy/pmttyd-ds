#include "opening_title_bgm.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    OpeningBgmScene demo;demo.begin(0);OpeningTitleBgm title;
    title.begin(demo.snapshot());auto snapshot=title.snapshot();
    assert(snapshot.tracks[0]==4 && snapshot.volumes[0]==88 && snapshot.tracks[1]==5 && snapshot.volumes[1]==90);
    for(unsigned n=0;n<90;++n)title.tick();assert(title.snapshot().volumes[1]==45);
    for(unsigned n=90;n<180;++n)title.tick();assert(title.snapshot().tracks[1]==-1);
    title.offForIdle();assert(title.snapshot().volumes[0]==88);
    for(unsigned n=0;n<30;++n)title.tick();assert(title.snapshot().volumes[0]==44);
    for(unsigned n=30;n<60;++n)title.tick();assert(title.snapshot().volumes[0]==0);
    demo.begin(0);demo.transition();demo.advance(200);title.begin(demo.snapshot());
    // Original repeated off state2 does not restart a currently-running fade.
    assert(title.snapshot().volumes[1]==40);
    for(unsigned n=0;n<80;++n)title.tick();assert(title.snapshot().volumes[1]==20);
    for(unsigned n=80;n<160;++n)title.tick();assert(title.snapshot().tracks[1]==-1);
    // Original market request can occur during demo white exit. All three
    // actual owners retain their independent source integer/float envelopes.
    demo.begin(0);demo.transition();demo.advance(241);title.begin(demo.snapshot());snapshot=title.snapshot();
    assert(snapshot.tracks[0]==4 && snapshot.tracks[1]==6 && snapshot.tracks[2]==5);
    assert(snapshot.volumes[0]==88 && snapshot.volumes[1]==77 && snapshot.volumes[2]==29);
    for(unsigned n=0;n<30;++n)title.tick();snapshot=title.snapshot();assert(snapshot.volumes[1]==64 && snapshot.volumes[2]==9);
    for(unsigned n=30;n<47;++n)title.tick();assert(title.snapshot().tracks[2]==-1);
    for(unsigned n=47;n<180;++n)title.tick();assert(title.snapshot().tracks[1]==-1);
}

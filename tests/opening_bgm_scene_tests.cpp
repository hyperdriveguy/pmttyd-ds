#include "opening_bgm_scene.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    OpeningBgmScene bgm;bgm.begin(100);
    assert(bgm.snapshot().primaryTrack==5 && bgm.snapshot().primaryVolume==90);
    bgm.advance(130);assert(bgm.snapshot().primaryVolume==90);
    bgm.advance(160);assert(bgm.snapshot().primaryVolume==90);
    bgm.advance(200);bgm.transition();auto snapshot=bgm.snapshot();
    assert(snapshot.offRequested && snapshot.sourceMode==0x201 && snapshot.offMilliseconds==6000 && snapshot.defaultOnMilliseconds==1000 && !snapshot.fadeInRequested && snapshot.replacementMilliseconds==750);
    bgm.advance(380);assert(bgm.snapshot().primaryTrack==5 && bgm.snapshot().primaryVolume==45);
    bgm.advance(439);assert(bgm.snapshot().primaryTrack==5 && bgm.snapshot().primaryVolume==30);
    bgm.advance(440);snapshot=bgm.snapshot();assert(snapshot.primaryTrack==6 && snapshot.primaryVolume==77 && snapshot.tailTrack==5 && snapshot.tailActive && snapshot.tailVolume==30);
    bgm.advance(470);snapshot=bgm.snapshot();assert(snapshot.primaryVolume==77 && snapshot.tailVolume==10 && snapshot.tailActive);
    bgm.advance(484);assert(bgm.snapshot().tailActive && bgm.snapshot().tailVolume==0);
    bgm.advance(486);assert(bgm.snapshot().tailActive && bgm.snapshot().tailVolume==0);
    bgm.advance(487);snapshot=bgm.snapshot();assert(!snapshot.tailActive && snapshot.tailTrack==-1 && snapshot.tailVolume==0);
    bgm.advance(500);assert(bgm.snapshot().primaryVolume==77);
    bgm.advance(499);assert(bgm.snapshot().primaryVolume==77);bgm.transition();assert(bgm.snapshot().primaryTrack==6);
    bgm.begin(0);assert(!bgm.snapshot().offRequested && !bgm.snapshot().tailActive && bgm.snapshot().primaryTrack==5);
}

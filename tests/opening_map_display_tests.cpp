#include "opening_map_display.hpp"
#include <cassert>
#include "opening_map_bgm.hpp"
#include "opening_map_transition.hpp"
int main(){
    ttyd::OpeningMapDisplay m;m.begin(100);m.tick(117);assert(m.poseStage==0);m.tick(118);assert(m.poseStage==1);m.tick(178);assert(m.poseStage==2);
    m.tick(268);assert(m.drawRequested && !m.resourceReady && !m.flag0);m.tick(1000);assert(m.alpha==0 && !m.complete);
    m.admitted();assert(m.alpha==2);m.tick(1126);assert(m.alpha==254 && !m.opaqueRequested);m.tick(1127);assert(m.alpha==255 && m.opaqueRequested && !m.flag0);
    m.tick(2000);assert(!m.flag0 && !m.complete);m.presented(254);m.tick(2001);assert(!m.flag0);m.presented(255);m.tick(2002);assert(m.flag0);m.tick(2061);assert(!m.complete);m.tick(2062);assert(m.complete && m.musicOffRequested && m.environmentOffRequested);
    ttyd::OpeningMapBgm b;b.begin(0);b.on(7);assert(b.snapshot().volumes[0]==80);b.off();b.advance(60);assert(b.snapshot().volumes[0]==40);b.off();b.on(8);assert(b.snapshot().tracks[1]==7 && b.snapshot().volumes[1]==40 && b.snapshot().volumes[0]==86);b.advance(61);assert(b.snapshot().volumes[1]==39);b.advance(109);assert(b.snapshot().tracks[1]==-1);
    ttyd::OpeningMapTransition t;t.begin();t.presented(49,0);assert(!t.exitComplete);t.admitted(0,79);t.captured();assert(!t.captureReady);t.admitted(0,80);t.captured();t.presented(49,0);assert(t.exitComplete);t.enter();t.advance(1000);assert(!t.entryComplete && t.entryFrame==0);t.admitEntry(1000);t.advance(1080);assert(t.entryFrame==80 && !t.entryComplete);t.presented(50,79);assert(!t.entryComplete);t.presented(50,80);assert(t.entryComplete);
    b.begin(0);b.on(9);b.off(6000);b.advance(180);assert(b.snapshot().volumes[0]==39);b.off(2000);b.advance(360);assert(b.snapshot().volumes[0]==0 && b.snapshot().tracks[0]==-1);b.on(9);assert(b.snapshot().tracks[0]==-1);b.on(9,true);assert(b.snapshot().tracks[0]==9 && b.snapshot().volumes[0]==78);
    m.begin(0);assert(!m.resourceReady && !m.flag0 && !m.complete);
}

#include "opening_voyage_scene.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace ttyd;
int main(){
 OpeningVoyageScene s;s.begin(0);assert(s.shipX==-2000 && s.position[2]==-3000 && s.yaw==0);s.advance(29);assert(s.music()==-1);s.advance(30);assert(s.music()==8 && !s.logo.active);s.advance(90);assert(s.yaw==180 && s.position[2]==-3000);s.advance(239);assert(!s.logo.active);s.advance(240);assert(s.logo.active && s.logo.state==0 && s.shipX==-1800);s.logo.admitResources();s.advance(540);assert(std::fabs(s.shipX+900)<.002f && s.logo.state==2);
 s.advance(840,0x100);assert(s.shipX==0 && !s.inputEligible && !s.logo.accepted);s.advance(841,0x200);assert(s.inputEligible && s.logo.accepted && s.acceptedTick==841);s.advance(1080);assert(!s.headingRequested);s.advance(1081);assert(s.headingRequested && s.white==0 && !s.headingReady);s.advance(1129);assert(s.white==1 && !s.headingReady);s.presentedWhite(254);s.advance(1130);assert(!s.headingReady);s.presentedWhite(255);s.advance(1131);assert(s.headingReady && s.shipX>0 && s.logo.complete);
 OpeningVoyageScene music;music.begin(0);music.advance(1140);assert(music.music()==8);music.advance(1141);assert(music.music()==9 && !music.logo.accepted);
 OpeningVoyageScene dense,sparse;dense.begin(0);sparse.begin(0);for(unsigned n=0;n<=150;++n)dense.advance(n);sparse.advance(150);assert(dense.shipX==sparse.shipX && dense.position==sparse.position && dense.parts==sparse.parts);
 std::puts("Original voyage delays, source logo B acceptance, departure and actual opaque-frame handoff passed");
}

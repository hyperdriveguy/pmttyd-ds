#include "opening_wake.hpp"
#include "opening_heading.hpp"
#include "opening_wake_effects.hpp"
#include <cassert>
#include <cmath>
#include <vector>
using namespace ttyd;
int main() {
    OpeningWake wake;wake.begin(0);wake.tick(3999);assert(!wake.ready && wake.eyelid==240 && wake.depth==50);
    wake.tick(4000);assert(wake.messageReady(0));wake.tick(100000);assert(wake.messageReady(0) && wake.depth==50);
    wake.messageComplete(0);wake.tick(101999);assert(!wake.ready);wake.tick(102000);assert(wake.messageReady(1));
    wake.messageComplete(1);wake.tick(102299);assert(wake.eyelid==240 && !wake.ready);
    wake.tick(103300);assert(std::fabs(wake.eyelid-180)<.002f);
    wake.tick(104300);assert(wake.eyelid==120);wake.tick(104400);assert(wake.eyelid==240);
    wake.tick(104500);assert(wake.eyelid==120);wake.tick(104600);assert(wake.eyelid==240);
    wake.tick(105100);assert(wake.depth==50 && wake.eyelid==240);
    wake.tick(106100);assert(wake.depth==37.5f && wake.eyelid==180);
    wake.tick(107100);assert(wake.depth==0 && wake.eyelid==0 && !wake.ready);
    wake.tick(110100);assert(wake.messageReady(2));wake.messageComplete(2);
    wake.tick(112399);assert(!wake.complete);wake.tick(112400);assert(wake.complete);
    OpeningWake dense,sparse;dense.begin(1000);sparse.begin(1000);
    for(unsigned i=1001;i<=3999;++i)dense.tick(i);sparse.tick(3999);
    assert(dense.viewShiftY==sparse.viewShiftY && dense.eyelid==sparse.eyelid);
    OpeningHeading heading;heading.begin(0);heading.tick(100000,0x1300);
    assert(!heading.complete && !heading.displayRequested);heading.entryComplete=true;heading.tick(100001,0);
    assert(heading.displayRequested);heading.tick(103000,0);assert(heading.phase==1);
    heading.tick(103001,0);heading.tick(105001,0);assert(heading.startFadeRequested && !heading.startRequested);
    heading.startFadeComplete=true;heading.tick(105002,0);assert(heading.startRequested);
    heading.tick(120000,0x1300);assert(!heading.inputReady && !heading.complete);
    heading.displayReady=heading.started=true;heading.tick(120001,0);
    heading.tick(123000,0x100);assert(!heading.exitRequested);
    heading.tick(123001,0);assert(heading.inputReady);heading.tick(123002,0x200);assert(heading.exitRequested);
    heading.tick(130000,0);assert(!heading.complete);heading.exitComplete=true;heading.tick(130001,0);assert(heading.complete);
    float weight=0;
    for(unsigned i=0;i<8;++i){auto p=openingWakeTap(i,50);weight+=p.weight;assert(std::fabs(p.u)<.0051f && std::fabs(p.v)<.0051f);auto zero=openingWakeTap(i,0);assert(zero.u==0 && zero.v==0);}
    assert(weight==1 && openingWakeTap(0,50).u==0 && openingWakeTap(0,50).v==.005f);
}

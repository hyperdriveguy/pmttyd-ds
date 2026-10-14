#include "opening_heading_display.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    OpeningHeadingDisplay d;d.begin(100);d.advance(500);assert(d.state==0 && d.backgroundAngle==0 && !d.start());
    d.admit();d.advance(501);assert(d.state==1 && d.backgroundAngle==2.5f && d.letters[0].delay==1);
    assert(d.start());d.advance(502);assert(d.letters[0].phase==0);d.advance(503);assert(d.letters[0].phase==1 && d.sparkleRequested[0]);
    d.advance(549);assert(d.letters[0].frame==46 && d.letters[0].angle==1586.25f);
    d.advance(594);assert(d.letters[0].phase==2 && d.letters[0].angle==1800);
    assert(d.letters[7].phase==0);d.advance(734);assert(d.letters[7].phase==2 && d.ringReveal==0);
    d.advance(735);assert(d.ringReveal==48 && d.ringRotation==.8f);
    OpeningHeadingDisplay sparse,dense;sparse.begin(0);sparse.admit();sparse.start();dense=sparse;
    sparse.advance(300);for(uint64_t i=0;i<=300;++i)dense.advance(i);
    assert(sparse.backgroundAngle==dense.backgroundAngle && sparse.palette==dense.palette && sparse.ringReveal==dense.ringReveal);
    for(unsigned i=0;i<8;++i)assert(sparse.letters[i].angle==dense.letters[i].angle && sparse.letters[i].phase==dense.letters[i].phase);
}

#include "opening_heading_display.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    OpeningHeadingStardust star;star.ready=true;star.constants={-.05f,.96f,.01f,.1f,6.2831854820251465f,8,1000};star.scaleCurve={60,90,100,104,106,108,109};
    assert(star.random()==16838 && star.random()==5758);star.randomState=1;
    assert(star.spawn(0,16,50));// Four draws per each of four particles.
    uint32_t oracle=1;for(unsigned i=0;i<16;++i)oracle=oracle*1103515245u+12345u;assert(star.randomState==oracle);
    for(unsigned i=0;i<4;++i)assert(star.groups[0].particles[i].life==20+int(i)*2 && star.groups[0].particles[i].frame==-1);
    const float firstY=star.groups[0].particles[0].y;star.step();assert(star.groups[0].particles[0].life==19 && star.groups[0].particles[0].frame==1 && std::fabs(star.groups[0].particles[0].scale-.6f)<.000001f);
    assert(star.groups[0].particles[0].vy==-.05f && star.groups[0].particles[0].y==firstY-.05f && star.groups[0].particles[1].frame==-1);
    for(unsigned i=0;i<30;++i)star.step();for(const auto& p:star.groups[0].particles)assert(p.frame==-1);

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

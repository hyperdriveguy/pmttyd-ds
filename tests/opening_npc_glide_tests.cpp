#include "opening_npc_glide.hpp"
#include "opening_house_scene.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
int main(){
    OpeningNpcGlide glide;assert(glide.begin({0,0,0},{100,0,0},0x1008));
    assert(glide.active && glide.result==0 && glide.duration==1.25f && glide.speed==80 && glide.height==-30);
    assert(glide.step(.625f,.625f)==0);
    assert(std::fabs(glide.position[0]-50)<.00001f && std::fabs(glide.position[1]+30)<.00001f);
    assert(glide.step(1.25f,.625f)==1 && !glide.active);
    assert(std::fabs(glide.position[0]-100)<.00001f && std::fabs(glide.position[1])<.00001f);
    OpeningNpcGlide terminal;terminal.begin({0,0,0},{100,0,0},0x1108);
    for(unsigned n=1;n<=5;++n)terminal.step(float(n)*.3f,.3f);
    // Original terminal dt -= duration-elapsed order produces an incremental
    // overshoot. A clamped endpoint would hide this source behavior.
    assert(!terminal.active && terminal.result==1 && terminal.position[0]>110 && terminal.position[0]<115);
    OpeningNpcGlide zero;assert(zero.begin({1,2,3},{1,2,3},0x1008) && !zero.active && zero.result==1);
    OpeningNpcGlide invalid;assert(!invalid.begin({std::numeric_limits<float>::infinity(),0,0},{0,0,0},0x1008));
    OpeningHouseScene dense,sparse;dense.begin(20);sparse.begin(20);
    for(uint64_t tick=21;tick<=500;++tick){dense.advance(tick);const auto previous=dense.actors[2].position;dense.advance(tick);assert(dense.actors[2].position==previous);}
    sparse.advance(500);assert(sparse.actors[2].position==dense.actors[2].position);
    assert(dense.parakarryGlide.sourceCommand==0x1008 && dense.parakarryGlide.active);
    const auto& p=dense.parakarryGlide;assert(p.position[1]<p.start[1]+(p.target[1]-p.start[1])*OpeningNpcGlide::curve11(p.elapsed/p.duration));
    uint64_t tick=500;
    while(dense.parakarryGlide.active){dense.advance(++tick);assert(tick<1000);}
    assert(dense.parakarryGlide.result==1 && dense.actors[2].yaw==0);
    dense.advance(tick);assert(dense.actors[2].yaw==0);
    dense.advance(++tick);assert(dense.actors[2].yaw==180);
    while(!dense.messageReady(0)){dense.advance(++tick);assert(tick<2000);}
    dense.messageComplete(0);dense.advance(tick);
    assert(dense.parakarryGlide.sourceCommand==0x1108 && dense.parakarryGlide.active);
    assert((dense.parakarryGlide.target==OpeningNpcGlide::Point{0,300,-100}));
    while(dense.parakarryGlide.active){dense.advance(++tick);assert(tick<3000);}
    assert(dense.actors[2].visible);
    dense.advance(++tick);assert(!dense.actors[2].visible);
}

#include "demo_chest_effects.hpp"
#include "opening_demo_scene.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
using namespace ttyd;
int main(int argc,char**){
    assert(!loadDemoChestEffectAssets("/missing-source-effect-assets"));
    if(argc<2){DemoChestEffects absent;absent.requestCrystals(0,{0,0,0});assert(absent.failed && !absent.started && absent.draw(0).empty());return 0;}
    assert(loadDemoChestEffectAssets("romfs"));DemoChestEffects fx;fx.requestCrystals(100,{1,30,450});
    assert(!fx.failed && fx.requests.size()==2 && fx.requests[0].sourceCommand==0x3994 && fx.requests[1].sourceCommand==0x39d4);
    assert(fx.crystals[0].remaining==100 && fx.crystals[1].remaining==59 && fx.draw(0).size()==3);
    assert(fx.crystals[0].age==1 && fx.crystals[0].alpha==12 && fx.crystals[1].alpha==9);
    assert(fx.crystals[1].scale==.5f*1.100000023841858f);fx.advance(100);assert(fx.crystals[1].age==1);
    fx.advance(101);assert(fx.crystals[0].alpha==26);fx.advance(130);fx.requestRadiation(130,{1,30,450});
    assert(fx.requests.size()==3 && fx.requests[2].sourceCommand==0x3a24 && fx.requests[2].type==3 && fx.requests[2].camera==5);
    assert(fx.rays[0].remaining==31 && fx.rays[15].remaining==61 && fx.rays[0].angle[0]==0 && fx.rays[0].angle[1]==-20 && fx.rays[0].age==1);
    fx.advance(134);assert(fx.rays[0].alpha==200 && fx.rays[0].remaining==27);fx.advance(135);assert(fx.rays[0].alpha==200);
    fx.advance(159);assert(fx.crystals[1].active && fx.crystals[1].remaining==0 && fx.crystals[1].alpha==0);
    fx.advance(160);assert(!fx.crystals[1].active && fx.crystals[0].active && fx.crystals[0].remaining==100);
    fx.advance(161);assert(fx.rays[0].remaining==32 && fx.rays[0].age==0 && fx.rays[0].alpha==0);
    const auto draw=fx.draw(0);assert(draw.size()==18 && draw[0].texture==109 && draw[0].count==6 && draw[2].texture==-1);
    for(const auto& d:draw)for(unsigned n=0;n<d.count;++n){const auto& v=d.vertices[n];for(float p:v.xyz)assert(std::isfinite(p));if(d.texture>=0){assert(v.uv[0]>=0 && v.uv[0]<=2 && v.uv[1]>=0 && v.uv[1]<=2);}}
    DemoChestEffects dense,sparse;dense.requestCrystals(0,{0,10,0});sparse.requestCrystals(0,{0,10,0});dense.requestRadiation(0,{0,10,0});sparse.requestRadiation(0,{0,10,0});
    for(uint64_t t=1;t<=200;++t)dense.advance(t);sparse.advance(200);assert(dense.crystals[0].alpha==sparse.crystals[0].alpha && dense.rays[8].angle==sparse.rays[8].angle);
    OpeningDemoScene scene;scene.begin(0);uint64_t t=0;
    for(unsigned n=0;n<13;++n){while(!scene.messageReady(n)){scene.advance(++t);assert(t<10000);}scene.messageComplete(n);scene.advance(t);}
    while(!scene.waitingForTitle){scene.advance(++t);assert(t<10000);}
    assert(!scene.effects.failed && scene.effects.requests.size()==3);
    const auto& requests=scene.effects.requests;assert(requests[1].tick==requests[0].tick && requests[2].tick==requests[0].tick+30);
    assert(requests[0].position[1]==scene.actors[2].position[1]+10 && requests[2].position==requests[0].position);
    const auto old=scene.effects.crystals[0].age;scene.advance(t+100);assert(scene.effects.crystals[0].age==old+100 && scene.effects.crystals[0].active && scene.effects.radiationActive);
    std::puts("Original demo effect request ownership, lifetime/fade/ray cycle, source meshes and simulation cadence passed");
}

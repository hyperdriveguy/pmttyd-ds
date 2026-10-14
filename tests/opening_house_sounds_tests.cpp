#include "opening_house_sounds.hpp"
#include "opening_house_scene.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main(int argc,char**){
    assert(!loadOpeningHouseSoundAssets("/missing-source-house-sounds"));
    OpeningHouseSounds s;s.begin(10);assert(s.requests.size()==2 && s.requests[0].control==0x110 && s.requests[0].name);
    s.startFlight(250);s.advance(429,{100.001f,80.001f,0});assert(s.requests.size()==2);
    s.advance(430,{100.001f,80.001f,0});assert(s.requests.size()==3 && s.requests.back().command==0xf38 && s.requests.back().tick==430 && s.requests.back().position[2]==-100);
    assert(s.requests.back().position[0]*1024==std::trunc(s.requests.back().position[0]*1024));
    s.advance(598,{100,80,0});assert(s.requests.size()==17 && s.requests.back().command==0xf80 && s.requests.back().tick==598);
    // WW1 stop is checked after the400ms wait, not when departure publishes.
    s.depart();s.advance(621,{100,80,0});assert(s.requests.size()==17);
    s.advance(622,{100,80,0});assert(s.requests.size()==18 && s.requests.back().command==0xfdc);
    s.advance(622+16*12,{100,80,0});assert(s.requests.size()==34 && !s.flightFinished);
    s.advance(622+17*12,{100,80,0});assert(s.flightFinished && s.requests.size()==34);
    const auto count=s.requests.size();s.advance(10000,{0,0,0});assert(s.requests.size()==count);
    OpeningHouseSounds early;early.begin(0);early.startFlight(0);early.depart();early.advance(180+14*12,{0,0,0});
    assert(early.requests.size()==17 && early.requests.back().command==0xf80); // Initial14 and first holding call still execute.
    if(argc<2)return 0;
    assert(loadOpeningHouseSoundAssets("romfs"));
    const uint32_t sampleIds[]={1176,479,840,851};
    for(unsigned i=0;i<4;++i){const auto* cue=openingHouseCue(i);assert(cue && cue->sampleId==sampleIds[i] && cue->rate==16000 && cue->frames>0);}
    OpeningHouseScene scene;scene.begin(0);uint64_t tick=0;
    while(!scene.messageReady(0)){scene.advance(++tick);assert(tick<2000);}
    const auto delivery=tick;scene.advance(tick+240);tick+=240;
    unsigned holding=0;for(const auto& r:scene.sounds.requests)if(r.command==0xf80)++holding;assert(holding>=10);
    scene.messageComplete(0);scene.advance(tick);assert(scene.sounds.departurePublished);
    for(unsigned msg=1;msg<4;++msg){while(!scene.messageReady(msg)){scene.advance(++tick);assert(tick<9000);}scene.messageComplete(msg);scene.advance(tick);}
    while(!scene.complete)scene.advance(++tick);
    unsigned initial=0,tail=0,postbox=0,opens=0,shuts=0,envOff=0,lpf=0;
    for(const auto& r:scene.sounds.requests){
        initial+=r.command==0xf38;tail+=r.command==0xfdc;postbox+=r.cue==1;opens+=r.cue==2;shuts+=r.cue==3;
        envOff+=r.command==0x1590;lpf+=r.command==0x159c;
        if(r.cue>=0){assert(r.positionResolved && r.control==0);for(float p:r.position)assert(std::isfinite(p));}
    }
    assert(initial==14 && tail==17 && postbox==2 && opens==2 && shuts==2 && envOff==1 && lpf==1 && scene.sounds.flightFinished);
    assert(delivery<1000);
}

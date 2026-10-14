#include "opening_voyage_callbacks.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main(){
    OpeningVoyageLogo logo;logo.begin(240);
    for(unsigned t=240;t<400;++t)logo.advance(t,0x1300,true);
    assert(logo.state==0 && !logo.accepted && logo.alpha==0);
    logo.admitResources();logo.advance(400,0x1300,true);
    assert(logo.state==1 && logo.counter==0 && !logo.accepted);
    logo.advance(401,0x1300,true);assert(logo.counter==2);
    const auto once=logo.counter;logo.advance(401,0x1300,true);assert(logo.counter==once);
    for(unsigned t=402;t<=490;++t)logo.advance(t,0x1300,true);
    assert(logo.counter==180 && logo.state==1 && logo.alpha<255 && logo.yOffset==0);
    for(unsigned t=491;t<495;++t)logo.advance(t,0x1300,true);
    assert(logo.state==1);logo.advance(495,0x1300,true);assert(logo.state==2 && !logo.accepted);
    logo.advance(496,0x1300,false);assert(!logo.accepted);
    auto other=logo;other.advance(497,0x400,true);assert(!other.accepted);
    for(uint32_t key:{0x100u,0x200u,0x1000u}){
        auto accepted=logo;accepted.advance(497,key,true);
        assert(accepted.accepted && accepted.acceptedTick==497 && accepted.state==3);
    }
    logo.advance(497,0x200,true);assert(logo.accepted && logo.acceptedTick==497 && logo.state==3);
    for(unsigned t=498;t<=557;++t)logo.advance(t,0,true);
    assert(logo.counter==120 && logo.state==3 && logo.alpha==0 && logo.yOffset==100);
    logo.advance(558,0,true);assert(logo.state==4 && !logo.complete);
    logo.advance(559,0,true);assert(logo.complete && !logo.active);
    auto identity=openingVoyageLocalTransform({0,0,4},-5);
    assert(identity.position[0]==0 && identity.position[1]==0 && identity.position[2]==4 && identity.rotationZ==-5);
    auto moved=openingVoyageLocalTransform({-1800,0,10},5);
    assert(std::fabs(moved.rotationZ-6.5f)<1e-5f);
    const float radians=1.5f*.01745329238474369f;
    assert(std::fabs(moved.position[0]-(-1800*std::cos(radians)-6000*std::sin(radians)))<.001f);
    assert(std::fabs(moved.position[1]-(-1800*std::sin(radians)+6000*std::cos(radians)-6000))<.001f);
    assert(moved.position[2]==10);
    auto fraction=openingVoyageLocalTransform({0,.0009f,.0009f},.0009f);
    assert(fraction.position[2]==0 && fraction.rotationZ==0);
    auto point=voyagePromptSourcePoint(0,0,288,32,0,-122);assert(point[0]==-144 && point[1]==-106);
    auto uv=voyagePromptPatternPoint(100,1,1);assert(uv[0]==.5f && uv[1]==1);
}

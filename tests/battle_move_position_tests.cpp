#include "battle_move_position.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
    MarioJumpUnit u;float distance=101,sine=1,cosine=0;
    int control=0,end=0,sets=0,stores=0,truncateCalls=0;
    float truncateInput=0,headingInputX=0,headingStartX=0;
    bool forcedConversion=false;int32_t converted=0;
    std::vector<int> order;
    std::array<int32_t,7> values{{-2,101,9,0,0,-1,0}};
    float querySpeed=0;int32_t storedValue=0,storedDestination=0;
    float* mutateDuringCos=nullptr;
    JumpMotionServices math() {
        JumpMotionServices s;s.user=this;
        s.getPosition=[](void* p,MarioJumpUnit& u) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(10);return u.visible;};
        s.setPosition=[](void* p,MarioJumpUnit& u,JumpVector position) {auto& f=*static_cast<Fixture*>(p);++f.sets;f.order.push_back(15);u.visible=position;};
        s.heading=[](void* p,float startX,float,float x,float) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(11);f.headingInputX=x;f.headingStartX=startX;return 90.f;};
        s.distance=[](void* p,float,float,float,float) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(12);return f.distance;};
        s.sin=[](void* p,double) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(13);return double(f.sine);};
        s.cos=[](void* p,double) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(14);if(f.mutateDuringCos)*f.mutateDuringCos=20;return double(f.cosine);};
        s.truncate=[](void* p,float value)->int32_t {auto& f=*static_cast<Fixture*>(p);++f.truncateCalls;f.truncateInput=value;f.order.push_back(18);
            if(f.forcedConversion)return f.converted;
            assert(std::isfinite(value) && double(value)>=-2147483648.0 && double(value)<2147483648.0);return static_cast<int32_t>(value);};
        return s;
    }
    JumpEventServices events() {
        JumpEventServices s;s.user=this;
        s.resolve=[](void* p,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(raw);assert(raw>=0 && raw<7);return f.values[raw];};
        s.resolveFloat=[](void* p,int32_t raw) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(30+raw);return f.querySpeed;};
        s.translateActor=[](void* p,int32_t actor)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(8);assert(actor==-2);return 7;};
        s.unit=[](void* p,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(9);assert(id==7);return &f.u;};
        s.store=[](void* p,int32_t raw,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(19);++f.stores;f.storedDestination=raw;f.storedValue=value;};
        return s;
    }
    MovementSoundServices sound() {
        MovementSoundServices s;s.user=this;
        s.control=[](void* p,MarioJumpUnit& u) {auto& f=*static_cast<Fixture*>(p);++f.control;f.order.push_back(16);assert(u.remaining>0);};
        s.end=[](void* p,MarioJumpUnit& u) {auto& f=*static_cast<Fixture*>(p);++f.end;f.order.push_back(17);assert(u.remaining<=0);assert(u.visible.x==u.target.x && u.visible.y==u.target.y && u.visible.z==u.target.z);};
        return s;
    }
};
}
int main() {
    Fixture f;f.u.speed=5;f.u.visible={0,7,0};f.u.remaining=88;f.u.baseline={4,5,6};
    assert(queryMoveFrame(f.u,101,0,0,f.math())==21);
    assert(f.u.speed==5 && f.u.remaining==88 && f.u.baseline.y==5 && f.u.visible.y==7);
    assert(queryMoveFrame(f.u,101,0,10,f.math())==11 && f.u.speed==5);
    f.distance=100;assert(queryMoveFrame(f.u,101,0,-5,f.math())==-20);
    f.distance=0;assert(queryMoveFrame(f.u,101,0,0,f.math())==1);
    f.distance=3;f.u.speed=-0.f;assert(queryMoveFrame(f.u,101,0,-0.f,f.math())==3);
    assert(std::signbit(f.u.speed));
    f.forcedConversion=true;f.converted=std::numeric_limits<int32_t>::max();f.distance=1e20f;
    assert(queryMoveFrame(f.u,101,0,1,f.math())==std::numeric_limits<int32_t>::min());
    f.converted=3;assert(queryMoveFrame(f.u,101,0,std::numeric_limits<float>::quiet_NaN(),f.math())==3);
    assert(std::isnan(f.truncateInput)); // NaN speed bypasses zero fallback.

    Fixture event;event.u.speed=5;const std::array<int32_t,6> query{{0,1,2,3,4,55}};
    assert(queryMoveFrameEvent(query,event.events(),event.math())==2);
    assert(event.storedDestination==55 && event.storedValue==21);
    assert(event.order==std::vector<int>({0,8,1,2,3,34,9,10,12,18,19}));
    event.order.clear();auto skip=query;skip[5]=-250000000;
    queryMoveFrameEvent(skip,event.events(),event.math());
    assert(event.stores==1 && event.order==std::vector<int>({0,8,1,2,3,34,9,10,12,18}));
    event.distance=-250000000;event.querySpeed=1;
    queryMoveFrameEvent(query,event.events(),event.math());
    assert(event.stores==2 && event.storedValue==-250000000); // Query tests destination, not computed count.

    Fixture move;move.u.speed=5;move.u.visible={0,7,0};
    move.u.phase=123;move.u.height=44;move.u.delta={9,8,7};
    move.u.soundSource[11]=0x45; // Type0 source word+8.
    MovePositionArguments a;a.destination={101,99,3};MarioJumpEvent e;
    assert(movePosition(move.u,e,a,true,move.math(),move.sound())==0);
    assert(e.phase==1 && move.u.remaining==19 && move.u.speed==5.05f);
    assert(move.u.baseline.y==7 && move.u.visible.y==7 && move.control==1 && move.end==0);
    assert(move.u.soundMode[3]==0x45 && move.u.phase==123 && move.u.height==44 && move.u.delta.y==8);
    for(int n=1;n<20;++n)assert(movePosition(move.u,e,a,false,move.math(),move.sound())==(n==19?1:0));
    assert(move.control==19 && move.end==1 && move.u.visible.x==101 && move.u.visible.y==99 && move.u.visible.z==3);
    assert(move.u.baseline.y==7 && move.u.baseline.z==0 && move.u.remaining==0);

    Fixture two;two.distance=10;two.u.visible={1,2,3};a.destination={12,22,33};a.count=2;
    MarioJumpEvent etwo;assert(movePosition(two.u,etwo,a,true,two.math(),two.sound())==0);
    assert(two.u.visible.x==6 && two.u.visible.y==2 && two.u.visible.z==3);
    assert(two.order==std::vector<int>({10,11,12,13,14,15,16}));
    two.order.clear();etwo.phase=-99;
    assert(movePosition(two.u,etwo,a,false,two.math(),two.sound())==1);
    assert(two.u.baseline.x==11 && two.u.visible.x==12 && two.u.baseline.y==2 && two.u.visible.y==22);
    assert(two.order==std::vector<int>({13,14,15,15,17}) && etwo.phase==-99);
    Fixture one;one.distance=0;one.u.speed=5;a.count=0;MarioJumpEvent eone;
    assert(movePosition(one.u,eone,a,true,one.math(),one.sound())==1);
    assert(one.u.speed==0 && one.control==0 && one.end==1 && one.sets==2);
    Fixture negative;negative.distance=10;a.count=-2;MarioJumpEvent en;
    assert(movePosition(negative.u,en,a,true,negative.math(),negative.sound())==1);
    assert(negative.u.remaining==-3 && negative.u.speed==-5 && negative.u.baseline.x==-5);
    Fixture wrap;wrap.u.remaining=std::numeric_limits<int32_t>::min();MarioJumpEvent ew;
    assert(movePosition(wrap.u,ew,a,false,wrap.math(),wrap.sound())==0);
    assert(wrap.u.remaining==std::numeric_limits<int32_t>::max() && wrap.control==1);
    Fixture noFallback;noFallback.distance=5;noFallback.u.speed=0;noFallback.forcedConversion=true;
    noFallback.converted=2;a.count=0;MarioJumpEvent enf;
    assert(movePosition(noFallback.u,enf,a,true,noFallback.math(),noFallback.sound())==0);
    assert(std::isinf(noFallback.truncateInput) && noFallback.u.speed==2.5f);

    Fixture sent;sent.u.visible={-250000000,4,5};sent.u.baseline={7,8,9};sent.u.target={10,20,30};
    sent.distance=0;a.destination={-250000000,-250000000,-250000000};a.count=1;MarioJumpEvent es;
    movePosition(sent.u,es,a,true,sent.math(),sent.sound());
    assert(sent.u.baseline.x==7 && sent.u.baseline.y==4 && sent.u.target.x==10 && sent.u.visible.y==20);
    assert(sent.headingStartX==-250000000 && sent.headingInputX==-250000000);
    assert(sent.u.direction==0); // Equal raw input/current X retains old direction.

    Fixture aliases;aliases.cosine=1;float axis=0;
    integrateMoveHorizontal(&axis,&axis,5,90,aliases.math());assert(axis==0);
    aliases.mutateDuringCos=&axis;aliases.cosine=0;
    integrateMoveHorizontal(&axis,&axis,5,90,aliases.math());assert(axis==25);
    assert(aliases.order==std::vector<int>({13,14,13,14}));

    Fixture resumed;resumed.values[4]=2;resumed.distance=10;MarioJumpEvent er;
    const std::array<int32_t,7> operands{{0,1,2,3,4,5,6}};
    assert(movePositionEvent(er,operands,true,resumed.events(),resumed.math(),resumed.sound())==0);
    assert(resumed.order==std::vector<int>({0,8,1,2,3,4,5,6,9,10,11,12,13,14,15,16}));
    resumed.order.clear();resumed.values[1]=999;resumed.values[4]=100;
    resumed.values[5]=0;resumed.values[6]=2;
    assert(movePositionEvent(er,operands,false,resumed.events(),resumed.math(),resumed.sound())==1);
    assert(resumed.order==std::vector<int>({0,8,1,2,3,4,5,6,9,13,14,15,15,17}));
    assert(resumed.u.visible.x==101 && resumed.u.soundMode[0x18]==255); // No resumed setup reload.
    std::cout<<"MovePosition tests passed\n";
}

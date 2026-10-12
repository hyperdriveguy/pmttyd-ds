#include "battle_movement_sound.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
void word(uint8_t* p,uint32_t v) {p[0]=v>>24;p[1]=v>>16;p[2]=v>>8;p[3]=v;}
uint32_t word(const uint8_t* p) {return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];}
void half(uint8_t* p,uint16_t v) {p[0]=v>>8;p[1]=v;}
uint16_t half(const uint8_t* p) {return uint16_t(p[0])<<8|p[1];}
struct Fixture {
    MarioJumpUnit u;std::vector<int> order;std::vector<uint32_t> keys;
    uint32_t returnHandle=7,positionHandle=0,offHandle=0;
    JumpVector triggered{},positioned{};bool changePosition=false;
    uint8_t* watchedRecord=nullptr;
    MovementPlaybackServices playback() {
        MovementPlaybackServices s;s.user=this;
        s.trigger=[](void* p,MarioJumpUnit& u,uint32_t key,int32_t sentinel,int32_t zero,JumpVector pos) {
            auto& f=*static_cast<Fixture*>(p);f.order.push_back(1);f.keys.push_back(key);f.triggered=pos;
            assert(sentinel==-250000000 && zero==0);if(f.changePosition)u.visible={11,22,33};return f.returnHandle;
        };
        s.unitPosition=[](void* p,MarioJumpUnit& u) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(2);return u.visible;};
        s.partPosition=[](void* p,void* part) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(3);return *static_cast<JumpVector*>(part);};
        s.position=[](void* p,uint32_t handle,JumpVector pos) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);f.positionHandle=handle;f.positioned=pos;};
        s.off=[](void* p,uint32_t handle) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(5);f.offHandle=handle;if(f.watchedRecord)assert(word(f.watchedRecord+0x18)==handle);};
        return s;
    }
};
}
int main() {
    Fixture f;auto* r=f.u.soundMode.data();r[8]=0xaa;half(r+0x12,2);half(r+0x14,3);word(r+0x18,0x1234);
    const auto empty=f.u.soundMode;
    controlMovementSound(f.u,r,nullptr,{1,2,3},f.playback());assert(f.u.soundMode==empty && f.order.empty());
    word(r,42);word(r+0x18,0xffffffff);f.changePosition=true;
    controlMovementSound(f.u,r,nullptr,{1,2,3},f.playback());
    assert(f.order==std::vector<int>({1,2,4}) && f.keys==std::vector<uint32_t>({42}));
    assert(word(r+0x18)==7 && f.positionHandle==7 && f.triggered.x==1 && f.positioned.x==11);
    f.order.clear();controlUnitMovementSound(f.u,f.playback());
    assert(f.order==std::vector<int>({2,2,4}) && f.keys.size()==1 && r[8]==0xaa);
    JumpVector part{55,66,77};f.order.clear();
    controlMovementSound(f.u,r,&part,{9,8,7},f.playback());
    assert(f.order==std::vector<int>({3,4}) && f.positioned.x==55);
    f.watchedRecord=r;f.order.clear();endUnitMovementSound(f.u,f.playback());
    assert(f.order==std::vector<int>({5}) && f.offHandle==7 && word(r+0x18)==0xffffffff);
    assert(half(r+0x12)==2 && half(r+0x14)==3 && word(r)==42);
    f.order.clear();endUnitMovementSound(f.u,f.playback());assert(f.order.empty());
    f.returnHandle=0xffffffff;f.order.clear();
    controlMovementSound(f.u,r,nullptr,{1,2,3},f.playback());
    assert(f.order==std::vector<int>({1,2,4}) && f.positionHandle==0xffffffff);
    f.order.clear();controlMovementSound(f.u,r,nullptr,{1,2,3},f.playback());
    assert(f.order==std::vector<int>({1,2,4})); // No invented failure classification.

    Fixture alternating;auto* a=alternating.u.soundMode.data();
    word(a,10);word(a+4,20);half(a+0xe,2);half(a+0x10,0);word(a+0x18,0x12345678);
    half(a+0x12,0);half(a+0x14,0);
    controlMovementSound(alternating.u,a,&part,{1,2,3},alternating.playback());
    assert(alternating.order==std::vector<int>({1}) && alternating.keys.back()==10);
    assert(half(a+0x12)==2 && half(a+0x14)==1 && word(a+0x18)==0x12345678);
    alternating.order.clear();
    for(int n=0;n<2;++n)controlUnitMovementSound(alternating.u,alternating.playback());
    assert(alternating.keys.size()==1 && half(a+0x12)==0);
    controlUnitMovementSound(alternating.u,alternating.playback());
    assert(alternating.keys.size()==2 && alternating.keys.back()==20 && half(a+0x14)==2);
    assert(alternating.order==std::vector<int>({2,2,2,1}));
    half(a+0xe,0xffff);controlMovementSound(alternating.u,a,nullptr,{},alternating.playback());
    assert(alternating.keys.back()==10 && half(a+0x12)==0xffff);
    const auto disabled=alternating.u.soundMode;alternating.order.clear();
    controlMovementSound(alternating.u,a,nullptr,{},alternating.playback());
    assert(alternating.u.soundMode==disabled && alternating.order.empty());
    half(a+0x12,0);half(a+0x14,0x7fff);half(a+0x10,0);
    controlMovementSound(alternating.u,a,nullptr,{},alternating.playback());
    assert(half(a+0x14)==0x8000 && alternating.keys.back()==20);
    half(a+0x14,0xffff);controlMovementSound(alternating.u,a,nullptr,{},alternating.playback());
    assert(half(a+0x14)==0 && alternating.keys.back()==20);
    word(a,0);half(a+0x12,0);half(a+0x14,0);half(a+0xe,0);
    alternating.order.clear();const auto keyCount=alternating.keys.size();
    controlMovementSound(alternating.u,a,nullptr,{},alternating.playback());
    assert(alternating.keys.size()==keyCount && half(a+0x14)==1 && alternating.order.empty());
    controlMovementSound(alternating.u,a,nullptr,{},alternating.playback());
    assert(alternating.keys.back()==20 && half(a+0x14)==2);

    // Compose real movement callback/loader/control/end request ownership.
    Fixture movement;movement.u.visible={1,2,3};word(movement.u.soundSource.data()+8,55);
    auto playback=movement.playback();auto sound=movementSoundAdapter(playback);
    JumpMotionServices math;math.user=&movement;
    math.getPosition=[](void*,MarioJumpUnit& u) {return u.visible;};
    math.setPosition=[](void*,MarioJumpUnit& u,JumpVector p) {u.visible=p;};
    math.heading=[](void*,float,float,float,float) {return 90.f;};
    math.distance=[](void*,float,float,float,float) {return 10.f;};
    math.sin=[](void*,double) {return 1.0;};math.cos=[](void*,double) {return 0.0;};
    MovePositionArguments args;args.destination={12,22,33};args.count=2;MarioJumpEvent event;
    assert(movePosition(movement.u,event,args,true,math,sound)==0);
    assert(movement.order==std::vector<int>({2,1,2,4}) && movement.triggered.x==6);
    assert(word(movement.u.soundMode.data()+0x18)==7 && movement.keys.back()==55);
    movement.order.clear();movement.watchedRecord=movement.u.soundMode.data();
    assert(movePosition(movement.u,event,args,false,math,sound)==1);
    assert(movement.order==std::vector<int>({5}) && movement.offHandle==7 && movement.u.visible.x==12);
    std::cout<<"movement sound tests passed\n";
}

#include "battle_jump_takeoff.hpp"
#include "battle_move_position.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    JumpVector home{4,7.9f,6},visible{};float scale=1;int8_t facing=1,belong=0;
    BattlePositionUnit actor{&home,&visible,&scale,&facing,&belong},target{&home,&visible,&scale,&facing,&belong},chosen{&home,&visible,&scale,&facing,&belong};
    BattlePositionPart targetPart,chosenPart;
    int work=1,otherWork=2,sideReads=0;uint8_t initialSide=1,laterSide=1;
    int8_t sampleCount=1;int32_t sampleResult=0,choiceReturn=-999,chosenId=30,enemySide=1;
    TakeoffSample* privateBuffer=nullptr;
    std::vector<int> order;std::vector<int32_t> destinations,outputs;
    int8_t observedBias=0;
    Fixture() {
        targetPart.attributes=chosenPart.attributes=0x10000000;
        targetPart.absolute={100.9f,300,40.9f};chosenPart.absolute={20.9f,400,80};
        targetPart.id=1;chosenPart.id=2;target.head=&targetPart;chosen.head=&chosenPart;
    }
    BattleHitPositionServices hit() {
        BattleHitPositionServices s;s.user=this;s.sideFactor=[](void* p,int8_t)->int8_t {static_cast<Fixture*>(p)->order.push_back(5);return 1;};return s;
    }
    TakeoffServices services() {
        TakeoffServices s;s.user=this;
        s.captureWork=[](void* p)->void* {auto& f=*static_cast<Fixture*>(p);f.order.push_back(1);return &f.work;};
        s.resolve=[](void* p,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(10+raw);return raw==0?-2:raw==1?20:123;};
        s.translate=[](void* p,int32_t id)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(2);return id==-2?7:id;};
        s.unit=[](void* p,void* work,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work);f.order.push_back(3);return id==7?&f.actor:id==20?&f.target:&f.chosen;};
        s.enemyBelong=[](void* p,BattlePositionUnit& u)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(&u==&f.actor);f.order.push_back(6);return f.enemySide;};
        s.sideByte=[](void* p,void* work,int8_t b) {auto& f=*static_cast<Fixture*>(p);assert(work==&f.work && b==int8_t(f.enemySide));f.order.push_back(7);return ++f.sideReads==1?f.initialSide:f.laterSide;};
        s.sample=[](void* p,TakeoffSample& buffer,const void* weapon,int32_t actor,int8_t b,uint32_t flags,uint32_t special,int8_t bias)->int32_t {
            auto& f=*static_cast<Fixture*>(p);assert(!weapon && actor==7 && b==int8_t(f.enemySide));
            assert(flags==0x01001240 && special==0x01000000);f.order.push_back(8);f.privateBuffer=&buffer;
            buffer.count=f.sampleCount;buffer.data=&f.otherWork;f.observedBias=bias;return f.sampleResult;
        };
        s.choose=[](void* p,TakeoffSample& buffer,int32_t mode,int32_t& unit,int32_t& part)->int32_t {
            auto& f=*static_cast<Fixture*>(p);assert(&buffer==f.privateBuffer && buffer.data==&f.otherWork && mode==4);f.order.push_back(9);
            if(buffer.count>0) {unit=f.chosenId;part=99;}return f.choiceReturn;
        };
        s.truncate=[](void* p,float v)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(14);return static_cast<int32_t>(v);};
        s.store=[](void* p,int32_t raw,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(15);f.destinations.push_back(raw);f.outputs.push_back(value);};return s;
    }
    void run() {assert(jumpTakeoffPositionEvent({{0,1,2,44,45,46}},services(),hit())==2);}
};
int main() {
    Fixture f;f.run();assert(f.outputs==std::vector<int32_t>({50,7,40}) && f.observedBias==-1);
    assert(f.destinations==std::vector<int32_t>({44,45,46}));
    assert(f.order==std::vector<int>({1,10,11,12,2,3,2,3,5,6,7,8,9,2,3,5,7,14,15,14,15,14,15}));
    Fixture reversed;reversed.sampleResult=1;reversed.run();assert(reversed.outputs[0]==-9);
    Fixture same;same.chosen.head=&same.targetPart;same.run();assert(same.outputs[0]==160);
    Fixture sameReverse;sameReverse.chosen.head=&sameReverse.targetPart;sameReverse.sampleResult=-1;sameReverse.run();assert(sameReverse.outputs[0]==40);
    for(int count:{0,-1,-128}) {
        Fixture absent;absent.sampleCount=count;absent.sampleResult=1;absent.run();
        assert(absent.outputs[0]==160 && absent.order.size()==20 && absent.sideReads==2);
    }
    Fixture tie;tie.targetPart.absolute.x=100;tie.chosenPart.absolute.x=-100;tie.run();assert(tie.outputs[0]==130);
    Fixture farther;farther.chosenPart.absolute.x=500;farther.run();assert(farther.outputs[0]==130);
    Fixture negative;negative.initialSide=255;negative.laterSide=255;negative.enemySide=255;negative.run();
    assert(negative.observedBias==1 && negative.outputs[0]==-9);
    Fixture changed;changed.initialSide=128;changed.laterSide=2;changed.run();
    assert(changed.observedBias==-128 && changed.outputs[0]==80); // Side reread, no ±1 clamp.
    Fixture noChoiceReturn;noChoiceReturn.choiceReturn=0;noChoiceReturn.run();assert(noChoiceReturn.outputs[0]==50);
    Fixture aliases;assert(jumpTakeoffPositionEvent({{0,1,2,-250000000,77,77}},aliases.services(),aliases.hit())==2);
    assert(aliases.destinations==std::vector<int32_t>({-250000000,77,77}) && aliases.outputs.back()==40);
    // Unordered distance comparison retains requested X rather than choosing
    // candidate X. Keep final coordinate finite so conversion is well-defined.
    Fixture unordered;unordered.visible.x=std::numeric_limits<float>::quiet_NaN();unordered.run();assert(unordered.outputs[0]==130);
    // Original takeoff's integer outputs are actual approach arguments, not a
    // fixed native approach offset; caller ownership remains explicit.
    MarioJumpUnit moving;moving.visible=f.visible;moving.speed=4;
    JumpMotionServices math;math.user=&f;
    math.getPosition=[](void*,MarioJumpUnit& u) {return u.visible;};
    math.distance=[](void* user,float currentX,float currentZ,float x,float z) {
        auto& fixture=*static_cast<Fixture*>(user);
        assert(currentX==fixture.visible.x && currentZ==fixture.visible.z);
        assert(x==fixture.outputs[0] && z==fixture.outputs[2]);return 64.f;
    };
    math.truncate=[](void*,float value)->int32_t {return static_cast<int32_t>(value);};
    assert(queryMoveFrame(moving,f.outputs[0],f.outputs[2],0,math)==16);
    assert(moving.speed==4 && moving.remaining==0);
    std::cout<<"takeoff position tests passed\n";
}

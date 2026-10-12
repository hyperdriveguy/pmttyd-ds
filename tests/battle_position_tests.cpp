#include "battle_position.hpp"
#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    JumpVector home{1,2,3},visible{10,20,30};float scale=2;int8_t facing=-1,belong=0,side=1;
    BattlePositionUnit unit{&home,&visible,&scale,&facing,&belong};
    BattlePositionPart part;std::vector<int> order;
    int32_t output=0;std::vector<int32_t> destinations,outputs;
    Fixture() {part.owner=&unit;part.id=1;unit.head=&part;}
    BattleHitPositionServices hit() {
        BattleHitPositionServices s;s.user=this;
        s.sideFactor=[](void* p,int8_t belong) {auto& f=*static_cast<Fixture*>(p);assert(belong==f.belong);f.order.push_back(5);return f.side;};return s;
    }
    BattlePositionEventServices events() {
        BattlePositionEventServices s;s.user=this;
        s.resolve=[](void* p,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(raw);return raw==0?-2:123;};
        s.translate=[](void* p,int32_t id)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(id==-2);f.order.push_back(2);return 7;};
        s.unit=[](void* p,int32_t id) {auto& f=*static_cast<Fixture*>(p);assert(id==7);f.order.push_back(3);return &f.unit;};
        s.part=[](void* p,int32_t id,int32_t part) {auto& f=*static_cast<Fixture*>(p);assert(id==7 && part==123);f.order.push_back(4);return getPositionPart(f.unit,part);};
        s.truncate=[](void* p,float v)->int32_t {auto& f=*static_cast<Fixture*>(p);f.order.push_back(6);return static_cast<int32_t>(v);};
        s.store=[](void* p,int32_t raw,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(7);f.destinations.push_back(raw);f.outputs.push_back(value);f.output=value;};return s;
    }
};
int main() {
    Fixture f;JumpVector result;
    getHomePosition(f.unit,&result.x,&result.y,&result.z);assert(result.x==1 && result.y==2 && result.z==3);
    getVisiblePosition(f.unit,&result.x,&result.y,&result.z);assert(result.x==10 && result.y==20 && result.z==30);
    float shared=0;getVisiblePosition(f.unit,&shared,&shared,&shared);assert(shared==30);
    // Output aliases affect later leaf loads; do not copy vector up front.
    getHomePosition(f.unit,&f.home.y,&result.y,&result.z);assert(result.y==1 && f.home.y==1);
    f.part.offset={4,5,6};getPartWorldPosition(f.part,&result.x,&result.y,&result.z);
    assert(result.x==6 && result.y==25 && result.z==36);
    f.part.hitOffsets={JumpVector{1,2,3},JumpVector{4,5,6}};
    getHitPosition(f.unit,f.part,&result.x,&result.y,&result.z,f.hit());
    assert(result.x==16 && result.y==39 && result.z==54); // Facing-1,side+1.
    f.side=-1;getHitPosition(f.unit,f.part,&result.x,&result.y,&result.z,f.hit());
    assert(result.x==-4 && result.y==39 && result.z==54);
    // Explicit hit unit can differ from part's retained owner.
    float otherScale=3;int8_t otherSide=0;
    BattlePositionUnit explicitUnit;explicitUnit.scale=&otherScale;explicitUnit.belong=&otherSide;
    f.side=1;getHitPosition(explicitUnit,f.part,&result.x,&result.y,&result.z,f.hit());
    assert(result.x==21 && result.y==46 && result.z==63);
    f.part.attributes=0x10000000;f.part.owner=nullptr;f.part.absolute={7,8,9};
    getPartWorldPosition(f.part,&result.x,&result.y,&result.z);assert(result.x==7 && result.y==8 && result.z==9);
    getHitPosition(f.unit,f.part,&result.x,&result.y,&result.z,f.hit());
    assert(result.x==17 && result.y==22 && result.z==27);
    // Scale is read again after each aliased output write.
    f.part.absolute={1,2,3};f.part.hitOffsets={JumpVector{2,3,4},JumpVector{5,6,7}};
    f.scale=2;getHitPosition(f.unit,f.part,&f.scale,&result.y,&result.z,f.hit());
    assert(f.scale==18 && result.y==119 && result.z==141);
    f.scale=4097;f.part.absolute={-16777216,-16777216,-16777216};
    f.part.hitOffsets={JumpVector{4097,4097,4097},JumpVector{}};
    getHitPosition(f.unit,f.part,&result.x,&result.y,&result.z,f.hit());
    assert(result.x==8192 && result.y==8192 && result.z==8193); // Z fuses the product; X/Y round it first.
    assert(getPositionPart(f.unit,-99)==&f.part && getPositionPart(f.unit,999)==&f.part);
    BattlePositionPart second;second.id=2;f.part.next=&second;
    assert(getPositionPart(f.unit,2)==&second && getPositionPart(f.unit,999)==nullptr);
    f.part.next=nullptr;
    // Full hit query occurs even with all event outputs skipped; only non-skipped
    // outputs truncate. Repeated destinations preserve ordered last-write wins.
    f.part.absolute={-1.9f,2.9f,-3.9f};f.part.hitOffsets={};f.order.clear();
    assert(hitPositionEvent({{0,1,44,-250000000,44}},f.events(),f.hit())==2);
    assert(f.order==std::vector<int>({0,2,1,3,4,5,6,7,6,7}));
    assert(f.destinations==std::vector<int32_t>({44,44}) && f.outputs==std::vector<int32_t>({-1,-3}) && f.output==-3);
    f.order.clear();hitPositionEvent({{0,1,-250000000,-250000000,-250000000}},f.events(),f.hit());
    assert(f.order==std::vector<int>({0,2,1,3,4,5}) && f.outputs.size()==2);
    // Coordinates produced by the event feed the integer motion inputs.
    f.order.clear();f.outputs.clear();hitPositionEvent({{0,1,10,11,12}},f.events(),f.hit());
    MarioJumpArguments arguments;arguments.x=f.outputs[0];arguments.y=f.outputs[1];arguments.z=f.outputs[2]+5;
    assert(arguments.x==-1 && arguments.y==2 && arguments.z==2);
    std::cout<<"battle position tests passed\n";
}

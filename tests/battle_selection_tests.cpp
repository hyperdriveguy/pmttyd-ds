#include "battle_selection.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
    uint32_t work=0x10000;DamageUnit unit,acting;
    std::map<uint32_t,uint8_t> bytes;std::map<uint32_t,uint16_t> halves;std::map<uint32_t,uint32_t> words;
    std::vector<int> order;std::vector<uint32_t> addresses,unitWork;
    std::vector<int32_t> outputs,destinations;
    uint32_t mutatePart=0;bool switchWork=false;
    BattleSelectionServices services() {
        BattleSelectionServices s;s.user=this;
        s.currentWork=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(1);return f.work;};
        s.byte=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(2);f.addresses.push_back(address);assert(f.bytes.count(address));return f.bytes[address];};
        s.half=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(3);f.addresses.push_back(address);assert(f.halves.count(address));return f.halves[address];};
        s.word=[](void* p,uint32_t address) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(4);f.addresses.push_back(address);assert(f.words.count(address));return f.words[address];};
        s.unit=[](void* p,uint32_t work,int32_t id) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(5);f.unitWork.push_back(work);assert(id==7 || id==8);return id==7?&f.unit:&f.acting;};
        s.resolve=[](void* p,int32_t raw)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(raw==-2);f.order.push_back(6);if(f.switchWork)f.work=0x20000;return raw;};
        s.translate=[](void* p,int32_t id)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(id==-2);f.order.push_back(7);return 7;};
        s.store=[](void* p,int32_t dest,int32_t value) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(8);f.destinations.push_back(dest);f.outputs.push_back(value);if(f.mutatePart)f.halves[f.mutatePart]=0xfffd;};return s;
    }
};
int main() {
    for(int8_t count:{int8_t(0),int8_t(-1),int8_t(-128)}) {
        Fixture f;f.bytes[f.work+0xe94]=uint8_t(count);
        assert(selectedTargetEvent({{55,55}},f.services())==2);
        assert(f.outputs==std::vector<int32_t>({-1,-1}) && f.order==std::vector<int>({1,2,8,8}));
    }
    Fixture target;const uint32_t buffer=target.work+0x428;
    target.bytes[buffer+0xa6c]=1;target.bytes[buffer+0xab7]=0xff;target.bytes[buffer+0xa6c]=1;
    // cursor-1 indexes the count byte itself; row index then equals1.
    const uint32_t row=buffer+4+0x24;
    target.halves[row]=0xfffe;target.halves[row+2]=99;target.mutatePart=row+2;
    selectedTargetEvent({{77,77}},target.services());
    assert(target.outputs==std::vector<int32_t>({-2,-3}) && target.destinations==std::vector<int32_t>({77,77}));
    assert(target.order==std::vector<int>({1,2,2,2,3,8,3,8}));
    Fixture negative;const auto b=negative.work+0x428;
    negative.bytes[b+0xa6c]=1;negative.bytes[b+0xab7]=0;negative.bytes[b+0xa6d]=0xff;
    negative.halves[b+4-0x24]=123;negative.halves[b+6-0x24]=0x8000;
    selectedTargetEvent({{1,2}},negative.services());assert(negative.outputs[0]==123 && negative.outputs[1]==-32768);
    const std::map<uint32_t,uint32_t> table{{0,0x4b0},{1,0x4bc},{2,0x51c},{3,0x528},{4,0x4ec},{6,0x4c8},{7,0x4d4},{8,0x4e0},{10,0x510}};
    Fixture cursors;cursors.words[cursors.work+0x420]=7;uint32_t cursor=0;
    for(const auto& entry:table) {assert(findCommandCursor(0x30000,entry.first,cursor,cursors.services()));assert(cursor==0x30000+entry.second);}
    for(int kind:{222,224,0}) {
        cursors.unit.kind=kind;
        assert(findCommandCursor(0x30000,5,cursor,cursors.services()) && cursor==0x30000+(kind==222?0x4f8:0x504));
        assert(findCommandCursor(0x30000,14,cursor,cursors.services()) && cursor==0x30000+(kind==222?0x498:0x4a4));
    }
    for(uint32_t type:{9u,11u,12u,13u,15u,0xffffffffu}) {
        cursor=0xfeedbeef;cursors.order.clear();assert(!findCommandCursor(0x30000,type,cursor,cursors.services()));
        assert(cursor==0xfeedbeef && cursors.order==std::vector<int>({1,4,5}));
    }
    for(int kind:{222,224,230}) {
        Fixture command;command.unit.kind=kind;command.switchWork=true;
        command.words[0x10000+0x1c5c]=5;command.words[0x20000+0x420]=7;
        const uint32_t menu=0x10000+0x171c;
        command.words[menu+(kind==222?0x4f8:0x504)]=0xffffffff;
        command.words[menu+0x80-28]=0x803538b0;
        assert(commandWeaponAddressEvent({{-2,-250000000}},command.services())==2);
        assert(command.outputs==std::vector<int32_t>({int32_t(-2143995728)}) && command.destinations[0]==-250000000);
        assert(command.unitWork==std::vector<uint32_t>({0x20000,0x20000}));
        assert(command.order==std::vector<int>({1,6,7,1,5,4,1,4,5,4,4,8}));
    }
    Fixture enemy;enemy.unit.kind=223;enemy.unit.heldItem=0xffffffff;
    enemy.words[0x803108a8u-40+0x24]=0;
    assert(commandWeaponAddressEvent({{-2,9}},enemy.services())==2 && enemy.outputs[0]==0);
    assert(enemy.order==std::vector<int>({1,6,7,1,5,4,8}));
    Fixture differing;differing.unit.kind=222;differing.acting.kind=225;
    differing.words[differing.work+0x420]=8;differing.words[differing.work+0x1c5c]=5;
    const uint32_t differingMenu=differing.work+0x171c;
    differing.words[differingMenu+0x504]=0;differing.words[differingMenu+0x80]=0x1234;
    assert(commandWeaponAddressEvent({{-2,9}},differing.services())==2 && differing.outputs[0]==0x1234);
    Fixture invalid;invalid.unit.kind=222;invalid.words[invalid.work+0x1c5c]=9;invalid.words[invalid.work+0x420]=7;
    assert(!commandWeaponAddressEvent({{-2,9}},invalid.services()) && invalid.outputs.empty());
    std::cout<<"selection tests passed\n";
}

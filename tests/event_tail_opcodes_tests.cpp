#include "event_tail_opcodes.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t command(int n,int op) {return uint32_t(n)<<16|uint32_t(op);}
struct Fixture {
    EventEntry e;
    std::array<EventEntry,256> field{},battle{};
    EventContext fieldContext{1,field.data(),0},battleContext{1,battle.data(),0};
    EventGlobals gp;
    uint32_t context=0x2000,global=0x3000;
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::map<int32_t,int32_t> resolved;
    std::vector<int> calls;
    std::vector<uint32_t> addresses;
    std::vector<EventDiagnosticCall> diagnostics;
    int queryCount=0,slotContexts=0;
    std::function<void()> onContext;
    std::function<void(bool)> onSlotContext;
    Fixture() {e.arguments=0x100;e.userWork=0x5000;e.userFlags=0x6000;words[0x100]=7;}
    EventTailOpcodeServices services() {
        EventTailOpcodeServices s;s.values.user=this;s.operands.user=this;s.manager.user=this;s.formatUser=this;
        s.values.word=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(2);f.addresses.push_back(a);return f.words[a];};
        s.values.resolve=[](void* p,EventEntry&,int32_t op) {auto& f=*static_cast<Fixture*>(p);++f.queryCount;return f.resolved.count(op)?f.resolved.at(op):op;};
        s.operands.contextAddress=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);if(f.onContext)f.onContext();return f.context;};
        s.operands.entryAddress=[](void*,EventEntry&)->uint32_t {return 0x1000;};
        s.operands.globalAddress=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);return f.global;};
        s.operands.readWord=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);f.addresses.push_back(a);return f.words[a];};
        s.operands.readByte=[](void* p,uint32_t a) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(5);f.addresses.push_back(a);return f.bytes[a];};
        s.manager.globals=[](void* p) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(6);return &f.gp;};
        s.manager.context=[](void* p,bool b) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(7);++f.slotContexts;if(f.onSlotContext)f.onSlotContext(b);return b?&f.battleContext:&f.fieldContext;};
        s.format=[](void* p,const EventDiagnosticCall& d) {auto& f=*static_cast<Fixture*>(p);f.calls.push_back(8);f.diagnostics.push_back(d);};return s;
    }
    EventDiagnosticCall diagnostic(int32_t op) {words[0x100]=uint32_t(op);assert(eventDebugRegister(e,services())==2);assert(queryCount==0);return diagnostics.back();}
};
}
int main() {
    Fixture unsupported;assert(!eventTailOpcode(unsupported.e,112,unsupported.services()));assert(!eventTailOpcode(unsupported.e,119,unsupported.services()));assert(unsupported.calls.empty());
    for(uint8_t op:{uint8_t(1),uint8_t(3),uint8_t(113),uint8_t(114),uint8_t(117)}) {
        Fixture f;f.e.initialized=3;f.e.field1a4=123;assert(eventTailOpcode(f.e,op,f.services())==2 && f.calls.empty() && f.e.field1a4==123 && f.e.initialized==3);
    }
    Fixture metadata;metadata.words[0x100]=uint32_t(-30000000);assert(eventTailOpcode(metadata.e,116,metadata.services())==2 && metadata.e.field1a4==uint32_t(-30000000) && metadata.queryCount==0);
    Fixture wait;wait.e.initialized=3;wait.resolved[7]=-1;assert(eventTailOpcode(wait.e,11,wait.services())==0);wait.resolved[7]=0;assert(eventTailOpcode(wait.e,11,wait.services())==2 && wait.queryCount==2 && wait.e.initialized==3);
    Fixture pointer;pointer.field[255].flags=0x93;assert(eventEntryPointer(255,pointer.services().manager)==&pointer.field[255]);pointer.gp.battleContext=-1;assert(eventEntryPointer(255,pointer.services().manager)==nullptr);pointer.battle[255].flags=1;assert(eventEntryPointer(255,pointer.services().manager)==&pointer.battle[255]);
    Fixture found;found.field[9].flags=1;assert(eventTailOpcode(found.field[9],118,found.services())==1 && found.slotContexts==10);
    Fixture absent;absent.field[9].flags=0x92;assert(eventTailOpcode(absent.field[9],118,absent.services())==1 && absent.slotContexts==256); // Capacity1 does not bound probes.
    Fixture switching;switching.field[7].flags=1;switching.battle[7].flags=1;
    switching.onSlotContext=[&](bool b) {if(switching.slotContexts==1) {assert(!b);switching.gp.battleContext=1;}else assert(b);};
    assert(eventTailOpcode(switching.battle[7],118,switching.services())==1 && switching.slotContexts==8);
    Fixture direct;auto addr=direct.diagnostic(-270000000);assert(addr.format==0x802bffb8 && addr.integers[0]==-270000000 && !addr.saveFloatArguments && direct.addresses==std::vector<uint32_t>({0x100}));
    auto number=direct.diagnostic(-250000000);assert(number.format==0x802bffc8 && number.saveFloatArguments && number.floating==-19531.25f);
    auto immediate=direct.diagnostic(123);assert(immediate.format==0x802c0130 && immediate.integers[0]==123 && !immediate.saveFloatArguments);
    struct Family {int32_t operand;uint32_t address,hex,floating,integer;};
    for(auto f:std::vector<Family>{{-190000000,0x5000,0x3c,0x4c,0x60},{-50000000,0x2004,0x118,0x128,0x13c},{-30000000,0x109c,0x14c,0x15c,0x170}}) {
        Fixture d;d.words[f.address]=0x80000000;auto h=d.diagnostic(f.operand);assert(h.format==0x802bffb0+f.hex && h.integers[1]==int32_t(-2147483647-1) && !h.saveFloatArguments);
        assert(h.integers[0]==(f.operand==-190000000?int32_t(-2147483647-1):0));
        d.words[f.address]=uint32_t(-229998464);auto fl=d.diagnostic(f.operand);assert(fl.format==0x802bffb0+f.floating && fl.saveFloatArguments && fl.floating==1.5f && fl.integers[0]==0);
        d.words[f.address]=42;auto i=d.diagnostic(f.operand);assert(i.format==0x802bffb0+f.integer && i.integers[0]==0 && i.integers[1]==42 && !i.saveFloatArguments);
    }
    Fixture saved;saved.words[0x3174]=0x80000000;auto sh=saved.diagnostic(-170000000);assert(sh.format==0x802c0020 && sh.integers[0]==int32_t(-2147483647-1) && sh.integers[1]==0x2000);
    saved.words[0x3174]=uint32_t(-229998464);auto sf=saved.diagnostic(-170000000);assert(sf.format==0x802c0030 && sf.saveFloatArguments && sf.floating==1.5f);
    saved.bytes[0x3579]=0xfe;auto sb=saved.diagnostic(-169999999);assert(sb.format==0x802c0044 && sb.integers[0]==1 && sb.integers[1]==254);
    saved.bytes[0x3db8]=0xfe;auto lb=saved.diagnostic(-150000000);assert(lb.format==0x802c0078 && lb.integers[1]==254);
    saved.words[0x3178]=1;auto gs=saved.diagnostic(-130000000);assert(gs.format==0x802c0088 && gs.integers[1]==1);
    saved.words[0x3d78]=1;auto ls=saved.diagnostic(-110000000);assert(ls.format==0x802c0098 && ls.integers[1]==1);
    for(auto f:std::vector<Family>{{-210000000,0x6000,0,0,0x2c},{-90000000,0x2084,0,0,0xf8},{-70000000,0x10dc,0,0,0x108}}) {
        Fixture d;d.words[f.address]=0x80000000;auto bit=d.diagnostic(f.operand+31);assert(bit.format==0x802bffb0+f.integer && bit.integers[0]==31 && bit.integers[1]==int32_t(-2147483647-1) && !bit.saveFloatArguments);
        auto negative=d.diagnostic(f.operand-1);assert(negative.integers[0]==-1 && negative.integers[1]==0);
    }
    Fixture order;order.words[0x100]=uint32_t(-170000000);order.words[0x4174]=0x80000000;
    order.onContext=[&] {order.e.arguments=0x9999;order.global=0x4000;};assert(eventDebugRegister(order.e,order.services())==2);
    assert(order.calls==std::vector<int>({1,2,3,4,8}) && order.addresses==std::vector<uint32_t>({0x100,0x4174}) && order.diagnostics[0].integers[1]==0x2000 && order.diagnostics[0].destination==0x803d8430);
    // Every signed boundary selects its documented template without going
    // through either operand getter. Sparse providers supply the raw domains.
    for(auto test:std::vector<std::pair<int32_t,uint32_t>>{{-270000001,8},{-270000000,8},{-269999999,0x18},{-220000000,0x18},{-219999999,0x2c},{-200000000,0x2c},{-199999999,0x60},{-180000000,0x60},{-179999999,0x94},{-160000000,0x94},{-159999999,0xc8},{-140000000,0xc8},{-139999999,0xd8},{-120000000,0xd8},{-119999999,0xe8},{-100000000,0xe8},{-99999999,0xf8},{-80000000,0xf8},{-79999999,0x108},{-60000000,0x108},{-59999999,0x13c},{-40000000,0x13c},{-39999999,0x170},{-20000000,0x170},{-19999999,0x180}}) {
        Fixture f;auto d=f.diagnostic(test.first);assert(d.format==0x802bffb0+test.second);
    }
    Fixture program;program.field[0].flags=1;auto& e=program.field[0];e.nextCommand=0x8000;
    program.words[0x8000]=command(0,1);program.words[0x8004]=command(0,3);program.words[0x8008]=command(1,11);program.words[0x800c]=7;program.words[0x8010]=command(0,118);program.words[0x8014]=command(0,2);program.resolved[7]=-1;
    auto dispatcher=program.services().values;dispatcher.handler=[](void* p,EventEntry& e,uint8_t op) {auto& f=*static_cast<Fixture*>(p);auto r=eventTailOpcode(e,op,f.services());assert(r);return *r;};dispatcher.remove=[](void*,EventEntry& e) {e.flags=0;};
    assert(dispatchEvent(e,dispatcher)==0 && e.opcode==11 && e.nextCommand==0x8010);
    program.resolved[7]=0;assert(dispatchEvent(e,dispatcher)==0 && e.opcode==0 && e.nextCommand==0x8014);
    assert(dispatchEvent(e,dispatcher)==-1 && e.flags==0 && program.queryCount==2);
    std::cout<<"event tail opcode tests passed\n";
}

#include "key_item_inventory.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <array>
#include <vector>
#include <utility>
#include <iostream>
using namespace ttyd;
struct Memory {
    std::array<uint16_t,130> slots{};std::vector<std::pair<unsigned,uint16_t>> writes;unsigned reads=0;
    EventMemoryServices service(){EventMemoryServices m;m.user=this;
        m.half=[](void* p,uint32_t at){auto& v=*static_cast<Memory*>(p);assert(at>=0xa0 && !((at-0xa0)&1));++v.reads;return v.slots.at((at-0xa0)/2);};
        m.storeHalf=[](void* p,uint32_t at,uint16_t value){auto& v=*static_cast<Memory*>(p);const unsigned i=(at-0xa0)/2;v.writes.push_back({i,value});v.slots.at(i)=value;};return m;}
};
int main(){
    Memory v;v.slots[0]=95;v.slots[1]=107;v.slots[7]=95;v.slots[120]=95;v.slots[121]=95;auto m=v.service();
    assert(packedKeyItemCount(0,95,m)==3 && v.reads==121);assert(packedKeyItemCount(0,107,m)==1);
    assert(removePackedKeyItem(0,95,m)==1 && v.writes.front()==std::make_pair(0u,uint16_t(0)) && v.writes.size()==241);
    assert(v.slots[0]==107 && !v.slots[1] && v.slots[6]==95 && v.slots[119]==95 && !v.slots[120] && v.slots[121]==95);
    for(unsigned i=0;i<120;++i){assert(v.writes[1+2*i].first==i && v.writes[2+2*i]==std::make_pair(i+1,uint16_t(0)));}
    const auto previous=v.slots;v.writes.clear();assert(!removePackedKeyItem(0,94,m) && v.writes.empty() && v.slots==previous);
    v.slots.fill(0);v.slots[120]=107;v.writes.clear();assert(removePackedKeyItem(0,107,m)==1 && v.writes.size()==1 && !v.slots[120]);
    ProloguePouch pouch;for(unsigned i=0;i<121;++i)pouch.half(0xa0+2*i,i%17==0?95:i==1?107:0);
    assert(pouch.keyItemCount(95)==8 && pouch.keyItemCount(107)==1);assert(pouch.removeKeyItem(95)==1 && pouch.keyItemCount(95)==7 && pouch.half(0xa0)==107);
    struct EventFixture {int32_t removal=1,dest=4;std::vector<int> trace;} events;
    EventEntry e;e.arguments=0x81000000;
    ItemInventoryEventServices ev;ev.user=&events;ev.operands.user=&events;
    ev.operands.word=[](void* p,uint32_t at){auto& v=*static_cast<EventFixture*>(p);if(at==0x81000000){v.trace.push_back(1);return 95u;}assert(at==0x81000004);v.trace.push_back(6);return uint32_t(v.dest);};
    ev.operands.resolve=[](void* p,EventEntry& e,int32_t item){assert(item==95);static_cast<EventFixture*>(p)->trace.push_back(2);e.arguments=0x82000000;return item;};
    ev.operands.store=[](void* p,EventEntry& e,int32_t dest,int32_t value){static_cast<EventFixture*>(p)->trace.push_back(7);e.locals[unsigned(dest)]=value;};
    ev.count=[](void* p,int32_t item){assert(item==95);auto& v=*static_cast<EventFixture*>(p);v.trace.push_back(3);v.dest=5;return 3;};
    ev.remove=[](void* p,int32_t item){assert(item==95);auto& v=*static_cast<EventFixture*>(p);v.trace.push_back(3);return v.removal;};
    ev.reviseMario=[](void* p){auto& v=*static_cast<EventFixture*>(p);v.trace.push_back(4);v.dest=6;};
    ev.reviseParty=[](void* p){auto& v=*static_cast<EventFixture*>(p);v.trace.push_back(5);v.dest=7;};
    assert(checkItemEvent(e,ev)==2 && e.locals[5]==3 && events.trace==std::vector<int>({1,2,3,6,7}));
    events.trace.clear();e.arguments=0x81000000;assert(removeItemEvent(e,ev)==2 && e.locals[7]==0 && events.trace==std::vector<int>({1,2,3,4,5,6,7}));
    for(int32_t result:{0,2,-1}) {events.removal=result;events.trace.clear();e.arguments=0x81000000;
        assert(removeItemEvent(e,ev)==2 && e.locals[7]==-1 && events.trace==std::vector<int>({1,2,3,6,7}));}
    std::cout<<"Original121-slot key item gaps/count/first-removal/order/extent and live pouch adapter tests passed\n";
}

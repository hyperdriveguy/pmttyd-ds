#include "trouble_quest_services.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Harness {
    ProloguePouch pouch;
    std::vector<int32_t> calls;
    int checks=0,removes=0;
};
TroubleQuestServices services(Harness& h) {
    TroubleQuestServices s;s.user=&h;
    s.clearFlag=[](void* p,int32_t f){static_cast<Harness*>(p)->calls.push_back(f);};
    s.setByte=[](void* p,int32_t b,uint8_t v){assert(v==0);static_cast<Harness*>(p)->calls.push_back(b);};
    s.countItem=[](void* p,int32_t item){auto& h=*static_cast<Harness*>(p);++h.checks;h.calls.push_back(-item);return h.pouch.keyItemCount(item);};
    s.removeItem=[](void* p,int32_t item){auto& h=*static_cast<Harness*>(p);++h.removes;h.calls.push_back(-1000-item);h.pouch.removeKeyItem(item);return -77;};
    return s;
}
void resetOrder(const Harness& h) {
    for(int i=0;i<17;++i)assert(h.calls[i]==5405+i);
    for(int i=0;i<6;++i)assert(h.calls[17+i]==1469+i);
}
int main() {
    Harness h;auto s=services(h);assert(startTroubleQuest(s)==2);resetOrder(h);assert(h.calls.size()==23);
    for(int quest:{2,13}) {
        h={};s=services(h);const int item=quest==2?95:107,other=quest==2?107:95;
        h.pouch.half(0xa0,item);h.pouch.half(0xa4,other);h.pouch.half(0xae,item);h.pouch.half(0x190,item);
        h.pouch.half(0x70,3);h.pouch.half(0x74,1);h.pouch.half(0x78,123);h.pouch.half(0x92,-9);
        const auto before=h.pouch;
        assert(cancelTroubleQuestItems({item},s)==2);resetOrder(h);
        assert(h.removes==3 && h.checks==4 && h.pouch.keyItemCount(item)==0 && h.pouch.keyItemCount(other)==1);
        for(unsigned n=0;n<before.bytes.size();++n)if(n<0xa0 || n>=0x192)assert(before.bytes[n]==h.pouch.bytes[n]);
        for(int n=0;n<3;++n){assert(h.calls[23+2*n]==-item);assert(h.calls[24+2*n]==-1000-item);}
        assert(h.calls.back()==-item); // Fresh zero count terminates, despite ignored-77 result.
        h.calls.clear();h.checks=h.removes=0;assert(cancelTroubleQuestItems({item},s)==2);resetOrder(h);assert(h.checks==1 && h.removes==0);
    }
    h={};s=services(h);
    for(int item:{-1,0,121,255})assert(!cancelTroubleQuestItems({item},s));
    assert(h.calls.empty());s.setByte=nullptr;assert(!startTroubleQuest(s) && !cancelTroubleQuestItems({95},s));assert(h.calls.empty());
    s=services(h);s.countItem=s.removeItem=nullptr;
    assert(cancelTroubleQuestItems({},s)==2);resetOrder(h);h.calls.clear();
    s=services(h);s.removeItem=nullptr;assert(!cancelTroubleQuestItems({107},s));assert(h.calls.empty());
}

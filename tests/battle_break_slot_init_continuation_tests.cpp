#include "battle_break_slot_init_continuation.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t B=0x84701000,OtherB=0x84721000,Global=0x8041eb90,ControllerOffset=0x1616c;
constexpr std::array<uint32_t,12> CallSites{0x80205094,0x8020511c,0x80205130,0x80205158,
    0x80205170,0x80205188,0x80205190,0x802051a8,0x802051c0,0x802051c8,0x802051e0,0x802051f8};
struct Fixture {
    std::map<uint32_t,uint32_t> ram;
    std::vector<BattleBreakSlotInitRequest> accepted;
    mutable std::vector<uint32_t> reads;
    std::shared_ptr<int> lifetime=std::make_shared<int>(0);
    bool alive=true,swap=false,changeCount=false,reenter=false,retire=false,zeroAllocation=false;
    int hold=-1,remaining=2;
    uint32_t next=0x84750000;
    const BattleBreakSlotInitRequest* heldAddress=nullptr;
    BattleBreakSlotInitCursor cursor;
    Fixture(){
        ram[Global]=B;
        for(unsigned i=0;i<21;++i)ram[0x802fe040+4*i]=0x1000+i;
        for(const auto b:{B,OtherB})for(unsigned i=0;i<0x134;i+=4)ram[b+ControllerOffset+i]=0xaaaaaaaa;
    }
    uint32_t word(uint32_t a)const{reads.push_back(a);return ram.at(a);}
    BattleBreakSlotInitServices services(){
        BattleBreakSlotInitServices s;s.user=this;s.lifetime=lifetime;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->ram[a]=v;};
        s.current=[](void* p){return static_cast<Fixture*>(p)->alive;};
        s.call=[](void* p,const BattleBreakSlotInitRequest& q){return static_cast<Fixture*>(p)->call(q);};return s;
    }
    std::optional<uint32_t> call(const BattleBreakSlotInitRequest& q){
        assert(q.callsite==CallSites.at(accepted.size()));
        if(reenter){assert(!resumePackedBattleBreakSlotInit(services(),cursor));assert(!cursor.reset());}
        if(int(accepted.size())==hold && remaining-- >0){
            if(heldAddress)assert(heldAddress==&q);else heldAddress=&q;
            return {};
        }
        if(retire){alive=false;return 0;}
        if(zeroAllocation && q.function==0x8000519c)return {};
        accepted.push_back(q);uint32_t value=0;
        switch(q.function){
        case 0x8020524c:value=word(Global)+ControllerOffset;break;
        case 0x80205210:value=word(Global)+ControllerOffset+0x18+q.argument*0x5c;break;
        case 0x800050b4:
            assert(q.bytes==0x134);for(uint32_t i=0;i<q.bytes;i+=4)ram[q.argument+i]=0;break;
        case 0x800f8934:{
            value=zeroAllocation?0:next;next+=q.argument;
            if(changeCount)ram[B+ControllerOffset+0x18+0x18]=2;
            if(swap)ram[Global]=OtherB;
            break;
        }
        case 0x8000519c:
            assert(q.privateSourceOffset && !(q.bytes%4));
            for(uint32_t i=0;i<q.bytes;i+=4){
                const auto& v=q.privateWords.at((*q.privateSourceOffset+i)/4);assert(v);ram[q.argument+i]=*v;
            }
            break;
        default:assert(false);
        }
        return value;
    }
    bool resume(){return resumePackedBattleBreakSlotInit(services(),cursor);}
};
void complete(Fixture& f){
    while(!f.resume()){
        const auto before=f.ram;const auto count=f.accepted.size();const auto reads=f.reads;
        assert(!f.resume());assert(before==f.ram && count==f.accepted.size() && reads==f.reads);
    }
    assert(f.accepted.size()==12 && f.cursor.pending()==0);
}
}
int main(){
    Fixture baseline;complete(baseline);assert(baseline.word(B+ControllerOffset+4)==0);
    for(unsigned i=0;i<3;++i){
        const auto r=B+ControllerOffset+0x18+i*0x5c;const auto count=i==2?5u:8u;
        assert(baseline.word(r)==i && baseline.word(r+8)==0 && baseline.word(r+0x18)==count);
        for(unsigned j=0;j<count;++j)assert(baseline.word(baseline.word(r+0x14)+j*4)==0x1000+i*8+j);
    }
    for(int held=0;held<12;++held){Fixture f;f.hold=held;complete(f);assert(f.ram==baseline.ram);}
    Fixture reentry;reentry.reenter=true;complete(reentry);assert(reentry.ram==baseline.ram);
    Fixture captured;captured.hold=1;assert(!captured.resume());
    for(unsigned i=0;i<21;++i)captured.ram[0x802fe040+i*4]=0xffff;
    assert(!captured.resume());assert(captured.resume());
    assert(captured.word(captured.word(B+ControllerOffset+0x18+0x14))==0x1000);
    Fixture swapped;swapped.swap=true;complete(swapped);
    // First reel pointer survives allocation's source-global swap; later
    // getter calls see the actual changed BattleWork, rather than a fixed base.
    assert(swapped.word(B+ControllerOffset+0x18+0x14)==0x84750000);
    assert(swapped.word(OtherB+ControllerOffset+0x18+0x5c+0x14)==0x84750020);
    Fixture count;count.changeCount=true;complete(count);
    assert(count.accepted[4].argument==32 && count.accepted[5].bytes==8);
    Fixture stale;stale.hold=4;assert(!stale.resume());const auto effects=stale.ram;
    auto different=stale.services();different.lifetime=std::make_shared<int>(2);
    assert(!resumePackedBattleBreakSlotInit(different,stale.cursor) && stale.ram==effects);
    stale.alive=false;assert(!stale.resume());stale.alive=true;assert(!stale.resume());assert(stale.ram==effects);
    assert(stale.cursor.reset());
    Fixture nullAllocation;nullAllocation.zeroAllocation=true;
    assert(!nullAllocation.resume() && nullAllocation.cursor.pending()==0x8000519c);
    assert(nullAllocation.word(B+ControllerOffset+0x18+0x14)==0);
    const auto nullPrefix=nullAllocation.ram;const auto nullCalls=nullAllocation.accepted.size();
    assert(!nullAllocation.resume() && nullAllocation.ram==nullPrefix && nullAllocation.accepted.size()==nullCalls);
    Fixture retired;retired.retire=true;assert(!retired.resume());retired.alive=true;assert(!retired.resume());
}

#include "battle_break_slot_init_continuation.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Controller=0x8020524c, Reel=0x80205210;
constexpr uint32_t Clear=0x800050b4, Allocate=0x800f8934, Copy=0x8000519c;
constexpr uint32_t ControllerBytes=0x134, ReelBytes=0x5c;
constexpr std::array<uint32_t,3> TableBases{0x802fe040,0x802fe060,0x802fe080};
constexpr std::array<uint32_t,3> Counts{8,8,5}, StackOffsets{0x3c,0x1c,8};
enum class Step { Controller, Capture, Clear, InitializeReels, StoreReels, GetReel, Count, Allocate, Copy, Done };
bool same(const BattleBreakSlotInitServices& a,const BattleBreakSlotInitServices& b) {
    const auto& x=a.actors.memory;const auto& y=b.actors.memory;
    return a.user==b.user && a.current==b.current && a.call==b.call &&
        a.lifetime==b.lifetime && !a.lifetime.owner_before(b.lifetime) && !b.lifetime.owner_before(a.lifetime) &&
        x.user==y.user && x.word==y.word && x.storeWord==y.storeWord;
}
struct Guard {bool& busy;explicit Guard(bool& b):busy(b){busy=true;}~Guard(){busy=false;}};
}
struct BattleBreakSlotInitCursor::State {
    BattleBreakSlotInitServices services;
    Step step=Step::Controller;
    uint32_t controller=0,reel=0,index=0,pending=0;
    bool invalid=false;
    std::array<std::optional<uint32_t>,28> words{};
    std::optional<BattleBreakSlotInitRequest> held;
    bool current(){if(invalid || !services.current(services.user)){invalid=true;return false;}return true;}
    uint32_t word(uint32_t a){return services.actors.memory.word(services.actors.memory.user,a);}
    void word(uint32_t a,uint32_t v){services.actors.memory.storeWord(services.actors.memory.user,a,v);}
    std::optional<uint32_t> call(uint32_t pc,uint32_t function,uint32_t argument=0,uint32_t bytes=0,
                                 std::optional<uint32_t> offset={}) {
        if(!held)held=BattleBreakSlotInitRequest{function,pc,argument,bytes,offset,words};
        pending=function;
        if(!current())return {};
        const auto result=services.call(services.user,*held);
        if(!current() || !result)return {};
        held.reset();pending=0;return result;
    }
};
BattleBreakSlotInitCursor::BattleBreakSlotInitCursor()=default;
BattleBreakSlotInitCursor::~BattleBreakSlotInitCursor()=default;
bool BattleBreakSlotInitCursor::active()const{return bool(state_);}
uint32_t BattleBreakSlotInitCursor::pending()const{return state_?state_->pending:0;}
bool BattleBreakSlotInitCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleBreakSlotInit(const BattleBreakSlotInitServices& services,BattleBreakSlotInitCursor& cursor) {
    const auto& m=services.actors.memory;
    if(cursor.invoking_ || !services.lifetime || !services.current || !services.call || !m.word || !m.storeWord)return false;
    if(cursor.state_ && !same(cursor.state_->services,services))return false;
    Guard guard(cursor.invoking_);
    if(!cursor.state_){cursor.state_=std::make_unique<BattleBreakSlotInitCursor::State>();cursor.state_->services=services;}
    auto& c=*cursor.state_;
    for(;;){
        if(!c.current())return false;
        switch(c.step){
        case Step::Controller:{const auto r=c.call(0x80205094,Controller);if(!r)return false;c.controller=*r;c.step=Step::Capture;break;}
        case Step::Capture:
            // Original paired lwz/lwzu: read both source words before either
            // private-stack store. Final third-table word is a separate load.
            for(unsigned table=0;table<3;++table){
                unsigned i=0;
                for(;i+1<Counts[table];i+=2){
                    const auto a=c.word(TableBases[table]+i*4),b=c.word(TableBases[table]+(i+1)*4);
                    c.words[StackOffsets[table]/4+i]=a;c.words[StackOffsets[table]/4+i+1]=b;
                }
                if(i<Counts[table])c.words[StackOffsets[table]/4+i]=c.word(TableBases[table]+i*4);
            }
            c.step=Step::Clear;break;
        case Step::Clear:
            if(!c.call(0x8020511c,Clear,c.controller,ControllerBytes))return false;
            c.word(c.controller+4,0);c.step=Step::InitializeReels;break;
        case Step::InitializeReels:{
            const auto r=c.call(0x80205130,Reel,0);if(!r)return false;c.reel=*r;c.step=Step::StoreReels;break;
        }
        case Step::StoreReels:
            for(unsigned i=0;i<3;++i){c.word(c.reel+i*ReelBytes,i);c.word(c.reel+i*ReelBytes+8,0);}
            c.index=0;c.step=Step::GetReel;break;
        case Step::GetReel:{
            constexpr std::array<uint32_t,3> pcs{0x80205158,0x80205190,0x802051c8};
            const auto r=c.call(pcs[c.index],Reel,c.index);if(!r)return false;c.reel=*r;c.step=Step::Count;break;
        }
        case Step::Count:
            c.word(c.reel+0x18,Counts[c.index]);c.step=Step::Allocate;break;
        case Step::Allocate:{
            constexpr std::array<uint32_t,3> pcs{0x80205170,0x802051a8,0x802051e0};
            const auto r=c.held?c.call(0,0):c.call(pcs[c.index],Allocate,c.word(c.reel+0x18)<<2);
            if(!r)return false;
            c.word(c.reel+0x14,*r);c.step=Step::Copy;break;
        }
        case Step::Copy:{
            constexpr std::array<uint32_t,3> pcs{0x80205188,0x802051c0,0x802051f8};
            // Count and destination are loaded afresh AFTER allocation, once
            // per source invocation; a suspended memcpy retains those values.
            if(!c.held){const auto bytes=c.word(c.reel+0x18)<<2;const auto dest=c.word(c.reel+0x14);
                if(!c.call(pcs[c.index],Copy,dest,bytes,StackOffsets[c.index]))return false;
            }else if(!c.call(0,0))return false;
            if(++c.index==3)c.step=Step::Done;else c.step=Step::GetReel;break;
        }
        case Step::Done:return true;
        }
    }
}
}

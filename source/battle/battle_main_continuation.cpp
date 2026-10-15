#include "battle_main_continuation.hpp"
namespace ttyd {
namespace {
bool same(const BattleActorServices& a,const BattleActorServices& b){
    const auto& x=a.memory;const auto& y=b.memory;
    return a.user==b.user && a.byte==b.byte && x.user==y.user &&
        x.word==y.word && x.half==y.half && x.storeWord==y.storeWord &&
        x.storeHalf==y.storeHalf && x.storeByte==y.storeByte &&
        x.flushBlock==y.flushBlock && x.systemCall==y.systemCall &&
        a.audienceLifecycle.user==b.audienceLifecycle.user &&
        a.audienceLifecycle.event==b.audienceLifecycle.event;
}
// RAII guard refuses recursive run/reset even from inside an admitted child.
// Successful completion drops retained state; the next invocation starts anew.
struct Invoking {
    bool& flag;
    explicit Invoking(bool& f):flag(f){flag=true;}
    ~Invoking(){flag=false;}
};
}
struct BattleMainCursor::State {
    BattleMainServices owner;
    uint32_t outer=0,inner=0;
    size_t index=0;
    bool wrapper=false;
};
BattleMainCursor::BattleMainCursor()=default;
BattleMainCursor::~BattleMainCursor()=default;
bool BattleMainCursor::active()const{return bool(state_);}
uint32_t BattleMainCursor::pending()const{return state_ && state_->index<battleMainRequests.size()?battleMainRequests[state_->index]:0;}
bool BattleMainCursor::reset(){if(invoking_)return false;state_.reset();return true;}
struct BattleMainRunner {
    // Lower wrapper captures outer B before independently capturing upper B.
    // Null OUTER completes without children. Otherwise keep all11 child indices
    // and inner owner across holds, then compute source result and publish only
    // to retained outer. Engaged0 completes; wrapper returns has_value, not!=0.
    static std::optional<int32_t> run(const BattleMainServices& s,BattleMainCursor& cursor,bool wrapper){
        if(cursor.invoking_ || !s.actors.memory.word)return {};
        if(cursor.state_){
            const auto& c=*cursor.state_;
            if(c.wrapper!=wrapper || c.owner.user!=s.user || c.owner.request!=s.request || !same(c.owner.actors,s.actors))return {};
        }
        Invoking guard(cursor.invoking_);
        if(!cursor.state_){
            cursor.state_=std::make_unique<BattleMainCursor::State>();
            auto& c=*cursor.state_;c.owner=s;c.wrapper=wrapper;
            if(wrapper){c.outer=currentBattleAddress(s.actors);if(!c.outer){cursor.state_.reset();return 0;}}
            c.inner=currentBattleAddress(s.actors);
        }
        auto& c=*cursor.state_;
        if(!s.actors.memory.storeWord)return {};
        while(c.index<battleMainRequests.size()){
            if(!s.request || !s.request(s.user,battleMainRequests[c.index],c.inner))return {};
            ++c.index;
        }
        const auto result=packedBattleMainAfterRequests(c.inner,s.actors);
        if(wrapper)publishPackedBattleMainResult(c.outer,result,s.actors);
        cursor.state_.reset();return result;
    }
};
std::optional<int32_t> resumePackedBattleMain(const BattleMainServices& s,BattleMainCursor& c){return BattleMainRunner::run(s,c,false);}
bool resumePackedBattleMainWrapper(const BattleMainServices& s,BattleMainCursor& c){return BattleMainRunner::run(s,c,true).has_value();}

struct BattleSequenceDispatchCursor::State {
    BattleSequenceDispatchServices owner;
    std::array<BattleSequenceDispatchRequest,2> calls{};
    size_t count=0,index=0;
};
BattleSequenceDispatchCursor::BattleSequenceDispatchCursor()=default;
BattleSequenceDispatchCursor::~BattleSequenceDispatchCursor()=default;
bool BattleSequenceDispatchCursor::active()const{return bool(state_);}
uint32_t BattleSequenceDispatchCursor::pending()const{return state_ && state_->index<state_->count?state_->calls[state_->index].function:0;}
bool BattleSequenceDispatchCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// Capture global B and bank+4 once, queue up to TWO exact requests, then
// resume them without rereading route even if child changes bank/global B.
// Bank 0: first entry then pinch(mode 1); 1: pinch0/start; 2: pinch0/act; 3: end;
// other banks complete with no child. Child bool denotes whole-call admission.
bool resumePackedBattleSequenceManager(const BattleSequenceDispatchServices& s,BattleSequenceDispatchCursor& cursor){
    if(cursor.invoking_ || !s.actors.memory.word)return false;
    if(cursor.state_){
        const auto& c=*cursor.state_;
        if(c.owner.user!=s.user || c.owner.request!=s.request || !same(c.owner.actors,s.actors))return false;
    }
    Invoking guard(cursor.invoking_);
    if(!cursor.state_){
        cursor.state_=std::make_unique<BattleSequenceDispatchCursor::State>();
        auto& c=*cursor.state_;c.owner=s;
        const auto b=currentBattleAddress(s.actors),bank=s.actors.memory.word(s.actors.memory.user,b+4);
        const auto append=[&](uint32_t f,int32_t mode=0){c.calls[c.count++]={f,b,mode};};
        switch(bank){
        case 0:append(0x8011ee40);append(0x8011efd8,1);break;
        case 1:append(0x8011efd8);append(0x8011e5c0);break;
        case 2:append(0x8011efd8);append(0x8011c510);break;
        case 3:append(0x80215234);break;
        default:break;
        }
    }
    auto& c=*cursor.state_;
    while(c.index<c.count){
        if(!s.request || !s.request(s.user,c.calls[c.index]))return false;
        ++c.index;
    }
    cursor.state_.reset();return true;
}
}

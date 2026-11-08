#include "battle_init_sequence_continuation.hpp"
#include "battle_sequence_startup.hpp"
namespace ttyd {namespace {
constexpr uint32_t InitBase=0x01000000;
struct Guard{bool&b;explicit Guard(bool&v):b(v){b=true;}~Guard(){b=false;}};
bool same(const BattleInitSequenceServices&a,const BattleInitSequenceServices&b){const auto&x=a.actors.memory;const auto&y=b.actors.memory;return a.user==b.user&&a.current==b.current&&a.call==b.call&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.actors.user==b.actors.user&&a.actors.byte==b.actors.byte&&x.user==y.user&&x.word==y.word&&x.half==y.half&&x.storeWord==y.storeWord&&x.storeHalf==y.storeHalf&&x.storeByte==y.storeByte&&x.flushBlock==y.flushBlock&&x.systemCall==y.systemCall&&a.actors.audienceLifecycle.user==b.actors.audienceLifecycle.user&&a.actors.audienceLifecycle.event==b.actors.audienceLifecycle.event;}
enum class Step{Dispatch,Mario,PartyId,Party,Setup,Ready,Paper1,Paper2,Paper3,Nice,StatusEvent,StatusSystem};
}
struct BattleInitSequenceCursor::State {
 BattleInitSequenceServices s;uint32_t battle=0,pending=0,partyId=0;Step step=Step::Dispatch;bool invalid=false;std::optional<BattleFirstActRequest>held;
 bool current(){if(invalid||!s.current||!s.current(s.user)){invalid=true;return false;}return true;}
 void set(uint32_t n){packedBattleSetSequence(battle,1,InitBase+n,s.actors.memory);}
 std::optional<uint32_t>call(uint32_t site,uint32_t fn,std::array<uint32_t,4>a={}){pending=fn;if(!held){held=BattleFirstActRequest{};held->callsite=site;held->function=fn;held->integers=a;}if(!current()||!s.call)return {};auto v=s.call(s.user,*held);if(!current()||!v)return {};held.reset();pending=0;return v;}
};
BattleInitSequenceCursor::BattleInitSequenceCursor()=default;BattleInitSequenceCursor::~BattleInitSequenceCursor()=default;
bool BattleInitSequenceCursor::active()const{return bool(state_);}uint32_t BattleInitSequenceCursor::pending()const{return state_?state_->pending:0;}bool BattleInitSequenceCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleSequenceInit(uint32_t b,const BattleInitSequenceServices&s,BattleInitSequenceCursor&cursor){
 if(cursor.invoking_||!s.lifetime||!s.current||!s.actors.memory.word||!s.actors.memory.storeWord)return false;
 if(cursor.state_&&(cursor.state_->battle!=b||!same(cursor.state_->s,s)))return false;
 Guard guard(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<BattleInitSequenceCursor::State>();cursor.state_->s=s;cursor.state_->battle=b;}
 auto&c=*cursor.state_;if(!c.current())return false;const auto finish=[&](){cursor.state_.reset();return true;};
 for(;;){switch(c.step){
 case Step::Dispatch:switch(packedBattleGetSequence(b,1,s.actors.memory)){
  case InitBase:c.set(1);c.step=Step::Mario;break;case InitBase+1:c.step=Step::Mario;break;
  case InitBase+2:c.step=Step::Ready;break;case InitBase+3:c.step=Step::Paper1;break;
  case InitBase+4:c.set(5);return finish();case InitBase+5:c.step=Step::StatusEvent;break;default:return finish();}break;
 case Step::Mario:{const auto p=c.call(0x8011eeb4,0x8005e0c0);if(!p)return false;const auto&m=s.actors.memory;m.storeWord(m.user,*p+4,m.word(m.user,*p+4)|1);c.step=Step::PartyId;break;}
 case Step::PartyId:{const auto id=c.call(0x8011eec4,0x80141050);if(!id)return false;c.partyId=*id;c.step=Step::Party;break;}
 case Step::Party:{const auto p=c.call(0x8011eec8,0x800bf1b4,{c.partyId});if(!p)return false;if(*p){const auto&m=s.actors.memory;m.storeWord(m.user,*p+4,m.word(m.user,*p+4)|1);}c.step=Step::Setup;break;}
 case Step::Setup:if(!c.call(0x8011eee4,0x800f8aac,{b}))return false;c.set(2);return finish();
 case Step::Ready:{const auto p=c.call(0x8011ef04,0x80129994,{b});if(!p)return false;if(*p)c.set(3);return finish();}
 case Step::Paper1:{const auto p=c.call(0x8011ef30,0x800414e8,{0x802efbe4,1});if(!p)return false;if(!*p)return finish();c.step=Step::Paper2;break;}
 case Step::Paper2:{const auto p=c.call(0x8011ef44,0x800414e8,{0x802efbec,1});if(!p)return false;if(!*p)return finish();c.step=Step::Paper3;break;}
 case Step::Paper3:{const auto p=c.call(0x8011ef58,0x800414e8,{0x802efbf8,1});if(!p)return false;if(!*p)return finish();c.step=Step::Nice;break;}
 case Step::Nice:{const auto p=c.call(0x8011ef68,0x80144d44,{1});if(!p)return false;if(*p)c.set(4);return finish();}
 case Step::StatusEvent:if(!c.call(0x8011ef9c,0x800f7650))return false;c.step=Step::StatusSystem;break;
 case Step::StatusSystem:if(!c.call(0x8011efa0,0x800f7624))return false;packedBattleSetSequence(b,0,1,s.actors.memory);packedBattleSetSequence(b,2,0x02000000,s.actors.memory);return finish();
 }}
}
}

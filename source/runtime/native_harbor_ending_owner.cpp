#include "native_harbor_ending_owner.hpp"
#include "platform.hpp"
#include "native_harbor_reward_flow.hpp"
#include "harbor_battle_music_owner.hpp"
namespace ttyd {namespace {
bool sameOwner(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){
 return a&&b&&a==b&&!a.owner_before(b)&&!b.owner_before(a);
}
}
struct NativeHarborEndingOwner::State:std::enable_shared_from_this<State>{
 struct Proof{std::weak_ptr<State>state;};
 Game*game=nullptr;Renderer*renderer=nullptr;Audio*audio=nullptr;
 const HarborTutorial*facade=nullptr;NativeFacadeRegistration::Proof facadeProof;
 std::shared_ptr<const void>runtime;
 std::shared_ptr<Proof>proof;
 std::shared_ptr<NativeHarborRewardCounter>counter;
 std::shared_ptr<NativeHarborRewardPresentation>presentation;
 std::unique_ptr<NativeHarborRewardFlow>flow;
 bool alive=true,battleMusic=false;
 bool current()const{
  // Proof before borrowing the facade/Game; replacing Game invalidates the
  // original admission even when its facade address is reused.
  return alive&&facadeProof.current()&&facade&&game&&renderer&&audio&&
   facade==&game->harborTutorial&&facade->nativeGameplayFacadeProof().current()&&
   sameOwner(runtime,facade->nativeGameplayLifetime().lock());
 }
 static std::shared_ptr<State>borrow(void*p){auto s=static_cast<Proof*>(p)->state.lock();return s&&s->current()?s:nullptr;}
 HarborBattleMusicBinding musicBinding(){HarborBattleMusicBinding b;
  b.runtime=runtime;b.proof=proof;b.user=proof.get();
  b.current=[](void*p){return bool(borrow(p));};return b;
 }
 bool advance(){
  if(!current())return false;
  if(!battleMusic){if(!audio->beginHarborBattleMusic(musicBinding(),game->progress.sound))return false;battleMusic=true;}
  const auto conclusion=game->harborTutorial.nativeGameplayConclusion();
  if(!conclusion||*conclusion!=1)return true;
  if(!counter){counter=game->harborTutorial.beginNativeRewardCounter();if(!counter)return false;}
  if(!presentation){presentation=renderer->nativeHarborRewardPresentation(runtime);if(!presentation)return false;}
  if(!flow){NativeHarborRewardFlowServices services;services.user=proof.get();services.lifetime=proof;
   services.current=[](void*p){return bool(borrow(p));};
   services.victoryMusic=[](void*p){auto s=borrow(p);return s&&s->audio->beginHarborVictoryMusic(s->musicBinding(),s->game->progress.sound);};
   services.victoryEvent=[](void*p){auto s=borrow(p);return s&&s->game->harborTutorial.startNativeRewardVictoryEvent();};
   services.victoryEventComplete=[](void*p){auto s=borrow(p);return s&&s->game->harborTutorial.nativeRewardVictoryEventComplete();};
   flow=std::make_unique<NativeHarborRewardFlow>(counter,presentation,std::move(services));
  }
  return flow->advance(game->ticks,game->generalPad().held.entries[0]);
 }
};
NativeHarborEndingOwner::NativeHarborEndingOwner()=default;
NativeHarborEndingOwner::~NativeHarborEndingOwner(){if(state_)state_->alive=false;}
bool NativeHarborEndingOwner::advance(Game&g,Renderer&r,Audio&a){
 if(state_&&!state_->current()){
  // Actual generation-bound retirement precedes releasing its proof. A
  // foreign stream holds rather than being stopped by matching track number.
  if(!a.retireHarborBattleAudio(state_->runtime))return false;
  state_->alive=false;state_.reset();
 }
 if(!g.harborTutorial.running())return true;
 if(!state_){const auto p=g.harborTutorial.nativeGameplayFacadeProof();
  const auto runtime=g.harborTutorial.nativeGameplayLifetime().lock();if(!p.current()||!runtime)return false;
  auto s=std::make_shared<State>();s->game=&g;s->renderer=&r;s->audio=&a;s->facade=&g.harborTutorial;s->facadeProof=p;s->runtime=runtime;
  s->proof=std::make_shared<State::Proof>();s->proof->state=s;state_=std::move(s);
 }
 return state_->advance();
}
bool NativeHarborEndingOwner::updateAudio(Game&g,Audio&a){
 if(!state_||!state_->current()||state_->game!=&g||state_->audio!=&a)return false;
 // Audio's owner preserves the actual battle stream before the victory
 // request and defers legacy track12 replay after the victory stream begins.
 return a.updateHarborBattleMusic(state_->runtime,g.progress.sound);
}
}

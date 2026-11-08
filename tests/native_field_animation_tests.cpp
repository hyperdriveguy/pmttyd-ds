#include "native_field_animation.hpp"
#include "field_party.hpp"
#include "game.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
EventNativePlatform platform(){EventNativePlatform s;s.flushBlock=[](void*,uint32_t){};s.systemCall=[](void*){};return s;}
}
void runNativeFieldAnimationFixture(const char* root){
 assert(loadFieldPlayerPoseAssets(root));
 assert(!NativeFieldAnimation::create(root,{},platform()));
 auto files=NativeFileCache::create();auto native=NativeFieldAnimation::create(root,files,platform());assert(native && native->ready());
 auto pool=native->pool();FieldPartyRegistry roles;FieldMarioBattleState mario;
 assert(roles.configureNativeAnimation(native,fieldPlayerPoseResources()) && roles.initializeMario(mario,{},0));
 assert((mario.poseHandles==std::array<int32_t,3>{0,1,2}));
 for(unsigned i=0;i<3;++i){const auto* p=pool->pose(mario.poseHandles[i]);assert(p && p->sourceBuffers && p->word(0xc)==2 && p->word(0)==1 && p->timestamp()==0);assert(pool->models()[p->word(0x10)].file());}
 BattlePoseClock c;assert(native->clock(c) && !c.modeZero && !c.modeNonzero);
 for(unsigned i=0;i<60;++i)assert(native->tick(false));assert(native->clock(c) && c.modeZero==1000 && c.modeNonzero==1000);
 assert(native->tick(true) && native->clock(c) && c.modeZero==1000 && c.modeNonzero==1016);
 assert(native->tick(false,true) && native->clock(c) && c.modeZero==1000 && c.modeNonzero==1016);
 assert(native->tick(false) && native->clock(c) && c.modeZero==1016 && c.modeNonzero==1033);
 MarioPresentation presenter;presenter.supported=true;presenter.poseUploaded=true;presenter.selected="M_W_1";presenter.poseUploadSerial=1;
 assert(native->mario(presenter,mario.poseHandles) && native->marioPose()==pool->pose(0));auto* front=pool->pose(0);assert(front->timestamp()==c.modeNonzero && !front->shape.vertices.empty());
 // The native clock owns selection/sampling; stale external clock arguments do
 // not switch the running source into an unrelated millisecond domain.
 assert(pool->selectName(0,"M_R_1",true,{900000,900000}) && front->timestamp()==c.modeNonzero);
 presenter.selected="M_W_1";++presenter.poseUploadSerial;presenter.poseStartFrame=10;
 assert(native->mario(presenter,mario.poseHandles) && front->timestamp()==c.modeNonzero-166);
 assert(front->word(0x88)==front->word(0x18) && front->word(0x8c)==front->word(0x1c));
 const auto epoch=front->timestamp();assert(native->tick(false) && native->mario(presenter,mario.poseHandles) && front->timestamp()==epoch && front->scalar(0x24)>10);
 // Repeated draws/readers do not advance time. Equal-name uploads do reset.
 const auto state=front->state;for(unsigned i=0;i<10;++i)assert(native->marioPose()==front);assert(front->state==state);
 ++presenter.poseUploadSerial;presenter.poseStartFrame=0;assert(native->mario(presenter,mario.poseHandles) && front->timestamp()!=epoch);
 presenter.displayFlags=0x80000000;presenter.selected="M_W_1R";++presenter.poseUploadSerial;
 assert(native->mario(presenter,mario.poseHandles) && native->marioBank()==1 && native->marioPose()==pool->pose(1));
 presenter.displayFlags=0x90000000;presenter.selected="M_N_5B";++presenter.poseUploadSerial;
 assert(native->mario(presenter,mario.poseHandles) && native->marioBank()==2 && native->marioPose()==pool->pose(2));
 presenter.selected="unconverted";++presenter.poseUploadSerial;assert(!native->mario(presenter,mario.poseHandles) && !native->marioPose());
 // Rejected caller/resource mismatches must not poison the retained Entry.
 assert(!pool->enter(fieldPlayerPoseResources()[0],2,{},"missing-model"));
 // Additional consumers route real Entry into the same source namespace.
 const auto partner=pool->enter(fieldPlayerPoseResources()[3],2,{999999,999999});assert(partner && *partner==3 && pool->pose(*partner)->sourceBuffers);
 assert(native->clock(c) && pool->pose(*partner)->timestamp()==c.modeNonzero);
 assert(pool->selectName(*partner,"PKR_W_1",true,{}) && pool->main(*partner,{999999,999999}));
 auto& memory=native->process().vm().memory;const uint32_t gp=word(memory,0x80415fd0);assert(word(memory,gp+0x3c)>0);
 FieldPartyRegistry detached(roles);assert(!detached.nativeAnimation && detached.originalPool!=pool && !detached.originalPool->pose(0)->sourceBuffers);
 assert(detached.originalPool->selectName(0,"M_W_1",true,{7,9}) && detached.originalPool->pose(0)->timestamp()==9);
 roles.nativeAnimation.reset();native.reset();assert(!front->sourceBuffers); // Last process owner freezes outputs.
}
void gameLifetime(const char* root){
 auto game=std::make_unique<Game>(false);assert(game->initializeNativeFieldAnimation(root,NativeFileCache::create(),platform()));
 const auto owner=game->fieldPartyState().nativeAnimation;const auto pool=owner->pool();
 assert(game->nativeMarioPose()); // First visible frame needs no speculative tick.
 for(unsigned i=0;i<3;++i)game->tick({});assert(game->nativeMarioPose() && game->nativeMarioPose()->sourceBuffers);
 Input travel;travel.x=1;for(unsigned i=0;i<8;++i)game->tick(travel);assert(game->nativeMarioPose() && !std::strcmp(game->marioPresentation.selected,"M_R_1"));
 travel={};travel.z=-1;for(unsigned i=0;i<8;++i)game->tick(travel);assert(game->nativeMarioPose() && game->nativeMarioBank()==1 && !std::strcmp(game->marioPresentation.selected,"M_R_1R"));
 pool->pose(0)->word(0xe8,0x1234);pool->pose(0)->word(0xec,0x40);
 auto rejected=std::make_unique<Game>(*game);rejected->fieldPartyState().originalPool->pose(0)->word(0x14,0);
 assert(!rejected->retainNativeFieldAnimation(*game) && pool->pose(0)->word(0xe8)==0x1234);
 auto admitted=std::make_unique<Game>(*game);admitted->restore(admitted->progress);assert(pool->pose(0)->word(0xe8)==0x1234);
 assert(admitted->retainNativeFieldAnimation(*game) && pool->pose(0)->word(0xe8)==0 && pool->pose(0)->word(0xec)==0x42);
 auto missingClock=std::make_unique<Game>(false);auto& memory=owner->process().vm().memory;const uint32_t gp=word(memory,0x80415fd0);
 const auto liveState=pool->pose(0)->state;assert(memory.writeWord(0x80415fd0,0));
 assert(!missingClock->retainNativeFieldAnimation(*game,true) && game->fieldPartyState().nativeAnimation==owner && !missingClock->fieldPartyState().nativeAnimation && pool->pose(0)->state==liveState);
 assert(memory.writeWord(0x80415fd0,gp));
 auto candidate=std::make_unique<Game>(*game);assert(!candidate->fieldPartyState().nativeAnimation && candidate->retainNativeFieldAnimation(*game));
 assert(candidate->fieldPartyState().nativeAnimation==owner && candidate->fieldPartyState().originalPool==pool);
 BattlePoseClock before,after;assert(owner->clock(before));auto saved=game->progress;game->restore(saved);assert(game->fieldPartyState().nativeAnimation==owner && owner->clock(after) && before.modeNonzero==after.modeNonzero);
 game->tick({});assert(game->nativeMarioPose() && owner->clock(after) && after.modeNonzero>before.modeNonzero);
 auto checkpoint=std::make_unique<Game>(false);checkpoint->restore(saved);assert(checkpoint->retainNativeFieldAnimation(*game,true));assert(checkpoint->fieldPartyState().nativeAnimation==owner && checkpoint->fieldPartyState().originalPool==pool);
 checkpoint->tick({});assert(checkpoint->nativeMarioPose());
}
void lowHpHarborResume(const char* root){
 // Story4 restores an ordinary rear pose before the low-HP idle owner requests
 // front-only M_S_3/display4. The original special RearAnime suffix must retire
 // the stale rear handle even while the player remains in the rear sector.
 assert(loadHarborEncounterAssets(root));
 auto game=std::make_unique<Game>(false);
 assert(game->initializeNativeFieldAnimation(root,NativeFileCache::create(),platform()));
 auto saved=game->progress;saved.room=1;saved.originalStory=4;saved.position={0,-1};saved.hp=2;
 game->restore(saved);bool breathing=false;
 constexpr unsigned SimulationTicks=64,JumpCadenceTicks=16,JumpHeldTicks=6,MaximumDrainVisits=100;
 for(unsigned tick=0;tick<SimulationTicks;++tick){
  Input input;input.confirm=tick%JumpCadenceTicks==0;input.jumpHeld=tick%JumpCadenceTicks<JumpHeldTicks;game->tick(input);
  for(unsigned drain=0;game->nativeAnimationPending() && drain<MaximumDrainVisits;++drain)game->drainNativeAnimation();
  assert(!game->nativeAnimationPending() && game->nativeMarioPose());
  if(!std::strcmp(game->marioPresentation.selected,"M_S_3")){
   breathing=true;assert(game->nativeMarioBank()==0);
   assert(!(game->marioPresentation.displayFlags&0x80000000u));
  }
 }
 assert(breathing);
}
int main(){runNativeFieldAnimationFixture("romfs");gameLifetime("romfs");lowHpHarborResume("romfs");}

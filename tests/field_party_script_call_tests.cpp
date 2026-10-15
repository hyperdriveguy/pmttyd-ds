#include "field_party_script_call.hpp"
#include "event_native_runtime.hpp"
#include "professor_story.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
struct StoryPartyWorld {
 FieldPartyRegistry parties;FieldMarioBattleState mario;
 std::shared_ptr<ProloguePouch> pouch=std::make_shared<ProloguePouch>();
 std::shared_ptr<NativeFileCache> files=NativeFileCache::create();std::shared_ptr<NativeFieldAnimation> animation;
 std::optional<int32_t> current=0;EventNativeRuntime story;EventEntry entry;
 FieldPartyScriptBindings binding;static constexpr uint32_t E=0x81818000,A=0x81819000;
 static EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f)->int32_t{assert(std::isfinite(f));return int32_t(f);};return p;}
 explicit StoryPartyWorld(const char* root):story(0x81000000,platform()){
  assert(loadFieldPlayerPoseAssets(root));animation=NativeFieldAnimation::create(root,files,platform());assert(animation);
  assert(parties.configureNativeAnimation(animation,fieldPlayerPoseResources()) && parties.initializeMario(mario,{},0));pouch->half(20,10);
  entry.id=31;entry.arguments=A;assert(story.entryAddresses.registerEntries(E,&entry,1));assert(story.memory.registerRam(A,std::vector<uint8_t>(64)));
  // Same guest addresses deliberately contain different operands in the native
  // producer VM. A bridge accidentally reading that VM cannot pass these tests.
  assert(animation->process().vm().entryAddresses.registerEntries(E,&nativeEntry,1));
  nativeEntry.arguments=A;assert(animation->process().vm().memory.registerRam(A,std::vector<uint8_t>(64,0xff)));
  binding.parties=&parties;binding.mario=&mario;binding.currentIndex=&current;binding.pouch=pouch;binding.root=root;
  binding.appearance.display=[](void*){return 0.f;};
 }
 EventEntry nativeEntry;
 void args(std::initializer_list<uint32_t> words){unsigned i=0;entry.arguments=A;for(auto v:words)assert(story.memory.writeWord(A+4*i++,v));}
 int32_t step(NativeFieldPartyScriptCall& c,uint32_t fn,bool first=true){return c.step(entry,first,fn,story,binding,1000);}
 FieldPartyActor* seed(bool auxiliary=false){auto c=NativeFieldPartyEntry::create(binding.root.c_str(),parties,mario,pouch);int32_t result=-1;assert(c && c->position(auxiliary?13:1,auxiliary,{{25,0,625}},{},1000,binding.appearance,result));assert(result==(auxiliary?1:0));mario.partyIndices[auxiliary?1:0]=uint8_t(result);return parties.pointer(result);}
 void manager(bool ready){assert(animation->process().vm().memory.writeWord(NativeFileCache::Global,ready?NativeFileCache::Manager:0));}
};
void joinAndModes(const char* root){
 StoryPartyWorld w(root);auto* p=w.seed();auto* a=w.seed(true);w.pouch->half(14,0x120);const auto hp=w.pouch->half(20);
 w.args({uint32_t(-30000000)});w.entry.locals[0]=1;NativeFieldPartyScriptCall join;assert(w.step(join,0x800f0a8c,false)==2);
 assert(w.pouch->half(14)==0x121 && w.pouch->half(20)==hp && w.mario.partyIndices[0]==0 && w.parties.pointer(0)==p);
 w.binding.pouch.reset();NativeFieldPartyScriptCall lateJoin;assert(w.step(lateJoin,0x800f0a8c)==0 && lateJoin.pending()==0x80140f74);
 w.entry.locals[0]=7;w.binding.pouch=w.pouch;assert(w.step(lateJoin,0x800f0a8c,false)==2 && w.pouch->half(14)==0x121 && w.pouch->half(98)==0);
 w.args({8});NativeFieldPartyScriptCall high;assert(w.step(high,0x800f0a8c)==2 && w.pouch->half(14)==0x121);
 w.args({uint32_t(-1)});NativeFieldPartyScriptCall negative;assert(w.step(negative,0x800f0a8c)==0 && negative.pending()==0x80140f74);
 // Every nonzero RAW operand targets table1. Fresh auxiliary mode2 means run
 // does nothing, including no bit8 write and no saved-mode publication.
 w.args({uint32_t(-30000000)});a->word(8,0x2000);a->bytes[0x35]=77;NativeFieldPartyScriptCall auxRun;assert(w.step(auxRun,0x800eae70)==2 && a->bytes[0x34]==2 && a->bytes[0x35]==77 && a->word(8)==0x2000);
 NativeFieldPartyScriptCall auxStop;assert(w.step(auxStop,0x800eaeb8)==2 && a->bytes[0x34]==0 && a->bytes[0x35]==2 && a->word(8)==0x2008);
 NativeFieldPartyScriptCall auxAgain;assert(w.step(auxAgain,0x800eae70)==2 && a->bytes[0x34]==1 && a->bytes[0x35]==0);
 // Independent current-index publication differs from Mario245.
 w.current=1;w.args({0});NativeFieldPartyScriptCall stop;assert(w.step(stop,0x800eaeb8)==2 && a->bytes[0x34]==0 && p->bytes[0x34]==0);
 w.current=0;NativeFieldPartyScriptCall run;assert(w.step(run,0x800eae70)==2 && p->bytes[0x34]==1);
 p->bytes[0x34]=12;p->word(4,0x100040);NativeFieldPartyScriptCall stop12;assert(w.step(stop12,0x800eaeb8)==2 && p->bytes[0x35]==12 && p->word(4)==0x40);
 p->bytes[0x34]=9;p->bytes[0x35]=99;NativeFieldPartyScriptCall stop9;assert(w.step(stop9,0x800eaeb8)==2 && p->bytes[0x35]==99 && p->bytes[0x34]==0);
 w.current.reset();NativeFieldPartyScriptCall missing;assert(w.step(missing,0x800eae70)==0);w.current=-1;assert(w.step(missing,0x800eae70,false)==2);
}
void retainedRemovalAndPrevious(const char* root){
 StoryPartyWorld w(root);auto* p=w.seed();assert(p);w.args({0});w.manager(false);NativeFieldPartyScriptCall kill;
 assert(w.step(kill,0x800e2e0c)==0 && kill.pending());assert(!kill.releaseForRetirement());assert(w.mario.partyIndices[0]==255 && w.mario.savedPartyKinds[0]==1);
 // Reached removal selector and child must not be re-read on a later VM visit.
 w.args({1});w.entry.id=32;assert(w.step(kill,0x800e2e0c,false)==0 && kill.pending()==0x800e2e0c);w.entry.id=31;
 w.manager(true);assert(w.step(kill,0x800e2e0c,false)==2 && !w.parties.pointer(0) && w.mario.partyIndices[0]==255 && w.mario.savedPartyKinds[0]==1);
 w.args({0,uint32_t(-30000000),uint32_t(-29999999),uint32_t(-29999998)});w.entry.locals[0]=17;w.entry.locals[1]=0;w.entry.locals[2]=301;
 NativeFieldPartyScriptCall previous;
 // Preserve first=true even when field ownership is unavailable at admission.
 assert(previous.step(w.entry,true,0x800e2f00,w.story,{},1000)==0);
 w.manager(false);assert(w.step(previous,0x800e2f00,false)==0 && previous.pending());p=w.parties.pointer(0);assert(p && w.mario.partyIndices[0]==255);
 w.args({1,900,900,900});w.entry.locals.fill(0);w.manager(true);assert(w.step(previous,0x800e2f00,false)==2 && w.mario.partyIndices[0]==0);
 assert(p==w.parties.pointer(0) && p->scalar(0x58)==17 && p->scalar(0x60)==301 && p->bytes[0x34]==0);
 w.args({0});NativeFieldPartyScriptCall clear;assert(w.step(clear,0x800e2da0)==2 && !w.parties.pointer(0) && w.mario.savedPartyKinds[0]==1);
 w.seed(true);w.mario.savedPartyKinds[1]=13;w.args({1});NativeFieldPartyScriptCall clearAux;assert(w.step(clearAux,0x800e2da0)==2 && !w.parties.pointer(1) && w.mario.savedPartyKinds[1]==0);
}
void slitAndLeaseGuards(const char* root){
 {StoryPartyWorld w(root);auto* p=w.seed();w.args({0});p->bytes[0x34]=1;p->word(0,p->word(0)|0x1000);NativeFieldPartyScriptCall stop;
  assert(w.step(stop,0x800eaeb8)==0 && stop.pending()==0x8014c448 && p->bytes[0x34]==1 && !w.parties.nativeEntryOwner.expired());
  p->word(0,p->word(0)&~0x1000u);assert(w.step(stop,0x800eaeb8,false)==0 && stop.pending()==0x8014c448 && p->bytes[0x34]==1);
  auto removal=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(removal && !removal->kill(0,false) && w.parties.pointer(0)==p);
  assert(stop.releaseForRetirement() && w.parties.nativeEntryOwner.expired());
  assert(removal->kill(0,false) && !w.parties.pointer(0));
 }
 {StoryPartyWorld w(root);auto* p=w.seed();w.args({0});auto blocker=std::make_shared<int>(1);w.parties.nativeEntryOwner=blocker;NativeFieldPartyScriptCall run;
  assert(w.step(run,0x800eae70)==0 && p->bytes[0x34]==0);blocker.reset();
  auto removal=NativeFieldPartyLifetime::create(root,w.parties,w.mario);assert(removal && removal->kill(0,false));
  // Pointer lookup follows lease admission: removal before admission is seen.
  assert(w.step(run,0x800eae70,false)==2);
 }
}
void gameHandoffGuards(const char* root){
 auto game=std::make_unique<Game>(false);assert(loadFieldPlayerPoseAssets(root) && game->initializeNativeFieldAnimation(root,NativeFileCache::create(),StoryPartyWorld::platform()));
 auto b=game->fieldPartyScriptBindings();assert(b.bound() && b.root==root && b.pouch==game->progress.originalPouch);
 auto candidate=std::make_unique<Game>(*game);auto lease=std::make_shared<int>(1);b.parties->nativeEntryOwner=lease;
 assert(game->nativePartyPending() && !candidate->retainNativeFieldAnimation(*game) && !candidate->retainNativeFieldAnimation(*game,true));
 auto saved=game->progress;const auto hp=saved.hp;saved.hp=hp-1;game->restore(saved);assert(game->progress.hp==hp);
 lease.reset();assert(!game->nativePartyPending() && candidate->retainNativeFieldAnimation(*game));
 auto nextLease=std::make_shared<int>(2);candidate->fieldPartyScriptBindings().parties->nativeRemovalOwner=nextLease;
 assert(!candidate->retainNativeFieldAnimation(*game,true));
}
void actualLectureKillBoundary(const char* root){
 StoryPartyWorld w(root);w.seed();assert(loadProfessorStoryAssets(root));ProfessorStory lesson;assert(lesson.beginLecture(0,{-260,0,200},{-290,0,200}));lesson.fieldParty=w.binding;
 lesson.tick(17,0,false);lesson.actors[0].position[2]=260;w.manager(false);
 for(unsigned i=0;i<12000 && !lesson.pendingService;++i){MessageSelectionInput input;if(lesson.selection.active() && lesson.selection.state==1)input.buttons=0x100|(lesson.selection.current==1?0u:4u);lesson.tick(lesson.now+17,0x100,false,input);}
 assert(!lesson.failed && lesson.pendingService && !lesson.goombellaNpc && w.mario.partyIndices[0]==255);
 lesson.fieldParty={};lesson.tick(lesson.now+17,0x100,false);assert(lesson.pendingService && !lesson.goombellaNpc && lesson.nativePartyPending());lesson.fieldParty=w.binding;
 w.manager(true);for(unsigned i=0;i<12000 && !lesson.goombellaNpc;++i)lesson.tick(lesson.now+17,0x100,false);
 assert(!lesson.failed && lesson.goombellaNpc && !w.parties.pointer(0) && w.mario.partyIndices[0]==255 && w.mario.savedPartyKinds[0]==1);
 for(unsigned i=0;i<12000 && lesson.messageOrder.back()!="mac_0_084_06";++i)lesson.tick(lesson.now+17,0x100,false);
 assert(!lesson.failed && lesson.messageOrder.back()=="mac_0_084_06" && lesson.story==10);
}
}
void runNativeFieldPartyStoryFixtures(const char* root){joinAndModes(root);retainedRemovalAndPrevious(root);slitAndLeaseGuards(root);gameHandoffGuards(root);actualLectureKillBoundary(root);}
int main(){runNativeFieldPartyStoryFixtures("romfs");std::cout<<"cross-VM native party story dispatch: PASS\n";}

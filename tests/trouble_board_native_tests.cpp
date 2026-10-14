#include "game.hpp"
#include "prologue_pouch.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
Game initial(bool lecture=false) {
 Game g(false);g.progress.room=2;g.progress.originalStory=23;g.progress.position={2.5f,.04f};
 initializeTroubleQuestState(g.progress.originalNpcTalkState);
 setTroubleQuestFlag(g.progress.originalNpcTalkState,5374,lecture);
 g.fieldDoor.interiorMask=4;g.wallContact.sourceNode=0x12d518;
 return g;
}
void run(Game& g,int choice=0,int row=1) {
 for(unsigned n=0;n<8000 && g.troubleBoardLocked();++n) {
  Input i;auto* p=g.troubleBoardPresentation();assert(p);
  if(p->message.canAdvanceText())i.confirm=true;
  if(p->selection.state==1) {if(p->selection.current!=choice)i.z=1;else i.confirm=true;}
  if(auto* menu=p->menu();menu && menu->displayed() && menu->work().state==1) {
   if(menu->work().cursor!=row)i.z=1;else i.confirm=true;
  }
  g.tick(i);
 }
 assert(!g.troubleBoardLocked() && !g.fieldKeysOff);
}
int main() {
 assert(loadHarborStoryAssets("romfs") && loadNativeTroubleBoardAssets("romfs"));
 assert(loadFieldActorCatalog("romfs") && loadFieldSaveObjectAssets("romfs"));
 assert(loadGround("romfs/gor_02.ground") && loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
 auto wrong=initial(true);wrong.wallContact.sourceNode=0x12d514;assert(!wrong.tryTroubleBoardContact(0x100));
 wrong.wallContact.sourceNode=0x12d518;wrong.fieldDoor.interiorMask=0;assert(!wrong.tryTroubleBoardContact(0x100));
 wrong.fieldDoor.interiorMask=4;wrong.roadAdmission.window=true;assert(!wrong.tryTroubleBoardContact(0x100));
 wrong.roadAdmission.window=false;assert(!wrong.tryTroubleBoardContact(0));
 auto legacy=initial();legacy.progress.originalNpcTalkState={};
 assert(legacy.tryTroubleBoardContact(0x100) && !legacy.troubleBoardLocked()); // No fabricated fresh quest state.
 auto simultaneous=initial(true);Input both;both.confirm=both.cancel=true;
 simultaneous.tick(both);assert(simultaneous.troubleBoardLocked()); // Original case9 A ignores B.
 auto lecture=initial();lecture.marioPresentation.display=lecture.marioPresentation.target=180;
 assert(lecture.fieldPartyCurrentIndex==0 && !lecture.fieldPartyState().pointer(0));
 assert(lecture.tryTroubleBoardContact(0x100) && lecture.troubleBoardLocked());
 assert(!lecture.troubleBoardPresentation()->message.active()); // Deferred EVT; no contact-frame callback.
 bool turn=false,endpoint=false,external=false;
 for(unsigned n=0;n<40;++n) {
  lecture.tick({});
  if(lecture.marioPresentation.display<180 && lecture.marioPresentation.display>0)turn=true;
  if(lecture.marioPresentation.display==0)endpoint=true;
  if(lecture.troubleBoardPresentation()->message.active() && !external) {
   setTroubleQuestFlag(lecture.progress.originalNpcTalkState,5405,true);external=true;
  }
 }
 assert(turn && endpoint && external && lecture.troubleBoardPresentation()->anchor().fixed);
 run(lecture);assert(*troubleQuestFlag(lecture.progress.originalNpcTalkState,5374));
 assert(*troubleQuestFlag(lecture.progress.originalNpcTalkState,5405));
 lecture.progress.originalNpcTalkState.quest1420=13;lecture.tick({});
 assert(*lecture.progress.originalNpcTalkState.quest1420==13); // Completed VM cannot overwrite later field writes.
 auto accepted=initial(true);assert(accepted.tryTroubleBoardContact(0x100));run(accepted);
 assert(*accepted.progress.originalNpcTalkState.quest1420==2);
 assert(!accepted.troubleBoardPresentation()->menu());
 assert((accepted.troubleBoardSounds==std::vector<uint32_t>{42,5,18,43,18}));
 // A nonempty unsupported start child survives its parent. Case reentry
 // appends into the SAME manager, rather than dropping or waiting on that child.
 auto child=initial(true);assert(child.tryTroubleBoardContact(0x100));run(child,0,0);
 assert(*child.progress.originalNpcTalkState.quest1420==1);
 const FieldActorRecord* garf=nullptr;
 for(const auto& actor:fieldActorCatalog().actors())if(actor.initialization==FieldActorInit::Garf)garf=&actor;
 assert(garf);for(int n=0;n<40;++n)child.tick({});
 const auto* startedGarf=child.fieldNpcState(garf->key);
 assert(startedGarf && startedGarf->body.root.x==445 && startedGarf->body.root.z==120);
 auto* retained=child.troubleBoardPresentation();
 child.wallContact.sourceNode=0x12d3e0;
 assert(child.tryTroubleBoardContact(0x100) && child.troubleBoardLocked());
 assert(child.troubleBoardPresentation()==retained);run(child,2);
 assert(*child.progress.originalNpcTalkState.quest1420==1);
 // Counter is a separate exact contact and uses actual original pouch storage.
 auto counter=initial(true);counter.wallContact.sourceNode=0x12d3e0;
 counter.progress.originalNpcTalkState.quest1420=2;
 counter.progress.originalPouch=std::make_shared<ProloguePouch>();
 counter.progress.originalPouch->half(0x78,45);
 assert(counter.progress.originalPouch->insertKeyItem(95)==1);
 assert(counter.progress.originalPouch->insertKeyItem(95)==1);
 assert(counter.tryTroubleBoardContact(0x100));
 bool fee=false;unsigned confirmations=0;
 for(unsigned n=0;n<8000 && counter.troubleBoardLocked();++n) {
  Input i;auto* p=counter.troubleBoardPresentation();
  if(p->message.canAdvanceText())i.confirm=true;
  if(p->selection.state==1) {
   const int wanted=p->selection.count==3?1:0;
   if(p->selection.current!=wanted)i.z=1;else i.confirm=true;
  }
  if(p->selection.state==7)++confirmations;
  counter.tick(i);
  if(counter.progress.originalPouch->half(0x78)==15)fee=true;
 }
 assert(fee && confirmations && !counter.troubleBoardLocked());
 assert(*counter.progress.originalNpcTalkState.quest1420==0);
 assert(counter.progress.originalPouch->keyItemCount(95)==0 && !counter.fieldKeysOff);
 // All original task-table lists, including true empty lists, drive actual
 // counter cancellation. Other keys/resources survive the original loop.
 constexpr int expectedItems[30]={49,95,0,0,0,0,0,0,0,0,0,0,107,0,0,110,98,0,73,0,0,0,99,0,109,0,0,0,100,0};
 for(unsigned quest=1;quest<=30;++quest) {
  auto task=initial(true);task.progress.originalStory=401;task.wallContact.sourceNode=0x12d3e0;
  task.progress.originalNpcTalkState.quest1420=uint8_t(quest);
  task.progress.originalPouch=std::make_shared<ProloguePouch>();
  auto& pouch=*task.progress.originalPouch;pouch.half(0x78,70);pouch.half(0x70,10);
  for(auto item:{49,95,107,110,98,73,99,109,100})for(int count=0;count<3;++count)assert(pouch.insertKeyItem(item)==1);
  for(int f=5405;f<=5421;++f)setTroubleQuestFlag(task.progress.originalNpcTalkState,f,true);
  assert(task.tryTroubleBoardContact(0x100));
  for(unsigned n=0;n<8000 && task.troubleBoardLocked();++n) {
   Input i;auto* p=task.troubleBoardPresentation();
   if(p->message.canAdvanceText())i.confirm=true;
   if(p->selection.state==1){const int wanted=p->selection.count==3?1:0;if(p->selection.current!=wanted)i.z=1;else i.confirm=true;}
   task.tick(i);
  }
  assert(!task.troubleBoardLocked() && !task.fieldKeysOff && *task.progress.originalNpcTalkState.quest1420==0);
  assert(pouch.half(0x78)==40 && pouch.half(0x70)==10);
  for(auto item:{49,95,107,110,98,73,99,109,100})assert(pouch.keyItemCount(item)==(item==expectedItems[quest-1]?0:3));
  for(int f=5405;f<=5421;++f)assert(!*troubleQuestFlag(task.progress.originalNpcTalkState,f));
  if(quest==1 || quest==4) {
   // Its original independently launched end child moves the actual actor,
   // even though init placed it below-map with5406 set before cancellation.
   for(int n=0;n<40;++n)task.tick({});
   for(const auto& actor:fieldActorCatalog().actors())if(actor.initialization==(quest==1?FieldActorInit::Garf:FieldActorInit::Borodo2)) {
    const auto* live=task.fieldNpcState(actor.key);assert(live && live->initialized);
    assert(live->body.root.x==(quest==1?10:-475) && live->body.root.y==0 && live->body.root.z==(quest==1?-460:-180));
   }
  }
 }
 auto start4=initial(true);start4.progress.originalStory=401;start4.tick({});
 const FieldActorRecord* borodo=nullptr;
 for(const auto& actor:fieldActorCatalog().actors())if(actor.initialization==FieldActorInit::Borodo2)borodo=&actor;
 assert(borodo && start4.setFieldNpcPosition(borodo->eventName,{350,0,120}));
 start4.wallContact.sourceNode=0x12d518; // Contact fixture after the NPC initialization visit.
 assert(start4.tryTroubleBoardContact(0x100));run(start4,0,3);
 for(int n=0;n<40;++n)start4.tick({});
 assert(*start4.progress.originalNpcTalkState.quest1420==4);
 const auto* moved=start4.fieldNpcState(borodo->key);
 assert(moved && moved->body.root.x==-475 && moved->body.root.y==0 && moved->body.root.z==-180);
 // Unknown source selector global is retained as pending, not guessed from Mario.
 auto unknown=initial();unknown.fieldPartyCurrentIndex.reset();assert(unknown.tryTroubleBoardContact(0x100));
 unknown.tick({});unknown.fieldPartyCurrentIndex=0;unknown.tick({});
 assert(!unknown.troubleBoardPresentation()->message.id().empty());
}

#include "goomba_acquisition.hpp"
#include <cassert>
#include <string>
#include <vector>
using namespace ttyd;
struct Harness {
 FieldItemRegistry* registry=nullptr;ProloguePouch* pouch=nullptr;
 float initialY=-1000;bool ready=true;int action=1;std::vector<uint32_t> windows;
};
FieldItemGetServices presentation(Harness& h) {
 FieldItemGetServices s;s.user=&h;
 s.ready=[](void* p){return static_cast<Harness*>(p)->ready;};
 s.mario=[](void*){return ItemGetPoint{3,20,-7};};
 s.request=[](void* p,ItemGetRequest r){auto& h=*static_cast<Harness*>(p);
  if(r==ItemGetRequest::Camera8FlagOn)assert(h.registry->find("irai_item")->position.y==h.initialY);
  auto* icon=h.registry->findIcon("irai_item");
  if(icon){if(r==ItemGetRequest::IconFlag100On)icon->flags|=0x100;
   if(r==ItemGetRequest::IconFlag80On){icon->flags|=0x80;assert(h.registry->find("irai_item")->status&1 || h.pouch->keyItemCount(95)==0);}
   if(r==ItemGetRequest::IconFlag100Off)icon->flags&=~0x100;
   if(r==ItemGetRequest::IconFlag80Off)icon->flags&=~0x80;}
 };
 s.effect=[](void* p,int32_t type,ItemGetPoint xyz){auto& h=*static_cast<Harness*>(p);assert(type==6 && xyz.y==82);
  const auto* entry=h.registry->find("irai_item");assert(entry && entry->substate==2 && entry->position.y==70 && entry->position.x==3);return uintptr_t(1);};
 s.window=[](void* p,uint32_t definition,const char* name){auto& h=*static_cast<Harness*>(p);assert(std::string(name)=="irai_item");
  assert(h.registry->find(name)->position.y==70 && h.pouch->keyItemCount(95)==0);h.windows.push_back(definition);return int32_t(h.windows.size());};
 s.windowAction=[](void* p,int32_t slot){assert(slot==1);return static_cast<Harness*>(p)->action;};
 s.bgmSilent=[](void*){return false;};s.bgmCheck=[](void*,int32_t group){assert(group==1);return int32_t(0);};
 // receive/remove callbacks intentionally absent: the actual owner supplies them.
 return s;
}
void finishPrint(GoombaAcquisition& event,FieldActorTalk& talk,uint64_t& tick) {
 while(talk.message.active() && tick<3000){talk.message.tick(++tick*1000/60,0x100);event.eventMain();}
 assert(!talk.message.active());event.eventMain();
}
int main() {
 MessageFont font;assert(font.load("romfs/font.pmfont") && loadFieldActorCatalog("romfs"));
 const FieldActorRecord* actor=nullptr;for(const auto& a:fieldActorCatalog().actors())if(a.room==0 && a.key==0x42ff5f1f)actor=&a;assert(actor);
 FieldActorTalkState state;assert(!goombaQuest2AcquisitionAdmitted(*actor,state));
 state.quest1420=2;state.flag5405=false;assert(!goombaQuest2AcquisitionAdmitted(*actor,state));
 state.flag5406=false;assert(goombaQuest2AcquisitionAdmitted(*actor,state));
 for(bool full:{false,true}) {
  auto pouch=initializeProloguePouch("Yoshi");pouch.half(0x70,3);pouch.half(0x74,1);
  if(full)for(unsigned i=0;i<121;++i)pouch.half(0xa0+i*2,107);
  FieldItemRegistry registry;FieldActorTalk talk;GoombaAcquisition event;Harness h{&registry,&pouch};uint64_t tick=0;
  assert(event.begin(*actor,pouch,state,talk,font,registry,presentation(h),0));
  assert(event.active() && talk.message.id()=="irai_02_00" && talk.actor.key==actor->key);
  assert(!event.begin(*actor,pouch,state,talk,font,registry,presentation(h),0));
  event.eventMain();event.itemMain(0x100);assert(!registry.find("irai_item") && pouch.keyItemCount(95)==0 && state.flag5405==false);
  finishPrint(event,talk,tick);
  auto* entry=registry.find("irai_item");assert(entry && entry->flags==0x1003 && entry->mode==16 && entry->sequence==2 && entry->substate==0 && entry->position.y==-1000);
  assert(h.windows.empty() && pouch.keyItemCount(95)==0 && state.flag5405==false);
  event.eventMain();assert(state.flag5405==false); // Poll before itemMain cannot grant/complete.
  event.itemMain(0x100);assert(event.get().substate()==2 && entry->substate==2 && entry->position.y==70);
  assert(h.windows[0]==0x80313dc8 && h.windows[1]==0x80313df0);
  assert(registry.findIcon("irai_item")->position.y==70 && registry.findIcon("irai_item")->camera()==5);
  assert(pouch.keyItemCount(95)==(full?0:1) && state.flag5405==false);
  event.eventMain();event.itemMain(0x100);assert(event.get().substate()==3 && state.flag5405==false);
  h.action=0;event.itemMain(0);assert(event.active() && state.flag5405==false);
  event.itemMain(0x200);
  if(full){assert(event.get().state()==ItemGetStep::DiscardPending && h.windows.size()==3 && h.windows[2]==0x80313e18);
   event.eventMain();assert(event.active() && state.flag5405==false && entry->flags&1);continue;}
  assert(event.get().state()==ItemGetStep::Complete && !(entry->flags&1) && !registry.findIcon("irai_item"));
  assert(state.flag5405==false && event.active()); // Item cleanup is not the event's5405 write.
  event.eventMain();assert(event.complete() && !event.active() && state.flag5405==true);
  event.itemMain(0x300);event.eventMain();assert(pouch.keyItemCount(95)==1 && pouch.half(0x70)==3 && pouch.half(0x74)==1);
  assert(state.quest1420==2 && state.flag5406==false);state.flag5405=false;
 }
 // Source first-name lookup retains an older entry's position through the
 // pre-effect callbacks; cleanup still deletes both identically named entries.
 {
  auto pouch=initializeProloguePouch("Yoshi");FieldItemRegistry registry;FieldActorTalk talk;
  auto* older=registry.enter95("irai_item",{9,42,1},true);assert(older);
  Harness h{&registry,&pouch};h.initialY=42;GoombaAcquisition event;uint64_t tick=0;
  assert(event.begin(*actor,pouch,state,talk,font,registry,presentation(h),0));finishPrint(event,talk,tick);
  assert(registry.find("irai_item")==older && older->position.y==42);
  event.itemMain(0);assert(pouch.keyItemCount(95)==1);h.action=0;event.itemMain(0x100);
  assert(!registry.findIcon("irai_item"));
  for(const auto& entry:registry.items())assert(!(entry.flags&1));
  assert(state.flag5405==false);event.eventMain();assert(state.flag5405==true);state.flag5405=false;
 }
 // Missing presentation does not print, allocate an entry or mutate the pouch.
 auto pouch=initializeProloguePouch("Yoshi");FieldItemRegistry registry;FieldActorTalk talk;Harness missing{&registry,&pouch};missing.ready=false;
 GoombaAcquisition unavailable;assert(!unavailable.begin(*actor,pouch,state,talk,font,registry,presentation(missing),0));
 assert(!talk.message.active() && !registry.find("irai_item") && !pouch.keyItemCount(95));
}

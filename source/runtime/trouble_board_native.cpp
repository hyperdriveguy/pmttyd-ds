#include "game.hpp"
#include "field_party_appearance.hpp"
#include <map>
namespace ttyd {
namespace {
std::string assetRoot;
struct Turn {float initial=0;int counter=0;int32_t party=-1;};
TroubleBoardInput inputFor(const Input& i,const PhysicalInputProducer& producer) {
 TroubleBoardInput input;
 if(i.physicalSnapshot) {
  const auto& p=producer.channels[0];
  input.messageTriggers=p.pressed;
  input.selection={p.pressed,p.directionPressed,p.repeat,p.directionRepeat};
  input.menu={p.pressed,p.directionRepeat,p.directionPressed};
 } else {
  input.messageTriggers=(i.confirm?0x100u:0u)|(i.cancel?0x200u:0u);
  input.selection.buttons=input.messageTriggers;
  input.selection.directions=i.z<-.5f?0x1000u:i.z>.5f?0x2000u:0;
  input.menu={input.messageTriggers,input.selection.directions,input.selection.directions};
 }
 if(i.touchX>=0){input.messageTriggers|=0x100;input.selection.buttons|=0x100;input.menu.buttons|=0x100;}
 return input;
}
}
bool loadNativeTroubleBoardAssets(const char* root) {
 if(!root)return false;
 MessageAssets probe;
 const std::string path=root;
 if(!loadTroubleBoardEventAssets(root) || !loadFieldDoorDataAssets(root) || !probe.loadGlobal((path+"/messages/global.txt").c_str()) ||
    !probe.loadRoom((path+"/messages/gor_02.txt").c_str()))return false;
 assetRoot=path;return true;
}
struct NativeTroubleBoard {
 Game* owner=nullptr;
 TroubleBoardEvent event;
 TroubleBoardPresentation presentation;
 MessageAssets messages;
 std::map<uint32_t,Turn> turns;
};
bool Game::tryTroubleBoardContact(uint32_t keys) {
 if(progress.room!=2 || !(fieldDoor.interiorMask&4) || !progress.originalStory || *progress.originalStory<23 ||
    fieldDoor.active() || fieldNpcTalkActive() || saveLocked())return false;
 const uint32_t node=wallContact.sourceNode;
 if(node!=0x12d518 && node!=0x12d3e0)return false;
 if(!fieldDoorAdmitted(node,node,keys,fieldJump.motion,false,fieldNpcTalkCandidateAlive(),roadAdmission))return false;
 if(assetRoot.empty())return true;
 if(troubleBoard && troubleBoard->owner==this) {
  if(troubleBoard->event.synchronizeSavedState(*progress.originalStory,progress.originalNpcTalkState))
   troubleBoard->event.appendRoot(node==0x12d518?TroubleBoardRoot::Board:TroubleBoardRoot::Counter);
  saveRequested=false;return true;
 }
 auto next=std::make_shared<NativeTroubleBoard>();next->owner=this;
 if(!next->messages.loadGlobal((assetRoot+"/messages/global.txt").c_str()) ||
    !next->messages.loadRoom((assetRoot+"/messages/gor_02.txt").c_str()))return true;
 TroubleBoardPresentationServices services;
 services.world={this,[](void* p,TroubleEventRequest& r){return static_cast<Game*>(p)->troubleBoardCall(r);}};
 services.soundUser=this;
 services.menuSound=[](void* p,int32_t row){static_cast<Game*>(p)->troubleBoardSounds.push_back(uint32_t(row));};
 services.selectionSound=[](void* p,uint32_t id){static_cast<Game*>(p)->troubleBoardSounds.push_back(id&0x1fff);};
 if(!next->presentation.bind(next->event,next->messages,harborMessageFont(),(assetRoot+"/messages/gor_02.txt").c_str(),services) ||
    !next->event.begin(ticks*1000/60,*progress.originalStory,progress.originalNpcTalkState,
      node==0x12d518?TroubleBoardRoot::Board:TroubleBoardRoot::Counter))return true;
 troubleBoard=std::move(next);saveRequested=false;return true;
}
bool Game::troubleBoardLocked() const {
 return troubleBoard && troubleBoard->owner==this && troubleBoard->event.rootAlive;
}
TroubleBoardPresentation* Game::troubleBoardPresentation() const {
 return troubleBoard && troubleBoard->owner==this?&troubleBoard->presentation:nullptr;
}
void Game::troubleBoardEventMain(const Input& i) {
 if(!troubleBoard || troubleBoard->owner!=this)return;
 synchronizeFieldNpcs(); // Invalidate area data before publishing any GW address.
 auto& s=*troubleBoard;
 const bool running=s.event.active();
 if(running) {
  s.event.synchronizeWorldWork(11,progress.room==2?fieldDoorData.workAddress(11):0);
  if(!progress.originalStory || !s.event.synchronizeSavedState(*progress.originalStory,progress.originalNpcTalkState))return;
 }
 s.presentation.eventMain(ticks,ticks*1000/60,inputFor(i,inputProducer));
 if(running && !s.event.failed) {
  // One serialized field owner: import immediately before the EVT visit, export
  // only its finite saved-bank domain immediately after. Other state survives.
  progress.originalNpcTalkState.quest1420=s.event.switches.quest1420;
  for(int32_t f=5374;f<=5421;++f)setTroubleQuestFlag(progress.originalNpcTalkState,f,*troubleQuestFlag(s.event.switches,f));
  for(int32_t b=1469;b<=1474;++b)setTroubleQuestByte(progress.originalNpcTalkState,b,*troubleQuestByte(s.event.switches,b));
 }
}
void Game::troubleBoardPresentationMain(const Input& i) {
 if(auto* p=troubleBoardPresentation())p->presentationMain(inputFor(i,inputProducer));
}
int32_t Game::troubleBoardCall(TroubleEventRequest& r) {
 if(!troubleBoard || troubleBoard->owner!=this)return 0;
 auto& s=*troubleBoard;
 switch(r.kind) {
 case TroubleEventCall::Key:
  return applyFieldItemStateRequest(r.values[0]?ItemGetRequest::KeyOn:ItemGetRequest::KeyOff)?2:0;
 case TroubleEventCall::MarioDirection:
 case TroubleEventCall::PartyDirection: {
  const bool mario=r.kind==TroubleEventCall::MarioDirection;
  // Exact finite requests in this original closure; no general turn ABI claim.
  if((mario && (r.values[0]!=270 || r.values[1]!=200 || r.values[2]!=0)) ||
     (!mario && (r.values[1]!=270 || r.values[2]!=200)))return 0;
  auto found=s.turns.find(r.eventId);
  if(found==s.turns.end()) {
   Turn turn;
   if(mario)turn.initial=marioPresentation.display;
   else {
    if(!r.operands[0] && !fieldPartyCurrentIndex)return 0;
    turn.party=r.operands[0]?1:*fieldPartyCurrentIndex;
    if(turn.party>=2)return 0; // Native supported registry domain, not original bounds.
    const auto* actor=fieldParties.pointer(turn.party);
    if(!actor)return 2;
    turn.initial=actor->scalar(0x10c);
   }
   found=s.turns.emplace(r.eventId,turn).first;
  }
  auto& turn=found->second;
  FieldPartyActor* actor=nullptr;
  if(!mario){actor=fieldParties.pointer(turn.party);if(!actor){s.turns.erase(found);return 2;}}
  constexpr int count=200*TickRate/1000;
  const float target=0.f;
  volatile float product=(target-turn.initial)*(float(turn.counter)/float(count));
  const float direction=fieldPartyWrap(turn.initial+product);
  const float heading=fieldPartyHeading(direction,fieldMovement.headingOffsetDegrees,0,0);
  if(mario) {
   marioPresentation.display=marioPresentation.target=direction;marioPresentation.facing=heading;
   fieldMovement.facingDegrees=heading;
   fieldMovement.headingX=std::sin(heading*3.14159265358979323846f/180.f);
   fieldMovement.headingZ=-std::cos(heading*3.14159265358979323846f/180.f);
  } else {actor->scalar(0x10c,direction);actor->scalar(0x110,direction);actor->scalar(0x100,heading);}
  if(++turn.counter>count){s.turns.erase(found);return 1;}return 0;
 }
 case TroubleEventCall::NpcPosition:
  return setFieldNpcPosition(r.text,{float(r.values[1]),float(r.values[2]),float(r.values[3])})?2:0;
 case TroubleEventCall::DoorParameter:
  if(progress.room!=2)return 0;
  return fieldDoorData.setParameter(uint32_t(r.values[0]),r.values[1],uint32_t(r.values[2]))?2:0;
 case TroubleEventCall::CoinRead:
  if(!progress.originalPouch)return 0;
  r.result=progress.originalPouch->half(0x78);return 2;
 case TroubleEventCall::CoinAdd:
  if(!progress.originalPouch)return 0;
  progress.originalPouch->addCoins(r.values[0]);return 2;
 case TroubleEventCall::Cancel: {
  if(!r.cancellationItems || (!r.cancellationItems->empty() && !progress.originalPouch))return 0;
  struct Context {Game* game;TroubleQuestServices* switches;} context{this,&r.switches};
  TroubleQuestServices services;services.user=&context;
  services.clearFlag=[](void* p,int32_t f){auto& c=*static_cast<Context*>(p);c.switches->clearFlag(c.switches->user,f);};
  services.setByte=[](void* p,int32_t b,uint8_t v){auto& c=*static_cast<Context*>(p);c.switches->setByte(c.switches->user,b,v);};
  services.countItem=[](void* p,int32_t item){return static_cast<Context*>(p)->game->progress.originalPouch->keyItemCount(item);};
  services.removeItem=[](void* p,int32_t item){return static_cast<Context*>(p)->game->progress.originalPouch->removeKeyItem(item);};
  return cancelTroubleQuestItems(*r.cancellationItems,services).value_or(0);
 }
 default:return 0; // Actual NPC/door/item/world providers remain pending.
 }
}
}

#include "trouble_board_presentation.hpp"
#include <cassert>
#include <type_traits>
#include <vector>
using namespace ttyd;
static_assert(!std::is_copy_constructible<TroubleBoardPresentation>::value,"Stable callback owner");
struct World {
 int keys=0,directions=0;
 std::vector<int32_t> menuSounds;
 std::vector<uint32_t> selectionSounds;
 TroubleBoardPresentationServices services() {
  TroubleBoardPresentationServices s;
  s.world={this,[](void* p,TroubleEventRequest& r)->int32_t {
   auto& w=*static_cast<World*>(p);
   if(r.kind==TroubleEventCall::Key){w.keys+=r.values[0]?-1:1;return 2;}
   if(r.kind==TroubleEventCall::MarioDirection || r.kind==TroubleEventCall::PartyDirection){++w.directions;return 2;}
   return 0;
  }};
  s.soundUser=this;
  s.menuSound=[](void* p,int32_t row){static_cast<World*>(p)->menuSounds.push_back(row);};
  s.selectionSound=[](void* p,uint32_t id){static_cast<World*>(p)->selectionSounds.push_back(id);};
  return s;
 }
};
FieldActorTalkState initial(bool lecture=false) {
 FieldActorTalkState s;initializeTroubleQuestState(s);setTroubleQuestFlag(s,5374,lecture);return s;
}
int main() {
 MessageFont font;MessageAssets assets;
 assert(font.load("romfs/font.pmfont") && loadMessageIcons("romfs/message-icons.bin"));
 assert(assets.loadGlobal("romfs/messages/global.txt") && assets.loadRoom("romfs/messages/gor_02.txt"));
 assert(loadTroubleBoardEventAssets("romfs"));
 TroubleBoardEvent lecture;assert(lecture.begin(0,23,initial()));
 TroubleBoardPresentation p;World world;
 assert(p.bind(lecture,assets,font,"romfs/messages/gor_02.txt",world.services()));
 assert(!p.bind(lecture,assets,font,"romfs/messages/gor_02.txt",world.services()));
 bool sawPrint=false,sawSelection=false,sawStopped=false,sawZeroFade=false;
 uint64_t serial=0;
 for(unsigned n=0;n<4000 && lecture.active();++n) {
  TroubleBoardInput input;
  if(p.message.canAdvanceText())input.messageTriggers=0x100;
  if(p.selection.state==1)input.selection.buttons=0x100;
  assert(p.visit(n,n*17,input));
  if(p.message.id()=="irai_00") {
   sawPrint=true;assert(p.message.placementOverride==7);
   assert(p.anchor().fixed && p.anchor().x==150 && p.anchor().y==12 && p.anchor().z==-156);
   if(!serial)serial=p.message.serial;
  }
  if(p.message.stopped())sawStopped=true;
  if(p.selection.active())sawSelection=true;
  if(p.selection.state==7 && !p.selection.opacity) {sawZeroFade=true;assert(lecture.active());}
  const auto state=p.selection.state;const auto opacity=p.selection.opacity;
  assert(!p.visit(n,n*17,input));assert(p.selection.state==state && p.selection.opacity==opacity);
 }
 assert(lecture.finished && !lecture.failed && sawPrint && sawSelection && sawStopped && sawZeroFade);
 assert(*troubleQuestFlag(lecture.switches,5374) && world.keys==0 && world.directions==3 && !p.menu());
 assert(p.message.serial==serial && p.message.rawText().find(assets.find("irai_03"))!=std::string::npos);
 assert((world.selectionSounds==std::vector<uint32_t>{0x20012}));
 // Real list, original diary/help/confirmation, decline and return to the same
 // retained menu and cursor, then accept quest2 (its original start is empty).
 TroubleBoardEvent board;assert(board.begin(0,23,initial(true)));
 TroubleBoardPresentation q;World second;
 assert(q.bind(board,assets,font,"romfs/messages/gor_02.txt",second.services()));
 TroubleQuestPresentation* retained=nullptr;bool moved=false,declined=false,reopened=false;
 unsigned choices=0;bool choicePressed=false;uint64_t helpSerial=0;bool help=false;
 for(unsigned n=0;n<6000 && board.active();++n) {
  TroubleBoardInput input;
  if(q.message.canAdvanceText())input.messageTriggers=0x100;
  if(q.menu() && q.menu()->displayed() && q.menu()->work().state==1) {
   if(!retained)retained=q.menu();else assert(retained==q.menu());
   if(!moved){input.menu.directionRepeat=0x2000;input.menu.directionTrigger=0x2000;moved=true;}
   else {assert(q.menu()->work().cursor==1);input.menu.buttons=0x100;if(declined)reopened=true;}
  }
  if(q.selection.state==1 && !choicePressed) {
   if(!choices){input.selection.buttons=4;declined=true;} // No, then A on next visit.
   else input.selection.buttons=0x100;
   choicePressed=true;
  } else if(q.selection.state==1 && choicePressed) input.selection.buttons=0x100;
  if(q.selection.state==7 && choicePressed){++choices;choicePressed=false;}
  assert(q.visit(n,n*17,input));
  if(q.message.id()=="irai_help_02") {
   help=true;assert(q.message.windowType==7 && q.message.placementOverride==0 && !q.anchor().fixed);
   helpSerial=q.message.serial;
  }
 }
 assert(board.finished && !board.failed && *board.switches.quest1420==2);
 assert(help && helpSerial && declined && reopened && choices==2 && second.keys==0 && !q.menu());
 assert((second.menuSounds==std::vector<int32_t>{42,5,18,43,42,18,43}));
 // A resource arriving after USER_FUNC firstCall was consumed still opens
 // once and retains the original fixed anchor through subsequent polls.
 TroubleBoardEvent late;assert(late.begin(0,23,initial()));
 MessageAssets delayed;TroubleBoardPresentation waiting;World lateWorld;
 assert(waiting.bind(late,delayed,font,"romfs/messages/gor_02.txt",lateWorld.services()));
 for(unsigned n=0;n<30;++n)assert(waiting.visit(n,n*17,{}));
 assert(late.active() && !waiting.message.active());
 assert(delayed.loadRoom("romfs/messages/gor_02.txt"));
 assert(waiting.visit(30,510,{}) && waiting.message.id()=="irai_00" && waiting.anchor().fixed);
 const auto once=waiting.message.serial;
 for(unsigned n=31;n<40;++n)assert(waiting.visit(n,n*17,{}));
 assert(waiting.message.serial==once && lateWorld.directions==3);
 // Equal wall times still represent independent simulation visits; an absent
 // world provider yields KeyOff rather than reporting a fabricated completion.
 TroubleBoardEvent pending;assert(pending.begin(0,23,initial(true)));
 TroubleBoardPresentation blocked;assert(blocked.bind(pending,assets,font,"romfs/messages/gor_02.txt",{}));
 assert(blocked.visit(1,0,{}) && pending.active() && !blocked.menu());
 assert(blocked.visit(2,0,{}) && pending.active() && !blocked.menu());
}

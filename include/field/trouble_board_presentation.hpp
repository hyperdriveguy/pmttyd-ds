#pragma once
#include "trouble_board_event.hpp"
#include "trouble_quest_presentation.hpp"
#include "message_selection.hpp"
#include <map>
namespace ttyd {
struct TroubleBoardInput {
 uint32_t messageTriggers=0;
 MessageSelectionInput selection;
 TroubleQuestMenuInput menu;
};
struct TroubleBoardPresentationServices {
 TroubleEventServices world;
 void* soundUser=nullptr;
 void (*menuSound)(void*,int32_t originalRow)=nullptr;
 void (*selectionSound)(void*,uint32_t originalId)=nullptr;
};
struct TroubleBoardAnchor {bool fixed=false;float x=0,y=0,z=0;};
// Stable presentation owner for one original event invocation. The caller owns
// the event/font/resources and their lifetimes; world calls retain their source
// first/poll contract. Rendering never advances this owner.
class TroubleBoardPresentation {
public:
 TroubleBoardPresentation()=default;
 TroubleBoardPresentation(const TroubleBoardPresentation&)=delete;
 TroubleBoardPresentation& operator=(const TroubleBoardPresentation&)=delete;
 bool bind(TroubleBoardEvent&,const MessageAssets&,const MessageFont&,const char* roomResource,
           const TroubleBoardPresentationServices&);
 // Explicit monotonically increasing simulation visit, independent of ms and FPS.
 bool visit(uint64_t visit,uint64_t milliseconds,const TroubleBoardInput&);
 bool eventMain(uint64_t visit,uint64_t milliseconds,const TroubleBoardInput&);
 bool presentationMain(const TroubleBoardInput&);
 MessagePlayback message;
 MessageSelection selection;
 const TroubleBoardAnchor& anchor() const{return anchor_;}
 TroubleQuestPresentation* menu(){return menu_.get();}
private:
 struct Toge {int32_t type=0,x=0,y=0,z=0;};
 TroubleBoardEvent* event_=nullptr;
 const MessageAssets* assets_=nullptr;
 const MessageFont* font_=nullptr;
 std::string room_;
 TroubleBoardPresentationServices services_;
 std::unique_ptr<TroubleQuestPresentation> menu_;
 std::map<uint32_t,Toge> toge_;
 std::map<uint64_t,bool> opened_;
 TroubleBoardAnchor anchor_;
 TroubleBoardInput input_;
 uint64_t now_=0,lastVisit_=0;
 bool visited_=false,presentationPending_=false;
 int32_t activeQuest_=0;
 int32_t call(TroubleEventRequest&);
};
}

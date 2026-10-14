#pragma once
#include "field_actors.hpp"
#include "trouble_quest_services.hpp"
#include <array>
#include <memory>
namespace ttyd {
bool loadTroubleBoardEventAssets(const char* root);
enum class TroubleEventCall:uint32_t {
 Key,Toge,Print,Select,Append,Continue,MenuInit,MenuMain,MenuEnd,
 MarioDirection,PartyDirection,RegisterCase,Cancel,CoinRead,CoinAdd,
 NpcPosition,DoorParameter,ItemDelete,ItemDeleteCheck,Check,Start
};
struct TroubleEventRequest {
 TroubleEventCall kind=TroubleEventCall::Key;
 uint32_t function=0,eventId=0;
 bool first=false;
 uint8_t argumentCount=0;
 std::array<int32_t,6> operands{},values{};
 std::array<int32_t,16> locals{}; // Installer case wrapper saves all16 LW words.
 int32_t selection=0,quest=0,work=0,result=0,story=0,activeQuest=0;
 TroubleQuestServices switches; // Actual VM working-flag/byte writers for cancellation.
 const std::vector<int32_t>* cancellationItems=nullptr; // Immutable original table list, borrowed from this runtime.
 uint32_t cancellationItemsAddress=0;
 std::string text; // Original ID/name bytes; no rewritten dialogue.
};
struct TroubleEventServices {
 void* user=nullptr;
 //0 retains/yields;2 clears/continues. Direction providers also return1
 // to clear/yield at the original positive-duration endpoint.
 // MenuInit publishes a retained nonzero work pointer; MenuMain/Select publish
 // LW0. Providers must retain their first request before yielding for resources
 // or a later world service; subsequent polls keep original first=false.
 // Unbound callbacks must yield, never acknowledge synthetic success.
 int32_t (*call)(void*,TroubleEventRequest&)=nullptr;
};
enum class TroubleBoardRoot {Board,Counter,Installer};
struct TroubleBoardEventRuntime;
class TroubleBoardEvent {
public:
 TroubleBoardEvent()=default;
 TroubleBoardEvent(const TroubleBoardEvent&)=delete;
 TroubleBoardEvent& operator=(const TroubleBoardEvent&)=delete;
 TroubleBoardEvent(TroubleBoardEvent&&)=default;
 TroubleBoardEvent& operator=(TroubleBoardEvent&&)=default;
 bool begin(uint64_t milliseconds,int32_t story,const FieldActorTalkState&,TroubleBoardRoot=TroubleBoardRoot::Board);
 void tick(uint64_t milliseconds,const TroubleEventServices&);
 bool synchronizeSavedState(int32_t story,const FieldActorTalkState&);
 bool synchronizeWorldWork(uint32_t index,uint32_t address); // Actual per-area copied-record binding.
 bool appendRoot(TroubleBoardRoot); // Source case reentry shares the retained manager and outstanding children.
 bool active() const{return runtime_ && started && !finished && !failed;}
 FieldActorTalkState switches;
 bool started=false,finished=false,failed=false,waitingForProvider=false,rootAlive=false;
 uint32_t rootEventId=0;
 uint32_t pendingFunction=0,diagnosticCount=0;
private:
 std::shared_ptr<TroubleBoardEventRuntime> runtime_;
};
}

#pragma once
#include "trouble_quest_window.hpp"
#include "message_icons.hpp"
namespace ttyd {
struct TroubleQuestIcon {
 const char* name=nullptr;const MessageIcon* resource=nullptr;
 uint16_t flags=0,frame=0,delay=0;float x=0,y=0,z=0,scale=1;
 bool visible() const{return (flags&1) && !(flags&2);}
};
struct TroubleQuestStateServices {
 void* user=nullptr;
 bool (*ready)(void*)=nullptr;
 std::optional<int32_t> (*activeQuest)(void*)=nullptr;
 std::optional<bool> (*flag)(void*,int32_t)=nullptr;
 void (*sound)(void*,int32_t)=nullptr;
};
// Finite exclusive five-name icon domain; original global pool/case/EVT lifetime
// and native cue playback belong to their providers. Owner must remain stable.
class TroubleQuestPresentation {
public:
 bool begin(int32_t story,const MessageFont&,const char* originalRoomResource,const TroubleQuestStateServices&);
 std::optional<int32_t> invoke(TroubleQuestMenuInput);
 void iconMain(bool resourcesReady=true);
 bool canAdvance() const{return owner_==this && menu_.canAdvance();}
 bool displayed() const{return canAdvance() && displayWork_;}
 TroubleQuestMenuWork* displayWork(){return displayed()?displayWork_:nullptr;}
 TroubleQuestWindowCache& cache(){return cache_;}
 const TroubleQuestMenuWork& work() const{return menu_.work();}
 const std::array<TroubleQuestIcon,5>& icons() const{return icons_;}
 std::optional<int32_t> result() const{return result_;}
 const TroubleQuestStateServices& stateServices() const{return services_;}
private:
 const TroubleQuestPresentation* owner_=nullptr;
 TroubleQuestMenu menu_;TroubleQuestWindowCache cache_;
 TroubleQuestStateServices services_;
 TroubleQuestMenuWork* displayWork_=nullptr;
 std::array<TroubleQuestIcon,5> icons_{};
 std::optional<int32_t> result_;
 TroubleQuestIcon* find(const char*);
};
}

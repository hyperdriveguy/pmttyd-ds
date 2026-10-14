#include "trouble_quest_services.hpp"
namespace ttyd {
namespace {
bool switchesReady(const TroubleQuestServices& s){return s.clearFlag && s.setByte;}
void resetWorkingSwitches(const TroubleQuestServices& s) {
    for(int32_t flag=5405;flag<=5421;++flag)s.clearFlag(s.user,flag);
    for(int32_t byte=1469;byte<=1474;++byte)s.setByte(s.user,byte,0);
}
}
std::optional<int32_t> startTroubleQuest(const TroubleQuestServices& s) {
    if(!switchesReady(s))return {};
    resetWorkingSwitches(s);return 2;
}
std::optional<int32_t> cancelTroubleQuest(int32_t quest,const TroubleQuestServices& s) {
    // These are the recovered original irai_item_table bindings. Remaining
    // tasks require their actual tables; no arbitrary quest index is admitted.
    const int32_t item=quest==2?95:quest==13?107:0;
    if(!item || !switchesReady(s) || !s.countItem || !s.removeItem)return {};
    resetWorkingSwitches(s);
    // Source rechecks after every removal and ignores the removal result. No
    // parameter revision, cached quantity, synthetic clamp or quest1420 write.
    while(s.countItem(s.user,item)>0)s.removeItem(s.user,item);
    return 2;
}
}

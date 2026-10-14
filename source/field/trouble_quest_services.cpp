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
std::optional<int32_t> cancelTroubleQuestItems(const std::vector<int32_t>& items,const TroubleQuestServices& s) {
    if(!switchesReady(s) || (!items.empty() && (!s.countItem || !s.removeItem)))return {};
    // Native finite key-item branch. Validate before the source reset; list order
    // and repeated fresh counts remain the original table-driven operation.
    for(auto item:items)if(item<1 || item>120)return {};
    resetWorkingSwitches(s);
    for(auto item:items)while(s.countItem(s.user,item)>0)s.removeItem(s.user,item);
    return 2;
}
}

#pragma once
#include <cstdint>
#include <optional>
#include <vector>
namespace ttyd {
// Original gor_irai.o callbacks. Switch storage and pouch ownership belong to
// their providers; these functions neither select nor accept a task.
struct TroubleQuestServices {
    void* user=nullptr;
    void (*clearFlag)(void*,int32_t)=nullptr;
    void (*setByte)(void*,int32_t,uint8_t)=nullptr;
    int32_t (*countItem)(void*,int32_t)=nullptr;
    int32_t (*removeItem)(void*,int32_t)=nullptr; // Low-level pouchRemoveItem.
};
// Contained2 is the original event return. nullopt is native missing-provider
// or invalid-list admission, before any source write.
std::optional<int32_t> startTroubleQuest(const TroubleQuestServices&);
std::optional<int32_t> cancelTroubleQuestItems(const std::vector<int32_t>& originalItems,const TroubleQuestServices&);
}

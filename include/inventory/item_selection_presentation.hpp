#pragma once
#include "item_selection_runtime.hpp"
#include "window_font.hpp"
#include "message_assets.hpp"
#include <memory>
namespace ttyd {
bool loadItemSelectorAssets(const char* root);
// Retained source IDs/US resource pointers; source table order is not inventory
// sorting or quantity collapsing. Window draw publication remains deferred.
class ItemSelectionPresentation {
public:
    bool begin(const std::vector<uint16_t>& items,const MessageFont&);
    void tick(const ItemSelectionInput&,int32_t refreshRate,uint32_t sourceFrameCounter);
    ItemSelectionRuntime owner;
    const char* name(size_t row) const {return names_.at(row);}
    const char* description(size_t row) const {return descriptions_.at(row);}
    const char* heading() const {return heading_;}
    const char* header() const {return header_;}
    uint32_t displaySlots() const {return displaySlots_;}
    uint32_t iconTick=0; // Supplied GP.word1c adapter, not time since selector entry.
    std::vector<uint32_t> soundRequests;
    bool statusOpen=false;
    uint32_t palette0=0x1e1414ff; // Original initial palette; supplied mutable font state.
private:
    const MessageFont* font_=nullptr; // Caller retains font through release (native global font does).
    std::shared_ptr<const MessageResource> messages_;
    std::vector<const char*> names_,descriptions_;
    const char* heading_=nullptr,*header_=nullptr;
    uint32_t displaySlots_=0;
    int32_t refreshRate_=0;
};
}

#include "item_selection_presentation.hpp"
#include "item_presentation.hpp"
#include "message_assets.hpp"
#include "message_icons.hpp"
#include "field_camera_events.hpp"
#include <cassert>
namespace ttyd {
namespace {std::shared_ptr<const MessageResource> messages;}
bool loadItemSelectorAssets(const char* root) {
    if(!root)return false;
    MessageResource next;if(!next.load((std::string(root)+"/messages/global.txt").c_str()))return false;
    if(!next.find("msg_window_title_1") || !next.find("msg_window_select_1"))return false;
    messages=std::make_shared<MessageResource>(std::move(next));return true;
}
bool ItemSelectionPresentation::begin(const std::vector<uint16_t>& items,const MessageFont& font) {
    if(!messages || owner.retained() || items.empty() || !messageIcon(504))return false;
    if(items.size()>8 && (!messageIcon(445) || !messageIcon(446)))return false;
    std::vector<const char*> names,descriptions;names.reserve(items.size());descriptions.reserve(items.size());
    WindowFontMeasurement measured;
    const auto bank=messages;
    const char* heading=bank->find("msg_window_title_1"),*header=bank->find("msg_window_select_1");
    if(!measurePlainWindowFont(font,heading,2,measured) || !measurePlainWindowFont(font,header,2,measured))return false;
    for(auto item:items) {
        const auto* record=itemPresentation(item);if(!record || !messageIcon(record->icon))return false;
        const char* name=bank->find(record->name()),*description=bank->find(record->description());
        if(!measurePlainWindowFont(font,name,2,measured) || !measurePlainWindowFont(font,description,2,measured))return false;
        if(measured.lineFeeds+1>2 && (!messageIcon(445) || !messageIcon(446) || !messageIcon(120)))return false;
        names.push_back(name);descriptions.push_back(description);
    }
    if(!owner.enter(items))return false;
    names_=std::move(names);descriptions_=std::move(descriptions);font_=&font;
    messages_=bank;heading_=heading;header_=header;displaySlots_=0;iconTick=0;soundRequests.clear();statusOpen=false;return true;
}
void ItemSelectionPresentation::tick(const ItemSelectionInput& input,int32_t refresh,uint32_t counter) {
    displaySlots_=0;soundRequests.clear();if(!owner.retained())return;
    refreshRate_=refresh;
    ItemSelectionRuntimeServices services;
    services.user=this;services.sequence.user=this;
    services.sequence.refreshRate=[](void* p){return static_cast<ItemSelectionPresentation*>(p)->refreshRate_;};
    services.sequence.interpolate=[](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return *cameraInterpolation(mode,step,count,a,b);};
    services.sound=[](void* p,uint32_t id){static_cast<ItemSelectionPresentation*>(p)->soundRequests.push_back(id);};
    services.status=[](void* p,bool open){static_cast<ItemSelectionPresentation*>(p)->statusOpen=open;};
    services.descriptionMain=[](void* p,ItemSelectionRuntimeWindow& window,const ItemSelectionInput& input){
        ItemSelectionHelpServices font{p,[](void* p,const char* text){WindowFontMeasurement measured;
            const bool valid=measurePlainWindowFont(*static_cast<ItemSelectionPresentation*>(p)->font_,text,2,measured);assert(valid);return measured.lineFeeds;}};
        tickItemSelectionHelp(window,input,font);
    };
    services.registerDisplay=[](void* p,unsigned slot){static_cast<ItemSelectionPresentation*>(p)->displaySlots_|=1u<<slot;};
    owner.tick(input,services);iconTick=counter;
}
}

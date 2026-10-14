#include "item_selection_runtime.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <utility>
namespace ttyd {
void tickItemSelectionHelp(ItemSelectionRuntimeWindow& window,const ItemSelectionInput& input,
                          const ItemSelectionHelpServices& font) {
    if(window.sequence.flags&4)return;
    if(window.helpText){assert(font.lineFeeds);window.helpLines=int32_t(font.lineFeeds(font.user,window.helpText))+1;}
    if(window.helpLines<=2)return;
    if(input.directionTrigger&0x100)window.helpPage=std::max<int32_t>(window.helpPage-2,0);
    else if((input.directionTrigger&0x200) &&
            (window.helpLines+1)/2-1>(window.helpPage+1)/2)window.helpPage+=2;
}
bool ItemSelectionRuntime::enter(const std::vector<uint16_t>& items,bool cancel) {
    // Native admission policy; source authored quest tables are positive. Do
    // not invent source empty-list selection or permit overlapping ownership.
    if(retained_ || items.empty() || items.size()>size_t(std::numeric_limits<int32_t>::max()))return false;
    ItemSelectionMain main;main.flags=cancel?0x100:0;
    main.rows.reserve(items.size()+2);
    for(auto item:items)main.rows.push_back({0,item});
    main.windows={0,1,2};
    std::array<ItemSelectionRuntimeWindow,3> windows{};
    windows[0].rectangle={-24,116,300,240};windows[0].decoration=1;
    if(items.size()<8)windows[0].rectangle.height-=24*(8-int32_t(items.size()));
    windows[1].rectangle={-280,120,240,45};windows[1].sequence.sequence=2;
    windows[2].rectangle={-240,-130,500,80};
    for(auto& window:windows)window.sequence.flags=1;
    main_=std::move(main);windows_=windows;retained_=true;return true;
}
void ItemSelectionRuntime::tick(const ItemSelectionInput& input,const ItemSelectionRuntimeServices& services) {
    if(!retained_)return;
    assert(services.sound && services.status);
    struct Context {ItemSelectionRuntime* owner;const ItemSelectionRuntimeServices* services;} context{this,&services};
    ItemSelectionMainServices main{&context,
        [](void* p,uint32_t slot){return static_cast<Context*>(p)->owner->windows_[slot].sequence.flags;},
        [](void* p,uint32_t slot,int32_t action){auto& w=static_cast<Context*>(p)->owner->windows_[slot].sequence;w.action=action;w.clock=0;},
        [](void* p,uint32_t slot){auto& w=static_cast<Context*>(p)->owner->windows_[slot];w.helpPage=0;w.helpLines=0;w.helpText=nullptr;},
        [](void* p,uint32_t slot){static_cast<Context*>(p)->owner->windows_[slot].sequence.flags&=~1u;},
        [](void* p,uint32_t id){const auto& c=*static_cast<Context*>(p);c.services->sound(c.services->user,id);},
        [](void* p,bool open){const auto& c=*static_cast<Context*>(p);c.services->status(c.services->user,open);}};
    for(unsigned slot=0;slot<3;++slot) {
        auto& window=windows_[slot];
        if(!(window.sequence.flags&1))continue;
        tickItemSelectionSequence(window.sequence,services.sequence);
        if(slot==0)tickItemSelectionMain(main_,input,main);
        else if(slot==2 && services.descriptionMain)services.descriptionMain(services.user,window,input);
        if((window.sequence.flags&2) && services.registerDisplay)services.registerDisplay(services.user,slot);
    }
}
int32_t ItemSelectionRuntime::poll() const {
    if(!retained_ || !(main_.flags&0x1000))return 0;
    if(main_.flags&0x2000)return -1;
    return main_.rows[size_t(main_.selected)].item;
}
bool ItemSelectionRuntime::release() {
    if(!retained_ || !(main_.flags&0x1000))return false;
    main_=ItemSelectionMain{};windows_={};retained_=false;return true;
}
ItemSelectionRuntimeWindow& ItemSelectionRuntime::displayWindow(unsigned slot) {
    assert(retained_ && slot<3);return windows_[slot];
}
bool itemSelectionListContents(const ItemSelectionRuntimeWindow& window) {
    return (window.sequence.flags&2) && !(window.sequence.flags&4);
}
ItemSelectionClip itemSelectionRowsClip(const ItemSelectionRuntimeWindow& window) {
    const auto& r=window.rectangle;return {r.x+304,274-r.y,r.width,r.height-50};
}
ItemSelectionRowDraw itemSelectionRowDraw(const ItemSelectionMain& main,size_t row) {
    assert(row<main.rows.size());const auto& item=main.rows[row];
    const float offset=24.f*float(row);
    return {item.item,item.flags,float(main.listX+35),float(main.listY-44)-offset+main.displacement,
        float(main.listX+60),float(main.listY-32)-offset+main.displacement,.5f,185,
        item.flags&1?0xa0a0a0ffu:0x000000ffu};
}
ItemSelectionIconDraw itemSelectionCursorDraw(const ItemSelectionMain& main) {
    return {504,main.cursorX,main.cursorY,1,1,20};
}
ItemSelectionArrows itemSelectionArrowDraws(const ItemSelectionMain& main,
    const ItemSelectionRuntimeWindow& window,uint32_t counter) {
    ItemSelectionArrows result;
    if(main.rows.size()<=8 || (counter&31)>=20)return result;
    const auto& r=window.rectangle;const float x=std::fma(float(r.width),.5f,float(r.x));
    if(main.firstVisible!=0)result.icons[result.count++]={445,x,float(r.y-36),1,.6000000238418579f,16};
    if(main.firstVisible!=int32_t(main.rows.size())-8)
        result.icons[result.count++]={446,x,float(r.y-r.height-12),1,.6000000238418579f,16};
    return result;
}
ItemSelectionTextDraw itemSelectionHeadingDraw(const ItemSelectionRuntimeWindow& window,uint32_t measured) {
    const auto& r=window.rectangle;
    const float width=std::min(float(uint16_t(measured)),120.f);
    const int32_t center=int32_t((float(r.width)-width)*.5f);
    return {r.x+int32_t(uint16_t(center)),r.y+14,1,1,120,0xffffffff,true};
}
ItemSelectionTextDraw itemSelectionHeaderDraw(const ItemSelectionRuntimeWindow& window,uint32_t measured,uint16_t lines,uint32_t palette) {
    const auto& r=window.rectangle;float width=float(uint16_t(measured)),scale=1;
    if(width>float(r.width-30)){scale=float(r.width-30)/width;width=float(r.width-30);}
    const int32_t center=int32_t((float(r.width)-width)*.5f);
    return {r.x+int32_t(uint16_t(center)),r.y-(r.height-24*(int32_t(lines)+1))/2,
        scale,1,0,palette,false};
}
void itemSelectionHeaderAfterDraw(ItemSelectionRuntimeWindow& window,uint16_t lines) {
    if(!(window.sequence.flags&1))return;
    const int32_t increase=22*int32_t(lines);
    window.rectangle.y=120+increase/2;window.rectangle.height=45+increase;
}
ItemSelectionTextDraw itemSelectionHelpDraw(const ItemSelectionRuntimeWindow& window,uint32_t palette) {
    return {-210,int32_t(std::fma(28.f,float(window.helpPage),-144.f)),1,1,0,palette,false};
}
ItemSelectionHelpIcons itemSelectionHelpIconDraws(const ItemSelectionRuntimeWindow& window,uint32_t counter) {
    ItemSelectionHelpIcons result;if(window.helpLines<=2)return result;
    const auto& r=window.rectangle;const float x=float(r.x+r.width-12);
    const auto add=[&](uint32_t icon,int32_t y){result.icons[result.count++]={icon,x,float(y),0,.6000000238418579f,16};};
    if((counter&31)<20) {
        if(window.helpPage==0)add(446,r.y-r.height-4);
        else if(window.helpPage+2==window.helpLines)add(445,r.y-r.height+44);
        else {add(445,r.y-r.height+44);add(446,r.y-r.height-4);}
    }
    add(120,r.y-r.height+10);return result;
}
}

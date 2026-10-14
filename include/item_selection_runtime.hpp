#pragma once
#include "item_selection.hpp"
#include <array>
#include <cstddef>
namespace ttyd {
struct ItemSelectionRectangle {int32_t x=0,y=0,width=0,height=0;};
struct ItemSelectionRuntimeWindow {
    ItemSelectionWindow sequence;
    ItemSelectionRectangle rectangle;
    int32_t decoration=0,helpPage=0,helpLines=0;
    const char* helpText=nullptr;
};
struct ItemSelectionRuntimeServices {
    ItemSelectionSequenceServices sequence;
    void* user=nullptr;
    void (*sound)(void*,uint32_t sourceID)=nullptr;
    void (*status)(void*,bool open)=nullptr;
    // Original description main runs after its own sequence, and observes the
    // previous display's text/line publication. Font/input services own it.
    void (*descriptionMain)(void*,ItemSelectionRuntimeWindow&,const ItemSelectionInput&)=nullptr;
    // Registration occurs after each slot's sequence/main, in slot order.
    void (*registerDisplay)(void*,unsigned slot)=nullptr;
};
struct ItemSelectionHelpServices {
    void* user=nullptr;
    uint16_t (*lineFeeds)(void*,const char* text)=nullptr;
};
// select_main3: prior display text, source LF count, direction-trigger namespace.
void tickItemSelectionHelp(ItemSelectionRuntimeWindow&,const ItemSelectionInput&,
                           const ItemSelectionHelpServices&);
// Native retained owner for explicit tables with second argument0. This owns
// one three-window template; shared manager-slot allocation remains external.
class ItemSelectionRuntime {
public:
    bool enter(const std::vector<uint16_t>& items,bool allowCancel=true);
    void tick(const ItemSelectionInput&,const ItemSelectionRuntimeServices&);
    int32_t poll() const;
    int32_t selectedIndex() const {return main_.selected;}
    bool retained() const {return retained_;}
    bool release(); // Ready-only deletion; key restoration belongs to event caller.
    const ItemSelectionMain& main() const {return main_;}
    const std::array<ItemSelectionRuntimeWindow,3>& windows() const {return windows_;}
    // Source display callbacks publish help text and mutate the header rectangle.
    // Display services use this after their slot is registered, not during entry.
    ItemSelectionRuntimeWindow& displayWindow(unsigned slot);
private:
    bool retained_=false;
    ItemSelectionMain main_;
    std::array<ItemSelectionRuntimeWindow,3> windows_{};
};
struct ItemSelectionRowDraw {
    uint16_t item=0,flags=0;
    float iconX=0,iconY=0,nameX=0,nameY=0;
    float iconScale=.5f,nameWidth=185;
    uint32_t nameColor=0x000000ff; // Item icons remain white; flags1 uses their gray service.
};
struct ItemSelectionClip {int32_t x=0,y=0,width=0,height=0;};
struct ItemSelectionIconDraw {uint32_t icon=0;float x=0,y=0,z=1,scale=1;uint32_t flags=0;};
// Pure source-space requests: retain all rows for the original scissor, rather
// than throwing away easing rows outside an inferred eight-row visible range.
bool itemSelectionListContents(const ItemSelectionRuntimeWindow&);
ItemSelectionClip itemSelectionRowsClip(const ItemSelectionRuntimeWindow&);
ItemSelectionRowDraw itemSelectionRowDraw(const ItemSelectionMain&,size_t row);
ItemSelectionIconDraw itemSelectionCursorDraw(const ItemSelectionMain&);
struct ItemSelectionArrows {std::array<ItemSelectionIconDraw,2> icons{};unsigned count=0;};
ItemSelectionArrows itemSelectionArrowDraws(const ItemSelectionMain&,
    const ItemSelectionRuntimeWindow&,uint32_t sourceFrameCounter);
struct ItemSelectionTextDraw {
    float x=0,y=0;
    float scaleX=1,scaleY=1,widthLimit=0;
    uint32_t color=0;
    bool outlined=false;
};
// Measurements are original FontGetMessageWidth/WidthLine results supplied by
// the font service, not decoded glyph rectangle widths or bubble records.
ItemSelectionTextDraw itemSelectionHeadingDraw(const ItemSelectionRuntimeWindow&,uint32_t measuredWidth);
ItemSelectionTextDraw itemSelectionHeaderDraw(const ItemSelectionRuntimeWindow&,uint32_t measuredWidth,uint16_t lineFeeds,uint32_t fontPalette0);
void itemSelectionHeaderAfterDraw(ItemSelectionRuntimeWindow&,uint16_t lineFeeds);
ItemSelectionTextDraw itemSelectionHelpDraw(const ItemSelectionRuntimeWindow&,uint32_t fontPalette0);
struct ItemSelectionHelpIcons {std::array<ItemSelectionIconDraw,3> icons{};unsigned count=0;};
ItemSelectionHelpIcons itemSelectionHelpIconDraws(const ItemSelectionRuntimeWindow&,uint32_t sourceFrameCounter);
}

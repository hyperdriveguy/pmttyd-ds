#include "item_selection_presentation.hpp"
#include "item_presentation.hpp"
#include "message_icons.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main() {
    MessageFont font;assert(font.load("romfs/font.pmfont"));
    assert(loadItemPresentations("romfs/item-presentations.bin"));assert(loadMessageIcons("romfs/message-icons.bin"));
    assert(loadItemSelectorAssets("romfs"));
    ItemSelectionPresentation selector;
    assert(selector.begin({95,107,107},font));assert(!selector.begin({95},font));
    assert(std::strlen(selector.heading())==5 && std::strlen(selector.header())==11);
    assert(std::strlen(selector.name(0))==3 && std::strlen(selector.name(1))==13);
    assert(selector.name(1)==selector.name(2)); // Original row duplicates, shared immutable text.
    WindowFontMeasurement measured;
    assert(measurePlainWindowFont(font,selector.description(0),2,measured) && measured.lineFeeds==1);
    assert(measurePlainWindowFont(font,selector.description(1),2,measured) && measured.lineFeeds==1);
    selector.tick({},60,99);assert(selector.statusOpen && selector.displaySlots()==6);
    assert((selector.soundRequests==std::vector<uint32_t>{0x2002a}));
    for(unsigned i=0;i<10;++i)selector.tick({},60,99);
    assert(selector.displaySlots()==7 && selector.iconTick==99);
    const char* retainedName=selector.name(0);assert(loadItemSelectorAssets("romfs"));assert(selector.name(0)==retainedName && std::strlen(retainedName)==3);
    // Source help observes prior display publication, never inferred row text.
    auto& help=selector.owner.displayWindow(2);assert(help.helpLines==0);
    help.helpText=selector.description(0);selector.tick({},60,99);assert(help.helpLines==2 && help.helpPage==0);
    selector.tick({0x2000,0x2000,0},60,99);assert(selector.owner.selectedIndex()==1);
    selector.tick({0,0,0x100},60,99);assert(selector.owner.poll()==0);
    for(unsigned i=0;i<11;++i)selector.tick({},60,99);
    assert(selector.owner.poll()==107 && selector.owner.selectedIndex()==1 && !selector.statusOpen);
    assert(selector.displaySlots()==0 && selector.owner.release());
    assert(selector.begin({95},font));assert(selector.owner.main().rows.size()==1 && selector.iconTick==0);
    std::cout<<"Original selector text/metadata admission, duplicate rows, deferred publication and native retained presentation passed\n";
}

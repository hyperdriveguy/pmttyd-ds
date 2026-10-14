#include "item_selection_runtime.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <iostream>
#include <string>
using namespace ttyd;
struct Probe {
    std::vector<std::string> trace;
    unsigned descriptions=0;
    ItemSelectionRuntimeServices services() {
        return {{this,[](void*){return 60;},
            [](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return *cameraInterpolation(mode,step,count,a,b);}},this,
            [](void* p,uint32_t id){static_cast<Probe*>(p)->trace.push_back("sound"+std::to_string(id));},
            [](void* p,bool on){static_cast<Probe*>(p)->trace.push_back(on?"statusOn":"statusOff");},
            [](void* p,ItemSelectionRuntimeWindow&,const ItemSelectionInput&){++static_cast<Probe*>(p)->descriptions;},
            [](void* p,unsigned slot){static_cast<Probe*>(p)->trace.push_back("display"+std::to_string(slot));}};
    }
};
int main() {
    Probe probe;auto service=probe.services();ItemSelectionRuntime selector;
    assert(!selector.enter({}) && !selector.retained());
    assert(selector.enter({95,95,107}));assert(!selector.enter({107}));
    assert(selector.main().rows.size()==3 && selector.main().rows[0].item==95 && selector.main().rows[1].item==95);
    assert(selector.windows()[0].rectangle.height==120 && selector.windows()[1].rectangle.height==45);
    assert(selector.windows()[2].rectangle.x==-240 && selector.windows()[2].rectangle.y==-130);
    assert(selector.poll()==0 && !selector.release());
    selector.tick({0,0,0x100},service); // State0 requests open, not confirmation.
    assert(selector.main().state==1 && selector.poll()==0);
    assert((probe.trace==std::vector<std::string>{"sound131114","statusOn","display1","display2"}));
    // Header and description process their newly requested action this pass.
    // List already ran its sequence before its state0 main requested opening.
    assert(selector.windows()[0].sequence.clock==0 && selector.windows()[2].sequence.clock==1);
    assert(selector.windows()[1].sequence.flags==7 && selector.windows()[1].sequence.action==0);
    assert(!itemSelectionListContents(selector.windows()[0]));
    for(unsigned i=0;i<9;++i)selector.tick({0x2000,0x2000,0x100},service);
    assert(selector.main().state==1 && selector.selectedIndex()==0 && selector.windows()[0].sequence.action==0);
    assert(!itemSelectionListContents(selector.windows()[0]));
    selector.tick({0x2000,0x2000,0},service);
    assert(selector.selectedIndex()==1 && itemSelectionListContents(selector.windows()[0]));
    // Duplicate row identity survives navigation; A beats simultaneous B.
    selector.tick({0,0,0x300},service);assert(selector.main().state==2 && selector.poll()==0);
    selector.tick({},service);assert(selector.main().state==3 && selector.poll()==0 && !selector.release());
    assert(selector.windows()[0].sequence.clock==0 && selector.windows()[2].sequence.clock==1);
    for(unsigned i=0;i<9;++i)selector.tick({},service);
    assert(selector.poll()==0 && selector.retained());
    const unsigned descriptions=probe.descriptions;
    selector.tick({},service);assert(selector.poll()==95 && selector.selectedIndex()==1);
    assert(probe.descriptions==descriptions); // List cleanup prevents later slot invocation.
    for(const auto& w:selector.windows())assert(!(w.sequence.flags&1));
    const auto trace=probe.trace;selector.tick({0,0,0x100},service);assert(probe.trace==trace);
    assert(selector.release() && !selector.retained() && selector.poll()==0);
    assert(!selector.release());
    assert(selector.enter({107}));assert(selector.windows()[0].rectangle.height==72);
    for(unsigned i=0;i<11;++i)selector.tick({},service);
    selector.tick({0,0,0x200},service);assert(selector.main().state==2);
    for(unsigned i=0;i<11;++i)selector.tick({},service);
    assert(selector.poll()==-1 && selector.release());
    assert(selector.enter({65535},false));for(unsigned i=0;i<11;++i)selector.tick({},service);
    selector.tick({0,0,0x200},service);assert(selector.main().state==1);
    selector.tick({0,0,0x100},service);for(unsigned i=0;i<11;++i)selector.tick({},service);
    assert(selector.poll()==65535 && selector.release()); // No narrowing to signed item result.
    std::vector<uint16_t> items(12,107);assert(selector.enter(items));
    assert(selector.windows()[0].rectangle.height==240);
    auto main=selector.main();main.firstVisible=2;main.displacement=5.5f;main.cursorX=-20;main.cursorY=-43;
    auto row=itemSelectionRowDraw(main,7);
    assert(row.item==107 && row.iconX==11 && row.iconY==-90.5f && row.nameX==36 && row.nameY==-78.5f);
    assert(row.iconScale==.5f && row.nameWidth==185 && row.nameColor==0x000000ff);
    main.rows[7].flags=1;assert(itemSelectionRowDraw(main,7).nameColor==0xa0a0a0ff);
    const auto clip=itemSelectionRowsClip(selector.windows()[0]);
    assert(clip.x==280 && clip.y==158 && clip.width==300 && clip.height==190);
    const auto cursor=itemSelectionCursorDraw(main);
    assert(cursor.icon==504 && cursor.x==-20 && cursor.y==-43 && cursor.z==1 && cursor.scale==1 && cursor.flags==20);
    auto arrows=itemSelectionArrowDraws(main,selector.windows()[0],19);
    assert(arrows.count==2 && arrows.icons[0].icon==445 && arrows.icons[0].x==126 && arrows.icons[0].y==80);
    assert(arrows.icons[1].icon==446 && arrows.icons[1].y==-136 && arrows.icons[1].flags==16);
    assert(itemSelectionArrowDraws(main,selector.windows()[0],20).count==0);
    assert(itemSelectionArrowDraws(main,selector.windows()[0],31).count==0);
    assert(itemSelectionArrowDraws(main,selector.windows()[0],32).count==2);
    main.firstVisible=0;assert(itemSelectionArrowDraws(main,selector.windows()[0],0).count==1);
    main.firstVisible=4;arrows=itemSelectionArrowDraws(main,selector.windows()[0],0);
    assert(arrows.count==1 && arrows.icons[0].icon==445);
    main.rows.resize(8);assert(itemSelectionArrowDraws(main,selector.windows()[0],0).count==0);
    const auto heading=itemSelectionHeadingDraw(selector.windows()[0],0x100a0);
    assert(heading.x==66 && heading.y==130 && heading.widthLimit==120 && heading.outlined && heading.color==0xffffffff);
    auto narrow=selector.windows()[0];narrow.rectangle.width=20;
    assert(itemSelectionHeadingDraw(narrow,120).x==65462); // Source zero-extends low16 of negative center.
    auto& header=selector.displayWindow(1);header.sequence.flags=3;
    const auto headerText=itemSelectionHeaderDraw(header,400,2,0x1e1414ff);
    assert(headerText.x==-265 && headerText.y==133 && headerText.scaleX==.525f && headerText.scaleY==1);
    assert(header.rectangle.y==120 && header.rectangle.height==45); // Text uses old rectangle.
    itemSelectionHeaderAfterDraw(header,2);
    assert(header.rectangle.x==-280 && header.rectangle.y==142 && header.rectangle.width==240 && header.rectangle.height==89);
    const auto customPalette=itemSelectionHeaderDraw(header,100,0,0x11223344);
    assert(customPalette.color==0x11223344); // DrawStart reads mutable original palette0.
    header.sequence.flags=0;itemSelectionHeaderAfterDraw(header,0);assert(header.rectangle.y==142);
    auto& help=selector.displayWindow(2);help.helpLines=5;help.helpPage=0;
    auto helpIcons=itemSelectionHelpIconDraws(help,0);
    assert(helpIcons.count==2 && helpIcons.icons[0].icon==446 && helpIcons.icons[0].x==248 && helpIcons.icons[0].y==-214);
    assert(helpIcons.icons[1].icon==120 && helpIcons.icons[1].y==-200 && helpIcons.icons[1].z==0);
    help.helpPage=2;helpIcons=itemSelectionHelpIconDraws(help,0);
    assert(helpIcons.count==3 && helpIcons.icons[0].icon==445 && helpIcons.icons[0].y==-166 && helpIcons.icons[1].icon==446);
    help.helpPage=3;helpIcons=itemSelectionHelpIconDraws(help,0);assert(helpIcons.count==2 && helpIcons.icons[0].icon==445);
    helpIcons=itemSelectionHelpIconDraws(help,20);assert(helpIcons.count==1 && helpIcons.icons[0].icon==120);
    const auto helpText=itemSelectionHelpDraw(help,0x1e1414ff);
    assert(helpText.x==-210 && helpText.y==-60 && helpText.color==0x1e1414ff);
    help.helpLines=2;assert(itemSelectionHelpIconDraws(help,0).count==0);
    struct FontProbe {unsigned calls=0;uint16_t lines=4;} font;
    ItemSelectionHelpServices fontService{&font,[](void* p,const char* text){assert(std::string(text)=="prior-text");auto& f=*static_cast<FontProbe*>(p);++f.calls;return f.lines;}};
    help.sequence.flags=7;help.helpText="prior-text";help.helpPage=0;help.helpLines=0;
    tickItemSelectionHelp(help,{0,0x200,0},fontService);assert(font.calls==0 && help.helpLines==0 && help.helpPage==0);
    help.sequence.flags=3;tickItemSelectionHelp(help,{0,0x200,0},fontService);
    assert(font.calls==1 && help.helpLines==5 && help.helpPage==2);
    tickItemSelectionHelp(help,{0,0x200,0},fontService);assert(help.helpPage==4);
    tickItemSelectionHelp(help,{0,0x200,0},fontService);assert(help.helpPage==4);
    tickItemSelectionHelp(help,{0,0x300,0x100},fontService);assert(help.helpPage==2); // Horizontal left wins, not A.
    tickItemSelectionHelp(help,{0,0x100,0},fontService);assert(help.helpPage==0);
    tickItemSelectionHelp(help,{0,0x100,0},fontService);assert(help.helpPage==0);
    help.helpText=nullptr;const auto calls=font.calls;
    tickItemSelectionHelp(help,{0,0,0x200},fontService);assert(font.calls==calls && help.helpLines==5 && help.helpPage==0);
    help.helpText="prior-text";font.lines=1;tickItemSelectionHelp(help,{0,0x200,0},fontService);
    assert(help.helpLines==2 && help.helpPage==0);
    std::cout<<"Retained original three-window selector ordering, close-before-result and source draw requests passed\n";
}

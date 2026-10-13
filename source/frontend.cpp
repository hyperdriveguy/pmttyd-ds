#include "frontend.hpp"
namespace ttyd {
std::string saveSlotPath(const std::string& directory,unsigned slot) {
    if(slot>=4)return {};
    // Slot0 preserves existing saves and checkpoint fixtures without migration.
    return directory+(slot==0?"/progress.bin":"/progress"+std::to_string(slot+1)+".bin");
}
void scanSaveSlots(Frontend& f,const std::string& directory) {
    for(unsigned n=0;n<4;++n){f.slots[n]={};f.slots[n].result=loadSave(saveSlotPath(directory,n),f.slots[n].progress);}
}
void Frontend::tick(const FrontInput& i) {
    if(screen==FrontScreen::Demo){if(i.start || i.confirm)screen=FrontScreen::Title;return;}
    if(screen==FrontScreen::Title){if(i.start || i.confirm)screen=FrontScreen::Files;return;}
    if(screen!=FrontScreen::Files || loadRequested)return;
    if(i.cancel){screen=FrontScreen::Title;return;}
    if(i.previous)selected=(selected+3)%4;
    if(i.next)selected=(selected+1)%4;
    const bool touch=i.touchedSlot>=0 && i.touchedSlot<4;
    if(touch)selected=unsigned(i.touchedSlot);
    if(!(i.confirm || i.start || touch))return;
    if(slots[selected].occupied())loadRequested=true;
    else if(slots[selected].result==LoadResult::Missing){newRequested=true;screen=FrontScreen::Opening;}
    // An invalid occupied slot is never silently replaced by New Game.
}
}

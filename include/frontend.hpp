#pragma once
#include "save.hpp"
#include "opening_title.hpp"
#include <array>
namespace ttyd {
enum class FrontScreen { Demo, Title, Files, Opening, Playing };
struct FrontInput {bool confirm=false,cancel=false,start=false,previous=false,next=false;int touchedSlot=-1;};
struct SaveSlot {LoadResult result=LoadResult::Missing;Progress progress;bool occupied() const{return result==LoadResult::Loaded || result==LoadResult::Recovered;}};
// Native menu/controller adaptation, separate from the original card driver.
class Frontend {
public:
    OpeningTitle title;
    FrontScreen screen=FrontScreen::Demo;
    std::array<SaveSlot,4> slots{};
    unsigned selected=0;
    bool loadRequested=false,newRequested=false;
    void tick(const FrontInput&);
    void demoComplete(){if(screen==FrontScreen::Demo){screen=FrontScreen::Title;title.begin();}}
    void openingComplete(){if(screen==FrontScreen::Opening)screen=FrontScreen::Playing;}
    void rejectLoad(){loadRequested=false;screen=FrontScreen::Files;}
    void acceptLoad(){loadRequested=false;screen=FrontScreen::Playing;}
};
std::string saveSlotPath(const std::string& directory,unsigned slot);
void scanSaveSlots(Frontend&,const std::string& directory);
}

#include "frontend.hpp"
#include <cassert>
#include <filesystem>
#include <unistd.h>
using namespace ttyd;
int main(){
    Frontend f;FrontInput a;a.confirm=true;
    f.tick(a);assert(f.screen==FrontScreen::Title);f.tick(a);assert(f.screen==FrontScreen::Files);
    f.tick(a);assert(f.newRequested && f.screen==FrontScreen::Opening);f.openingComplete();assert(f.screen==FrontScreen::Playing);
    Frontend load;load.demoComplete();load.tick(a);load.slots[0].result=LoadResult::Recovered;load.tick(a);
    assert(load.loadRequested && !load.newRequested);load.rejectLoad();assert(load.screen==FrontScreen::Files);
    load.slots[0].result=LoadResult::Invalid;load.tick(a);assert(!load.loadRequested && !load.newRequested);
    FrontInput b;b.cancel=true;load.tick(b);assert(load.screen==FrontScreen::Title);
    load.tick(a);FrontInput left;left.previous=true;load.tick(left);assert(load.selected==3);
    FrontInput touch;touch.touchedSlot=1;load.tick(touch);assert(load.selected==1 && load.newRequested);
    assert(saveSlotPath("/x",0)=="/x/progress.bin" && saveSlotPath("/x",3)=="/x/progress4.bin" && saveSlotPath("/x",4).empty());
    const auto path=std::filesystem::temp_directory_path()/("ttyd-front-"+std::to_string(getpid()));std::filesystem::create_directory(path);
    Progress p;p.originalStory=3;assert(writeSave(saveSlotPath(path.string(),2),p));
    Frontend disk;scanSaveSlots(disk,path.string());assert(disk.slots[2].occupied() && disk.slots[2].progress.originalStory==3);
    assert(disk.slots[0].result==LoadResult::Missing && disk.slots[3].result==LoadResult::Missing);std::filesystem::remove_all(path);
}

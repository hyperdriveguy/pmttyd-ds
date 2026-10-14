#include "frontend.hpp"
#include "game.hpp"
#include <cassert>
#include <filesystem>
#include <unistd.h>
using namespace ttyd;
static void titleReady(Frontend& f){
    f.title.admitMusic(true);f.title.admitResources();
    for(unsigned n=0;n<300 && !f.title.inputReady();++n){
        f.title.admitCurtain(f.title.curtainType);f.tick({});
        f.title.presented(f.title.curtainType,f.title.curtainFrame(),f.title.paperScale);
    }
    assert(f.title.inputReady());
}
static void chooseFiles(Frontend& f,const FrontInput& a){
    titleReady(f);f.tick(a);assert(f.screen==FrontScreen::Title);
    for(unsigned n=0;n<200 && f.screen==FrontScreen::Title;++n){
        f.title.admitCurtain(f.title.curtainType);f.tick({});
        f.title.presented(f.title.curtainType,f.title.curtainFrame(),f.title.paperScale);
    }
    assert(f.screen==FrontScreen::Files);
}
int main(){
    Frontend f;FrontInput a;a.confirm=true;
    f.tick(a);assert(f.screen==FrontScreen::Demo);f.demoComplete();assert(f.screen==FrontScreen::Title);chooseFiles(f,a);
    f.tick(a);assert(f.newRequested && f.screen==FrontScreen::Opening);f.openingComplete();assert(f.screen==FrontScreen::Playing);
    Frontend load;load.demoComplete();chooseFiles(load,a);load.slots[0].result=LoadResult::Recovered;load.tick(a);
    assert(load.loadRequested && !load.newRequested);load.rejectLoad();assert(load.screen==FrontScreen::Files);
    load.slots[0].result=LoadResult::Invalid;load.tick(a);assert(!load.loadRequested && !load.newRequested);
    FrontInput b;b.cancel=true;load.tick(b);assert(load.screen==FrontScreen::Title);
    chooseFiles(load,a);FrontInput left;left.previous=true;load.tick(left);assert(load.selected==3);
    FrontInput touch;touch.touchedSlot=1;load.tick(touch);assert(load.selected==1 && load.newRequested);
    assert(saveSlotPath("/x",0)=="/x/progress.bin" && saveSlotPath("/x",3)=="/x/progress4.bin" && saveSlotPath("/x",4).empty());
    // The same physical press must not be replayed by a fresh field owner.
    Game front;std::array<RawPad,4> pads{};pads[0].buttons=BattleButtonA;front.publishPhysicalInput(pads);
    Game field;Progress start;start.position={-4,4};field.restore(start);field.retainPhysicalInput(front);
    Input held;held.physicalSnapshot=true;held.physicalButtons=BattleButtonA;held.jumpHeld=true;
    field.tick(held);assert(!(field.physicalChannel().pressed&BattleButtonA) && !field.airborne());
    held.physicalButtons=0;held.jumpHeld=false;field.tick(held);
    held.physicalButtons=BattleButtonA;held.jumpHeld=true;field.tick(held);assert(field.physicalChannel().pressed&BattleButtonA);
    const auto path=std::filesystem::temp_directory_path()/("ttyd-front-"+std::to_string(getpid()));std::filesystem::create_directory(path);
    Progress p;p.originalStory=3;assert(writeSave(saveSlotPath(path.string(),2),p));
    Frontend disk;scanSaveSlots(disk,path.string());assert(disk.slots[2].occupied() && disk.slots[2].progress.originalStory==3);
    assert(disk.slots[0].result==LoadResult::Missing && disk.slots[3].result==LoadResult::Missing);std::filesystem::remove_all(path);
}

#include "game.hpp"
#include "save.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <unistd.h>
using namespace ttyd;
int main() {
    assert(loadFieldActorCatalog("romfs") && loadFieldSaveObjectAssets("romfs"));
    assert(loadGround("romfs/map.ground") && loadCollision("romfs/map.walls") && loadHead("romfs/map.head"));
    char temporary[]="/tmp/pm-live-save-XXXXXX";const int fd=mkstemp(temporary);assert(fd>=0);close(fd);
    for(int choice:{1,0}) {
        Game game;Progress checkpoint;checkpoint.originalStory=8;checkpoint.crumpDefeated=true;checkpoint.position={1.85f,-.55f};
        game.restore(checkpoint);assert(writeSave(temporary,checkpoint));
        assert(game.saveObjects->instances().size()==1);
        const uint32_t owner=game.saveObjects->instances()[0].state.owner;
        assert(game.saveObjects->find(owner)->frame==0);
        Input jump;jump.confirm=true;jump.jumpHeld=true;game.tick(jump);
        bool hit=false,animated=false,launched=false;unsigned writes=0;uint64_t request=0,requestedAt=0;
        const uint32_t camera=game.fieldCameraFlags;
        for(unsigned tick=0;tick<6000;++tick) {
            Input input;input.jumpHeld=tick<15;
            auto& objects=*game.saveObjects;auto& p=objects.presentation;
            if(p.selection.state==1) {
                input.physicalSnapshot=true;
                input.physicalButtons=p.selection.current==choice?0x100u:4u;
                if(game.physicalChannel().buttons&input.physicalButtons)input.physicalButtons=0;
            } else if(p.message.waiting() || p.message.state==1)input.confirm=true;
            game.tick(input);
            const auto* object=objects.find(owner);assert(object);
            if(object->state.stage==1) {
                hit=true;animated=true;assert(object->pose=="A_1" && game.saveLocked() && (objects.camera8Flags&0x200));
                assert((game.fieldCameraFlags&0x200)==(camera&0x200));
            }
            if(p.event.started)launched=true;
            if(game.saveLocked())assert(!game.saveRequested);
            uint64_t ticket;SaveBlockSwitches switches;
            if(p.takeWriteRequest(ticket,switches)) {
                assert(!request);request=ticket;requestedAt=game.ticks;++writes;
                auto saved=game.progress;saved.originalFlag233=switches.firstHit;saved.originalFlag236=switches.postStoryHit;
                saved.originalSaveMode=switches.mode;saved.originalStory=switches.story;
                assert(writeSave(temporary,saved));
            }
            if(request && game.ticks-requestedAt>=10){assert(p.completeWrite(request,true));request=0;}
            if(launched && p.event.finished && !game.saveLocked() && !p.message.active())break;
        }
        assert(hit && animated && launched && !game.saveLocked() && !game.saveObjects->failed);
        assert(game.saveObjects->presentation.event.finished && game.scene==Scene::Explore && !game.airborne());
        assert(game.saveObjects->keys.depth==0 && !(game.saveObjects->camera8Flags&0x200));
        assert(game.progress.originalFlag233 && game.progress.originalFlag236);
        assert(writes==unsigned(choice==0));
        Progress saved;assert(loadSave(temporary,saved)==LoadResult::Loaded);
        assert(saved.originalFlag233==(choice==0) && saved.originalFlag236==(choice==0));
        assert(game.saveObjects->find(owner)->pose=="S_1");
    }
    // Per-instance clocks survive render cadence and name-equal contacts cannot retrigger.
    FieldSaveObjects objects;objects.switches={8,true,true,0};objects.configure(0,8,0,0,1000);
    const uint32_t owner=objects.instances()[0].state.owner;
    objects.tickObjects(1000,owner,0,0);assert(objects.find(owner)->pose=="A_1" && objects.find(owner)->ratio==0);
    objects.tickObjects(1250,owner,0,0);assert(objects.find(owner)->state.stage==1 && objects.find(owner)->ratio==1);
    objects.tickObjects(1267,owner,0,0);assert(objects.find(owner)->state.stage==3 && objects.find(owner)->pose=="S_1" && objects.find(owner)->ratio==0);
    std::remove(temporary);std::remove((std::string(temporary)+".bak").c_str());
}

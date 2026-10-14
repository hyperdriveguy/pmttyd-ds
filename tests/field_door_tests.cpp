#include "field_door.hpp"
#include "game.hpp"
#include "save.hpp"
#include <cassert>
#include <cstdio>
#include <string>
using namespace ttyd;
int main(int argc,char**) {
    RoadCaseAdmission gate;
    assert(fieldDoorAdmitted(10,10,0x100,0,false,false,gate));
    assert(!fieldDoorAdmitted(11,10,0x100,0,false,false,gate));
    assert(!fieldDoorAdmitted(0,0,0x100,0,false,false,gate));
    assert(!fieldDoorAdmitted(10,10,0,0,false,false,gate));
    assert(!fieldDoorAdmitted(10,10,0x100,3,false,false,gate));
    assert(fieldDoorAdmitted(10,10,0x100,3,true,false,gate));
    assert(!fieldDoorAdmitted(10,10,0x100,0,false,true,gate));
    gate.window=true;assert(!fieldDoorAdmitted(10,10,0x100,0,false,false,gate));
    FieldDoorRecord door;door.mask=1;door.outside={0,0,.4f};door.inside={0,0,-.4f};
    FieldDoorState state;state.begin(0,door,{0,.4f});
    assert(state.opening && state.interiorMask==1);
    int ticks=0;while(state.active() && ticks<200){state.tick(door);++ticks;}
    assert(!state.active() && ticks>=62 && state.position[1]==-.4f && state.interiorMask==1);
    state.begin(0,door,state.position);assert(!state.opening);
    ticks=0;while(state.active() && ticks<200){state.tick(door);++ticks;}
    assert(!state.active() && ticks>=62 && state.position[1]==.4f && !state.interiorMask);
    if(argc>1) {
        assert(loadHarborStoryAssets("romfs") && loadFieldActorCatalog("romfs"));
        assert(loadFieldDoors("romfs/doors.pmdoor"));assert(fieldDoors().size()==3);
        for(const auto& d:fieldDoors())assert(d.openNode && d.closeNode && d.outside!=d.inside);
        const auto first=fieldDoors()[0].openNode;
        assert(!loadFieldDoors("/missing-door-pack"));assert(fieldDoors()[0].openNode==first);
        assert(fieldDoors()[2].mask==4 && fieldDoors()[2].storyMinimum==23);
        assert(!fieldDoors()[2].unlocked({}) && !fieldDoors()[2].unlocked(22) && fieldDoors()[2].unlocked(23));
        assert(fieldDoors()[2].outsideNodes.size()>5 && !fieldDoors()[2].interiorNodes.empty());
        for(const auto& d:fieldDoors()) {
            clearCollision();const char* prefix=roomAsset(d.room);
            assert(loadCollision((std::string("romfs/")+prefix+".walls").c_str()));
            assert(loadGround((std::string("romfs/")+prefix+".ground").c_str()));
            assert(loadHead((std::string("romfs/")+prefix+".head").c_str()));
            Progress p;p.room=d.room;p.position={d.outside[0],d.outside[2]};p.originalStory=d.mask==4?23:7;
            Game g;g.restore(p);
            Input forward;forward.z=-1;
            for(int tick=0;tick<24;++tick)g.tick(forward);
            assert(g.wallContact.sourceNode==d.openNode);
            Input a;a.confirm=true;
            if(d.mask==4) {
                auto locked=p;locked.originalStory=22;g.restore(locked);
                for(int t=0;t<24;++t)g.tick(forward);
                assert(g.wallContact.sourceNode==d.openNode);g.tick(a);
                assert(!g.fieldDoor.active() && !g.fieldDoor.interiorMask && g.troubleDoorNotice);
                assert(g.fieldActorTalk.message.id()=="gor_02_035" && g.fieldActorTalk.actor.key==0 && g.fieldActorTalk.message.placementOverride==0 && g.fieldActorTalk.message.windowType==4);
                assert(g.fieldMarioBattleState().keyDepth==1 && g.fieldKeysOff);
                int t=0;while(g.fieldActorTalk.message.active() && t++<3000)g.tick(a);
                assert(!g.troubleDoorNotice && !g.fieldKeysOff && g.fieldMarioBattleState().keyDepth==0 && g.progress.originalStory==22);
                g.restore(p);for(int t=0;t<24;++t)g.tick(forward);
            }
            g.tick(a);assert(g.fieldDoor.active());
            int tick=0;while(g.fieldDoor.active() && tick++<300)g.tick({});
            assert(!g.fieldDoor.active() && (g.fieldDoor.interiorMask&d.mask));
            assert(std::fabs(g.progress.position.z-d.inside[2])<.001f);
            g.saveRequested=true;g.tick({});assert(g.saveRequested);
            Progress restored;assert(decodeSave(encodeSave(g.progress),restored));
            assert(restored.interiorMask==d.mask && restored.originalStory==p.originalStory);
            g.restore(restored);assert(g.fieldDoor.interiorMask==d.mask && !g.fieldDoor.active());
            Input backward;backward.z=1;
            for(int t=0;t<24;++t)g.tick(backward);
            assert(g.wallContact.sourceNode==d.closeNode);
            g.tick(a);assert(g.fieldDoor.active());
            g.saveRequested=true;g.tick({});assert(!g.saveRequested);
            tick=0;while(g.fieldDoor.active() && tick++<300)g.tick({});
            assert(!g.fieldDoor.active() && !(g.fieldDoor.interiorMask&d.mask));
            assert(g.progress.originalStory==p.originalStory);
            assert(g.progress.interiorMask==0);
        }
    }
    std::puts("Source door contact gates and scoped movement/wait bridge passed");
}

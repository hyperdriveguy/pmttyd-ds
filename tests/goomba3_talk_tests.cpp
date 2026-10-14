#include "game.hpp"
#include "camera_road_assets.hpp"
#include <cassert>
using namespace ttyd;
int main() {
    FieldActorRecord actor;actor.room=0;actor.key=0x42ff5f1f;actor.talkRulesBound=true;
    actor.talkRules={{0,56,"gor_01_028_00"},{3,0,"gor_01_028_08"}};
    FieldActorTalkState state;
    assert(!fieldActorTalkAvailable(actor,8,state));
    state.quest1420=0;assert(fieldActorTalkKey(actor,8,state)=="gor_01_028_00");
    state.quest1420=13;assert(fieldActorTalkKey(actor,500,state)=="gor_01_028_08");
    state.quest1420=2;assert(!fieldActorTalkAvailable(actor,8,state));
    state.flag5405=true;assert(!fieldActorTalkAvailable(actor,8,state));
    state.flag5406=false;assert(fieldActorTalkKey(actor,8,state)=="irai_02_01");
    state.flag5405=false;assert(!fieldActorTalkAvailable(actor,8,state)); // Acquisition owner pending.
    state.flag5406=true;assert(!fieldActorTalkAvailable(actor,8,state)); // Reward owner pending.
    assert(loadFieldActorCatalog("romfs") && loadHarborStoryAssets("romfs") && loadCameraRoadAssets("romfs"));
    const FieldActorRecord* source=nullptr;
    for(const auto& a:fieldActorCatalog().actors())if(a.room==0 && a.key==actor.key)source=&a;
    assert(source && source->npcFlags==0x80603 && source->pose=="S_1" && source->wander.enabled);
    assert(source->wander.radius==100 && source->wander.speed==30);
    assert((source->position==std::array<float,3>{-400,0,-325}) && source->yaw==90);
    assert(source->visible(0,8,0) && !source->visible(0,56,0));
    assert(fieldActorCatalog().models()[source->model].originalName=="c_kuribo_n");
    MessageAssets original;assert(original.loadGlobal("romfs/messages/global.txt") && original.loadRoom("romfs/messages/gor_01.txt"));
    assert(loadCollision("romfs/map.walls") && loadGround("romfs/map.ground") && loadHead("romfs/map.head"));
    Game wandering;Progress far;far.room=0;far.originalStory=8;far.position={0,0};far.originalNpcTalkState.quest1420=0;
    wandering.restore(far);bool moved=false,contact=false,walked=false;
    for(int n=0;n<2000;++n) {
        wandering.tick({});const auto* live=wandering.fieldNpcState(source->key);
        assert(live && live->initialized && live->presentationReady && live->body.flags==0x00480603);
        assert(live->body.root.y==0 && live->movement.components.y==0 && !(live->body.flags&0x40000000));
        moved|=live->body.root.x!=source->position[0] || live->body.root.z!=source->position[2];
        contact|=live->movement.wall!=0;walked|=live->pose=="W_1";
    }
    assert(moved && contact && walked); // Real original geometry invokes map-side response.
    for(bool waiting:{false,true}) {
        Progress checkpoint;checkpoint.room=0;checkpoint.originalStory=8;checkpoint.position={-4,-3.55f};
        checkpoint.originalNpcTalkState.quest1420=waiting?2:0;
        checkpoint.originalNpcTalkState.flag5405=waiting;checkpoint.originalNpcTalkState.flag5406=false;
        assert(loadCollision((std::string("romfs/")+roomAsset(0)+".walls").c_str()) && loadGround((std::string("romfs/")+roomAsset(0)+".ground").c_str()) && loadHead((std::string("romfs/")+roomAsset(0)+".head").c_str()));
        Game game;game.restore(checkpoint);game.marioPresentation.display=game.marioPresentation.target=180;
        for(int n=0;n<360;++n)game.tick({});
        const auto* candidate=game.nearbyFieldActor();assert(candidate && candidate->key==source->key);
        Input confirm;confirm.confirm=true;game.tick(confirm);game.tick({});
        const auto id=waiting?"irai_02_01":"gor_01_028_00";
        assert(game.fieldActorTalk.message.active() && game.fieldActorTalk.message.id()==id);
        assert(game.fieldActorTalk.actor.key==source->key && game.fieldActorTalk.message.rawText()==original.find(id));
        for(int n=0;n<5000 && game.fieldNpcTalkActive();++n)game.tick(confirm);
        assert(!game.fieldNpcTalkActive() && game.scene==Scene::Explore && game.progress.originalStory==8);
        assert(game.progress.originalNpcTalkState.quest1420==checkpoint.originalNpcTalkState.quest1420);
        assert(game.progress.originalNpcTalkState.flag5405==checkpoint.originalNpcTalkState.flag5405 && game.progress.originalNpcTalkState.flag5406==false);
    }
}

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
    // Actual Game NPC admission delegates quest2 to the acquisition owner.
    // Presentation is injected here; normal native providers remain unbound.
    {
        Progress checkpoint;checkpoint.room=0;checkpoint.originalStory=8;checkpoint.position={-4,-3.55f};
        checkpoint.originalNpcTalkState.quest1420=2;checkpoint.originalNpcTalkState.flag5405=false;
        checkpoint.originalNpcTalkState.flag5406=false;checkpoint.originalPouch=std::make_shared<ProloguePouch>(initializeProloguePouch("Yoshi"));
        Game game;game.restore(checkpoint);game.marioPresentation.display=game.marioPresentation.target=180;
        for(int n=0;n<360;++n)game.tick({});
        assert(!game.nearbyFieldActor()); // Unsupported presentation cannot advertise acquisition.
        FieldItemGetServices services;services.user=&game;
        services.ready=[](void*){return true;};
        game.bindFieldItemGetPresentation(services);assert(!game.nearbyFieldActor());
        services.mario=[](void* p){const auto& g=*static_cast<Game*>(p);return ItemGetPoint{g.progress.position.x*100,0,g.progress.position.z*100};};
        services.request=[](void* p,ItemGetRequest r){
            if(r==ItemGetRequest::GetItemMotion || r==ItemGetRequest::StayMotion ||
               r==ItemGetRequest::ControlOff || r==ItemGetRequest::ControlOn)
                assert(static_cast<Game*>(p)->applyFieldItemStateRequest(r));
            else if(r==ItemGetRequest::KeyOff || r==ItemGetRequest::KeyOn || r==ItemGetRequest::SystemLevel2 ||
                    r==ItemGetRequest::SystemLevel0 || r==ItemGetRequest::Camera8FlagOn || r==ItemGetRequest::Camera8FlagOff ||
                    r==ItemGetRequest::SoundFlag80On || r==ItemGetRequest::SoundFlag80Off || r==ItemGetRequest::ClearMarioMotion || r==ItemGetRequest::Camera4)
                assert(static_cast<Game*>(p)->applyFieldItemStateRequest(r));
        };
        services.effect=[](void*,int32_t,ItemGetPoint){return uintptr_t(1);};
        services.window=[](void*,uint32_t definition,const char*){return definition==0x80313dc8?1:2;};
        services.windowAction=[](void*,int32_t){return int32_t(0);};
        services.bgmSilent=[](void*){return false;};services.bgmCheck=[](void*,int32_t){return int32_t(0);};
        game.bindFieldItemGetPresentation(services);assert(!game.nearbyFieldActor());
        game.bindFieldItemMarioPost({&game,[](void* p,unsigned old){assert(old==0 && static_cast<Game*>(p)->fieldMarioMotion()==15);}});
        const auto* candidate=game.nearbyFieldActor();assert(candidate && candidate->key==source->key);
        Input confirm;confirm.confirm=true;game.tick(confirm);game.tick({});
        assert(game.fieldAcquisition().active() && game.fieldActorTalk.message.id()=="irai_02_00");
        assert(game.fieldMarioBattleState().keyDepth==1 && (game.fieldMarioBattleState().flags&8));
        Game snapshot=game;assert(snapshot.fieldAcquisition().active() && !snapshot.fieldAcquisition().canAdvance());
        for(int n=0;n<1500;++n)snapshot.tick(confirm);
        assert(!game.fieldItems().find("irai_item") && !game.progress.originalPouch->keyItemCount(95));
        assert(game.fieldActorTalk.message.active() && game.fieldAcquisition().canAdvance());
        assert(!game.fieldItems().find("irai_item") && !game.progress.originalPouch->keyItemCount(95));
        for(int n=0;n<5000 && game.fieldActorTalk.message.active();++n)game.tick(confirm);
        assert(!game.fieldActorTalk.message.active() && game.fieldAcquisition().active() && game.scene==Scene::Dialogue);
        game.tick({});assert(game.progress.originalPouch->keyItemCount(95)==1 && game.fieldAcquisition().get().substate()==2);
        assert(game.fieldAcquisition().stopped() && game.itemSystemMask==3 && game.itemCamera8Flags&0x200 && game.fieldMarioBattleState().keyDepth==2);
        assert(game.fieldMarioMotion()==15 && game.fieldMarioMotionPending() && std::strcmp(game.marioPresentation.selected,"M_I_U"));
        assert(game.progress.originalNpcTalkState.flag5405==false);
        Game obtaining=game;assert(!obtaining.fieldAcquisition().canAdvance());
        for(int n=0;n<5;++n)obtaining.tick(confirm);
        assert(game.fieldItems().find("irai_item")->flags&1);
        assert(game.progress.originalNpcTalkState.flag5405==false);
        game.tick(confirm);assert(game.fieldAcquisition().active() && game.progress.originalNpcTalkState.flag5405==false);
        assert(game.fieldMarioBattleState().keyDepth==1 && game.fieldKeysOff);
        assert(!std::strcmp(game.marioPresentation.selected,"M_I_U") && game.fieldMarioMotion()==0 && game.fieldMarioMotionPending());
        game.tick({});assert(game.fieldAcquisition().complete() && game.progress.originalNpcTalkState.flag5405==true);
        assert(!game.fieldAcquisition().stopped() && !game.itemSystemMask && !(game.itemCamera8Flags&0x200) && !game.fieldMarioBattleState().keyDepth);
        assert(game.scene==Scene::Explore && !game.fieldNpcTalkActive() && !game.fieldKeysOff);
        assert(game.fieldControlEnabled() && !(game.fieldMarioBattleState().flags&2) && std::strcmp(game.marioPresentation.selected,"M_I_U"));
        game.tick({});assert(game.progress.originalPouch->keyItemCount(95)==1);
    }

}

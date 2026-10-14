#include "game.hpp"
#include "camera_road_assets.hpp"
#include "field_camera_events.hpp"
#include "save.hpp"
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

    // Real Game admission drives reward EVT, coin service and independent
    // closing-window owner, then persists the source completion flag.
    {
        Progress checkpoint;checkpoint.room=0;checkpoint.originalStory=8;checkpoint.position={-4,-3.55f};
        checkpoint.originalNpcTalkState.quest1420=2;checkpoint.originalNpcTalkState.flag5406=true;
        checkpoint.originalNpcTalkState.flag5376=false;
        checkpoint.originalPouch=std::make_shared<ProloguePouch>(initializeProloguePouch("Yoshi"));
        Game game;game.restore(checkpoint);game.marioPresentation.display=game.marioPresentation.target=180;
        for(int n=0;n<360;++n)game.tick({});
        assert(!game.nearbyFieldActor());
        struct Services {Game* game;uint32_t time=0;unsigned grants=0;std::vector<FieldCoinRequest> requests;} h{&game};
        game.coinWindows=std::make_shared<CoinGetWindow>();assert(loadItemGetWindowAssets("romfs") && game.coinWindows->prepare(harborMessageFont()));
        FieldCoinGetServices coin;coin.user=&h;
        coin.ready=[](void*){return true;};
        game.bindFieldCoinGetPresentation(coin);assert(!game.nearbyFieldActor()); // Complete service shape is required.
        coin.mario=[](void* p){return static_cast<Services*>(p)->game->fieldItemMarioPosition();};
        coin.heading=[](void*){return 0.f;};coin.clock=[](void* p){return FieldCoinClock{0,static_cast<Services*>(p)->time,4000};};
        coin.effect=[](void* p,int32_t kind,ItemGetPoint xyz){assert(kind==7);static_cast<Services*>(p)->game->acquisitionEffect.beginCoin({xyz.x,xyz.y,xyz.z});return uintptr_t(1);};
        coin.effectPayload=[](void* p,uintptr_t handle,int32_t camera){assert(handle==1 && camera==5);static_cast<Services*>(p)->game->acquisitionEffect.camera(camera);};
        coin.softDelete=[](void* p,uintptr_t){static_cast<Services*>(p)->game->acquisitionEffect.softDelete();};
        coin.iconEnter=[](void* p,const char* name,uint32_t descriptor,ItemGetPoint xyz){assert(descriptor==403);assert(static_cast<Services*>(p)->game->fieldItems().enterCoin403(name,xyz));};
        coin.iconFlags=[](void* p,const char* name,uint16_t on,uint16_t off){static_cast<Services*>(p)->game->fieldItems().iconFlags(name,on,off);};
        coin.iconDelete=[](void* p,const char* name){static_cast<Services*>(p)->game->fieldItems().deleteIcon(name);};
        coin.request=[](void* p,FieldCoinRequest r){auto& h=*static_cast<Services*>(p);h.requests.push_back(r);
            if(r==FieldCoinRequest::GetMotion || r==FieldCoinRequest::StayMotion)
                assert(h.game->applyFieldItemMarioRequest(r==FieldCoinRequest::GetMotion?ItemGetRequest::GetItemMotion:ItemGetRequest::StayMotion));};
        coin.window=[](void* p,uint32_t definition,const int32_t* parameter){return static_cast<Services*>(p)->game->coinWindows->enter(definition,parameter);};
        coin.windowClose=[](void* p,int32_t slot){static_cast<Services*>(p)->game->coinWindows->closeAutoDelete(slot);};
        coin.windowAction=[](void* p,int32_t slot){return static_cast<Services*>(p)->game->coinWindows->action(slot);};
        coin.windowRetained=[](void* p,int32_t){return static_cast<Services*>(p)->game->coinWindows->retained();};
        coin.addCoins=[](void* p,int32_t amount){auto& h=*static_cast<Services*>(p);++h.grants;return h.game->progress.originalPouch->addCoins(amount);};
        game.bindFieldCoinGetPresentation(coin);assert(!game.nearbyFieldActor());
        game.bindFieldItemMarioPost({nullptr,[](void*,unsigned){}});
        assert(game.nearbyFieldActor() && game.nearbyFieldActor()->key==source->key);
        Input confirm;confirm.confirm=true;game.tick(confirm);game.tick({});
        assert(game.fieldReward().active() && game.fieldActorTalk.message.id()=="irai_02_02");
        assert(game.fieldActorTalk.actor.key==source->key && game.fieldMarioBattleState().keyDepth==1 && game.fieldKeysOff);
        Game snapshot=game;assert(!snapshot.fieldReward().canAdvance());
        for(int n=0;n<10;++n)snapshot.tick(confirm);
        assert(!h.grants && game.progress.originalNpcTalkState.flag5376==false);
        int32_t rate=60;ItemSelectionSequenceServices seq{&rate,[](void* p){return *static_cast<int32_t*>(p);},
            [](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return cameraInterpolation(mode,step,count,a,b).value_or(a);}};
        bool coinSeen=false,finishSeen=false;
        for(int n=0;n<6000 && game.fieldReward().active();++n) {
            h.time+=17;game.tick(confirm);
            if(game.fieldCoinGet().state()==1)coinSeen=true;
            if(game.fieldActorTalk.message.id()=="sys_request_finish") {
                finishSeen=true;assert(game.fieldActorTalk.actor.key==0 && h.grants==1 && game.progress.originalNpcTalkState.quest1420==0 && game.progress.originalNpcTalkState.flag5376==true);
            }
            game.coinWindows->managerTick(seq);game.retireFieldCoinWindow();game.acquisitionEffect.tick();
        }
        assert(coinSeen && finishSeen && game.fieldReward().complete() && h.grants==1);
        assert(game.scene==Scene::Explore && !game.fieldKeysOff && !game.fieldMarioBattleState().keyDepth && !game.fieldNpcTalkActive());
        assert(game.progress.originalPouch->half(0x78)==120 && game.progress.originalNpcTalkState.flag5406==true);
        assert(!game.fieldCoinGet().retained() && !game.coinWindows->retained() && game.coinWindows->action(0)==1);
        Progress saved;assert(decodeSave(encodeSave(game.progress),saved) && saved.originalNpcTalkState.flag5376==true && saved.originalNpcTalkState.quest1420==0 && saved.originalPouch->half(0x78)==120);
        assert((h.requests==std::vector<FieldCoinRequest>{FieldCoinRequest::GetMotion,FieldCoinRequest::BgmOff3800,FieldCoinRequest::FanfareOn201,FieldCoinRequest::StayMotion,FieldCoinRequest::BgmResumeA0,FieldCoinRequest::BgmOff201}));
    }

}

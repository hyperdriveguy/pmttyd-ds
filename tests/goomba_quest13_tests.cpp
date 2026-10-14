#include "goomba_quest13.hpp"
#include "actor_animation.hpp"
#include "item_presentation.hpp"
#include "message_icons.hpp"
#include "save.hpp"
#include "camera_road_assets.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
using namespace ttyd;
int main() {
    MessageFont font;assert(font.load("romfs/font.pmfont"));
    assert(loadFieldActorCatalog("romfs") && loadPouchRevisionAssets("romfs"));
    assert(loadItemPresentations("romfs/item-presentations.bin") && loadMessageIcons("romfs/message-icons.bin") && loadItemSelectorAssets("romfs"));
    const FieldActorRecord* actor=nullptr;for(const auto& a:fieldActorCatalog().actors())if(a.room==2 && a.key==0x2b0a9f83)actor=&a;assert(actor);
    ActorAnimationPack empty;assert(!bindGoombaQuest13Poses(empty));
    FieldActorTalkState state{uint8_t(13),false,true,false};assert(!goombaQuest13Admitted(*actor,state,{{},3}));
    ActorAnimationPack poses;assert(poses.load("romfs/goomba-quest-mario.pmanim") && bindGoombaQuest13Poses(poses));
    state.flag5407.reset();assert(!goombaQuest13Admitted(*actor,state,{{},3}));
    state.flag5407=false;assert(goombaQuest13Admitted(*actor,state,{{},3}));
    assert(!goombaQuest13Admitted(*actor,state,{{},0}));
    for(bool continuing:{false,true})for(bool cancel:{false,true}) {
        auto p=initializeProloguePouch("Yoshi");for(unsigned i=0;i<121;++i)p.half(0xa0+2*i,0);
        for(unsigned i:{0u,60u,120u})p.half(0xa0+2*i,107);
        p.half(0x70,3);p.half(0x74,1);
        state.flag5407=continuing;FieldActorTalk talk;ItemSelectionPresentation selector;MarioPresentation mario;GoombaQuest13 event;
        assert(event.begin(*actor,p,state,talk,selector,font,mario,0));assert(event.keyDepth()==1 && event.table().size()==4);
        const auto* allocation=event.table().data();uint64_t ticks=0,lastSerial=0;std::vector<std::string> messages;
        auto observe=[&](){if(talk.message.active() && talk.message.serial!=lastSerial){lastSerial=talk.message.serial;messages.push_back(talk.message.id());assert(talk.actor.key==actor->key);}};
        observe();
        auto step=[&](uint32_t buttons=0x100u) {
            ++ticks;
            if(selector.owner.retained())selector.tick({0,0,buttons},60,uint32_t(ticks));
            else talk.message.tick(ticks*1000/60,buttons);
            event.tick(ticks*1000/60);observe();
            if(event.active()){assert(event.table().data()==allocation && event.table().size()==4);assert(event.keyDepth()==(selector.owner.retained()?2:1));}
        };
        if(!continuing) {
            while(std::strcmp(mario.front,"M_N_5B") && ticks<2000)step();assert(!std::strcmp(mario.front,"M_N_5B"));
            for(unsigned i=0;i<29;++i){step();assert(!std::strcmp(mario.front,"M_N_5B"));}
            step();assert(!std::strcmp(mario.front,"M_N_C"));
            for(unsigned i=0;i<29;++i){step();assert(!std::strcmp(mario.front,"M_N_C"));}
            step();assert(!std::strcmp(mario.front,"M_S_1") && talk.message.id()=="irai_13_002");
        } else assert(talk.message.id()=="irai_13_005" && !std::strcmp(mario.front,"M_S_1"));
        unsigned selections=0;
        while(event.active() && ticks<10000) {
            while(event.active() && !selector.owner.retained() && ticks<10000)step();
            if(!event.active())break;
            assert(selector.owner.main().rows.size()==unsigned(p.keyItemCount(107)));
            for(const auto& row:selector.owner.main().rows)assert(row.item==107);
            for(unsigned i=0;i<11;++i)step(0);
            step(cancel?0x200u:0x100u);++selections;
            while(selector.owner.retained() && ticks<10000)step(0);
            if(!cancel && p.keyItemCount(107)==0 && event.active()) {
                assert(talk.message.id()=="irai_13_004" && event.table()[0]==107 && event.table()[1]==-1);
                assert(state.flag5407==true); // First flag precedes final completion print.
            }
        }
        assert(!event.active() && event.table().empty() && event.keyDepth()==0 && state.flag5407==true);
        assert(state.quest1420==13 && state.flag5406==true && state.flag5405==false && p.half(0x70)==3 && p.half(0x74)==1);
        assert(p.keyItemCount(107)==(cancel?3:0) && selections==(cancel?1u:3u));
        const std::vector<std::string> expected=continuing?
            (cancel?std::vector<std::string>{"irai_13_005"}:std::vector<std::string>{"irai_13_005","irai_13_005","irai_13_005","irai_13_004"}):
            (cancel?std::vector<std::string>{"irai_13_001","irai_13_002"}:std::vector<std::string>{"irai_13_001","irai_13_002","irai_13_003","irai_13_003","irai_13_004"});
        assert(messages==expected);
        Progress saved;saved.originalStory=8;saved.originalNpcTalkState=state;saved.originalPouch=std::make_shared<ProloguePouch>(p);Progress loaded;
        assert(decodeSave(encodeSave(saved),loaded) && loaded.originalNpcTalkState.flag5407==true && loaded.originalPouch->bytes==p.bytes);
    }
    assert(loadHarborStoryAssets("romfs") && loadCameraRoadAssets("romfs"));
    assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
    for(bool continuing:{false,true}) {
        Progress saved;saved.room=2;saved.originalStory=8;saved.position={1.2f,-4};
        saved.originalNpcTalkState={uint8_t(13),false,true,continuing};
        saved.originalPouch=std::make_shared<ProloguePouch>(initializeProloguePouch("Yoshi"));saved.originalPouch->half(0xa0,107);
        Progress checkpoint;assert(decodeSave(encodeSave(saved),checkpoint));Game game;game.restore(checkpoint);
        for(unsigned i=0;i<360;++i)game.tick({});
        const auto* live=game.fieldNpcState(actor->key);assert(live);const auto root=live->body.root;
        bool found=false;
        for(float dx:{-.30f,.30f}) {
            for(float facing:{0.f,180.f}) {
                game.progress.position={root.x*.01f+dx,root.z*.01f};game.marioPresentation.display=game.marioPresentation.target=facing;game.tick({});
                const auto* candidate=game.nearbyFieldActor();if(candidate && candidate->key==actor->key){found=true;break;}
            }
            if(found)break;
        }
        assert(found);Input press;press.confirm=true;game.tick(press);game.tick({});
        assert(game.fieldActorTalk.message.id()==(continuing?"irai_13_005":"irai_13_001") && game.fieldKeysOff && game.scene==Scene::Dialogue);
        bool firstPose=false,secondPose=false;
        for(unsigned i=0;i<3000 && (!game.itemSelection || !game.itemSelection->owner.retained());++i) {
            game.tick(press);firstPose|=!std::strcmp(game.marioPresentation.selected,"M_N_5B");secondPose|=!std::strcmp(game.marioPresentation.selected,"M_N_C");
            assert(game.fieldKeysOff && game.fieldNpcTalkActive() && game.scene==Scene::Dialogue && !game.saveRequested);
        }
        assert(game.itemSelection && game.itemSelection->owner.retained() && firstPose==!continuing && secondPose==!continuing);
        for(unsigned i=0;i<12;++i)game.tick({});Input cancel;cancel.cancel=true;game.tick(cancel);
        for(unsigned i=0;i<100 && game.itemSelection->owner.retained();++i)game.tick({});
        assert(!game.fieldKeysOff && game.scene==Scene::Explore && game.progress.originalNpcTalkState.flag5407==true && game.progress.originalPouch->keyItemCount(107)==1 && game.progress.originalStory==8);
        Progress loaded;assert(decodeSave(encodeSave(game.progress),loaded) && loaded.originalNpcTalkState.flag5407==true && loaded.originalPouch->keyItemCount(107)==1);
    }
    std::cout<<"Original quest13 loops,500ms poses, retained table, cancel flags, final print and native Game/checkpoint ownership passed\n";
}

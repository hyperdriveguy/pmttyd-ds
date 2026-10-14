#include "goomba_hand_in.hpp"
#include "item_presentation.hpp"
#include "message_icons.hpp"
#include "game.hpp"
#include "camera_road_assets.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    MessageFont font;assert(font.load("romfs/font.pmfont"));
    assert(loadFieldActorCatalog("romfs") && loadPouchRevisionAssets("romfs"));
    assert(loadItemPresentations("romfs/item-presentations.bin") && loadMessageIcons("romfs/message-icons.bin") && loadItemSelectorAssets("romfs"));
    const FieldActorRecord* actor=nullptr;
    for(const auto& a:fieldActorCatalog().actors())if(a.room==2 && a.key==0x2b0a9f83)actor=&a;
    assert(actor);
    FieldActorTalkState state;FieldActorTalkInventory items{1,{}};
    assert(!goombaQuest2Admitted(*actor,state,items));
    state.quest1420=2;assert(!goombaQuest2Admitted(*actor,state,items));
    state.flag5405=true;assert(goombaQuest2Admitted(*actor,state,items));
    items.item95=0;assert(!goombaQuest2Admitted(*actor,state,items));
    for(int scenario=0;scenario<3;++scenario) {
        auto p=initializeProloguePouch("Yoshi");p.half(0xa0,95);p.half(0x70,3);p.half(0x74,1);
        state.flag5406=false;FieldActorTalk talk;ItemSelectionPresentation selector;GoombaHandIn event;
        assert(event.begin(*actor,p,state,talk,selector,font,0));
        assert(event.keyDepth()==1 && talk.message.id()=="irai_02_03" && talk.actor.key==actor->key);
        assert(!event.begin(*actor,p,state,talk,selector,font,0));
        uint64_t tick=0;
        while(!selector.owner.retained() && tick<2000){talk.message.tick(++tick*1000/60,0x100);event.tick(tick*1000/60);}
        assert(selector.owner.retained() && event.keyDepth()==2 && p.keyItemCount(95)==1 && state.flag5406==false);
        for(unsigned i=0;i<11;++i)selector.tick({},60,uint32_t(++tick));
        selector.tick({0,0,scenario==0?0x200u:0x100u},60,uint32_t(++tick));
        while(selector.owner.retained() && tick<3000){selector.tick({},60,uint32_t(++tick));event.tick(tick*1000/60);}
        assert(!selector.owner.retained());
        if(scenario==0){assert(!event.active() && event.keyDepth()==0 && p.keyItemCount(95)==1 && state.flag5406==false);continue;}
        assert(event.active() && event.keyDepth()==1 && talk.message.id()=="irai_02_04" && state.flag5406==false);
        assert(talk.message.rawText().find("<wait 500>")!=std::string::npos);
        if(scenario==2)assert(p.removeKeyItem(95)==1); // Original event ignores removal failure.
        while(event.active() && tick<4000){talk.message.tick(++tick*1000/60,0x100);event.tick(tick*1000/60);}
        assert(!event.active() && event.keyDepth()==0 && state.flag5406==true && p.keyItemCount(95)==0);
        assert(p.half(0x70)==3 && p.half(0x74)==1 && state.quest1420==2 && state.flag5405==true);
    }
    assert(loadHarborStoryAssets("romfs") && loadCameraRoadAssets("romfs"));
    assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
    Game game;Progress checkpoint;checkpoint.room=2;checkpoint.originalStory=8;
    checkpoint.position={1.2f,-4};checkpoint.originalNpcTalkState.quest1420=2;checkpoint.originalNpcTalkState.flag5405=true;
    checkpoint.originalNpcTalkState.flag5406=false;
    checkpoint.originalPouch=std::make_shared<ProloguePouch>(initializeProloguePouch("Yoshi"));checkpoint.originalPouch->half(0xa0,95);
    game.restore(checkpoint);for(unsigned i=0;i<360;++i)game.tick({});
    const auto* live=game.fieldNpcState(actor->key);assert(live);
    const auto root=live->body.root;
    // Native checkpoint placement is a harness input, not a claimed original
    // approach/timing observation. Real scan/proximity/event entry remain live.
    bool found=false;
    for(float dx:{-.30f,.30f}) {
      for(float facing:{0.f,180.f}) {
        game.progress.position={root.x*.01f+dx,root.z*.01f};
        game.marioPresentation.display=game.marioPresentation.target=facing;game.tick({});
        const auto* nearby=game.nearbyFieldActor();if(nearby && nearby->key==actor->key){found=true;break;}
      }
      if(found)break;
    }
    assert(found);Input press;press.confirm=true;game.tick(press);game.tick({});
    assert(game.fieldKeysOff && game.scene==Scene::Dialogue && game.fieldActorTalk.message.id()=="irai_02_03");
    for(unsigned i=0;i<2000 && (!game.itemSelection || !game.itemSelection->owner.retained());++i)game.tick(press);
    assert(game.itemSelection && game.itemSelection->owner.retained() && game.fieldKeysOff);
    for(unsigned i=0;i<12;++i)game.tick({});
    Input cancel;cancel.cancel=true;game.tick(cancel);
    for(unsigned i=0;i<100 && game.itemSelection->owner.retained();++i)game.tick({});
    assert(!game.fieldKeysOff && game.scene==Scene::Explore && game.progress.originalPouch->keyItemCount(95)==1 &&
           game.progress.originalNpcTalkState.flag5406==false && game.progress.originalStory==8);
    std::cout<<"Original quest2 guard, message order/tokens, nested keys, cancel, hand-in and ignored remove failure passed\n";
}

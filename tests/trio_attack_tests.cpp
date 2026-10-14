#include "trio_attack.hpp"
#include "battle_pose_inner.hpp"
#include "native_shadow_program.hpp"
#include "actor_animation.hpp"
#include "battle_actor_rotation.hpp"
#include "game.hpp"
#include "native_camera_draw.hpp"
#include "native_display_state.hpp"
#include "battle_dive.hpp"
#include "battle_ending_fade.hpp"
#include "battle_pose_pool.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace ttyd;
int main(){
    BattleEndingFade sourceFade;sourceFade.enter(200);for(unsigned n=0;n<10;++n)sourceFade.tick(50);
    assert(sourceFade.ratio==1 && !sourceFade.finished && sourceFade.textureOffset==-100);
    sourceFade.tick(50);assert(sourceFade.finished && sourceFade.ratio==1 && sourceFade.alpha()==255);
    sourceFade.enter(0);sourceFade.tick(60);assert(sourceFade.finished && sourceFade.alpha()==255);

    PointGetEffect effect;effect.enter(10,0,90,0,0);assert(effect.active && effect.x==-2000 && effect.y==60);
    effect.tick();assert(effect.depthPosition[0]==-2000 && effect.timer==1 && effect.phase==0 && effect.x>-1000 && effect.x<0);
    for(unsigned n=1;n<45;++n)effect.tick();assert(effect.phase==1 && effect.timer==0 && std::fabs(effect.x)<.001f);
    const auto quads=pointGetQuads(effect);assert(quads.size()==8);
    const float layoutScale=200.f/322.f;
    assert(quads[0].shadow && quads[0].texture==125 && std::fabs(quads[0].width-200*layoutScale)<.001f);
    assert(!quads[4].shadow && quads[4].texture==124 && std::fabs(quads[4].centerX+222*layoutScale)<.001f);
    assert(quads[5].texture==126 && std::fabs(quads[5].u0-.1f)<.00001f && std::fabs(quads[5].u1-.2f)<.00001f);
    assert(std::fabs(quads[5].width-48*layoutScale*1.4285714626312256f)<.001f);
    assert(quads[6].u0==0 && quads[7].texture==130 && std::fabs(quads[7].centerX-158*layoutScale)<.001f);
    assert(std::fabs(quads[0].bottomY-(60-4*layoutScale))<.001f && quads[4].bottomY==60);
    effect.tick();assert(effect.phase==1 && effect.timer==1);effect.softDelete=true;effect.tick();assert(effect.phase==2 && effect.timer==0 && !effect.softDelete);
    for(unsigned n=0;n<59;++n)effect.tick();assert(effect.active && effect.timer==59);effect.tick();assert(!effect.active && pointGetQuads(effect).empty());
    effect.enter(1,0,90,0,0);effect.softDelete=true;for(unsigned n=0;n<45;++n)effect.tick();assert(effect.phase==1 && effect.softDelete);effect.tick();assert(effect.phase==2);
    assert(pointGetQuads(effect).back().texture==128);

    assert(battleDiveInterpolate(0,2,8,0,64)==16 && battleDiveInterpolate(1,2,8,0,64)==4 && battleDiveInterpolate(4,2,8,0,64)==28);
    assert(battleMirrorZ(0,90,0,0) && battleMirrorZ(0,270,0,0));
    assert(!battleMirrorZ(0,89,0,0) && !battleMirrorZ(0,271,0,0));
    assert(battleMirrorZ(180,0,0,0x08000000) && !battleMirrorZ(180,0,0,0x01000000));
    assert(battleMirrorZ(0,45,45,0) && battleMirrorZ(180,180,180,0));
    const auto ordered=BattleActorRotation({90,90,90},{0,0,0}).point({1,2,3});
    assert(std::fabs(ordered[0]+2)<.00001f && std::fabs(ordered[1]-1)<.00001f && std::fabs(ordered[2]-3)<.00001f);
    const auto pivoted=BattleActorRotation({0,90,0},{100,0,0}).point({2,0,0});
    assert(std::fabs(pivoted[0]-1)<.00001f && std::fabs(pivoted[2]+1)<.00001f);
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs") && loadTrioAttackAssets("romfs"));
    for(const auto& resource:std::array<std::pair<const char*,const char*>,2>{{
        {"romfs/trio-attack-mario.pmanim","M_Z_1"},
        {"romfs/trio-attack-goombella.pmanim","PKR_Z_1"}}}){
        ActorAnimationPack bank;ActorAnimationSample sample;
        assert(bank.load(resource.first));
        const auto* fallback=bank.pose(resource.second);
        assert(fallback && fallback->evaluate(0,sample) && !sample.vertices.empty());
    }
    {
        auto pouch=createProloguePouch();assert(pouch);pouch->half(0x70,7);pouch->half(0x74,2);pouch->half(20,6);
        ProfessorStory entryScene;assert(entryScene.beginTrioBattleIntro(0));TrioBattleCommands entryMenu;assert(entryMenu.begin(entryScene));entryMenu.input(0x100,0);entryMenu.input(0x100,0);
        TrioAttack supplied;assert(supplied.begin(entryMenu,entryScene,10,pouch));uint32_t resource=0;
        assert(supplied.readWord(0x8410110c,resource) && resource==0x00070005);
        assert(supplied.readWord(0x84101110,resource) && resource==0x00050002);
        assert(supplied.readWord(0x8410210c,resource) && resource==0x00060005);
        assert(supplied.readWord(0x84102110,resource) && resource==0x00050005);
        assert(supplied.readWord(0x84102114,resource) && resource==0x3f800000);
        assert(supplied.readWord(0x85000070,resource) && resource==0x0007000a);
        assert(pouch->half(0x70)==7 && pouch->half(0x74)==2 && pouch->half(20)==6);
        Progress entry;entry.originalPouch=pouch;entry.hp=7;Game retained;retained.restore(entry);assert(retained.progress.originalPouch==pouch);
        const auto saved=retained.progress;retained.restore(saved);assert(retained.progress.originalPouch==pouch && pouch->half(0x74)==2);
    }
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));
    TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.chooseTarget(2);menu.input(0x100,0);
    TrioAttack jump;assert(jump.begin(menu,scene,10));
    assert(!jump.statusWindowVisible());
    uint32_t statusOwner=0;assert(jump.readWord(0x84002744,statusOwner) && statusOwner==0);
    assert(jump.readWord(0x84002748,statusOwner) && statusOwner==1);
    uint32_t selectedRecord=0;assert(jump.readWord(0x84016f38,selectedRecord) && (selectedRecord>>24)==1);
    assert(jump.readWord(0x841012a0,selectedRecord) && selectedRecord==0x80355cf0); // Selection precedes source event execution.
    jump.tick(17,scene);
    uint32_t value=0;assert(jump.readWord(0x8410116c,value) && value==20);
    // Read the original three Mario definitions through their live links.
    // Extra Mario parts must never overwrite Goombella's primary record.
    const uint32_t marioParts[]{0x84201000,0x84200000,0x84200500};
    assert(jump.readWord(0x84101014,value) && value==marioParts[0]);
    for(unsigned i=0;i<3;++i){
        const auto part=marioParts[i];
        assert(jump.readWord(part,value) && value==(i<2?marioParts[i+1]:0));
        assert(jump.readWord(part+4,value) && value==0x80372a18+0x4c*i);
        assert(jump.readWord(part+0x4ec,value) && value==0x84101000);
        assert(jump.readWord(part+0x208,value) && value==0x3f800000);
        assert(jump.readWord(part+0x4f0,value) && value==0xffffffff);
    }
    assert(jump.readWord(0x84102014,value) && value==0x84202000);
    assert(jump.readWord(0x84202000,value) && value==0);
    assert(jump.readWord(0x84202004,value) && value==0x80393ba0);
    assert(jump.readWord(0x842024ec,value) && value==0x84102000);
    assert(jump.battlePosePool() && jump.battlePosePool()->models()[0].references==3 && jump.battlePosePool()->models()[1].references==1);
    for(unsigned i=0;i<3;++i){
        const auto part=marioParts[i];assert(jump.readWord(part+0x1c0,value) && value==i);
        const auto* pose=jump.battlePose(part);assert(pose && pose->word(0xc)==1 && pose->word(0x14)==0 && pose->resource->modelName=="b_mario" && !pose->shape.vertices.empty());
    }
    assert(jump.readWord(0x84202000+0x1c0,value) && value==3 && jump.battlePose(0x84202000));
    assert(jump.readWord(0x84200000+0x1c4,value) && value==0x4b524e5f); // Authored KRN_H_1 is copied, not selected.
    assert(!jump.battlePose(0x84203000)); // No enemy raw handle aliases Mario's slot0.
    assert(jump.readWord(0x84016ee0,value) && value==1); // Original active ambient record.
    assert(jump.readWord(0x84016f00,value) && value==1); // Separate active background record.
    assert(jump.readWord(0x84016f04,value) && value==0x8c8c0000);
    assert(jump.readWord(0x84016f20,value) && value==0x43910000); // 290 source float.
    assert(jump.readWord(0x84016f24,value) && value==0x43480000); // 200, escape-wall provider.
    assert(jump.readWord(0x8041ebdc,value) && value==5);
    assert(jump.readWord(0x8041ebe0,value) && value==0x85600040);
    assert(jump.readWord(0x8041ebe4,value) && value==0);
    assert(jump.readWord(0x804295d4,value) && value==0);
    for(unsigned i=0;i<5;++i) {
        const auto r=0x85600040+0x2c*i;
        assert(jump.readWord(r,value) && value==0x00010000);
        assert(jump.readWord(r+0x1c,value) && value==0x41a00000);
        assert(jump.readWord(r+0x24,value) && value==0x43160000);
        assert(jump.readWord(r+0x28,value) && value==0xffffffff);
    }
    assert(jump.readWord(0x84101170,value) && value==0);
    assert(jump.readWord(0x84101174,value) && value==0x3f330000);
    bool approached=false,airborne=false;
    for(unsigned t=34;t<6000 && !jump.pendingFunction();t+=17) {
        jump.tick(t,scene);approached|=scene.actors[0].position[0]>-100;airborne|=scene.actors[0].position[1]>0;
    }
    assert(jump.readWord(0x841012a8,value) && value!=0);
    assert(jump.readWord(0x84201204,value) && (value&0xb)==3); // Source pose request force0.
    assert(jump.active() && jump.pendingFunction()==0x8010a78c && approached && airborne && scene.actors[0].pose=="M_J_1D");
    assert(jump.readWord(0x84001cb8,value) && value==0); // No input: failed timing, source per-hit flags100.
    const auto& requests=jump.requests();
    for(unsigned i=0;i<16;++i)assert(requests[i].function==0x80005da0 && requests[i].rawOperands==std::vector<int32_t>{1});
    assert(requests[16].function==0x800f8934 && requests[17].function==0x801a83f8 && requests.back().function==0x8010a78c);
    assert(jump.readWord(0x84015fcc,value) && value==0); // Window actual count.
    assert(jump.readWord(0x84002864,value) && value==1); // Owned normal TPL handle.
    assert(jump.readWord(0x84002868,value) && value==2); // Owned Luigi TPL handle.
    assert(requests.back().rawOperands[4]==0x100);
    auto count=requests.size();for(unsigned t=6017;t<7000;t+=17)jump.tick(t,scene);
    assert(jump.active() && !jump.finished() && jump.requests().size()==count);
    for(unsigned index:{0u,1u,2u}) {
        ProfessorStory targetScene;assert(targetScene.beginTrioBattleIntro(0));TrioBattleCommands chosen;assert(chosen.begin(targetScene));chosen.input(0x100,0);chosen.chooseTarget(index);chosen.input(0x100,0);
        TrioAttack success;assert(success.begin(chosen,targetScene,10));bool pressed=false,lethalBeforeStatus=false,retiredNextCall=false;unsigned markedAt=0;
        for(unsigned t=17;t<6000 && !success.pendingFunction();t+=17) {
            const bool timingPress=!pressed && targetScene.actors[0].pose=="M_J_1D";
            success.tick(t,targetScene,timingPress?0x100:0);pressed|=timingPress;
            if(index==0) {
                uint32_t hp=0,status=0,flags=0,registered=0;
                assert(success.readWord(0x8410310c,hp) && success.readWord(0x84103134,status));
                lethalBeforeStatus|=(hp>>16)==0 && !(status&0x00ff0000);
                assert(success.readWord(0x8410301c,flags) && success.readWord(0x8400002c,registered));
                if((flags&0x80000000) && registered){markedAt=t;assert(!targetScene.actors[3].hidden);}
                if(!registered && !retiredNextCall){assert(markedAt && t==markedAt+17);retiredNextCall=true;}
            }
        }
        assert(pressed && (index==0?(success.finished() && !success.pendingFunction()):success.pendingFunction()==0x8010a78c));
        assert(success.readWord(0x84001cb8,value) && value==(index==0?0u:2u));
        assert(success.readWord(0x84001cb4,value) && value==1);
        const auto damage=std::find_if(success.requests().rbegin(),success.requests().rend(),[](const ProfessorServiceRequest& r){return r.function==0x8010a78c;});
        assert(damage!=success.requests().rend() && damage->rawOperands.size()==6 && damage->rawOperands[4]==(index==0?0x100:0x20000)); // Source first-success hit flags.
        assert(success.readWord(0x84101000+0x27c,value) && ((value&8)!=0)==(index!=0)); // Precheck charge token occurs before impact.
        assert(success.readWord(0x84103000+index*0x1000+0x10c,value) && value>>16==(index==0?0u:2u)); // Ground hits apply damage; other kind callbacks stay pending.
        if(index==0) {
            assert(lethalBeforeStatus && retiredNextCall);
            assert(success.readWord(0x84103278,value) && value==0x10717); // Second missed hit is lethal.
            assert(success.readWord(0x84103134,value) && (value&0x00ff0000)==0x00010000); // Original death child sets status27.
            assert(success.readWord(0x84103260,value) && value>>16==2); // Both source damage calls ran.
            assert(success.readWord(0x8400002c,value) && value==0 && targetScene.actors[3].hidden); // Deferred registry removal.
            assert(success.readWord(0x84000f00,value) && value==3); // Original group-count factor/pouch1.
            assert(success.readWord(0x84400014,value) && value<=1); // Source one-draw coin reward.
            const auto spins=std::count_if(success.requests().begin(),success.requests().end(),[](const ProfessorServiceRequest& r){return r.function==0x801115f8 && r.rawOperands[2]==-15;});
            const auto tips=std::count_if(success.requests().begin(),success.requests().end(),[](const ProfessorServiceRequest& r){return r.function==0x801115f8 && r.rawOperands[1]==3;});
            assert(spins==48 && tips==30 && targetScene.actors[3].battleRotation[0]==90 && targetScene.actors[3].battleRotation[1]==0);
            assert(success.readWord(0x84103ae8,value) && value==0xff000000); // Source status-info record reset.
        }
        assert(success.finished()==(index==0));
    }
    ProfessorStory twiceScene;assert(twiceScene.beginTrioBattleIntro(0));
    TrioBattleCommands twiceMenu;assert(twiceMenu.begin(twiceScene));twiceMenu.input(0x100,0);twiceMenu.input(0x100,0);
    TrioAttack twice;assert(twice.begin(twiceMenu,twiceScene,10));std::string previousPose;unsigned timingPresses=0;
    for(unsigned t=17;t<6000 && !twice.pendingFunction();t+=17) {
        const auto pose=twiceScene.actors[0].pose;const bool press=pose=="M_J_1D" && previousPose!=pose;
        twice.tick(t,twiceScene,press?0x100:0);timingPresses+=press;previousPose=pose;
    }
    assert(timingPresses==2 && twice.finished() && !twice.pendingFunction());
    assert(twice.statusWindowVisible());
    assert(twice.readWord(0x84001cb8,value) && value==2 && twice.readWord(0x84001cb4,value) && value==2);
    const auto secondHit=std::find_if(twice.requests().rbegin(),twice.requests().rend(),[](const ProfessorServiceRequest& r){return r.function==0x8010a78c;});
    assert(secondHit!=twice.requests().rend() && secondHit->rawOperands[4]==0x20100);
    assert(twice.readWord(0x8410310c,value) && value>>16==0);
    ActorAnimationPack animation;assert(animation.load("romfs/trio-attack-mario.pmanim"));
    const auto* stylish=animation.pose("M_A_5");assert(stylish && stylish->keys.size()==1 && stylish->start==0 && stylish->end==2);
    ActorAnimationSample at0,at2,after;assert(stylish->evaluate(0,at0) && stylish->evaluate(2,at2) && stylish->evaluate(60,after));
    assert(at0.nodes==at2.nodes && at0.nodes==after.nodes && !at0.vertices.empty());
    CollisionTransition collision;assert(collision.load("romfs/tik_01"));collision.commit();
    Progress checkpoint;checkpoint.room=5;checkpoint.originalStory=11;checkpoint.originalFlag1207=true;checkpoint.position={-1,.7f};Game game;game.restore(checkpoint);
    Input advance;advance.confirm=true;
    for(unsigned i=0;i<3600 && !game.trioBattleActive;++i)game.tick(advance);
    for(unsigned i=0;i<3600 && !game.trioBattleIntro.finished;++i)game.tick(advance);
    for(unsigned i=0;i<120 && game.trioCommands.state==TrioBattleCommands::State::Closed;++i)game.tick(Input{});
    assert(game.trioCommands.state==TrioBattleCommands::State::Command);
    game.tick(advance);assert(game.trioCommands.state==TrioBattleCommands::State::Target);game.tick(advance);
    const auto fieldPosition=game.progress.position;
    for(unsigned i=0;i<1000 && !game.trioAttack.pendingFunction();++i)game.tick(Input{});
    assert(game.trioAttack.finished() && !game.trioAttack.pendingFunction());
    assert(game.trioBattleIntro.actors[0].pose=="M_S_1" && game.trioBattleIntro.actors[3].pose=="KUR_S_1" && game.trioBattleIntro.actors[2].yaw==90);
    assert(game.trioAttack.readWord(0x8410310c,value) && value>>16==1); // A missed first timing produces one damage.
    assert(game.trioAttack.readWord(0x84103278,value) && value==0x10117);
    assert(game.trioAttack.readWord(0x84001f54,value) && value==0);
    for(unsigned o:{0x1ca0u,0x1ca4u,0x1ca8u,0x1cacu})assert(game.trioAttack.readWord(0x84000000+o,value) && value==0);
    const auto completedRequests=game.trioAttack.requests().size();game.tick(advance);
    assert(game.trioAttack.finished() && game.trioAttack.requests().size()==completedRequests);
    assert(game.trioCommands.actor==2 && game.trioCommands.state==TrioBattleCommands::State::Target);
    assert(game.trioAttack.readWord(0x84101020,value) && ((value>>24)&255)==10 && ((value>>8)&255)==0);
    assert(game.trioAttack.readWord(0x84102020,value) && ((value>>8)&255)==1);
    game.tick(advance);game.tick(Input{});assert(game.trioAttack.active());
    assert(game.trioAttack.readWord(0x84001c5c,value) && value==6);
    assert(game.trioAttack.readWord(0x84016f54,value) && ((value>>16)&255)==1);
    for(unsigned i=0;i<1200 && !game.trioAttack.pendingFunction() && !game.trioAttack.finished();++i)game.tick(Input{});
    assert(game.trioAttack.finished() && !game.trioAttack.pendingFunction());
    assert(game.trioBattleIntro.actors[3].hidden && game.trioBattleIntro.actors[1].pose=="PKR_S_1");
    assert(game.trioAttack.readWord(0x8410210c,value) && value>>16==10);
    assert(game.trioAttack.readWord(0x84000f00,value) && value==3);
    assert(game.trioAttack.readWord(0x84102020,value) && ((value>>24)&255)==10 && ((value>>8)&255)==0);
    const auto headbonk=std::find_if(game.trioAttack.requests().begin(),game.trioAttack.requests().end(),[](const ProfessorServiceRequest& r){return r.command==0x803941a8;});
    assert(headbonk!=game.trioAttack.requests().end());
    game.tick(Input{});assert(game.trioAttack.commandActor()==-1 && game.trioAttack.readWord(0x84000f1c,value) && value==0x0500000b);
    std::vector<uint32_t> phaseOrder,audienceOrder;uint32_t priorPhase=0;
    for(unsigned i=0;i<1200 && !game.trioAttack.pendingFunction() && game.trioAttack.commandActor()!=1;++i){
        game.tick(Input{});assert(game.trioAttack.readWord(0x84000f18,value));
        if(value!=priorPhase){phaseOrder.push_back(value);priorPhase=value;}
        assert(game.trioAttack.readWord(0x84002858,value));
        if(value&8){assert(game.trioAttack.readWord(0x84016034,value));if(audienceOrder.empty() || audienceOrder.back()!=value)audienceOrder.push_back(value);}
    }
    assert(phaseOrder==std::vector<uint32_t>({0x04000002,0x04000003,0x04000004,0x04000005,0x04000006,0x04000000,0x04000001,0x04000002}));
    assert(audienceOrder==std::vector<uint32_t>({0,13,15}));
    assert(game.trioAttack.readWord(0x84002858,value) && !(value&8));
    assert(game.trioAttack.readWord(0x84000f14,value) && value==0x0300004c);
    assert(game.trioAttack.readWord(0x84000000,value) && value>>16==2); // Only source TurnFirst increments the turn.
    assert(game.trioAttack.pendingFunction()==0 && game.trioAttack.finished());
    assert(game.trioAttack.readWord(0x84106010,value) && value==0x9352ff60); // Frankly participates through his original kind.
    assert(game.trioAttack.readWord(0x84106024,value) && value>>24==2); // Nonacting Frankly still receives both turn preparations.
    assert(game.trioAttack.readWord(0x84016f54,value) && (value&255)==2); // Source turn action-record byte16f57.
    assert(game.trioAttack.readWord(0x84016160,value) && value==3); // First TurnEnd chance increment survived the source RNG test.
    assert(game.trioAttack.readWord(0x8401615c,value) && value==0);
    assert(game.trioAttack.readWord(0x84000420,value) && value==1); // Source phase selection returned to Mario.
    assert(game.trioAttack.readWord(0x841042a0,value) && value==0x935238e0); // Installed Spiked root.
    assert(game.trioAttack.readWord(0x841052ac,value) && value==1);
    assert(game.trioAttack.readWord(0x841052a8,value) && value==0);
    assert(game.trioAttack.readWord(0x84000ee0,value) && value==5); // Source sampler's actor ID, not a menu actor.
    assert(game.trioAttack.commandActor()==1 && game.trioCommands.actor==1 && game.trioCommands.state==TrioBattleCommands::State::Command);
    const auto phaseChecks=std::count_if(game.trioAttack.requests().begin(),game.trioAttack.requests().end(),[](const ProfessorServiceRequest& r){return r.function==0x80113708;});
    assert(phaseChecks==5 && !game.trioBattleIntro.message.active()); // Frankly gates phase3/4/5 without lecture replay.
    assert(game.trioBattleIntro.actors[5].pose=="TGK_S_1"); // Original type43 band fallback69.
    assert(game.trioAttack.readWord(0x8410629c,value) && value==0); // Finished tracked phase hook cleared by barrier.
    assert(game.trioAttack.readWord(0x8410110c,value) && value>>16==10);
    assert(game.trioAttack.readWord(0x8410210c,value) && value>>16==7); // Original startup RNG now selects Goombella for both enemy hits.
    assert(game.trioAttack.readWord(0x84101278,value) && value==0);
    assert(game.trioAttack.readWord(0x84102278,value) && value==0x10117);
    assert(game.trioAttack.readWord(0x84001cb0,value) && value==2);
    assert(game.trioAttack.readWord(0x84001d34,value) && value==0); // Private reset independent of general pad.
    assert(game.trioBattleIntro.actors[0].pose=="M_S_1" && game.trioBattleIntro.actors[4].pose=="PTK_S_1");
    for(unsigned id:{4u,5u})assert(game.trioAttack.readWord(0x84100020+id*0x1000,value) && ((value>>24)&255)==0 && ((value>>8)&255)==1);
    assert(game.progress.originalStory==11 && game.fieldBattleState().result==0 && !game.fieldControlEnabled() && !game.saveRequested);
    assert(game.progress.position.x==fieldPosition.x && game.progress.position.z==fieldPosition.z && game.progress.hp==checkpoint.hp);
    game.trioCommands.chooseCommand(1);game.tick(advance);
    assert(game.trioCommands.state==TrioBattleCommands::State::Target && game.trioCommands.count()==1 && game.trioCommands.selected()[0]==4);
    game.tick(advance);game.tick(Input{});
    for(unsigned n=0;n<1500 && !game.trioAttack.pendingFunction() && game.trioAttack.commandActor()!=2;++n)game.tick(Input{});
    assert(!game.trioAttack.pendingFunction() && game.trioAttack.commandActor()==2);
    assert(game.trioAttack.readWord(0x8410410c,value) && value>>16==1);
    assert(game.trioAttack.readWord(0x84104278,value) && value==0x10117);
    assert(game.trioAttack.readWord(0x84001cb8,value) && value==0);
    assert(game.trioBattleIntro.actors[0].pose=="M_S_1" && game.trioBattleIntro.actors[5].pose=="TGK_S_1");
    for(unsigned o:{0x1ca0u,0x1ca4u,0x1ca8u,0x1cacu})assert(game.trioAttack.readWord(0x84000000+o,value) && value==0);
    for(unsigned scenario=0;scenario<3;++scenario) {
        const bool successful=scenario!=0,fastReward=scenario==2;
        Game charged;charged.restore(checkpoint);
        if(fastReward) {
            // Explicit checkpoint history of the authored Goombella join.
            charged.progress.originalPouch=createProloguePouch();assert(charged.progress.originalPouch);
            charged.progress.originalPouch->half(14,1);
            // Explicit retained material history proves copying live owners,
            // rather than substituting creation defaults or fixed light2.
            for(unsigned i=0;i<3;++i) {
                auto* owner=charged.fieldPartyState().pose(charged.fieldMarioBattleState().poseHandles[i]);assert(owner);
                owner->materialFlags=0x1800u+i;owner->materialLightFlags=0x42u+i;
            }
        }
        for(unsigned n=0;n<3600 && !charged.trioBattleActive;++n)charged.tick(advance);
        for(unsigned n=0;n<3600 && !charged.trioBattleIntro.finished;++n)charged.tick(advance);
        for(unsigned n=0;n<3000 && !charged.trioAttack.pendingFunction();++n){
            if(charged.trioAttack.readWord(0x84000000,value) && value>>16==2 && charged.trioAttack.commandActor()==1)break;
            Input next;next.physicalSnapshot=true;
            const auto state=charged.trioCommands.state;
            if(!charged.trioAttack.active() && (state==TrioBattleCommands::State::Command || state==TrioBattleCommands::State::Target))next=advance;
            charged.tick(next);
        }
        assert(!charged.trioAttack.pendingFunction() && charged.trioCommands.actor==1);
        charged.trioCommands.chooseCommand(1);charged.tick(advance);charged.tick(advance);
        bool released=false,heldSeen=false;uint32_t settledResult=1,classification=0;
        for(unsigned n=0;n<1500 && !charged.trioAttack.pendingFunction() && charged.trioAttack.commandActor()!=2;++n){
            if(charged.trioAttack.readWord(0x84001f50,value) && value>=uint32_t(successful?5:2))released=true;
            Input next;next.physicalSnapshot=true;next.x=released?0.f:-1.f;charged.tick(next);
            if(!released && charged.trioAttack.readWord(0x84001034,value))heldSeen|=(value&0x40000)!=0;
            if(charged.trioAttack.readWord(0x84001cb8,value) && !(value&1)){settledResult=value;assert(charged.trioAttack.readWord(0x84001cf0,classification));}
        }
        assert(!charged.trioAttack.pendingFunction() && charged.trioAttack.commandActor()==2);
        assert(heldSeen && released && settledResult==(successful?2u:0u) && classification==(successful?3u:1u));
        assert(charged.trioAttack.readWord(0x8410410c,value) && value>>16==(successful?0u:1u));
        assert(charged.trioAttack.readWord(0x84104278,value) && value==(successful?0x30717u:0x10117u));
        assert(charged.trioBattleIntro.actors[5].hidden==successful);
        assert(charged.trioAttack.readWord(0x84000030,value) && value==(successful?0u:0x84104000u));
        for(unsigned o:{0x1ca0u,0x1ca4u,0x1ca8u,0x1cacu})assert(charged.trioAttack.readWord(0x84000000+o,value) && value==0);
        assert(charged.progress.originalStory==11 && charged.fieldBattleState().result==0 && !charged.fieldControlEnabled());
        if(successful){
            charged.tick(advance);assert(charged.trioCommands.state==TrioBattleCommands::State::Target && charged.trioCommands.count()==1 && charged.trioCommands.selected()[0]==5);
            charged.tick(advance);charged.tick(Input{});
            for(unsigned n=0;n<1500 && !charged.trioAttack.pendingFunction() && !charged.trioAttack.finished();++n)charged.tick(Input{});
            assert(!charged.trioAttack.pendingFunction() && charged.trioAttack.finished());
            assert(charged.trioAttack.readWord(0x8410510c,value) && value>>16==1);
            assert(charged.trioAttack.readWord(0x84105278,value) && value==0x410117);
            assert(charged.trioAttack.readWord(0x84105008,value) && value==1);
            assert(charged.trioAttack.readWord(0x84105010,value) && value==0x93521b48); // Original flying kind definition retained.
            assert(charged.trioAttack.readWord(0x842051ac,value) && value==9);
            assert(charged.trioAttack.readWord(0x84205004,value) && value==0x93522800);
            assert(charged.trioAttack.readWord(0x842051bc,value) && value==0x93522728);
            assert(charged.trioAttack.readWord(0x84105018,value) && value==0x935227c8);
            assert(charged.trioAttack.readWord(0x84105034,value) && value==0);
            assert(charged.trioBattleIntro.actors[4].position[1]==0 && !charged.trioBattleIntro.actors[4].hidden);
            for(unsigned n=0;n<1500 && !charged.trioAttack.pendingFunction() && charged.trioAttack.commandActor()!=1;++n)charged.tick(Input{});
            assert(!charged.trioAttack.pendingFunction() && charged.trioAttack.commandActor()==1);
            assert(charged.trioAttack.readWord(0x841052a0,value) && value==0x935229e4);
            assert(charged.trioAttack.readWord(0x8410528c,value) && value==0x93523194);
            assert(charged.trioAttack.readWord(0x841052b0,value) && value==0x935229b4);
            assert(charged.trioAttack.readWord(0x841052a4,value) && value==0x935229e4);
            assert(charged.trioBattleIntro.actors[4].pose=="KUR_S_1");
            uint32_t marioHp=0,partnerHp=0;assert(charged.trioAttack.readWord(0x8410110c,marioHp) && charged.trioAttack.readWord(0x8410210c,partnerHp));
            assert((marioHp>>16)+(partnerHp>>16)==16); // The original grounded attacker dealt one damage to its sampled target.
            assert(std::string(charged.trioCommands.targetName(5))==std::string(charged.trioCommands.targetName(3)));
            charged.trioCommands.chooseCommand(1);charged.tick(advance);
            assert(charged.trioCommands.count()==1 && charged.trioCommands.selected()[0]==5); // Ground replacement admits Hammer.
            charged.tick(advance);charged.tick(Input{});
            for(unsigned n=0;n<1500 && !charged.trioAttack.pendingFunction() && !charged.trioAttack.rewardEffect();++n)charged.tick(Input{});
            assert(charged.trioAttack.pendingFunction()==0 && charged.trioAttack.rewardEffect());
            assert(charged.trioAttack.readWord(0x84000034,value) && value==0 && charged.trioBattleIntro.actors[4].hidden);
            assert(charged.trioAttack.readWord(0x84105278,value) && value==0x10717);
            assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x0700000b);
            assert(charged.trioAttack.readWord(0x84400010,value) && value==1); // Packed original result, not field completion.
            assert(charged.trioAttack.readWord(0x84000f28,value) && value!=0);
            const auto end=value;
            assert(charged.trioAttack.readWord(0x8041e80c,value) && value==0x85600020);
            assert(end>=0x85600040 && end<0x85664000); // Source spotlight and slot allocations precede EndingWork.
            assert(charged.trioAttack.readWord(end-28,value) && value==0x2c0);
            assert(charged.trioAttack.readWord(end-24,value) && (value>>16)==1);
            assert(charged.trioAttack.readWord(end+0x2c0+4,value) && value==0x85664000-(end+0x2c0));
            for(unsigned n=0x2ac;n<0x2c0;n+=4)assert(charged.trioAttack.readWord(end+n,value) && value==0); // All704 bytes were cleared.

            assert(charged.trioAttack.readWord(end,value) && value==0);
            assert(charged.trioAttack.readWord(end+0x30,value) && value==2);
            assert(charged.trioAttack.readWord(end+0x29c,value) && value==0x85200000);
            assert(charged.trioAttack.readWord(end+0x2a0,value) && value>=60 && value<180);
            assert(charged.trioAttack.readWord(end+0x2a4,value) && value==0);
            assert(charged.trioAttack.rewardIcons());const auto& icons=*charged.trioAttack.rewardIcons();
            assert(!icons[0].hidden && icons[0].alpha==255 && std::fabs(icons[0].position[0]-6)<.0001f && icons[0].position[1]==25);
            for(unsigned n=1;n<19;++n)assert(icons[n].hidden && icons[n].alpha==0);
            assert(charged.trioBattleIntro.actors[0].pose=="M_V_2" && charged.trioBattleIntro.actors[1].pose=="PKR_Y_1");
            assert(charged.trioAttack.readWord(end+4,value) && value==0);
            uint32_t stockExp=0;assert(charged.trioAttack.readWord(0x84000f00,stockExp) && stockExp==10);
            assert(charged.trioAttack.readWord(end+8,value) && value==0x000a000a); // Original stock EXP, remaining EXP, no pouch grant yet.
            assert(charged.trioAttack.readWord(0x85000094,value) && (value&0xffff)==0);
            for(unsigned unit:{1u,2u,6u})assert(charged.trioAttack.readWord(0x84100000+unit*0x1000+0x310,value) && ((value>>16)&255)==255);
            assert(charged.trioAttack.readWord(0x8410110c,value) && (value&0xffff)==5); // Original Mario max FP.
            assert(charged.trioAttack.readWord(0x84101110,value) && (value&0xffff)==5); // Original current FP remains5.
            assert(charged.trioAttack.readWord(0x8440010c,value) && value==0); // Actual early source item roll.
            assert(charged.trioAttack.readWord(0x84400110,value) && value<=2);
            assert(charged.trioAttack.readWord(0x84400114,value) && value<=2);
            assert(charged.trioAttack.readWord(0x85000098,value) && (value>>16)==0x0101);
            assert(charged.trioAttack.readWord(0x850000a0,value) && value==0x00090032);
            const auto& endingRequests=charged.trioAttack.requests();
            assert(std::count_if(endingRequests.begin(),endingRequests.end(),[](const ProfessorServiceRequest& r){return r.function==0x801a1a74;})==2);
            assert(std::any_of(endingRequests.begin(),endingRequests.end(),[](const ProfessorServiceRequest& r){return r.function==0x80212794 && r.rawOperands.size()==2;}));
            assert(std::any_of(endingRequests.begin(),endingRequests.end(),[](const ProfessorServiceRequest& r){return r.function==0x801a0cec && r.rawOperands==std::vector<int32_t>({-1,0});}));
            assert(std::count_if(endingRequests.begin(),endingRequests.end(),[](const ProfessorServiceRequest& r){return r.function==0x800d3300 && r.rawOperands==std::vector<int32_t>({314});})==2);
            const auto dropDraws=std::count_if(endingRequests.begin(),endingRequests.end(),[](const ProfessorServiceRequest& r){return r.function==0x80005da0;})-424;assert(dropDraws==3 || dropDraws==5 || dropDraws==7);
            assert(charged.progress.originalStory==11 && charged.fieldBattleState().result==0 && !charged.fieldControlEnabled());
            assert(charged.trioAttack.rewardEffect()->timer==1 && charged.trioAttack.rewardEffect()->depthPosition[0]==-2000);
            const unsigned countTicks=fastReward?10:40;
            for(unsigned n=0;n<countTicks;++n){Input countInput;countInput.physicalSnapshot=true;countInput.physicalButtons=fastReward && n==0?0x100:0;charged.tick(countInput);
                const unsigned granted=fastReward?n+1:(n+1)/4;
                assert(charged.trioAttack.readWord(0x85000094,value) && (value&0xffff)==granted);
                assert(charged.trioAttack.readWord(end+8,value) && (value&0xffff)==10-granted);
                assert(!charged.trioAttack.pendingFunction());
            }
            assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x0700000b);
            assert(charged.trioAttack.readWord(end+4,value) && value==(fastReward?1010u:40u));
            charged.tick(Input{});assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x0700000c);
            assert(charged.trioAttack.readWord(end+4,value) && value==(fastReward?10u:100u));
            for(unsigned n=0;n<(fastReward?10u:100u);++n)charged.tick(Input{});
            assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x0700000d && !charged.trioAttack.rewardIcons());
            assert(charged.trioAttack.rewardPresentationActive() && charged.trioAttack.rewardEffect());
            for(unsigned n=0;n<5;++n)charged.tick(Input{});
            assert(charged.trioAttack.readWord(end+0x29c,value) && value==0 && charged.trioAttack.rewardEffect());
            assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x0700000e);
            charged.tick(Input{});assert(!charged.trioAttack.pendingFunction() && charged.trioAttack.readWord(0x84000f24,value) && value==0x07000010);
            charged.tick(Input{});assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x07000019);
            charged.tick(Input{});assert(charged.trioBattleIntro.actors[0].pose=="M_V_1" && charged.trioAttack.readWord(end+4,value) && value==60);
            for(unsigned n=0;n<59;++n)charged.tick(Input{});assert(charged.trioAttack.readWord(0x84000f24,value) && value==0x0700001a);
            charged.tick(Input{});assert(charged.trioBattleIntro.actors[0].pose=="M_R_1" && charged.trioBattleIntro.actors[1].pose=="PKR_R_1");
            uint32_t retainedBattleHeader=0;assert(charged.trioAttack.readWord(0x84000000,retainedBattleHeader));
            for(unsigned n=0;n<100 && !charged.trioAttack.pendingFunction();++n)charged.tick(Input{});
            assert(charged.trioAttack.pendingFunction()==0x800f8e28u);
            const auto engineCursor=charged.trioAttack.requests().size()+1; // Source shadowMain precedes battle wrapper.
            // A missing matrix holds after Main; supplying it resumes the exact
            // depth/entry continuation without replaying the source prefix.
            charged.trioAttack.setCameraDepthSnapshot(std::nullopt);
            charged.trioAttack.tick((charged.ticks+1)*1000/60,charged.trioBattleIntro);
            assert(charged.trioAttack.pendingFunction()==0x8001086cu && charged.trioAttack.readWord(0x84000f24,value) && value==0x0700001f);
            const auto* pose=charged.trioAttack.battlePose(0x84201000);assert(pose && pose->word(0x14)==4 && pose->resource->names[4]=="M_R_1" && pose->scalar(0x44)==1 && !pose->shape.vertices.empty()); // Finishing run: original index4, not pack ordinal2.
            assert(pose->word(0x3c)==pose->word(0x28) && pose->scalar(0x40)==pose->scalar(0x30));
            const auto draw=charged.trioAttack.poseDrawRequest();assert(draw && draw->part==0x84201000 && draw->camera==4 && draw->drawClass==1);
            assert(charged.trioAttack.readWord(0x84002760,value) && value==17 && charged.trioAttack.cameraFrameState());
            assert(charged.trioAttack.readWord(0x84201060,value) && value==0x3f800000 && charged.trioAttack.readWord(0x842011a0,value) && (value&0xffff)==20);
            assert(charged.trioAttack.readWord(0x842014f4,value) && value==0xffffffff);
            const auto history=pose->word(0x3c);const auto cacheTime=pose->scalar(0x40);
            const auto prefixCalls=std::count_if(charged.trioAttack.requests().begin(),charged.trioAttack.requests().end(),[](const auto& request){return request.function==0x8003ed2c && !request.rawOperands.empty() && request.rawOperands[0]==0;});
            CameraDepthSnapshot explicitCamera;
            for(unsigned axis=0;axis<3;++axis)explicitCamera.view[axis][axis]=1;
            for(unsigned axis=0;axis<4;++axis)explicitCamera.projection[axis][axis]=1;
            charged.trioAttack.setCameraDepthSnapshot(explicitCamera);
            charged.trioAttack.tick((charged.ticks+1)*1000/60,charged.trioBattleIntro);
            assert(charged.trioAttack.pendingFunction()==0x8000f704 && pose->word(0x3c)==history && pose->scalar(0x40)==cacheTime);
            assert(std::count_if(charged.trioAttack.requests().begin(),charged.trioAttack.requests().end(),[](const auto& request){return request.function==0x8003ed2c && !request.rawOperands.empty() && request.rawOperands[0]==0;})==prefixCalls);
            const auto* queue=charged.trioAttack.displayQueue();assert(queue && queue->count()==20);
            const auto* registered=queue->record(8);assert(registered && registered->camera==4 && registered->drawClass==1 && registered->callback==0x80101774 && registered->userdata==draw->part);
            assert(registered->key==400000.f-cameraQueueDepth(explicitCamera,draw->depth));
            const auto* shadow=charged.trioAttack.shadowWork();assert(shadow==charged.shadows.get() && shadow->count()==3 && shadow->record(0)->position[1]==10 && shadow->record(0)->radius>=8);
            assert(queue->record(9)->userdata==0x84202000 && queue->record(9)->callback==0x80101774);
            assert(queue->record(10)->userdata==0x84206000 && queue->record(10)->callback==0x80101774);
            const auto* frankly=charged.trioAttack.battlePose(0x84206000);assert(frankly && frankly->resource->modelName=="c_kuri_h" && frankly->resource->names.size()==17 && frankly->word(0x14)==0);
            assert(charged.trioAttack.readWord(0x84206004,value) && value==0x9353007c && charged.trioAttack.readWord(0x842061bc,value) && value==0x9353004c);
            assert(charged.trioAttack.readWord(0x842061a0,value) && (value&0xffff)==20);
            assert(queue->record(11)->camera==8 && queue->record(11)->callback==0x80100258 && queue->record(11)->key==399099);
            assert(queue->record(12)->callback==0x80138da8 && queue->record(12)->key==605000);
            assert(queue->record(13)->callback==0x801389a0 && queue->record(13)->key==606000);
            assert(charged.trioAttack.commandPalette()==charged.commandPalette.get() && charged.commandPalette->rgba[1]==193 && charged.commandPalette->rgba[3]==207);
            assert(queue->record(14)->callback==0x8011fde0 && queue->record(14)->userdata==0x84000000);
            assert(queue->record(15)->callback==0x801a38f8 && queue->record(16)->callback==0x801a38f8);
            assert(queue->record(17)->callback==0x801a37f0 && queue->record(18)->callback==0x801a36c8 && queue->record(19)->callback==0x802036d4);
            assert(charged.trioAttack.readWord(0x84000ef4,value) && (value&0x80000000) && !(value&0x30000));
            assert(charged.trioAttack.readWord(0x84000f08,value) && value==1); // Original StockExp counter mutation precedes registration.

            for(const auto extra:{0x84200000u,0x84200500u}){
                const auto* extraPose=charged.trioAttack.battlePose(extra);assert(extraPose && extraPose->word(0x14)==0);
                assert(charged.trioAttack.readWord(extra+0x204,value) && value==0);
                assert(charged.trioAttack.readWord(extra+0x1ac,value) && (value&0x03000000)==0x03000000);
                for(unsigned index=0;index<queue->count();++index)assert(queue->record(index)->userdata!=extra);

            }

            assert(shadow->record(0)->flags==1 && shadow->record(1)->flags==1); // Preparation has not been invented.

            auto* retainedCamera=charged.cameraManager->camera(4);
            retainedCamera->fov=31;retainedCamera->nearPlane=2;retainedCamera->farPlane=20000;retainedCamera->aspect=1.7f;
            charged.trioAttack.tick((charged.ticks+2)*1000/60,charged.trioBattleIntro);
            assert(queue->count()==20 && shadow->count()==3); // Camera publication does not repeat collection.
            assert(charged.trioAttack.pendingFunction()==0x8000f454); // No manual depth-lens hook is required.
            assert(charged.trioAttack.cameras()==charged.cameraManager.get());
            assert(retainedCamera->fov==31 && retainedCamera->nearPlane==2 && retainedCamera->farPlane==20000 && retainedCamera->aspect==1.7f);
            assert(retainedCamera->matrices.projection==cameraPerspective(31,1.2666666507720947f,2,20000)); // Source mode2 uses fixed aspect, retaining C3c.
            const auto callsBeforeCameraResume=std::count_if(charged.trioAttack.requests().begin(),charged.trioAttack.requests().end(),[](const auto& request){return request.function==0x8003ed2c && !request.rawOperands.empty() && request.rawOperands[0]==0;});
            assert(queue->count()==20 && shadow->count()==3 && !charged.cameraManager->currentIndex());
            assert(charged.cameraManager->camera(2)->projectionKind==1 && charged.cameraManager->camera(8)->viewport[4]==.1f);
            assert(charged.cameraManager->camera(4)->matrices.view==charged.trioAttack.cameraDepthSnapshot()->view);
            const auto publishedProjection=charged.trioAttack.cameraDepthSnapshot()->projection;
            charged.trioAttack.setCameraDepthLensAdapter(30,608.f/480.f,1,32768);
            assert(charged.trioAttack.cameraDepthSnapshot()->projection==publishedProjection); // Draw consumes publication, not a recomputed adapter.
            assert(charged.cameraManager->camera(5)->eye==charged.cameraManager->camera(4)->eye && charged.cameraManager->camera(7)->at==charged.cameraManager->camera(4)->at);
            assert(charged.cameraManager->camera(6)->eye[2]!=charged.cameraManager->camera(4)->eye[2]);
            assert(std::count_if(charged.trioAttack.requests().begin(),charged.trioAttack.requests().end(),[](const auto& request){return request.function==0x8003ed2c && !request.rawOperands.empty() && request.rawOperands[0]==0;})==callsBeforeCameraResume);
            charged.trioAttack.tick((charged.ticks+4)*1000/60,charged.trioBattleIntro);
            assert(charged.trioAttack.pendingFunction()==0x8007ac40 && queue->count()==20 && !charged.cameraManager->currentIndex());
            assert(charged.originalSequence->state.current==2 && charged.originalSequence->state.requested==4);
            unsigned sequenceSyncs=0;
            struct SequenceFixture {Game* game;unsigned* syncs;} sequenceFixture{&charged,&sequenceSyncs};
            OriginalSequenceServices sequenceServices;sequenceServices.user=&sequenceFixture;
            sequenceServices.drawSync=[](void* user){++*static_cast<SequenceFixture*>(user)->syncs;return true;};
            sequenceServices.callback=[](void* user,uint32_t identity,OriginalSequenceWork&){
                if(identity!=0x80007390)return false;
                originalFieldSequenceExit(static_cast<SequenceFixture*>(user)->game->fieldSystemFlags);return true;
            };
            assert(!charged.originalSequence->main(sequenceServices) && sequenceSyncs==1);
            assert(charged.originalSequence->state.current==4 && charged.originalSequence->pending()==0x80073f20);
            assert(!charged.originalSequence->main(sequenceServices) && sequenceSyncs==1); // No replay of sync/Exit/publication.
            SoundFlagCursor soundFixture;
            assert(charged.soundFlags->set(0x20,{},soundFixture));
            const auto retainedSound=charged.soundFlags->value();
            charged.trioAttack.setSoundFlags({}); // Deliberately remove the native leaf provider to test its independent hold.
            charged.beroExec->onOff(1,1); // Explicit original resolved clear request restores knowledge of consumer bit1.
            charged.trioAttack.tick((charged.ticks+4)*1000/60,charged.trioBattleIntro);
            assert(charged.trioAttack.pendingFunction()==0x800dc0f8 && queue->count()==20); // Retained current4 reaches the separate sound-clear provider.
            charged.trioAttack.setSoundFlags(charged.soundFlags);
            charged.trioAttack.tick((charged.ticks+5)*1000/60,charged.trioBattleIntro);
            assert(charged.trioAttack.pendingFunction()==0x802ba56c && queue->count()==21 && charged.cameraManager->currentIndex()==0);
            assert(charged.soundFlags->value()==uint16_t(retainedSound&~0x20u));
            charged.trioAttack.tick((charged.ticks+6)*1000/60,charged.trioBattleIntro);
            assert(queue->count()==21 && charged.soundFlags->value()==uint16_t(retainedSound&~0x20u)); // Native clear and registration do not replay at missing viewport.
            struct FrameFixture {NativeCameraDraw backend;std::vector<NativeCameraPacket> packets;} frame;
            NativeCameraSubmit submit{&frame,[](void* user,const NativeCameraPacket& packet){static_cast<FrameFixture*>(user)->packets.push_back(packet);return true;}};
            CameraDrawServices frameServices;frameServices.user=&submit;
            frameServices.request=[](void* user,const CameraDrawRequest& request){
                const auto& submit=*static_cast<NativeCameraSubmit*>(user);
                return static_cast<FrameFixture*>(submit.user)->backend.request(request,submit);
            };
            assert(charged.trioAttack.finishingDrawPending());
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.trioAttack.pendingFunction()==0x802b9380 && charged.cameraManager->currentIndex()==1 && queue->count()==21);
            assert(std::count_if(frame.packets.begin(),frame.packets.end(),[](const auto& p){return p.kind==NativeCameraCommand::DummyDraw;})==1);
            assert(frame.backend.restore(submit));const auto restored=frame.packets.size();
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(frame.packets.size()==restored && queue->count()==21 && charged.trioAttack.pendingFunction()==0x802b9380);
            charged.trioAttack.tick((charged.ticks+7)*1000/60,charged.trioBattleIntro);
            assert(frame.packets.size()==restored && queue->count()==21); // Simulation never emits frame-scoped GPU work.
            NativeDisplayState pipeline;pipeline.afterDummy(); // Actual preceding dummy in this frame fixture established this state.
            std::vector<NativeDisplayPipeline> statePackets;
            struct PipelineFixture {NativeDisplayState* state;std::vector<NativeDisplayPipeline>* packets;} pipelineFixture{&pipeline,&statePackets};
            frameServices.display.user=&pipelineFixture;
            frameServices.display.state=[](void* user,const DisplayStateRequest& request){
                auto& f=*static_cast<PipelineFixture*>(user);
                NativeDisplaySubmit submit{f.packets,[](void* user,const NativeDisplayPipeline& p){static_cast<std::vector<NativeDisplayPipeline>*>(user)->push_back(p);return true;}};
                return f.state->request(request,submit);
            };
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.trioAttack.pendingFunction()==0x8002d3d8 && charged.cameraManager->currentIndex()==1 && queue->count()==21);
            assert(statePackets.size()==7 && pipeline.state()->color && pipeline.state()->alpha && !pipeline.state()->depthWrite && pipeline.state()->sourceBlend==4 && pipeline.state()->destinationBlend==5 && pipeline.state()->rasterAdmitted());
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(statePackets.size()==7 && charged.trioAttack.pendingFunction()==0x8002d3d8); // Captured shadow callback resumes without replaying class states.
            struct ShadowFrameBinding {const TrioAttack* attack;DisplayDispatchServices original;unsigned requests=0,lookups=0;} binding{&charged.trioAttack,frameServices.display};
            frameServices.display.user=&binding;
            frameServices.display.state=[](void* user,const DisplayStateRequest& r){auto& b=*static_cast<ShadowFrameBinding*>(user);return b.original.state(b.original.user,r);};
            frameServices.display.callback=[](void* user,uint32_t camera,uint32_t identity,uint32_t borrowed){
                auto& b=*static_cast<ShadowFrameBinding*>(user);if(identity!=0x8002d3d8)return false;
                ShadowDisplayServices source;source.user=&b;
                source.gx=[](void* user,const ShadowGXRequest&){++static_cast<ShadowFrameBinding*>(user)->requests;return true;};
                source.texture44=[](void* user,ShadowTexture& texture){++static_cast<ShadowFrameBinding*>(user)->lookups;texture=ShadowTexture::N64Texture44;return true;};
                source.collision.ray=[](void*,const ShadowCollisionRay&,ShadowCollisionHit&){return ShadowRayResult::Unavailable;}; // Explicit host query hold, independent of real runtime registry.
                return b.attack->resumeShadowDisplay(camera,borrowed,source);
            };
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.cameraManager->currentIndex()==2 && queue->count()==21 && charged.trioAttack.pendingFunction()==0x800158a4);
            assert(binding.requests==73 && binding.lookups==1);
            const auto classStates=statePackets.size();charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(binding.requests==73 && binding.lookups==1 && statePackets.size()==classStates); // Completed camera1 setup never replays while camera2 ray is held.
            struct NativeRayBinding {const TrioAttack* attack;DisplayDispatchServices original;} nativeRay{&charged.trioAttack,frameServices.display};
            frameServices.display.user=&nativeRay;
            frameServices.display.state=[](void* user,const DisplayStateRequest& r){auto& b=*static_cast<NativeRayBinding*>(user);return b.original.state(b.original.user,r);};
            frameServices.display.callback=[](void* user,uint32_t camera,uint32_t identity,uint32_t borrowed){
                if(identity!=0x8002d3d8)return false;
                return static_cast<NativeRayBinding*>(user)->attack->resumeShadowDisplay(camera,borrowed,{}); // Retained runtime registry, no field/B.floor substitute.
            };
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.trioAttack.pendingFunction()==0x802b7b54 && charged.cameraManager->currentIndex()==2 && charged.trioAttack.battleHitRegistry()->known());
            const auto* prepared=charged.trioAttack.shadowWork();assert(prepared->count()==3);
            const auto& shadowProfile=prepared->profiles[1];const auto* camera4=charged.cameraManager->camera(4);
            assert(prepared->profileIndex==1 && shadowProfile.at==camera4->at);
            assert(shadowProfile.eye[0]==camera4->at[0] && shadowProfile.eye[1]==float(1000.f+camera4->at[1]) && shadowProfile.eye[2]==camera4->at[2]);
            assert((shadowProfile.up==std::array<float,3>{0,0,-1}));
            for(unsigned i=0;i<3;++i){const auto& r=*prepared->record(i);assert(r.flags==3 && std::fabs(r.matrix[7]-1.f)<.000001f && r.matrix[3]==r.position[0] && r.matrix[11]==r.position[2]);}
            const auto savedMatrix=prepared->record(0)->matrix;charged.trioAttack.invalidateBattleHitRegistry();
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(!charged.trioAttack.battleHitRegistry()->known() && prepared->record(0)->matrix==savedMatrix); // Completed preparation is not replayed by the subsequent profile/image hold.
            // Bind full counter0 class5 against the retained runtime camera/GP
            // owners; the counter0 auxiliary callbacks then reach the first camera4 audience callback.
            struct ShadowPassBinding {const TrioAttack* attack;DisplayDispatchServices original;NativeCameraDraw* backend;NativeCameraSubmit* submit;unsigned requests=0,passes=0;uint32_t finalMaterial=0;} projectionBinding{&charged.trioAttack,frameServices.display,&frame.backend,&submit};
            frameServices.display.user=&projectionBinding;
            frameServices.display.state=[](void* user,const DisplayStateRequest& r){auto& b=*static_cast<ShadowPassBinding*>(user);return b.original.state(b.original.user,r);};
            frameServices.display.callback=[](void* user,uint32_t camera,uint32_t identity,uint32_t borrowed){
                auto& b=*static_cast<ShadowPassBinding*>(user);if(identity!=0x8002d3d8)return false;
                ShadowDisplayServices source;source.user=&b;
                source.gx=[](void* user,const ShadowGXRequest& r){auto& b=*static_cast<ShadowPassBinding*>(user);++b.requests;if(r.kind==ShadowGX::ChanMaterial)b.finalMaterial=r.color;return true;};
                source.pass=[](void* user,ShadowPassRequest& r){
                    auto& b=*static_cast<ShadowPassBinding*>(user);++b.passes;
                    switch(r.kind){
                    case ShadowPassKind::InvalidateTextures:return true; // Host trace; native provider queues the actual cache clear.
                    case ShadowPassKind::ReadViewport:r.viewport=*b.backend->viewport();return true;
                    case ShadowPassKind::ReadScissor:r.scissor=*b.backend->scissor();return true;
                    case ShadowPassKind::Viewport:return b.backend->viewport(r.viewport,*b.submit,true);
                    case ShadowPassKind::Scissor:return b.backend->scissor(r.scissor,*b.submit,true);
                    case ShadowPassKind::Projection:return b.backend->projection(r.projection,*b.submit);
                    }
                    return false;
                };
                return b.attack->resumeShadowDisplay(camera,borrowed,source);
            };
            const auto originalCamera2View=charged.cameraManager->camera(2)->matrices.view;
            const auto originalViewport=frame.backend.viewport();const auto originalScissor=frame.backend.scissor();
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.trioAttack.pendingFunction()==0x801a38f8 && charged.cameraManager->currentIndex()==4 && queue->count()==21);
            assert(projectionBinding.requests==26 && projectionBinding.passes==24 && projectionBinding.finalMaterial==0xffffffff);
            assert(charged.cameraManager->camera(2)->matrices.view==originalCamera2View && frame.backend.viewport()==originalViewport && frame.backend.scissor()==originalScissor);
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(projectionBinding.requests==26 && projectionBinding.passes==24 && prepared->record(0)->matrix==savedMatrix);
            // Actual retained camera4 callback: the original initializer's
            // zero actor bank completes its genuine prefix/200 admissions.
            for(unsigned i=0;i<200;++i){
                assert(charged.trioAttack.readWord(0x84002a14+0x134*i,value) && value==0);
                assert(charged.trioAttack.readWord(0x84002a58+0x134*i,value) && (value>>24)==0);
            }
            struct AudienceBinding {const TrioAttack* attack;DisplayDispatchServices original;NativeShadowProgram program;unsigned requests=0;} audienceBinding{&charged.trioAttack,frameServices.display};
            audienceBinding.program.afterDummy();
            frameServices.display.user=&audienceBinding;
            frameServices.display.state=[](void* user,const DisplayStateRequest& r){auto& b=*static_cast<AudienceBinding*>(user);return b.original.state(b.original.user,r);};
            frameServices.display.callback=[](void* user,uint32_t camera,uint32_t identity,uint32_t borrowed){
                auto& b=*static_cast<AudienceBinding*>(user);
                if(identity!=0x801a38f8)return b.original.callback(b.original.user,camera,identity,borrowed);
                BattleAudienceCallbackServices source;source.user=&b;
                source.request=[](void* user,const BattleAudienceGXRequest& r){auto& b=*static_cast<AudienceBinding*>(user);
                    ShadowGXRequest q;q.kind=static_cast<ShadowGX>(r.function);q.values=r.arguments;
                    const auto next=b.program.request(q);assert(next);b.program=*next;++b.requests;return true;};
                return b.attack->resumeAudienceCallback(camera,borrowed,source);
            };
            assert(!charged.trioAttack.resumeAudienceCallback(3,0,{})); // Must match the actual borrowed invocation.
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(audienceBinding.requests==16 && charged.cameraManager->currentIndex()==4 && charged.trioAttack.pendingFunction()==0x80101774);
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(audienceBinding.requests==16); // Completed prefix never replays at the following actor callback hold.
            // Inactivity is established from each actual retained bank, not
            // inferred from the200 audience actors or absent texture services.
            for(unsigned i=0;i<100;++i){
                assert(charged.trioAttack.readWord(0x84002858+0xf25c+0x48*i,value) && (value&3)==0);
                assert(charged.trioAttack.readWord(0x84002858+0x10e7c+0x60*i,value) && !(value&1));
            }
            assert(!charged.trioAttack.resumeAudienceAux(4,0x801a37f0,0,{})); // Different live borrowed callback cannot be consumed.
            // Run the actual borrowed camera4 part through matrix/material work.
            // No inner geometry service is bound: the queue must retain its owner.
            struct PartBinding {const TrioAttack* attack;DisplayDispatchServices original;bool enabled=false;unsigned gx=0;} partBinding{&charged.trioAttack,frameServices.display};
            frameServices.display.user=&partBinding;
            frameServices.display.state=[](void* user,const DisplayStateRequest& r){auto& b=*static_cast<PartBinding*>(user);return b.original.state(b.original.user,r);};
            frameServices.display.callback=[](void* user,uint32_t camera,uint32_t identity,uint32_t borrowed){auto& b=*static_cast<PartBinding*>(user);
                if(identity==0x80101774){BattlePoseInnerServices inner;inner.user=&b;
                    inner.gx=[](void* user,const BattleAudienceGXRequest&){auto& b=*static_cast<PartBinding*>(user);if(!b.enabled)return false;++b.gx;return true;};
                    BattlePartDisplayServices part;part.inner=&inner;return b.attack->resumePartDisplay(camera,borrowed,part);}
                return b.original.callback(b.original.user,camera,identity,borrowed);};
            const auto* actualPose=charged.trioAttack.battlePose(0x84201000);assert(actualPose);
            uint32_t oldHistory=0;assert(charged.trioAttack.readWord(0x84201228,oldHistory));
            assert(!charged.trioAttack.resumePartDisplay(3,0x84201000,{}));
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert((charged.trioAttack.pendingFunction()==0x8001ca2c || charged.trioAttack.pendingFunction()==0x802b93d8) && queue->count()==21);
            assert(charged.trioAttack.readWord(0x84201218,value) && actualPose->word(0xf0)==value);
            assert(actualPose->word(0xe8)&0x40);
            assert(charged.trioAttack.readWord(0x84201228,value) && value==oldHistory);
            const auto material=actualPose->word(0xf0),flags=actualPose->word(0xe8);
            charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.trioAttack.pendingFunction()==0x802b93d8 && actualPose->word(0xf0)==material && actualPose->word(0xe8)==flags);
            partBinding.enabled=true;charged.trioAttack.resumeFinishingDraw(frameServices);
            assert(charged.trioAttack.pendingFunction()==0x80296a54 && partBinding.gx==4 && queue->count()==21);
            assert(charged.trioAttack.readWord(0x84201228,value) && value==oldHistory);
            charged.trioAttack.resumeFinishingDraw(frameServices);assert(partBinding.gx==4);



            assert(charged.trioAttack.readWord(0x84201204,value) && (value&3)==2); // Source name request consumed; forced path stays active.
            assert(charged.trioAttack.readWord(0x84000ef4,value) && (value&0x80000000));
            constexpr uint32_t slotWords[3][8]{{0,1,2,1,2,0,3,4},{3,1,0,1,4,2,0,2},{0,1,2,3,4}};
            for(unsigned reel=0;reel<3;++reel) {
                const auto address=0x84016184+0x5c*reel;uint32_t array=0;
                assert(charged.trioAttack.readWord(address,value) && value==reel);
                assert(charged.trioAttack.readWord(address+0x14,array) && array>=0x85600000);
                const auto count=reel==2?5:8;
                assert(charged.trioAttack.readWord(address+0x18,value) && value==count);
                assert(charged.trioAttack.readWord(address+4,value) && !(value&1));
                assert(charged.trioAttack.readWord(address+0x58,value) && value==0xff000000);
                for(unsigned j=0;j<unsigned(count);++j)assert(charged.trioAttack.readWord(array+4*j,value) && value==slotWords[reel][j]);
            }

            const auto& engineRequests=charged.trioAttack.requests();
            constexpr uint32_t enginePrefix[]{0x800f7b28,0x8011a43c,0x8011f1ec,0x800fa5b0,0x80149798,0x801aa168,0x80139150,0x8013800c,0x802168a4,0x801a836c};
            assert(engineRequests.size()>=engineCursor+10);
            for(unsigned i=0;i<10;++i)assert(engineRequests[engineCursor+i].function==enginePrefix[i]);
            assert(charged.battleSpotlight->work[0]==5 && charged.battleSpotlight->work[1]==0x85600040 && charged.battleSpotlight->work[2]==0);
            assert(charged.trioAttack.readWord(0x8401900c,value) && value==0);
            const auto* stageMap=charged.trioAttack.stageMap(false);const auto* backMap=charged.trioAttack.stageMap(true);
            assert(stageMap && backMap && stageMap->vertices()==3393 && backMap->vertices()==11721);
            assert(!(*stageMap->flags("isi_1")&1) && !(*stageMap->flags("isi_2")&1) && (*stageMap->flags("B_kabe")&1));
            assert((*backMap->flags("escape_wall")&1) && (*backMap->flags("spotlight")&1));
            for(unsigned record=0;record<7;++record)assert(charged.trioAttack.readWord(0x8401715c+record*0x7c,value) && value==record+1);
            assert(charged.trioAttack.readWord(0x8401715c+7*0x7c,value) && value==0);
            assert(charged.fieldMarioBattleState().partyIndices[0]==(fastReward?0:255) && charged.fieldMarioBattleState().partyIndices[1]==255);
            if(fastReward) {
                assert(charged.fieldPartyState().pointer(0) && !charged.fieldPartyState().pointer(1));
                const auto* actor=charged.fieldPartyState().pointer(0);
                assert(actor->word(0)==1 && actor->word(4)==0 && actor->word(0xc)==3);
                const auto* pose=charged.fieldPartyState().pose(int32_t(actor->word(0xc)));assert(pose && pose->resource && pose->flags==1 && pose->mode==2 && pose->archiveType==0);
                assert(pose->materialFlags==0x1800 && pose->materialLightFlags==0x42 && !pose->shape.positions.empty() && !pose->shape.world.empty());
                assert(pose->shape.vertices.empty()); // Entry has not selected a pose or submitted a draw.
                assert(actor->word(0x160)==FieldMarioOwnerAddress && actor->word(0x164)==charged.fieldMarioBattleState().cameraId);
                assert(charged.fieldMarioBattleState().cameraId==4);
                assert(actor->bytes[0x31]==1 && actor->bytes[0x2f]==0 && actor->word(0x18)==(charged.progress.originalPouch->half(20)<=5?0x802cb52cu:0x802cb4dcu) && actor->word(8)==0x2000);
                assert(actor->bytes[0x32]==0 && actor->bytes[0x33]==1 && actor->word(0xf0)==0x41c00000 && actor->word(0xf4)==0x41d00000);
                assert(actor->bytes[0x34]==1 && actor->bytes[0x38]==10 && actor->word(0x70)==0x3f800000 && actor->word(0x74)==0x3f800000 && actor->word(0x78)==0x3f800000);
                assert(actor->word(0x58)==actor->word(0xb8) && actor->word(0x5c)==actor->word(0xbc) && actor->word(0x60)==actor->word(0xc0));
                assert(charged.fieldPartyState().current(charged.fieldMarioBattleState())==0);
                assert(std::count_if(charged.trioAttack.requests().begin(),charged.trioAttack.requests().end(),[](const ProfessorServiceRequest& r){return r.function==0x800c6718;})==2);
                assert(charged.fieldMarioBattleState().savedPartyKinds[0]==1 && charged.fieldPartyState().currentDefinition==0x80314410);
                assert(charged.progress.originalPouch->half(14)==1);
            }else {
                assert(!charged.fieldPartyState().pointer(0) && !charged.fieldPartyState().pointer(1));
                assert(!(charged.progress.originalPouch->half(14)&1));
            }
            assert(charged.fieldMarioBattleState().secondaryFlags==0 && charged.fieldMarioBattleState().controlDepth==1 && (charged.fieldMarioBattleState().flags&2));
            const auto& audio=charged.trioAttack.audienceAudioRequests();assert(audio.size()==3); // Actual initial noise attempt, then two ending claps.
            for(const auto& request:audio)assert(!request.key && request.right==0 && request.left==0 && request.handle && *request.handle==-1);
            assert(charged.trioAttack.readWord(0x84015c78,value) && value==0);
            assert(charged.trioAttack.readWord(0x84015c7c,value) && value==0xffffffff); // Actual failed native request, not a fabricated silent voice0.
            assert(charged.trioAttack.readWord(0x84015c90,value) && value==30); // Original SetVolAll duration persists.
            assert(charged.trioAttack.readWord(0x84400018,value) && value==0x00060000); // Actual captured Rule0 descriptor status6.
            assert(charged.trioAttack.readWord(0x84000f28,value) && value==0);
            assert(charged.trioAttack.readWord(0x84000004,value) && value==4);
            assert(charged.trioAttack.readWord(0x84000000,value) && value==retainedBattleHeader);
            assert(charged.trioAttack.readWord(0x85600020,value) && value==0x85600120);
            assert(charged.trioAttack.readWord(0x85600024,value) && value==0xe0);
            assert(charged.trioAttack.readWord(0x85600028,value) && value==0x00010000);
            assert(charged.trioAttack.readWord(end-32,value) && value==0);
            assert(charged.trioAttack.readWord(end-28,value) && value==0x85664000-(end-32));
            assert(charged.trioAttack.readWord(end-24,value) && value==0);
            assert(charged.trioAttack.endingFadeAlpha()==255 && charged.professorStory.actors[3].npcBattleFlags==0x100);
            assert(charged.progress.originalPouch && charged.progress.originalPouch->half(0x96)==10);
            assert(charged.trioAttack.readWord(0x8410110c,value) && charged.progress.originalPouch->half(0x70)==int16_t(value>>16));
            assert(charged.trioAttack.readWord(0x84101110,value) && charged.progress.originalPouch->half(0x74)==int16_t(value&0xffff));
            assert(charged.trioAttack.readWord(0x8410210c,value) && charged.progress.originalPouch->half(20)==int16_t(value>>16));
            for(unsigned n=0;n<110;++n)charged.tick(Input{});assert(!charged.trioAttack.rewardEffect());
            const auto& finalRequests=charged.trioAttack.requests();
            assert(std::count_if(finalRequests.begin(),finalRequests.end(),[](const ProfessorServiceRequest& r){return r.function==0x800d98bc && r.rawOperands==std::vector<int32_t>{int32_t(0x802feaa8)};})==(fastReward?1:5));
            assert(charged.progress.originalStory==11 && charged.fieldBattleState().result==0 && !charged.fieldControlEnabled());

        }
    }
    for(uint32_t button:{0x100u,0x200u}) {
        Game defended;defended.restore(checkpoint);
        for(unsigned n=0;n<3600 && !defended.trioBattleActive;++n)defended.tick(advance);
        for(unsigned n=0;n<3600 && !defended.trioBattleIntro.finished;++n)defended.tick(advance);
        bool pressed=false,sawGuard=false,sawSuperguard=false;uint32_t defence=0;
        for(unsigned n=0;n<3000 && !defended.trioAttack.pendingFunction();++n){
            if(defended.trioAttack.readWord(0x84000000,value) && value>>16==2 && defended.trioAttack.commandActor()==1)break;
            Input next;next.physicalSnapshot=true;
            const auto menuState=defended.trioCommands.state;
            if(!defended.trioAttack.active() && (menuState==TrioBattleCommands::State::Command || menuState==TrioBattleCommands::State::Target))next=advance;
            if(!pressed && defended.trioBattleIntro.actors[5].battleScale[0]>1.3f){next.physicalSnapshot=true;next.physicalButtons=button;next.confirm=false;pressed=true;}
            defended.tick(next);
            for(unsigned player:{1u,2u})if(defended.trioAttack.readWord(0x84100278+0x1000*player,value)) {
                sawGuard|=value==0x50117;sawSuperguard|=value==0x8012a;
            }
            if(defended.trioAttack.readWord(0x84001cb0,value) && value && !defence){
                defence=value;
                if(defence==5)assert(defended.trioAttack.readWord(0x84104280,value) && value==1);
            }
        }
        assert(pressed && defence==(button==0x100?4u:5u));
        uint32_t mario=0,partner=0;assert(defended.trioAttack.readWord(0x8410110c,mario) && defended.trioAttack.readWord(0x8410210c,partner));
        assert((mario>>16)+(partner>>16)==(button==0x100?18u:19u));
        if(button==0x100){assert(defended.trioAttack.pendingFunction()==0 && sawGuard);}
        else {
            assert(defended.trioAttack.readWord(0x8410410c,value) && value>>16==1);
            assert(sawSuperguard);
            assert(defended.trioAttack.readWord(0x84104278,value) && value==0x137);
            assert(defended.trioAttack.readWord(0x841042a8,value) && value==0);
            assert(defended.trioAttack.pendingFunction()==0);
            assert(defended.trioAttack.readWord(0x841042b4,value) && value==0);
            assert(defended.trioAttack.readWord(0x84016f28,value) && value==0x42b40000);
            assert(defended.trioAttack.readWord(0x84016f30,value) && value==0xc2480000);
            assert(std::any_of(defended.trioAttack.requests().begin(),defended.trioAttack.requests().end(),
                [](const ProfessorServiceRequest& r){return r.function==0x8010510c && r.command==0x803516b8;}));
        }
    }
    std::cout<<"Original Jump damage, owned reactions, return/idle, source move consumption, Mario-to-Goombella handoff, persistent Headbonk damage, deferred removal and source enemy impact/recovery, private defence, audience drain and original failed/charged Hammer and wing-fall/grounded attack passed\n";
}

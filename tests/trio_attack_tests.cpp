#include "trio_attack.hpp"
#include "actor_animation.hpp"
#include "battle_actor_rotation.hpp"
#include "game.hpp"
#include "battle_dive.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>
using namespace ttyd;
int main(){
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
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));
    TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.chooseTarget(2);menu.input(0x100,0);
    TrioAttack jump;assert(jump.begin(menu,scene,10));jump.tick(17,scene);
    uint32_t value=0;assert(jump.readWord(0x8410116c,value) && value==20);
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
    const auto& requests=jump.requests();assert(requests.front().function==0x8010d6cc && requests.back().function==0x8010a78c);
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
    for(unsigned i=0;i<1200 && !game.trioAttack.pendingFunction();++i){
        game.tick(Input{});assert(game.trioAttack.readWord(0x84000f18,value));
        if(value!=priorPhase){phaseOrder.push_back(value);priorPhase=value;}
        assert(game.trioAttack.readWord(0x84400400,value));
        if(value&8){assert(game.trioAttack.readWord(0x84413bdc,value));if(audienceOrder.empty() || audienceOrder.back()!=value)audienceOrder.push_back(value);}
    }
    assert(phaseOrder==std::vector<uint32_t>({0x04000002,0x04000003,0x04000004,0x04000005,0x04000006,0x04000000}));
    assert(audienceOrder==std::vector<uint32_t>({0,13,15}));
    assert(game.trioAttack.readWord(0x84400400,value) && !(value&8));
    assert(game.trioAttack.readWord(0x84000f14,value) && value==0x0300004d);
    assert(game.trioAttack.readWord(0x84000000,value) && value>>16==1); // Phase changes do not increment turns.
    assert(game.trioAttack.pendingFunction()==0x8019f298 && game.trioAttack.finished());
    assert(game.trioAttack.readWord(0x84000420,value) && value==5); // Spiked completed; original next enemy.
    assert(game.trioAttack.readWord(0x841042a0,value) && value==0x935238e0); // Installed Spiked root.
    assert(game.trioAttack.readWord(0x841052ac,value) && value==1);
    assert(game.trioAttack.readWord(0x841052a8,value) && value==0);
    assert(game.trioAttack.readWord(0x84000ee0,value) && value==5); // Source sampler's actor ID, not a menu actor.
    assert(game.trioAttack.commandActor()==-1 && game.trioCommands.actor==2 && game.trioCommands.state==TrioBattleCommands::State::Closed);
    const auto phaseChecks=std::count_if(game.trioAttack.requests().begin(),game.trioAttack.requests().end(),[](const ProfessorServiceRequest& r){return r.function==0x80113708;});
    assert(phaseChecks==3 && !game.trioBattleIntro.message.active()); // Frankly gates phase3/4/5 without lecture replay.
    assert(game.trioBattleIntro.actors[5].pose=="TGK_S_1"); // Original type43 band fallback69.
    assert(game.trioAttack.readWord(0x8410629c,value) && value==0); // Finished tracked phase hook cleared by barrier.
    assert(game.trioAttack.readWord(0x8410110c,value) && value>>16==8);
    assert(game.trioAttack.readWord(0x8410210c,value) && value>>16==9);
    assert(game.trioAttack.readWord(0x84101278,value) && value==0x10117);
    assert(game.trioAttack.readWord(0x84001cb0,value) && value==2);
    assert(game.trioAttack.readWord(0x84001d34,value) && value==0); // Private reset independent of general pad.
    assert(game.trioBattleIntro.actors[0].pose=="M_S_1" && game.trioBattleIntro.actors[4].pose=="PTK_S_1");
    for(unsigned id:{4u,5u})assert(game.trioAttack.readWord(0x84100020+id*0x1000,value) && ((value>>24)&255)==10 && ((value>>8)&255)==0);
    assert(game.progress.originalStory==11 && game.fieldBattleState().result==0 && !game.fieldControlEnabled() && !game.saveRequested);
    assert(game.progress.position.x==fieldPosition.x && game.progress.position.z==fieldPosition.z && game.progress.hp==checkpoint.hp);
    for(uint32_t button:{0x100u,0x200u}) {
        Game defended;defended.restore(checkpoint);
        for(unsigned n=0;n<3600 && !defended.trioBattleActive;++n)defended.tick(advance);
        for(unsigned n=0;n<3600 && !defended.trioBattleIntro.finished;++n)defended.tick(advance);
        bool pressed=false;uint32_t defence=0;
        for(unsigned n=0;n<3000 && !defended.trioAttack.pendingFunction();++n){
            Input next;next.physicalSnapshot=true;
            const auto menuState=defended.trioCommands.state;
            if(!defended.trioAttack.active() && (menuState==TrioBattleCommands::State::Command || menuState==TrioBattleCommands::State::Target))next=advance;
            if(!pressed && defended.trioBattleIntro.actors[5].battleScale[0]>1.3f){next.physicalSnapshot=true;next.physicalButtons=button;next.confirm=false;pressed=true;}
            defended.tick(next);
            if(defended.trioAttack.readWord(0x84001cb0,value) && value && !defence)defence=value;
        }
        assert(pressed && defence==(button==0x100?4u:5u));
        assert(defended.trioAttack.readWord(0x8410110c,value) && value>>16==(button==0x100?9u:10u));
        if(button==0x100){assert(defended.trioAttack.pendingFunction()==0x8019f298);assert(defended.trioAttack.readWord(0x84101278,value) && value==0x50117);}
        else {assert(defended.trioAttack.readWord(0x84104280,value) && value==1);assert(defended.trioAttack.readWord(0x8410410c,value) && value>>16==1);assert(defended.trioAttack.readWord(0x84101278,value) && value==0x8012a);assert(defended.trioAttack.readWord(0x84104278,value) && value==0x137);assert(defended.trioAttack.readWord(0x841042a8,value) && value==0);assert(defended.trioAttack.pendingFunction()==0x8010510c);}
    }
    std::cout<<"Original Jump damage, owned reactions, return/idle, source move consumption, Mario-to-Goombella handoff, persistent Headbonk damage, deferred removal and source enemy impact/recovery, private defence and audience drain passed\n";
}

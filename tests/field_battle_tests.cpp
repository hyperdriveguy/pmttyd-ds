#include "field_battle.hpp"
#include "game.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace ttyd;
#define CHECK(x) do {if(!(x)){std::cerr<<__LINE__<<": " #x "\n";std::exit(1);}}while(0)
struct Fixture {
    FieldBattleWork work{99,73};int32_t seq=2;uintptr_t partner=12;
    std::vector<std::string> calls;
    FieldBattleServices services() {
        FieldBattleServices s;s.user=this;
        s.work=[](void* p)->FieldBattleWork& {auto& f=*static_cast<Fixture*>(p);f.calls.push_back("work");return f.work;};
        s.npc=[](void* p,int32_t name)->uintptr_t {auto& f=*static_cast<Fixture*>(p);f.calls.push_back("npc"+std::to_string(name));return 42;};
        s.prepareMario=[](void* p) {auto& f=*static_cast<Fixture*>(p);CHECK(f.work.npc==42 && f.work.result==73);f.calls.push_back("prepare");};
        s.marioControl=[](void* p,bool on) {static_cast<Fixture*>(p)->calls.push_back(on?"marioOn":"marioOff");};
        s.partyId=[](void* p)->int32_t {static_cast<Fixture*>(p)->calls.push_back("partyId");return -7;};
        s.party=[](void* p,int32_t id)->uintptr_t {CHECK(id==-7);auto& f=*static_cast<Fixture*>(p);f.calls.push_back("party");return f.partner;};
        s.partyControl=[](void* p,uintptr_t token,bool on) {CHECK(token==12);static_cast<Fixture*>(p)->calls.push_back(on?"partyOn":"partyOff");};
        s.soundFlag=[](void* p,uint32_t mask) {auto& f=*static_cast<Fixture*>(p);CHECK(mask==0x10 && f.work.result==0);f.calls.push_back("sound");};
        s.setSequence=[](void* p,int32_t seq) {auto& f=*static_cast<Fixture*>(p);CHECK(seq==4);f.calls.push_back("setSequence");f.seq=seq;};
        s.sequence=[](void* p)->int32_t {auto& f=*static_cast<Fixture*>(p);f.calls.push_back("sequence");return f.seq;};
        s.stopBattleBgm=[](void* p) {static_cast<Fixture*>(p)->calls.push_back("stopBgm");};
        s.postMario=[](void* p) {static_cast<Fixture*>(p)->calls.push_back("post");};return s;
    }
};
int main(int argc,char**) {
    // Source control nesting and byte wrapping preserve unrelated flags.
    FieldMarioBattleState mario;mario.flags=0x80401000;mario.secondaryFlags=0x12345679;
    CHECK(fieldMarioControlOff(mario)==1 && mario.flags==0x80401002);
    CHECK(fieldMarioControlOff(mario)==2 && mario.controlDepth==2);
    CHECK(fieldMarioControlOn(mario)==1 && mario.flags==0x80401002);
    CHECK(fieldMarioControlOn(mario)==0 && mario.flags==0x80401000);
    CHECK(fieldMarioControlOn(mario)==-1 && mario.controlDepth==255 && mario.flags==0x80401000);
    CHECK(fieldMarioControlOff(mario)==0 && mario.controlDepth==0 && mario.flags==0x80401002);
    CHECK(fieldMarioControlOffCheck(mario)==0); // Source check reads byte38, not word0 bit2.
    mario.controlDepth=127;CHECK(fieldMarioControlOff(mario)==-128 && mario.flags==0x80401002);
    CHECK(fieldMarioControlOffCheck(mario)==-128);
    CHECK(fieldMarioControlOn(mario)==127 && mario.flags==0x80401002);
    restoreFieldMarioAfterBattle(mario);
    CHECK(mario.secondaryFlags==0x12345678 && mario.flags==0x80401002 && mario.controlDepth==127);
    CHECK((mario.poseHandles==std::array<int32_t,3>{{-1,-1,-1}}));
    mario.flags=0xffffffff;mario.secondaryFlags=0x08000001;CHECK(fieldMarioAnimationId(mario)==0);
    mario.secondaryFlags=0x88000001;CHECK(fieldMarioAnimationId(mario)==1);
    mario.secondaryFlags=0x18000001;CHECK(fieldMarioAnimationId(mario)==2);
    mario.secondaryFlags=0x98000001;CHECK(fieldMarioAnimationId(mario)==2); // Effect wins with rear also set.
    mario.poseHandles={{13,7,29}};restoreFieldMarioAfterBattle(mario);
    CHECK(fieldMarioAnimationId(mario)==2 && mario.poseHandles[unsigned(fieldMarioAnimationId(mario))]==29);
    CHECK(mario.secondaryFlags==0x98000000); // Ending clears only bit0; it does not select a new pose.
    FieldPartyRegistry registry;FieldMarioBattleState partyOwner;
    CHECK(!registry.pointer(-1) && registry.current(partyOwner)==-1);
    CHECK(registry.allocateFirst()==0 && registry.allocateFirst()==1 && registry.allocateFirst()==-1);
    for(unsigned i=0;i<2;++i)for(auto byte:registry.pointer(int32_t(i))->bytes)CHECK(byte==0);
    partyOwner.partyIndices={{1,0}};
    registry.pointer(1)->word(0,0x80000005);registry.pointer(0)->word(0,1);
    CHECK(registry.current(partyOwner)==1);
    registry.pointer(1)->word(0,0x8000000d);CHECK(registry.current(partyOwner)==0);
    registry.pointer(0)->word(0,9);CHECK(registry.current(partyOwner)==-1);
    registry.pointer(0)->word(0,1);registry.pointer(0)->word(8,8);
    CHECK(registry.current(partyOwner)==0); // The pending-mode word8 bit is independent.
    registry.control(false);CHECK(!registry.controlsEnabled());
    CHECK(registry.pointer(0)->word(0)==0x20000001 && registry.pointer(1)->word(0)==0xa000000d);
    registry.control(true);CHECK(registry.controlsEnabled());
    CHECK(registry.pointer(0)->word(0)==1 && registry.pointer(1)->word(0)==0x8000000d);
    FieldPartyRegistry fork=registry;fork.pointer(0)->word(0,9);
    CHECK(registry.pointer(0)->word(0)==1 && fork.current(partyOwner)==-1);
    partyOwner.partyIndices={{255,0}};CHECK(registry.current(partyOwner)==0);
    registry.actors[0].reset();CHECK(registry.current(partyOwner)==-1 && registry.allocateFirst()==0);
    partyOwner.cameraId=7;partyOwner.flags=0x80000002;partyOwner.savedPartyKinds={{0,8}};
    registry.initializeBeforePose(0,1,partyOwner);
    auto* constructed=registry.pointer(0);
    CHECK(constructed->word(0x164)==7 && constructed->word(0x160)==FieldMarioOwnerAddress);
    CHECK(constructed->word(0x0c)==0xffffffff && constructed->word(0x10)==0xffffffff && constructed->word(0x14)==0xffffffff);
    CHECK(constructed->word(0)==1 && constructed->word(4)==0x8000 && constructed->bytes[0x31]==1 && constructed->bytes[0x2f]==0);
    CHECK(registry.currentDefinition==0x80314410 && partyOwner.savedPartyKinds[0]==1 && partyOwner.savedPartyKinds[1]==8);
    CHECK(partyOwner.partyIndices[0]==255 && partyOwner.partyIndices[1]==0); // Entry wrapper publishes only after completion.
    partyOwner.savedPartyKinds[0]=3;registry.initializeBeforePose(1,1,partyOwner);
    CHECK(registry.pointer(1)->word(0)==5 && registry.pointer(1)->bytes[0x2f]==1 && partyOwner.savedPartyKinds[0]==3);
    for(int16_t hp:{int16_t(6),int16_t(5),int16_t(0),int16_t(-1)}) {
        FieldPartyRegistry fresh;FieldMarioBattleState owner;CHECK(fresh.allocateFirst()==0);
        fresh.initializeBeforePose(0,1,owner);auto* actor=fresh.pointer(0);
        actor->bytes[0x20]=9;actor->bytes[0x21]=8;actor->bytes[0x22]=7;actor->word(8,0x40);actor->word(0x1c,123);
        fresh.goombellaInitWork=99;CHECK(fresh.requestFreshGoombellaPose(0,hp));fresh.kuriboInit(0);
        CHECK(actor->word(0x18)==(hp<=5?0x802cb52cu:0x802cb4dcu) && actor->word(8)==0x2040 && actor->word(0x1c)==0);
        CHECK(actor->bytes[0x20]==0 && actor->bytes[0x21]==0 && actor->bytes[0x22]==7);
        CHECK(actor->bytes[0x32]==0 && actor->bytes[0x33]==1 && actor->word(0xf0)==0x41c00000 && actor->word(0xf4)==0x41d00000 && fresh.goombellaInitWork==0);
        CHECK(owner.partyIndices[0]==255 && !fresh.requestFreshGoombellaPose(0,hp)); // Used fresh constructor only; no invented re-entry request.
    }
    Game cameraOwner;CHECK(cameraOwner.fieldMarioBattleState().cameraId==4);
    cameraOwner.setFieldMarioCameraId(0xffffffff);cameraOwner.restore(Progress{});
    CHECK(cameraOwner.fieldMarioBattleState().cameraId==0xffffffff); // Raw ID survives native re-entry, without a range clamp.
    if(argc>1) {
    auto bank=std::make_shared<ActorAnimationPack>();CHECK(bank->load("romfs/partner.pmanim"));
    const auto* lowHp=bank->pose("PKR_D_2");ActorAnimationSample lowHpSample;
    CHECK(lowHp && lowHp->evaluate(lowHp->start,lowHpSample) && !lowHpSample.vertices.empty());
    FieldPartyRegistry animation;
    CHECK(animation.enterPose(0x802cb340,2,bank,17)==-1 && !animation.poses[0]); // No substitute for the original d_mario override.
    CHECK(animation.enterPose(0x804213e0,0,bank,17)==-1 && animation.enterPose(0x804213e0,2,nullptr,17)==-1);
    CHECK(animation.enterPose(0x804213e0,2,bank,17)==0 && animation.enterPose(0x804213e0,2,bank,34)==1);
    for(int32_t slot=2;slot<5;++slot)CHECK(animation.enterPose(0x804213e0,2,bank,51)==slot);
    CHECK(animation.enterPose(0x804213e0,2,bank,51)==-1 && animation.poses[0]->enteredMilliseconds==17);
    CHECK(animation.poses[0]->resource==animation.poses[1]->resource && !animation.poses[0]->shape.positions.empty());
    animation.poses[0]->flags=0x1009;animation.poses[0]->paperOff();CHECK(animation.poses[0]->flags==0x1001);
    FieldPartyRegistry animatedFork=animation;animatedFork.poses[0]->shape.positions[0][0]+=1;
    CHECK(animatedFork.poses[0]->resource==animation.poses[0]->resource && animatedFork.poses[0]->shape.positions[0]!=animation.poses[0]->shape.positions[0]);
    auto marioBank=std::make_shared<ActorAnimationPack>();CHECK(marioBank->load("romfs/characters.pmanim"));
    FieldPartyRegistry shared;FieldMarioBattleState parents;
    CHECK(shared.initializeMario(parents,marioBank,123));
    CHECK((parents.poseHandles==std::array<int32_t,3>{{0,1,2}}));
    for(unsigned i=0;i<3;++i) {
        auto* owner=shared.pose(parents.poseHandles[i]);CHECK(owner && owner->resource==marioBank && !owner->shape.positions.empty());
        CHECK(owner->enteredMilliseconds==123 && owner->shape.vertices.empty());
        owner->materialFlags=0x1800;owner->materialLightFlags=0x40u<<i;
    }
    CHECK(shared.pose(0)->sourceResource==0x802c18a0 && shared.pose(1)->sourceResource==0x802c18a8 && shared.pose(2)->sourceResource==0x802c18b4);
    CHECK(shared.enterPose(0x804213e0,2,bank,234)==3 && shared.enterPose(0x804213e0,2,bank,345)==4);
    shared.reinitializeMario(parents);
    for(unsigned i=0;i<3;++i)CHECK(shared.pose(int32_t(i))->materialFlags==0 && shared.pose(int32_t(i))->materialLightFlags==((0x40u<<i)|2));
    CHECK(shared.pose(3)->materialLightFlags==0 && !shared.pose(-1) && !shared.pose(5));
    CHECK(loadTrioAttackAssets("romfs"));Game persistent;
    auto handles=persistent.fieldMarioBattleState().poseHandles;
    auto* original=persistent.fieldPartyState().pose(handles[1]);CHECK(original && original->resource);
    original->materialFlags=0x1800;original->materialLightFlags=0x40;original->enteredMilliseconds=987;
    persistent.restore(Progress{});
    CHECK(persistent.fieldMarioBattleState().poseHandles==handles && persistent.fieldPartyState().pose(handles[1])==original);
    CHECK(original->enteredMilliseconds==987 && original->materialFlags==0 && original->materialLightFlags==0x42);
    Game independent=persistent;independent.fieldPartyState().pose(handles[1])->materialLightFlags|=0x100;
    CHECK(original->materialLightFlags==0x42 && independent.fieldPartyState().pose(handles[1])->resource==original->resource);
    }
    for(bool partner:{false,true}) {
        Fixture f;if(!partner)f.partner=0;auto s=f.services();
        CHECK(startFieldBattle(-3,true,s)==0);
        std::vector<std::string> expected={"work","npc-3","prepare","marioOff","partyId","party"};
        if(partner)expected.push_back("partyOff");expected.push_back("sound");expected.push_back("setSequence");
        CHECK(f.calls==expected && f.seq==4 && f.work.result==0);
        f.calls.clear();f.work.result=91;f.work.npc=17;
        CHECK(startFieldBattle(8,false,s)==2);
        CHECK((f.calls==std::vector<std::string>{"work","npc8"}));
        CHECK(f.work.result==91 && f.work.npc==17);
        for(int32_t seq:{0,1,3,4}) {f.seq=seq;f.calls.clear();CHECK(waitFieldBattleEnd(s)==0);CHECK((f.calls==std::vector<std::string>{"work","sequence"}));}
        f.seq=2;f.work.result=0;f.calls.clear();CHECK(waitFieldBattleEnd(s)==0);
        CHECK((f.calls==std::vector<std::string>{"work","sequence"}));
        for(int32_t result:{1,-1,73,INT32_MIN}) {
            f.work.result=result;f.calls.clear();CHECK(waitFieldBattleEnd(s)==2);
            expected={"work","sequence","marioOn","partyId","party"};
            if(partner)expected.push_back("partyOn");expected.push_back("stopBgm");expected.push_back("post");
            CHECK(f.calls==expected && f.work.result==result);
            f.calls.clear();CHECK(waitFieldBattleEnd(s)==2);CHECK(f.calls==expected);
        }
    }
    // Native harness owns outcome production separately from the original protocol.
    Game g;g.startBattle();CHECK(!g.fieldControlEnabled() && g.fieldBattleState().result==0);
    g.scene=Scene::Victory;Input confirm;confirm.confirm=true;g.tick(confirm);
    CHECK(g.scene==Scene::Explore && g.fieldControlEnabled() && g.fieldBattleState().result==1);
    g.startBattle();CHECK(!g.fieldControlEnabled() && g.fieldBattleState().result==0);
    g.scene=Scene::Defeat;Input cancel;cancel.cancel=true;g.tick(cancel);
    CHECK(g.scene==Scene::Explore && g.fieldControlEnabled() && g.fieldBattleState().result==-1);
    g.startBattle();g.scene=Scene::Defeat;g.tick(confirm);CHECK(!g.fieldControlEnabled() && g.scene==Scene::Battle);
    CHECK(g.fieldMarioBattleState().controlDepth==1);
    g.scene=Scene::Defeat;g.tick(cancel);CHECK(g.fieldControlEnabled() && g.fieldMarioBattleState().controlDepth==0);
    std::cout<<"Field battle protocol passed\n";
}

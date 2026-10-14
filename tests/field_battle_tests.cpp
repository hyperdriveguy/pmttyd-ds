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
int main() {
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

#include "battle_hammer.hpp"
#include <cstdio>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
using namespace ttyd;
#define CHECK(x) do { if(!(x)) throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x); } while(0)
struct SoundLog {
    std::vector<std::string> keys;
    static int32_t play(void* user,const char* key) {
        auto& log=*static_cast<SoundLog*>(user);log.keys.emplace_back(key);
        return int32_t(log.keys.size()+100);
    }
    HammerSoundSink sink() {return {this,play};}
};
HammerContext left(int difficulty=3) {HammerContext input;input.held=BattleHeldLeft;input.difficulty=difficulty;return input;}
void start(HammerCommand& command,BattleRandom& random) {
    CHECK(command.tick({},random)==1 && command.state==99 && command.pending());
    command.state=100;
    CHECK(command.tick(left(),random)==1 && command.state==1001);
    CHECK(command.stage==1 && command.countdown==1 && command.enteredCharge);
}
void reachFinal(HammerCommand& command,BattleRandom& random,HammerSoundSink sink={}) {
    int calls=0;
    const int expected=command.stage<command.chargeSteps+1 ? command.countdown+1+
        (command.chargeSteps-command.stage)*(command.period+1) : 0;
    while(command.stage<command.chargeSteps+1) {
        CHECK(command.tick(left(),random,sink)==1 && command.pending());
        CHECK(++calls<1000);
    }
    CHECK(calls==expected);
    CHECK(command.countdown==command.period && command.stepCount==command.chargeSteps);
}
void releaseBoundaries() {
    constexpr int allowances[]={80,70,60,40,20,10,5};
    for(int period:{25,30,40,90})for(int difficulty=0;difficulty<7;++difficulty) {
        for(int elapsed:{1,allowances[difficulty],allowances[difficulty]+1,period,period+1}) {
            if(elapsed>period+1)continue;
            BattleRandom random(1);HammerCommand command;command.period=period;
            start(command,random);reachFinal(command,random);
            for(int n=1;n<elapsed;++n)CHECK(command.tick(left(difficulty),random)==1);
            HammerContext release;release.difficulty=difficulty;
            CHECK(command.tick(release,random)==1 && !command.pending() && command.state==1004);
            const bool success=elapsed<=period && elapsed<=allowances[difficulty];
            CHECK(command.result==(success?2u:0u));
            CHECK(command.classification==(success?3:elapsed>period?2:1));
            CHECK(command.count==0 && random.state==1);
        }
    }
    BattleRandom random(1);HammerCommand command;
    start(command,random);
    CHECK(command.tick({},random)==1 && command.state==1004 && command.result==0);
    CHECK(command.classification==1 && command.countdown==0); // Early release at stage1.
    // Release on a stage-increment call is deferred until the following pass.
    command={};start(command,random);reachFinal(command,random);
    command.stage=4;command.countdown=0;command.finalSound=false;
    CHECK(command.tick({},random)==1 && command.state==1001 && command.pending());
    CHECK(command.stage==5 && command.countdown==25);
    CHECK(command.tick({},random)==1 && command.state==1004 && command.result==2);
}
void soundsAndWaits() {
    BattleRandom random(1);HammerCommand command;SoundLog log;
    command.suppression=7;command.mode=0;command.period=25;
    start(command,random);CHECK(command.suppression==0 && command.suppressionLatch==0);
    reachFinal(command,random,log.sink());
    CHECK(log.keys.size()==4);
    for(int n=0;n<3;++n)CHECK(log.keys[n]=="SFX_AC_PI1" && command.handles[n]==101+n);
    CHECK(log.keys[3]=="SFX_AC_PONE1" && command.handles[3]==104 && command.finalSound);
    for(int n=4;n<12;++n)CHECK(command.handles[n]==-1);
    CHECK(std::strcmp(command.soundRequest,"SFX_AC_PONE1")==0);
    command.suppression=1;const auto before=command.countdown;
    CHECK(command.tick({},random,log.sink())==1 && command.countdown==before && command.pending());
    CHECK(command.suppressionLatch==1 && command.soundRequest==nullptr);
    command.suppression=0;
    CHECK(command.tick({},random,log.sink())==1 && command.result==2 && command.state==1004);
    for(int n=0;n<5;++n)CHECK(command.tick({},random,log.sink())==1 && command.state==1004);
    CHECK(log.keys.size()==4); // Finish busy loop has no sound-stop request.
    command.stop();CHECK(command.state==1005);
    CHECK(command.tick({},random)==1 && command.state==1006 && command.finishCountdown==60);
    for(int n=0;n<59;++n)CHECK(command.tick({},random)==1 && command.state==1006);
    CHECK(command.tick({},random)==1 && command.state==1007 && command.callbacksLive);
    CHECK(command.tick({},random)==1 && !command.callbacksLive);
    command={};CHECK(command.tick({},random)==1);command.state=100;
    CHECK(command.tick({},random)==1 && command.state==1000 && command.countdown==119);
    for(int n=0;n<118;++n)CHECK(command.tick({},random)==1 && command.state==1000);
    CHECK(command.tick({},random)==1 && command.state==1004 && command.result==0);
    CHECK(command.classification==0 && command.stage==0);
}
void specialRoutes() {
    BattleRandom random(1);HammerCommand command;start(command,random);
    HammerContext token=left();token.setupOptions=3;token.actorFlags=0x10;
    command.state=100;command.specialDelay=1;
    CHECK(command.tick(token,random)==1 && command.specialDelay==0 && command.countdown==120);
    CHECK(random.state==1);
    CHECK(command.tick(token,random)==0 && command.result==0 && command.count==0);
    CHECK(command.specialDelay==-1 && random.state==0x41c67ea6 && command.state==1000);
    CHECK(command.callbacksLive); // Main returned0; manager owns clearing main.
    command={};start(command,random);command.forcedCounter=2;
    CHECK(command.tick(left(),random)==1 && command.forcedCounter==1);
    CHECK(command.result==2 && command.state==1004 && command.countdown==0 && command.stage==1);
    command={};start(command,random);command.forcedCounter=1;command.countdown=0;
    SoundLog log;
    CHECK(command.tick(left(),random,log.sink())==1 && command.forcedCounter==0);
    CHECK(command.state==1004 && command.result==2 && command.stage==2 && command.countdown==25);
    CHECK(log.keys.size()==1); // Forced-result route still executes stage/sound logic.
    command={};start(command,random);command.mode=2;command.classification=9;
    CHECK(command.tick({},random)==1 && command.state==1001 && command.classification==9);
    reachFinal(command,random);
    for(int n=0;n<26;++n)CHECK(command.tick(left(),random)==1);
    CHECK(command.state==1004 && command.result==2 && command.classification==2);
    CHECK(command.count==0); // Timeout success does not increment result count.
    command={};CHECK(command.tick({},random)==1);command.state=100;
    HammerContext automatic;automatic.automatic=true;
    CHECK(command.tick(automatic,random)==1 && command.state==1001);
    for(int n=0;n<80;++n)CHECK(command.tick(automatic,random)==1 && command.pending());
    CHECK(command.stage==5 && command.countdown==25);
    CHECK(command.tick(automatic,random)==1 && command.result==2 && command.classification==3);
}
void parameters() {
    CHECK(ordinaryHammerPeriod(0x80353de8)==25);
    CHECK(ordinaryHammerPeriod(0x80354da8)==30 && ordinaryHammerPeriod(0x80354ce8)==30);
    CHECK(ordinaryHammerPeriod(0x12345678)==40);
    CHECK(attackDifficulty(3,0,0)==3 && attackDifficulty(3,4,0)==0);
    CHECK(attackDifficulty(3,0,4)==6 && attackDifficulty(3,2,1)==2);
    CHECK(attackDifficulty(255,0,0)==6); // Original unsigned weapon byte.
    CHECK(attackDifficulty(std::numeric_limits<int32_t>::max(),0,1)==0); // Word wrap.
    BattleRandom random(1);HammerCommand command;command.chargeSteps=13;
    CHECK(command.tick({},random)==-1 && command.state==0 && random.state==1);
    command.chargeSteps=4;HammerContext invalid;invalid.difficulty=7;
    CHECK(command.tick(invalid,random)==-1 && command.state==0 && random.state==1);
    command.period=-1;CHECK(command.tick({},random)==-1 && command.state==0);
    command.period=0;start(command,random);reachFinal(command,random);
    CHECK(command.tick({},random)==1 && command.state==1004 && command.classification==2);
}
int main() {
    try {releaseBoundaries();soundsAndWaits();specialRoutes();parameters();std::puts("Hammer contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}

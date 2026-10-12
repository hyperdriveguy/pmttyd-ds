#include "battle_timing_a.hpp"
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>
using namespace ttyd;
#define CHECK(x) do { if(!(x)) throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x); } while(0)

void historyAndResults() {
    BattleTriggerHistory history;
    for(uint32_t n=1;n<=31;++n)history.push(n);
    CHECK(history.entries[0]==31 && history.entries[29]==2);
    TimingAContext context;
    BattleRandom random(1);
    constexpr int firstWindows[]={10,10,9,8,7,6,5};
    for(int difficulty=0;difficulty<7;++difficulty) {
        context.difficulty=difficulty;
        for(int sample=0;sample<30;++sample) {
            history={};history.entries[sample]=BattleButtonA;
            TimingACommand command;
            CHECK(command.evaluate(history,context,random)==(sample<firstWindows[difficulty]?2u:0u));
            CHECK(command.count==(sample<firstWindows[difficulty]?1:0));
            CHECK(command.classification==(sample<firstWindows[difficulty]?3:sample<20?1:2));
            CHECK(history.entries[sample]==BattleButtonA); // Evaluation does not consume.
        }
    }
    context.difficulty=3;
    // Any second edge in samples0..19 defeats the otherwise valid newest edge.
    for(int second=1;second<30;++second) {
        history={};history.entries[0]=BattleButtonA;history.entries[second]=BattleButtonA;
        TimingACommand command;
        CHECK(command.evaluate(history,context,random)==(second<20?0u:2u));
        CHECK(command.classification==(second<20?1:3));
    }
    history={};history.entries[0]=0x200; // B is not A and cannot succeed.
    TimingACommand command;
    CHECK(command.evaluate(history,context,random)==0 && command.classification==2);
    history.entries[0]|=BattleButtonA;
    CHECK(command.evaluate(history,context,random)==2 && command.count==1);
    CHECK(command.evaluate(history,context,random)==2 && command.count==2);
    CHECK(random.state==1); // Ordinary/empty/mash routes consume no random draws.
}
void windowsAndCutoff() {
    TimingACommand command;
    command.count=100000;
    CHECK(command.successWindow(3)==8); // Count has no effect with bit1 clear.
    command.flags=2;command.count=-1;CHECK(command.successWindow(0)==10);
    command.count=4;CHECK(command.successWindow(0)==10);
    command.count=5;CHECK(command.successWindow(0)==9);
    command.count=6;CHECK(command.successWindow(0)==8);
    command.count=7;CHECK(command.successWindow(3)==1); // Missing threshold7: use8.
    CHECK(command.successWindow(1)==4);CHECK(command.successWindow(2)==2);
    command.count=6;CHECK(command.successWindow(4)==1);CHECK(command.successWindow(5)==1);
    CHECK(command.successWindow(6)==1);
    command.count=99999;
    for(int d=0;d<7;++d)CHECK(command.successWindow(d)==1);
    command.count=100000;
    for(int d=0;d<7;++d)CHECK(command.successWindow(d)==0);
    BattleTriggerHistory history;history.entries[0]=BattleButtonA;
    TimingAContext context;BattleRandom random(1);
    CHECK(command.evaluate(history,context,random)==0 && command.classification==1);
    CHECK(command.count==100000);
    command.result=7;command.classification=8;
    context.difficulty=-1;
    CHECK(command.evaluate(history,context,random)==7);
    context.difficulty=7;CHECK(command.evaluate(history,context,random)==7);
    CHECK(command.count==100000 && command.classification==8 && random.state==1);
}
void overridesAndRandomness() {
    BattleRandom random(1);
    CHECK(random.bounded(0)==0 && random.state==1);
    CHECK(random.bounded(10)==8 && random.state==0x41c67ea6);
    CHECK(random.next()==5758 && random.state==0x967eb0e7);
    CHECK(random.next()==10113 && random.state==0x2781e494);
    CHECK(random.next()==17515 && random.state==0xc46b9b3d);
    CHECK(random.next()==31051 && random.state==0xf94bdf32);
    random.state=1;CHECK(random.bounded(-10)==8 && random.state==0x41c67ea6);
    BattleTriggerHistory history;
    TimingAContext context;context.setupOptions=1;context.actorFlags=0x10;
    context.automatic=1;context.battleFlags=0x01000000;
    TimingACommand command;command.classification=9;command.count=4;
    random.state=1;
    CHECK(command.evaluate(history,context,random)==0); // RNG route takes precedence.
    CHECK(command.classification==9 && command.count==4 && random.state==0x41c67ea6);
    context.actorFlags=0;
    CHECK(command.evaluate(history,context,random)==2); // Auto preserves classification.
    CHECK(command.classification==9 && command.count==5 && random.state==0x41c67ea6);
    context.automatic=0;
    CHECK(command.evaluate(history,context,random)==2 && command.classification==3);
    CHECK(command.count==6 && random.state==0x41c67ea6);
    context.battleFlags=0;command.count=std::numeric_limits<int32_t>::max();
    history.entries[0]=BattleButtonA;
    CHECK(command.evaluate(history,context,random)==2);
    CHECK(command.count==std::numeric_limits<int32_t>::min());
}
void lifecycle() {
    TimingACommand command;
    command.flags=7;command.result=5;command.count=9;command.classification=3;
    command.scratch.fill(0xffffffff);
    CHECK(command.tick()==1 && command.state==99);
    CHECK(command.flags==6 && command.result==5 && command.count==9 && command.classification==0);
    for(size_t n=0;n<command.scratch.size();++n)
        CHECK(command.scratch[n]==(n==5?0xc3960000u:n==8?20u:0u));
    CHECK(command.tick()==1 && command.state==99 && command.callbacksLive);
    command.state=100;CHECK(command.tick()==1 && command.state==100);
    command.state=1000;
    CHECK(command.tick()==1 && command.state==1001 && command.finishCountdown==59);
    for(int n=0;n<58;++n)CHECK(command.tick()==1 && command.state==1001);
    CHECK(command.finishCountdown==1 && command.callbacksLive);
    CHECK(command.tick()==1 && command.state==1002 && command.callbacksLive);
    CHECK(command.tick()==0 && !command.callbacksLive);
}
int main() {
    try {
        historyAndResults();windowsAndCutoff();overridesAndRandomness();lifecycle();
        std::puts("TimingA contract scenarios passed");
    } catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}

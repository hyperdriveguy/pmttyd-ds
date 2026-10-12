#include "battle_defense.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x);} while(0)

void checkReset(const BattlePadWork& pad) {
    CHECK(pad.current.buttons==0 && pad.previous.buttons==0 && pad.current.mainX==0);
    CHECK(pad.repeatPhase==0 && pad.repeatCounter==0);
    for(size_t n=0;n<30;++n)CHECK(pad.triggers.entries[n]==0 && pad.repeats.entries[n]==0 && pad.held.entries[n]==0);
}
void padSamples() {
    BattlePadWork pad;BattlePadSample input;
    input.snapshot.buttons=BattleButtonA;input.pressed=BattleButtonB;
    input.repeat=BattleButtonA;input.released=0x400;input.snapshot.analogA=255;input.snapshot.analogB=123;
    pad.update(input);
    CHECK(pad.held.entries[0]==BattleButtonA && pad.triggers.entries[0]==BattleButtonB);
    CHECK(pad.current.analogA==0 && pad.current.analogB==0); // No locally manufactured A edge.
    CHECK(pad.repeats.entries[0]==BattleButtonA && pad.released[0]==0x400);
    input.pressed=0;pad.update(input);
    CHECK(pad.triggers.entries[0]==0 && pad.triggers.entries[1]==BattleButtonB);
    CHECK(pad.previous.buttons==BattleButtonA);
    for(uint32_t n=1;n<=31;++n) {input.pressed=n;input.repeat=n+100;pad.update(input);}
    CHECK(pad.triggers.entries[0]==31 && pad.triggers.entries[29]==2);
    CHECK(pad.repeats.entries[0]==131 && pad.repeats.entries[29]==102);
    pad.released.fill(0x12345678);pad.init();checkReset(pad);
    for(auto release:pad.released)CHECK(release==0x12345678);
    input={};pad.update(input);CHECK(pad.released[0]==0 && pad.released[29]==0x12345678);
}
void virtualEdges() {
    struct Axis {int8_t BattlePadSnapshot::* field;int sign;uint32_t mask;};
    const Axis axes[]={
        {&BattlePadSnapshot::mainX,-1,0x40000},{&BattlePadSnapshot::mainX,1,0x80000},
        {&BattlePadSnapshot::mainY,-1,0x20000},{&BattlePadSnapshot::mainY,1,0x10000},
        {&BattlePadSnapshot::subX,-1,0x400000},{&BattlePadSnapshot::subX,1,0x800000},
        {&BattlePadSnapshot::subY,-1,0x200000},{&BattlePadSnapshot::subY,1,0x100000}};
    for(auto axis:axes) {
        BattlePadWork pad;BattlePadSample input;
        input.snapshot.*axis.field=int8_t(29*axis.sign);pad.update(input);
        CHECK(pad.held.entries[0]==0 && pad.triggers.entries[0]==0);
        input.snapshot.*axis.field=int8_t(30*axis.sign);pad.update(input);
        CHECK(pad.held.entries[0]==axis.mask && pad.triggers.entries[0]==axis.mask);
        CHECK(pad.repeats.entries[0]==axis.mask);
        input.snapshot.*axis.field=int8_t(70*axis.sign);pad.update(input);
        CHECK(pad.triggers.entries[0]==0 && pad.repeats.entries[0]==0);
        input.snapshot.*axis.field=0;pad.update(input);
        CHECK(pad.released[0]==axis.mask && pad.held.entries[0]==0);
        CHECK(pad.repeatPhase==0 && pad.repeatCounter==0);
    }
    BattlePadWork pad;BattlePadSample input;input.snapshot.mainX=-30;pad.update(input);
    input.snapshot.mainX=30;pad.update(input);
    CHECK(pad.triggers.entries[0]==0x80000 && pad.released[0]==0x40000);
}
void repeatCadence() {
    BattlePadWork pad;BattlePadSample input;input.snapshot.mainX=-30;
    pad.update(input);CHECK(pad.repeatPhase==1 && pad.repeatCounter==0);
    for(int n=1;n<16;++n) {pad.update(input);CHECK(pad.repeats.entries[0]==0 && pad.repeatCounter==n);}
    pad.update(input);CHECK(pad.repeats.entries[0]==0x40000 && pad.repeatPhase==2 && pad.repeatCounter==0);
    for(int n=1;n<8;++n) {pad.update(input);CHECK(pad.repeats.entries[0]==0);}
    pad.update(input);CHECK(pad.repeats.entries[0]==0x40000 && pad.repeatCounter==0);
    input.snapshot.buttons=BattleButtonA;input.pressed=input.repeat=BattleButtonA;
    pad.update(input);CHECK(pad.repeatPhase==1 && pad.repeatCounter==0);
    CHECK(pad.repeats.entries[0]==BattleButtonA); // Physical change restarts delay; stick is no new edge.
    input.pressed=0;
    for(int n=1;n<16;++n) {pad.update(input);CHECK(pad.repeats.entries[0]==BattleButtonA);}
    pad.update(input);CHECK(pad.repeats.entries[0]==(BattleButtonA|0x40000));
    input.snapshot={};pad.update(input);CHECK(pad.repeats.entries[0]==0);
    CHECK(pad.repeatPhase==0 && pad.repeatCounter==0);
}
void singlePresses() {
    constexpr int a[]={10,9,8,8,7,6,5},b[]={5,4,3,3,2,1,1};
    for(int difficulty=0;difficulty<7;++difficulty)for(bool eligible:{false,true}) {
        DefenseContext context;context.weaponGate=eligible?1:0;
        context.simplifiers=difficulty<3?uint8_t(3-difficulty):0;
        context.unsimplifiers=difficulty>3?uint8_t(difficulty-3):0;
        for(int sample=0;sample<30;++sample)for(uint32_t mask:{BattleButtonA,BattleButtonB}) {
            BattlePadWork pad;pad.triggers.entries[sample]=mask;pad.released[0]=0x3456;
            DefenseCommand command;
            const int expected=mask==BattleButtonA && sample<a[difficulty]?4:
                mask==BattleButtonB && eligible && sample<b[difficulty]?5:sample<15?1:2;
            CHECK(command.evaluate(pad,context)==expected && command.result==expected);
            checkReset(pad);CHECK(pad.released[0]==0x3456);
        }
    }
    DefenseContext context;context.weaponGate=255;context.hp=0;context.automatic=1;
    BattlePadWork pad;pad.triggers.entries[0]=BattleButtonB;DefenseCommand command;
    CHECK(command.evaluate(pad,context)==1); // HP gate beats automatic.
    context.hp=-1;pad.triggers.entries[0]=BattleButtonB;CHECK(command.evaluate(pad,context)==1);
    context.hp=10;context.simplifiers=255;context.automatic=0;
    pad.triggers.entries[9]=BattleButtonA;CHECK(command.evaluate(pad,context)==4);
    context.simplifiers=0;context.unsimplifiers=255;
    pad.triggers.entries[5]=BattleButtonA;CHECK(command.evaluate(pad,context)==1);
}
void mashAndOverrides() {
    for(bool eligible:{false,true})for(int second=0;second<30;++second) {
        BattlePadWork pad;DefenseCommand command;DefenseContext context;context.weaponGate=eligible;
        pad.triggers.entries[0]=BattleButtonA;
        pad.triggers.entries[second]|=BattleButtonB;
        CHECK(command.evaluate(pad,context)==(second<15?1:4));checkReset(pad);
    }
    for(bool eligible:{false,true})for(bool automatic:{false,true})for(int hp:{0,10}) {
        BattlePadWork pad;pad.triggers.entries[0]=BattleButtonA|BattleButtonB;
        DefenseCommand command;command.overrideCounter=2;DefenseContext context;
        context.weaponGate=eligible;context.automatic=automatic;context.hp=hp;
        CHECK(command.evaluate(pad,context)==(eligible && hp>0?5:4));
        CHECK(command.overrideCounter==1);checkReset(pad);
    }
    BattlePadWork general,privatePad;general.triggers.entries[0]=BattleButtonA;
    privatePad.triggers.entries[0]=BattleButtonB;DefenseContext context;context.weaponGate=1;
    DefenseCommand command;command.overrideCounter=-1;
    CHECK(command.evaluate(privatePad,context)==5 && command.overrideCounter==-1);
    CHECK(general.triggers.entries[0]==BattleButtonA); // No general/private aliasing.
    CHECK(command.evaluate(privatePad,context)==2); // No reused edge on the next hit.
    context.automatic=1;privatePad.triggers.entries[0]=BattleButtonA|BattleButtonB;
    CHECK(command.evaluate(privatePad,context)==5);checkReset(privatePad);
    context.weaponGate=0;CHECK(command.evaluate(privatePad,context)==2); // Auto cannot force normal guard.
}
int main() {
    try {padSamples();virtualEdges();repeatCadence();singlePresses();mashAndOverrides();std::puts("Defense and pad contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}

#include "physical_input.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>
using namespace ttyd;
#define CHECK(x) do {if(!(x))throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x);} while(0)
void rawCopies() {
    struct Axis {int8_t RawPad::* member;uint16_t negative,positive;};
    for(auto axis:{Axis{&RawPad::mainX,0x4000,0x8000},Axis{&RawPad::mainY,0x2000,0x1000},
        Axis{&RawPad::subX,0x400,0x800},Axis{&RawPad::subY,0x200,0x100}}) {
        DemoPad d;RawPad raw;
        for(int value:{-49,-48,48,49}) {
            raw.*axis.member=int8_t(value);d.copy(raw);
            CHECK(d.direction==(value<-48?axis.negative:value>48?axis.positive:0));
        }
    }
    DemoPad d;RawPad raw;raw.buttons=0x100;raw.mainX=-128;raw.leftTrigger=123;raw.padding=77;
    d.copy(raw);CHECK(d.pressed==0x100 && d.delta[0]==-128);
    RawPad retry;retry.error=-3;retry.buttons=0;retry.mainX=127;
    d.copy(retry);CHECK(d.raw.buttons==0x100 && d.raw.mainX==-128 && d.raw.padding==77);
    CHECK(d.direction==0x4000 && d.pressed==0 && d.released==0 && d.delta[0]==0);
    raw.buttons=0;raw.mainX=127;raw.error=-2;d.copy(raw);
    CHECK(d.released==0x100 && d.delta[0]==255 && d.raw.error==-2 && d.direction==0x8000);
}
void globalRepeats() {
    for(int rate:{60,50,30}) {
        PhysicalInputProducer p;std::array<RawPad,4> raw{};raw[0].buttons=0x100;
        const int first=rate*24/60,next=rate*6/60;
        p.update(raw,rate,false);CHECK(p.channels[0].pressed==0x100 && p.channels[0].repeat==0x100);
        for(int n=1;n<first;++n) {p.update(raw,rate,false);CHECK(p.channels[0].pressed==0 && p.channels[0].repeat==0);}
        p.update(raw,rate,false);CHECK(p.channels[0].repeat==0x100);
        for(int n=1;n<next;++n) {p.update(raw,rate,false);CHECK(p.channels[0].repeat==0);}
        p.update(raw,rate,false);CHECK(p.channels[0].repeat==0x100);
        raw[0].buttons=0x300;p.update(raw,rate,false);CHECK(p.channels[0].pressed==0x200 && p.channels[0].repeat==0x200);
        raw[0].buttons=0x100;p.update(raw,rate,false);CHECK(p.channels[0].pressed==0 && p.channels[0].released==0x200);
        CHECK(p.channels[0].buttonCountdown==first);
    }
    PhysicalInputProducer p;std::array<RawPad,4> raw{};raw[0].buttons=0x100;
    p.update(raw,0,false);p.update(raw,0,false);CHECK(p.channels[0].repeat==0 && p.channels[0].buttonCountdown==-1);
    p={};p.update(raw,60,false);raw[0].error=-3;raw[0].buttons=0;
    for(int n=1;n<=24;++n)p.update(raw,60,false);
    CHECK(p.channels[0].buttons==0x100 && p.channels[0].repeat==0x100 && p.demo[0].pressed==0);
}
void chordHardwareAndSnapshots() {
    PhysicalInputProducer p;std::array<RawPad,4> raw{};
    raw[0].buttons=0x1d00;raw[0].mainX=70;raw[0].leftTrigger=99;
    raw[1].error=-1;raw[2].error=-3;raw[3].error=-2;
    struct Observer {
        PhysicalInputProducer* p;bool resetBeforePublish=false;int motors=0;
        static void reset(void* user,uint32_t mask) {
            auto& o=*static_cast<Observer*>(user);CHECK(mask==0x40000000);
            o.resetBeforePublish=o.p->channels[0].buttons==0 && o.p->demo[0].raw.buttons==0x1d00;
        }
        static void motor(void* user,int channel,int mode) {auto& o=*static_cast<Observer*>(user);CHECK(channel==0 && mode==2);++o.motors;}
    } observer{&p};
    p.channels[0].forceMotor=1;p.channels[0].motorMode=1;
    auto requests=p.update(raw,60,true,{&observer,Observer::reset,Observer::motor});
    CHECK(requests.resetMask==0x40000000 && p.countedChannels==2 && observer.resetBeforePublish);
    CHECK(observer.motors==1 && p.channels[0].previousMotorMode==0 && p.ready==1);
    CHECK(p.channels[0].buttons==0 && p.channels[0].direction==0 && p.channels[0].analog.mainX==0);
    CHECK(p.channels[0].analog.leftTrigger==0 && p.channels[0].buttonCountdown==24 && p.demo[0].raw.buttons==0x1d00);
    p.update(raw,60,true,{&observer,nullptr,Observer::motor});CHECK(observer.motors==2);
    p.channels[0].forceMotor=0;p.channels[0].motorMode=99;p.update(raw,60,true);
    CHECK(p.channels[0].previousMotorMode==99);
    p.channels[0].motorMode=2;p.update(raw,60,false);CHECK(p.channels[0].previousMotorMode==99);
    requests=p.update(raw,60,true);CHECK(requests.motors[0]==2 && p.channels[0].previousMotorMode==2);
    BattlePadWork pad;pad.current.retainedA=55;pad.current.retainedB=66;pad.previous.retainedB=77;
    BattlePadSample input;input.snapshot.leftTrigger=17;input.snapshot.rightTrigger=18;
    input.snapshot.analogA=99;input.snapshot.retainedA=12;input.snapshot.retainedB=13;
    pad.update(input);CHECK(pad.current.leftTrigger==17 && pad.current.rightTrigger==18 && pad.current.analogA==0);
    CHECK(pad.current.retainedA==55 && pad.current.retainedB==66 && pad.previous.retainedB==66);
    pad.init();CHECK(pad.current.retainedA==0 && pad.current.retainedB==66 && pad.previous.retainedB==66);
}
int main() {
    try {rawCopies();globalRepeats();chordHardwareAndSnapshots();std::puts("Physical input contract scenarios passed");}
    catch(const std::exception& e) {std::fprintf(stderr,"%s\n",e.what());return 1;}
}

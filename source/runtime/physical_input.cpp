#include "physical_input.hpp"
namespace ttyd {
namespace {
int32_t word(uint32_t x) {return x<=0x7fffffff?int32_t(x):int32_t(int64_t(x)-0x100000000LL);}
int32_t delay(int32_t rate,uint32_t multiplier) {return word(uint32_t(rate)*multiplier)/60;}
void publish(uint16_t mask,uint32_t& held,uint32_t& pressed,uint32_t& repeat,int32_t& countdown,int32_t longDelay,int32_t shortDelay) {
    pressed=mask&(mask^held);repeat=pressed;
    if(mask && mask==held) {
        countdown=word(uint32_t(countdown)-1);
        if(countdown==0) {repeat=mask;countdown=shortDelay;}
    } else countdown=longDelay;
    held=mask;
}
}
void DemoPad::copy(const RawPad& input) {
    if(input.error==-3) {pressed=released=directionPressed=directionReleased=0;delta={};return;}
    uint16_t next=0;
    if(input.mainX<-48)next|=0x4000;
    if(input.mainX>48)next|=0x8000;
    if(input.mainY<-48)next|=0x2000;
    if(input.mainY>48)next|=0x1000;
    if(input.subX<-48)next|=0x400;
    if(input.subX>48)next|=0x800;
    if(input.subY<-48)next|=0x200;
    if(input.subY>48)next|=0x100;
    directionPressed=next&~direction;directionReleased=direction&~next;direction=next;
    pressed=input.buttons&~raw.buttons;released=raw.buttons&~input.buttons;
    delta={int16_t(int(input.mainX)-raw.mainX),int16_t(int(input.mainY)-raw.mainY),
        int16_t(int(input.subX)-raw.subX),int16_t(int(input.subY)-raw.subY)};
    raw=input;
}
BattlePadSample GlobalInputChannel::battleSample() const {
    BattlePadSample sample;sample.snapshot=analog;sample.snapshot.buttons=uint16_t(buttons);
    sample.pressed=pressed;sample.repeat=repeat;sample.released=released;return sample;
}
InputHardwareRequests PhysicalInputProducer::update(const std::array<RawPad,4>& raw,int32_t frameRate,bool motorsEnabled,InputHardwareServices hardware) {
    InputHardwareRequests requests;countedChannels=0;
    for(int i=0;i<4;++i) {
        if(raw[i].error==0 || raw[i].error==-3)++countedChannels;
        else if(raw[i].error==-1)requests.resetMask|=0x80000000u>>i;
        demo[i].copy(raw[i]);
    }
    if(requests.resetMask && hardware.reset)hardware.reset(hardware.user,requests.resetMask);
    const int32_t longDelay=delay(frameRate,24),shortDelay=delay(frameRate,6);
    for(int i=0;i<4;++i) {
        auto& c=channels[i];publish(demo[i].direction,c.direction,c.directionPressed,c.directionRepeat,c.directionCountdown,longDelay,shortDelay);
    }
    for(int i=0;i<4;++i) {
        auto& c=channels[i];publish(demo[i].raw.buttons,c.buttons,c.pressed,c.repeat,c.buttonCountdown,longDelay,shortDelay);
        c.released=demo[i].released;
    }
    for(int i=0;i<4;++i) {
        auto& a=channels[i].analog;const auto& r=demo[i].raw;
        a.mainX=r.mainX;a.mainY=r.mainY;a.subX=r.subX;a.subY=r.subY;
        a.leftTrigger=r.leftTrigger;a.rightTrigger=r.rightTrigger;
    }
    for(auto& c:channels)if((c.buttons&0x1c00)==0x1c00) {
        c.buttons=c.pressed=c.repeat=c.released=c.direction=c.directionPressed=c.directionRepeat=0;
        c.buttonCountdown=c.directionCountdown=longDelay;
        c.analog.mainX=c.analog.mainY=c.analog.subX=c.analog.subY=0;
        c.analog.leftTrigger=c.analog.rightTrigger=0;
    }
    if(motorsEnabled)for(int i=0;i<4;++i) {
        auto& c=channels[i];
        if(c.forceMotor) {
            requests.motors[i]=2;
            if(hardware.motor)hardware.motor(hardware.user,i,2);
        }
        else if(c.motorMode!=c.previousMotorMode) {
            if(c.motorMode<=2) {
                requests.motors[i]=c.motorMode;
                if(hardware.motor)hardware.motor(hardware.user,i,c.motorMode);
            }
            c.previousMotorMode=c.motorMode;
        }
    }
    ready=1;return requests;
}
}

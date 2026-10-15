#include "physical_input_gp.hpp"
#include <cstring>
namespace ttyd {
namespace {
uint32_t readWord(const PhysicalInputProducer& p,uint32_t offset){
    if(offset==0x1324)return p.ready;
    const auto& c=p.channels[((offset-0x1328)&15)/4];
    switch((offset-0x1328)/16){
    case 0:return c.buttons;case 1:return c.pressed;case 2:return c.repeat;
    case 3:return uint32_t(c.buttonCountdown);case 4:return c.released;
    case 5:return c.direction;case 6:return c.directionPressed;case 7:return c.directionRepeat;
    default:return uint32_t(c.directionCountdown);
    }
}
void writeWord(PhysicalInputProducer& p,uint32_t offset,uint32_t value){
    if(offset==0x1324){p.ready=value;return;}
    auto& c=p.channels[((offset-0x1328)&15)/4];int32_t signedValue;std::memcpy(&signedValue,&value,4);
    switch((offset-0x1328)/16){
    case 0:c.buttons=value;break;case 1:c.pressed=value;break;case 2:c.repeat=value;break;
    case 3:c.buttonCountdown=signedValue;break;case 4:c.released=value;break;
    case 5:c.direction=value;break;case 6:c.directionPressed=value;break;case 7:c.directionRepeat=value;break;
    default:c.directionCountdown=signedValue;break;
    }
}
}
bool readPhysicalInputGpByte(const PhysicalInputProducer& p,uint32_t offset,uint8_t& value){
    if(offset>=0x1324 && offset<0x13b8){value=uint8_t(readWord(p,offset&~3u)>>(24-8*(offset&3)));return true;}
    if(offset>=0x1310 && offset<0x1314){value=p.channels[offset-0x1310].forceMotor;return true;}
    if(offset<0x13b8 || offset>=0x13d8)return false;
    const auto& c=p.channels[offset&3];const auto& a=c.analog;
    switch((offset-0x13b8)/4){
    case 0:value=uint8_t(a.mainX);break;case 1:value=uint8_t(a.mainY);break;
    case 2:value=uint8_t(a.subX);break;case 3:value=uint8_t(a.subY);break;
    case 4:value=a.leftTrigger;break;case 5:value=a.rightTrigger;break;
    case 6:value=c.motorMode;break;default:value=c.previousMotorMode;break;
    }return true;
}
bool writePhysicalInputGpByte(PhysicalInputProducer& p,uint32_t offset,uint8_t value){
    if(offset>=0x1324 && offset<0x13b8){const unsigned shift=24-8*(offset&3);const auto at=offset&~3u;writeWord(p,at,(readWord(p,at)&~(0xffu<<shift))|(uint32_t(value)<<shift));return true;}
    if(offset>=0x1310 && offset<0x1314){p.channels[offset-0x1310].forceMotor=value;return true;}
    if(offset<0x13b8 || offset>=0x13d8)return false;
    auto& c=p.channels[offset&3];auto& a=c.analog;const int8_t signedValue=value<128?int8_t(value):int8_t(int(value)-256);
    switch((offset-0x13b8)/4){
    case 0:a.mainX=signedValue;break;case 1:a.mainY=signedValue;break;
    case 2:a.subX=signedValue;break;case 3:a.subY=signedValue;break;
    case 4:a.leftTrigger=value;break;case 5:a.rightTrigger=value;break;
    case 6:c.motorMode=value;break;default:c.previousMotorMode=value;break;
    }return true;
}
}

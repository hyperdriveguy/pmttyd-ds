#include "map_object_lifecycle.hpp"
#include <cassert>
namespace ttyd {
bool runMapObjectEvent(MapObjectState& object,uint32_t script,const MapObjectServices& s) {
    if(!script)return false;
    assert(s.eventCheck && s.eventEntry);
    if(s.eventCheck(s.user,object.eventId)!=0)return false;
    object.eventId=0;
    object.eventId=s.eventEntry(s.user,script,object.owner,30,26);
    return true;
}
void advanceSaveBlock(MapObjectState& object,const MapObjectServices& s) {
    switch(object.stage) {
    case 0:
        if(!(object.flags&8))return;
        assert(s.key && s.system && s.sound && s.animation && s.cameraFlag);
        s.key(s.user,false);s.system(s.user,1);s.sound(s.user,object.owner,431);
        s.animation(s.user,object.owner,"A_1",true);s.cameraFlag(s.user,8,0x200,true);++object.stage;return;
    case 1:
        assert(s.loopRatio && s.animation);
        if(s.loopRatio(s.user,object.owner)<1.f)return;
        s.animation(s.user,object.owner,"S_1",true);
        if(object.optionalEvent){runMapObjectEvent(object,object.optionalEvent,s);++object.stage;}
        else object.stage=3;
        return;
    case 2:
        assert(s.eventCheck);
        if(s.eventCheck(s.user,object.eventId)==1)return;
        object.eventId=0;++object.stage;return;
    case 3:
        runMapObjectEvent(object,object.systemEvent,s);++object.stage;return;
    case 4:
        assert(s.eventCheck && s.system && s.cameraFlag && s.key);
        if(s.eventCheck(s.user,object.eventId)==1)return;
        s.system(s.user,0);s.cameraFlag(s.user,8,0x200,false);object.flags&=~8u;
        s.key(s.user,true);object.stage=0;return;
    default:return;
    }
}
}

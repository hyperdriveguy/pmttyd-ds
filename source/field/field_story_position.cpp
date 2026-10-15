#include "field_story_position.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t w){return w<=0x7fffffff?int32_t(w):int32_t(int64_t(w)-0x100000000LL);}
float readFloat(const EventOperandServices& s,uint32_t address){const uint32_t bits=s.readWord(s.user,address);float value;std::memcpy(&value,&bits,4);return value;}
}
// Mario root+8c/+90/+94 uses engine units. Any nonzero mode returns Y-1.
// In that mode Y is read BEFORE destination; mode0 reads destination first.
// Each evtSetFloat may mutate later input, so preserve per-axis routing.
int32_t fieldMarioGetPosition(EventEntry& e,const FieldStoryPositionServices& s) {
    assert(s.mario && s.values.readWord);
    const uint32_t arguments=e.arguments;
    const uint32_t mario=s.mario(s.user);
    const int32_t mode=getEventValue(e,signedWord(s.values.readWord(s.values.user,arguments)),s.values);
    // Resolve each source field/write separately: setter services may change work.
    const int32_t xDestination=signedWord(s.values.readWord(s.values.user,arguments+4));
    const float x=readFloat(s.values,mario+0x8c);
    setEventFloat(e,xDestination,x,s.values);
    int32_t yDestination;float y;
    if(mode) {
        y=readFloat(s.values,mario+0x90);
        yDestination=signedWord(s.values.readWord(s.values.user,arguments+8));
        assert(s.subtract);y=s.subtract(s.user,y,1.f);
    } else {
        yDestination=signedWord(s.values.readWord(s.values.user,arguments+8));
        y=readFloat(s.values,mario+0x90);
    }
    setEventFloat(e,yDestination,y,s.values);
    const int32_t zDestination=signedWord(s.values.readWord(s.values.user,arguments+12));
    const float z=readFloat(s.values,mario+0x94);
    setEventFloat(e,zDestination,z,s.values);
    return 2;
}
// 800e71f8 is the US evt_mario_get_pos callback identity.
bool bindFieldMarioPositionCallback(EventCallbackRegistry& registry,FieldStoryPositionServices& s) {
    return registry.bind(0x800e71f8,&s,[](void* p,EventEntry& e,int32_t){return fieldMarioGetPosition(e,*static_cast<FieldStoryPositionServices*>(p));});
}
// -30000000 is LW0 EVT token; +2 selects LW2. evtGetValue intentionally
// truncates encoded positions for original integer IF comparisons.
bool harborPositionGate(EventEntry& e,HarborPositionGate gate,const EventOperandServices& s) {
    if(getEventValue(e,-30000000+2,s)>-100)return false;
    return gate==HarborPositionGate::GoombellaAttack || getEventValue(e,-30000000,s)>=-25;
}
}

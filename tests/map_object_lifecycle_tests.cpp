#include "map_object_lifecycle.hpp"
#include <cassert>
#include <limits>
#include <vector>
using namespace ttyd;
struct Fixture {
    MapObjectKeyControl keys;std::vector<std::string> order;
    float ratio=0;int32_t check=0;uint16_t camera=0;int32_t system=0;
    MapObjectServices services() {
        MapObjectServices s;s.user=this;
        s.key=[](void* p,bool enabled){auto& f=*static_cast<Fixture*>(p);f.order.push_back(enabled?"key-on":"key-off");if(enabled)f.keys.on();else f.keys.off();};
        s.system=[](void* p,int32_t mode){auto& f=*static_cast<Fixture*>(p);f.system=mode;f.order.push_back(mode?"system-1":"system-0");};
        s.sound=[](void* p,uint32_t owner,uint32_t id){assert(owner==17 && id==431);static_cast<Fixture*>(p)->order.push_back("sound431");};
        s.animation=[](void* p,uint32_t owner,const char* pose,bool restart){assert(owner==17 && restart);static_cast<Fixture*>(p)->order.push_back(pose);};
        s.loopRatio=[](void* p,uint32_t owner){assert(owner==17);return static_cast<Fixture*>(p)->ratio;};
        s.cameraFlag=[](void* p,uint32_t camera,uint16_t mask,bool enabled){assert(camera==8 && mask==0x200);auto& f=*static_cast<Fixture*>(p);f.camera=enabled?f.camera|mask:f.camera&~mask;f.order.push_back(enabled?"camera-on":"camera-off");};
        s.eventCheck=[](void* p,int32_t){return static_cast<Fixture*>(p)->check;};
        s.eventEntry=[](void* p,uint32_t script,uint32_t owner,uint8_t priority,uint8_t type){assert(script && owner==17 && priority==30 && type==26);static_cast<Fixture*>(p)->order.push_back("event");return 41;};
        return s;
    }
};
int main() {
    MapObjectHeadCache names;assert(!names.publish({}));assert(names.publish("HOBJ_mobj_save"));
    assert(!names.publish(std::string("HOBJ_mobj_save")));assert(names.publish("map_static"));
    assert(names.publish("HOBJ_mobj_save"));assert(!names.publish({}));assert(names.publish("HOBJ_mobj_save"));
    for(auto blocked:std::vector<MapObjectHeadInput>{{17,2,0,0},{17,0,31,0},{17,0,32,0},{17,0,0,1},{18,0,0,0}}) {
        MapObjectState object;object.owner=17;produceSaveBlockHead(object,blocked);assert(!(object.flags&8));
    }
    MapObjectState inactive;inactive.owner=17;inactive.flags=0;produceSaveBlockHead(inactive,{17});assert(!inactive.flags);
    MapObjectState disabled;disabled.owner=17;disabled.flags|=0x100;produceSaveBlockHead(disabled,{17});assert(!(disabled.flags&8) && disabled.cause==0x400000);
    Fixture f;auto services=f.services();MapObjectState o;o.owner=17;o.systemEvent=0x8030b4ec;
    advanceSaveBlock(o,services);assert(o.stage==0 && f.order.empty());
    produceSaveBlockHead(o,{17});advanceSaveBlock(o,services);
    assert(o.stage==1 && o.cause==0x400000 && f.keys.depth==1 && f.system==1 && f.camera==0x200);
    assert((f.order==std::vector<std::string>{"key-off","system-1","sound431","A_1","camera-on"}));
    f.order.clear();produceSaveBlockHead(o,{0,0,0,1});assert(!(o.flags&8));
    for(float ratio:{0.f,.999999f}){f.ratio=ratio;advanceSaveBlock(o,services);assert(o.stage==1 && f.order.empty());}
    // Ratio publication after callback: newly published1 waits until next call.
    f.ratio=1;advanceSaveBlock(o,services);assert(o.stage==3 && f.order==std::vector<std::string>{"S_1"});
    advanceSaveBlock(o,services);assert(o.stage==4 && o.eventId==41);
    f.keys.off();assert(f.keys.depth==2); // Original system event's nested owner.
    f.check=1;advanceSaveBlock(o,services);assert(o.stage==4 && f.system==1);
    f.keys.on();assert(f.keys.depth==1 && (f.keys.marioFlags&8));
    f.check=0;advanceSaveBlock(o,services);assert(o.stage==0 && o.eventId==41 && !(o.flags&8));
    assert(!f.keys.depth && !(f.keys.marioFlags&8) && !f.system && !f.camera);
    // Optional child checks exactly1, while the entry helper checks nonzero.
    o.stage=1;o.optionalEvent=99;o.eventId=0;f.ratio=std::numeric_limits<float>::quiet_NaN();
    advanceSaveBlock(o,services);assert(o.stage==2 && o.eventId==41);
    f.check=2;advanceSaveBlock(o,services);assert(o.stage==3 && !o.eventId);
    const auto count=f.order.size();advanceSaveBlock(o,services);assert(o.stage==4 && f.order.size()==count);
    assert(!runMapObjectEvent(o,0,services));
    o.stage=8;advanceSaveBlock(o,services);assert(o.stage==8);
    MapObjectKeyControl wrap;for(unsigned n=0;n<256;++n)wrap.off();assert(!wrap.depth && (wrap.marioFlags&8));
    wrap.on();assert(wrap.depth==255 && (wrap.marioFlags&8));
}

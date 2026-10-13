#include "field_story_position.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventEntry event;
    uint32_t mario=0x2000;
    std::map<uint32_t,uint32_t> words;
    std::vector<uint32_t> writes;
    bool mutateOnMode=false,mutateOnX=false,mutateOnMario=false;
    Fixture(){event.arguments=0x3000;words[0x3000]=0;for(int i=0;i<3;++i)words[0x3004+4*i]=uint32_t(-30000000+i);position(-25.5f,15.25f,-100);}
    void fp(uint32_t at,float f){uint32_t w;std::memcpy(&w,&f,4);words[at]=w;}
    void position(float x,float y,float z){fp(0x208c,x);fp(0x2090,y);fp(0x2094,z);}
    FieldStoryPositionServices services(){
        FieldStoryPositionServices s;s.user=this;
        s.mario=[](void* p){auto& f=*static_cast<Fixture*>(p);if(f.mutateOnMario)f.event.arguments=0x6000;return f.mario;};
        s.subtract=[](void*,float a,float b){return a-b;};
        auto& v=s.values;v.user=this;
        v.contextAddress=[](void* p){auto& f=*static_cast<Fixture*>(p);if(f.mutateOnMode){f.mutateOnMode=false;f.mario=0x4000;f.position(-25.5f,16.25f,-101);}return 0x5000u;};
        v.entryAddress=[](void*,EventEntry&){return 0x1000u;};
        v.readWord=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        v.writeWord=[](void* p,uint32_t a,uint32_t w){auto& f=*static_cast<Fixture*>(p);f.writes.push_back(a);f.words[a]=w;if(f.mutateOnX && a==0x109c){f.fp(0x2090,18.f);f.fp(0x2094,-102.f);}};
        v.truncate=[](void*,float f){assert(std::isfinite(f) && f>=-2147483648.f && double(f)<2147483648.);return int32_t(f);};return s;
    }
};
int main(){
    Fixture f;auto s=f.services();assert(fieldMarioGetPosition(f.event,s)==2);
    assert(f.writes==std::vector<uint32_t>({0x109c,0x10a0,0x10a4}));
    assert(getEventFloat(f.event,-30000000,s.values)==-25.5f && getEventFloat(f.event,-29999999,s.values)==15.25f);
    assert(harborPositionGate(f.event,HarborPositionGate::GoombellaAttack,s.values));
    assert(harborPositionGate(f.event,HarborPositionGate::BeforeBattle,s.values)); // Integer -25 admits float -25.5.
    f.position(-26.f,15.f,-100.f);fieldMarioGetPosition(f.event,s);
    assert(harborPositionGate(f.event,HarborPositionGate::GoombellaAttack,s.values) && !harborPositionGate(f.event,HarborPositionGate::BeforeBattle,s.values));
    f.position(-25.f,15.f,-99.999f);fieldMarioGetPosition(f.event,s);
    assert(!harborPositionGate(f.event,HarborPositionGate::GoombellaAttack,s.values));
    f.position(-25.9999f,15.f,-100.0009f);fieldMarioGetPosition(f.event,s);
    assert(getEventFloat(f.event,-30000000,s.values)==-25.9990234375f && harborPositionGate(f.event,HarborPositionGate::BeforeBattle,s.values));
    f.words[0x3000]=uint32_t(-7);fieldMarioGetPosition(f.event,s);
    assert(getEventFloat(f.event,-29999999,s.values)==14.f); // Every nonzero mode subtracts 1.
    Fixture capture;capture.mutateOnMode=true;auto c=capture.services();fieldMarioGetPosition(capture.event,c);
    assert(capture.mario==0x4000 && getEventFloat(capture.event,-29999999,c.values)==16.25f); // Captured original work pointer.
    Fixture ordered;ordered.mutateOnX=true;auto o=ordered.services();fieldMarioGetPosition(ordered.event,o);
    assert(getEventFloat(ordered.event,-29999999,o.values)==18.f && getEventFloat(ordered.event,-29999998,o.values)==-102.f);
    Fixture arguments;arguments.mutateOnMario=true;auto a=arguments.services();fieldMarioGetPosition(arguments.event,a);
    assert(arguments.event.arguments==0x6000 && getEventFloat(arguments.event,-30000000,a.values)==-25.5f);
    EventCallbackRegistry registry;assert(bindFieldMarioPositionCallback(registry,s));assert(!bindFieldMarioPositionCallback(registry,s));
    int32_t result=0;assert(registry.invoke(f.event,0x800e71f8,0,result) && result==2);
    std::cout<<"field position and original harbor integer gates passed\n";
}

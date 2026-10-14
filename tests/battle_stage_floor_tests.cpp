#include "battle_stage_floor.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float value){uint32_t word;std::memcpy(&word,&value,4);return word;}
struct Fixture {
    std::map<uint32_t,uint32_t> words;
    std::vector<int32_t> resolved;
    int reloads=0;bool switchOwner=false,changeOutput=false;
    int32_t destination=0,output=0;EventEntry event;
    Fixture(){event.arguments=0x1000;words={{0x8041eb90,0x20000},{0x80422170,bits(0)},{0x80422b44,bits(90)},{0x80422b48,bits(-50)},{0x1000,17},{0x1004,999},{0x1008,91}};}
    EventMemoryServices memory(){EventMemoryServices m;m.user=this;
        m.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);if(at==0x8041eb90){++f.reloads;if(f.switchOwner && f.reloads==2)f.words[at]=0x40000;}assert(f.words.count(at));return f.words.at(at);};
        m.storeWord=[](void* p,uint32_t at,uint32_t value){static_cast<Fixture*>(p)->words[at]=value;};return m;}
    BattleUnitEventServices services(){BattleUnitEventServices s;s.user=this;s.memory=memory();s.values.user=this;
        s.values.resolve=[](void* p,EventEntry&,int32_t raw){auto& f=*static_cast<Fixture*>(p);f.resolved.push_back(raw);return raw;};
        s.values.store=[](void* p,EventEntry&,int32_t raw,int32_t value){auto& f=*static_cast<Fixture*>(p);f.destination=raw;f.output=value;};
        s.truncateFloat=[](void*,float value){assert(std::isfinite(value));return int32_t(value);};return s;}
    static float convert(void* p,int32_t v){auto& f=*static_cast<Fixture*>(p);if(f.changeOutput && v==17)f.words[0x1004]=123;return float(v);}
};
int main(){
    Fixture f;auto m=f.memory();f.words[0x36f24]=0x12345678;initializePackedBattleStageFloorFields(0x20000,m);
    assert(f.words[0x36f28]==bits(90) && f.words[0x36f30]==bits(-50) && f.words[0x36f24]==0x12345678);
    assert(packedBattleStageFloorHeight(0,99,-99,90,m)==0);
    assert(packedBattleStageFloorHeight(0,0,0,std::nextafter(90.f,100.f),m)==-50);
    assert(packedBattleStageFloorHeight(0,0,0,std::numeric_limits<float>::quiet_NaN(),m)==-50);
    f.words[0x36f30]=bits(-17.75f);f.changeOutput=true;auto s=f.services();
    assert(battleStageFloorPositionEvent(f.event,s,&f,Fixture::convert)==2);
    assert((f.resolved==std::vector<int32_t>{17,91}) && f.destination==123 && f.output==-17);
    // Wrapper captures B, then GetFloorHeight reloads the global stage owner.
    f.reloads=0;f.switchOwner=true;f.words[0x56f28]=bits(80);f.words[0x56f30]=bits(-42.5f);f.resolved.clear();
    assert(battleStageFloorPositionEvent(f.event,s,&f,Fixture::convert)==2 && f.reloads==2 && f.output==-42);
    f.words[0x56f28]=bits(std::numeric_limits<float>::quiet_NaN());
    assert(packedBattleStageFloorHeight(0x20000,0,0,-999,m)==-42.5f);
    // Existing mutable fields are consumed; no per-query fresh-stage reset.
    f.words[0x56f28]=bits(1000);assert(packedBattleStageFloorHeight(0x20000,0,0,91,m)==0);
}

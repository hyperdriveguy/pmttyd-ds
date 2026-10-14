#include "battle_hit_code_events.hpp"
#include <cassert>
#include <map>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words{{0x1000,uint32_t(-2)},{0x1004,0x100100},{0x1008,uint32_t(-30000001)},{0x8041eb90,0x2000},{0x4278,0x112}};
    EventEntry event;int resolves=0;int32_t destination=0,result=-1;bool mutate=false;
    Fixture(){event.arguments=0x1000;event.owner=2;}
    BattleUnitEventServices services(){BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);assert(f.words.count(at));return f.words.at(at);};
        s.values.resolve=[](void* p,EventEntry&,int32_t value){auto& f=*static_cast<Fixture*>(p);assert(value==-2);++f.resolves;return value;};
        s.translate=[](void* p,EventEntry& e,int32_t id){auto& f=*static_cast<Fixture*>(p);assert(id==-2);if(f.mutate){f.words[0x8041eb90]=0x3000;f.words[0x1004]=0x100;}return e.owner;};
        s.unit=[](void* p,uint32_t battle,int32_t id){auto& f=*static_cast<Fixture*>(p);assert(battle==(f.mutate?0x3000u:0x2000u) && id==2);if(f.mutate){f.words[0x1008]=77;f.words[0x1004]=88;}return uint32_t(0x4000);};
        s.values.store=[](void* p,EventEntry&,int32_t out,int32_t value){auto& f=*static_cast<Fixture*>(p);f.destination=out;f.result=value;};return s;}
};
int main(){Fixture f;auto s=f.services();assert(battleCheckHitCodeEvent(f.event,s)==2 && f.result==1 && f.resolves==1 && f.destination==-30000001); // Any overlap, not all mask bits.
 f.words[0x1004]=0;assert(battleCheckHitCodeEvent(f.event,s)==2 && f.result==0);
 f.words[0x1004]=0x80000000;f.words[0x4278]=0x800001ff;assert(battleCheckHitCodeEvent(f.event,s)==2 && f.result==1);
 f.words[0x1004]=uint32_t(-30000002);assert(battleGetHitCodeEvent(f.event,s)==2 && f.result==255 && f.destination==-30000002);
 f.words[0x4278]=0x1200;assert(battleGetHitCodeEvent(f.event,s)==2 && f.result==0);
 f.words[0x4274]=0x101;f.words[0x1004]=1;
 assert(battleCheckHitPatternEvent(f.event,s)==2 && f.result==0); // Exact pattern, not code-mask overlap.
 f.words[0x1004]=0x101;assert(battleCheckHitPatternEvent(f.event,s)==2 && f.result==1);
 Fixture changed;changed.mutate=true;assert(battleCheckHitCodeEvent(changed.event,changed.services())==2 && changed.result==1 && changed.destination==-30000001);
 changed.words[0x1004]=uint32_t(-30000003); // Translator changes it to100 before capture; unit callback changes it to88 after capture.
 assert(battleGetHitCodeEvent(changed.event,changed.services())==2 && changed.destination==0x100 && changed.result==18);
}

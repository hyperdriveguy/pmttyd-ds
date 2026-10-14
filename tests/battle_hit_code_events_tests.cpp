#include "battle_hit_code_events.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    std::map<uint32_t,uint32_t> words{{0x1000,uint32_t(-2)},{0x1004,0x100100},{0x1008,uint32_t(-30000001)},{0x8041eb90,0x2000},{0x4278,0x112}};
    EventEntry event;int resolves=0;int32_t destination=0,result=-1;bool mutate=false,captured=false,nullUnit=false;
    std::map<uint32_t,uint8_t> bytes{{0x400c,1},{0x2012,255},{0x4188,77}};
    Fixture(){event.arguments=0x1000;event.owner=2;}
    BattleActorServices byteServices(){BattleActorServices b;b.user=this;b.byte=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);assert(f.bytes.count(at));return f.bytes.at(at);};return b;}
    BattleUnitEventServices services(){BattleUnitEventServices s;s.user=this;s.memory.user=this;s.values.user=this;
        s.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);assert(f.words.count(at));return f.words.at(at);};
        s.memory.half=[](void* p,uint32_t at){return uint16_t(static_cast<Fixture*>(p)->words.at(at));};
        s.memory.storeHalf=[](void* p,uint32_t at,uint16_t value){static_cast<Fixture*>(p)->words[at]=value;};
        s.memory.storeWord=[](void* p,uint32_t at,uint32_t value){static_cast<Fixture*>(p)->words[at]=value;};
        s.memory.storeByte=[](void* p,uint32_t at,uint8_t value){static_cast<Fixture*>(p)->bytes[at]=value;};
        s.values.resolve=[](void* p,EventEntry&,int32_t value){auto& f=*static_cast<Fixture*>(p);assert(value==-2);++f.resolves;return value;};
        s.translate=[](void* p,EventEntry& e,int32_t id){auto& f=*static_cast<Fixture*>(p);assert(id==-2);if(f.mutate){f.words[0x8041eb90]=0x3000;f.words[0x1004]=0x100;}return e.owner;};
        s.unit=[](void* p,uint32_t battle,int32_t id){auto& f=*static_cast<Fixture*>(p);assert(battle==(f.mutate && !f.captured?0x3000u:0x2000u) && id==2);if(f.mutate){f.words[0x1008]=77;f.words[0x1004]=88;}return f.nullUnit?uint32_t(0):uint32_t(0x4000);};
        s.values.store=[](void* p,EventEntry&,int32_t out,int32_t value){auto& f=*static_cast<Fixture*>(p);f.destination=out;f.result=value;};return s;}
};
struct DamageWrapperFixture {
 std::map<uint32_t,uint32_t> words{{0x1000,10},{0x1004,11},{0x1008,12},{0x100c,13},{0x1010,0x80000100},{0x1014,uint32_t(-30000001)},{0x8041eb90,0x2000}};
 EventEntry event;std::vector<int> order;int32_t coreResult=18,stored=-77,destination=0;bool ready=true,exists=true,nullDefender=false;
 DamageWrapperFixture(){event.arguments=0x1000;event.id=41;event.flags=0;}
 BattleCheckDamageEventServices services(){BattleCheckDamageEventServices s;s.user=this;s.units.user=this;s.units.memory.user=this;s.units.values.user=this;
 s.units.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<DamageWrapperFixture*>(p);f.order.push_back(int(at));return f.words.at(at);};
 s.units.values.resolve=[](void* p,EventEntry&,int32_t raw){auto& f=*static_cast<DamageWrapperFixture*>(p);assert(raw>=10 && raw<=13);f.order.push_back(100+raw);return raw+20;};
 s.units.translate=[](void* p,EventEntry&,int32_t id){auto& f=*static_cast<DamageWrapperFixture*>(p);f.order.push_back(200+id);f.words[0x8041eb90]=0x3000;return id+40;};
 s.units.unit=[](void* p,uint32_t battle,int32_t id){auto& f=*static_cast<DamageWrapperFixture*>(p);assert(battle==0x2000 && (id==70 || id==71));f.order.push_back(300+id);return uint32_t(id==70?0x4000:f.nullDefender?0:0x5000);};
 s.part=[](void* p,uint32_t unit,int32_t id){auto& f=*static_cast<DamageWrapperFixture*>(p);assert(unit==(f.nullDefender?0u:0x5000u) && id==32);f.order.push_back(400);return uint32_t(f.nullDefender?0:0x6000);};
 s.checkDamage=[](void* p,uint32_t a,uint32_t d,uint32_t part,uint32_t weapon,uint32_t flags,int32_t& result){auto& f=*static_cast<DamageWrapperFixture*>(p);assert(a==0x4000 && d==(f.nullDefender?0u:0x5000u) && part==(f.nullDefender?0u:0x6000u) && weapon==33 && flags==0x80000100);f.order.push_back(500);f.words[0x1014]=88;result=f.coreResult;return f.ready;};
 s.units.values.store=[](void* p,EventEntry& e,int32_t out,int32_t value){auto& f=*static_cast<DamageWrapperFixture*>(p);f.order.push_back(600);f.destination=out;f.stored=value;e.id=42;};
 s.eventExists=[](void* p,uint32_t id){auto& f=*static_cast<DamageWrapperFixture*>(p);assert(id==42);f.order.push_back(700);return f.exists;};return s;}
};
void damageWrapperTests(){
 DamageWrapperFixture f;assert(battleCheckDamageEvent(f.event,f.services())==2 && f.stored==18 && f.destination==-30000001);
 const std::vector<int> expected{int(0x8041eb90),0x1000,110,230,370,0x1004,111,231,371,0x1008,112,400,0x100c,113,0x1010,0x1014,500,600,700};assert(f.order==expected);
 DamageWrapperFixture cancelled;cancelled.exists=false;cancelled.event.flags=1;assert(battleCheckDamageEvent(cancelled.event,cancelled.services())==255 && cancelled.stored==18); // Output precedes the live-ID query even when caller absent.
 DamageWrapperFixture nullTarget;nullTarget.nullDefender=true;assert(battleCheckDamageEvent(nullTarget.event,nullTarget.services())==2 && nullTarget.stored==18);
 DamageWrapperFixture negative;negative.coreResult=-1;assert(battleCheckDamageEvent(negative.event,negative.services())==255 && negative.stored==-77 && negative.order.back()==500);
 DamageWrapperFixture unbound;unbound.ready=false;assert(battleCheckDamageEvent(unbound.event,unbound.services())==0 && unbound.stored==-77 && unbound.order.back()==500);
}
int main(){damageWrapperTests();Fixture f;auto s=f.services();assert(battleCheckHitCodeEvent(f.event,s)==2 && f.result==1 && f.resolves==1 && f.destination==-30000001); // Any overlap, not all mask bits.
 f.words[0x1004]=0;assert(battleCheckHitCodeEvent(f.event,s)==2 && f.result==0);
 f.words[0x1004]=0x80000000;f.words[0x4278]=0x800001ff;assert(battleCheckHitCodeEvent(f.event,s)==2 && f.result==1);
 f.words[0x1004]=uint32_t(-30000002);assert(battleGetHitCodeEvent(f.event,s)==2 && f.result==255 && f.destination==-30000002);
 f.words[0x4278]=0x1200;assert(battleGetHitCodeEvent(f.event,s)==2 && f.result==0);
 f.words[0x4274]=0x101;f.words[0x1004]=1;
 assert(battleCheckHitPatternEvent(f.event,s)==2 && f.result==0); // Exact pattern, not code-mask overlap.
 f.words[0x1004]=0x101;assert(battleCheckHitPatternEvent(f.event,s)==2 && f.result==1);
 f.words[0x4104]=0x80000000;f.words[0x1004]=0x80000001;
 assert(battleCheckUnitAttributeEvent(f.event,s)==2 && f.result==1); // Raw mask, any overlap.
 f.words[0x1004]=1;assert(battleCheckUnitAttributeEvent(f.event,s)==2 && f.result==0);
 Fixture changed;changed.mutate=true;assert(battleCheckHitCodeEvent(changed.event,changed.services())==2 && changed.result==1 && changed.destination==-30000001);
 changed.words[0x1004]=uint32_t(-30000003); // Translator changes it to100 before capture; unit callback changes it to88 after capture.
 assert(battleGetHitCodeEvent(changed.event,changed.services())==2 && changed.destination==0x100 && changed.result==18);
 Fixture flags;flags.words[0x401c]=0x10;flags.words[0x1004]=0x80000100;
 assert(battleUnitFlagEvent(flags.event,true,flags.services())==2 && flags.words[0x401c]==0x80000110);
 assert(battleUnitFlagEvent(flags.event,false,flags.services())==2 && flags.words[0x401c]==0x10);
 Fixture facing;assert(battleResetUnitFacingEvent(facing.event,facing.services(),facing.byteServices())==2 && facing.bytes[0x4189]==255 && facing.bytes[0x4188]==77);
 facing.mutate=true;facing.captured=true;facing.bytes[0x400c]=255;facing.bytes[0x2002]=7;
 assert(battleResetUnitFacingEvent(facing.event,facing.services(),facing.byteServices())==2 && facing.bytes[0x4189]==7); // Captured battle, signed belong=-1.

 Fixture amount;amount.words[0x1004]=uint32_t(-30000004);
 for(const auto raw:{0,1,127,128,255}){amount.bytes[0x4270]=uint8_t(raw);assert(battleGetHitDamageEvent(amount.event,amount.services(),amount.byteServices())==2 && amount.result==(raw<128?raw:raw-256) && amount.destination==-30000004);}
 amount.mutate=true;assert(battleGetHitDamageEvent(amount.event,amount.services(),amount.byteServices())==2 && amount.destination==88); // Output capture AFTER unit lookup.
 Fixture absent;absent.nullUnit=true;assert(battleGetHitDamageEvent(absent.event,absent.services(),absent.byteServices())==2 && absent.result==-1 && absent.destination==0);

 Fixture marker;marker.words[0x41fe]=0xa500;assert(battleMarkUnit1feEvent(marker.event,marker.services())==2 && marker.words[0x41fe]==0xa501);
 marker.mutate=true;marker.captured=true;assert(battleMarkUnit1feEvent(marker.event,marker.services())==2 && marker.words[0x41fe]==0xa501);
 Fixture noMarker;noMarker.nullUnit=true;assert(battleMarkUnit1feEvent(noMarker.event,noMarker.services())==2);

}

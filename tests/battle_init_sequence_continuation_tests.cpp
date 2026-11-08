#include <memory>
#include "battle_init_sequence_continuation.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Battle=0x1000,Mario=0x21000,Party=0x22000,Unit=0x23000;
struct F {
 EventNativeRuntime vm{0x81000000,{}};std::shared_ptr<const uint8_t>life=std::make_shared<const uint8_t>(0);BattleInitSequenceCursor cursor;
 std::vector<uint32_t>trace;bool party=true,live=false,nice=true,alive=true,reenter=false;uint32_t blocked=0;size_t success=0,hold=SIZE_MAX;unsigned attempts=0;const BattleFirstActRequest*held=nullptr;
 F(){assert(vm.memory.registerRam(Battle,std::vector<uint8_t>(0x30000)));w(Battle+0xf0c,0x01000000);w(Mario+4,0x80);w(Party+4,0x40);w(Battle+0x20,Unit);w(Unit+0x288,99);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 BattleStartupServices reference(){BattleStartupServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
 s.mario=[](void*p){auto&f=*static_cast<F*>(p);f.trace.push_back(0x8005e0c0);return Mario;};s.marioPartyId=[](void*p)->int32_t{auto&f=*static_cast<F*>(p);f.trace.push_back(0x80141050);return 77;};s.party=[](void*p,int32_t i){assert(i==77);auto&f=*static_cast<F*>(p);f.trace.push_back(0x800bf1b4);return f.party?Party:0;};s.unitSetup=[](void*p,uint32_t b){assert(b==Battle);static_cast<F*>(p)->trace.push_back(0x800f8aac);};
 s.eventExists=[](void*p,uint32_t id){auto&f=*static_cast<F*>(p);f.trace.push_back(id);return f.live;};s.paperReady=[](void*p,uint32_t key,int32_t mode){assert(mode==1);auto&f=*static_cast<F*>(p);f.trace.push_back(key);return key!=f.blocked;};s.niceReady=[](void*p,int32_t mode){assert(mode==1);auto&f=*static_cast<F*>(p);f.trace.push_back(0x80144d44);return f.nice;};s.statusEventOn=[](void*p){static_cast<F*>(p)->trace.push_back(0x800f7650);};s.statusSystemOn=[](void*p){static_cast<F*>(p)->trace.push_back(0x800f7624);};return s;}
 BattleInitSequenceServices services(){BattleInitSequenceServices s;s.actors=nativeBattleActorServices(vm);s.lifetime=life;s.user=this;s.current=[](void*p){return static_cast<F*>(p)->alive;};s.call=[](void*p,const BattleFirstActRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<F*>(p);assert(!q.firstAttackUnits);if(f.reenter){assert(!f.cursor.reset());assert(!resumePackedBattleSequenceInit(Battle,f.services(),f.cursor));}if(f.success==f.hold&&f.attempts++<2){if(f.held)assert(f.held==&q);f.held=&q;return {};}f.held=nullptr;++f.success;auto old=f.reference();switch(q.function){
 case 0x8005e0c0:return old.mario(&f);case 0x80141050:return old.marioPartyId(&f);case 0x800bf1b4:assert(q.integers[0]==77);return old.party(&f,int32_t(q.integers[0]));case 0x800f8aac:assert(q.integers[0]==Battle);old.unitSetup(&f,Battle);return 0;
 case 0x80129994:return packedBattleUnitInitReady(Battle,old);case 0x800414e8:return old.paperReady(&f,q.integers[0],int32_t(q.integers[1]));case 0x80144d44:return old.niceReady(&f,int32_t(q.integers[0]));case 0x800f7650:old.statusEventOn(&f);return 0;case 0x800f7624:old.statusSystemOn(&f);return 0;default:assert(false);return {};}};return s;}
 void run(){for(unsigned i=0;i<100;++i)if(resumePackedBattleSequenceInit(Battle,services(),cursor))return;assert(false);}
 std::vector<uint32_t>snapshot(){std::vector<uint32_t>v;for(auto a:{Battle+4,Battle+0xf0c,Battle+0xf10,Mario+4,Party+4,Unit+0x288})v.push_back(w(a));return v;}
};
void configure(F&f,unsigned state,unsigned policy){f.w(Battle+0xf0c,0x01000000+state);f.party=policy&1;f.live=policy&2;f.nice=policy&4;if(policy&8)f.blocked=0x802efbec;}
void differential(){for(unsigned state=0;state<8;++state)for(unsigned policy=0;policy<16;++policy){F a,b;configure(a,state,policy);configure(b,state,policy);packedBattleSequenceInit(Battle,a.reference());b.run();assert(a.trace==b.trace&&a.snapshot()==b.snapshot());}}
void waits(){for(unsigned state=0;state<6;++state){F base;configure(base,state,5);base.run();for(size_t at=0;at<base.success;++at){F f;configure(f,state,5);f.hold=at;f.reenter=true;f.run();assert(f.attempts==3&&f.success==base.success&&f.trace==base.trace&&f.snapshot()==base.snapshot());}}}
void zeroVersusHold(){F f;configure(f,3,13);f.run();assert(!f.cursor.active()&&f.w(Battle+0xf0c)==0x01000003&&f.trace==std::vector<uint32_t>({0x802efbe4,0x802efbec}));f.trace.clear();f.blocked=0;f.run();assert(f.w(Battle+0xf0c)==0x01000004&&f.trace.size()==4);F held;held.hold=3;assert(!resumePackedBattleSequenceInit(Battle,held.services(),held.cursor));assert(held.w(Mario+4)==0x81&&held.w(Party+4)==0x41);const auto prefix=held.trace;auto changed=held.services();changed.user=&f;assert(!resumePackedBattleSequenceInit(Battle,changed,held.cursor)&&held.trace==prefix);held.alive=false;assert(!resumePackedBattleSequenceInit(Battle,held.services(),held.cursor));held.alive=true;assert(!resumePackedBattleSequenceInit(Battle,held.services(),held.cursor));}
}
int main(){differential();waits();zeroVersusHold();std::cout<<"retained whole Init differential/resource0/holds/lifetime tests PASS\n";}

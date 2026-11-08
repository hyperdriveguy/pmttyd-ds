#include <memory>
#include "battle_first_act_continuation.hpp"
#include "battle_first_act.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <deque>
#include <set>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t Battle=0x1000,Mario=0x23000,Enemy=0x24000,Info=0x25000,Npc=0x26000,FieldMario=0x2f000,EventBase=0x30000;
struct F {
 EventNativeRuntime vm{0x81000000,{}};std::shared_ptr<const uint8_t>life=std::make_shared<const uint8_t>(0);
 std::vector<uint32_t>trace;std::deque<EventEntry>events;std::set<uint32_t>live;
 bool fade=true,ready=true,done=false,alive=true,reenter=false,unknownPair=false,partialPair=false;uint32_t message=0,announce=0,reaction=0,slot=0,motion=0,status=0;
 size_t successes=0,holdAt=SIZE_MAX;unsigned attempts=0;BattleFirstActCursor cursor;const BattleFirstActRequest*held=nullptr;
 F(){assert(vm.memory.registerRam(Battle,std::vector<uint8_t>(0x40000)));assert(vm.memory.registerRam(0x80422550,std::vector<uint8_t>(4)));w(Battle+0x2738,Info);w(Info+12,Npc);w(0x80422550,255);state(0);w(Battle+0x20,Mario);w(Battle+0x24,Enemy);w(Mario,1);w(Enemy,2);w(Mario+8,222);w(Enemy+8,199);w(Mario+0x2b8,0x1234);w(Enemy+0x2b8,0x2345);w(Enemy+0x2b0,0x3456);w(Npc+0xb0,0x4567);w(Enemy+0x2a0,0x4567);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void state(unsigned i){w(Battle+0xf10,0x02000000+i);}void b(uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));}
 EventEntry&create(uint32_t script){trace.push_back(script);events.emplace_back();auto&e=events.back();e.id=uint32_t(100+events.size());e.owner=-1;return e;}
 uint32_t normalize(uint32_t fn){switch(fn){case 0x8005e0c0:return 1;case 0x8012733c:return 2;case 0x80013368:return 3;case 0x80010d94:return 4;case 0x800d98bc:return 5;case 0x800d95f8:return 6;case 0x8011ace8:return 7;case 0x8011aa34:return 8;case 0x801956f8:return 9;case 0x80195af8:return 10;case 0x801958bc:return 11;case 0x800f7650:return 12;case 0x800f7624:return 13;case 0x800f74b4:return 14;case 0x801a7e94:return 15;case 0x801a7e30:return 16;case 0x80203394:return 17;case 0x80148824:return 18;case 0x80148414:return 19;case 0x80127814:return 20;default:return fn;}}
 BattleFirstActServices old(){BattleFirstActServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
 s.create=[](void*p,uint32_t x,int32_t priority,uint32_t flags)->EventEntry&{assert(priority==10&&!flags);return static_cast<F*>(p)->create(x);};
 s.fieldMarioMotion=[](void*p){auto&f=*static_cast<F*>(p);f.trace.push_back(1);return uint16_t(f.motion);};s.unitData=[](void*p,uint32_t,int32_t key){assert(key==65);auto&f=*static_cast<F*>(p);f.trace.push_back(2);return uint32_t(0x9876);};
 s.fadeEntry=[](void*p,int32_t t,int32_t d,uint32_t c){assert(t==23&&d==200&&c==255);static_cast<F*>(p)->trace.push_back(3);};s.fadeFinished=[](void*p){auto&f=*static_cast<F*>(p);f.trace.push_back(4);return f.fade;};s.sound=[](void*p,uint32_t x){assert(x==0x802efbc4);static_cast<F*>(p)->trace.push_back(5);return uint32_t(77);};s.soundOff=[](void*p,uint32_t x){assert(x==77);static_cast<F*>(p)->trace.push_back(6);};
 s.allEventsReady=[](void*p,uint32_t){auto&f=*static_cast<F*>(p);f.trace.push_back(7);return f.ready;};s.eventExists=[](void*p,uint32_t x){auto&f=*static_cast<F*>(p);f.trace.push_back(x);return bool(f.live.count(x));};s.concluded=[](void*p,uint32_t){auto&f=*static_cast<F*>(p);f.trace.push_back(8);return f.done;};
 s.statusMessage=[](void*p,uint32_t){auto&f=*static_cast<F*>(p);f.trace.push_back(9);return int32_t(f.message);};s.statusAnnounce=[](void*p,uint32_t){auto&f=*static_cast<F*>(p);f.trace.push_back(10);return int32_t(f.announce);};s.statusAdjust=[](void*p,uint32_t){static_cast<F*>(p)->trace.push_back(11);};
 s.statusEventOn=[](void*p){static_cast<F*>(p)->trace.push_back(12);};s.statusSystemOn=[](void*p){static_cast<F*>(p)->trace.push_back(13);};s.consumeReserve=[](void*p){static_cast<F*>(p)->trace.push_back(14);};s.audiencePerAct=[](void*p){static_cast<F*>(p)->trace.push_back(15);};s.audienceReaction=[](void*p){auto&f=*static_cast<F*>(p);f.trace.push_back(16);return int32_t(f.reaction);};s.slotReaction=[](void*p){auto&f=*static_cast<F*>(p);f.trace.push_back(17);return int32_t(f.slot);};s.wallClose=[](void*p){static_cast<F*>(p)->trace.push_back(18);};s.nozzleCheck=[](void*p,int32_t mode){assert(!mode);static_cast<F*>(p)->trace.push_back(19);};s.status=[](void*p,uint32_t,int32_t type){assert(type==27);auto&f=*static_cast<F*>(p);f.trace.push_back(20);return int32_t(f.status);};return s;}
 BattleFirstActContinuationServices services(){BattleFirstActContinuationServices s;s.actors=nativeBattleActorServices(vm);s.lifetime=life;s.user=this;s.current=[](void*p){return static_cast<F*>(p)->alive;};s.call=[](void*p,const BattleFirstActRequest&q)->std::optional<uint32_t>{return static_cast<F*>(p)->call(q);};return s;}
 std::optional<uint32_t>call(const BattleFirstActRequest&q){if(reenter){assert(!cursor.reset());assert(!resumePackedBattleFirstAct(Battle,services(),cursor));}if(successes==holdAt&&attempts++<2){if(held)assert(held==&q);held=&q;return {};}held=nullptr;++successes;
  const auto&a=q.integers;if(q.function==0x800374ac){auto&e=create(a[0]);assert(a[1]==10);const auto address=EventBase+uint32_t(events.size()-1)*0x200;w(address+0x15c,e.id);w(address+0x160,uint32_t(e.owner));return address;}
  if(q.function==0x80036604){trace.push_back(a[0]);return live.count(a[0]);}
  trace.push_back(normalize(q.function));switch(q.function){case 0x8005e0c0:assert(vm.memory.writeHalf(FieldMario+0x2e,uint16_t(motion)));return FieldMario;case 0x8012733c:return 0x9876;case 0x80013368:assert(a[0]==23&&a[1]==200&&a[2]==255);return 0;case 0x80010d94:return fade;case 0x800d98bc:return 77;case 0x800d95f8:assert(a[0]==77);return 0;case 0x8011ace8:return ready;case 0x8011aa34:return done;case 0x801956f8:return message;case 0x80195af8:return announce;case 0x801a7e30:return reaction;case 0x80203394:return slot;case 0x80127814:return status;
  case 0x800feaf8:assert(q.firstAttackUnits);if(!unknownPair){(*q.firstAttackUnits)[0]=Mario;if(!partialPair)(*q.firstAttackUnits)[1]=Enemy;}return 0;
  case 0x8011edb8:return Npc;case 0x80128144:return status;case 0x800f8728:return Enemy;case 0x800f87a8:return Mario;default:return 0;}}
 void run(){for(unsigned i=0;i<100;++i)if(resumePackedBattleFirstAct(Battle,services(),cursor))return;assert(false);}
 std::vector<uint32_t>snapshot(){std::vector<uint32_t>v;for(auto span:std::vector<std::pair<uint32_t,unsigned>>{{Battle,0x19400},{Mario,0xb34},{Enemy,0xb34}})for(unsigned i=0;i<span.second/4;++i)v.push_back(w(span.first+4*i));return v;}
};
void configure(F&f,unsigned state,unsigned bits){f.state(state);f.fade=bits&1;f.ready=bits&2;f.done=bits&4;f.message=bits&8;f.announce=bits&16;f.reaction=bits&32;f.slot=bits&64;f.motion=bits&128?26:0;f.w(Battle+0x19070,77);f.w(Battle+0xefc,bits&256?2:0);if(bits&512)f.w(Npc+4,0x100010);if(bits&1024){f.b(Mario+0x304,1);f.w(Battle+0x1905c,123);f.w(Battle+0x182c0,123);f.live.insert(123);}}
void differential(){for(unsigned state=0;state<15;++state)for(unsigned bits:{0u,3u,7u,11u,19u,35u,67u,131u,259u,515u,1027u}){F a,b;configure(a,state,bits);configure(b,state,bits);assert(packedBattleFirstAct(Battle,a.old()));b.run();assert(a.trace==b.trace);assert(a.snapshot()==b.snapshot());assert(a.events.size()==b.events.size());for(size_t i=0;i<a.events.size();++i)assert(uint32_t(a.events[i].owner)==b.w(EventBase+uint32_t(i)*0x200+0x160));}}
void waits(){for(unsigned state=0;state<15;++state){F base;configure(base,state,131);base.run();for(size_t at=0;at<base.successes;++at){F f;configure(f,state,131);f.holdAt=at;f.reenter=true;f.run();assert(f.attempts==3&&f.successes==base.successes&&f.trace==base.trace&&f.snapshot()==base.snapshot());}}}
void strikes(){for(int32_t first:{-1,1,3,4,6,7,8,9,10}){F base;base.w(Info+8,uint32_t(first));base.run();assert(base.w(Battle+0xf10)==0x02000001&&(base.w(Battle+0xef4)&2)&&base.w(Battle+0x420)==1);for(size_t at=0;at<base.successes;++at){F f;f.w(Info+8,uint32_t(first));f.holdAt=at;f.reenter=true;f.run();assert(f.attempts==3&&f.successes==base.successes&&f.trace==base.trace&&f.snapshot()==base.snapshot());}}
 F partial;partial.w(Info+8,1);partial.partialPair=true;assert(!resumePackedBattleFirstAct(Battle,partial.services(),partial.cursor));assert(partial.w(Battle+0x420)==1&&(partial.w(Mario+0x27c)&0x1000));const auto partialCalls=partial.successes;partial.w(Battle+0x420,77);assert(!resumePackedBattleFirstAct(Battle,partial.services(),partial.cursor)&&partial.w(Battle+0x420)==77&&partial.successes==partialCalls);
 F missing;missing.w(Info+8,1);missing.unknownPair=true;assert(!resumePackedBattleFirstAct(Battle,missing.services(),missing.cursor));const auto calls=missing.successes;assert(!resumePackedBattleFirstAct(Battle,missing.services(),missing.cursor)&&missing.successes==calls&&missing.cursor.pending()==0x800feaf8);
}
void identity(){F f;f.holdAt=0;assert(!resumePackedBattleFirstAct(Battle,f.services(),f.cursor));auto changed=f.services();changed.lifetime=std::make_shared<const uint8_t>(0);assert(!resumePackedBattleFirstAct(Battle,changed,f.cursor));f.alive=false;assert(!resumePackedBattleFirstAct(Battle,f.services(),f.cursor));f.alive=true;assert(!resumePackedBattleFirstAct(Battle,f.services(),f.cursor));}
}
int main(){differential();waits();strikes();identity();std::cout<<"retained whole FirstAct source differential/hold/first-strike tests PASS\n";}

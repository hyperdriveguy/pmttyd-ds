#include "battle_first_act.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <deque>
#include <set>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<uint32_t> calls;std::deque<EventEntry> entries;std::set<uint32_t> live;
 bool fade=false,ready=false,done=false;int32_t message=0,announce=0,reaction=0,slot=0;uint16_t motion=0;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x28000)));assert(vm.memory.registerRam(0x80422550,std::vector<uint8_t>(4)));w(0x3738,0x25000);w(0x2500c,0x26000);w(0x80422550,0xff);state(0);}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void state(unsigned n){w(0x1f10,0x02000000+n);}unsigned state(){return w(0x1f10)-0x02000000;}
 BattleFirstActServices services(){BattleFirstActServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
 s.create=[](void* p,uint32_t script,int32_t priority,uint32_t flags)->EventEntry&{auto& f=*static_cast<Fixture*>(p);assert(priority==10 && flags==0);f.calls.push_back(script);f.entries.emplace_back();auto& e=f.entries.back();e.id=100+f.entries.size();e.owner=-1;return e;};
 s.fieldMarioMotion=[](void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);return f.motion;};s.unitData=[](void* p,uint32_t u,int32_t key){assert(u==0x23000 && key==65);static_cast<Fixture*>(p)->calls.push_back(2);return uint32_t(0x9876);};
 s.fadeEntry=[](void* p,int32_t type,int32_t raw,uint32_t color){assert(type==23 && raw==200 && color==0xff);static_cast<Fixture*>(p)->calls.push_back(3);};s.fadeFinished=[](void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);return f.fade;};s.sound=[](void* p,uint32_t key){assert(key==0x802efbc4);static_cast<Fixture*>(p)->calls.push_back(5);return uint32_t(77);};s.soundOff=[](void* p,uint32_t voice){assert(voice==77);static_cast<Fixture*>(p)->calls.push_back(6);};
 s.allEventsReady=[](void* p,uint32_t b){assert(b==0x1000);auto& f=*static_cast<Fixture*>(p);f.calls.push_back(7);return f.ready;};s.eventExists=[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(id);return f.live.count(id)!=0;};
 s.concluded=[](void* p,uint32_t){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(8);return f.done;};s.statusMessage=[](void* p,uint32_t)->int32_t{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(9);return f.message;};s.statusAnnounce=[](void* p,uint32_t)->int32_t{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(10);return f.announce;};s.statusAdjust=[](void* p,uint32_t){static_cast<Fixture*>(p)->calls.push_back(11);};
 s.statusEventOn=[](void* p){static_cast<Fixture*>(p)->calls.push_back(12);};s.statusSystemOn=[](void* p){static_cast<Fixture*>(p)->calls.push_back(13);};s.consumeReserve=[](void* p){static_cast<Fixture*>(p)->calls.push_back(14);};s.audiencePerAct=[](void* p){static_cast<Fixture*>(p)->calls.push_back(15);};s.audienceReaction=[](void* p)->int32_t{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(16);return f.reaction;};s.slotReaction=[](void* p)->int32_t{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(17);return f.slot;};s.wallClose=[](void* p){static_cast<Fixture*>(p)->calls.push_back(18);};s.nozzleCheck=[](void* p,int32_t mode){assert(mode==0);static_cast<Fixture*>(p)->calls.push_back(19);};s.status=[](void* p,uint32_t,int32_t type)->int32_t{assert(type==27);static_cast<Fixture*>(p)->calls.push_back(20);return 0;};return s;
 }
 void run(){calls.clear();assert(packedBattleFirstAct(0x1000,services()));}
};
int main(){
 Fixture f;f.w(0x1020,0x23000);f.w(0x23000,0);f.w(0x23008,222);f.w(0x232b8,0x1234);f.motion=26;f.run();assert(f.state()==1 && f.w(0x232bc)==102 && f.entries.size()==2 && f.entries[0].owner==-1 && f.entries[1].owner==0 && (f.calls==std::vector<uint32_t>{0x80416d70,1,2,0x9876}));
 f.run();assert(f.state()==2 && f.w(0x1efc)==0xffffffff && f.w(0x1a070)==77 && (f.calls==std::vector<uint32_t>{3,5,4,7,7}));
 f.ready=true;f.run();assert(f.state()==2 && (f.calls==std::vector<uint32_t>{4,7,12,7,4}));f.fade=true;f.w(0x2301c,0x04000080);f.run();assert(f.state()==3 && f.w(0x1a070)==0xffffffff && f.w(0x2301c)==0x80 && (f.calls==std::vector<uint32_t>{4,6,7,12,7,4}));
 f.message=1;f.run();assert(f.state()==4 && (f.calls==std::vector<uint32_t>{8,11,7,9,10}));f.message=0;f.reaction=1;f.run();assert(f.state()==6 && (f.calls==std::vector<uint32_t>{7,9,10,13,14,15,16}));f.reaction=0;f.slot=1;f.run();assert(f.state()==6 && (f.calls==std::vector<uint32_t>{16,8,17}));f.slot=0;f.run();assert(f.state()==10 && (f.calls==std::vector<uint32_t>{16,8,17,9,10,7}));f.run();assert(f.state()==12);f.ready=false;f.run();assert(f.state()==13 && (f.calls==std::vector<uint32_t>{19,7}));f.ready=true;f.run();assert(f.state()==14 && f.w(0x1004)==2 && f.w(0x1f14)==0x03000000);
 Fixture suppressed;suppressed.w(0x1020,0x23000);suppressed.w(0x23008,224);suppressed.w(0x232b8,0x1234);suppressed.w(0x26004,0x100000);suppressed.run();assert(suppressed.entries.size()==1 && suppressed.calls==std::vector<uint32_t>{0x80416d70});
 Fixture other;other.w(0x25008,1);assert(!packedBattleFirstAct(0x1000,other.services()) && other.state()==0 && other.entries.empty());
 Fixture end;end.state(3);end.done=true;end.run();assert(end.w(0x1004)==3 && end.w(0x1f24)==0x07000000 && (end.calls==std::vector<uint32_t>{8,15}));assert(end.w(0x25010)==0);
 Fixture wall;wall.state(8);wall.ready=true;wall.w(0x26004,0x10);wall.run();assert(wall.state()==9 && (wall.calls==std::vector<uint32_t>{7,18}));wall.w(0x192c0,91);wall.live.insert(91);wall.run();assert(wall.state()==9);wall.live.clear();wall.run();assert(wall.state()==10 && wall.w(0x192c0)==0);
 Fixture luck;luck.state(10);luck.w(0x1020,0x23000);assert(luck.vm.memory.writeByte(0x23304,1));luck.run();assert(luck.state()==11 && luck.entries[0].owner==-5 && luck.w(0x1a05c)==101 && luck.w(0x19ff4)==0xffffffff);luck.live.insert(101);luck.run();assert(luck.state()==11 && (luck.calls==std::vector<uint32_t>{101,7,10,9}));luck.live.clear();luck.ready=true;luck.run();assert(luck.state()==12 && luck.w(0x1a05c)==0);luck.w(0x1ef4,2);luck.run();assert(luck.state()==14 && luck.w(0x1004)==0);luck.run();assert(luck.w(0x1004)==2 && !(luck.w(0x1ef4)&2));
 std::cout<<"Original first-action entry ownership, fade/event gates, status/reaction ordering, wall/luck and Turn handoff passed\n";
}

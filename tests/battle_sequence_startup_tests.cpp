#include "battle_sequence_startup.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<uint32_t> calls;std::set<uint32_t> live;int32_t blockedPaper=0;bool nice=false,swapLanguage=false;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x14000)));}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 BattleStartupServices services(){BattleStartupServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
 s.mario=[](void* p){static_cast<Fixture*>(p)->calls.push_back(1);return uint32_t(0x11000);};s.marioPartyId=[](void* p)->int32_t{static_cast<Fixture*>(p)->calls.push_back(2);return 1;};s.party=[](void* p,int32_t id){assert(id==1);static_cast<Fixture*>(p)->calls.push_back(3);return uint32_t(0x12000);};
 s.unitSetup=[](void* p,uint32_t b){auto& f=*static_cast<Fixture*>(p);assert(b==0x1000 && f.w(0x11004)==0x81 && f.w(0x12004)==0x41 && f.w(0x1f0c)==0x01000001);f.calls.push_back(4);};
 s.eventExists=[](void* p,uint32_t id){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(id);return f.live.count(id)!=0;};
 s.paperReady=[](void* p,uint32_t key,int32_t context){auto& f=*static_cast<Fixture*>(p);assert(context==1);f.calls.push_back(key);return int32_t(key)!=f.blockedPaper;};s.niceReady=[](void* p,int32_t mode){auto& f=*static_cast<Fixture*>(p);assert(mode==1);f.calls.push_back(5);return f.nice;};
 s.statusEventOn=[](void* p){static_cast<Fixture*>(p)->calls.push_back(6);};s.statusSystemOn=[](void* p){static_cast<Fixture*>(p)->calls.push_back(7);};return s;
 }
 BattleSequenceManagerServices manager(){BattleSequenceManagerServices s;s.memory=nativeBattleActorServices(vm).memory;s.user=this;s.init=[](void* p,uint32_t b){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(8);f.w(b+4,2);};s.firstAct=[](void* p,uint32_t){static_cast<Fixture*>(p)->calls.push_back(9);};s.turn=[](void* p,uint32_t){static_cast<Fixture*>(p)->calls.push_back(10);};s.end=[](void* p,uint32_t){static_cast<Fixture*>(p)->calls.push_back(11);};s.pinch=[](void* p,uint32_t,int32_t mode){static_cast<Fixture*>(p)->calls.push_back(20+mode);};return s;}
 BattleNiceAdmissionServices admission(){BattleNiceAdmissionServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.animationReady=[](void* p,uint32_t key,int32_t mode,int32_t zero){auto& f=*static_cast<Fixture*>(p);assert(mode==1 && zero==0);f.calls.push_back(key);if(f.swapLanguage)f.w(0x8100016c,1);return f.nice;};return s;}
};
int main(int argc,char** argv){
 Fixture f;auto s=f.services();f.w(0x1f0c,0x01000000);f.w(0x11004,0x80);f.w(0x12004,0x40);packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000002 && (f.calls==std::vector<uint32_t>{1,2,3,4}));
 f.calls.clear();f.w(0x1020,0x13000);f.w(0x1024,0x14000);f.w(0x13288,90);f.w(0x14288,91);f.live.insert(90);packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000002 && f.w(0x14288)==0 && f.w(0x13288)==90 && (f.calls==std::vector<uint32_t>{90,91}));f.live.clear();packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000003 && f.w(0x13288)==0);
 f.calls.clear();f.blockedPaper=0x802efbec;packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000003 && (f.calls==std::vector<uint32_t>{0x802efbe4,0x802efbec}));f.blockedPaper=0;f.calls.clear();packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000003 && (f.calls==std::vector<uint32_t>{0x802efbe4,0x802efbec,0x802efbf8,5}));f.nice=true;packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000004);f.calls.clear();packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000005 && f.w(0x1004)==0 && f.calls.empty());packedBattleSequenceInit(0x1000,s);assert(f.w(0x1f0c)==0x01000005 && f.w(0x1004)==1 && f.w(0x1f10)==0x02000000 && (f.calls==std::vector<uint32_t>{6,7}));
 const auto m=nativeBattleActorServices(f.vm).memory;f.w(0x1004,0xffffffff);packedBattleIncrementSequence(0x1000,0,m);assert(f.w(0x1004)==0);assert(packedBattleGetSequence(0x1000,8,m)==0);packedBattleSetSequence(0x1000,8,123,m);assert(f.w(0x1004)==0);
 f.calls.clear();packedBattleSequenceManager(0x1000,f.manager());assert((f.calls==std::vector<uint32_t>{8,21}));f.calls.clear();packedBattleSequenceManager(0x1000,f.manager());assert((f.calls==std::vector<uint32_t>{20,10}));f.w(0x1004,1);f.calls.clear();packedBattleSequenceManager(0x1000,f.manager());assert((f.calls==std::vector<uint32_t>{20,9}));f.w(0x1004,3);f.calls.clear();packedBattleSequenceManager(0x1000,f.manager());assert(f.calls==std::vector<uint32_t>{11});
 if(argc>1 && !std::strcmp(argv[1],"original")){Fixture original;EventScriptAssets assets;assert(assets.load("romfs/harbor-tutorial-events.pmevt") && original.vm.memory.importScripts(assets));original.w(0x8100016c,1);assert(!packedBattleNiceReady(1,original.admission()));assert((original.calls==std::vector<uint32_t>{0x802f3d00,0x802f3d0c,0x802f3d18,0x802f3d28,0x802f3d3c,0x802f3d50}));original.nice=true;original.calls.clear();assert(packedBattleNiceReady(1,original.admission()));original.calls.clear();original.w(0x8100016c,0);original.swapLanguage=true;assert(packedBattleNiceReady(1,original.admission()) && original.calls[0]==0x802f3cb0 && original.calls[1]==0x802f3d0c);std::cout<<"Original US grade/lucky six-resource AND and per-slot language reload passed\n";}
 std::cout<<"Source init fall-throughs, root cleanup, ordered readiness and captured sequence dispatch passed\n";
}

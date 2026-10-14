#include "field_battle_information.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<int32_t> draws,partyCalls;int32_t flagResult=0;bool highDraw=false;uint32_t partyPointer=0;std::function<void(Fixture&,int32_t)> onRandom;std::function<void(Fixture&,uint32_t,uint8_t)> onStoreByte;std::function<void(Fixture&,uint32_t,uint32_t)> onStoreWord;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x9000)));}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void h(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}void b(uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));}uint8_t b(uint32_t a){uint8_t v;assert(vm.memory.readByte(a,v));return v;}
 PackedNpcBattleSetupServices setup(){PackedNpcBattleSetupServices s;s.memory=nativeBattleActorServices(vm);s.memory.memory.user=this;s.memory.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->w(a);};s.memory.memory.half=[](void* p,uint32_t a){uint16_t v;assert(static_cast<Fixture*>(p)->vm.memory.readHalf(a,v));return v;};s.memory.memory.storeWord=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.w(a,v);if(f.onStoreWord)f.onStoreWord(f,a,v);};s.memory.memory.storeByte=[](void* p,uint32_t a,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.b(a,v);if(f.onStoreByte)f.onStoreByte(f,a,v);};s.user=this;s.random=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.draws.push_back(bound);assert(bound>0);if(f.onRandom)f.onRandom(f,bound);return f.highDraw?bound-1:0;};s.savedFlag=[](void* p,int32_t index){auto& f=*static_cast<Fixture*>(p);assert(index==0);return f.flagResult;};return s;}
 PackedFieldBattleInformationServices information(){PackedFieldBattleInformationServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.marioPartyId=[](void* p){static_cast<Fixture*>(p)->partyCalls.push_back(1);return 3;};s.party=[](void* p,int32_t id){assert(id==3);auto& f=*static_cast<Fixture*>(p);f.partyCalls.push_back(2);return f.partyPointer;};s.partyKind=[](void* p,int32_t id){assert(id==2);static_cast<Fixture*>(p)->partyCalls.push_back(3);return 225;};return s;}
 void config(){w(0x5000,1);w(0x500c,0xffffffff);w(0x5014,0x6000);w(0x5018,0x20000010);w(0x5040,0x7777);w(0x6000,50);w(0x6004,0x7000);w(0x6008,0x8888);w(0x7000,1);w(0x7004,0x7100);w(0x7100,0x7200);b(0x7212,9);}
};
int main(int argc,char** argv){
 Fixture empty;empty.w(0x1000,0x12);empty.b(0x1316,55);empty.w(0x1230,0xffffffff);setupPackedNpcBattleInformation(0x1000,0,empty.setup());assert(empty.w(0x1230)==0 && empty.w(0x1000)==0x12 && empty.b(0x1316)==55 && empty.draws.empty());
 Fixture f;f.config();f.w(0x1000,0x12);setupPackedNpcBattleInformation(0x1000,0x5000,f.setup());assert(f.draws.size()==17 && f.draws.back()==50);for(unsigned i=0;i<16;++i)assert(f.draws[i]==1);assert(f.w(0x1230)==0x8888 && f.w(0x1248)==0x7000 && f.w(0x1234)==0x20000010 && f.w(0x12f0)==0x7777 && f.w(0x1000)==0x10000012 && f.b(0x1316)==9);
 Fixture signedRange;signedRange.config();signedRange.highDraw=true;signedRange.b(0x5020,255);signedRange.b(0x5021,1);setupPackedNpcBattleInformation(0x1000,0x5000,signedRange.setup());assert(signedRange.draws[0]==3 && signedRange.b(0x12e0)==1);
 Fixture item;item.config();item.w(0x501c,1);item.w(0x712c,0x7300);item.w(0x7300,9);item.h(0x7304,0xffff);item.w(0x7308,10);item.h(0x730c,2);item.highDraw=true;setupPackedNpcBattleInformation(0x1000,0x5000,item.setup());assert((item.draws==std::vector<int32_t>{50,1}) && item.w(0x124c)==10 && item.w(0x126c)==0x7300);
 // RNG is an observable call: width uses the old signed range, addition reloads the unsigned first byte.
 Fixture audienceMutation;audienceMutation.config();audienceMutation.b(0x5020,255);audienceMutation.b(0x5021,1);audienceMutation.onRandom=[](Fixture& f,int32_t){if(f.draws.size()==1)f.b(0x5020,42);};setupPackedNpcBattleInformation(0x1000,0x5000,audienceMutation.setup());assert(audienceMutation.draws[0]==3 && audienceMutation.b(0x12e0)==42);
 // The final kind lookup follows the group currently installed after the byte clear.
 Fixture groupMutation;groupMutation.config();groupMutation.w(0x7404,0x7500);groupMutation.w(0x7500,0x7600);groupMutation.b(0x7612,27);groupMutation.onStoreByte=[](Fixture& f,uint32_t a,uint8_t v){if(a==0x1316 && v==0)f.w(0x1248,0x7400);};setupPackedNpcBattleInformation(0x1000,0x5000,groupMutation.setup());assert(groupMutation.b(0x1316)==27 && groupMutation.draws.size()==17);
 Fixture clearedGroup;clearedGroup.config();clearedGroup.onStoreByte=[](Fixture& f,uint32_t a,uint8_t v){if(a==0x1316 && v==0)f.w(0x1248,0);};setupPackedNpcBattleInformation(0x1000,0x5000,clearedGroup.setup());assert(clearedGroup.b(0x1316)==0);
 // Mutable source registry uses first exact name match and original +8/+c fields.
 Fixture area;area.config();area.w(0x4008,0x4100);area.w(0x400c,0x4120);area.w(0x4010,0x4140);area.w(0x4100,0x4200);area.w(0x4120,0x4210);area.w(0x4140,0x4220);
 const char* names[]={"Gor","gor","gor"};for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j)area.b(0x4200+16*i+j,uint8_t(names[i][j]));
 auto areaServices=area.setup();assert(packedBattleAreaData("gor",areaServices.memory,0x4000)==0x4120 && packedBattleAreaData("none",areaServices.memory,0x4000)==0);
 registerPackedBattleArea("gor",0x5000,0x7770,areaServices.memory,0x4000);assert(area.w(0x4128)==0x5000 && area.w(0x412c)==0x7770 && area.w(0x4148)==0);registerPackedBattleArea("missing",1,2,areaServices.memory,0x4000);
 area.w(0x80415fd0,0x3000);area.b(0x312c,'g');area.b(0x312d,'o');area.b(0x312e,'r');
 area.w(0x1330,0x2000);area.b(0x2316,99);setPackedNpcBattleInformation(0x1000,0,areaServices,0x4000);assert(area.draws.size()==17 && area.w(0x2234)==0x20000010 && area.b(0x2316)==99);for(unsigned i=0;i<200;i+=4)assert(area.w(0x1230+i)==area.w(0x2230+i));
 // Positive indices select 0x44-byte rows; other negatives use row zero.
 area.w(0x5044,1);area.w(0x5060,1);area.w(0x5050,0xffffffff);area.w(0x505c,0x12345678);setPackedNpcBattleInformation(0x1000,1,areaServices,0x4000);assert(area.w(0x1234)==0x12345678 && area.draws.size()==17);setPackedNpcBattleInformation(0x1000,-2,areaServices,0x4000);assert(area.w(0x1234)==0x20000010 && area.draws.size()==34);
 // Copy reads each associated slot when reached, and loads both words before stores.
 area.w(0x1334,0);area.onStoreWord=[](Fixture& f,uint32_t a,uint32_t){if(a==0x2230)f.w(0x1334,0x2800);};setPackedNpcBattleInformation(0x1000,0,areaServices,0x4000);assert(area.w(0x2a30)==0x8888 && area.w(0x2a34)==0x20000010);
 area.onStoreWord={};area.w(0x1330,0x1004);area.w(0x1334,0);setPackedNpcBattleInformation(0x1000,0,areaServices,0x4000);assert(area.w(0x1234)==0x8888 && area.w(0x1238)==0x20000010);
 // -1 never reads GP/registry, but still copies the cleared information block.
 Fixture noArea;noArea.w(0x1330,0x2000);noArea.b(0x1316,77);noArea.b(0x2316,88);setPackedNpcBattleInformation(0x1000,-1,noArea.setup());assert(noArea.w(0x2230)==0 && noArea.b(0x1316)==77 && noArea.b(0x2316)==88 && noArea.draws.empty());
 Fixture info;info.w(0x3010,0x800000);info.b(0x12f4,60);info.b(0x12f5,61);info.b(0x12f6,62);assert(preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,info.information())==0x3020);assert(info.w(0x3020)==0 && info.w(0x3024)==0 && info.w(0x3028)==7 && info.w(0x302c)==0x1230 && info.w(0x3030)==0 && info.b(0x3038)==60 && info.b(0x3039)==0 && info.b(0x303a)==61 && info.b(0x303b)==62);assert((info.partyCalls==std::vector<int32_t>{1,2}));
 info.partyCalls.clear();info.partyPointer=0x6000;info.b(0x6031,2);info.w(0x3010,0x10000000);info.w(0x3014,8);preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,info.information());assert(info.w(0x3024)==225 && info.w(0x3028)==8 && (info.partyCalls==std::vector<int32_t>{1,2,3}));info.h(0x401c,2);info.w(0x301c,1);preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,info.information());assert(info.w(0x3024)==0 && info.w(0x3028)==0);
 if(argc>1 && !std::strcmp(argv[1],"original")){Fixture original;EventScriptAssets assets;assert(assets.load("romfs/harbor-tutorial-events.pmevt") && original.vm.memory.importScripts(assets));const auto configuration=0x95540e30;assert(original.w(configuration+0x18)==0x20000010);setupPackedNpcBattleInformation(0x1000,configuration,original.setup());assert(original.draws.size()==17 && original.draws.back()==50 && original.w(0x1230)==0x95540870 && original.w(0x1248)==0x95540c08 && original.w(0x1234)==0x20000010 && original.w(0x124c)==0 && original.b(0x1316)==0);preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,original.information());assert(original.w(0x3020)==0 && original.w(0x3024)==0 && original.w(0x3028)==0 && original.w(0x302c)==0x1230 && original.w(0x3038)==0);std::cout<<"Original setup2 requests17 draws, actual stage/group, no item draw, firstAttack0 and source information pointers passed\n";}
 std::cout<<"Packed NPC setup, signed audience/item lotteries and original field information constructor passed\n";
}

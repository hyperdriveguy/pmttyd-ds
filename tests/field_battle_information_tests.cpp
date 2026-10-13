#include "field_battle_information.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<int32_t> draws,partyCalls;int32_t flagResult=0;bool highDraw=false;uint32_t partyPointer=0;
 Fixture(){assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(0x9000)));}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void h(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}void b(uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));}uint8_t b(uint32_t a){uint8_t v;assert(vm.memory.readByte(a,v));return v;}
 PackedNpcBattleSetupServices setup(){PackedNpcBattleSetupServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.random=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.draws.push_back(bound);assert(bound>0);return f.highDraw?bound-1:0;};s.savedFlag=[](void* p,int32_t index){auto& f=*static_cast<Fixture*>(p);assert(index==0);return f.flagResult;};return s;}
 PackedFieldBattleInformationServices information(){PackedFieldBattleInformationServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.marioPartyId=[](void* p){static_cast<Fixture*>(p)->partyCalls.push_back(1);return 3;};s.party=[](void* p,int32_t id){assert(id==3);auto& f=*static_cast<Fixture*>(p);f.partyCalls.push_back(2);return f.partyPointer;};s.partyKind=[](void* p,int32_t id){assert(id==2);static_cast<Fixture*>(p)->partyCalls.push_back(3);return 225;};return s;}
 void config(){w(0x5000,1);w(0x500c,0xffffffff);w(0x5014,0x6000);w(0x5018,0x20000010);w(0x5040,0x7777);w(0x6000,50);w(0x6004,0x7000);w(0x6008,0x8888);w(0x7000,1);w(0x7004,0x7100);w(0x7100,0x7200);b(0x7212,9);}
};
int main(int argc,char** argv){
 Fixture empty;empty.w(0x1000,0x12);empty.b(0x1316,55);empty.w(0x1230,0xffffffff);setupPackedNpcBattleInformation(0x1000,0,empty.setup());assert(empty.w(0x1230)==0 && empty.w(0x1000)==0x12 && empty.b(0x1316)==55 && empty.draws.empty());
 Fixture f;f.config();f.w(0x1000,0x12);setupPackedNpcBattleInformation(0x1000,0x5000,f.setup());assert(f.draws.size()==17 && f.draws.back()==50);for(unsigned i=0;i<16;++i)assert(f.draws[i]==1);assert(f.w(0x1230)==0x8888 && f.w(0x1248)==0x7000 && f.w(0x1234)==0 && f.w(0x12f0)==0x7777 && f.w(0x1000)==0x10000012 && f.b(0x1316)==9);
 Fixture signedRange;signedRange.config();signedRange.highDraw=true;signedRange.b(0x5020,255);signedRange.b(0x5021,1);setupPackedNpcBattleInformation(0x1000,0x5000,signedRange.setup());assert(signedRange.draws[0]==3 && signedRange.b(0x12e0)==1);
 Fixture item;item.config();item.w(0x501c,1);item.w(0x712c,0x7300);item.w(0x7300,9);item.h(0x7304,0xffff);item.w(0x7308,10);item.h(0x730c,2);item.highDraw=true;setupPackedNpcBattleInformation(0x1000,0x5000,item.setup());assert((item.draws==std::vector<int32_t>{50,1}) && item.w(0x124c)==10 && item.w(0x126c)==0x7300);
 Fixture info;info.w(0x3010,0x800000);info.b(0x12f4,60);info.b(0x12f5,61);info.b(0x12f6,62);assert(preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,info.information())==0x3020);assert(info.w(0x3020)==0 && info.w(0x3024)==0 && info.w(0x3028)==7 && info.w(0x302c)==0x1230 && info.w(0x3030)==0 && info.b(0x3038)==60 && info.b(0x3039)==0 && info.b(0x303a)==61 && info.b(0x303b)==62);assert((info.partyCalls==std::vector<int32_t>{1,2}));
 info.partyCalls.clear();info.partyPointer=0x6000;info.b(0x6031,2);info.w(0x3010,0x10000000);info.w(0x3014,8);preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,info.information());assert(info.w(0x3024)==225 && info.w(0x3028)==8 && (info.partyCalls==std::vector<int32_t>{1,2,3}));info.h(0x401c,2);info.w(0x301c,1);preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,info.information());assert(info.w(0x3024)==0 && info.w(0x3028)==0);
 if(argc>1 && !std::strcmp(argv[1],"original")){Fixture original;EventScriptAssets assets;assert(assets.load("romfs/harbor-tutorial-events.pmevt") && original.vm.memory.importScripts(assets));const auto configuration=0x95540e30;assert(original.w(configuration+0x18)==0x20000010);setupPackedNpcBattleInformation(0x1000,configuration,original.setup());assert(original.draws.size()==17 && original.draws.back()==50 && original.w(0x1230)==0x95540870 && original.w(0x1248)==0x95540c08 && original.w(0x1234)==0 && original.w(0x124c)==0 && original.b(0x1316)==0);preparePackedFieldBattleInformation(0x3000,0x1000,0x4000,original.information());assert(original.w(0x3020)==0 && original.w(0x3024)==0 && original.w(0x3028)==0 && original.w(0x302c)==0x1230 && original.w(0x3038)==0);std::cout<<"Original setup2 requests17 draws, actual stage/group, no item draw, firstAttack0 and source information pointers passed\n";}
 std::cout<<"Packed NPC setup, signed audience/item lotteries and original field information constructor passed\n";
}

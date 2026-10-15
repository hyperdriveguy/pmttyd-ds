#include "battle_sequence_entry.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    uint32_t gp=0x81000000,f=0x85000000,n=0x86000000,d=0x87000000,pouch=0x88000000,hold=0;
    std::map<uint32_t,uint32_t> words;
    std::vector<BattleSequenceCall> calls;
    std::vector<uint32_t> reads;
    std::vector<uint32_t> stories{11,11};unsigned storyIndex=0;
    uint32_t async=1,fade=1,end=0,initResult=0,cameraCalls=0;
    bool missingField=false;
    std::function<void(Fixture&,BattleSequenceCall&)> hook;
    OriginalSequenceWork work{};
    Fixture(){words[gp+0x168]=f;words[f+8]=n;words[n+0x230]=d;words[n+0x2f0]=0x12345678;words[d+4]=7;words[0x804203b0]=words[0x804203b4]=255;words[0x804203c4]=0;words[0x804203c8]=0x43c80000;words[0x804203cc]=0x43480000;words[0x8041eb90]=0x89000000;words[0x8a000114]=0x42280000;}
    BattleSequenceEntryServices services(){
        BattleSequenceEntryServices s;s.user=this;
        s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(a==0x80415fd0)return f.gp;if(f.missingField && a==0x81000168)return {};return f.words[a];};
        s.half=[](void* p,uint32_t a)->std::optional<uint16_t>{return uint16_t(static_cast<Fixture*>(p)->words[a]);};
        s.store=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;return true;};
        s.call=[](void* p,BattleSequenceCall& c){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(c);if(c.function==f.hold)return false;
            switch(c.function){case 0x80031260:c.result=f.stories.at(f.storyIndex++);break;case 0x80007388:c.result=0x90000000+uint32_t(f.calls.size());break;case 0x8018a0e8:c.result=f.async;break;case 0x80010d94:c.result=f.fade;break;case 0x800d5eb0:c.result=f.pouch;break;case 0x800738ac:c.result=f.initResult;break;case 0x800f903c:c.result=f.end;break;case 0x8005e0c0:c.result=0x8b000000;break;case 0x8000f2fc:++f.cameraCalls;c.result=0x8a000000;break;default:break;}
            if(f.hook)f.hook(f,c);
            return true;};return s;
    }
    unsigned count(uint32_t fn)const{unsigned n=0;for(const auto& c:calls)n+=c.function==fn;return n;}
};
int main(){
    {Fixture f;BattleSequenceEntryCursor c;f.missingField=true;
     assert(!initializeBattleSequenceEntry(f.services(),c) && c.getterCaptured);f.gp=0x82000000;f.words[f.gp+0x168]=0x85100000;f.missingField=false;
     assert(initializeBattleSequenceEntry(f.services(),c) && c.npc==f.n);}
    for(auto values:std::vector<std::vector<uint32_t>>{{109,110},{11,402},{110,111,402},{11,11}}){Fixture f;f.stories=values;BattleSequenceEntryCursor c;
     assert(initializeBattleSequenceEntry(f.services(),c));assert(f.storyIndex==values.size());assert(f.count(0x800daefc)==unsigned(values.back()==11));
     assert(f.count(0x800d76b8)==1 && f.count(0x80035c68)==1 && f.count(0x800dc110)==1);}
    {Fixture f;BattleSequenceEntryCursor c;f.hold=0x800daefc;
     assert(!initializeBattleSequenceEntry(f.services(),c) && c.pending==f.hold && f.storyIndex==2);
     f.words[f.f+8]=f.n+0x1000;f.words[f.n+0x2f0]=99;f.words[f.f+0x10]=0x800000;f.hold=0;
     assert(initializeBattleSequenceEntry(f.services(),c) && f.storyIndex==2 && f.count(0x8005c2c4)==1 && f.count(0x800bed44)==1 && f.count(0x80005f28)==2);
     for(auto& call:f.calls)if(call.function==0x800db7a0)assert(call.args[1]==99);}
    {Fixture f;BattleSequenceEntryCursor c;f.hold=0x80013368;
     assert(!updateBattleSequenceEntry(f.work,f.services(),c));f.words[0x804203b0]=0;f.work[1]=2;f.hold=0;
     assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==3 && f.calls.back().rgba==255 && f.work[4]==0);}
    {Fixture f;BattleSequenceEntryCursor c;f.work[1]=1;f.async=0;
     assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.work[4]==0 && f.count(0x80007388)==1);
     f.async=1;f.hold=0x80010d94;assert(!updateBattleSequenceEntry(f.work,f.services(),c) && f.work[4]==1 && f.count(0x80007388)==3);
     f.hold=0;assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==2 && f.work[4]==1 && f.count(0x80007388)==3);
     for(auto& a:f.calls)if(a.function==0x8018a0e8)assert(a.args[0]==4 && a.args[1]==0 && a.args[2]==0x804203bc);}
    for(uint32_t type:{0u,0x800000u,0x1000000u}){Fixture f;BattleSequenceEntryCursor c;f.work[1]=2;f.words[f.f+0x10]=type;f.hold=0x800738ac;f.words[0x8b000000]=0x80000003;
     f.hook=[](Fixture& f,BattleSequenceCall& a){if(a.function==0x8005e0c0)f.words[f.f+0x10]=0;};
     assert(!updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==4 && c.pending==0x800738ac);
     assert(f.count(0x80005f28)==unsigned(type==0) && f.count(0x8005c2c4)==unsigned(type==0));assert(f.words[0x8b000000]==0x80000002);}
    {Fixture f;BattleSequenceEntryCursor c;f.work[1]=3;f.words[f.f+0x550]=f.words[f.f+0x554]=1;f.words[f.f+0x558]=2;f.words[f.f+0x570]=198;
     assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==3 && f.words[f.f+0x570]==199 && f.count(0x8002892c)==0);
     f.hold=0x800738ac;assert(!updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==4 && f.words[f.f+0x570]==200 && f.count(0x8002892c)==1);}
    for(uint32_t rank:{0xffffu,0u,1u,2u,3u}){Fixture f;BattleSequenceEntryCursor c;f.work[1]=3;f.words[f.pouch+0x88]=rank;f.hold=0x800738ac;
     f.hook=[](Fixture& f,BattleSequenceCall& a){if(a.function==0x800d5eb0){f.words[f.n+0x230]=0xdead0000;f.words[f.d+4]=99;}};
     assert(!updateBattleSequenceEntry(f.work,f.services(),c));for(auto& a:f.calls)if(a.function==0x8002892c)assert(a.args[0]==(rank<=2?0x802c2ac4+rank*8:0x802c2adc) && a.args[1]==99);}
    {Fixture f;BattleSequenceEntryCursor c;f.work[1]=5;f.end=1;f.words[0x89000ef4]=0x100;f.hold=0x800dc380;
     assert(!updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==6 && f.work[4]==0 && f.cameraCalls==2);
     f.hold=0;assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.count(0x800f903c)==1 && f.work[4]==0 && f.calls.back().position[2]==600 && f.calls.back().yaw==42);
     for(unsigned i=0;i<30;++i)assert(updateBattleSequenceEntry(f.work,f.services(),c));
     assert(f.count(0x8002e218)==0);
     assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.work[4]==31 && f.calls.back().args[0]==2);}
    {Fixture f;BattleSequenceEntryCursor c;f.work[1]=10;assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.work[1]==11 && f.calls.back().args[1]==200);
     f.words[f.gp+0xc]=9;assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.words[f.gp+0xc]==0 && f.calls.back().args[0]==5);}
    for(uint32_t state:{7u,8u,9u,12u,0xffffffffu}){Fixture f;BattleSequenceEntryCursor c;f.work[1]=state;assert(updateBattleSequenceEntry(f.work,f.services(),c) && f.calls.empty());}
}

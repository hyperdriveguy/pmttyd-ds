#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
using namespace ttyd;
uint32_t bits(float value) {uint32_t word;std::memcpy(&word,&value,4);return word;}
int32_t half(const BattleCandidateBuffer& b,unsigned o) {const uint16_t v=uint16_t(b.bytes[o])<<8|b.bytes[o+1];return v<32768?v:int32_t(v)-65536;}
uint32_t word(const BattleCandidateBuffer& b,unsigned o) {return uint32_t(b.bytes[o])<<24|uint32_t(b.bytes[o+1])<<16|uint32_t(b.bytes[o+2])<<8|b.bytes[o+3];}
struct Fixture {
    BattleCandidateBuffer buffer;std::vector<float> fmaScales;std::function<void()> onFma;int fmas=0;
    static EventNativePlatform platform(Fixture* f) {
        EventNativePlatform p;p.user=f;p.truncate=[](void*,float v)->int32_t {return int32_t(v);};p.signedWordFloat=[](void*,int32_t v) {return float(v);};p.multiplyFloat=[](void*,float a,float b) {return a*b;};p.addFloat=[](void*,float a,float b) {return a+b;};p.subtractFloat=[](void*,float a,float b) {return a-b;};p.divideFloat=[](void*,float a,float b) {return a/b;};p.fmaFloat=[](void* p,float a,float b,float c) {auto& f=*static_cast<Fixture*>(p);++f.fmas;f.fmaScales.push_back(a);if(f.onFma)f.onFma();return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c) {return -std::fma(a,b,-c);};return p;
    }
    EventNativeRuntime runtime{0x81000000,platform(this)};
    Fixture() {
        for(uint32_t a:{0x8000u,0x10000u,0x12000u,0x13000u,0x15000u,0x16000u,0x18000u,0x19000u})assert(runtime.memory.registerRam(a,std::vector<uint8_t>(0x400)));
        for(uint32_t a:{0x11000u,0x14000u,0x17000u})assert(runtime.memory.registerRam(a,std::vector<uint8_t>(0x500)));
        assert(runtime.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));w(0x8041eb90,0x8000);
        for(int i=0;i<3;++i) {const uint32_t u=0x10000+0x3000*i,p=u+0x1000,k=u+0x2000;w(0x8020+4*i,u);w(u,i);w(u+8,i==0?222:10);w(u+0x10,k);w(u+0x14,p);w(u+0x114,bits(2));w(u+0x104,2);b(u+0xc,i==0?0:1);b(k+0xb4,i==1?1:0);w(p+4,0x19000+4*i);w(0x19000+4*i,i+1);w(p+0x1ac,0x10000001);w(p+0x4ec,0xdeadbeef);b(p+0xbd,1);w(p+0x18,bits(i==2?100:20));w(p+0x1c,bits(10));w(p+0x20,bits(0));}
        w(0x10034,bits(7));w(0x1003c,bits(0));b(0x8012,1);b(0x800a,255);
        assert(runtime.memory.registerRam(0x804221f0,std::vector<uint8_t>(32)));const float constants[]={1.5f,.1f,.4f,.5f,2,1,.05f,5};for(unsigned i=0;i<8;++i)w(0x804221f0+4*i,bits(constants[i]));
        assert(runtime.memory.registerRam(0x80422668,std::vector<uint8_t>(12)));w(0x80422668,bits(-800));w(0x8042266c,bits(800));w(0x80422670,bits(40));assert(runtime.memory.registerRam(0x804226a8,std::vector<uint8_t>(4)));w(0x804226a8,bits(1));buffer.bytes.fill(0xaa);
    }
    void w(uint32_t a,uint32_t v) {assert(runtime.memory.writeWord(a,v));}
    void b(uint32_t a,uint8_t v) {assert(runtime.memory.writeByte(a,v));}
    void h(uint32_t a,uint16_t v) {assert(runtime.memory.writeHalf(a,v));}
    BattlePackedSamplingServices services() {return nativeBattleSamplingServices(runtime);}
    int32_t run(uint32_t admission=0,uint32_t attributes=0,int32_t direction=1,int32_t actor=0,int32_t belong=1) {return samplePackedBattleCandidates(privateBattleCandidateRecords(buffer),0,actor,belong,admission,attributes,direction,services());}
};
int main() {
    Fixture geometry;for(unsigned i=0;i<6;++i)geometry.w(0x14188+4*i,bits(float(3+i)));geometry.h(0x141a0,2);assert(geometry.run()==0 && geometry.buffer.bytes[0xa6c]==1);assert(half(geometry.buffer,4)==1 && half(geometry.buffer,6)==2 && half(geometry.buffer,8)==38 && half(geometry.buffer,10)==32 && half(geometry.buffer,12)==26);assert(word(geometry.buffer,4+0xc)==22 && word(geometry.buffer,4+0x10)==10 && word(geometry.buffer,4+0x14)==5 && word(geometry.buffer,4+0x18)==2);assert(geometry.buffer.bytes[4+0xa]==0xaa && geometry.buffer.bytes[4+0x1e]==0xaa && word(geometry.buffer,4+0x20)==0xffffffff && geometry.buffer.bytes[0xab7]==0);
    Fixture high;assert(high.run(0x80000000)==0 && high.buffer.bytes[0xa6c]==255 && high.buffer.bytes[0xab7]==0xaa && high.buffer.bytes[4]==0xaa);
    for(uint32_t a:{0x100u,0x200u}) {Fixture flip;flip.w(0x1027c,0x10);assert(flip.run(a)==1 && word(flip.buffer,0xac0)==(a^0x300));}
    Fixture both;both.w(0x1027c,0x10);assert(both.run(0x300)==0 && word(both.buffer,0xac0)==0x300);
    struct Admission {uint32_t mask;int32_t kind;uint32_t partFlags;int actor,belong;};
    for(auto t:std::vector<Admission>{{0x10000,10,1,0,1},{0x20000,10,1,0,1},{1,222,1,0,1},{2,225,1,0,1},{0x10,10,1,0,1},{0x40,218,1,0,1},{0x20,216,1,0,1},{0x1000,10,1,1,1},{0x100,10,1,0,1},{0x200,10,1,0,0},{0x2000,222,1,0,1},{0x4000,10,1,0,1},{0x100000,10,0x40001,0,1},{0x200000,10,0x11,0,1}}) {Fixture f;f.w(0x13008,uint32_t(t.kind));f.w(0x141ac,0x10000000|t.partFlags);f.run(t.mask,0,1,t.actor,t.belong);assert(f.buffer.bytes[0xa6c]==0);}
    struct Attribute {uint32_t mask,unitFlags,partFlags;};
    for(auto t:std::vector<Attribute>{{1,2,0x80001},{2,4,1},{4,2,1},{8,4,1},{0x10,0,1},{0x1000,2,0x100001},{0x2000,2,0x200001},{0x4000,2,0x400001},{0x8000,2,1},{0x2000000,0x20,1},{0x4000000,2,0x2000001}}) {Fixture f;f.w(0x13104,t.unitFlags);f.w(0x141ac,0x10000000|t.partFlags);f.run(0,t.mask);assert(f.buffer.bytes[0xa6c]==0);}
    Fixture self;self.w(0x141ac,0x10200001);self.run(0,0x102000,1,1);assert(self.buffer.bytes[0xa6c]==1);
    Fixture mono;auto ms=mono.services();int monoCalls=0;ms.user=&monoCalls;ms.monosiri=[](void* p,int32_t kind)->uint32_t {assert(kind==10);++*static_cast<int*>(p);return 0x19000;};mono.w(0x19004,1);samplePackedBattleCandidates(privateBattleCandidateRecords(mono.buffer),0,0,1,0,1,1,ms);assert(monoCalls==2 && mono.buffer.bytes[0xa6c]==1);
    Fixture nullMono;auto ns=nullMono.services();ns.monosiri=[](void*,int32_t)->uint32_t {return 0;};samplePackedBattleCandidates(privateBattleCandidateRecords(nullMono.buffer),0,0,1,0,1,1,ns);assert(nullMono.buffer.bytes[0xa6c]==0);
    Fixture status27;status27.b(0x13135,1);status27.run();assert(status27.buffer.bytes[0xa6c]==0);
    Fixture hidden;hidden.w(0x13104,0x42);hidden.run();assert(hidden.buffer.bytes[0xa6c]==0);
    Fixture hiddenPart;hiddenPart.w(0x141ac,0x10010001);hiddenPart.run();assert(hiddenPart.buffer.bytes[0xa6c]==0);
    Fixture compact;compact.b(0x180b4,1);compact.w(0x13008,218);compact.run(0x40);assert(compact.buffer.bytes[0xa6c]==1 && half(compact.buffer,4)==2 && compact.buffer.bytes[4+0xa]==0xaa);
    for(int direction:{-1,1}) {Fixture occlusion;occlusion.b(0x180b4,1);occlusion.run(0,0x1000000,direction);assert(occlusion.buffer.bytes[0xa6c]==1 && half(occlusion.buffer,4)==(direction>0?1:2));}
    Fixture lane;lane.b(0x180b4,1);lane.w(0x17020,bits(-100));lane.run(0,0x1000000);assert(lane.buffer.bytes[0xa6c]==2);
    Fixture bypass;bypass.b(0x180b4,1);bypass.w(0x171ac,0x10000041);bypass.run(0,0x1000000);assert(bypass.buffer.bytes[0xa6c]==2);
    for(int direction:{0,1,-1}) {Fixture ties;ties.b(0x180b4,1);ties.w(0x17018,bits(20));ties.run(0,0,direction);assert(ties.buffer.bytes[0xa6c]==2 && ties.buffer.bytes[0xa6d]==(direction>0?0:1));}
    Fixture shadow;shadow.w(0x14000,0x17000);shadow.b(0x150b4,2);shadow.w(0x171ac,0x10000002);shadow.run(0x100000);assert(shadow.buffer.bytes[0xa6c]==1 && half(shadow.buffer,6)==2); // Same unit's body row suppresses flag2 second part.
    Fixture live;live.w(0x14188,bits(1));live.w(0x1418c,bits(1));live.w(0x14190,bits(1));live.onFma=[&] {if(live.fmas==1) {live.w(0x13114,bits(3));live.b(0x140bd,255);}};live.run();assert(half(live.buffer,8)==22 && half(live.buffer,10)==13 && half(live.buffer,12)==3); // Captured facing +1;X scale2 then live Y/Z scale3.
    Fixture raw;assert(raw.runtime.memory.registerRam(0x20000,std::vector<uint8_t>(0xacc,0xaa)));BattleCandidateRecords records{nativeBattleActorServices(raw.runtime),0x20000};samplePackedBattleCandidates(records,0,0,1,0,0,1,raw.services());uint8_t count;assert(raw.runtime.memory.readByte(0x20a6c,count) && count==1);uint16_t unit;assert(raw.runtime.memory.readHalf(0x20004,unit) && unit==1);
    Fixture full;assert(full.runtime.memory.registerRam(0x30000,std::vector<uint8_t>(24)));std::array<EventEntry,1> entries{};assert(full.runtime.entryAddresses.registerEntries(0x82000000,entries.data(),1));auto& e=entries[0];e.arguments=0x30000;e.owner=0;full.w(e.arguments,uint32_t(-2));full.w(e.arguments+4,1);full.w(e.arguments+8,999);for(unsigned i=0;i<3;++i)full.w(e.arguments+12+4*i,uint32_t(-30000000+int32_t(i)));BattleTakeoffBackend backend;backend.runtime=&full.runtime;auto services=nativeBattleTakeoffServices(full.runtime,nativeBattleTakeoffCoreServices(backend));EventCallbackRegistry registry;assert(bindBattleTakeoffCallback(registry,services));int32_t result;assert(registry.invoke(e,0x80109054,0,result) && result==2 && e.locals[0]==80 && e.locals[1]==7 && e.locals[2]==0);
    std::cout<<"packed sampling admission/geometry/order/default takeoff tests passed\n";
}

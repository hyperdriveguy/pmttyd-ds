#include "hit_n64_effect.hpp"
#include "effect_deletion.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Manager=0x1000,GP=0x2000,Pool=0x3000,Payload=0x4000;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
    std::map<uint32_t,uint8_t> bytes;
    HitN64Services s;
    unsigned heapCalls=0,randomCalls=0,depthCalls=0,displayCalls=0,deletes=0,frees=0,byteWrites=0;
    bool heapReady=true,randomReady=true,depthReady=true,displayReady=true,deleteReady=true;
    uint32_t allocatedBytes=0;std::vector<int32_t> draws;std::array<float,3> queuedPosition{};
    HitN64MainCursor main;EffectMainCursor poolMain;
    uint32_t word(uint32_t a){uint32_t n=0;for(unsigned i=0;i<4;++i)n=n<<8|bytes[a+i];return n;}
    void put(uint32_t a,uint32_t n){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(n>>(24-8*i));}
    float real(uint32_t a){auto n=word(a);float f;std::memcpy(&f,&n,4);return f;}
    Fixture(){
        put(0x80415ee0,Manager);put(0x80415fd0,GP);put(Manager,2);put(Manager+4,Pool);put(GP+0x14,1);put(Manager+0x14,0);put(GP+0x16c,0);
        put(Pool,2); // Exact nonzero free predicate skips this original record.
        for(unsigned i=0;i<4;++i)bytes[0x80419888+i]=13;
        const uint8_t anim[]={64,104,144,164,184,192,208,216,224,232,240,248,255};
        const uint8_t radius[]={10,20,28,35,42,48,53,57,60,62,63,64,65};
        for(unsigned i=0;i<13;++i){bytes[0x8041988c+i]=anim[i];bytes[0x8041989c+i]=radius[i];}
        put(0x802fb0b8,0x43300000);put(0x802fb0bc,0x80000000);put(0x802fb0c0,0x43300000);put(0x802fb0c4,0);
        put(0x80425468,0);put(0x8042547c,bits(1));put(0x80425478,bits(1.f/256));
        s.user=this;s.pool.user=this;s.pool.memory.user=this;
        s.pool.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.pool.memory.storeWord=[](void* p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->put(a,n);};
        s.pool.memory.storeByte=[](void* p,uint32_t a,uint8_t n){auto& f=*static_cast<Fixture*>(p);++f.byteWrites;f.bytes[a]=n;};
        s.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.pool.heapAllocate=[](void* p,int32_t heap,uint32_t bytes)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(heap==3);++f.heapCalls;f.allocatedBytes=bytes;if(!f.heapReady)return {};return Payload;};
        s.randomRaw=[](void* p)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.randomReady)return {};const auto i=f.randomCalls++;return i<f.draws.size()?f.draws[i]:int32_t(i*17);};
        s.depth=[](void* p,const std::array<float,3>& v)->std::optional<float>{auto& f=*static_cast<Fixture*>(p);++f.depthCalls;f.queuedPosition=v;if(!f.depthReady)return {};return 12.f;};
        s.display=[](void* p,int32_t camera,int32_t layer,uint32_t callback,uint32_t effect,float depth){auto& f=*static_cast<Fixture*>(p);++f.displayCalls;assert(camera==4 && layer==2 && callback==0x801c5d6c && effect==Pool+40 && depth==12);return f.displayReady;};
        s.effectDelete=[](void* p,uint32_t effect){auto& f=*static_cast<Fixture*>(p);++f.deletes;if(!f.deleteReady)return false;
            EffectDeletionServices del;del.user=&f;del.memory=f.s.pool.memory;
            del.freeHeap=[](void* p,int32_t heap,uint32_t payload){auto& f=*static_cast<Fixture*>(p);assert(heap==3 && payload==Payload);++f.frees;};
            hardDeleteEffect(effect,del);return true;};
        s.pool.update=[](void* p,uint32_t callback,uint32_t effect){auto& f=*static_cast<Fixture*>(p);assert(callback==0x801c61bc);return resumeHitN64Main(effect,f.s,f.main);};
    }
    uint32_t enter(int32_t kind=0,int32_t amount=3){HitN64EntryCursor c;const auto e=resumeHitN64Entry(kind,amount,{1,2,3},1,s,c);assert(e && !c.active());return *e;}
};
void entryAndHolds(){
    Fixture f;HitN64EntryCursor c;f.heapReady=false;
    assert(!resumeHitN64Entry(0,3,{1,2,3},1,f.s,c));assert(c.pending()==0x800300f0);
    assert(f.word(Pool)==2 && f.word(Pool+40)==1 && f.word(Pool+44)==1 && f.word(Pool+48)==12 && f.word(Pool+60)==0x802fb0c8);
    const auto writes=f.byteWrites;assert(!resumeHitN64Entry(0,3,{1,2,3},1,f.s,c));assert(f.byteWrites==writes); // No second effEntry.
    assert(!resumeHitN64Entry(0,4,{1,2,3},1,f.s,c));
    f.heapReady=true;f.randomReady=false;assert(!resumeHitN64Entry(0,3,{1,2,3},1,f.s,c));assert(c.pending()==0x8026c324);
    assert(f.word(Pool+52)==Payload && f.word(Pool+56)==0x801c61bc && f.real(Payload+8)==2);
    const auto heap=f.heapCalls;assert(!resumeHitN64Entry(0,3,{1,2,3},1,f.s,c));assert(f.heapCalls==heap);
    f.draws={-1,std::numeric_limits<int32_t>::min(),360};f.randomReady=true;
    auto changed=f.s;changed.display=nullptr;assert(!resumeHitN64Entry(0,3,{1,2,3},1,changed,c));
    const auto e=resumeHitN64Entry(0,3,{1,2,3},1,f.s,c);assert(e && *e==Pool+40 && f.randomCalls==11 && f.allocatedBytes==816);
    assert(f.word(Payload+0x30)==13 && f.word(Payload+0x34)==0 && f.real(Payload+0x28)==1);
    assert(f.real(Payload+0x44+0x1c)==32 && f.real(Payload+11*0x44+0x1c)==360);
    assert(f.real(Payload+0x44+0x24)==-1 && f.real(Payload+2*0x44+0x24)==-128 && f.real(Payload+3*0x44+0x24)==0);
    for(unsigned i=0;i<6;++i)assert(f.bytes[Payload+0x3c+i]==255);
}
void lifetime(){
    Fixture f;const auto e=f.enter();f.depthReady=false;
    assert(!resumeEffectMain(f.s.pool,f.poolMain));assert(f.main.pending()==0x8001086c && f.word(Payload+0x30)==12 && f.word(Payload+0x34)==1);
    assert(!resumeEffectMain(f.s.pool,f.poolMain));assert(f.word(Payload+0x30)==12 && f.word(Payload+0x34)==1);
    f.depthReady=true;f.displayReady=false;assert(!resumeEffectMain(f.s.pool,f.poolMain));assert(f.main.pending()==0x80010bc4);
    const auto depth=f.depthCalls;assert(!resumeEffectMain(f.s.pool,f.poolMain));assert(f.depthCalls==depth && f.word(Payload+0x34)==1);
    f.displayReady=true;assert(resumeEffectMain(f.s.pool,f.poolMain));assert((f.queuedPosition==std::array<float,3>{1,2,3}));
    assert(f.real(Payload+0x38)==.25f && f.real(Payload+0x44+4)==10);
    for(unsigned i=1;i<13;++i)assert(resumeEffectMain(f.s.pool,f.poolMain));
    assert(f.word(Payload+0x30)==0 && f.real(Payload+0x38)==255.f/256 && f.real(Payload+0x44+4)==65);
    f.deleteReady=false;assert(!resumeEffectMain(f.s.pool,f.poolMain));assert(f.main.pending()==0x8005ea30 && f.word(Payload+0x34)==14);
    assert(!resumeEffectMain(f.s.pool,f.poolMain));assert(f.word(Payload+0x34)==14 && f.frees==0);
    f.deleteReady=true;assert(resumeEffectMain(f.s.pool,f.poolMain));assert(f.word(e)==0 && f.frees==1 && !f.main.active());
}
void variantsAndRawCounts(){
    const uint8_t colors[][6]={{255,255,255,255,255,255},{255,255,0,255,0,0},{255,255,0,200,255,230},{0,255,255,0,0,230}};
    for(int32_t kind=0;kind<4;++kind){Fixture f;f.enter(kind,-4);assert(f.word(Pool+48)==1 && f.randomCalls==0 && f.allocatedBytes==68);for(unsigned i=0;i<6;++i)assert(f.bytes[Payload+0x3c+i]==colors[kind][i]);}
    Fixture f;f.enter(0,-5);assert(f.word(Pool+48)==0 && f.allocatedBytes==0 && f.randomCalls==0); // No invented minimum count.
}
}
int main(){entryAndHolds();lifetime();variantsAndRawCounts();std::cout<<"PASS retained original HitN64 Entry/Main, actual EffectPool callback/lifetime and held children\n";}

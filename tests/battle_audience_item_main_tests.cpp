#include "battle_audience_actor_control.hpp"
#include "battle_audience_item_main.hpp"
#include "battle_audience_item_control.hpp"
#include "field_camera_events.hpp"
#include "battle_audience_population.hpp"
#include "battle_timing_a.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <map>
#include <vector>
#include <limits>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,A=B+0x2858,P=0x40000,S=0x50000;
    EventScriptAssets assets;std::map<uint32_t,uint8_t> bytes;BattleRandom rng{1};
    std::vector<int32_t> bounds;std::vector<uint32_t> writes;unsigned rawCalls=0;
    std::optional<int32_t> forcedRaw;bool mutateRaw=false,rawAvailable=true;
    Fixture(){assert(assets.load("romfs/trio-attack-events.pmevt"));word(0x8041eb90,B);word(0x8041eb00,P);
        word(B+0x2738,S-0x100);word(S-0x100+0xc,S);half(P+0x8a,1);real(B+0x163fc+0xb34,17);
        for(unsigned i=0;i<16;++i)bytes[S+0xb0+i]=i==10?5:i>=13?1:0;
    }
    uint32_t word(uint32_t a){
        if(bytes.count(a)||bytes.count(a+1)||bytes.count(a+2)||bytes.count(a+3)){
            uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;
        }
        uint32_t v=0;if(a>=0x80000000){if(!assets.readWord(a,v)){std::fprintf(stderr,"Missing original word %08x\n",a);assert(false);}}return v;
    }
    uint8_t byte(uint32_t a){if(bytes.count(a) || a<0x80000000)return bytes[a];return word(a&~3u)>>(24-8*(a&3u));}
    void word(uint32_t a,uint32_t v){writes.push_back(a);for(unsigned i=0;i<4;++i)bytes[a+i]=v>>(24-8*i);}
    uint16_t half(uint32_t a){return uint16_t(byte(a))<<8|byte(a+1);}
    void half(uint32_t a,uint16_t v){bytes[a]=v>>8;bytes[a+1]=v;}
    void real(uint32_t a,float v){uint32_t b;std::memcpy(&b,&v,4);word(a,b);}
    float real(uint32_t a){uint32_t b=word(a);float v;std::memcpy(&v,&b,4);return v;}
    uint32_t member(unsigned i){return A+0x1bc+i*0x134;}
    BattleAudiencePopulationServices services(){
        BattleAudiencePopulationServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};
        auto& m=s.actors.memory;m.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        m.half=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};
        m.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        m.storeHalf=[](void* p,uint32_t a,uint16_t v){static_cast<Fixture*>(p)->half(a,v);};
        m.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.random=[](void* p,int32_t bound){auto& f=*static_cast<Fixture*>(p);f.bounds.push_back(bound);return f.rng.bounded(bound);};
        s.randomRaw=[](void* p)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);++f.rawCalls;
            if(f.mutateRaw)f.word(0x8041eb90,0x20000);if(!f.rawAvailable)return {};return f.forcedRaw?*f.forcedRaw:f.rng.next();};
        s.truncate=[](void*,float v)->std::optional<int32_t>{if(std::isfinite(v) && v>=-2147483648.f && v<2147483648.f)return int32_t(v);return {};};
        s.fma=[](void*,float a,float b,float c){return std::fma(a,b,c);};return s;
    }
    void positive(int32_t count,unsigned mode=0){half(P+0x8c,1);word(B+0x19078,0x10000);word(B+0x19084,count);word(S+0xac,mode);}
};

struct Lifecycle : Fixture {
    static constexpr int slot=4;static constexpr uint32_t U=0x90000,R=0x91000,D=0x92000;
    std::optional<int32_t> forced=0;bool randomReady=true,hitReady=true,curveReady=true,convertReady=true,stopReady=true;
    std::vector<int32_t> draws,stops,flags;std::vector<int32_t> steps;int32_t switchResult=1;
    std::array<float,3> hit{0,20,0};
    Lifecycle(){word(member(slot),9);bytes[member(slot)+0x1b]=10;half(member(slot)+0x1c,0xffff);word(member(slot)+0x20,0x60000);
        for(unsigned i=0;i<10;++i)word(0x60000+4*i,0x70000+16*i);
        for(unsigned i=0;i<3;++i){real(member(slot)+0x48+4*i,0);real(member(slot)+0xc0+4*i,0);}
        player(1,U,R,D,1);word(B+0xef4,0x80);
    }
    void player(unsigned id,uint32_t u,uint32_t p,uint32_t d,unsigned group){word(B+0x20+4*id,u);word(u,id);bytes[u+0xc]=uint8_t(group-1);half(u+0x10c,10);word(u+0x14,p);word(p,0);word(p+4,d);word(d,1);word(p+0x1ac,1);}
    uint32_t item(unsigned i=0){return A+0xf25c+i*0x48;}
    BattleAudienceItemEntryServices entry(){BattleAudienceItemEntryServices s;s.actors=services().actors;s.user=this;
        s.switchValue=[](void* p,int32_t n)->std::optional<int32_t>{auto& f=*static_cast<Lifecycle*>(p);f.flags.push_back(n);return f.switchResult;};return s;}
    bool create(int32_t kind=237,int32_t limit=1){return createPackedBattleAudienceItem(slot,kind,limit,entry())==1;}
    BattleAudienceItemMainServices main(){BattleAudienceItemMainServices s;s.actors=services().actors;s.user=this;
        s.random=[](void* p,int32_t n)->std::optional<int32_t>{auto& f=*static_cast<Lifecycle*>(p);f.draws.push_back(n);if(!f.randomReady)return {};return f.forced?*f.forced:f.rng.bounded(n);};
        s.hitPosition=[](void* p,uint32_t u,uint32_t part)->std::optional<std::array<float,3>>{auto& f=*static_cast<Lifecycle*>(p);assert(u==U && part==R);if(!f.hitReady)return {};return f.hit;};
        s.interpolate=[](void* p,int32_t mode,int32_t step,int32_t duration,float a,float b)->std::optional<float>{auto& f=*static_cast<Lifecycle*>(p);assert(mode==0 && duration==5 && a==-10 && b==0);f.steps.push_back(step);return f.curveReady?cameraInterpolation(mode,step,duration,a,b):std::nullopt;};
        s.truncate=[](void* p,float v)->std::optional<int32_t>{if(!static_cast<Lifecycle*>(p)->convertReady || !std::isfinite(v) || v<-2147483648.f || v>=2147483648.f)return {};return int32_t(v);};
        s.fma=[](void*,float x,float y,float z){return std::fma(x,y,z);};
        s.stopSound=[](void* p,int32_t slot){auto& f=*static_cast<Lifecycle*>(p);f.stops.push_back(slot);return f.stopReady;};return s;
    }
    bool tick(){return processPackedBattleAudienceItems(main());}
    void state(unsigned phase){word(item(),1);word(item()+0xc,slot);word(item()+4,phase);word(item()+0x10,237);}
};
int main(){
    Lifecycle f;const auto r=f.member(Lifecycle::slot),i=f.item();
    assert(f.create() && f.byte(r+0x19)==7 && f.half(r+0x1c)==0 && f.word(i)==1 && f.word(i+4)==0 && f.word(i+0xc)==4 && f.word(i+0x10)==237);
    // Creation admits original states, low16 item IDs and first inactive slot.
    Lifecycle gates;for(unsigned state=0;state<256;++state){gates.word(gates.item(),0);gates.bytes[gates.member(4)+0x19]=state;
        const bool blocked=state==7 || state==8 || state==13 || state==12 || state==11 || state==18 || state==17 || state==21 || state==10;
        assert((createPackedBattleAudienceItem(4,237,1,gates.entry())==1)==!blocked);}
    Lifecycle full;full.word(full.item(),1);assert(!full.create());assert(full.create(0x100ec,2) && full.word(full.item(1)+0x10)==236 && full.half(full.member(4)+0x1c)==1);
    Lifecycle limit;limit.writes.clear();assert(!limit.create(237,0) && limit.writes.empty());
    Lifecycle disabled;disabled.word(Lifecycle::A,0x40000);assert(!disabled.create());
    Lifecycle species;species.bytes[species.member(4)+0x1b]=6;assert(!species.create());
    Lifecycle seat;seat.word(seat.member(4),1);assert(!seat.create());
    Lifecycle absent;absent.word(absent.member(4),0);assert(!absent.create());
    Lifecycle sw;sw.switchResult=0;assert(!sw.create(121) && sw.flags==std::vector<int32_t>{235});
    sw.switchResult=1;assert(sw.create(235));Lifecycle unknown;auto es=unknown.entry();es.switchValue=nullptr;
    assert(!createPackedBattleAudienceItem(4,121,1,es));assert(createPackedBattleAudienceItem(4,236,1,es)==1);
    // Original weighted species table, then state0 -> state1 telegraph placement.
    Lifecycle weighted;weighted.forced=0;assert(weighted.create(0));assert(weighted.tick());
    assert(weighted.word(weighted.item()+0x10)==237 && weighted.draws.size()==1 && weighted.draws[0]>0);
    assert(weighted.word(weighted.item()+4)==1 && weighted.word(weighted.item()+8)==180 && (weighted.word(weighted.item())&3)==3 && weighted.steps==std::vector<int32_t>{0});
    const auto sy=weighted.real(0x803b4de4+10*32+0x10);assert(weighted.real(weighted.item()+0x18)==sy-10 && weighted.real(weighted.item()+0x20)==1 && weighted.real(weighted.item()+0x24)==0);
    Lifecycle missingCurve;assert(missingCurve.create());missingCurve.curveReady=false;assert(!missingCurve.tick());assert(missingCurve.word(missingCurve.item()+4)==1 && missingCurve.word(missingCurve.item()+8)==180);
    Lifecycle emptyTable;emptyTable.state(0);emptyTable.word(emptyTable.item()+0x10,0);emptyTable.bytes[emptyTable.member(4)+0x1b]=6;
    assert(emptyTable.tick() && emptyTable.word(emptyTable.item())==0 && emptyTable.stops==std::vector<int32_t>({10,11}));
    Lifecycle failedStop;failedStop.state(6);failedStop.word(failedStop.item()+8,0);failedStop.stopReady=false;
    assert(!failedStop.tick() && failedStop.word(failedStop.item())==1 && failedStop.word(failedStop.item()+4)==6 && failedStop.stops==std::vector<int32_t>{10});
    // Slot index enters180+index and the lift has an upper clamp only.
    Lifecycle later;later.word(later.item(),1);later.word(later.item()+4,8);later.word(later.item()+0xc,4);assert(later.create(237,2));assert(later.tick());assert(later.steps.back()==-1 && later.word(later.item(1)+8)==181);
    // Target scan uses body/head fallback, signed belong/HP and bit10000.
    Lifecycle target;assert(detectPackedBattleAudiencePlayerTarget(target.main())==Lifecycle::U);
    target.word(Lifecycle::R+0x1ac,0);assert(detectPackedBattleAudiencePlayerTarget(target.main())==Lifecycle::U);
    target.word(Lifecycle::R+0x1ac,0x10001);assert(detectPackedBattleAudiencePlayerTarget(target.main())==0);
    target.word(Lifecycle::R+0x1ac,1);target.half(Lifecycle::U+0x10c,0);assert(detectPackedBattleAudiencePlayerTarget(target.main())==0);
    target.half(Lifecycle::U+0x10c,10);target.bytes[Lifecycle::U+0xc]=255;assert(detectPackedBattleAudiencePlayerTarget(target.main())==0);
    Lifecycle retained;auto rs=retained.main();rs.random=[](void* p,int32_t n)->std::optional<int32_t>{assert(n==1);static_cast<Lifecycle*>(p)->word(0x8041eb90,0x80000);return 0;};
    assert(detectPackedBattleAudiencePlayerTarget(rs)==Lifecycle::U); // Return uses captured explicit battle.
    Lifecycle noTarget;assert(noTarget.create());assert(noTarget.tick());noTarget.word(noTarget.item()+8,0);noTarget.half(Lifecycle::U+0x10c,0);
    assert(noTarget.tick() && noTarget.word(noTarget.item())==0 && noTarget.byte(noTarget.member(4)+0x19)==0 && noTarget.half(noTarget.member(4)+0x1c)==0xffff && noTarget.stops==std::vector<int32_t>({10,11}));
    Lifecycle launch;assert(launch.create());assert(launch.tick());launch.word(launch.item()+8,0);launch.hit={15,20,0};
    assert(launch.tick() && launch.word(launch.item()+4)==2 && launch.word(launch.item()+0x40)==Lifecycle::U);
    assert(launch.word(launch.item()+8)>=30 && launch.real(launch.item()+0x2c)==7 && launch.real(launch.item()+0x34)==0 && launch.real(launch.item()+0x3c)==0);
    Lifecycle hold;assert(hold.create());assert(hold.tick());hold.word(hold.item()+8,0);hold.convertReady=false;hold.real(hold.item()+0x34,123);
    assert(!hold.tick() && hold.word(hold.item()+4)==1 && hold.real(hold.item()+0x34)==123 && hold.word(hold.item()+0x40)==Lifecycle::U);
    // Flight stores position XYZ before velocities XYZ, resets actor at20 and
    // advances state on the following zero-count call, not on the last move.
    Lifecycle flight;flight.state(2);flight.word(flight.item()+8,20);flight.word(flight.item()+0x10,239);flight.word(flight.item()+0x44,1);
    for(unsigned a=0;a<3;++a){flight.real(flight.item()+0x14+4*a,float(a));flight.real(flight.item()+0x28+4*a,2);flight.real(flight.item()+0x34+4*a,1);}
    assert(flight.tick() && flight.word(flight.item()+8)==19 && flight.byte(flight.member(4)+0x19)==0 && flight.real(flight.item()+0x14)==2 && flight.real(flight.item()+0x28)==3 && flight.real(flight.item()+0x24)==18);
    flight.word(flight.item()+8,1);assert(flight.tick() && flight.word(flight.item()+4)==2 && flight.word(flight.item()+8)==0);
    const auto x=flight.real(flight.item()+0x14);assert(flight.tick() && flight.word(flight.item()+4)==3 && flight.real(flight.item()+0x14)==x);
    // Presentation publishes original fields/descriptors; it never grants pouch.
    for(int kind:{121,128,236,237,238,239}){Lifecycle present;present.state(3);present.word(present.item()+0x40,Lifecycle::U);present.word(present.item()+0x44,1);present.word(present.item()+0x10,kind);present.real(present.item()+0x18,100);
        const auto coins=present.half(Lifecycle::P+0x78);assert(present.tick());assert(present.word(Lifecycle::A+0x137e4)==uint32_t(kind) && present.word(Lifecycle::A+0x137ec)==1 && present.half(Lifecycle::P+0x78)==coins);
        assert(present.word(present.item()+4)==(kind<236?7u:4u));
        if(kind>=236){const auto table=0x8039c880+uint32_t(kind-236)*0xc0;for(unsigned n=0;n<0xc0;n+=4)assert(present.word(Lifecycle::A+0x137fc+n)==present.word(table+n));}
    }
    Lifecycle multiple;multiple.state(3);multiple.word(multiple.item()+0x40,Lifecycle::U);multiple.word(multiple.item()+0x44,2);multiple.real(multiple.item()+0x18,100);multiple.word(Lifecycle::A+0x137f8,3);multiple.word(Lifecycle::A+0x137e4,999);
    assert(multiple.tick() && multiple.word(multiple.item()+4)==4 && multiple.word(Lifecycle::A+0x137f8)==3 && multiple.word(Lifecycle::A+0x137e4)==999);
    Lifecycle combo;combo.state(3);combo.word(combo.item()+0x40,Lifecycle::U);combo.word(combo.item()+0x44,1);combo.word(Lifecycle::A+0x137f8,2);combo.real(combo.item()+0x18,100);
    assert(combo.tick() && combo.word(Lifecycle::A+0x1381c)==combo.word(0x8039c940+0x20)+2 && combo.word(Lifecycle::A+0x137f8)==0);
    // Rejection gates do not publish an item: turn flag/status/shadow guard,
    // blocked flag4, and full inventory. Negative duration remains signed.
    for(unsigned gate=0;gate<6;++gate){Lifecycle reject;reject.state(3);reject.word(reject.item()+0x40,Lifecycle::U);reject.word(reject.item()+0x44,1);reject.word(Lifecycle::A+0x137e4,999);reject.real(reject.item()+0x18,100);
        if(gate==0)reject.word(Lifecycle::B+0xef4,0);if(gate==1)reject.bytes[Lifecycle::U+0x12c]=1;if(gate==2)reject.word(Lifecycle::U+0x104,0x10);
        if(gate==3){reject.word(Lifecycle::U+8,228);reject.word(Lifecycle::U+0x228,1);}if(gate==4)reject.word(reject.item(),5);
        if(gate==5){reject.word(reject.item()+0x10,128);for(unsigned n=0;n<10;++n)reject.half(Lifecycle::P+0x192+2*n,128);}
        assert(reject.tick() && reject.word(reject.item()+4)==4 && reject.word(Lifecycle::A+0x137e4)==999);
    }
    Lifecycle space;assert(packedBattleAudienceEmptyItemSlots(space.services().actors)==10);space.half(Lifecycle::P+0xa0,1);assert(packedBattleAudienceEmptyItemSlots(space.services().actors)==20);
    Lifecycle negative;negative.state(3);negative.word(negative.item()+0x40,Lifecycle::U);negative.word(negative.item()+0x44,1);negative.bytes[Lifecycle::U+0x12c]=128;negative.real(negative.item()+0x18,100);assert(negative.tick() && negative.word(Lifecycle::A+0x137e4)==237);
    // Ground bounce, wait/blink, cleanup, invalid owner and unsigned state skip.
    Lifecycle bounce;bounce.state(4);bounce.real(bounce.item()+0x18,0);assert(bounce.tick() && bounce.word(bounce.item()+4)==5 && bounce.real(bounce.item()+0x18)==bounce.real(0x804249e8) && bounce.real(bounce.item()+0x2c)==1);
    bounce.real(bounce.item()+0x18,0);bounce.real(bounce.item()+0x2c,-1);assert(bounce.tick() && bounce.word(bounce.item()+4)==6 && bounce.word(bounce.item()+8)==60);
    for(unsigned n=0;n<60;++n)assert(bounce.tick());assert(bounce.word(bounce.item())&1);assert(bounce.tick() && bounce.word(bounce.item())==0 && bounce.half(bounce.member(4)+0x1c)==0xffff);
    Lifecycle invalid;invalid.state(2);invalid.bytes[invalid.member(4)+0x19]=12;assert(invalid.tick() && invalid.word(invalid.item())==0 && invalid.stops.empty());
    Lifecycle unknownState;unknownState.state(8);unknownState.word(unknownState.item()+8,123);assert(unknownState.tick() && unknownState.word(unknownState.item()+8)==123);
    // A source item created at slot0 reaches holding, launch and full cleanup
    // with the existing actor7/8 controllers, one actual target RNG draw, and
    // original presentation metadata. It is not a full BattleMain fixture.
    Lifecycle chain;chain.forced.reset();assert(chain.create());assert(chain.tick());BattleRandom oracle=chain.rng;oracle.bounded(1);
    BattleAudienceItemControlServices control;control.actors=chain.services().actors;control.user=&chain;
    control.throwSound=[](void*,int32_t,int32_t){return true;}; // Explicit sound service fixture.
    unsigned ticks=0;while((chain.word(chain.item())&1) && ticks<500){
        const auto state=chain.byte(chain.member(4)+0x19);
        if(state==7)assert(controlPackedBattleAudienceItemState(0x80191a48,4,0,control));
        else if(state==8)assert(controlPackedBattleAudienceItemState(0x80191980,4,0,control));
        assert(chain.tick());++ticks;
    }
    assert(ticks<500 && chain.word(chain.item())==0 && chain.half(chain.member(4)+0x1c)==0xffff && chain.rng.state==oracle.state && chain.draws==std::vector<int32_t>{1});
}

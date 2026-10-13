#include "battle_event_callbacks.hpp"
#include "battle_jump_motion.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <vector>
using namespace ttyd;
uint32_t bits(float value) {uint32_t word;std::memcpy(&word,&value,4);return word;}
struct Fixture {
    std::array<EventEntry,1> entries{};int sinCalls=0,playCalls=0,conversions=0;std::vector<uint32_t> stores;std::vector<std::array<float,3>> played;
    std::function<void(uint32_t)> onStore;std::function<void(int)> onConvert;std::function<void()> onPlay;
    EventCallbackRegistry* registry=nullptr;
    static EventNativePlatform platform(Fixture* f) {
        EventNativePlatform p;p.user=f;p.call=[](void* p,EventEntry& e,uint32_t address,int32_t first)->int32_t {auto& f=*static_cast<Fixture*>(p);assert(f.registry);return EventCallbackRegistry::dispatch(f.registry,e,address,first);};
        p.signedWordFloat=[](void* p,int32_t v) {auto& f=*static_cast<Fixture*>(p);if(f.onConvert)f.onConvert(++f.conversions);return float(v);};p.divideWord=[](void*,int32_t a,int32_t b)->int32_t {return a/b;};p.truncate=[](void*,float v)->int32_t {return int32_t(v);};
        p.addFloat=[](void*,float a,float b) {return a+b;};p.subtractFloat=[](void*,float a,float b) {return a-b;};p.multiplyFloat=[](void*,float a,float b) {return a*b;};p.divideFloat=[](void*,float a,float b) {return a/b;};p.fmaFloat=[](void*,float a,float b,float c) {return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c) {return -std::fma(a,b,-c);};
        p.distanceFloat=[](void*,float a,float b,float c,float d) {return std::hypot(c-a,d-b);};p.headingFloat=[](void*,float a,float b,float c,float d) {return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};p.sineDouble=[](void* p,double v) {++static_cast<Fixture*>(p)->sinCalls;return std::sin(v);};p.cosineDouble=[](void*,double v) {return std::cos(v);};p.roundDoubleFloat=[](void*,double v) {return float(v);};
        p.addDouble=[](void*,double a,double b) {return a+b;};p.subtractDouble=[](void*,double a,double b) {return a-b;};p.multiplyDouble=[](void*,double a,double b) {return a*b;};p.divideDouble=[](void*,double a,double b) {return a/b;};p.fmaDouble=[](void*,double a,double b,double c) {return std::fma(a,b,c);};p.fmodDouble=[](void*,double a,double b) {return std::fmod(a,b);};return p;
    }
    EventNativeRuntime runtime{0x81000000,platform(this)};BattleMarioJumpBackend backend;
    Fixture() {
        assert(runtime.entryAddresses.registerEntries(0x82000000,entries.data(),1));entries[0].arguments=0x1000;entries[0].owner=7;
        for(auto span:std::vector<std::pair<uint32_t,unsigned>>{{0x1000,0x100},{0x2000,0x100},{0x3000,0x100},{0x4000,0x300},{0x8041eb90,4},{0x80422280,0x60},{0x804226c4,4},{0x802ee6f0,0x50},{0x802ee470,12},{0x802bf240,8},{0x8041f448,4},{0x8041f45c,4}})assert(runtime.memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
        w(0x8041eb90,0x2000);w(0x203c,0x4000);w(0x303c,0x4000);w(0x4008,222);w(0x1000,uint32_t(-2));w(0x1004,100);w(0x1008,50);w(0x100c,0);w(0x1010,42);w(0x1014,0);w(0x1018,uint32_t(-1));w(0x4040,bits(2));w(0x4170,bits(5));w(0x41c4,42);w(0x41c8,43);
        for(auto v:std::vector<std::pair<uint32_t,float>>{{0x80422280,0},{0x804222bc,360},{0x804222c0,6.2831854820251465f},{0x804222c4,3.1415927410125732f},{0x804222c8,90},{0x804222cc,1.7999999523162842f},{0x804222d0,.15000000596046448f},{0x804222d4,60},{0x804226c4,-250000000},{0x8041f448,0},{0x8041f45c,360}})w(v.first,bits(v.second));
        const double values[]={20,6,47,1.12,1,.53,1.25,.8,16.5,33};for(unsigned i=0;i<10;++i)wide(0x802ee6f0+8*i,values[i]);wide(0x802bf240,360);
        backend.runtime=&runtime;backend.user=this;backend.play=[](void* p,uint32_t key,const std::array<float,3>& xyz)->uint32_t {auto& f=*static_cast<Fixture*>(p);assert(key==42);++f.playCalls;f.played.push_back(xyz);if(f.onPlay)f.onPlay();return 7;};
    }
    void w(uint32_t a,uint32_t v) {assert(runtime.memory.writeWord(a,v));}
    uint32_t w(uint32_t a) {uint32_t v;assert(runtime.memory.readWord(a,v));return v;}
    void wide(uint32_t a,double v) {uint64_t bits;std::memcpy(&bits,&v,8);w(a,uint32_t(bits>>32));w(a+4,uint32_t(bits));}
    float f(uint32_t a) {const uint32_t v=w(a);float f;std::memcpy(&f,&v,4);return f;}
    BattlePackedMarioJumpServices services() {auto s=nativeBattleMarioJumpServices(backend);auto m=s.motion.query.common.position.unit.memory;m.user=this;m.word=[](void* p,uint32_t a) {return static_cast<Fixture*>(p)->w(a);};m.storeWord=[](void* p,uint32_t a,uint32_t v) {auto& f=*static_cast<Fixture*>(p);f.w(a,v);f.stores.push_back(a);if(f.onStore)f.onStore(a);};m.half=[](void* p,uint32_t a) {uint16_t v;assert(static_cast<Fixture*>(p)->runtime.memory.readHalf(a,v));return v;};m.storeHalf=[](void* p,uint32_t a,uint16_t v) {assert(static_cast<Fixture*>(p)->runtime.memory.writeHalf(a,v));};m.storeByte=[](void* p,uint32_t a,uint8_t v) {assert(static_cast<Fixture*>(p)->runtime.memory.writeByte(a,v));};s.motion.query.common.position.unit.memory=m;s.motion.query.common.position.memory.memory=m;return s;}
    int32_t run(int first) {auto s=services();return packedMarioJumpPositionEvent(entries[0],first,s);}
};
bool close(float a,float b) {return std::fabs(a-b)<.0002f || (std::isnan(a) && std::isnan(b));}
JumpMotionServices oracleServices() {JumpMotionServices s;s.heading=[](void*,float a,float b,float c,float d) {return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};s.distance=[](void*,float a,float b,float c,float d) {return std::hypot(c-a,d-b);};s.sin=[](void*,double v) {return std::sin(v);};s.fmod=[](void*,double a,double b) {return std::fmod(a,b);};s.truncate=[](void*,float v)->int32_t {return int32_t(v);};s.eligible=[](void*,MarioJumpUnit&) {return true;};s.getPosition=[](void*,MarioJumpUnit& u) {return u.visible;};s.setPosition=[](void*,MarioJumpUnit& u,JumpVector v) {u.visible=v;};s.playSound=[](void*,uint32_t key,JumpVector) {assert(key==42);};return s;}
int main() {
    // Cross-compose portable and native bodies under identical authored math
    // providers;this is not a PowerPC/original-execution numerical oracle.
    for(int mode:{-1,0,2,3,4,5}) {
        Fixture f;f.w(0x1014,uint32_t(mode));MarioJumpUnit u;u.visible={0,2,0};u.speed=5;u.soundSource[3]=42;u.soundSource[7]=43;MarioJumpEvent e;MarioJumpArguments a;a.x=100;a.y=50;a.z=0;a.count=42;a.mode=mode;a.soundMode=-1;
        for(int i=0;i<42;++i) {const int nr=f.run(i==0),pr=marioJumpMotion(u,e,a,i==0,oracleServices());assert(nr==pr && nr==(i==41?1:0));for(auto v:std::vector<std::pair<uint32_t,float>>{{0x403c,u.visible.x},{0x4040,u.visible.y},{0x4044,u.visible.z},{0x4154,u.baseline.x},{0x4158,u.baseline.y},{0x415c,u.baseline.z},{0x4170,u.speed},{0x4174,u.phaseIncrement},{0x4178,u.phase},{0x4184,u.height},{0x41b4,u.angle},{0x41b8,u.angleStep},{0x41bc,u.lastY}})assert(close(f.f(v.first),v.second));assert(int32_t(f.w(0x416c))==u.remaining && f.entries[0].initialized==uint8_t(e.phase));}
        assert(f.playCalls==1 && f.sinCalls==85 && f.w(0x41a4)==0xffffffff && f.w(0x4178)==bits(-0.0f) && f.f(0x4040)==50 && f.played[0]==std::array<float,3>({0,2,0}));
    }
    Fixture m0,m3;m3.w(0x1014,3);m0.run(1);m3.run(1);assert(close(m0.f(0x41b8),360.f/42) && m3.f(0x41b8)==8 && m0.w(0x4178)!=m3.w(0x4178));
    Fixture zero;zero.w(0x1004,0);zero.w(0x1010,0);zero.w(0x4184,bits(123));zero.w(0x4178,bits(456));assert(zero.run(1)==2 && zero.w(0x416c)==0 && zero.f(0x4184)==123 && zero.f(0x4178)==456 && zero.playCalls==0 && zero.sinCalls==0 && zero.f(0x4040)==2);
    Fixture negative;negative.w(0x1010,uint32_t(-2));assert(negative.run(1)==0 && int32_t(negative.w(0x416c))==-3);
    Fixture mode1;mode1.w(0x1014,1);mode1.w(0x4154,bits(12));mode1.w(0x4158,bits(23));mode1.w(0x415c,bits(34));mode1.w(0x403c,bits(90));mode1.w(0x4040,bits(80));mode1.w(0x4044,bits(70));mode1.w(0x41bc,bits(44));assert(mode1.run(1)==0 && mode1.entries[0].initialized==3 && mode1.f(0x4154)==90 && mode1.f(0x4158)==80 && mode1.f(0x415c)==70 && mode1.playCalls==0 && mode1.sinCalls==0);assert(mode1.run(0)==1 && mode1.f(0x403c)==90 && mode1.f(0x4040)==80 && mode1.f(0x4044)==70 && mode1.sinCalls==2 && mode1.f(0x41b4)==90);
    for(uint32_t field:{0x400cu,0x4008u,0x4118u,0x411au,0x4122u,0x4135u,0x4138u}) {Fixture f;f.w(0x1014,1);if(field==0x4008)f.w(field,223);else if(field==0x4138)f.w(field,1);else assert(f.runtime.memory.writeByte(field,1));assert(f.run(1)==2 && f.entries[0].initialized==2 && f.playCalls==0 && f.sinCalls==0);}
    Fixture eligibility;assert(eligibility.runtime.memory.writeHalf(0x410c,0));assert(packedACPossibility(0x4000,nativeBattleActorServices(eligibility.runtime))==1); // No HP gate.
    Fixture clamp;clamp.entries[0].initialized=1;clamp.w(0x416c,3);clamp.w(0x4164,bits(10));clamp.w(0x4184,bits(0));assert(clamp.run(0)==0 && clamp.f(0x4040)==0);assert(clamp.run(0)==0 && clamp.f(0x4040)==10 && clamp.f(0x4158)==0);assert(clamp.run(0)==1 && clamp.f(0x4040)==10);
    Fixture countWrap;countWrap.entries[0].initialized=1;countWrap.w(0x416c,0x80000000);assert(countWrap.run(0)==0 && countWrap.w(0x416c)==0x7fffffff);
    Fixture unsnapped;unsnapped.entries[0].initialized=1;unsnapped.w(0x416c,1);unsnapped.w(0x4154,bits(5));unsnapped.w(0x41a8,bits(1));unsnapped.w(0x4160,bits(999));assert(unsnapped.run(0)==1 && unsnapped.f(0x403c)==6 && unsnapped.f(0x4160)==999);
    Fixture unknown;unknown.entries[0].initialized=255;assert(unknown.run(0)==0 && unknown.stores.empty() && unknown.sinCalls==0);
    Fixture phaseOverride;phaseOverride.onPlay=[&] {phaseOverride.entries[0].initialized=255;};assert(phaseOverride.run(1)==0 && phaseOverride.w(0x416c)==42 && phaseOverride.f(0x4154)==0); // Phase reread after audio provider.
    Fixture prefetched;prefetched.w(0x1010,2);prefetched.onConvert=[&](int n) {if(n==1)prefetched.w(0x1008,999);if(n==2)prefetched.w(0x100c,55);if(n==3) {prefetched.w(0x1010,1);prefetched.w(0x8041eb90,0x3000);}};assert(prefetched.run(1)==0 && prefetched.f(0x4164)==50 && prefetched.f(0x4168)==0 && prefetched.w(0x416c)==1);
    Fixture angle;auto as=angle.services();assert(revisePackedJumpAngle(-0.0f,as)==0 && std::signbit(revisePackedJumpAngle(-0.0f,as)) && revisePackedJumpAngle(-30,as)==330 && revisePackedJumpAngle(std::numeric_limits<float>::quiet_NaN(),as)==0);
    Fixture refresh;refresh.w(0x1014,1);refresh.w(0x403c,bits(90));refresh.w(0x4040,bits(80));refresh.onStore=[&](uint32_t address) {if(address==0x4154)refresh.w(0x4040,bits(123));};assert(refresh.run(1)==0 && refresh.f(0x4158)==123); // Mode1 GetPos writes baselineX before reading visibleY.
    Fixture vm;vm.w(0x1010,2);auto vs=vm.services();EventCallbackRegistry registry;assert(bindBattleMarioJumpPositionCallback(registry,vs));vm.registry=&registry;assert(vm.runtime.memory.registerRam(0x7000,std::vector<uint8_t>(40)));const uint32_t script[]={0x0008005b,0x801093a8,uint32_t(-2),100,50,0,2,0,uint32_t(-1),2};for(unsigned i=0;i<10;++i)vm.w(0x7000+4*i,script[i]);auto& ve=vm.entries[0];ve.flags=1;ve.opcode=0;ve.nextCommand=0x7000;assert(dispatchEventInterpreter(ve,vm.runtime.interpreterServices())==0 && ve.opcode==91 && ve.cachedFunction==0x801093a8 && vm.w(0x416c)==1);assert(dispatchEventInterpreter(ve,vm.runtime.interpreterServices())==0 && ve.opcode==0 && ve.nextCommand==0x7024 && vm.w(0x416c)==0 && vm.f(0x4040)==50 && vm.playCalls==1);
    std::cout<<"native MarioJumpPosition modes/phases/curve/field/order/VM tests passed\n";
}

#include "battle_jump_motion.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
    MarioJumpUnit unit;
    float distance=100,heading=12,sineValue=0;
    bool eligible=true,realSine=false;
    int sineCalls=0,getCalls=0,setCalls=0,playCalls=0,eligibilityCalls=0;
    uint32_t played=0;
    int storeCalls=0;
    int32_t storedDestination=0,storedCount=0;
    JumpVector soundPosition;
    float destinationX=0;
    std::vector<int> order;
    std::array<int32_t,7> values{{-2,100,0,0,42,0,-1}};
    JumpMotionServices services() {
        JumpMotionServices s;s.user=this;
        s.heading=[](void* p,float,float,float x,float) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(10);f.destinationX=x;return f.heading;};
        s.distance=[](void* p,float,float,float,float) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(11);return f.distance;};
        s.sin=[](void* p,double x) {auto& f=*static_cast<Fixture*>(p);++f.sineCalls;return f.realSine?std::sin(x):double(f.sineValue);};
        s.fmod=[](void*,double x,double y) {return std::fmod(x,y);};
        s.truncate=[](void*,float x) {assert(std::isfinite(x) && double(x)>=-2147483648.0 && double(x)<2147483648.0);return int32_t(x);};
        s.eligible=[](void* p,MarioJumpUnit&) {auto& f=*static_cast<Fixture*>(p);++f.eligibilityCalls;return f.eligible;};
        s.getPosition=[](void* p,MarioJumpUnit& u) {auto& f=*static_cast<Fixture*>(p);++f.getCalls;return u.visible;};
        s.setPosition=[](void* p,MarioJumpUnit& u,JumpVector v) {auto& f=*static_cast<Fixture*>(p);++f.setCalls;u.visible=v;};
        s.playSound=[](void* p,uint32_t id,JumpVector v) {auto& f=*static_cast<Fixture*>(p);++f.playCalls;f.played=id;f.soundPosition=v;};
        return s;
    }
    JumpEventServices events() {
        JumpEventServices s;s.user=this;
        s.resolve=[](void* p,int32_t operand) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(operand);assert(operand>=0 && operand<7);return f.values[operand];};
        s.translateActor=[](void* p,int32_t actor) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(8);assert(actor==-2);return 7;};
        s.unit=[](void* p,int32_t actor) {auto& f=*static_cast<Fixture*>(p);f.order.push_back(9);assert(actor==7);return &f.unit;};
        s.store=[](void* p,int32_t destination,int32_t count) {auto& f=*static_cast<Fixture*>(p);++f.storeCalls;f.order.push_back(12);f.storedDestination=destination;f.storedCount=count;};
        return s;
    }
};
MarioJumpArguments args(int count,int mode=0) {MarioJumpArguments a;a.x=100;a.count=count;a.mode=mode;return a;}
uint32_t word(const uint8_t* p) {return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];}
}
int main() {
    // First call integrates, N calls complete, X/Z are interpolated, lastY does
    // not become landingY. Constant injected sine makes service count explicit.
    Fixture f;f.unit.visible={4,8,12};f.sineValue=1;
    f.unit.soundSource[3]=0x31;f.unit.soundSource[7]=0x99;
    MarioJumpEvent e;auto a=args(42);a.y=20;a.z=33;
    assert(marioJumpMotion(f.unit,e,a,true,f.services())==0);
    assert(e.phase==1 && f.unit.remaining==41 && f.sineCalls==3);
    assert(f.playCalls==1 && f.played==0x31 && f.soundPosition.x==4 && f.soundPosition.y==8);
    assert(f.unit.baseline.y!=f.unit.visible.y && f.unit.direction==1);
    assert(f.unit.angleStep==360.f/42.f);
    for(int n=1;n<42;++n) {
        const int result=marioJumpMotion(f.unit,e,a,false,f.services());
        assert(result==(n==41?1:0));
    }
    assert(f.unit.remaining==0 && f.unit.visible.y==20 && f.unit.lastY>20);
    assert(f.unit.visible.x==f.unit.baseline.x && f.unit.visible.z==f.unit.baseline.z);
    assert(std::signbit(f.unit.phase) && f.unit.phase==0 && f.unit.phaseIncrement==1.7999999523162842f);
    assert(f.sineCalls==85 && f.playCalls==1 && e.phase==1);
    // Resuming a completed callback is not a benign extra polling call.
    assert(marioJumpMotion(f.unit,e,a,false,f.services())==0 && f.unit.remaining==-1);

    Fixture m3;m3.sineValue=1;MarioJumpEvent e3;
    assert(marioJumpMotion(m3.unit,e3,args(42,3),true,m3.services())==0);
    assert(m3.unit.angleStep==8 && m3.unit.phase!=f.unit.phase);
    Fixture m0;m0.sineValue=1;MarioJumpEvent e0;
    marioJumpMotion(m0.unit,e0,args(42),true,m0.services());
    assert(m0.unit.phase!=m3.unit.phase);
    Fixture m2,m4;MarioJumpEvent e2,e4;
    marioJumpMotion(m2.unit,e2,args(42,2),true,m2.services());
    marioJumpMotion(m4.unit,e4,args(42,4),true,m4.services());
    const float h1=float(double(100.f)-20.0),h2=float(double(h1)/6.0),h3=float(double(h2)+47.0);
    assert(m0.unit.height==h3 && m2.unit.height==float(double(h3)*1.12));
    assert(m4.unit.height==float(double(h3)*1.25));

    // Sentinels retain targets, but distance/heading receive raw inputs.
    Fixture sentinel;sentinel.unit.target={10,20,30};MarioJumpEvent es;
    auto as=args(2);as.x=as.y=as.z=-250000000;
    marioJumpMotion(sentinel.unit,es,as,true,sentinel.services());
    assert(sentinel.destinationX==-250000000 && sentinel.unit.target.x==10);
    assert(sentinel.unit.baseline.x==5 && sentinel.unit.direction==-1);
    assert(sentinel.unit.visible.y==20 && sentinel.setCalls==2); // strict Y clamp, oldN2.
    Fixture strict;strict.unit.delta={1,0,2};strict.unit.target.y=10;
    strict.unit.height=0;strict.unit.remaining=3;MarioJumpEvent er;er.phase=1;
    assert(marioJumpMotion(strict.unit,er,args(3),false,strict.services())==0);
    assert(strict.unit.visible.y==0 && strict.setCalls==1);
    assert(marioJumpMotion(strict.unit,er,args(3),false,strict.services())==0);
    assert(strict.unit.visible.y==10 && strict.unit.baseline.y==0 && strict.setCalls==3);

    Fixture zero;zero.distance=1;zero.unit.speed=100;zero.unit.visible={3,4,5};
    zero.unit.height=123;zero.unit.phase=456;zero.unit.soundMode.fill(0x77);
    MarioJumpEvent ez;
    assert(marioJumpMotion(zero.unit,ez,args(0),true,zero.services())==2);
    assert(zero.unit.remaining==0 && zero.unit.baseline.x==3 && zero.unit.target.x==100);
    assert(zero.unit.height==123 && zero.unit.phase==456 && zero.unit.soundMode[0]==0x77);
    assert(zero.setCalls==0 && zero.sineCalls==0 && zero.playCalls==0);
    Fixture derived;derived.unit.speed=5;MarioJumpEvent ed;
    marioJumpMotion(derived.unit,ed,args(0),true,derived.services());
    assert(derived.unit.remaining==19 && derived.unit.speed==5);
    Fixture negative;MarioJumpEvent en;
    assert(marioJumpMotion(negative.unit,en,args(-2),true,negative.services())==0);
    assert(negative.unit.remaining==-3 && negative.unit.angleStep==-180);
    Fixture wrap;wrap.unit.remaining=std::numeric_limits<int32_t>::min();
    MarioJumpEvent ew;ew.phase=1;
    assert(marioJumpMotion(wrap.unit,ew,args(3),false,wrap.services())==0);
    assert(wrap.unit.remaining==std::numeric_limits<int32_t>::max());

    Fixture mode1;mode1.unit.baseline={12,23,34};mode1.unit.visible={90,80,70};
    mode1.unit.lastY=44;mode1.unit.target={8,9,10};MarioJumpEvent e1;
    assert(marioJumpMotion(mode1.unit,e1,args(10,1),true,mode1.services())==0);
    assert(e1.phase==3 && mode1.unit.remaining==1 && mode1.unit.height==0);
    assert(mode1.unit.baseline.x==12 && mode1.unit.target.x==8 && mode1.playCalls==0);
    assert(mode1.getCalls==1 && mode1.sineCalls==0 && mode1.eligibilityCalls==1);
    assert(marioJumpMotion(mode1.unit,e1,args(10,1),false,mode1.services())==1);
    assert(mode1.unit.visible.x==12 && mode1.unit.visible.y==23 && mode1.unit.visible.z==34);
    assert(mode1.sineCalls==2 && mode1.unit.angle==90);
    Fixture denied;denied.eligible=false;denied.unit.remaining=88;MarioJumpEvent deny;
    assert(marioJumpMotion(denied.unit,deny,args(10,1),true,denied.services())==2);
    assert(denied.getCalls==0 && denied.unit.remaining==88 && deny.phase==2);
    MarioJumpEvent invalid;invalid.phase=-1;
    assert(marioJumpMotion(denied.unit,invalid,args(1),false,denied.services())==0);

    const auto math=f.services();
    assert(reviseJumpAngle(-1,math)==359 && reviseJumpAngle(720,math)==0);
    assert(reviseJumpAngle(std::numeric_limits<float>::quiet_NaN(),math)==0);
    assert(std::signbit(reviseJumpAngle(-0.f,math)));
    assert(!std::signbit(reviseJumpAngle(std::numeric_limits<float>::infinity(),math)));
    Fixture exceptional;exceptional.unit.baseline.y=std::numeric_limits<float>::infinity();
    exceptional.unit.remaining=1;MarioJumpEvent ex;ex.phase=1;
    assert(marioJumpMotion(exceptional.unit,ex,args(1),false,exceptional.services())==1);
    assert(std::isnan(exceptional.unit.phase));

    Fixture event;MarioJumpEvent ev;const std::array<int32_t,7> operands{{0,1,2,3,4,5,6}};
    assert(marioJumpMotionEvent(ev,operands,true,event.events(),event.services())==0);
    assert(event.order==std::vector<int>({0,8,1,2,3,4,5,6,9,10,11}));
    event.order.clear();event.values[5]=3;
    assert(marioJumpMotionEvent(ev,operands,false,event.events(),event.services())==0);
    assert(event.order==std::vector<int>({0,8,1,2,3,4,5,6,9}));

    Fixture parameter;parameter.distance=60;parameter.unit.speed=5;
    parameter.unit.visible={12,34,56};parameter.unit.baseline={7,8,9};
    parameter.unit.remaining=123;parameter.unit.phase=4;
    assert(marioJumpCount(parameter.unit,100,0,36,parameter.services())==36);
    parameter.distance=100;
    assert(marioJumpCount(parameter.unit,100,0,36,parameter.services())==42);
    assert(marioJumpCount(parameter.unit,100,0,0,parameter.services())==26);
    assert(marioJumpCount(parameter.unit,100,0,std::numeric_limits<int32_t>::max(),parameter.services())==std::numeric_limits<int32_t>::max());
    assert(parameter.unit.visible.x==12 && parameter.unit.baseline.x==7);
    assert(parameter.unit.remaining==123 && parameter.unit.phase==4 && parameter.unit.speed==5);
    parameter.order.clear();parameter.values[4]=36;parameter.values[2]=999999;
    const std::array<int32_t,5> paramOperands{{0,1,2,3,4}};
    assert(marioJumpParameterEvent(paramOperands,parameter.events(),parameter.services())==2);
    assert(parameter.storedDestination==4 && parameter.storedCount==42 && parameter.storeCalls==1);
    assert(parameter.order==std::vector<int>({0,8,1,2,3,4,9,11,12}));
    parameter.distance=60;parameter.values[4]=-250000000;parameter.order.clear();
    assert(marioJumpParameterEvent(paramOperands,parameter.events(),parameter.services())==2);
    assert(parameter.storeCalls==1 && parameter.order==std::vector<int>({0,8,1,2,3,4,9,11}));
    parameter.distance=100;
    marioJumpParameterEvent(paramOperands,parameter.events(),parameter.services());
    assert(parameter.storeCalls==2 && parameter.storedCount==-249999994);
    Fixture composition;MarioJumpEvent composeEvent;
    auto composed=args(marioJumpCount(composition.unit,100,0,36,composition.services()));
    assert(composed.count==42);
    marioJumpMotion(composition.unit,composeEvent,composed,true,composition.services());
    assert(composition.unit.remaining==41);

    // All partial loader modes retain gap bytes and perform ordered alias reads.
    std::array<uint8_t,0x36> source;for(unsigned i=0;i<source.size();++i)source[i]=i+1;
    std::array<uint8_t,0x1c> dest;
    for(int type=0;type<=4;++type) {
        dest.fill(0xaa);loadUnitSoundMode(type,-1,source.data(),dest.data());
        assert(dest[8]==0xaa && dest[11]==0xaa && dest[0x14]==0 && word(dest.data()+0x18)==0xffffffff);
        const unsigned off=type==0?8:type==1?0x18:type==3?0x28:0;
        assert(word(dest.data())==(type==4?0:word(source.data()+off)));
        if(type==2 || type==4)assert(dest[0xc]==0 && dest[0x10]==0);
        else assert(dest[0xc]==source[off+8] && dest[0x11]==source[off+13]);
        assert(dest[0x12]==dest[0xc] && dest[0x13]==dest[0xd]);
    }
    dest.fill(0x55);loadUnitSoundMode(999,-2,nullptr,dest.data());
    assert(word(dest.data())==0x55555555 && dest[0xc]==0x55 && dest[0x12]==0x55);
    for(int mode:{-3,0,1,99}) {
        dest.fill(0x55);loadUnitSoundMode(2,mode,nullptr,dest.data());
        assert(word(dest.data())==0 && dest[8]==0x55 && dest[0x12]==0);
    }
    std::array<uint8_t,0x40> alias;alias.fill(0x55);
    loadUnitSoundMode(1,-1,alias.data(),alias.data());
    // The unconditional destination+18=-1 is also source type1's first word.
    assert(word(alias.data())==0xffffffff && word(alias.data()+4)==0x55555555);
    Fixture mute;MarioJumpEvent em;auto am=args(2);am.soundMode=0;mute.unit.soundSource[3]=44;
    marioJumpMotion(mute.unit,em,am,true,mute.services());assert(mute.playCalls==0);
    std::cout<<"Mario jump motion tests passed\n";
}

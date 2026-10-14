#include "battle_camera_frame.hpp"
#include "battle_event_callbacks.hpp"
#include <cassert>
#include <cstring>
#include <vector>
using namespace ttyd;
struct Fixture {
    EventNativeRuntime vm{0x81000000,{}};
    BattleCameraFrame frame;std::vector<int32_t> steps,rumble;
    Fixture(){
        for(auto span:std::array<std::pair<uint32_t,unsigned>,8>{{{0x2000,0x19400},{0x40000,0x1000},{0x42000,0x1000},{0x50000,0x100},{0x60000,0x1000},{0x8041eb90,4},{0x8041eb00,4},{0x802f0240,12}}})assert(vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)));
        w(0x8041eb90,0x2000);w(0x8041eb00,0x60000);w(0x4738,0x50000);w(0x50008,1);
        w(0x2020,0x40000);w(0x2024,0x42000);w(0x40008,222);w(0x42008,224);
    }
    void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}
    uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
    void f(uint32_t a,float v){uint32_t bits;std::memcpy(&bits,&v,4);w(a,bits);}
    uint16_t h(uint32_t a){uint16_t v;assert(vm.memory.readHalf(a,v));return v;}
    BattleCameraFrameServices services(){
        BattleCameraFrameServices s;s.requests.positions.memory=nativeBattleActorServices(vm);s.user=this;
        s.interpolate=[](void* u,int32_t,int32_t step,int32_t duration,float a,float b)->std::optional<float>{static_cast<Fixture*>(u)->steps.push_back(step);return duration==0?b:a+(b-a)*float(step)/float(duration);};
        s.rumbleOff=[](void* u,int32_t channel){static_cast<Fixture*>(u)->rumble.push_back(channel);};return s;
    }
};
int main(){
    Fixture f;auto s=f.services();f.w(0x4750,0x1234);f.w(0x4858,0x5678);initializeBattleCamera(0x2000,s.requests);
    assert(f.w(0x4750)==0x1234 && f.w(0x4858)==0x5678 && f.w(0x4760)==15 && f.h(0x477c)==1);
    assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.frame.eye==CameraDepthPoint({0,110,500}) && f.frame.at==CameraDepthPoint({0,60,0}));
    assert(f.h(0x477c)==3 && f.rumble==std::vector<int32_t>{0});
    f.f(0x40030,-100);f.f(0x42030,-120);f.f(0x4003c,999);f.f(0x4203c,999); // Current positions must not supply home center.
    assert(setBattleCameraMode(0x2000,0,17,s.requests));setBattleCameraOffset(0x2000,0,100,100,100,s.requests);
    assert(updateBattleCameraFrame(0x2000,f.frame,s));
    assert(f.frame.eye==CameraDepthPoint({-7.333333492279052734f,109.33333587646484375f,490}) && f.frame.at==CameraDepthPoint({-7.333333492279052734f,59.333332061767578125f,0}) && f.h(0x477c)==1);
    const float frozenX=f.frame.eye[0];f.w(0x4754,4);assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.frame.eye[0]==frozenX && f.w(0x47bc)==0);
    f.w(0x4754,0);setBattleCameraMode(0x2000,0,18,s.requests);assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.frame.eye[1]<108);
    setBattleCameraMode(0x2000,0,3,s.requests);setBattleCameraMoveTo(0x2000,0,2,4,{0,100,300},{0,50,0},f.frame,s.requests);
    assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.w(0x47d8)==1 && f.steps==std::vector<int32_t>(6,1));
    assert(f.h(0x477c)==1 && f.h(0x477e)==3);
    assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.w(0x47d8)==0 && f.frame.eye==CameraDepthPoint({0,100,300}));
    const auto calls=f.steps.size();assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.steps.size()==calls && f.w(0x4760)==3);
    f.w(0x4754,2);auto missing=s;missing.interpolate=nullptr;BattleCameraTremorCursor continuation;
    assert(!updateBattleCameraFrame(0x2000,f.frame,missing,&continuation));
    assert(continuation.active && continuation.axis==1 && continuation.stage==1 && f.h(0x477c)==3 && f.h(0x477e)==3);
    const auto heldBaseline=f.w(0x4780);assert(updateBattleCameraFrame(0x2000,f.frame,s,&continuation));
    assert(!continuation.active && !(f.w(0x4754)&2) && f.w(0x483c)==0xffffffff && f.w(0x4780)==heldBaseline);
    Fixture shake;auto ss=shake.services();initializeBattleCamera(0x2000,ss.requests);updateBattleCameraFrame(0x2000,shake.frame,ss);
    setBattleCameraMode(0x2000,0,0,ss.requests);shake.w(0x4754,3);
    shake.w(0x4828,0);shake.w(0x482c,0);shake.w(0x4830,6);shake.w(0x4834,6);shake.w(0x4838,6);shake.w(0x483c,4);
    shake.f(0x4840,4);shake.f(0x4844,0);shake.f(0x4848,6);shake.f(0x484c,0);
    assert(updateBattleCameraFrame(0x2000,shake.frame,ss));
    assert(shake.frame.eye[0]==4 && shake.frame.at[0]==4 && shake.frame.eye[1]==106 && shake.frame.at[1]==56);
    assert(shake.w(0x4780)==0 && shake.w(0x4784)==0x42dc0000 && shake.w(0x4798)==0); // Baseline X0/Y110 predates tremor.
    assert(shake.steps==std::vector<int32_t>({0,1}) && shake.h(0x477c)==1 && shake.h(0x477e)==1);
    assert(shake.w(0x4838)==5 && shake.w(0x483c)==3 && shake.w(0x4850)==0x40800000 && shake.w(0x4854)==0x40800000);
    // Zero and negative timers still interpolate/decrement; zero clears only its bit.
    shake.w(0x4838,0);shake.w(0x483c,0xffffffff);const auto callsBefore=shake.steps.size();
    assert(updateBattleCameraFrame(0x2000,shake.frame,ss));
    assert(shake.steps.size()==callsBefore+2 && shake.w(0x4754)==2 && shake.w(0x4838)==0xffffffff && shake.w(0x483c)==0xfffffffe);
    // A missing tail provider cannot replay the baseline or overwrite earlier X shake.
    Fixture resumed;auto rs=resumed.services();initializeBattleCamera(0x2000,rs.requests);updateBattleCameraFrame(0x2000,resumed.frame,rs);
    setBattleCameraMode(0x2000,0,0,rs.requests);resumed.w(0x4754,3);
    resumed.w(0x4830,6);resumed.w(0x4834,6);resumed.w(0x4838,6);resumed.w(0x483c,6);
    resumed.f(0x4840,4);resumed.f(0x4848,6);
    auto unavailable=rs;unavailable.interpolate=[](void* user,int32_t,int32_t step,int32_t duration,float a,float b)->std::optional<float>{
        auto& f=*static_cast<Fixture*>(user);f.steps.push_back(step);if(f.steps.size()==2)return {};return duration==0?b:a+(b-a)*float(step)/float(duration);
    };
    BattleCameraTremorCursor tail;assert(!updateBattleCameraFrame(0x2000,resumed.frame,unavailable,&tail));
    assert(tail.axis==1 && resumed.frame.eye[0]==4 && resumed.w(0x4838)==5 && resumed.h(0x477c)==3);
    const auto baselineZ=resumed.w(0x4788);assert(updateBattleCameraFrame(0x2000,resumed.frame,rs,&tail));
    assert(resumed.frame.eye[0]==4 && resumed.frame.eye[1]==116 && resumed.w(0x4788)==baselineZ && resumed.w(0x4838)==5 && resumed.w(0x483c)==5);

    Fixture intro;auto is=intro.services();intro.w(0x50008,0);initializeBattleCamera(0x2000,is.requests);
    for(unsigned i=0;i<10;++i)assert(updateBattleCameraFrame(0x2000,intro.frame,is));
    assert(intro.w(0x4760)==3 && intro.w(0x47d8)==120 && intro.steps.empty());
    assert(updateBattleCameraFrame(0x2000,intro.frame,is) && intro.steps==std::vector<int32_t>(6,1));
    Fixture attack;auto as=attack.services();initializeBattleCamera(0x2000,as.requests);updateBattleCameraFrame(0x2000,attack.frame,as);
    attack.f(0x4003c,30);attack.f(0x40040,20);attack.w(0x4764,0);attack.w(0x476c,99);
    attack.w(0x40014,0x40400);attack.w(0x40400,0x40800);attack.w(0x40404,0x40500);attack.w(0x40500,1);attack.w(0x40804,0x40900);attack.w(0x40900,2);
    assert(attack.vm.memory.writeHalf(0x400cc,36) && attack.vm.memory.writeHalf(0x400ce,50));
    setBattleCameraMode(0x2000,0,7,as.requests);setBattleCameraSpeed(0x2000,0,3,as.requests);
    assert(updateBattleCameraFrame(0x2000,attack.frame,as) && attack.frame.eye==CameraDepthPoint({30,132,768}) && attack.frame.at==CameraDepthPoint({30,82,18})); // Width compensation1.5; mode7 discards missing part.
    const auto retained=attack.frame.eye;setBattleCameraMode(0x2000,0,10,as.requests);
    assert(updateBattleCameraFrame(0x2000,attack.frame,as) && attack.w(0x4760)==0 && attack.frame.eye==retained); // Missing part resets mode without running default this call.
}

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
        s.interpolate=[](void* u,int32_t,int32_t step,int32_t duration,float a,float b)->std::optional<float>{static_cast<Fixture*>(u)->steps.push_back(step);return a+(b-a)*float(step)/float(duration);};
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
    assert(f.frame.eye==CameraDepthPoint({-22,108,470}) && f.frame.at==CameraDepthPoint({-22,58,0}) && f.h(0x477c)==1);
    f.w(0x4754,4);assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.frame.eye[0]==-22 && f.w(0x47bc)==0);
    f.w(0x4754,0);setBattleCameraMode(0x2000,0,18,s.requests);assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.frame.eye[1]<108);
    setBattleCameraMode(0x2000,0,3,s.requests);setBattleCameraMoveTo(0x2000,0,2,4,{0,100,300},{0,50,0},f.frame,s.requests);
    assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.w(0x47d8)==1 && f.steps==std::vector<int32_t>(6,1));
    assert(f.h(0x477c)==1 && f.h(0x477e)==3);
    assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.w(0x47d8)==0 && f.frame.eye==CameraDepthPoint({0,100,300}));
    const auto calls=f.steps.size();assert(updateBattleCameraFrame(0x2000,f.frame,s) && f.steps.size()==calls && f.w(0x4760)==3);
    f.w(0x4754,2);assert(!updateBattleCameraFrame(0x2000,f.frame,s)); // Tremor remains a real provider.
    Fixture intro;auto is=intro.services();intro.w(0x50008,0);initializeBattleCamera(0x2000,is.requests);
    for(unsigned i=0;i<10;++i)assert(updateBattleCameraFrame(0x2000,intro.frame,is));
    assert(intro.w(0x4760)==3 && intro.w(0x47d8)==120 && intro.steps.empty());
    assert(updateBattleCameraFrame(0x2000,intro.frame,is) && intro.steps==std::vector<int32_t>(6,1));
}

#include "battle_unit_rotation_events.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Args=0x1000,Battle=0x2000,OtherBattle=0x8000,Unit=0x10000;
constexpr uint32_t Global=0x8041eb90,Bias=0x802ee608;
struct F {
 EventScriptAssets original;std::map<uint32_t,uint32_t>mem;std::vector<uint32_t>reads;std::vector<std::pair<uint32_t,uint32_t>>stores;BattleUnitRotationEventServices services;EventEntry event{};unsigned biases=0;bool mutate=false,intercept=false;uint32_t selected=0;
 F(){assert(original.load("romfs/harbor-tutorial-events.pmevt"));mem[Global]=Battle;mem[Args]=2;mem[Args+4]=10;mem[Args+8]=20;mem[Args+12]=30;event.arguments=Args;
  auto&m=services.units.memory;m.user=this;m.word=[](void*p,uint32_t a){return static_cast<F*>(p)->word(a);};m.storeWord=[](void*p,uint32_t a,uint32_t v){auto&f=*static_cast<F*>(p);f.stores.emplace_back(a,v);f.mem[a]=v;if(f.intercept&&a==Unit+0x6c){const float full=360;std::memcpy(&f.mem[a],&full,4);}};
  services.units.user=this;services.units.values.user=this;services.units.values.resolve=[](void*,EventEntry&,int32_t v){return v;};services.units.translate=[](void*,EventEntry&,int32_t id){assert(id==2);return id;};services.units.unit=[](void*p,uint32_t b,int32_t id){auto&f=*static_cast<F*>(p);assert(id==2);f.selected=b;return Unit;};
  auto&pos=services.angle.motion.query.common.position;pos.unit.memory=m;pos.numericUser=this;pos.add=[](void*,float a,float b){return a+b;};services.angle.subtract=[](void*,double a,double b){return a-b;};services.angle.fmod=[](void*,double a,double b){return std::fmod(a,b);};services.angle.motion.roundDouble=[](void*,double v){return float(v);};
  real(Unit+0x6c,350);real(Unit+0x70,-30);real(Unit+0x74,720);
 }
 uint32_t word(uint32_t a){reads.push_back(a);if(a==Bias&&mutate){++biases;if(biases==1)mem[Args+8]=999;if(biases==3)mem[Global]=OtherBattle;}auto i=mem.find(a);if(i!=mem.end())return i->second;uint32_t v=0;assert(original.readWord(a,v));return v;}
 void real(uint32_t a,float v){std::memcpy(&mem[a],&v,4);}float real(uint32_t a){auto bits=mem.at(a);float v;std::memcpy(&v,&bits,4);return v;}
};
void add(){F f;f.mutate=true;assert(battleAddUnitRotateEvent(f.event,f.services)==2);assert(f.selected==Battle&&f.mem[Global]==OtherBattle);assert(f.real(Unit+0x6c)==0&&f.real(Unit+0x70)==350&&f.real(Unit+0x74)==30);std::vector<uint32_t>prefix{Args,Args+4,Args+8,Bias,Bias+4,Args+12,Bias,Bias+4,Global,Bias,Bias+4};assert(std::equal(prefix.begin(),prefix.end(),f.reads.begin()));}
void freshStoredClamp(){F f;f.intercept=true;assert(battleAddUnitRotateEvent(f.event,f.services)==2);assert(f.real(Unit+0x6c)==360); // interceptor sees both successful stores
 assert(std::count_if(f.stores.begin(),f.stores.end(),[](auto v){return v.first==Unit+0x6c;})==2);
 auto i=std::find(f.reads.begin(),f.reads.end(),0x804226c8);assert(i!=f.reads.end()&&i+1!=f.reads.end()&&i[1]==Unit+0x6c);
}
void angles(){for(float v:{-720.f,-360.f,-1.f,-0.f,0.f,360.f,721.f,INFINITY,-INFINITY,NAN}){F f;f.real(Unit+0x6c,v);f.mem[Args+4]=0;assert(battleAddUnitRotateEvent(f.event,f.services)==2);float expected=float(std::fmod(double(v),360.));if(std::isnan(expected))expected=0;if(expected<0)expected+=360;assert(f.real(Unit+0x6c)==expected);}}
void set(){for(unsigned kept=0;kept<3;++kept){F f;for(unsigned axis=0;axis<3;++axis)f.mem[Args+4+4*axis]=axis==kept?uint32_t(-250000000):90;const std::array<float,3>old{350,330,0};assert(battleSetUnitRotateEvent(f.event,f.services)==2);for(unsigned axis=0;axis<3;++axis)assert(f.real(Unit+0x6c+4*axis)==(axis==kept?old[axis]:90));}
 F f;f.mem[Args+4]=uint32_t(-250000000);f.mem[Args+8]=uint32_t(-90);f.mem[Args+12]=720;assert(battleSetUnitRotateEvent(f.event,f.services)==2);assert(f.real(Unit+0x6c)==350&&f.real(Unit+0x70)==270&&f.real(Unit+0x74)==0);}
void priority(){F f;f.mem[Args]=0xfffffff9;f.services.units.values.resolve=[](void*p,EventEntry&,int32_t v){static_cast<F*>(p)->mem[Global]=OtherBattle;return v;};assert(battleCameraPriorityLimitEvent(f.event,f.services.units)==2);assert(f.mem[OtherBattle+0x275c]==0xfffffff9&&!f.mem.count(Battle+0x275c));assert((f.reads==std::vector<uint32_t>{Args,Global}));}
}
int main(){add();freshStoredClamp();angles();set();priority();std::cout<<"whole source AddRotate/camera-prilimit capture/order/math tests PASS\n";}

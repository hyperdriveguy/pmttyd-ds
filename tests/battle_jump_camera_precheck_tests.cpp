#include "battle_event_callbacks.hpp"
#include "battle_camera_requests.hpp"
#include "battle_packed_generic_jump.hpp"
#include "battle_stage_floor.hpp"
#include "battle_packed_hit_precheck.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <set>
#include <vector>
#include <iostream>
using namespace ttyd;
uint32_t bits(float v){uint32_t b;std::memcpy(&b,&v,4);return b;}
struct Fixture {
 EventEntry entry;std::array<EventEntry,1> entries{};std::vector<int32_t> rumble;std::vector<uint32_t> sounds;std::vector<int32_t> draws;size_t cursor=0;std::set<std::pair<uint32_t,int32_t>> statuses;
 static EventNativePlatform platform(){EventNativePlatform p;p.truncate=[](void*,float v)->int32_t{return int32_t(v);};p.signedWordFloat=[](void*,int32_t v){return float(v);};p.divideFloat=[](void*,float a,float b){return a/b;};p.multiplyFloat=[](void*,float a,float b){return a*b;};p.addFloat=[](void*,float a,float b){return a+b;};p.subtractFloat=[](void*,float a,float b){return a-b;};p.fmaFloat=[](void*,float a,float b,float c){return std::fma(a,b,c);};p.negativeMultiplySubtractFloat=[](void*,float a,float b,float c){return -std::fma(a,b,-c);};p.distanceFloat=[](void*,float a,float b,float c,float d){return std::hypot(c-a,d-b);};p.headingFloat=[](void*,float a,float b,float c,float d){return float(std::atan2(double(c-a),double(b-d))*180/3.141592653589793);};p.sineDouble=[](void*,double v){return std::sin(v);};p.cosineDouble=[](void*,double v){return std::cos(v);};p.roundDoubleFloat=[](void*,double v){return float(v);};return p;}
 EventNativeRuntime vm{0x81000000,platform()};
 Fixture(){for(auto span:std::array<std::pair<uint32_t,unsigned>,8>{{{0x1000,0x100},{0x2000,0x19400},{0x40000,0x1000},{0x42000,0x1000},{0x44000,0x1000},{0x8041eb90,4},{0x80422000,0x1000},{0x802ee494,12}}})assert(vm.memory.registerRam(span.first,std::vector<uint8_t>(span.second)));assert(vm.entryAddresses.registerEntries(0x82000000,entries.data(),1));entry.arguments=0x1000;entry.owner=0;entries[0]=entry;w(0x8041eb90,0x2000);w(0x2020,0x40000);w(0x2024,0x42000);w(0x40000,0);w(0x42000,1);w(0x40014,0x44000);w(0x44004,0x44800);w(0x44800,1);w(0x441ac,1);w(0x444ec,0x40000);w(0x40114,bits(1));w(0x42010,0x44880);w(0x80422690,bits(6.2831854820251465f));w(0x80422694,bits(360));w(0x804226c4,bits(-250000000));w(0x804226a8,bits(1));w(0x804222a0,bits(.5f));w(0x804221c4,bits(.5f));w(0x80422b44,bits(90));w(0x80422b48,bits(-50));initializePackedBattleStageFloorFields(0x2000,nativeBattleActorServices(vm).memory);h(0x4210c,10);assert(vm.memory.writeByte(0x4488c,5));}
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}void h(uint32_t a,uint16_t v){assert(vm.memory.writeHalf(a,v));}void b(uint32_t a,uint8_t v){assert(vm.memory.writeByte(a,v));}float f(uint32_t a){const auto x=w(a);float v;std::memcpy(&v,&x,4);return v;}
 void args(std::initializer_list<int32_t> values){entry.argumentCount=values.size();unsigned i=0;for(auto v:values)w(entry.arguments+4*i++,uint32_t(v));}
 BattlePackedGenericJumpServices jump(){BattlePackedGenericJumpServices s;s.movement=nativeBattleMovePositionServices(vm,{});s.user=this;s.play=[](void* p,uint32_t u,uint32_t key,const std::array<float,3>&){assert(u==0x40000);static_cast<Fixture*>(p)->sounds.push_back(key);};s.floor=[](void* p,uint32_t battle,float x,float y,float z){return packedBattleStageFloorHeight(battle,x,y,z,nativeBattleActorServices(static_cast<Fixture*>(p)->vm).memory);};return s;}
 BattleCameraRequestServices camera(){BattleCameraRequestServices s;s.positions=nativeBattlePartPositionServices(vm);s.values=s.positions.unit.values;s.user=this;s.signedWordFloat=[](void*,int32_t v){return float(v);};s.rumbleOn=[](void* p,int32_t channel){static_cast<Fixture*>(p)->rumble.push_back(channel);};return s;}
 BattlePackedHitPrecheckServices hit(){BattlePackedHitPrecheckServices s;s.memory=nativeBattleActorServices(vm);s.user=this;s.status=[](void* p,uint32_t u,int32_t id){return static_cast<Fixture*>(p)->statuses.count({u,id})!=0;};s.random=[](void* p,int32_t bound)->int32_t{auto& f=*static_cast<Fixture*>(p);assert(bound==100 && f.cursor<f.draws.size());return f.draws[f.cursor++];};s.multiply=[](void*,float a,float b){return a*b;};s.truncate=[](void*,float v)->int32_t{return int32_t(v);};return s;}
};
int main(){
 Fixture j;j.args({-2,10,0,0,2,-1});j.w(0x40170,bits(2));j.w(0x40174,bits(1));j.w(0x401c4,31);j.w(0x401c8,32);auto js=j.jump();assert(packedGenericJumpPositionEvent(j.entry,1,js)==0 && j.w(0x4016c)==1 && j.f(0x40040)==1 && j.f(0x40178)==0);assert(j.sounds==std::vector<uint32_t>{31});assert(packedGenericJumpPositionEvent(j.entry,0,js)==1 && j.f(0x4003c)==10 && j.f(0x40040)==0 && j.f(0x40158)==1 && j.f(0x40178)==-1);assert((j.sounds==std::vector<uint32_t>{31,32}));
 // JumpContinue copies snapped visible root, retains derivative, then uses strict floor comparison.
 j.args({-2});assert(packedGenericJumpContinueEvent(j.entry,1,js)==1 && j.f(0x40040)==0 && j.f(0x40178)==-2);
 Fixture zero;zero.args({-2,0,99,0,0,-1});zero.w(0x40170,bits(2));zero.w(0x40174,bits(1));assert(packedGenericJumpPositionEvent(zero.entry,1,zero.jump())==2 && zero.w(0x4016c)==0 && zero.f(0x40040)==0 && zero.f(0x40164)==99 && zero.sounds.empty());
 Fixture equality;equality.args({-2});equality.w(0x40178,bits(0));equality.w(0x40174,bits(1));assert(packedGenericJumpContinueEvent(equality.entry,1,equality.jump())==0 && equality.f(0x40040)==0);assert(packedGenericJumpContinueEvent(equality.entry,0,equality.jump())==1 && equality.f(0x40040)==0);
 // The real source floor provider also preserves equality and clamping on the
 // lower stage tier; it is not an unconditional ground-zero fixture.
 Fixture lower;lower.args({-2});lower.w(0x40040,bits(-50));lower.w(0x40044,bits(91));lower.w(0x40178,bits(0));lower.w(0x40174,bits(1));
 assert(packedGenericJumpContinueEvent(lower.entry,1,lower.jump())==0 && lower.f(0x40040)==-50);
 assert(packedGenericJumpContinueEvent(lower.entry,0,lower.jump())==1 && lower.f(0x40040)==-50);
 Fixture shake;auto shakeServices=shake.camera();shake.w(0x475c,1);shake.w(0x4754,0x80);shake.args({0,3,-4,20,7});assert(battleCameraShakeHorizontalEvent(shake.entry,shakeServices)==2 && shake.w(0x4754)==0x80 && shake.w(0x4848)==0 && shake.rumble.empty());
 shake.args({1,16777217,-4,-3,7});assert(battleCameraShakeHorizontalEvent(shake.entry,shakeServices)==2 && shake.w(0x4754)==0x82 && shake.f(0x4848)==16777216 && shake.f(0x484c)==-4 && shake.w(0x482c)==7 && shake.w(0x4834)==uint32_t(-3) && shake.w(0x483c)==uint32_t(-3) && shake.rumble==std::vector<int32_t>{0});
 shake.w(0x4754,0x40);shake.args({1,3,4,5,6});battleCameraShakeHorizontalEvent(shake.entry,shakeServices);assert(shake.w(0x4754)==0x42 && shake.rumble.size()==1);
 Fixture capture;auto captured=capture.camera();capture.args({0,3,4,5,6});captured.signedWordFloat=[](void* self,int32_t v){auto& f=*static_cast<Fixture*>(self);if(v==3)f.w(0x1008,99);if(v==4){f.w(0x100c,88);f.w(0x8041eb90,0x3000);}return float(v);};battleCameraShakeHorizontalEvent(capture.entry,captured);assert(capture.f(0x5848)==3 && capture.f(0x584c)==4 && capture.w(0x5834)==5 && capture.w(0x4754)==0); // Raw following operands captured before float service; battle reloaded after arguments.
 Fixture c;auto cs=c.camera();c.w(0x2000+0x275c,1);c.w(0x2000+0x2754,0x3c);c.args({0,8});assert(battleCameraModeEvent(c.entry,cs)==2 && c.w(0x2000+0x2760)==0);c.args({1,8});assert(battleCameraModeEvent(c.entry,cs)==2 && c.w(0x2000+0x2760)==8 && c.w(0x2000+0x2754)==0x1c);c.args({1,6});battleCameraSpeedEvent(c.entry,cs);uint16_t level=0;assert(c.vm.memory.readHalf(0x2000+0x277c,level) && level==1);c.args({1,-230000000+205722});battleCameraZoomEvent(c.entry,cs);assert(c.f(0x2000+0x2778)==200);
 c.w(0x40030,bits(25));c.w(0x2000+0x281c,bits(123));c.args({0,-2,-1});battleCameraHomingEvent(c.entry,cs);assert(c.w(0x2000+0x2764)==0 && c.w(0x2000+0x276c)==1 && c.f(0x2000+0x2810)==25 && c.f(0x2000+0x281c)==123); // Homing ignores priority; -1 preserves second vector.
 Fixture h;h.b(0x44110,100);h.draws={99};assert(packedBattleHitPrecheck(0x40000,0x42000,0x44000,0x44100,0,h.hit())==1 && h.cursor==1);h.cursor=0;h.draws={90};h.b(0x422e1,1);assert(packedBattleHitPrecheck(0x40000,0x42000,0x44000,0x44100,0,h.hit())==3 && h.cursor==1);h.cursor=0;h.draws={89,99};assert(packedBattleHitPrecheck(0x40000,0x42000,0x44000,0x44100,0,h.hit())==1 && h.cursor==2);
 h.b(0x422e1,0);h.b(0x44110,99);h.statuses.insert({0x40000,3});h.w(0x2000+0x182bc,1);h.draws={23};h.cursor=0;assert(packedBattleHitPrecheck(0x40000,0x42000,0x44000,0x44100,0,h.hit())==1);h.draws={24};h.cursor=0;assert(packedBattleHitPrecheck(0x40000,0x42000,0x44000,0x44100,0,h.hit())==2);
 h.w(0x44174,4);h.w(0x441ac,0x4000);h.cursor=0;assert(packedBattleHitPrecheck(0x40000,0x42000,0x44000,0x44100,0,h.hit())==2 && h.w(0x4027c)==8 && h.cursor==0);
 std::cout<<"Source generic jump timing/snap/floor, camera request gates and precheck draw boundaries passed\n";
}

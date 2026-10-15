#include <memory>
#include "battle_weapon_draw_continuation.hpp"
#include "battle_weapon_ui_continuation.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
struct Event{uint32_t kind=0;std::vector<uint32_t> data;bool operator==(const Event&o)const{return kind==o.kind&&data==o.data;}};
struct Fixture{
 static constexpr uint32_t B=0x10000,B2=0x30000,U=0x50000,V=0x60000,W=0x70000,C=0x80000,Rows=B+0x179c,Gp=0x90000,Weapon=0xa0000;
 EventScriptAssets originals;std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint16_t> halves;std::map<uint32_t,uint8_t> bytes;
 std::vector<Event> events,ops;std::optional<Event> pendingOp;const BattleWeaponDrawRequest* pendingRequest=nullptr;
 int hold=-1;unsigned failures=0;bool alive=true,probe=false;uint32_t generation=1;
 std::function<void(Fixture&,uint32_t)> onRead,onWrite;
 std::function<void(Fixture&,const Event&)> onOp;
 BattleWeaponDrawCursor cursor;BattleWeaponDrawOutput captured;
 static uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
 static float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
 Fixture(){assert(originals.load("romfs/trio-attack-events.pmevt"));words={{0x8041eb90,B},{0x80415fd0,Gp},{Gp+4,60},{0x8041e80c,0xb0000},{B+0x420,1},{B+0x24,U},{B+0x28,V},{U+8,222},{V+8,224},{C,0},{C+4,0},{C+8,3},{B+0x1bb4,2},{B+0x1bbc,5},{B+0x1bc0,1},{B+0x1bc8,3}};
  for(unsigned i=0;i<7;++i){words[Rows+28*i]=Weapon+0x100*i;words[Rows+28*i+4]=0x1000+i;words[Rows+28*i+8]=1;halves[B+0x171c+28*i+0x8c]=uint16_t(100+i);bytes[Weapon+0x100*i+0x12]=uint8_t(i+1);words[Weapon+0x100*i+8]=0;words[Weapon+0x100*i+0xc]=0x2000+i;}
  BattleWeaponUiServices ui;ui.actors=actors();ui.user=this;ui.allocationCurrent=[](void*){return true;};ui.request=[](void*,const BattleWeaponUiRequest&q)->std::optional<uint32_t>{assert(q.function==0x8002ff58);return W;};BattleWeaponUiCursor setup;assert(resumePackedBattleWeaponUiInit(0xdead,C,Rows,0,ui,setup));events.clear();
 }
 uint32_t word(uint32_t a){uint32_t n=0;const auto it=words.find(a);if(it!=words.end())n=it->second;else if(!originals.readWord(a,n))n=words[a];events.push_back({1,{a,n}});if(onRead)onRead(*this,a);return n;}
 uint16_t half(uint32_t a){const auto n=halves[a];events.push_back({2,{a,n}});if(onRead)onRead(*this,a);return n;}
 uint8_t byte(uint32_t a){const auto n=bytes[a];events.push_back({3,{a,n}});if(onRead)onRead(*this,a);return n;}
 void store(uint32_t a,uint32_t n){words[a]=n;events.push_back({4,{a,n}});if(onWrite)onWrite(*this,a);}
 void put(uint32_t a,uint8_t n){bytes[a]=n;events.push_back({5,{a,n}});if(onWrite)onWrite(*this,a);}
 float real(uint32_t a){return scalar(word(a));}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;a.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};a.memory.half=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};a.byte=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};a.memory.storeWord=[](void*p,uint32_t a,uint32_t n){static_cast<Fixture*>(p)->store(a,n);};a.memory.storeByte=[](void*p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->put(a,n);};return a;}
 static void floats(std::vector<uint32_t>&a,const BattleActMatrix&m){for(float n:m)a.push_back(bits(n));}
 static void snapshot(std::vector<uint32_t>&a,const BattleWeaponDrawOutput&out){a.push_back(uint32_t(out.classes.size()));for(const auto&slot:out.classes){a.push_back(bool(slot.position));if(slot.position)for(float n:*slot.position)a.push_back(bits(n));a.push_back(bool(slot.matrix));if(slot.matrix)floats(a,*slot.matrix);a.push_back(bool(slot.parameter));if(slot.parameter)a.push_back(bits(*slot.parameter));}
  for(const auto&slot:out.weapons){a.push_back(bool(slot.matrix));if(slot.matrix)floats(a,*slot.matrix);for(const auto*v:{&slot.label,&slot.enabled,&slot.cost,&slot.bp}){a.push_back(bool(*v));if(*v)a.push_back(**v);}a.push_back(bool(slot.icon));if(slot.icon)a.push_back(*slot.icon);}}
 bool admit(uint32_t fn,std::vector<uint32_t>a){Event op{fn,std::move(a)};
  if(!pendingOp){pendingOp=op;events.push_back(op);ops.push_back(op);if(onOp)onOp(*this,op);}else assert(*pendingOp==op);
  if(probe){assert(!cursor.reset());assert(!resumePackedBattleWeaponQueuedDraw(8,B,services(),cursor));}
  if(int(ops.size()-1)==hold&&failures++<2)return false;
  pendingOp.reset();return true;
 }
 std::optional<BattleActMatrix> matrix(uint32_t fn,const std::array<float,3>&xyz={},float angle=0,uint32_t axis=0,const BattleActMatrix&left={},const BattleActMatrix&right={}){
  std::vector<uint32_t>a{bits(xyz[0]),bits(xyz[1]),bits(xyz[2]),bits(angle),axis};floats(a,left);floats(a,right);if(!admit(fn,a))return {};
  BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);BattleAudienceMatrixRequest q;q.function=fn;q.xyz=xyz;q.radians=angle;q.axis=axis;q.left=left;q.right=right;assert(math.matrix(nullptr,q));return q.output;
 }
 BattleWeaponDrawServices services(){BattleWeaponDrawServices s;s.user=this;s.math.user=this;s.math.memory=actors();s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
 s.math.center=[](void*p)->std::optional<std::array<float,3>>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(0x80119cf0,{}))return {};return std::array<float,3>{100,50,0};}; // Abstract child contract, no native-camera claim.
 s.math.offset=[](void*p,float angle)->std::optional<std::array<float,2>>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(0x80119c7c,{bits(angle)}))return {};return std::array<float,2>{std::cos(angle)*20,std::sin(angle)*10};};
 s.math.interpolate=[](void*p,int32_t mode,int32_t step,int32_t n,float a,float b)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(0x80006c8c,{uint32_t(mode),uint32_t(step),uint32_t(n),bits(a),bits(b)}))return {};return cameraInterpolation(mode,step,n,a,b);};
 s.math.translation=[](void*p,const std::array<float,3>&xyz){return static_cast<Fixture*>(p)->matrix(0x8029e230,xyz);};s.math.scale=[](void*p,float x,float y,float z){return static_cast<Fixture*>(p)->matrix(0x8029e2b0,{x,y,z});};s.math.rotation=[](void*p,float n,uint32_t a){return static_cast<Fixture*>(p)->matrix(0x8029dff0,{},n,a);};s.math.concat=[](void*p,const BattleActMatrix&a,const BattleActMatrix&b){return static_cast<Fixture*>(p)->matrix(0x8029dd64,{},0,0,a,b);};s.identity=[](void*p){return static_cast<Fixture*>(p)->matrix(0x8029dd04);};
 s.truncate=[](void*p,float v)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(0xf0000001,{bits(v)}))return {};assert(std::isfinite(v)&&v>=-2147483648.0&&v<2147483648.0);return int32_t(v);};
 s.fma=[](void*p,float a,float b,float c)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.admit(0xf0000002,{bits(a),bits(b),bits(c)}))return {};return std::fma(a,b,c);};
 s.request=[](void*p,const BattleWeaponDrawRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);std::vector<uint32_t>a{q.callsite,q.count};a.insert(a.end(),q.arguments.begin(),q.arguments.begin()+q.count);if(q.output){snapshot(a,*q.output);f.captured=*q.output;}if(q.cursorWindow)a.insert(a.end(),q.cursorWindow->begin(),q.cursorWindow->end());if(q.x)a.push_back(bits(*q.x));if(q.y)a.push_back(bits(*q.y));if(f.pendingRequest)assert(f.pendingRequest==&q);else f.pendingRequest=&q;if(!f.admit(q.function,a))return {};f.pendingRequest=nullptr;return q.function==0x80126968?7:0;};return s;}
 bool run(){return resumePackedBattleWeaponQueuedDraw(8,B,services(),cursor);}
};
using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup={},bool every=true){Fixture immediate;if(setup)setup(immediate);immediate.events.clear();assert(immediate.run()&&!immediate.cursor.active());if(!every)return;
 for(unsigned gate=0;gate<immediate.ops.size();++gate){Fixture delayed;if(setup)setup(delayed);delayed.events.clear();delayed.hold=int(gate);delayed.probe=true;assert(!delayed.run()&&delayed.cursor.active());auto events=delayed.events;auto words=delayed.words;auto bytes=delayed.bytes;
  auto bad=delayed.services();bad.math.memory.audienceLifecycle.user=&immediate;assert(!resumePackedBattleWeaponQueuedDraw(8,Fixture::B,bad,delayed.cursor));bad=delayed.services();bad.user=&immediate;assert(!resumePackedBattleWeaponQueuedDraw(8,Fixture::B,bad,delayed.cursor));bad=delayed.services();bad.identity=nullptr;assert(!resumePackedBattleWeaponQueuedDraw(8,Fixture::B,bad,delayed.cursor));bad=delayed.services();bad.math.user=&immediate;assert(!resumePackedBattleWeaponQueuedDraw(8,Fixture::B,bad,delayed.cursor));assert(!resumePackedBattleWeaponQueuedDraw(4,Fixture::B,delayed.services(),delayed.cursor));assert(!resumePackedBattleWeaponQueuedDraw(8,Fixture::B+4,delayed.services(),delayed.cursor));
  assert(!delayed.run()&&delayed.events==events&&delayed.words==words&&delayed.bytes==bytes);assert(delayed.run()&&!delayed.cursor.active());assert(delayed.events==immediate.events&&delayed.words==immediate.words&&delayed.bytes==immediate.bytes);
 }
}
int main(){
 Fixture cold;assert(cold.bytes[Fixture::W+0x14]==0);cold.events.clear();assert(cold.run());assert(cold.bytes[Fixture::W+0x14]==1&&cold.captured.classes.size()==5);for(unsigned i=0;i<5;++i){const auto&slot=cold.captured.classes[i];assert(!slot.position);assert(bool(slot.matrix)==(i==2));}assert(cold.captured.weapons[0].matrix&&cold.captured.weapons[0].cost==7&&cold.captured.weapons[0].bp==1);assert(!cold.captured.weapons[3].label);assert(cold.ops.back().kind==0x80117c1c);
 retained();
 for(uint32_t mode=0;mode<=41;++mode)retained([=](auto&f){f.words[Fixture::W+0xc]=mode;},mode==0||mode==2||mode==10||mode==20||mode==30||mode==40);
 for(uint32_t state:{0u,1u,2u,9u,10u,11u,12u,0xffffffffu})retained([=](auto&f){for(unsigned i=0;i<5;++i)f.words[Fixture::W+0x34+4*i]=state;},true);
 for(uint32_t state=0;state<=41;++state)retained([=](auto&f){for(unsigned i=0;i<7;++i){f.words[Fixture::W+0xc0+4*i]=state;f.words[Fixture::W+0xdc+4*i]=0;}},state==0||state==1||state==2||state==10||state==20||state==30||state==40);
 for(int32_t timer:{-1,0,3,12,13})retained([=](auto&f){f.words[Fixture::W+0x34]=0;f.words[Fixture::W+0x48]=uint32_t(timer);f.words[Fixture::W+0xdc]=uint32_t(timer);});
 retained([](auto&f){f.words[Fixture::C+8]=9;f.words[Fixture::C+4]=3;f.words[Fixture::C]=0;});
 retained([](auto&f){f.words[Fixture::C+8]=9;f.words[Fixture::C+4]=0;f.words[Fixture::C]=8;});
 retained([](auto&f){f.words[Fixture::Weapon+8]=4;f.words[0x803108a8+40*4+8]=0x4444;});
 retained([](auto&f){f.words[Fixture::Rows]=0x80365488;f.words[0x80365488+8]=0;f.words[Fixture::Rows+0x14]=6;f.words[0x803108a8+40*6+8]=0x6666;});
 retained([](auto&f){f.words[Fixture::Rows]=0;f.words[Fixture::Rows+0x14]=6;f.words[0x803108a8+40*6+8]=0;});
 // The original cursor count is loaded BEFORE fctiwz. A held conversion
 // may mutate live count, but cannot retroactively replace that captured row.
 Fixture countCapture;bool changed=false;countCapture.onOp=[&](auto&f,const Event&e){if(e.kind==0xf0000001&&!changed){changed=true;f.words[Fixture::C+8]=0;}};assert(countCapture.run());assert(countCapture.captured.weapons[0].label&& !countCapture.captured.weapons[1].label);
 Fixture lateBp;bool costChanged=false;lateBp.onOp=[&](auto&f,const Event&e){if(e.kind==0x80126968&&!costChanged){costChanged=true;f.words[Fixture::Rows]=Fixture::Weapon+0x100;}};assert(lateBp.run());assert(lateBp.captured.weapons[0].bp==2&&lateBp.captured.weapons[0].label==0x1000); // Fresh post-cost weapon load.
 for(const auto&transition:std::array<std::array<uint32_t,4>,4>{{{{3,0,9,30}},{{0,8,9,40}},{{0,5,9,10}},{{3,3,9,20}}}}){Fixture scroll;scroll.words[Fixture::C+4]=transition[0];scroll.words[Fixture::C]=transition[1];scroll.words[Fixture::C+8]=transition[2];assert(scroll.run());assert(scroll.words[Fixture::W+0xc]==transition[3]);}
 Fixture held;held.hold=0;assert(!held.run());held.alive=false;assert(!held.run()&&held.cursor.stale());auto events=held.events;held.alive=true;assert(!held.run()&&held.events==events);assert(held.cursor.reset());
 Fixture unavailable;auto s=unavailable.services();s.request=nullptr;assert(!resumePackedBattleWeaponQueuedDraw(8,Fixture::B,s,unavailable.cursor)&&unavailable.cursor.pending()==0x80126968);assert(unavailable.bytes[Fixture::W+0x14]==1); // Genuine prefix never means whole completion.
 std::cout<<"Whole queued weapon draw branch/control traces, actual private unknowns, every-child holds and ownership passed\n";
}

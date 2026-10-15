#include "animation_main.hpp"
#include "animation_facing_binding.hpp"
#include "native_field_animation.hpp"
#include "field_party.hpp"
#include "field_camera_publication.hpp"
#include "field_camera.hpp"
#include <cassert>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t W=0x90000000,B=0x91000000,J=0x92000000,R=B+0x170*3,E=0x93000000,H=0x94000000,D=0x95000000,A=0x96000000,T=0x97000000;
struct Fixture {
 std::map<uint32_t,uint32_t> words{{0x80415dc0,W},{W+0x10,B},{W,J},{R+0x10,2},{R+0x14,5},{J+40,E},{E+0xa0,H},{H,D},{D+0x1ac,A},{A+320+0x3c,T},{W+0xe8,0},{R+0x4c,0x1110},{R+0x54,0x2220},{W+0xf8,0x3330},{W+0xfc,0x4440},{R+0x90,0xffffffff},{B+0x170*7+4,0},{R,0}};
 std::vector<std::string> trace;unsigned calls=0,hold=0;bool held=false,mutate=false,evalWait=false,linkedWait=false;
 unsigned evaluations=0,links=0;AnimationMainEvaluation capture;
 bool admit(std::string op){++calls;if(hold==calls && !held){held=true;return false;}trace.push_back(op);return true;}
 AnimationMainServices services(){
  AnimationMainServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(!f.admit("r"+std::to_string(a)))return {};auto i=f.words.find(a);assert(i!=f.words.end());return i->second;};
  s.gqr=[](void* u,uint32_t reg,uint32_t value){auto& f=*static_cast<Fixture*>(u);return f.admit("q"+std::to_string(reg)+"="+std::to_string(value));};
  s.linked=[](void* u,uint32_t h){auto& f=*static_cast<Fixture*>(u);assert(h==7);if(f.linkedWait){f.linkedWait=false;++f.links;return false;}if(!f.admit("linked"))return false;++f.links;if(f.mutate){f.words[0x80415dc0]=0;f.words[W+0x10]=0;f.words[R+0x4c]=0;f.words[R+0x54]=0;f.words[A+320+0x3c]=0;f.words[H]=0;f.words[R+0x90]=99;}return true;};
  s.evaluate=[](void* u,const AnimationMainEvaluation& request){auto& f=*static_cast<Fixture*>(u);if(f.evalWait){f.evalWait=false;++f.evaluations;f.capture=request;return false;}if(!f.admit("evaluate"))return false;++f.evaluations;f.capture=request;return true;};
  s.facing.user=this;s.facing.word=s.word;return s;
 }
 bool run(AnimationMainCursor& c){return advanceAnimationMain(3,services(),c);}
};
void orderAndHolds(){
 Fixture f;AnimationMainCursor c;assert(f.run(c) && !c.active && f.evaluations==1);
 const std::vector<std::string> expected={"r"+std::to_string(0x80415dc0u),"r"+std::to_string(W+0x10),"r"+std::to_string(W),"r"+std::to_string(R+0x10),"r"+std::to_string(R+0x14),"r"+std::to_string(J+40),"r"+std::to_string(E+0xa0),"r"+std::to_string(H),"r"+std::to_string(D+0x1ac),"r"+std::to_string(A+320+0x3c),"q914=262148","q915=327685","q916=393222","q917=458759","r"+std::to_string(W+0xe8),"r"+std::to_string(R+0x4c),"r"+std::to_string(R+0x54),"r"+std::to_string(R+0x90),"evaluate","r"+std::to_string(R)};
 assert(f.trace==expected);assert(f.capture.work==W && f.capture.pool==B && f.capture.pose==R && f.capture.model==D && f.capture.timeline==T && f.capture.positions==0x1110 && f.capture.normals==0x2220);
 for(bool global:{false,true})for(bool linked:{false,true})for(bool null:{false,true})for(bool skip:{false,true}){
  Fixture base;base.words[W+0xe8]=global?0x80000000:0;base.words[R+0x90]=linked?7:0xffffffff;base.words[A+320+0x3c]=null?0:T;base.words[B+0x170*7+4]=skip?2:0;AnimationMainCursor done;assert(base.run(done));const unsigned count=base.calls;
  for(unsigned hold=1;hold<=count;++hold){Fixture held;held.words=base.words;held.hold=hold;AnimationMainCursor cursor;assert(!held.run(cursor) && cursor.active && held.held);auto other=held.services();other.user=&base;assert(!advanceAnimationMain(3,other,cursor));assert(!advanceAnimationMain(4,held.services(),cursor));assert(held.run(cursor) && !cursor.active && held.trace==base.trace);assert(held.evaluations==unsigned(!null));assert(held.links==unsigned(linked && !skip));}
 }
}
void captureAcrossChildren(){
 Fixture f;f.words[R+0x90]=7;f.mutate=true;f.linkedWait=true;f.evalWait=true;AnimationMainCursor c;
 assert(!f.run(c) && c.phase==19 && f.links==1 && !f.evaluations);
 assert(!f.run(c) && c.phase==21 && f.links==2 && f.evaluations==1);
 assert(f.capture.pose==R && f.capture.model==D && f.capture.timeline==T && f.capture.positions==0x1110 && f.capture.normals==0x2220);
 assert(f.run(c) && f.links==2 && f.evaluations==2);
 // A child's false may retain progress; the wrapper never repeats its prefix.
 unsigned gqr=0;for(const auto& op:f.trace)if(op[0]=='q')++gqr;assert(gqr==4);
 Fixture null;null.words[A+320+0x3c]=0;null.words[R+0x90]=7;null.linkedWait=true;AnimationMainCursor n;assert(!null.run(n) && n.phase==19);assert(null.run(n) && null.links==2 && !null.evaluations);
 Fixture global;global.words[W+0xe8]=1;global.words[R+0x90]=7;global.mutate=true;AnimationMainCursor g;assert(global.run(g));assert(global.capture.positions==0x3330 && global.capture.normals==0x4440);
}
void missingOwnersAndModulo(){
 Fixture f;AnimationMainCursor c;auto s=f.services();s.gqr=nullptr;assert(!advanceAnimationMain(3,s,c) && c.phase==10);auto repaired=f.services();assert(!advanceAnimationMain(3,repaired,c)); // Provider identity is pinned.
 Fixture missing;AnimationMainCursor e;auto es=missing.services();es.evaluate=nullptr;assert(!advanceAnimationMain(3,es,e) && e.phase==21 && !missing.evaluations);
 Fixture wrap;AnimationMainCursor w;auto ws=wrap.services();wrap.words[W+0x10]=0x170;const uint32_t pose=0;wrap.words[pose+0x10]=2;wrap.words[pose+0x14]=5;wrap.words[pose+0x4c]=0x1110;wrap.words[pose+0x54]=0x2220;wrap.words[pose+0x90]=0xffffffff;wrap.words[pose]=0;
 assert(advanceAnimationMain(0xffffffff,ws,w) && wrap.capture.pose==0); // Original 32-bit mulli/add wrap, no invented handle guard.
}
void actualSourceSampling(){
 assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 auto native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);auto pool=native->pool();
 const auto handle=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(handle && pool->selectName(*handle,"M_W_1",true,{}));
 for(unsigned i=0;i<9;++i){assert(native->tick(false));}
 BattlePoseClock clock;assert(native->clock(clock));
 auto* pose=pool->pose(*handle);assert(pose && pose->sourceBuffers && pool->evaluate(*handle,clock));
 const auto ordinary=pose->state;const auto positions=pose->shape.positions;const auto normals=pose->shape.normals;
 pose->word(0,pose->word(0)|8);assert(pool->evaluate(*handle,clock));
 for(unsigned i=4;i<ordinary.size();++i)assert(pose->state[i]==ordinary[i]);
 assert(pose->shape.positions.size()==positions.size() && pose->shape.normals.size()==normals.size());
 for(unsigned i=0;i<positions.size();++i)assert(pose->shape.positions[i]==positions[i]);
 for(unsigned i=0;i<normals.size();++i)assert(pose->shape.normals[i]==normals[i]);
 auto cameras=std::make_shared<CameraManager>();cameras->initializeBoot(608,480);assert(publishNativeFieldCamera(*cameras,adaptedFieldCameraFrame(0,0,0)));
 auto& memory=native->process().vm().memory;bool current=true;AnimationFacingAddressPolicy address;address.base=0x81900000;address.unmapped=[&](uint32_t a,uint32_t n){for(uint32_t i=0;i<n;++i){uint8_t v;if(memory.readByte(a+i,v))return false;}return true;};
 // The process exposes its actual pool aliases, not a second detached view.
 auto facing=AnimationFacingBinding::bind(pool,memory,*native->process().animationMemory(),cameras,address,[&](){return current;});assert(facing);
 struct Context {EventAddressSpace* memory;BattlePosePool* pool;BattlePoseClock clock;int32_t handle;bool* current;unsigned evaluations=0,gqrs=0;AnimationMainEvaluation captured;};
 Context context{&memory,pool.get(),clock,*handle,&current,0,0,{}};AnimationMainServices services;services.user=&context;
 services.word=[](void* u,uint32_t a)->std::optional<uint32_t>{uint32_t v;return static_cast<Context*>(u)->memory->readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 services.gqr=[](void* u,uint32_t reg,uint32_t v){auto& x=*static_cast<Context*>(u);assert(reg==914+x.gqrs && v==0x40004+x.gqrs*0x10001);++x.gqrs;return true;}; // Explicit platform fixture.
 services.evaluate=[](void* u,const AnimationMainEvaluation& r){auto& x=*static_cast<Context*>(u);++x.evaluations;x.captured=r;auto* p=x.pool->pose(x.handle);assert(p && r.positions==p->word(0x4c) && r.normals==p->word(0x54));assert(x.pool->evaluate(x.handle,x.clock));*x.current=false;return true;}; // Assert this synchronous admitted prefix completed; never retry failure.
 services.facing=facing->services();AnimationMainCursor cursor;
 assert(!advanceAnimationMain(uint32_t(*handle),services,cursor) && cursor.phase==22 && context.evaluations==1 && context.gqrs==4);
 const auto sampled=pose->state;const auto epoch=pose->timestamp();pose->word(0x18,0);pose->word(0x1c,0);current=true;
 assert(advanceAnimationMain(uint32_t(*handle),services,cursor) && context.evaluations==1 && !cursor.active);
 for(unsigned i=0x20;i<0x70;++i)assert(pose->state[i]==sampled[i]);
 pose->word(0x18,uint32_t(epoch>>32));pose->word(0x1c,uint32_t(epoch));
 // Source null-descriptor exit still follows platform setup and skips both
 // sampling and paper facing, preserving the previous sign.
 auto read=[&](uint32_t a){uint32_t v;assert(memory.readWord(a,v));return v;};
 const auto directory=read(context.captured.model+0x1ac),slot=directory+64*pose->word(0x14)+0x3c,timeline=read(slot);
 assert(memory.writeWord(slot,0));context.gqrs=0;const auto sign=pose->word(0x78);current=false;
 assert(advanceAnimationMain(uint32_t(*handle),services,cursor) && context.evaluations==1 && context.gqrs==4 && pose->word(0x78)==sign);
 assert(memory.writeWord(slot,timeline));
}

}
int main(){orderAndHolds();captureAcrossChildren();missingOwnersAndModulo();actualSourceSampling();}

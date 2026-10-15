#include "animation_main_interpolation.hpp"
#include "animation_main_replay_binding.hpp"
#include "animation_main_time_binding.hpp"
#include "native_field_animation.hpp"
#include "field_party.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t R=0x90000000,D=0x91000000,T=0x92000000,K=0x93000000;
constexpr uint32_t CO[]={0xf0,0xf8,0x13c,0x140,0x12c},PO[]={0x48,0x50,0x58,0x60,0x68},BO[]={0x154,0x15c,0x1a0,0x1a4,0x190},KO[]={4,12,28,36,20},SO[]={0x2c,0x30,0x38,0x3c,0x34},DW[]={12,12,1,4,24},SW[]={4,4,2,4,12};
uint32_t bits(float f){uint32_t b;std::memcpy(&b,&f,4);return b;}
float real(uint32_t b){float f;std::memcpy(&f,&b,4);return f;}
struct Op{char kind;uint32_t address,value;bool operator==(const Op& x) const{return kind==x.kind && address==x.address && value==x.value;}};
struct Fixture {
 std::map<uint32_t,uint8_t> memory;std::vector<Op> trace;unsigned calls=0,hold=0;bool held=false;unsigned entries=0;void (*after)(Fixture&,char,uint32_t)=nullptr;
 AnimationMainEvaluation input{0,0,R,D,T,0,0};
 uint32_t cache(unsigned c) const{return 0x94000000+0x1000*c;}uint32_t base(unsigned c) const{return 0x95000000+0x1000*c;}uint32_t stream(unsigned c) const{return 0x96000000+0x1000*c;}
 void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)memory[a+i]=uint8_t(v>>(24-8*i));}
 uint32_t word(uint32_t a) const{uint32_t v=0;for(unsigned i=0;i<4;++i){auto p=memory.find(a+i);assert(p!=memory.end());v=v<<8|p->second;}return v;}
 bool has(uint32_t a,unsigned n) const{for(unsigned i=0;i<n;++i)if(!memory.count(a+i))return false;return true;}
 explicit Fixture(unsigned n=1):entries(n){word(R+0x28,0);word(R+0x2c,1);word(R+0x3c,0xffffffff);word(R+0x30,bits(.5f));word(R+0x40,bits(.75f));word(T+0x28,K);word(0x8041fbac,bits(.0625f));word(0x8041fb28,0);
  for(unsigned c=0;c<5;++c){word(D+CO[c],n);word(D+BO[c],base(c));word(R+PO[c],cache(c));word(T+SO[c],stream(c));for(unsigned key=0;key<3;++key){word(K+44*key+KO[c],0);word(K+44*key+KO[c]+4,n);}
   for(unsigned i=0;i<(n?n:1)*DW[c];++i){memory[base(c)+i]=0;memory[cache(c)+i]=0;}
   for(unsigned i=0;i<n;++i){const uint32_t a=base(c)+DW[c]*i;for(unsigned lane=0;lane<DW[c]/4;++lane)word(a+4*lane,bits(float(i+lane+1)));if(c==2 || c==4)memory[a]=250;
    const uint32_t p=stream(c)+SW[c]*i;for(unsigned b=0;b<SW[c];++b)memory[p+b]=0;memory[p]=i?1:0;memory[p+1]=c==2 || c==4?7:1;if(c<2){memory[p+2]=254;memory[p+3]=3;}if(c==4){word(p+4,bits(.25f));word(p+8,bits(.5f));}
   }
  }
 }
 bool admit(char kind,uint32_t a,uint32_t v=0){++calls;if(calls==hold && !held){held=true;return false;}trace.push_back({kind,a,v});return true;}
 AnimationMainReplayServices services(){AnimationMainReplayServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(!f.has(a,4) || !f.admit('w',a))return {};const auto v=f.word(a);if(f.after)f.after(f,'w',a);return v;};
  s.store=[](void* u,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit('W',a,v))return false;f.word(a,v);if(f.after)f.after(f,'W',a);return true;};
  s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(u);if(!f.has(a,1) || !f.admit('b',a))return {};auto v=f.memory[a];if(f.after)f.after(f,'b',a);return v;};
  s.storeByte=[](void* u,uint32_t a,uint8_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit('B',a,v))return false;f.memory[a]=v;if(f.after)f.after(f,'B',a);return true;};
  s.quantized=[](void* u,uint32_t a)->std::optional<float>{auto& f=*static_cast<Fixture*>(u);if(!f.has(a,1) || !f.admit('q',a))return {};const uint8_t v=f.memory[a];if(f.after)f.after(f,'q',a);return float(v<128?int(v):int(v)-256);};return s;
 }
 bool run(AnimationMainReplayCursor& c,AnimationMainReplayResult& out){return advanceAnimationMainReplay(input,services(),c,out);}
 void finish(AnimationMainReplayCursor& c,AnimationMainReplayResult& out){for(unsigned i=0;i<2000;++i)if(run(c,out))return;assert(false);}
 void baseCache(){for(unsigned c=0;c<5;++c)for(unsigned i=0;i<entries*DW[c];++i)memory[cache(c)+i]=memory[base(c)+i];}
};
constexpr uint32_t P=0x97000000,N=0x97100000,O=0x97200000;
float rounded(float f){volatile float v=f;return v;}
struct Interpolation:Fixture {
 AnimationMainReplayResult captured;
 explicit Interpolation(unsigned n=1):Fixture(n){input.positions=P;input.normals=N;word(R+0x64,O);word(K,bits(1));word(K+44,bits(3));word(0x8041fb64,bits(2));word(0x8041fbb0,bits(3));word(0x8041fb98,bits(-2));baseCache();captured={0,1,.5f,{n,n,n,n,n}};
  for(int i=-39;i<=216;++i)word(0x803d9580u+uint32_t(i)*4u,bits(float(i-89)/32));
  for(unsigned i=0;i<(n?n:1)*12;++i){memory[P+i]=0;memory[N+i]=0;memory[O+i]=0;}
  for(unsigned i=0;i<n;++i){memory[stream(3)+4*i+2]=1;memory[stream(3)+4*i+3]=254;}
 }
 bool run(AnimationMainInterpolationCursor& c){return advanceAnimationMainInterpolation(input,captured,services(),c);}
 void finish(AnimationMainInterpolationCursor& c){for(unsigned i=0;i<3000;++i)if(run(c))return;assert(false);}
};
float expectedNode(float old,float fraction,float duration,float delta,float a,float b){const float square=rounded(fraction*fraction),cube=rounded(fraction*square);const float h10=rounded(fraction+-std::fma(2.f,square,-cube));const float h01=std::fma(-2.f,cube,rounded(3.f*square));float term=rounded(h10*rounded(duration*a));term=std::fma(h01,rounded(.0625f*delta),term);term=std::fma(rounded(cube-square),rounded(duration*b),term);return rounded(old+term);}
void valuesAndHolds(){
 for(unsigned n:{0u,1u,3u,4u,5u,7u,8u,9u})for(int path:{-1,0,1}){Interpolation f(n);if(path==0)f.captured.next=0;else if(path<0){f.captured.current=1;f.captured.next=0;}AnimationMainInterpolationCursor c;f.finish(c);assert(!c.active && f.word(R+0x3c)==f.captured.current && f.word(R+0x40)==bits(.5f));
  if(path>=0)for(unsigned i=0;i<n;++i){for(unsigned lane=0;lane<3;++lane){const float delta=lane==0?1.f:lane==1?-2.f:3.f;assert(f.word(P+12*i+4*lane)==bits(float(i+lane+1)+(path?delta*.03125f:0)));assert(f.word(N+12*i+4*lane)==bits(float(i+lane+1)+(path?delta*.03125f:0)));}const float old=float(i+1);assert(f.word(O+4*i)==bits(path?expectedNode(old,.5f,2.f,1.f,1.f/32,-2.f/32):old));}
  else assert(f.trace==std::vector<Op>({{'W',R+0x3c,1},{'W',R+0x40,bits(.5f)}}));
  if(n!=5)continue;
  for(unsigned hold=1;hold<=f.calls;++hold){Interpolation test(n);test.captured=f.captured;test.hold=hold;AnimationMainInterpolationCursor cursor;for(unsigned i=0;!test.held && i<3000;++i)assert(!test.run(cursor));assert(test.held);auto changed=test.captured;changed.fraction=.75f;const auto before=test.trace;assert(!advanceAnimationMainInterpolation(test.input,changed,test.services(),cursor) && test.trace==before);auto input=test.input;input.positions+=4;assert(!advanceAnimationMainInterpolation(input,test.captured,test.services(),cursor));auto other=test.services();other.user=&f;assert(!advanceAnimationMainInterpolation(test.input,test.captured,other,cursor));test.finish(cursor);assert(test.trace==f.trace && test.memory==f.memory);}
 }
}
size_t find(const Fixture& f,char kind,uint32_t a,size_t start=0){for(size_t i=start;i<f.trace.size();++i)if(f.trace[i].kind==kind && f.trace[i].address==a)return i;assert(false);return 0;}
void exactReadsAndAliases(){
 Interpolation empty(0);AnimationMainInterpolationCursor e;empty.finish(e);std::vector<Op> expected{{'w',T+0x28,0},{'w',0x8041fbac,0},{'w',T+0x2c,0},{'w',K+48,0},{'w',K+52,0},{'w',R+0x48,0},{'w',K+56,0},{'w',K+60,0},{'w',T+0x30,0},{'w',R+0x50,0},{'w',T+0x28,0},{'w',K+80,0},{'w',K+84,0},{'w',K+44,0},{'w',K,0},{'w',T+0x3c,0},{'w',R+0x60,0},{'w',R+0x64,0},{'W',R+0x3c,0},{'W',R+0x40,bits(.5f)}};assert(empty.trace==expected);
 Interpolation equal(1);equal.captured.next=0;AnimationMainInterpolationCursor eq;equal.finish(eq);assert(find(equal,'w',R+0x64)<find(equal,'w',R+0x60));
 Interpolation f(1);AnimationMainInterpolationCursor c;f.finish(c);assert(find(f,'w',R+0x60)<find(f,'w',R+0x64));size_t q=find(f,'q',f.stream(0)+1);assert(f.trace[q+1].kind=='q' && f.trace[q+2].kind=='q' && f.trace[q+3].kind=='w' && f.trace[q+3].address==P);
 const uint32_t s=f.stream(3);std::vector<Op> node(f.trace.begin()+find(f,'b',s+3),f.trace.end());std::vector<Op> nodeExpected{{'b',s+3,0},{'b',s,0},{'q',s+1,0},{'b',s+2,0},{'w',0x8041fb64,0},{'w',0x8041fbb0,0},{'w',0x803d9580+90*4,0},{'w',0x8041fb98,0},{'w',0x8041fbac,0},{'w',0x803d9580+87*4,0},{'w',O,0},{'W',O,f.word(O)},{'W',R+0x3c,0},{'W',R+0x40,bits(.5f)}};assert(node==nodeExpected);
 Interpolation retained(1);const uint32_t NEW=K+0x10000;retained.word(NEW,bits(5));retained.word(NEW+44,bits(10));retained.after=[](Fixture& x,char kind,uint32_t a){if(kind=='W' && a==P && x.word(a)==bits(1.03125f)){x.word(T+0x28,K+0x10000);x.word(R+0x28,99);x.word(R+0x2c,99);x.word(R+0x30,bits(.875f));x.word(D+CO[1],0);x.word(R+0x4c,0);x.word(R+0x54,0);x.memory[x.stream(0)+2]=100;}};AnimationMainInterpolationCursor rc;retained.finish(rc);assert(retained.word(P+4)==bits(1.9375f) && retained.word(N)==bits(1.03125f));assert(retained.word(O)==bits(expectedNode(1,.5f,5,1,1.f/32,-2.f/32)));assert(retained.word(R+0x3c)==0 && retained.word(R+0x40)==bits(.5f));
}
void nodeGatesAndAdjacent(){
 for(int angle:{-128,-90,-89,-88,0,88,89,90,127}){Interpolation f(2);f.memory[f.stream(3)+3]=uint8_t(angle);f.memory[f.stream(3)]=1;f.memory[f.stream(3)+4]=0;f.memory[f.stream(3)+6]=128;f.memory[f.stream(3)+7]=0;AnimationMainInterpolationCursor c;f.finish(c);assert(f.word(O)==bits(1));const float first=angle<=-89 || angle>=89?2:expectedNode(2,.5f,2,1,1.f/32,float(angle)/32);assert(f.word(O+4)==bits(expectedNode(first,.5f,2,1,-4,0)));if(angle<=-89 || angle>=89)assert(find(f,'b',f.stream(3)+3)+2==find(f,'b',f.stream(3)+7));assert(find(f,'w',0x803d94e4)<find(f,'w',O+4,find(f,'b',f.stream(3)+7)));}
 Interpolation missing(1);missing.memory[missing.stream(3)+2]=127;const uint32_t adjacent=0x803d98e0;for(unsigned i=0;i<4;++i)missing.memory.erase(adjacent+i);AnimationMainInterpolationCursor mc;for(unsigned i=0;i<1000 && mc.phase!=56;++i)assert(!missing.run(mc));assert(mc.phase==56);const auto trace=missing.trace;assert(!missing.run(mc) && missing.trace==trace && missing.word(R+0x3c)==0xffffffff);missing.word(adjacent,bits(2));missing.finish(mc);assert(missing.word(O)==bits(expectedNode(1,.5f,2,1,2,-2.f/32)));
 // Zero model counts skip copies, not stream processing or node output reads.
 Interpolation zero(1);zero.captured.counts={};zero.word(P,bits(10));zero.word(P+4,bits(20));zero.word(P+8,bits(30));zero.word(N,bits(10));zero.word(N+4,bits(20));zero.word(N+8,bits(30));zero.word(O,bits(10));AnimationMainInterpolationCursor z;zero.finish(z);assert(zero.word(P)==bits(10.03125f) && zero.word(O)==bits(expectedNode(10,.5f,2,1,1.f/32,-2.f/32)));
 // Source word helper's shifted zero count executes first copy then wraps.
 Interpolation wrap(0);wrap.captured.current=wrap.captured.next=0;wrap.captured.counts[3]=0x40000000;AnimationMainInterpolationCursor w;assert(!wrap.run(w) && w.phase==4 && w.copy.remaining==0xffffffff);
 // Signed comparison determines branch even across the unsigned boundary.
 Interpolation signedOrder(0);signedOrder.captured.current=0x7fffffff;signedOrder.captured.next=0x80000000;AnimationMainInterpolationCursor so;signedOrder.finish(so);assert(signedOrder.trace.size()==2);
}
void freshConstantsAndIdentity(){
 Interpolation fresh(2);fresh.after=[](Fixture& x,char kind,uint32_t a){if(kind=='W' && a==O && x.word(a)==bits(expectedNode(1,.5f,2,1,1.f/32,-2.f/32))){x.word(0x8041fb64,bits(4));x.word(0x8041fbb0,bits(5));x.word(0x8041fb98,bits(-6));x.word(0x8041fbac,bits(.5f));x.word(0x803d9580+90*4,bits(3));x.word(0x803d9580+87*4,bits(4));}};AnimationMainInterpolationCursor fc;fresh.finish(fc);assert(fresh.word(O+4)==bits(-1));
 Interpolation gate(1);gate.after=[](Fixture& x,char kind,uint32_t a){if(kind=='b' && a==x.stream(3))x.memory[x.stream(3)+3]=89;};AnimationMainInterpolationCursor gc;gate.finish(gc);assert(gate.word(O)==bits(expectedNode(1,.5f,2,1,1.f/32,-2.f/32)));
 for(uint32_t fraction:{0u,0x80000000u,0x7fc12345u}){Interpolation f(0);f.captured.current=1;f.captured.next=0;f.captured.fraction=real(fraction);f.hold=2;AnimationMainInterpolationCursor c;assert(!f.run(c) && c.phase==91 && f.trace.size()==1);auto changed=f.captured;changed.fraction=real(fraction^0x80000000);assert(!advanceAnimationMainInterpolation(f.input,changed,f.services(),c) && f.trace.size()==1);f.finish(c);assert(f.word(R+0x40)==fraction && f.trace.size()==2);}
}

struct LiveHold {
 AnimationMainReplayServices base;AnimationMainGeometryCursor* cursor=nullptr;uint32_t finalAddress=0;bool held=false;unsigned stores=0;
 AnimationMainReplayServices services(){AnimationMainReplayServices s;s.user=this;
  s.word=[](void* u,uint32_t a){auto& x=*static_cast<LiveHold*>(u);return x.base.word(x.base.user,a);};
  s.store=[](void* u,uint32_t a,uint32_t v){auto& x=*static_cast<LiveHold*>(u);if(a==x.finalAddress && x.cursor->phase==1 && !x.held){x.held=true;return false;}const bool result=x.base.store(x.base.user,a,v);if(result)++x.stores;return result;};
  s.byte=[](void* u,uint32_t a){auto& x=*static_cast<LiveHold*>(u);return x.base.byte(x.base.user,a);};
  s.storeByte=[](void* u,uint32_t a,uint8_t v){auto& x=*static_cast<LiveHold*>(u);return x.base.storeByte(x.base.user,a,v);};
  s.quantized=[](void* u,uint32_t a){auto& x=*static_cast<LiveHold*>(u);return x.base.quantized(x.base.user,a);};return s;
 }
};
void actualGeometry(){
 assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};auto native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);auto pool=native->pool();auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h && pool->selectName(*h,"M_W_1",true,{}));for(unsigned i=0;i<17;++i)assert(native->tick(false));
 auto& memory=native->process().vm().memory;auto read=[&](uint32_t a){uint32_t v;assert(memory.readWord(a,v));return v;};auto* pose=pool->pose(*h);BattlePoseClock clock;assert(native->clock(clock) && pool->evaluate(*h,clock));const auto* aliases=native->process().animationMemory();const auto work=read(0x80415dc0),model=read(read(read(read(work)+16*pose->word(0x10)+8)+0xa0)),timeline=read(read(model+0x1ac)+64*pose->word(0x14)+0x3c);AnimationMainEvaluation input{work,read(work+0x10),aliases->poseAddress(unsigned(*h)),model,timeline,pose->word(0x4c),pose->word(0x54)};
 std::array<std::vector<uint8_t>,8> expected;std::array<uint32_t,8> pointers{},lengths{};for(unsigned c=0;c<5;++c){pointers[c]=pose->word(PO[c]);lengths[c]=read(model+CO[c])*DW[c];}pointers[5]=input.positions;pointers[6]=input.normals;pointers[7]=pose->word(0x64);lengths[5]=lengths[0];lengths[6]=lengths[1];lengths[7]=lengths[3];for(unsigned c=0;c<8;++c){expected[c].resize(lengths[c]);for(unsigned i=0;i<lengths[c];++i)assert(memory.readByte(pointers[c]+i,expected[c][i]));}
 const auto expectedCurrent=pose->word(0x3c),expectedFraction=pose->word(0x40);bool current=true;std::optional<uint32_t> gqr=0;auto binding=AnimationMainReplayBinding::bind(pool,memory,*aliases,[&](){return current;},[&](){return gqr;});auto time=AnimationMainTimeBinding::bind(pool,memory,*aliases,[&](){return current;});assert(binding && time);pose->word(0x3c,0xffffffff);pose->scalar(0x40,.875f);AnimationMainGeometryCursor cursor;LiveHold live{binding->services(),&cursor,input.pose+0x40};auto services=live.services();bool quantizedHold=false;
 for(unsigned i=0;i<20000;++i){assert(!advanceAnimationMainGeometry(input,time->services(),services,cursor));if(cursor.phase==0 && cursor.cache.phase==1 && (cursor.cache.replay.phase==41 || cursor.cache.replay.phase==50)){quantizedHold=true;break;}}assert(quantizedHold);gqr=0x00060006;
 for(unsigned i=0;i<20000 && !live.held;++i){assert(!advanceAnimationMainGeometry(input,time->services(),services,cursor));}
 assert(live.held && cursor.phase==1 && cursor.interpolation.phase==91 && pose->word(0x3c)==expectedCurrent && pose->scalar(0x40)==.875f);
 for(unsigned c=0;c<8;++c)for(unsigned i=0;i<lengths[c];++i){uint8_t value;assert(memory.readByte(pointers[c]+i,value) && value==expected[c][i]);}
 const auto stores=live.stores;const auto scalar=pose->state;pose->word(0x18,0x12345678);pose->word(0x1c,0x87654321);pose->word(0x3c,77);auto changed=input;changed.normals+=4;assert(!advanceAnimationMainGeometry(changed,time->services(),services,cursor));assert(advanceAnimationMainGeometry(input,time->services(),services,cursor) && !cursor.active && live.stores==stores+1 && pose->word(0x40)==expectedFraction && pose->word(0x3c)==77 && pose->word(0x18)==0x12345678);
 for(unsigned off:{0x20u,0x24u,0x84u,0x28u,0x2cu,0x34u,0x38u,0x80u,0x30u})for(unsigned b=0;b<4;++b)assert(pose->state[off+b]==scalar[off+b]);
 current=false;AnimationMainInterpolationCursor wait;AnimationMainReplayResult result;result.counts[0]=1;assert(!advanceAnimationMainInterpolation(input,result,binding->services(),wait) && wait.phase==3);
}

}
int main(){valuesAndHolds();exactReadsAndAliases();nodeGatesAndAdjacent();freshConstantsAndIdentity();actualGeometry();}

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
void expectedValues(Fixture& f){for(unsigned c=0;c<5;++c)for(unsigned i=0;i<f.entries;++i){const auto a=f.cache(c)+DW[c]*i,b=f.base(c)+DW[c]*i;if(c<2){assert(f.word(a)==bits(real(f.word(b))+.0625f));assert(f.word(a+4)==bits(real(f.word(b+4))-.125f));assert(f.word(a+8)==bits(real(f.word(b+8))+.1875f));}else if(c==3)assert(f.word(a)==bits(real(f.word(b))+.0625f));else{assert(f.memory[a]==1);if(c==4){assert(f.word(a+4)==bits(real(f.word(b+4))+.25f));assert(f.word(a+8)==bits(real(f.word(b+8))+.5f));}}}}
void emptyAndSame(){
 Fixture f(0);AnimationMainReplayCursor c;AnimationMainReplayResult out;f.finish(c,out);std::vector<Op> expected{{'w',R+0x28,0},{'w',R+0x3c,0},{'w',R+0x2c,0},{'w',R+0x30,0}};for(auto off:CO)expected.push_back({'w',D+off,0});expected.push_back({'w',T+0x28,0});expected.push_back({'w',R+0x3c,0});expected.push_back({'W',R+0x3c,0});expected.push_back({'w',0x8041fbac,0});for(unsigned ch=0;ch<5;++ch){expected.push_back({'w',K+KO[ch],0});expected.push_back({'w',K+KO[ch]+4,0});expected.push_back({'w',T+SO[ch],0});expected.push_back({'w',R+PO[ch],0});}assert(f.trace==expected && out.current==0 && out.next==1 && out.fraction==.5f && f.word(R+0x40)==bits(.75f));
 for(bool nan:{false,true}){Fixture same(0);same.word(R+0x3c,0);if(nan)same.word(R+0x40,0x7fc00000);AnimationMainReplayCursor cursor;AnimationMainReplayResult result;same.finish(cursor,result);assert(same.trace.size()==(nan?10u:12u));assert(nan?std::isnan(real(same.word(R+0x40))):same.word(R+0x40)==0);}
}
void allChannelsAndHolds(){
 for(unsigned n:{0u,1u,3u,4u,5u,7u,8u,9u})for(bool forward:{false,true}){Fixture f(n);if(forward){f.baseCache();f.word(R+0x3c,0);f.word(R+0x28,1);}AnimationMainReplayCursor c;AnimationMainReplayResult out;f.finish(c,out);expectedValues(f);assert(f.word(R+0x3c)==0 && f.word(R+0x40)==bits(.75f) && out.current==(forward?1u:0u));
  if(n!=5)continue;
  const auto count=f.calls;
  for(unsigned hold=1;hold<=count;++hold){Fixture test(n);if(forward){test.baseCache();test.word(R+0x3c,0);test.word(R+0x28,1);}test.hold=hold;AnimationMainReplayCursor cursor;AnimationMainReplayResult result{99,99,99,{}};for(unsigned visit=0;!test.held && visit<1000;++visit)assert(!test.run(cursor,result));assert(test.held && result.current==99);auto input=test.input;input.normals=1;assert(!advanceAnimationMainReplay(input,test.services(),cursor,result));auto other=test.services();other.user=&f;assert(!advanceAnimationMainReplay(test.input,other,cursor,result));test.finish(cursor,result);assert(test.memory==f.memory && test.trace==f.trace && result.current==out.current && result.counts==out.counts);}
 }
}
void loadOrderAndAliases(){
 Fixture counts(1);for(auto off:CO)counts.word(D+off,0);AnimationMainReplayCursor zeroCounts;AnimationMainReplayResult zeroResult;counts.finish(zeroCounts,zeroResult);assert(counts.word(counts.cache(0))==bits(.0625f) && counts.memory[counts.cache(2)]==7 && counts.word(counts.cache(3))==bits(.0625f) && counts.word(counts.cache(4)+8)==bits(.5f));
 Fixture f(5);AnimationMainReplayCursor c;AnimationMainReplayResult out;f.finish(c,out);
 // Vector loads source signed-byte triple before ANY destination float.
 size_t first=0;while(first<f.trace.size() && !(f.trace[first].kind=='q' && f.trace[first].address==f.stream(0)+1))++first;assert(first+3<f.trace.size() && f.trace[first+1].kind=='q' && f.trace[first+2].kind=='q' && f.trace[first+3].kind=='w' && f.trace[first+3].address==f.cache(0));
 // Texture bulk0..2 read dest8 first; fourth and remainder read source8 first.
 for(unsigned i=0;i<5;++i){const uint32_t source=f.stream(4)+12*i+8,dest=f.cache(4)+24*i+8;size_t a=0,b=0;for(size_t j=0;j<f.trace.size();++j)if(f.trace[j].kind=='w'){if(f.trace[j].address==source)a=j;if(f.trace[j].address==dest)b=j;}assert(i<3?b<a:a<b);}
 Fixture vector(1);vector.after=[](Fixture& x,char kind,uint32_t a){if(kind=='W' && a==x.cache(0) && x.word(a)==bits(1.0625f))x.memory[x.stream(0)+2]=100;};AnimationMainReplayCursor v;vector.finish(v,out);assert(vector.word(vector.cache(0)+4)==bits(1.875f)); // captured -2 survives X-store writer.
 Fixture texture(1);texture.after=[](Fixture& x,char kind,uint32_t a){if(kind=='W' && a==x.cache(4)+4 && x.word(a)==bits(2.25f))x.word(x.stream(4)+8,bits(8));};AnimationMainReplayCursor tc;texture.finish(tc,out);assert(texture.word(texture.cache(4)+8)==bits(11)); // source8 is read after dest4 write.
 Fixture capture(1);capture.after=[](Fixture& x,char kind,uint32_t a){if(kind=='W' && a==x.cache(0)){x.word(T+0x28,0);x.word(R+0x3c,77);x.word(D+CO[4],0);}};AnimationMainReplayCursor cap;capture.finish(cap,out);assert(out.counts[4]==1 && out.current==0 && capture.word(R+0x3c)==77 && capture.word(capture.cache(4)+8)==bits(3.5f));
}
void copyAndWrapDomains(){
 AnimationMainReplayResult out;
 // Original byte-copy is backward when unsigned source<destination.
 Fixture b(0);b.word(R+0x28,0xffffffff);b.word(R+0x3c,0);b.word(D+CO[2],4);const uint32_t a=0x97000000;b.word(D+BO[2],a);b.word(R+PO[2],a+1);for(unsigned i=0;i<5;++i)b.memory[a+i]=uint8_t(i+1);AnimationMainReplayCursor bc;b.finish(bc,out);for(unsigned i=0;i<5;++i)assert(b.memory[a+i]==(i?i:1));
 Fixture forward(0);forward.word(R+0x28,0xffffffff);forward.word(R+0x3c,0);forward.word(D+CO[2],4);forward.word(D+BO[2],a+1);forward.word(R+PO[2],a);for(unsigned i=0;i<5;++i)forward.memory[a+i]=uint8_t(i+1);AnimationMainReplayCursor fc;forward.finish(fc,out);for(unsigned i=0;i<5;++i)assert(forward.memory[a+i]==(i<4?i+2:5));
 // Original word-copy is always forward, including overlapping source<dest.
 Fixture w(0);w.word(R+0x28,0xffffffff);w.word(R+0x3c,0);w.word(D+CO[0],1);w.word(D+BO[0],a);w.word(R+PO[0],a+4);for(unsigned i=0;i<4;++i)w.word(a+4*i,i+1);AnimationMainReplayCursor wc;w.finish(wc,out);assert(w.word(a+4)==1 && w.word(a+8)==1 && w.word(a+12)==1);
 Fixture zero(0);zero.word(R+0x28,0xffffffff);zero.word(R+0x3c,0);zero.word(D+CO[3],0x40000000);AnimationMainReplayCursor z;assert(!zero.run(z,out) && z.phase==9 && z.copy.remaining==0xffffffff && zero.word(zero.cache(3))==zero.word(zero.base(3)));
 // Wrapped texture range derives zero entries, then original remainder CTR0
 // executes 2^32 entries instead of certifying empty success.
 Fixture malformed(0);malformed.word(R+0x3c,0);malformed.word(R+0x28,1);malformed.word(K+44+KO[4]+4,0x15555555);malformed.word(T+SO[4],0);for(unsigned i=0;i<12;++i)malformed.memory[i]=0;AnimationMainReplayCursor m;for(unsigned i=0;i<10 && m.phase!=40;++i)assert(!malformed.run(m,out));assert(m.phase==40 && m.remaining==0xffffffffull && m.entryIndex==1);
}
void actualCache(){
 assert(loadFieldPlayerPoseAssets("romfs"));EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};auto native=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform);assert(native);auto pool=native->pool();auto h=pool->enter(fieldPlayerPoseResources()[0],2,{});assert(h && pool->selectName(*h,"M_W_1",true,{}));for(unsigned i=0;i<17;++i){assert(native->tick(false));}
 auto& memory=native->process().vm().memory;auto read=[&](uint32_t a){uint32_t v;assert(memory.readWord(a,v));return v;};auto* pose=pool->pose(*h);BattlePoseClock clock;assert(native->clock(clock) && pool->evaluate(*h,clock));const auto* aliases=native->process().animationMemory();const auto work=read(0x80415dc0),model=read(read(read(read(work)+16*pose->word(0x10)+8)+0xa0)),timeline=read(read(model+0x1ac)+64*pose->word(0x14)+0x3c);AnimationMainEvaluation input{work,read(work+0x10),aliases->poseAddress(unsigned(*h)),model,timeline,pose->word(0x4c),pose->word(0x54)};
 std::array<std::vector<uint8_t>,5> expected;for(unsigned c=0;c<5;++c){expected[c].resize(read(model+CO[c])*DW[c]);for(unsigned i=0;i<expected[c].size();++i)assert(memory.readByte(pose->word(PO[c])+i,expected[c][i]));}
 bool current=true;std::optional<uint32_t> gqr=0;auto binding=AnimationMainReplayBinding::bind(pool,memory,*aliases,[&](){return current;},[&](){return gqr;});auto time=AnimationMainTimeBinding::bind(pool,memory,*aliases,[&](){return current;});assert(binding && time);
 pose->word(0x3c,0xffffffff);pose->scalar(0x40,.875f);AnimationMainCacheCursor cursor;AnimationMainReplayResult result{99,99,99,{}};bool quantizedHold=false;
 for(unsigned i=0;i<10000;++i){assert(!advanceAnimationMainCache(input,time->services(),binding->services(),cursor,result));if(cursor.phase==1 && (cursor.replay.phase==41 || cursor.replay.phase==50)){quantizedHold=true;break;}}assert(quantizedHold && result.current==99);const auto scalar=pose->state;const auto epoch=pose->timestamp();pose->word(0x18,0x12345678);pose->word(0x1c,0x87654321);gqr={};assert(!advanceAnimationMainCache(input,time->services(),binding->services(),cursor,result));auto changed=input;changed.model+=4;assert(!advanceAnimationMainCache(changed,time->services(),binding->services(),cursor,result));gqr=0x00060006;
 bool done=false;for(unsigned i=0;i<10000 && !done;++i)done=advanceAnimationMainCache(input,time->services(),binding->services(),cursor,result);assert(done && !cursor.active && pose->word(0x3c)==0 && pose->scalar(0x40)==.875f);
 for(unsigned c=0;c<5;++c)for(unsigned i=0;i<expected[c].size();++i){uint8_t value;assert(memory.readByte(pose->word(PO[c])+i,value) && value==expected[c][i]);}
 for(unsigned off:{0x20u,0x24u,0x84u,0x28u,0x2cu,0x34u,0x38u,0x80u,0x30u})for(unsigned b=0;b<4;++b)assert(pose->state[off+b]==scalar[off+b]);
 pose->word(0x18,uint32_t(epoch>>32));pose->word(0x1c,uint32_t(epoch));
 current=false;AnimationMainReplayCursor wait;assert(!advanceAnimationMainReplay(input,binding->services(),wait,result) && wait.phase==0);assert(!AnimationMainReplayBinding::bind(pool,memory,*aliases,[](){return true;},{}));
}
}
int main(){emptyAndSame();allChannelsAndHolds();loadOrderAndAliases();copyAndWrapDomains();actualCache();}

#include "animation_source_render.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <functional>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Root=0x80415dc0,W=0x1000,D=0x2000,P=0x3000,H=0x4000,B=0x5000,U=0x6000,R=0x7000,T=0x8000,S=0x9000,X=0xa000;
struct Event {char kind;uint32_t key,value;std::vector<uint32_t> args;bool operator==(const Event& b)const{return kind==b.kind && key==b.key && value==b.value && args==b.args;}};
struct Fixture {
 std::map<uint32_t,uint32_t> words,bytes,halves;std::vector<Event> trace;std::vector<AnimationRenderCall> calls;std::vector<uint16_t> fifo;
 std::function<void(Fixture&,char,uint32_t)> after;uint32_t camera=0xb000,byIndex=0xb000;unsigned operations=0,hold=~0u;bool held=false,singular=false;
 explicit Fixture(int slots=1){
  words={{0x8041e8a0,0xc000},{Root,W},{W+0x20,D},{W+0x18,P},{W,0xd000},{W+8,0xe000},{W+0xe0,2},{W+0xe8,0},{D+0x14c,H},{D+0x19c,B},{H+0x98,0},{H+0xa0,0},{H+0x9c,1},{H+0x40,1},{H+0x48,2},{H+0x50,3},{H+0xa4,3},{0x80415dd4,0},{D+0x18c,U},{P+0xe4,1},{P+0xe8,0},{P+4,0},{P,0},{P+0x4c,0xf000},{P+0x54,0x10000},{B,uint32_t(slots)},{B+8,0},{B+0x38,0},{B+0x3c,1},{B+0x40,1},{B+0x44,2},{B+0x48,3},{D+0x150,R},{D+0x164,0x11000},{R,2},{R+4,0xabcd0002},{D+0x158,0x12000},{D+0x160,0x13000},{D+0x168,0x14000},{D+0x194,T},{D+0x198,X},{D+0x134,8},{P+0x6c,S},{W+0xcc,0},{W+0x24,4},{W+0x30,0},{W+0xd0,0},{W+0x28,5},{W+0x34,0},{W+0xd8,0},{W+0xdc,0},{W+0x38,0},{W+0x2c,6},{W+0xd4,0x16000}};
  for(unsigned k=0;k<8;++k){words[H+0x58+8*k]=10+k;words[B+0x4c+4*k]=4+k;words[D+0x16c+4*k]=0x15000+0x100*k;bytes[B+0x30+k]=k;words[B+0x10+4*k]=k;words[T+8*k]=k;bytes[S+24*k]=0;words[X+64*k+8]=0;}
  for(unsigned j=0;j<8;++j)for(unsigned v=0;v<8;++v){halves[0x12000+4*(2+1+v)+2]=100+v;halves[0x13000+4*(2+2+v)+2]=200+v;halves[0x14000+4*(2+3+v)+2]=300+v;halves[0x15000+0x100*j+4*(2+4+j+v)+2]=400+10*j+v;words[0x15000+0x100*j+4*(2+4+j+v)]=0xface0000u+400+10*j+v;}
 }
 bool gate(){if(operations==hold && !held){held=true;return false;}++operations;return true;}
 void record(char k,uint32_t a,uint32_t v,std::vector<uint32_t> args={}){trace.push_back({k,a,v,std::move(args)});if(after)after(*this,k,a);}
 static std::optional<uint32_t> word(void* u,uint32_t a){auto& f=*static_cast<Fixture*>(u);auto p=f.words.find(a);if(p==f.words.end() || !f.gate())return {};auto v=p->second;f.record('W',a,v);return v;}
 static std::optional<uint8_t> byte(void* u,uint32_t a){auto& f=*static_cast<Fixture*>(u);auto p=f.bytes.find(a);if(p==f.bytes.end() || !f.gate())return {};auto v=uint8_t(p->second);f.record('B',a,v);return v;}
 static std::optional<uint16_t> half(void* u,uint32_t a){auto& f=*static_cast<Fixture*>(u);auto p=f.halves.find(a);if(p==f.halves.end() || !f.gate())return {};auto v=uint16_t(p->second);f.record('H',a,v);return v;}
 static bool call(void* u,const AnimationRenderCall& q,AnimationRenderLocals& l,uint32_t& result){auto& f=*static_cast<Fixture*>(u);if(!f.gate())return false;
  std::vector<uint32_t> args;for(unsigned i=0;i<q.count;++i){args.push_back(uint32_t(q.arguments[i].kind));args.push_back(q.arguments[i].value);}f.calls.push_back(q);
  if(q.function==0x8000f2e4)result=f.camera;
  if(q.function==0x8000f2fc)result=f.byIndex;
  if(q.function==0x8002f33c)result=q.arguments[0].value+0x40;
  // Explicit child receipts isolate parent chronology; these are not SDK/GPU implementations.
  if(q.function==0x8029dd64)l.matrices[0]=BattleActMatrix{};
  if(q.function==0x8029df28 && !f.singular)l.matrices[1]=BattleActMatrix{};
  if(q.function==0x8029e6a4){assert(q.arguments[1].kind==AnimationRenderKind::Vector && q.arguments[1].value==q.arguments[2].value);l.vectors[0]={0x3f800000,0x40000000,0x40400000};}
  if(q.function==0x800208c0)assert((l.vectors[1]==std::array<uint32_t,3>{0x3f800000,0x40000000,0x40400000}));
  f.record('C',q.function,result,args);return true;
 }
 static bool write(void* u,uint32_t a,uint16_t v){auto& f=*static_cast<Fixture*>(u);assert(a==0xcc008000);if(!f.gate())return false;f.fifo.push_back(v);f.record('F',a,v);return true;}
 AnimationSourceRenderServices services(){return {this,word,byte,half,{this,call,write}};}
 bool finish(AnimationSourceRenderCursor& c,unsigned visits=1){for(unsigned i=0;i<4000;++i)if(resumeAnimationSourceRender(0,services(),c,visits))return true;return false;}
 std::vector<uint32_t> functions()const{std::vector<uint32_t> out;for(auto& q:calls)out.push_back(q.function);return out;}
 unsigned count(char k,uint32_t a)const{return std::count_if(trace.begin(),trace.end(),[&](const auto& e){return e.kind==k && e.key==a;});}
};
void defaults(Fixture& f){f.words[P+0xe4]=0;f.words[P+0xec]=7;f.words[P+0x10]=2;f.words[0xd000+32+12]=3;f.words[P+0xf4]=0x12345678;f.words[P+0xf0]=0x90abcdef;f.words[P+0xe8]=0x42;for(unsigned i=0;i<3;++i)f.words[0x802c103c+4*i]=0;}
void override(Fixture& f,unsigned kind,bool object=true,bool matrix=true){f.words[P+4]=1;f.words[X+8]=kind;f.words[W+(kind==1?0x30:0x34)]=object?0x17000:0;f.words[W+(kind==1?0xcc:0xd0)]=matrix?0x18000:0;}
using Setup=std::function<void(Fixture&)>;
void everyPrimitive(){std::vector<Setup> setups={[](auto&){},[](auto& f){defaults(f);},[](auto& f){f.words[H+0xa0]=9;f.bytes[P+0xf3]=255;f.bytes[P+0xf7]=255;},[](auto& f){f.words[H+0x9c]=0;},[](auto& f){override(f,1);},[](auto& f){override(f,2);},[](auto& f){override(f,1,false);},[](auto& f){override(f,2,true,false);},[](auto& f){override(f,1);f.words[W+0x38]=0x19000;},[](auto& f){override(f,2);f.words[W+0x38]=0x19000;f.words[W+0xe0]=3;f.words[H+0xa0]=3;},[](auto& f){f.words[P]=0x10;f.words[W+0x30]=0x17000;f.words[W+0xcc]=0x18000;f.words[X+8]=1;},[](auto& f){override(f,1);f.words[T]=0xffffffff;f.bytes[S]=255;},[](auto& f){f.words[W+0xe8]=1;f.words[W+0xfc]=0x22000;f.words[W+0xf8]=0x23000;},[](auto& f){f.words[B+0x3c]=0xffffffff;},[](auto& f){f.words[R+4]=0x12340000;}};
 for(int slots:{-1,0,1,2,3,4,5})for(unsigned scenario=0;scenario<setups.size();++scenario){Fixture baseline(slots);setups[scenario](baseline);AnimationSourceRenderCursor c;if(!baseline.finish(c)){std::fprintf(stderr,"slots=%d scenario=%u phase=%u last=%x\n",slots,scenario,unsigned(c.phase),unsigned(baseline.trace.back().key));assert(false);}auto expected=baseline.trace;unsigned total=baseline.operations;
  for(unsigned h=0;h<total;++h){Fixture f(slots);setups[scenario](f);f.hold=h;AnimationSourceRenderCursor held;assert(f.finish(held) && f.held);assert(f.trace==expected && f.fifo==baseline.fifo);}
 }
}
void originalGoldens(){
 Fixture empty;AnimationSourceRenderCursor zero;assert(resumeAnimationSourceRender(0xffffffff,{},zero,1));assert(!zero.active);
 Fixture reject;reject.words[H+0xa0]=9;reject.bytes[P+0xf3]=reject.bytes[P+0xf7]=255;AnimationSourceRenderCursor a;assert(reject.finish(a));assert((reject.functions()==std::vector<uint32_t>{0x8000f2e4,0x8029dd64,0x802ba2fc,0x8029df28,0x802ba34c,0x802ba39c}));assert(reject.count('W',H+0xa4)==0);
 Fixture f;defaults(f);AnimationSourceRenderCursor c;assert(f.finish(c));assert((f.functions()==std::vector<uint32_t>{0x8000f2e4,0x8029dd64,0x802ba2fc,0x8029df28,0x802ba34c,0x802ba39c,0x802b5c8c,0x8029e6a4,0x800208c0,0x8003dcfc,0x80021860,0x8000f2fc,0x8000f2e4,0x800217c8,0x80021b80,0x802b4520,0x802b3c08,0x802b4558,0x802b4db0,0x802b3c08,0x802b4558,0x802b4db0,0x802b3c08,0x802b4558,0x802b4db0,0x802b3c08,0x802b4558,0x802b4db0,0x802b5a6c}));
 assert((f.fifo==std::vector<uint16_t>{100,200,300,400,101,201,301,401}));assert(f.calls[1].arguments[0].value==0xb11c && f.calls[1].arguments[1].value==0xc000);
 auto tex=f.calls[9];assert(tex.count==7);std::array<uint32_t,7> values{1,B+0x10,T,S,8,X,0xe024};for(unsigned i=0;i<7;++i)assert(tex.arguments[i].value==values[i]);assert(c.locals.colors[0]==0x90abcdef && c.locals.colors[1]==0x12345678);
 std::vector<uint32_t> fields;for(auto& e:f.trace)if(e.kind=='W' && e.key>=H+0x58 && e.key<=H+0x90)fields.push_back(e.key-H);assert((fields==std::vector<uint32_t>{0x58,0x60,0x68,0x70,0x80,0x78,0x88,0x90}));
 Fixture noFog;defaults(noFog);noFog.after=[](auto& x,char k,uint32_t fn){if(k=='C' && fn==0x8000f2fc)x.camera=0xba00;};AnimationSourceRenderCursor nf;assert(noFog.finish(nf));assert(noFog.count('C',0x800217c8)==0 && noFog.count('C',0x8000f2e4)==2);
 Fixture dual;unsigned n=0;dual.words[W+0xe0]=1;dual.after=[&](auto& x,char k,uint32_t addr){if(k=='W' && addr==W+0xe0 && ++n==1)x.words[W+0xe0]=0;};AnimationSourceRenderCursor d;assert(dual.finish(d) && dual.count('W',W+0xe0)==2 && dual.count('B',P+0xf3)==0);
 Fixture alpha;alpha.words[H+0xa0]=8;alpha.bytes[P+0xf3]=1;AnimationSourceRenderCursor al;assert(alpha.finish(al) && alpha.count('B',P+0xf7)==0);
 Fixture singular;singular.singular=true;AnimationSourceRenderCursor ns;assert(singular.finish(ns) && !ns.locals.matrices[1]);
}
void overridesAndMutation(){
 Fixture nullMatch;override(nullMatch,1,false);AnimationSourceRenderCursor n;assert(nullMatch.finish(n));assert(n.found1 && !n.found2 && nullMatch.count('W',W+0xd0)==0 && nullMatch.count('C',0x8002f33c)==0);
 Fixture warning;override(warning,1);warning.words[T]=0xffffff00;warning.bytes[S]=250;AnimationSourceRenderCursor wc;assert(warning.finish(wc));auto it=std::find_if(warning.calls.begin(),warning.calls.end(),[](auto& q){return q.function==0x8026a2a8;});assert(it!=warning.calls.end() && it->arguments[1].value==250 && it->arguments[0].value==0x802c1078);
 Fixture extra;override(extra,1);extra.words[W+0x38]=0x19000;extra.after=[](auto& x,char k,uint32_t fn){if(k=='C' && fn==0x802b89a4){x.words[W+0x30]=0x1a000;x.words[W+0xcc]=0;} };AnimationSourceRenderCursor ec;assert(extra.finish(ec));assert(extra.count('C',0x802b93d8)==1 && extra.count('C',0x802b932c)==1 && extra.count('C',0x802b940c)==1 && extra.count('C',0x802b8d60)==1);
 bool zeroMatrix=false,liveObject=false;for(auto& q:extra.calls){if(q.function==0x802ba3d0 && q.arguments[0].value==0)zeroMatrix=true;if(q.function==0x802b7880 && q.arguments[0].value==0x1a000)liveObject=true;}assert(zeroMatrix && liveObject);
 Fixture frame;defaults(frame);frame.after=[](auto& x,char k,uint32_t fn){if(k=='C' && fn==0x800208c0)x.words[B]=0;};AnimationSourceRenderCursor fc;assert(frame.finish(fc));auto tev=std::find_if(frame.calls.begin(),frame.calls.end(),[](auto& q){return q.function==0x80021b80;});assert(tev!=frame.calls.end() && tev->arguments[0].value==1);auto tex=std::find_if(frame.calls.begin(),frame.calls.end(),[](auto& q){return q.function==0x8003dcfc;});assert(tex->arguments[0].value==0);
}
void streamChronology(){
 Fixture multi(5);multi.words[R+4]=1;multi.after=[](auto& x,char k,uint32_t fn){if(k=='C' && fn==0x802b5a6c){x.words[Root]=W+0x100;x.words[W+0x120]=D;}if(k=='F'){x.words[Root]=0xdead0000;}};AnimationSourceRenderCursor c;assert(multi.finish(c));assert((multi.fifo==std::vector<uint16_t>{100,200,300,440,430,420,410,400}));
 std::vector<char> sequence;bool began=false;for(auto& e:multi.trace){if(e.kind=='C' && e.key==0x802b5a6c)began=true;if(began && ((e.kind=='W' && e.key==W+0x120) || (e.kind=='B' && e.key>=B+0x30 && e.key<B+0x35)))sequence.push_back(e.kind);}
 assert((sequence==std::vector<char>{'W','B','B','W','B','W','B','W','W','B'}));assert(multi.count('W',Root)==8); // entry,UV,draw,primary,rebind,arrays,run,postBegin
 Fixture zero(0);zero.words[R+4]=0xbeef0000;AnimationSourceRenderCursor z;assert(zero.finish(z) && zero.fifo.empty() && zero.count('C',0x802b5a6c)==1);assert(zero.trace.back().kind=='W' && zero.trace.back().key==Root);
 Fixture captured(1);captured.after=[](auto& x,char k,uint32_t fn){if(k=='C' && fn==0x802b5a6c){x.words[Root]=0;for(unsigned a:{0x158,0x160,0x168,0x16c})x.words[D+a]=0;}};AnimationSourceRenderCursor sc;assert(captured.finish(sc));assert((captured.fifo==std::vector<uint16_t>{100,200,300,400,101,201,301,401}));
 Fixture multiple;multiple.words[H+0x9c]=2;multiple.words[B+0x3c]=2;multiple.words[R+8]=3;multiple.words[R+12]=1;
 auto firstWords=multiple.words;auto firstBytes=multiple.bytes;for(auto [a,v]:firstWords)if(a>=B && a<B+108)multiple.words[a+108]=v;for(auto [a,v]:firstBytes)if(a>=B && a<B+108)multiple.bytes[a+108]=v;
 multiple.after=[](auto& x,char k,uint32_t fn){if(k=='C' && fn==0x802b5a6c)x.words[H+0x9c]=0;};AnimationSourceRenderCursor md;assert(multiple.finish(md) && multiple.fifo.size()==24 && multiple.count('C',0x802b5a6c)==4 && multiple.count('W',H+0x9c)==1 && multiple.count('C',0x802b5c8c)==1);
 Fixture heldHalf;AnimationSourceRenderCursor hh;for(unsigned i=0;i<1000 && hh.phase!=622;++i)assert(!resumeAnimationSourceRender(0,heldHalf.services(),hh,1));assert(hh.phase==622 && heldHalf.fifo.empty());heldHalf.halves[hh.streams[0]]=999;assert(heldHalf.finish(hh) && heldHalf.fifo[0]==100);
 Fixture repair;repair.hold=0;AnimationSourceRenderCursor r;auto services=repair.services();assert(!resumeAnimationSourceRender(0,services,r,4096));auto other=services;other.children.fifo=nullptr;auto trace=repair.trace;assert(!resumeAnimationSourceRender(1,services,r,4096) && !resumeAnimationSourceRender(0,other,r,4096) && repair.trace==trace);assert(repair.finish(r));
 Fixture bad;bad.bytes[B+0x30]=255;AnimationSourceRenderCursor b;assert(!bad.finish(b));assert(b.phase==526 && bad.count('C',0x802b4558)==4 && bad.fifo.empty());bad.bytes[B+0x30]=0;assert(!bad.finish(b)); // captured selector cannot be repaired by rewriting its source
}
}
int main(){everyPrimitive();originalGoldens();overridesAndMutation();streamChronology();}

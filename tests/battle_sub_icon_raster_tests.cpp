#include <memory>
#include "battle_sub_icon_raster.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
using namespace ttyd;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Event{uint32_t kind;std::vector<uint32_t>a;bool operator==(const Event&o)const{return kind==o.kind&&a==o.a;}};
struct Fixture{
 static constexpr uint32_t B=0x10000,W=0x20000,C=0x30000,Cam=0x40000,Owner=0x50000,Palette=0x60000,Gp=0x70000,Extras=0x80000;
 EventScriptAssets originals;std::map<uint32_t,uint32_t>words;std::map<uint32_t,uint16_t>halves;std::map<uint32_t,uint8_t>bytes;
 std::shared_ptr<const void> lifetime=std::make_shared<int>(1);BattleSubIconRasterRequest input;BattleSubIconRasterCursor cursor;
 std::vector<Event>trace,ops;std::vector<BattleSubIconRasterCall>calls;std::optional<Event>pending;const BattleSubIconRasterCall*borrowed=nullptr;const BattleAudienceMatrixRequest*mathBorrowed=nullptr;
 std::function<void(Fixture&,const Event&)>onOp;int hold=-1;unsigned failures=0,widthCalls=0;std::vector<uint32_t> widths{80,80};bool alive=true,probe=false;
 Fixture(){assert(originals.load("romfs/trio-attack-events.pmevt"));input.work=W;input.row=1;input.label=1;input.icon=23;input.enabled=0x90000;input.matrix={1,0,0,13,0,1,0,7,0,0,1,0};words={{0x8041eb90,B},{W,C},{W+0x98,bits(0.25f)},{C,1},{B+0x163f8,Owner},{Owner+0xa0,Owner+0x100},{Owner+0x100,Palette},{0x80415fd0,Gp},{Gp+0x16c,0},{0x80429598,0x102030ff}};bytes={{0x80416d20,0x12},{0x80416d21,0x34},{0x80416d22,0x56}};for(unsigned i=0;i<12;++i)words[Cam+0x11c+4*i]=bits(i==0||i==5||i==10?1.f:0.f);for(unsigned i=0;i<7;++i){halves[Extras+24*i+0x14]=123;halves[Extras+24*i+0x16]=45;}}
 uint32_t word(uint32_t a){uint32_t v=0;const auto it=words.find(a);if(it!=words.end())v=it->second;else assert(originals.readWord(a,v));trace.push_back({1,{a,v}});return v;}
 uint16_t half(uint32_t a){const auto v=halves.at(a);trace.push_back({2,{a,v}});return v;}uint8_t byte(uint32_t a){const auto v=bytes.at(a);trace.push_back({3,{a,v}});return v;}
 bool admit(uint32_t fn,std::vector<uint32_t>a){const Event op{fn,std::move(a)};if(!pending){pending=op;trace.push_back(op);ops.push_back(op);if(onOp)onOp(*this,op);}else assert(*pending==op);if(probe){assert(!cursor.reset());assert(!run());}if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 static void append(std::vector<uint32_t>&v,const BattleActMatrix&m){for(float f:m)v.push_back(bits(f));}
 BattleSubIconRasterServices services(){BattleSubIconRasterServices s;s.user=this;s.lifetime=lifetime;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.actors.user=this;s.actors.memory.user=this;s.actors.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};s.actors.memory.half=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->half(a);};s.actors.byte=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};
 s.truncate=[](void*p,float f)->std::optional<int32_t>{auto&x=*static_cast<Fixture*>(p);if(!x.admit(0xf0000001,{bits(f)}))return {};if(!std::isfinite(f)||double(f)<-2147483648.0||double(f)>=2147483648.0)return {};return int32_t(f);};
 s.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&x=*static_cast<Fixture*>(p);std::vector<uint32_t>a{q.axis,bits(q.radians),bits(q.xyz[0]),bits(q.xyz[1]),bits(q.xyz[2])};append(a,q.left);append(a,q.right);if(x.mathBorrowed)assert(x.mathBorrowed==&q);else x.mathBorrowed=&q;if(!x.admit(q.function,a))return false;x.mathBorrowed=nullptr;BattleAudienceAuxServices m;bindAudienceAuxMathAdapter(m);return m.matrix(nullptr,q);};
 s.request=[](void*p,const BattleSubIconRasterCall&q)->std::optional<uint32_t>{auto&x=*static_cast<Fixture*>(p);std::vector<uint32_t>a{q.callsite,uint32_t(q.integers.size())};a.insert(a.end(),q.integers.begin(),q.integers.end());a.push_back(uint32_t(q.reals.size()));for(float f:q.reals)a.push_back(bits(f));a.push_back(bool(q.matrix));if(q.matrix)append(a,*q.matrix);a.push_back(bool(q.privateText));if(q.privateText){a.push_back((*q.privateText)[0]);a.push_back((*q.privateText)[1]);}if(x.borrowed)assert(x.borrowed==&q);else x.borrowed=&q;if(!x.admit(q.function,a))return {};x.borrowed=nullptr;x.calls.push_back(q);if(q.function==0x8000f2fc)return Cam;if(q.function==0x80037a80)return Palette+q.integers[1];if(q.function==0x80075528){assert(x.widthCalls<x.widths.size());return x.widths[x.widthCalls++];}if(q.function==0x80081b8c)return q.integers[0]+0x1000;return uint32_t(0);};return s;}
 bool run(){return resumePackedBattleSubIconRaster(input,services(),cursor);}
};
using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup={},bool every=true){Fixture expected;if(setup)setup(expected);assert(expected.run());if(!every)return;for(unsigned gate=0;gate<expected.ops.size();++gate){Fixture f;if(setup)setup(f);f.hold=int(gate);f.probe=true;assert(!f.run());auto trace=f.trace;auto words=f.words;auto s=f.services();s.actors.audienceLifecycle.user=&expected;assert(!resumePackedBattleSubIconRaster(f.input,s,f.cursor));s=f.services();s.matrix=nullptr;assert(!resumePackedBattleSubIconRaster(f.input,s,f.cursor));s=f.services();s.lifetime=std::make_shared<int>(2);assert(!resumePackedBattleSubIconRaster(f.input,s,f.cursor));auto copy=f.input;assert(!resumePackedBattleSubIconRaster(copy,f.services(),f.cursor));f.input.matrix[0]=2;assert(!f.run());f.input=copy;assert(!f.run()&&f.trace==trace&&f.words==words);assert(f.run()&&!f.cursor.active());assert(f.trace==expected.trace&&f.words==expected.words);}}
int main(){
 retained();for(unsigned row:{0u,1u,6u})for(float scroll:{-1.25f,0.f,0.25f,1.f})for(uint32_t enabled:{0u,1u,2u})retained([=](auto&f){f.input.row=row;f.input.label=enabled;f.words[Fixture::W+0x98]=bits(scroll);},row==0&&enabled==1);
 for(uint32_t cost:{0u,1u,9u,10u,99u,100u,999u,1000u,0xffffffffu})retained([=](auto&f){f.input.cost=cost;f.input.bp=cost;});
 for(uint32_t language:{0u,1u,5u})retained([=](auto&f){f.input.flag=Fixture::Extras;f.words[Fixture::Gp+0x16c]=language;});
 for(uint32_t width:{149u,150u,151u,65535u,0x10096u})retained([=](auto&f){f.input.stackFlag=1;f.widths={width,200};});
 Fixture plain;assert(plain.run());std::vector<uint32_t> functions;for(const auto&e:plain.ops)if(e.kind!=0xf0000001)functions.push_back(e.kind);
 assert((functions==std::vector<uint32_t>{0x8000f2fc,0x80103a14,0x8029dd64,0x802ba2fc,0x80037a80,0x802b7880,0x801037c4,0x801037c4,0x80037a80,0x802b7880,0x801037c4,0x8029e230,0x8029e2b0,0x8029dd64,0x8029dd64,0x8001a4bc,0x80077b4c,0x800777e0,0x8029e230,0x8029e2b0,0x8029dd64,0x8029dd64,0x80076cdc}));
 std::vector<uint32_t>textures;for(const auto&q:plain.calls)if(q.function==0x80037a80){assert(q.integers[0]==Fixture::Palette);textures.push_back(q.integers[1]);}assert((textures==std::vector<uint32_t>{2,3}));
 Fixture digits;digits.input.cost=123;digits.input.bp=45;digits.input.flag=Fixture::Extras;assert(digits.run());std::vector<uint8_t>text;for(const auto&q:digits.calls)if(q.privateText){assert((*q.privateText)[1]==0);text.push_back((*q.privateText)[0]);}assert((text==std::vector<uint8_t>{'3','2','1','5','4','3','2','1','5','4'}));
 // Source does not clamp edge alpha before raster/color narrowing.
 Fixture edge;edge.input.row=0;edge.words[Fixture::C]=0;edge.words[Fixture::W+0x98]=bits(0.5f);assert(edge.run());for(const auto&q:edge.calls)if(q.function==0x8001a4bc)assert((q.integers[2]&255)==127);
 Fixture fallback;fallback.input.icon=0;fallback.input.label=0;assert(fallback.run());for(const auto&q:fallback.calls)if(q.function==0x8001a4bc)assert(q.integers[0]==48&&q.integers[1]==403);
 Fixture negative;negative.input.cost=0xffffffffu;assert(negative.run());for(const auto&q:negative.calls)if(q.privateText)assert((*q.privateText)[0]=='/'); // Signed source remainder, no substitute number formatting.
 Fixture fresh;fresh.input.stackFlag=1;fresh.widths={151,300};fresh.input.flag=Fixture::Extras;bool changed=false;fresh.onOp=[&](auto&f,const Event&e){if(e.kind==0x80076cdc&&!changed){changed=true;f.words[Fixture::W+0x98]=bits(1.f);f.halves[Fixture::Extras+24*2+0x14]=7;f.halves[Fixture::Extras+24*2+0x16]=8;}};assert(fresh.run()&&fresh.widthCalls==2);text.clear();for(const auto&q:fresh.calls)if(q.privateText)text.push_back((*q.privateText)[0]);assert((text==std::vector<uint8_t>{'7','8'}));
 Fixture invalidDuringMatrix;invalidDuringMatrix.hold=3;invalidDuringMatrix.onOp=[](auto&f,const Event&e){if(e.kind==0x8029dd64)f.alive=false;};assert(!invalidDuringMatrix.run()&&invalidDuringMatrix.cursor.stale());auto invalidTrace=invalidDuringMatrix.trace;invalidDuringMatrix.alive=true;assert(!invalidDuringMatrix.run()&&invalidDuringMatrix.trace==invalidTrace);
 Fixture noOwner;auto unowned=noOwner.services();unowned.lifetime.reset();assert(!resumePackedBattleSubIconRaster(noOwner.input,unowned,noOwner.cursor)&&!noOwner.cursor.active());
 Fixture missing;auto s=missing.services();s.request=nullptr;assert(!resumePackedBattleSubIconRaster(missing.input,s,missing.cursor)&&missing.cursor.pending()==0x8000f2fc);Fixture stale;stale.hold=0;assert(!stale.run());stale.alive=false;assert(!stale.run()&&stale.cursor.stale());auto trace=stale.trace;stale.alive=true;assert(!stale.run()&&stale.trace==trace);assert(stale.cursor.reset());
 std::cout<<"Whole SubIconSub source call census, alpha/cost/width/language branches, fresh operands and every-child holds passed\n";
}

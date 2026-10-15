#include <memory>
#include "battle_sub_icon_continuation.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct Event{uint32_t kind;std::vector<uint32_t> args;bool operator==(const Event&o)const{return kind==o.kind&&args==o.args;}};
struct Fixture{
 static constexpr uint32_t B=0x10000,W=0x20000,C=0x30000,U=0x40000;
 EventScriptAssets originals;std::map<uint32_t,uint32_t> memory;
 BattleWeaponDrawOutput input;BattleSubIconCursor cursor;bool alive=true,probe=false;int hold=-1;unsigned failures=0;
 std::vector<Event> trace,ops;std::optional<Event> pending;const BattleSubIconRasterRequest* borrowed=nullptr;
 std::vector<BattleSubIconRasterRequest> draws;std::function<void(Fixture&,const Event&)> onOp;
 Fixture(){assert(originals.load("romfs/trio-attack-events.pmevt"));memory={{0x8041eb90,B},{B+0x420,1},{B+0x24,U},{W,C},{W+0x98,bits(0.25f)},{C+8,3}};for(unsigned i=0;i<7;++i){memory[W+0xa4+4*i]=1;auto&v=input.weapons[i];v.matrix=BattleActMatrix{1,0,0,float(i),0,1,0,0,0,0,1,0};v.label=0x100+i;v.icon=uint16_t(40+i);v.enabled=i%2;v.cost=i+2;v.bp=i+3;}}
 uint32_t word(uint32_t a){uint32_t v=0;const auto it=memory.find(a);if(it!=memory.end())v=it->second;else assert(originals.readWord(a,v));trace.push_back({1,{a,v}});return v;}
 bool admit(uint32_t fn,std::vector<uint32_t> args){const Event op{fn,std::move(args)};if(!pending){pending=op;trace.push_back(op);ops.push_back(op);if(onOp)onOp(*this,op);}else assert(*pending==op);if(probe){assert(!cursor.reset());assert(!run());}if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 static void append(std::vector<uint32_t>&a,const BattleActMatrix&m){for(float f:m)a.push_back(bits(f));}
 BattleSubIconServices services(){BattleSubIconServices s;s.user=this;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.math.user=this;s.math.memory.memory.user=this;s.math.memory.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
 s.truncate=[](void*p,float f)->std::optional<int32_t>{auto&x=*static_cast<Fixture*>(p);if(!x.admit(0xf0000001,{bits(f)}))return {};assert(std::isfinite(f)&&f>=-2147483648.0&&f<2147483648.0);return int32_t(f);};
 s.fma=[](void*p,float a,float b,float c)->std::optional<float>{auto&x=*static_cast<Fixture*>(p);if(!x.admit(0xf0000002,{bits(a),bits(b),bits(c)}))return {};return std::fma(a,b,c);};
 s.math.translation=[](void*p,const std::array<float,3>&v)->std::optional<BattleActMatrix>{auto&x=*static_cast<Fixture*>(p);if(!x.admit(0x8029e230,{bits(v[0]),bits(v[1]),bits(v[2])}))return {};BattleAudienceAuxServices m;bindAudienceAuxMathAdapter(m);BattleAudienceMatrixRequest q;q.function=0x8029e230;q.xyz=v;assert(m.matrix(nullptr,q));return q.output;};
 s.math.concat=[](void*p,const BattleActMatrix&a,const BattleActMatrix&b)->std::optional<BattleActMatrix>{auto&x=*static_cast<Fixture*>(p);std::vector<uint32_t> args;append(args,a);append(args,b);if(!x.admit(0x8029dd64,args))return {};BattleAudienceAuxServices m;bindAudienceAuxMathAdapter(m);BattleAudienceMatrixRequest q;q.function=0x8029dd64;q.left=a;q.right=b;assert(m.matrix(nullptr,q));return q.output;};
 s.request=[](void*p,const BattleSubIconRasterRequest&q)->std::optional<uint32_t>{auto&x=*static_cast<Fixture*>(p);std::vector<uint32_t>a{q.callsite,q.work,q.row,q.label,q.icon,q.enabled,q.cost,q.bp,q.flag,q.stackFlag};append(a,q.matrix);if(x.borrowed)assert(x.borrowed==&q);else x.borrowed=&q;if(!x.admit(q.function,a))return {};x.borrowed=nullptr;x.draws.push_back(q);return uint32_t(0);};return s;}
 bool run(){return resumePackedBattleSubIcon(W,input,17,29,services(),cursor);}
};
// Independent straight-line source transcription; providers are immediate.
void reference(Fixture&f){auto s=f.services();auto word=[&](uint32_t a){return f.word(a);};auto constant=[&](int32_t n){return real(word(0x80427440u+uint32_t(n)));};auto bias=[&](){const auto hi=word(0x802ef780),lo=word(0x802ef784);const uint64_t raw=(uint64_t(hi)<<32)|lo;double d;std::memcpy(&d,&raw,8);return d;};auto convert=[](int32_t n,double d){const uint64_t raw=(uint64_t(0x43300000)<<32)|(uint32_t(n)^0x80000000u);double v;std::memcpy(&v,&raw,8);return float(v-d);};auto signedWord=[](uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);};
 const auto b=word(0x8041eb90),id=word(b+0x420);if(signedWord(id)>=0&&signedWord(id)<64)(void)word(b+0x20+4*id);
 const float initial=real(word(Fixture::W+0x98));const auto top=s.truncate(s.user,initial);assert(top);const double d=bias();const float spacing=constant(-20416);const auto cursor=word(Fixture::W);volatile float remainder=initial-convert(*top,d);volatile float fraction=spacing*remainder;
 for(uint32_t row=0;row<7;++row){const auto scroll=real(word(Fixture::W+0x98));const auto count=word(cursor+8);const auto truncated=s.truncate(s.user,scroll);assert(truncated);if(signedWord(uint32_t(*truncated)+row)>=signedWord(count))continue;if(!(word(Fixture::W+0xa4+4*row)&1))continue;const auto rowBias=bias();const auto step=constant(-20416),base=constant(-20284),x=constant(-20296),z=constant(-20500);const auto fused=s.fma(s.user,step,convert(int32_t(row),rowBias),-base);assert(fused);volatile float y=-*fused+fraction;const auto trans=s.math.translation(s.math.user,{x,y,z});assert(trans&&f.input.weapons[row].matrix);const auto matrix=s.math.concat(s.math.user,*trans,*f.input.weapons[row].matrix);assert(matrix);const auto&v=f.input.weapons[row];assert(v.label&&v.icon&&v.enabled&&v.cost&&v.bp);BattleSubIconRasterRequest q;q.work=Fixture::W;q.row=row;q.matrix=*matrix;q.label=*v.label;q.icon=*v.icon;q.enabled=*v.enabled;q.cost=*v.cost;q.bp=*v.bp;q.flag=17;q.stackFlag=29;assert(s.request(s.user,q));}
}
using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup){Fixture baseline;setup(baseline);assert(baseline.run());Fixture oracle;setup(oracle);reference(oracle);assert(oracle.trace==baseline.trace&&oracle.memory==baseline.memory);for(unsigned i=0;i<baseline.ops.size();++i){Fixture f;setup(f);f.hold=int(i);f.probe=true;assert(!f.run());auto trace=f.trace;auto values=f.memory;auto s=f.services();s.math.memory.audienceLifecycle.user=&baseline;assert(!resumePackedBattleSubIcon(Fixture::W,f.input,17,29,s,f.cursor));s=f.services();s.fma=nullptr;assert(!resumePackedBattleSubIcon(Fixture::W,f.input,17,29,s,f.cursor));auto copy=f.input;assert(!resumePackedBattleSubIcon(Fixture::W,copy,17,29,f.services(),f.cursor));f.input.weapons[6].bp=99;assert(!f.run());f.input=copy;assert(!resumePackedBattleSubIcon(Fixture::W,f.input,18,29,f.services(),f.cursor));assert(!f.run()&&f.trace==trace&&f.memory==values);assert(f.run()&&!f.cursor.active());assert(f.trace==baseline.trace&&f.memory==baseline.memory&&f.draws.size()==baseline.draws.size());}}
int main(){
 retained([](auto&){});
 for(uint32_t count:{0u,1u,3u,7u,8u,0xffffffffu})for(float scroll:{-1.25f,0.0f,0.25f,2.75f,8.0f})retained([=](auto&f){f.memory[Fixture::C+8]=count;f.memory[Fixture::W+0x98]=bits(scroll);});
 for(unsigned row=0;row<7;++row)retained([=](auto&f){for(unsigned i=0;i<7;++i){f.memory[Fixture::W+0xa4+4*i]=i==row?1:2;f.input.weapons[i]=BattleWeaponDrawSlot{};}auto&v=f.input.weapons[row];v.matrix=BattleActMatrix{};v.label=1;v.icon=2;v.enabled=0;v.cost=0;v.bp=0;});
 Fixture cold;for(unsigned i=0;i<7;++i){cold.input.weapons[i]={};cold.memory[Fixture::W+0xa4+4*i]=0;}assert(cold.run()&&cold.draws.empty());assert(cold.ops.size()==8); // Eight actual fctiwz, no geometry/raster.
 Fixture unknown;unknown.input.weapons[0].matrix.reset();assert(!unknown.run()&&unknown.cursor.pending()==0x8029dd64);assert(unknown.ops.back().kind==0x8029e230);auto trace=unknown.trace;assert(!unknown.run()&&unknown.trace==trace);unknown.input.weapons[0].matrix=BattleActMatrix{};assert(!unknown.run()); // Held input immutable, never fabricate missing stack cell.
 Fixture fields;fields.input.weapons[0].label.reset();assert(!fields.run()&&fields.cursor.pending()==0x8011847c&&fields.draws.empty());
 Fixture changing;bool first=true;changing.onOp=[&](auto&f,const Event&e){if(e.kind==0xf0000001&&f.ops.size()==2&&first){first=false;f.memory[Fixture::C+8]=0;f.memory[Fixture::W+0x98]=bits(100.f);}};assert(changing.run()&&changing.draws.size()==1&&changing.draws[0].row==0); // Row's captured count/scroll precede conversion.
 Fixture stale;stale.hold=0;assert(!stale.run());stale.alive=false;assert(!stale.run()&&stale.cursor.stale());trace=stale.trace;stale.alive=true;assert(!stale.run()&&stale.trace==trace);assert(stale.cursor.reset());
 Fixture absent;auto s=absent.services();s.current=nullptr;assert(!resumePackedBattleSubIcon(Fixture::W,absent.input,17,29,s,absent.cursor)&&!absent.cursor.active());
 std::cout<<"Whole DrawSubIcon row gates, unknown operands, source captures, every-child holds and owner checks passed\n";
}

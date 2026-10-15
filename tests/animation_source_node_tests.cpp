#include "animation_source_node.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t N=0x90000000,B=0x803d8e70;
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
constexpr uint32_t J[]{0xe5b8,0xe5c8,0xe5d0,0xe5d8,0xe5f0,0xe5f8,0xe610,0xe628,0xe650,0xe658,0xe670,0xe688,0xe6b0,0xe6c8,0xe6f0,0xe718},T[]{0xebc8,0xebd8,0xebe0,0xebe8,0xec00,0xec08,0xec20,0xec38};
struct Op {char kind;uint32_t a=0,b=0,c=0;bool operator==(const Op& q)const{return kind==q.kind && a==q.a && b==q.b && c==q.c;}};
bool eq(AnimationNodeRef a,AnimationNodeRef b){return a.guest==b.guest && a.value==b.value;}
bool eq(const AnimationSourceMatrixCall& a,const AnimationSourceMatrixCall& b){return a.function==b.function && a.axis==b.axis && eq(a.left,b.left) && eq(a.right,b.right) && eq(a.output,b.output) && !std::memcmp(a.xyz.data(),b.xyz.data(),12) && !std::memcmp(&a.radians,&b.radians,4);}
struct Fixture {
 AnimationGroupNode input{N,false,{1,1,1}};std::map<uint32_t,uint32_t> ram{{0x8041fb24,bits(1)},{0x8041fb28,0},{0x8041fb64,bits(2)},{0x8041fb2c,0x3c8efa35},{0x8041e89c,0},{0x8041e8a0,B}};
 std::vector<Op> trace;std::vector<AnimationSourceMatrixCall> calls;size_t dispatchStart=0;unsigned attempts=0,hold=0;bool held=false;void (*after)(Fixture&,char,uint32_t)=nullptr;
 Fixture(){for(unsigned i=0;i<24;++i)ram[N+4*i]=bits(i>=3 && i<=5?1.f:0.f);for(unsigned i=0;i<16;++i)ram[0x80306698+4*i]=0x80030000+J[i];for(unsigned i=0;i<8;++i)ram[0x803066d8+4*i]=0x80030000+T[i];}
 bool admit(char k,uint32_t a=0,uint32_t b=0,uint32_t c=0){if(++attempts==hold && !held){held=true;return false;}trace.push_back({k,a,b,c});return true;}
 void changed(char k,uint32_t a){if(after)after(*this,k,a);}
 void mode(bool joint,unsigned mask){input.joint=joint;if(mask&1)ram[N]=bits(3);if(joint){input.parent=mask&2?std::array<float,3>{1,1,1}:std::array<float,3>{1,1,2};ram[N+0x14]=bits(mask&4?1.f:2.f);if(mask&8)ram[N+0x2c]=bits(7);}else{if(mask&2)ram[N+0xc]=bits(2);if(mask&4)ram[N+0x20]=bits(7);}}
 AnimationSourceNodeServices services(){AnimationSourceNodeServices s;s.user=this;
  s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);auto i=f.ram.find(a);if(i==f.ram.end() || !f.admit('R',a,i->second))return {};auto v=i->second;if(a>=0x80306698 && a<0x803066f8)f.dispatchStart=f.calls.size();f.changed('R',a);return v;};
  s.store=[](void* u,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(u);if(!f.admit('S',a,v))return false;f.ram[a]=v;f.changed('S',a);return true;};
  s.number=[](void* u,AnimationNodeNumber n,uint32_t a,uint32_t b)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);if(!f.admit(n==AnimationNodeNumber::Multiply?'*':'/',a,b))return {};volatile float v=n==AnimationNodeNumber::Multiply?real(a)*real(b):real(a)/real(b);return bits(float(v));};
  s.matrix=[](void* u,const AnimationSourceMatrixCall& q,AnimationNodeLocals&){auto& f=*static_cast<Fixture*>(u);if(!f.admit('M',q.function,q.output.value,q.output.guest))return false;f.calls.push_back(q);f.changed('M',q.function);return true;};return s;
 }
 bool finish(AnimationSourceNodeCursor& c,unsigned visits=5){for(unsigned i=0;i<2000;++i)if(resumeAnimationSourceNode(input,services(),c,visits))return true;return false;}
};
void allMasksAndEveryHold(){for(bool joint:{false,true})for(unsigned mask=0;mask<(joint?16u:8u);++mask){Fixture base;base.mode(joint,mask);AnimationSourceNodeCursor c;assert(base.finish(c) && c.mask==mask && base.ram[0x8041e89c]==1 && base.ram[0x8041e8a0]==B+48);// Golden local slot/call tuples independently transcribed from original dispatch listings.
 const std::vector<std::vector<std::array<unsigned,3>>> j{{{99,0,0}},{},{},{{4,2,0}},{},{{4,3,0}},{{2,3,0}},{{4,2,0},{0,3,4}},{},{{4,1,0}},{{2,1,0}},{{4,2,0},{0,1,4}},{{1,3,0}},{{4,1,0},{0,3,4}},{{2,1,0},{0,3,4}},{{4,2,0},{0,1,4},{4,3,0}}};
 const std::vector<std::vector<std::array<unsigned,3>>> t{{{99,0,0}},{},{},{{4,3,0}},{},{{4,1,0}},{{1,3,0}},{{4,1,0},{0,3,4}}};
 constexpr unsigned selectedJ[]{0,4,2,0,3,0,0,4,1,0,0,4,0,4,4,0},selectedT[]{0,4,3,0,1,0,0,4};const auto& golden=(joint?j:t)[mask];assert(base.calls.size()-base.dispatchStart==golden.size()+1);for(unsigned k=0;k<golden.size();++k){const auto& q=base.calls[base.dispatchStart+k];assert(q.output.value==golden[k][2]);if(golden[k][0]==99)assert(q.function==0x8029dd04);else assert(q.function==0x8029dd64 && q.left.value==golden[k][0] && q.right.value==golden[k][1]);}
 const auto& final=base.calls.back();assert(final.right.value==(joint?selectedJ[mask]:selectedT[mask]));assert(final.function==0x8029dd64 && final.left.guest && final.left.value==B && final.output.guest && final.output.value==B+48 && !final.right.guest);
 for(unsigned at=1;at<=base.attempts;++at){Fixture f;f.mode(joint,mask);f.hold=at;AnimationSourceNodeCursor h;assert(f.finish(h,1) && f.held && f.trace==base.trace && f.ram==base.ram && f.calls.size()==base.calls.size());for(unsigned i=0;i<f.calls.size();++i)assert(eq(f.calls[i],base.calls[i]));}
 // Source field set and prefix ordering are not node24 snapshots.
 std::vector<uint32_t> fields;for(auto& o:base.trace)if(o.kind=='R' && o.a>=N && o.a<N+96)fields.push_back(o.a-N);
 std::vector<uint32_t> expected{0,0x18,0x1c,0x20,4,8,0xc,0x10,0x14};for(unsigned i=joint?9:12;i<(joint?12u:24u);++i)expected.push_back(4*i);assert(fields==expected);
 const std::vector<Op> prefix{{'R',N,base.ram[N]},{'R',0x8041fb28,0},{'R',0x8041fb64,bits(2)},{'R',N+0x18,0},{'R',N+0x1c,0},{'R',N+0x20,base.ram[N+0x20]},{'*',bits(2),0},{'*',bits(2),0},{'R',N+4,0},{'*',bits(2),base.ram[N+0x20]},{'R',N+8,0}};for(unsigned i=0;i<prefix.size();++i)assert(prefix[i]==base.trace[i]);
}}
void anglesLiveConstantsAndGates(){
 Fixture f;f.input.joint=true;f.ram[N]=bits(3);for(unsigned i=6;i<12;++i)f.ram[N+4*i]=bits(float(i));f.after=[](Fixture& x,char k,uint32_t fn){if(k=='M' && fn==0x8029e230){x.ram[0x8041fb24]=bits(2);x.ram[0x8041fb2c]=bits(.5f);x.ram[0x8041fb64]=bits(99);for(unsigned i=6;i<12;++i)x.ram[N+4*i]=bits(99);}};AnimationSourceNodeCursor c;assert(f.finish(c));std::vector<float> angles;std::vector<uint32_t> axes;bool compensation=false;for(auto& q:f.calls){if(q.function==0x8029dff0){angles.push_back(q.radians);axes.push_back(q.axis);}if(q.function==0x8029e2b0 && q.output.value==2){assert((q.xyz==std::array<float,3>{2,2,2}));compensation=true;}}assert(compensation && (axes==std::vector<uint32_t>{122,121,120,122,121,120}) && (angles==std::vector<float>{5.5f,5,4.5f,8,7,6}));
 Fixture fallback;fallback.input.joint=true;fallback.input.parent={2,3,4};fallback.ram[0x8041fb28]=bits(2);AnimationSourceNodeCursor b;assert(fallback.finish(b));bool uniform=false;for(auto& q:fallback.calls)if(q.function==0x8029e2b0 && q.output.value==2){assert((q.xyz==std::array<float,3>{1,1,1}));uniform=true;}assert(uniform);
 for(bool last:{false,true}){Fixture nan;nan.input.joint=true;nan.input.parent=last?std::array<float,3>{1,1,real(0x7fc00000)}:std::array<float,3>{real(0x7fc00000),1,1};AnimationSourceNodeCursor n;assert(nan.finish(n) && bool(n.mask&2)!=last);}
 Fixture zeros;zeros.ram[N]=0x80000000;zeros.ram[N+0x20]=0x80000000;AnimationSourceNodeCursor z;assert(zeros.finish(z) && z.mask==0);
}
void pivotsAliasesAndOrdering(){
 Fixture suppressed;suppressed.ram[N+0x48]=bits(7);suppressed.ram[N+0x54]=bits(9);AnimationSourceNodeCursor a;assert(suppressed.finish(a) && a.mask==0);for(auto& q:suppressed.calls)assert(q.function!=0x8029e230);
 Fixture translated;translated.ram[N+0x30]=bits(2);translated.ram[N+0x3c]=bits(3);translated.ram[N+0x48]=bits(7);translated.ram[N+0x54]=bits(9);AnimationSourceNodeCursor b;assert(translated.finish(b) && b.mask==6);unsigned translations=0;for(auto& q:translated.calls)if(q.function==0x8029e230){++translations;assert(q.xyz[0]==(q.output.value==3?9:7));}assert(translations==2);
 Fixture full;full.mode(false,7);full.ram[N+0x30]=bits(2);full.ram[N+0x3c]=bits(3);AnimationSourceNodeCursor c;assert(full.finish(c));bool first=false,second=false,sign=false;for(auto& q:full.calls){if(q.function==0x8029dd64 && q.output.value==0 && q.left.value==0 && q.right.value==3)first=true;if(q.function==0x8029dd64 && q.output.value==1 && q.left.value==0 && q.right.value==1)second=true;if(q.function==0x8029e230 && q.output.value==5){assert(bits(q.xyz[0])==bits(-3) && bits(q.xyz[1])==0x80000000);sign=true;}}assert(first && second && sign);
 for(unsigned at=1;at<=full.attempts;++at){Fixture h;h.mode(false,7);h.ram[N+0x30]=bits(2);h.ram[N+0x3c]=bits(3);h.hold=at;AnimationSourceNodeCursor d;assert(h.finish(d,1) && h.trace==full.trace && h.calls.size()==full.calls.size());for(unsigned i=0;i<h.calls.size();++i)assert(eq(h.calls[i],full.calls[i]));}
}
void stackTablesAndIdentity(){
 Fixture f;f.mode(false,7);f.after=[](Fixture& x,char k,uint32_t a){if(k=='R' && a==0x803066d8+28)x.ram[0x8041e89c]=7;};AnimationSourceNodeCursor c;assert(f.finish(c));assert(f.calls.back().left.value==B+6*48 && f.calls.back().output.value==B+7*48 && f.ram[0x8041e8a0]==B+48);
 Fixture missing;missing.ram.erase(0x803066d8);AnimationSourceNodeCursor m;assert(!resumeAnimationSourceNode(missing.input,missing.services(),m) && missing.ram[0x8041e89c]==1);missing.ram[0x803066d8]=0x8003ebc8;assert(missing.finish(m) && missing.ram[0x8041e89c]==1);
 Fixture bad;bad.ram[0x803066d8]=123;AnimationSourceNodeCursor h;assert(!resumeAnimationSourceNode(bad.input,bad.services(),h));auto before=bad.trace;bad.ram[0x803066d8]=0x8003ebc8;assert(!resumeAnimationSourceNode(bad.input,bad.services(),h) && bad.trace==before && bad.ram[0x8041e89c]==1);
 Fixture identity;identity.input.parent[0]=0;AnimationSourceNodeCursor i;auto s=identity.services();assert(!resumeAnimationSourceNode(identity.input,s,i,1));auto trace=identity.trace;auto q=identity.input;q.parent[0]=-0.f;assert(!resumeAnimationSourceNode(q,s,i) && identity.trace==trace);s.matrix=nullptr;assert(!resumeAnimationSourceNode(identity.input,s,i) && identity.trace==trace);assert(identity.finish(i));
}
}
int main(){allMasksAndEveryHold();anglesLiveConstantsAndGates();pivotsAliasesAndOrdering();stackTablesAndIdentity();}

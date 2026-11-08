#include <memory>
#include "source_icon_setup.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace ttyd;
namespace {
using Event=std::vector<uint32_t>;
struct F {
 SourceIconSetupServices s;SourceIconSetupCursor c;std::map<uint32_t,uint8_t>m;std::vector<Event>trace;
 bool alive=true,reentry=false,mutate=false;unsigned kind=0,opens=0,allocations=0,closes=0,asyncs=0,formats=0;
 bool tpl=true,bin=true;size_t hold=size_t(-1);unsigned attempts=0;const SourceIconPath*borrowed=nullptr;
 static constexpr uint32_t T=0x89000000,B=0x89100000,H=0x90000000,D=0x91000000;
 F(){auto&g=s.common.objects.gx;g.user=this;g.lifetime=std::make_shared<int>(1);g.current=[](void*p){return static_cast<F*>(p)->alive;};
 g.word=[](void*p,uint32_t a)->std::optional<uint32_t>{return static_cast<F*>(p)->read(a,4);};
 g.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto v=static_cast<F*>(p)->read(a,1);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
 g.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto&f=*static_cast<F*>(p);if(!f.ready())return false;f.trace.push_back({2,a,n,v});f.put(a,n,v);return true;};
 s.request=[](void*p,const SourceIconSetupCall&q){return static_cast<F*>(p)->request(q);};
 put(D+0x2c,4,H);put(0x8041e630,4,T);put(0x8041e638,4,77);put(0x8041e63c,4,88);
 // Test-only minimal TPL; actual raw613-image file covered separately.
 put(T,4,0x0020af30);put(T+4,4,1);put(T+8,4,12);put(T+12,4,32);put(T+16,4,0);put(T+32+35,1,0);put(T+32+8,4,128);
 }
 void put(uint32_t a,unsigned n,uint32_t v){for(unsigned i=0;i<n;++i)m[a+i]=uint8_t(v>>(8*(n-i-1)));}
 std::optional<uint32_t>get(uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;++i){auto it=m.find(a+i);if(it==m.end())return {};v=v<<8|it->second;}return v;}
 uint32_t word(uint32_t a){auto v=get(a,4);assert(v);return *v;}
 bool ready(){if(reentry){assert(!c.reset());assert(!resume());}return trace.size()!=hold||attempts++>=2;}
 std::optional<uint32_t>read(uint32_t a,unsigned n){if(!ready())return {};auto v=get(a,n);if(v)trace.push_back({1,a,n,*v});return v;}
 std::optional<uint32_t>request(const SourceIconSetupCall&q){if(q.destination||q.path){auto*p=q.destination?q.destination:q.path;if(borrowed)assert(borrowed==p);borrowed=p;}if(!ready())return {};
 Event e{3,q.function,q.callsite,q.count};e.insert(e.end(),q.arguments.begin(),q.arguments.begin()+q.count);trace.push_back(e);
 switch(q.function){case 0x800300f0:{++allocations;if(q.callsite==0x8001b6bc||q.callsite==0x8001b6ec){assert(q.arguments[1]==1792);if(mutate)put(q.callsite==0x8001b6bc?0x8041e640:0x8041e648,4,3);return 0x92000000+allocations*0x1000;}assert(q.arguments[1]==(q.callsite==0x8001b760?96u:8192u));return q.callsite==0x8001b760?T:B;}
 case 0x800050b4:assert(q.count==3&&q.arguments[1]==0&&q.arguments[2]==(mutate?168u:1792u));for(uint32_t i=0;i<q.arguments[2];++i)put(q.arguments[0]+i,1,0);return q.arguments[0];
 case 0x80007388:return 0x80001000;
 case 0x8026a0b8:{++formats;assert(q.destination&&q.count==2&&q.arguments[1]==0x80001000);assert(q.arguments[0]==(formats==1?0x802bf858u:0x802bf864u));const char*a=formats==1?"./icon.tpl":"./icon.bin";unsigned i=0;do{q.destination->cells[i]=uint8_t(a[i]);}while(a[i++]);assert(!q.destination->byte(127));return i-1;}
 case 0x80144e88:++opens;assert(q.path&&q.path->byte(0)=='.'&&q.arguments[0]==2&&q.arguments[1]==0);return (opens==1?tpl:bin)?H+opens*0x100:0;
 case 0x80144dcc:return q.arguments[0]==H+0x100?65:8174;
 case 0x80144de4:++asyncs;assert(q.count==5&&q.arguments[3]==0);assert(q.arguments[0]==(q.callsite==0x8001b784?H+0x100:H+0x200));assert(q.arguments[1]==(q.callsite==0x8001b784?T:B));assert(q.arguments[4]==(q.callsite==0x8001b784?0x8001b83cu:0x8001b810u));return 0;
 case 0x80144dd4:assert(q.arguments[0]==H);++closes;return 0;
 default:assert(false);return {};}}
 bool resume(){return kind==0?resumeSourceIconSetup(s,c):kind==1?resumeSourceIconBinCompletion(D,s,c):resumeSourceIconTplCompletion(D,s,c);}
 void run(){for(unsigned n=0;n<10000;++n)if(resume())return;assert(false);}
};
void reference(F&f){auto w=[&](uint32_t a){auto v=f.s.common.objects.gx.word(&f,a);assert(v);return *v;};auto st=[&](uint32_t a,uint32_t v){assert(f.s.common.objects.gx.store(&f,a,4,v));};SourceIconPath path;
 auto call=[&](uint32_t fn,uint32_t site,std::initializer_list<uint32_t>args,bool dest=false,bool text=false){SourceIconSetupCall q;q.function=fn;q.callsite=site;q.count=uint8_t(args.size());unsigned i=0;for(auto v:args)q.arguments[i++]=v;q.destination=dest?&path:nullptr;q.path=text?&path:nullptr;auto v=f.request(q);assert(v);return *v;};
 if(f.kind){const auto handle=w(F::D+0x2c);if(f.kind==2){const auto p=w(0x8041e630);SourceWindowTextureSetupCursor u;assert(resumeSourceUnpackTexturePalette(p,f.s.common,u));}call(0x80144dd4,f.kind==1?0x8001b820:0x8001b85c,{handle});st(f.kind==1?0x8041e63c:0x8041e638,1);return;}
 st(0x8041e640,32);auto heap=call(0x800300f0,0x8001b6bc,{0,1792});auto count=w(0x8041e640);st(0x8041e644,heap);call(0x800050b4,0x8001b6d4,{heap,0,count*56});st(0x8041e648,32);heap=call(0x800300f0,0x8001b6ec,{0,1792});count=w(0x8041e648);st(0x8041e64c,heap);call(0x800050b4,0x8001b700,{heap,0,count*56});for(uint32_t a=0x8041e630;a<0x8041e640;a+=4)st(a,0);
 auto root=call(0x80007388,0x8001b718,{});call(0x8026a0b8,0x8001b730,{0x802bf858,root},true);auto h=call(0x80144e88,0x8001b740,{2,0},false,true);if(h){const auto n=(call(0x80144dcc,0x8001b74c,{h})+31)&0xffffffe0u;heap=call(0x800300f0,0x8001b760,{0,n});st(0x8041e630,heap);st(h+0x6c,h);auto p=w(0x8041e630);call(0x80144de4,0x8001b784,{h,p,n,0,0x8001b83c});}
 root=call(0x80007388,0x8001b788,{});call(0x8026a0b8,0x8001b7a0,{0x802bf864,root},true);h=call(0x80144e88,0x8001b7b0,{2,0},false,true);if(h){const auto n=(call(0x80144dcc,0x8001b7bc,{h})+31)&0xffffffe0u;heap=call(0x800300f0,0x8001b7d0,{0,n});st(0x8041e634,heap);st(h+0x6c,h);const auto p=w(0x8041e634);call(0x80144de4,0x8001b7f4,{h,p,n,0,0x8001b810});}}
void matrix(){for(unsigned mode=0;mode<10;++mode){F base,ref;base.kind=ref.kind=mode<8?0:mode-7;base.tpl=ref.tpl=bool(mode&1);base.bin=ref.bin=bool(mode&2);base.mutate=ref.mutate=bool(mode&4);base.run();reference(ref);assert(base.trace==ref.trace&&base.m==ref.m);for(size_t h=0;h<base.trace.size();++h){F f;f.kind=base.kind;f.tpl=base.tpl;f.bin=base.bin;f.mutate=base.mutate;f.hold=h;f.reentry=true;f.run();assert(f.attempts==3&&f.trace==base.trace&&f.m==base.m);}if(!base.kind){assert(base.word(0x8041e638)==0&&base.word(0x8041e63c)==0&&base.asyncs==unsigned(base.tpl)+unsigned(base.bin));}else assert(base.closes==1&&base.word(base.kind==1?0x8041e63c:0x8041e638)==1);}}
void identity(){F f;f.hold=1;assert(!f.resume());auto prefix=f.trace;auto s=f.s;s.request=nullptr;assert(!resumeSourceIconSetup(s,f.c)&&f.trace==prefix);s=f.s;s.common.objects.gx.lifetime=std::shared_ptr<const void>(std::make_shared<int>(1),f.s.common.objects.gx.lifetime.get());assert(!resumeSourceIconSetup(s,f.c));assert(!resumeSourceIconBinCompletion(F::D,f.s,f.c));f.alive=false;assert(!f.resume()&&f.c.stale());f.alive=true;assert(!f.resume());assert(f.c.reset());f.hold=size_t(-1);f.run();
 F cb;cb.kind=2;cb.hold=2;assert(!cb.resume());assert(!resumeSourceIconTplCompletion(F::D+4,cb.s,cb.c));cb.put(F::D+0x2c,4,123);cb.run();assert(cb.closes==1); // Original handle captured before relocation wait.
 F missing;missing.s.request=nullptr;assert(!missing.resume()&&missing.c.pending()==0x800300f0&&missing.word(0x8041e640)==32);}
void actual(){F f;std::ifstream in("romfs/source-icons/icon.tpl",std::ios::binary);assert(in);std::vector<uint8_t>raw((std::istreambuf_iterator<char>(in)),{});assert(raw.size()==677792);for(size_t i=0;i<raw.size();++i)f.m[F::T+uint32_t(i)]=raw[i];f.kind=2;f.run();assert(f.word(F::T+4)==613&&f.word(F::T+8)==F::T+12&&f.word(0x8041e638)==1&&f.word(0x8041e63c)==88&&f.closes==1);assert(raw[8]==0&&raw[11]==12);}
}
int main(){matrix();identity();actual();std::cout<<"source icon setup tests PASS\n";}

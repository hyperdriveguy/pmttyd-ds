#include "source_window_texture_setup.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>
using namespace ttyd;
namespace {
using Event=std::array<uint64_t,6>;
uint32_t bits(float x){uint32_t v;std::memcpy(&v,&x,4);return v;}
double real(uint64_t x){double v;std::memcpy(&v,&x,8);return v;}
uint32_t be(const std::vector<uint8_t>&b,unsigned n){assert(n+4<=b.size());return uint32_t(b[n])<<24|uint32_t(b[n+1])<<16|uint32_t(b[n+2])<<8|b[n+3];}
struct F {
 static constexpr uint32_t P=0x89000000,O=0x90000000,H=0x91000000,D=0x92000000;
 std::map<uint32_t,uint8_t> m;std::vector<Event>trace;SourceWindowTextureSetupServices s;SourceWindowTextureSetupCursor c;
 bool alive=true,archive=true,reenter=false,partial=false;unsigned clears=0,asyncs=0,closes=0,formats=0;uint32_t clearPos=0;
 uint32_t kind=0,arg=0;size_t holdAt=size_t(-1);unsigned attempts=0;const SourceWindowTexturePath* borrowed=nullptr;
 F(){s.objects.gx.user=this;s.objects.gx.lifetime=std::make_shared<int>(1);s.objects.gx.current=[](void*p){return static_cast<F*>(p)->alive;};
 s.objects.gx.word=[](void*p,uint32_t a)->std::optional<uint32_t>{return static_cast<F*>(p)->read(a,4);};
 s.objects.gx.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto v=static_cast<F*>(p)->read(a,1);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
 s.objects.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto v=static_cast<F*>(p)->read(a,2);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;};
 s.objects.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto&f=*static_cast<F*>(p);if(!f.ready())return false;f.trace.push_back({2,a,n,v});f.put(a,n,v);return true;};
 s.objects.gx.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<F*>(p);if(!f.ready())return {};if(!f.clearPos){++f.clears;f.trace.push_back({3,a});}while(f.clearPos<32){f.put(a+f.clearPos++,1,0);if(f.partial&&f.clearPos==16)return {};}f.clearPos=0;return 0;};
 s.objects.gx.truncate=[](void*p,float x)->std::optional<int32_t>{auto&f=*static_cast<F*>(p);if(!f.ready()||!std::isfinite(x)||double(x)<-2147483648.0||double(x)>=2147483648.0)return {};f.trace.push_back({4,bits(x)});return int32_t(x);};
 s.objects.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&f=*static_cast<F*>(p);if(!f.ready())return {};const auto hi=f.get(a,4),lo=f.get(a+4,4);if(!hi||!lo)return {};const auto v=uint64_t(*hi)<<32|*lo;f.trace.push_back({5,a,v});return v;};
 s.objects.subtractSingle=[](void*p,uint64_t a,uint64_t b)->std::optional<uint32_t>{auto&f=*static_cast<F*>(p);if(!f.ready())return {};volatile float x=float(real(a)-real(b));f.trace.push_back({6,a,b,bits(x)});return bits(x);};
 s.request=[](void*p,const SourceWindowTextureSetupCall&q)->std::optional<uint32_t>{return static_cast<F*>(p)->request(q);};
 synthetic();
 }
 bool ready(){if(reenter){assert(!c.reset());assert(!resume());}return trace.size()!=holdAt||attempts++>=2;}
 void put(uint32_t a,uint32_t n,uint32_t v){for(uint32_t i=0;i<n;++i)m[a+i]=uint8_t(v>>(8*(n-i-1)));}
 std::optional<uint32_t>get(uint32_t a,unsigned n){uint32_t v=0;for(unsigned i=0;i<n;++i){auto b=m.find(a+i);if(b==m.end())return {};v=v<<8|b->second;}return v;}
 uint32_t word(uint32_t a){auto v=get(a,4);assert(v);return *v;}
 std::optional<uint32_t>read(uint32_t a,unsigned n){if(!ready())return {};auto v=get(a,n);if(v)trace.push_back({1,a,n,*v});return v;}
 std::optional<uint32_t>request(const SourceWindowTextureSetupCall&q){
 if(q.destination||q.path){auto*ptr=q.destination?q.destination:q.path;if(borrowed)assert(borrowed==ptr);borrowed=ptr;}
 if(!ready())return {};
 Event e{7,q.function,q.callsite};for(unsigned i=0;i<3&&i<q.count;++i)e[i+3]=q.arguments[i];trace.push_back(e);
 switch(q.function){
 case 0x800782e4:assert(q.count==3&&q.arguments[0]==0x802c3080);return archive?P:0;
 case 0x80007388:return 0x80010000;
 case 0x8026a0b8:{assert(q.destination&&q.count==2&&q.arguments[0]==0x802c3094);++formats;const char text[]="dvd:/f/msgWindow.tpl";for(unsigned i=0;i<sizeof(text);++i)q.destination->cells[i]=uint8_t(text[i]);assert(!q.destination->byte(127));return sizeof(text)-1;}
 case 0x80144e88:assert(q.path&&q.path->byte(0)=='d'&&q.count==2&&q.arguments[0]==2);return H;
 case 0x80144dcc:assert(q.arguments[0]==H);return 65;
 case 0x800300f0:assert(q.count==2&&q.arguments[0]==0);return q.arguments[1]==96?P:O;
 case 0x80144de4:assert((q.count==5&&q.arguments==std::array<uint32_t,5>({H,P,96,0,0x8007fcf0})));++asyncs;return 0;
 case 0x80144dd4:assert(q.arguments[0]==H);++closes;return 0;
 case 0x80297894:assert(q.arguments[0]==0x802c0148&&q.arguments[1]==36&&q.arguments[2]==0x802c0158);return 0; // test-only returning panic owner
 default:assert(false);return {};
 }}
 void synthetic(){put(0x8041ea18,4,P);put(0x8041ea1c,4,77);put(0x8041ea24,4,0xdead0000);put(D+0x2c,4,H);put(P,4,0x0020af30);put(P+4,4,2);put(P+8,4,12);
 for(unsigned i=0;i<2;++i){const uint32_t h=0x40+0x40*i,t=0xc0+0x10*i;put(P+12+8*i,4,h);put(P+16+8*i,4,t);for(unsigned j=0;j<36;++j)m[P+h+j]=0;put(P+h,2,24);put(P+h+2,2,16);put(P+h+4,4,14);put(P+h+8,4,0x1000);put(P+h+20,4,1);put(P+h+24,4,1);put(P+h+33,1,0);put(P+h+34,1,0);for(unsigned j=0;j<12;++j)m[P+t+j]=0;put(P+t+8,4,0x2000);}
 uint32_t table[]={0x802b7430,0x802b7444,0x802b7444,0x802b7458,0x802b7458,0x802b7458,0x802b746c,0x802b7494,0x802b7430,0x802b7444,0x802b7458,0x802b7494,0x802b7494,0x802b7494,0x802b7480};for(unsigned i=0;i<15;++i)put(0x803c4e84+4*i,4,table[i]);uint8_t filters[]={0,4,1,5,2,6,0,0};for(unsigned i=0;i<8;++i)m[0x8041e558+i]=filters[i];uint32_t constants[]={0x41800000,0,0x43300000,0,0xc0800000,0x40800000,0x407f5c29,0x42000000,0,0x41200000};for(unsigned i=0;i<10;++i)put(0x80429440+4*i,4,constants[i]);put(0x802c0140,4,0x43300000);put(0x802c0144,4,0);
 }
 bool resume(){return kind==0?resumeSourceWindowTextureSetup(s,c):kind==1?resumeSourceWindowTextureCompletion(arg,s,c):resumeSourceUnpackTexturePalette(arg,s,c);}
 void run(){for(unsigned i=0;i<10000;++i)if(resume())return;assert(false);}
};
void prepare(F&f,unsigned mode){if(mode==1){f.archive=false;}if(mode==2){f.kind=1;f.arg=F::D;}if(mode==3){f.kind=2;f.arg=F::P;}}
void holds(){for(unsigned mode=0;mode<4;++mode){F baseline;prepare(baseline,mode);baseline.run();assert(!baseline.c.active());for(size_t at=0;at<baseline.trace.size();++at){F f;prepare(f,mode);f.holdAt=at;f.reenter=true;f.run();assert(f.attempts==3&&f.trace==baseline.trace&&f.m==baseline.m);}if(mode==0)assert(baseline.word(0x8041ea1c)==0&&baseline.clears==2);if(mode==1)assert(baseline.word(0x8041ea1c)==0&&baseline.asyncs==1&&baseline.word(F::H+0x6c)==F::H&&baseline.word(0x8041ea24)==0xdead0000);if(mode==2)assert(baseline.word(0x8041ea1c)==1&&baseline.closes==1&&baseline.clears==2);}}
// Independent direct translation of the complete relocation body, using the
// same optional memory callbacks solely to compare exact observable accesses.
void referenceUnpack(F&f,uint32_t p){auto w=[&](uint32_t a){auto v=f.s.objects.gx.word(&f,a);assert(v);return *v;};auto b=[&](uint32_t a){auto v=f.s.objects.gx.byte(&f,a);assert(v);return *v;};auto store=[&](uint32_t a,uint32_t n,uint32_t v){assert(f.s.objects.gx.store(&f,a,n,v));};if(w(p)!=0x0020af30){SourceWindowTextureSetupCall q;q.function=0x80297894;q.callsite=0x80037ba8;q.count=3;q.arguments={0x802c0148,36,0x802c0158};assert(f.request(q));}auto table=w(p+8);if(table>0x80000000u)return;store(p+8,4,table+p);uint32_t i=0;while(uint32_t(uint16_t(i))<w(p+4)){table=w(p+8);uint32_t off=(i<<3)&0x0007fff8;auto header=w(table+off);if(header){store(table+off,4,p+header);table=w(p+8);header=w(table+off);if(!b(header+35)){auto data=w(header+8);store(header+8,4,p+data);table=w(p+8);header=w(table+off);store(header+35,1,1);}}table=w(p+8);off+=4;header=w(table+off);if(header){store(table+off,4,p+header);table=w(p+8);header=w(table+off);if(!b(header+2)){auto data=w(header+8);store(header+8,4,p+data);table=w(p+8);header=w(table+off);store(header+2,1,1);}}++i;}}
void referenceParent(F&f,bool callback){
 auto w=[&](uint32_t a){auto v=f.s.objects.gx.word(&f,a);assert(v);return *v;};
 auto st=[&](uint32_t a,uint32_t v){assert(f.s.objects.gx.store(&f,a,4,v));};
 SourceWindowTexturePath path;
 auto call=[&](uint32_t fn,uint32_t site,std::initializer_list<uint32_t>a,bool dest=false,bool text=false){SourceWindowTextureSetupCall q;q.function=fn;q.callsite=site;q.count=uint8_t(a.size());unsigned i=0;for(auto v:a)q.arguments[i++]=v;q.destination=dest?&path:nullptr;q.path=text?&path:nullptr;auto v=f.request(q);assert(v);return *v;};
 if(!callback){st(0x8041ea18,0);st(0x8041ea1c,0);const auto p=call(0x800782e4,0x8007fc08,{0x802c3080,0,0});st(0x8041ea18,p);if(!p){auto root=call(0x80007388,0x8007fc18,{});call(0x8026a0b8,0x8007fc30,{0x802c3094,root},true);auto handle=call(0x80144e88,0x8007fc40,{2,0},false,true);const auto length=call(0x80144dcc,0x8007fc48,{handle}),rounded=(length+31)&0xffffffe0u;st(0x8041ea18,call(0x800300f0,0x8007fc5c,{0,rounded}));st(handle+0x6c,handle);const auto palette=w(0x8041ea18);call(0x80144de4,0x8007fc80,{handle,palette,rounded,0,0x8007fcf0});return;}referenceUnpack(f,p);}
 else{const auto handle=w(f.arg+0x2c),palette=w(0x8041ea18);referenceUnpack(f,palette);call(0x80144dd4,0x8007fd14,{handle});}
 auto palette=w(0x8041ea18);const auto count=w(palette+4);auto objects=call(0x800300f0,callback?0x8007fd28:0x8007fc9c,{0,count<<5});st(0x8041ea24,objects);uint32_t i=0,offset=0;
 for(;;){palette=w(0x8041ea18);if(i>=w(palette+4))break;objects=w(0x8041ea24);SourcePaletteTextureObjectCursor child;assert(resumeSourcePaletteTextureObject(palette,objects+offset,i,f.s.objects,child));offset+=32;++i;}
 if(callback)st(0x8041ea1c,1);
}
void parentReference(){for(unsigned mode=0;mode<3;++mode){F f,g;prepare(f,mode);prepare(g,mode);referenceParent(f,mode==2);g.run();assert(f.trace==g.trace&&f.m==g.m);}}
void lateGlobals(){
 F f;f.kind=1;f.arg=F::D;
 F reference;reference.kind=1;reference.arg=F::D;reference.run();size_t close=0;while(reference.trace[close][0]!=7||reference.trace[close][1]!=0x80144dd4)++close;
 f.holdAt=close;assert(!f.resume()&&f.c.pending()==0x80144dd4);const auto before=f.trace;f.put(F::D+0x2c,4,0x1234);f.put(0x8041ea18,4,F::P+0x10000);f.put(F::P+0x10004,4,0);f.run();assert(f.word(0x8041ea1c)==1&&f.closes==1&&!f.clears);assert(f.trace.size()>before.size());
 // Captured callback handle remains original H; later global palette/count
 // are genuinely fresh after the admitted Close, producing an empty array.
 F missing;missing.kind=1;missing.arg=F::D;missing.m.erase(F::D+0x2c);assert(!missing.resume()&&missing.c.pending()==0x8007fd04&&missing.trace.empty());
}
void relocation(){for(unsigned branch=0;branch<8;++branch){F f,g;for(auto*p:{&f,&g}){p->kind=2;p->arg=F::P;if(branch&1){p->put(F::P+12,4,0);p->put(F::P+24,4,0);}if(branch&2){p->put(F::P+0x40+35,1,1);p->put(F::P+0xc0+2,1,1);}if(branch&4)p->put(F::P,4,0);}referenceUnpack(f,F::P);g.run();assert(f.trace==g.trace&&f.m==g.m);}
 F absolute;absolute.kind=2;absolute.arg=F::P;absolute.put(F::P+8,4,0x80000001);absolute.run();assert(absolute.trace.size()==2);F threshold;threshold.kind=2;threshold.arg=F::P;threshold.put(F::P+8,4,0x80000000);threshold.put(F::P+4,4,0);threshold.run();assert(threshold.word(F::P+8)==0x09000000);F zero;zero.kind=1;zero.arg=F::D;zero.put(F::P+4,4,0);zero.run();assert(zero.word(0x8041ea1c)==1&&!zero.clears);
}
void actual(){F f;std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);auto read=[&](uint32_t a,unsigned n){std::vector<uint8_t>b(n);iso.seekg(a);iso.read(reinterpret_cast<char*>(b.data()),n);assert(iso);return b;};assert(read(0,8)==std::vector<uint8_t>({'G','8','M','E','0','1',0,0}));auto dol=be(read(0x420,4),0);auto header=read(dol,256);for(auto span:{std::pair<uint32_t,unsigned>{0x803c4e84,60},{0x8041e558,8},{0x80429440,40},{0x802c0140,8}}){bool found=false;for(unsigned i=0;i<18;++i){auto base=be(header,0x48+4*i),n=be(header,0x90+4*i);if(base<=span.first&&uint64_t(span.first)+span.second<=uint64_t(base)+n){auto bytes=read(dol+be(header,4*i)+span.first-base,span.second);for(unsigned j=0;j<bytes.size();++j)f.m[span.first+j]=bytes[j];found=true;}}assert(found);}auto tpl=read(947396944,321984);assert(be(tpl,4)==51);for(unsigned i=0;i<tpl.size();++i)f.m[F::P+i]=tpl[i];f.kind=1;f.arg=F::D;f.run();assert(f.word(0x8041ea1c)==1&&f.clears==51&&f.closes==1);const auto table=f.word(F::P+8),image=f.word(table);assert(f.word(image+8)>=F::P&&f.word(F::O+20)==14&&f.word(0x8041ea24)==F::O);}
void identity(){F f;f.holdAt=2;assert(!f.resume());auto prefix=f.trace;auto changed=f.s;changed.request=nullptr;assert(!resumeSourceWindowTextureSetup(changed,f.c)&&f.trace==prefix);changed=f.s;changed.objects.gx.lifetime=std::shared_ptr<const void>(std::make_shared<int>(2),f.s.objects.gx.lifetime.get());assert(!resumeSourceWindowTextureSetup(changed,f.c));f.alive=false;assert(!f.resume()&&f.c.stale());f.alive=true;assert(!f.resume());assert(f.c.reset());f.holdAt=size_t(-1);f.run();F missing;missing.s.request=nullptr;assert(!missing.resume()&&missing.c.pending()==0x800782e4);F partial;partial.partial=true;assert(!partial.resume());assert(partial.c.childPending()==0x800050b4);assert(partial.word(0x8041ea1c)==0);partial.run();assert(partial.clears==2);F cb;cb.kind=1;cb.arg=F::D;cb.holdAt=2;assert(!cb.resume());assert(!resumeSourceWindowTextureCompletion(F::D+4,cb.s,cb.c));}
}
int main(){holds();parentReference();lateGlobals();relocation();actual();identity();std::cout<<"source window texture setup tests PASS\n";}

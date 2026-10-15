#include "source_gx_texture_object.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct F {
 std::map<uint32_t,uint8_t> m;std::vector<uint64_t> trace;bool alive=true,holdAll=false,first=true,failClear=false;
 uint32_t obj=0x90000000,holdTrunc=0,truncateCalls=0,clearCalls=0;SourceGxTextureObjectCursor*reenter=nullptr;SourceGxTextureObjectServices*s=nullptr;SourceGxTextureObjectInput*q=nullptr;uint32_t fn=0;
 F(){uint32_t table[]={0x802b7430,0x802b7444,0x802b7444,0x802b7458,0x802b7458,0x802b7458,0x802b746c,0x802b7494,0x802b7430,0x802b7444,0x802b7458,0x802b7494,0x802b7494,0x802b7494,0x802b7480};for(unsigned i=0;i<15;++i)put(0x803c4e84+4*i,4,table[i]);uint8_t filters[]={0,4,1,5,2,6,0,0};for(unsigned i=0;i<8;++i)m[0x8041e558+i]=filters[i];uint32_t constants[]={0x41800000,0,0x43300000,0,0xc0800000,0x40800000,0x407f5c29,0x42000000,0,0x41200000};for(unsigned i=0;i<10;++i)put(0x80429440+4*i,4,constants[i]);for(unsigned i=0;i<32;++i)m[obj+i]=0xa5;}
 void put(uint32_t a,unsigned n,uint32_t v){for(unsigned i=0;i<n;++i)m[a+i]=uint8_t(v>>(8*(n-1-i)));}
 uint32_t get(uint32_t a,unsigned n=4){uint32_t v=0;for(unsigned i=0;i<n;++i){assert(m.count(a+i));v=(v<<8)|m[a+i];}return v;}
 bool ready(){if(!holdAll)return true;first=!first;return first;}
 SourceGxTextureObjectServices services(){SourceGxTextureObjectServices v;v.user=this;v.lifetime=std::make_shared<int>(1);v.current=[](void*p){return static_cast<F*>(p)->alive;};
 v.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<F*>(p);if(!f.ready())return {};for(unsigned i=0;i<4;++i)if(!f.m.count(a+i))return {};f.trace.push_back((uint64_t(1)<<32)|a);return f.get(a);};
 v.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<F*>(p);if(!f.ready()||!f.m.count(a))return {};f.trace.push_back((uint64_t(2)<<32)|a);return f.m[a];};
 v.store=[](void*p,uint32_t a,uint32_t n,uint32_t x){auto&f=*static_cast<F*>(p);if(!f.ready())return false;f.trace.push_back((uint64_t(n+2)<<32)|a);f.trace.push_back(x);f.put(a,n,x);return true;};
 v.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<F*>(p);if(f.failClear||!f.ready())return {};if(f.reenter){assert(!f.reenter->reset());assert(!resumeSourceGxTextureObject(f.fn,*f.q,*f.s,*f.reenter));}++f.clearCalls;f.trace.push_back(uint64_t(9)<<32|a);for(unsigned i=0;i<32;++i)f.m[a+i]=0;return 0;};
 v.truncate=[](void*p,float x)->std::optional<int32_t>{auto&f=*static_cast<F*>(p);++f.truncateCalls;if(f.holdTrunc){--f.holdTrunc;return {};}if(!f.ready()||!std::isfinite(x)||double(x)<-2147483648.0||double(x)>=2147483648.0)return {};f.trace.push_back(uint64_t(10)<<32|bits(x));return int32_t(x);};return v;}
};
SourceGxTextureObjectInput init(uint32_t format=0,uint32_t mip=0){SourceGxTextureObjectInput q;q.integers={0x90000000,0x81234560,512,256,format,0,0,mip};return q;}
SourceGxTextureObjectInput lod(){SourceGxTextureObjectInput q;q.integers={0x90000000,1,1,0,0,0,0,0};return q;}
void run(uint32_t fn,const SourceGxTextureObjectInput&q,SourceGxTextureObjectServices&s,SourceGxTextureObjectCursor&c){for(unsigned n=0;n<1000;++n)if(resumeSourceGxTextureObject(fn,q,s,c)){assert(!c.active());return;}assert(false);}
void allBoundaries(){for(uint32_t fn:{0x802b72b0u,0x802b7544u})for(unsigned k=0;k<17;++k){auto q=fn==0x802b72b0?init(k,k%2):lod();if(fn==0x802b7544){q.integers[1]=k%6;q.integers[2]=k%3;q.integers[3]=k;q.integers[4]=k;q.integers[5]=k;q.reals={bits(float(k)-3),bits(float(k)+2),bits(float(k)-8)};}F a,b;auto sa=a.services(),sb=b.services();b.holdAll=true;SourceGxTextureObjectCursor ca,cb;run(fn,q,sa,ca);run(fn,q,sb,cb);assert(a.m==b.m&&a.trace==b.trace);}}
void originalFont(){F f;auto s=f.services();SourceGxTextureObjectCursor c;auto q=init();f.reenter=&c;f.q=&q;f.s=&s;f.fn=0x802b72b0;run(f.fn,q,s,c);assert(f.clearCalls==1);assert(f.get(f.obj)==0x90);assert(f.get(f.obj+4)==0);assert(f.get(f.obj+8)==0x3fdff);assert(f.get(f.obj+12)==((0x81234560u>>5)&0x1fffff));assert(f.get(f.obj+20)==0);assert(f.get(f.obj+28,2)==2048);assert(f.get(f.obj+30,1)==1&&f.get(f.obj+31,1)==2);q=lod();run(0x802b7544,q,s,c);assert(f.get(f.obj)==0x190);assert(f.get(f.obj+4)==0);}
// Actual original BFN/initialized DOL witness; no generated font or fixture
// table can satisfy this source resource chain test.
void actualOriginalFont(){
 std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);
 auto read=[&](uint32_t at,unsigned n){std::vector<uint8_t>v(n);iso.seekg(at);iso.read(reinterpret_cast<char*>(v.data()),n);assert(iso);return v;};
 auto be=[](const std::vector<uint8_t>&v,unsigned n){assert(n+4<=v.size());return uint32_t(v[n])<<24|uint32_t(v[n+1])<<16|uint32_t(v[n+2])<<8|v[n+3];};
 assert(read(0,8)==std::vector<uint8_t>({'G','8','M','E','0','1',0,0}));
 const auto dol=be(read(0x420,4),0);const auto header=read(dol,256);
 F f;
 for(auto span:{std::pair<uint32_t,unsigned>{0x803c4e84,60},{0x8041e558,8},{0x80429440,40}}){bool found=false;for(unsigned i=0;i<18;++i){const auto base=be(header,0x48+4*i),size=be(header,0x90+4*i);if(span.first>=base&&uint64_t(span.first)+span.second<=uint64_t(base)+size){assert(!found);found=true;const auto bytes=read(dol+be(header,4*i)+span.first-base,span.second);for(unsigned j=0;j<span.second;++j)f.m[span.first+j]=bytes[j];}}assert(found);}
 const auto bfn=read(948050384,66560);assert(be(bfn,0)==0x464f4e54&&bfn[77]==1);
 uint32_t block=32,gly=0;for(unsigned i=0;i<be(bfn,12);++i){if(be(bfn,block)==0x474c5931)gly=block;block+=be(bfn,block+4);}assert(gly==992);
 auto half=[&](unsigned a){assert(a+2<=bfn.size());return uint32_t(bfn[a])<<8|bfn[a+1];};
 auto q=init();q.integers[1]=0x89000000+gly+32;q.integers[2]=half(gly+26);q.integers[3]=half(gly+28);assert(q.integers[2]==512&&q.integers[3]==256);
 auto services=f.services();SourceGxTextureObjectCursor cursor;run(0x802b72b0,q,services,cursor);run(0x802b7544,lod(),services,cursor);
 assert(f.get(f.obj)==0x190&&f.get(f.obj+4)==0&&f.get(f.obj+8)==0x3fdff&&f.get(f.obj+12)==((q.integers[1]>>5)&0x1fffff)&&f.get(f.obj+20)==0&&f.get(f.obj+28,2)==2048&&f.get(f.obj+30,1)==1&&f.get(f.obj+31,1)==2);
 assert(bfn[77]==1); // SDK object initialization does not mutate source WID1.
}
void formatCases(){const unsigned kind[]={1,2,2,2,2,2,3,2,1,2,2,2,2,2,0,2};const unsigned count[]={2048,4096,4096,8192,8192,8192,8192,8192,2048,4096,8192,8192,8192,8192,2048,8192};for(unsigned k=0;k<16;++k){F f;auto s=f.services();SourceGxTextureObjectCursor c;run(0x802b72b0,init(k),s,c);assert(f.get(f.obj+30,1)==kind[k]);assert(f.get(f.obj+28,2)==count[k]);}F f;auto s=f.services();SourceGxTextureObjectCursor c;auto q=init(0x12345618,0x100);q.integers[2]=0x10000;q.integers[3]=0x20000;run(0x802b72b0,q,s,c);assert(f.get(f.obj+20)==q.integers[4]);assert(f.get(f.obj+8)==0x8fffff);assert(f.get(f.obj+28,2)==0);assert(f.get(f.obj+31,1)==2);}
void aliasAndCaptures(){
 // Mip captures object+4 before held fctiwz; later foreign mutation is overwritten.
 F f;auto s=f.services();SourceGxTextureObjectCursor c;auto q=init(0,1);f.holdTrunc=1;assert(!resumeSourceGxTextureObject(0x802b72b0,q,s,c));assert(c.pending()==0x802b73a0);f.put(f.obj+4,4,0xdeadbeef);run(0x802b72b0,q,s,c);assert(f.get(f.obj+4)==0x9000);assert(f.clearCalls==1);
 // LOD bias similarly captures word0. Min/max fctiwz have different load order.
 F g;auto t=g.services();SourceGxTextureObjectCursor d;auto l=lod();g.holdTrunc=1;assert(!resumeSourceGxTextureObject(0x802b7544,l,t,d));assert(d.pending()==0x802b7580);g.put(g.obj,4,0xdeadbeef);run(0x802b7544,l,t,d);assert((g.get(g.obj)&0xffc00000)==(0xa5a5a5a5u&0xffc00000));
 F h;auto u=h.services();SourceGxTextureObjectCursor e;auto v=u.truncate;u.truncate=[](void*p,float x)->std::optional<int32_t>{auto&z=*static_cast<F*>(p);++z.truncateCalls;if(z.truncateCalls==3)return {};if(z.truncateCalls==4)z.put(z.obj+4,4,0x12345678);return int32_t(x);};(void)v;assert(!resumeSourceGxTextureObject(0x802b7544,l,u,e));assert(e.pending()==0x802b7688);assert((h.get(h.obj+4)&255)==0);run(0x802b7544,l,u,e);assert(h.get(h.obj+4)==0x12340078);
 // Full uint32 minFilter index is not clamped to the six legal enum entries.
 F j;auto z=j.services();SourceGxTextureObjectCursor n;auto p=lod();p.integers[1]=0xfffffff0;j.m[0x8041e548]=7;run(0x802b7544,p,z,n);assert((j.get(j.obj)>>5&7)==7);
}
void missingAndLifetime(){F f;auto s=f.services();SourceGxTextureObjectCursor c;auto q=init(0,1);f.failClear=true;assert(!resumeSourceGxTextureObject(0x802b72b0,q,s,c));assert(c.pending()==0x800050b4&&f.get(f.obj)==0xa5a5a5a5);f.failClear=false;f.holdTrunc=2;assert(!resumeSourceGxTextureObject(0x802b72b0,q,s,c));auto replacement=s;replacement.lifetime=std::make_shared<int>(2);const auto trace=f.trace;assert(!resumeSourceGxTextureObject(0x802b72b0,q,replacement,c)&&trace==f.trace);auto changed=q;changed.integers[2]++;assert(!resumeSourceGxTextureObject(0x802b72b0,changed,s,c)&&trace==f.trace);f.alive=false;assert(!resumeSourceGxTextureObject(0x802b72b0,q,s,c)&&c.stale());f.alive=true;assert(!resumeSourceGxTextureObject(0x802b72b0,q,s,c));assert(c.reset());run(0x802b72b0,q,s,c);
 F g;auto t=g.services();SourceGxTextureObjectCursor d;auto p=init();g.put(0x803c4e84,4,0x1234);assert(!resumeSourceGxTextureObject(0x802b72b0,p,t,d));assert(d.pending()==0x1234);g.put(0x803c4e84,4,0x802b7430);assert(!resumeSourceGxTextureObject(0x802b72b0,p,t,d));
 F h;auto u=h.services();SourceGxTextureObjectCursor e;auto l=lod();l.reals[2]=bits(std::numeric_limits<float>::quiet_NaN());assert(!resumeSourceGxTextureObject(0x802b7544,l,u,e));assert(e.pending()==0x802b7580);
}
}
int main(){allBoundaries();originalFont();actualOriginalFont();formatCases();aliasAndCaptures();missingAndLifetime();std::cout<<"source GX texture object tests PASS\n";}

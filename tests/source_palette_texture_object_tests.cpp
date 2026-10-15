#include "source_palette_texture_object.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t bits(float x){uint32_t n;std::memcpy(&n,&x,4);return n;}
double real(uint64_t n){double x;std::memcpy(&x,&n,8);return x;}
uint32_t be(const std::vector<uint8_t>&v,unsigned a){assert(a+4<=v.size());return uint32_t(v[a])<<24|uint32_t(v[a+1])<<16|uint32_t(v[a+2])<<8|v[a+3];}
struct Original {
 std::map<uint32_t,uint8_t> bytes;
 Original(){
  std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);
  auto read=[&](uint32_t a,unsigned n){std::vector<uint8_t>v(n);iso.seekg(a);iso.read(reinterpret_cast<char*>(v.data()),n);assert(iso);return v;};
  assert(read(0,8)==std::vector<uint8_t>({'G','8','M','E','0','1',0,0}));
  const auto dol=be(read(0x420,4),0);const auto h=read(dol,256);
  for(auto span:{std::pair<uint32_t,unsigned>{0x803c4e84,60},{0x8041e558,8},{0x80429440,40},{0x802c0140,8}}){bool found=false;for(unsigned i=0;i<18;++i){const auto base=be(h,0x48+4*i),size=be(h,0x90+4*i);if(span.first>=base&&uint64_t(span.first)+span.second<=uint64_t(base)+size){assert(!found);found=true;const auto v=read(dol+be(h,4*i)+span.first-base,span.second);for(unsigned j=0;j<span.second;++j)bytes[span.first+j]=v[j];}}assert(found);}
  // CPU construction reads only headers; actual pixel pointers are relocated
  // faithfully but neither the source helper nor this test samples pixels.
  const auto tpl=read(942647408,217920);assert(be(tpl,0)==0x0020af30&&be(tpl,4)==97);
  const uint32_t base=0x88000000,table=be(tpl,8);
  auto put=[&](uint32_t a,uint32_t value){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(value>>(24-8*i));};
  put(base+8,base+table);
  for(unsigned index=0;index<4;++index){const auto header=be(tpl,table+8*index);put(base+table+8*index,base+header);for(unsigned j=0;j<36;++j)bytes[base+header+j]=tpl[header+j];put(base+header+8,base+be(tpl,header+8));}
  const auto window=read(947396944,321984);assert(be(window,0)==0x0020af30);const uint32_t wb=0x89000000,wt=be(window,8),wh=be(window,wt);put(wb+8,wb+wt);put(wb+wt,wb+wh);for(unsigned j=0;j<36;++j)bytes[wb+wh+j]=window[wh+j];put(wb+wh+8,wb+be(window,wh+8));
 }
};
const Original&original(){static Original v;return v;}
using Event=std::array<uint64_t,4>;
struct Fixture {
 std::map<uint32_t,uint8_t> memory=original().bytes;std::vector<Event>trace;
 std::shared_ptr<const void> lifetime=std::make_shared<int>(1);SourcePrivateTextureObject output;
 SourcePaletteTextureObjectCursor cursor;SourcePaletteTextureObjectServices s;
 uint32_t palette=0x88000000,object=0x90000000,index=2;bool alive=true,reenter=false,failClear=false,partialClear=false;
 size_t holdAt=size_t(-1);unsigned attempts=0,clears=0,clearPosition=0,clearStores=0;bool clearStarted=false;
 Fixture(){assert(output.bind(object,lifetime,this,[](void*p){return static_cast<Fixture*>(p)->alive;}));s.gx.user=this;s.gx.lifetime=lifetime;s.gx.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.gx.word=[](void*p,uint32_t a)->std::optional<uint32_t>{return static_cast<Fixture*>(p)->read(a,4,1);};
  s.gx.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{const auto v=static_cast<Fixture*>(p)->read(a,1,2);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
  s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{const auto v=static_cast<Fixture*>(p)->read(a,2,3);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;};
  s.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&f=*static_cast<Fixture*>(p);if(!f.ready())return {};const auto hi=f.get(a,4),lo=f.get(a+4,4);if(!hi||!lo)return {};const auto n=uint64_t(*hi)<<32|*lo;f.trace.push_back({4,a,8,n});return n;};
  s.subtractSingle=[](void*p,uint64_t a,uint64_t b)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.ready())return {};const auto x=real(a),y=real(b);if(!std::isfinite(x)||!std::isfinite(y))return {};volatile float v=float(x-y);f.trace.push_back({5,a,b,bits(v)});return bits(v);};
  s.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t value){auto&f=*static_cast<Fixture*>(p);if(!f.ready())return false;f.trace.push_back({6,a,n,value});return f.put(a,n,value);};
  s.gx.truncate=[](void*p,float x)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.ready()||!std::isfinite(x)||double(x)<-2147483648.0||double(x)>=2147483648.0)return {};f.trace.push_back({7,bits(x),0,0});return int32_t(x);};
  s.gx.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.failClear||!f.ready())return {};if(f.reenter){assert(!f.cursor.reset());assert(!f.resume());}if(!f.clearStarted){f.clearStarted=true;++f.clears;f.trace.push_back({8,a,32,0});}while(f.clearPosition<32){assert(f.put(a+f.clearPosition,1,0));++f.clearPosition;++f.clearStores;if(f.partialClear&&f.clearPosition==16)return {};}return 0;};
 }
 bool ready(){return trace.size()!=holdAt||attempts++>=2;}
 std::optional<uint32_t>get(uint32_t a,unsigned n){if(output.overlaps(a,n))return output.read(a,n);uint32_t v=0;for(unsigned i=0;i<n;++i){const auto it=memory.find(a+i);if(it==memory.end())return {};v=(v<<8)|it->second;}return v;}
 bool put(uint32_t a,unsigned n,uint32_t value){if(output.overlaps(a,n))return output.store(a,n,value);for(unsigned i=0;i<n;++i)memory[a+i]=uint8_t(value>>(8*(n-i-1)));return true;}
 std::optional<uint32_t>read(uint32_t a,unsigned n,unsigned kind){if(!ready())return {};const auto value=get(a,n);if(value)trace.push_back({kind,a,n,*value});return value;}
 bool resume(){return resumeSourcePaletteTextureObject(palette,object,index,s,cursor);}
 void run(){for(unsigned i=0;i<1000;++i)if(resume()){assert(!cursor.active());return;}assert(false);}
 uint32_t word(uint32_t a){const auto v=get(a,4);assert(v);return *v;}
 uint32_t header(){return word(word(palette+8)+(index<<3));}
};
// Independent immediate parent translation. SDK children are the already
// separately verified complete original bodies, not mocks or whole-call acks.
void reference(Fixture&f){
 const auto&s=f.s;auto w=[&](uint32_t a){auto v=s.gx.word(s.gx.user,a);assert(v);return *v;};auto b=[&](uint32_t a){auto v=s.gx.byte(s.gx.user,a);assert(v);return uint32_t(*v);};auto h=[&](uint32_t a){auto v=s.half(s.gx.user,a);assert(v);return uint32_t(*v);};
 const auto table=w(f.palette+8),offset=f.index<<3;auto header=w(table+offset);const auto min=b(header+33),max=b(header+34);
 SourceGxTextureObjectInput init;init.integers[0]=f.object;init.integers[7]=min!=max;init.integers[1]=w(header+8);init.integers[2]=h(header+2);init.integers[3]=h(header);init.integers[4]=w(header+4);init.integers[5]=w(header+12);init.integers[6]=w(header+16);SourceGxTextureObjectCursor child;assert(resumeSourceGxTextureObject(0x802b72b0,init,s.gx,child));
 header=w(table+offset);const auto minimum=b(header+33),maximum=b(header+34);const auto conversion=s.doubleWord(s.gx.user,0x802c0140);assert(conversion);SourceGxTextureObjectInput lod;lod.integers[0]=f.object;const auto first=s.subtractSingle(s.gx.user,0x4330000000000000ULL|minimum,*conversion);assert(first);lod.reals[0]=*first;lod.integers[1]=w(header+20);lod.integers[2]=w(header+24);lod.reals[2]=w(header+28);const auto second=s.subtractSingle(s.gx.user,0x4330000000000000ULL|maximum,*conversion);assert(second);lod.reals[1]=*second;lod.integers[4]=b(header+32);assert(resumeSourceGxTextureObject(0x802b7544,lod,s.gx,child));
}
void differential(){for(unsigned index=0;index<5;++index){Fixture baseline;baseline.index=index==4?0:index;if(index==4)baseline.palette=0x89000000;reference(baseline);const auto trace=baseline.trace;const auto words=baseline.output.words();assert(words);
 for(size_t at=0;at<trace.size();++at){Fixture held;held.index=baseline.index;held.palette=baseline.palette;held.holdAt=at;held.reenter=true;held.run();assert(held.attempts==3&&held.trace==trace&&held.output.words()==words&&held.clears==1&&held.clearStores==32);}
 Fixture direct;direct.index=baseline.index;direct.palette=baseline.palette;direct.run();assert(direct.trace==trace&&direct.output.words()==words);}
}
void lateReads(){
 Fixture f;const auto originalHeader=f.header(),table=f.word(f.palette+8);f.failClear=true;assert(!f.resume()&&f.cursor.pending()==0x802b72b0&&f.cursor.childPending()==0x800050b4);assert(!f.output.words());
 // Init operands remain captured; LOD follows the freshly reloaded header in
 // the captured table, ignoring the independently changed palette table word.
 const uint32_t other=0x89100000;for(unsigned i=0;i<36;++i)f.memory[other+i]=f.memory.at(originalHeader+i);f.put(other+20,4,2);f.put(other+33,1,255);f.put(other+34,1,128);f.put(table+(f.index<<3),4,other);f.put(f.palette+8,4,0xdead0000);f.failClear=false;f.run();const auto words=f.output.words();assert(words&&(((*words)[2]&1023)+1)==16&&(((*words)[0]>>5)&7)==1);unsigned conversions=0;for(const auto&e:f.trace)if(e[0]==5){assert(e[1]==(0x4330000000000000ULL|(conversions?128:255)));assert(e[2]==0x4330000000000000ULL);++conversions;}assert(conversions==2&&f.clears==1);
 Fixture partial;partial.partialClear=true;assert(!partial.resume()&&partial.clearStores==16);assert(!partial.output.words());partial.run();assert(partial.clearStores==32&&partial.clears==1);
 Fixture numeric;numeric.put(numeric.header()+33,1,200);numeric.put(numeric.header()+34,1,250);Fixture immediate;immediate.put(immediate.header()+33,1,200);immediate.put(immediate.header()+34,1,250);immediate.run();size_t at=0;while(immediate.trace[at][0]!=5)++at;numeric.holdAt=at;assert(!numeric.resume()&&numeric.cursor.pending()==0x80037b1c);numeric.put(0x802c0140,4,0);numeric.put(0x802c0144,4,0);numeric.put(numeric.header()+33,1,1);numeric.run();for(const auto&e:numeric.trace)if(e[0]==5)assert(e[2]==0x4330000000000000ULL);
}
void missingAndIdentity(){
 Fixture f;f.s.gx.clear=nullptr;assert(!f.resume()&&f.cursor.pending()==0x802b72b0);auto changed=f.s;changed.half=nullptr;const auto trace=f.trace;assert(!resumeSourcePaletteTextureObject(f.palette,f.object,f.index,changed,f.cursor)&&trace==f.trace);assert(!resumeSourcePaletteTextureObject(f.palette,f.object+32,f.index,f.s,f.cursor)&&trace==f.trace);changed=f.s;changed.gx.lifetime=std::make_shared<int>(3);assert(!resumeSourcePaletteTextureObject(f.palette,f.object,f.index,changed,f.cursor));f.alive=false;assert(!f.resume()&&f.cursor.stale());f.alive=true;assert(!f.resume());
 Fixture missing;missing.s.doubleWord=nullptr;assert(!missing.resume()&&missing.cursor.pending()==0x80037b0c);Fixture numeric;numeric.s.subtractSingle=nullptr;assert(!numeric.resume()&&numeric.cursor.pending()==0x80037b1c);Fixture memory;memory.memory.erase(memory.header()+2);assert(!memory.resume()&&memory.cursor.pending()==0x80037acc);Fixture unknown;unknown.index=0x20000002;unknown.run(); // low29 index-offset wrap is source slwi, not a bounds check
}
void privateDomain(){
 auto life=std::make_shared<int>(4);SourcePrivateTextureObject p;bool live=true;assert(!p.bind(0xfffffff0,life,&live,[](void*q){return *static_cast<bool*>(q);}));assert(p.bind(0xffffffe0,life,&live,[](void*q){return *static_cast<bool*>(q);}));assert(!p.bind(0,life,&live,[](void*){return true;}));assert(p.owns(0xffffffe0,32)&&!p.read(0xffffffe0,4)&&!p.words());assert(p.overlaps(0xfffffffe,4)&&!p.owns(0xfffffffe,4));assert(!p.store(0xfffffffe,4,0));assert(!p.store(0xffffffe0,3,0));assert(p.store(0xffffffe1,2,0x1234));assert(p.read(0xffffffe1,2)==0x1234&&!p.read(0xffffffe0,4));for(unsigned i=0;i<8;++i)assert(p.store(0xffffffe0+4*i,4,0x01020304+i));assert(p.words()&&p.read(0xffffffe0,1)==1);live=false;assert(!p.words());live=true;assert(!p.words());
 // Actual source output can alias a descriptor-table slot. The initial slot
 // is already known, then real Init clear destroys it; the fresh reload sees the newly constructed sampler word.
 Fixture alias;const auto header=alias.header();alias.object=alias.word(alias.palette+8)+(alias.index<<3);for(unsigned i=0;i<32;++i)alias.memory[alias.object+i]=0;alias.put(alias.object,4,header);assert(!alias.resume()&&alias.cursor.pending()==0x80037af4&&alias.word(alias.object)==0x90);
 SourcePrivateTextureObject recursive;struct Context{SourcePrivateTextureObject*p;bool active=false;}context{&recursive};assert(recursive.bind(0x1000,life,&context,[](void*q){auto&c=*static_cast<Context*>(q);if(c.active){assert(!c.p->store(0x1000,1,0));assert(!c.p->read(0x1000,1));}return true;}));context.active=true;assert(recursive.store(0x1000,1,7)&&recursive.read(0x1000,1)==7);recursive.retire();assert(recursive.owns(0x1000,32)&&!recursive.read(0x1000,1));
}
}
int main(){differential();lateReads();missingAndIdentity();privateDomain();std::cout<<"source palette texture object tests PASS\n";return 0;}

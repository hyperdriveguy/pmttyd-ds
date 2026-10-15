#include <memory>
#include "source_scalar_font.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <map>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
uint32_t be32(const std::vector<uint8_t>&b,size_t at){assert(at+4<=b.size());return uint32_t(b[at])<<24|uint32_t(b[at+1])<<16|uint32_t(b[at+2])<<8|b[at+3];}
uint16_t be16(const std::vector<uint8_t>&b,size_t at){assert(at+2<=b.size());return uint16_t(uint16_t(b[at])<<8|b[at+1]);}
struct Event {uint32_t fn;std::vector<uint32_t>a;bool operator==(const Event&o)const{return fn==o.fn&&a==o.a;}};
struct Fixture {
 static constexpr uint32_t Bfn=0x30000,Cam=0x10000,Gp=0x20000;
 std::vector<uint8_t>bfn;MessageFont font;std::map<uint32_t,uint32_t>words;std::map<uint32_t,uint8_t>bytes;
 std::vector<std::optional<uint8_t>>textBytes;std::shared_ptr<const void>owner=std::make_shared<int>(7);
 SourceMatrixFontText text;SourceScalarFontCursor cursor;float x=3,y=4;
 std::vector<Event>trace,ops;std::optional<Event>pending;std::vector<SourceMatrixFontGlyph>glyphs;std::vector<BattleActMatrix>loaded;std::vector<WindowFontQuad>quads;
 std::function<void(Fixture&,const Event&)>onOp;int hold=-1;unsigned failures=0,randoms=0,emissions=0,halfReads=0,lastHalf=0;bool alive=true,probe=false,admit=true,atomicGlyph=true,atomicInvoking=false,nullZero=false;uint32_t tev=0x123456ff;
 Fixture(){assert(font.load("romfs/font.pmfont"));std::ifstream metadata("romfs/font.json");std::string json((std::istreambuf_iterator<char>(metadata)),{});auto at=json.find("source_offset");assert(at!=std::string::npos);at=json.find(':',at)+1;auto offset=std::stoull(json.substr(at));
  std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);iso.seekg(std::streamoff(offset));bfn.resize(32);iso.read(reinterpret_cast<char*>(bfn.data()),32);assert(iso&&be32(bfn,0)==0x464f4e54);auto size=be32(bfn,8);assert(size>=32&&size<1000000);bfn.resize(size);iso.seekg(std::streamoff(offset));iso.read(reinterpret_cast<char*>(bfn.data()),size);assert(iso);
  uint32_t block=32,info=0,gly=0;for(unsigned i=0;i<be32(bfn,12);++i){auto tag=be32(bfn,block);if(tag==0x494e4631)info=block;if(tag==0x474c5931)gly=block;block+=be32(bfn,block+4);}assert(info&&gly);
  words={{0x80415fd0,Gp},{Gp+0x16c,1},{0x8041e978,Bfn},{0x8041e9a0,Bfn+info},{0x8041e99c,Bfn+gly},{0x8041e9a4,0},{0x8041e9ac,0x123456ff},{0x80416010,0x1e1414ff},{0x80416020,2},
   {0x802c2b78,0},{0x802c2b7c,0},{0x802c2b80,0},{0x80420430,bits(-1)},{0x804203f8,0},{0x8042041c,bits(.5f)},{0x802c2b90,0x43300000},{0x802c2b94,0},{0x802c2b98,0x43300000},{0x802c2b9c,0x80000000},
   {0x8041e9b0,bits(.8f)},{0x8041e9b4,bits(.8f)},{0x8041e9b8,bits(1)}};
  bytes[0x8041e9a8]=255;for(unsigned i=0;i<12;++i)words[Cam+0x11c+4*i]=bits(i==0||i==5||i==10?1.f:0.f);
  text.user=this;text.lifetime=owner;text.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0xf0000002,{a}))return {};if(a>=f.textBytes.size())return {};return f.textBytes[a];};
  text.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0xf0000003,{a}))return {};if(uint64_t(a)+1>=f.textBytes.size()||!f.textBytes[a]||!f.textBytes[a+1])return {};++f.halfReads;f.lastHalf=uint16_t(uint16_t(*f.textBytes[a])<<8|*f.textBytes[a+1]);return uint16_t(f.lastHalf);};setText("AB");
 }
 void setText(const std::string&s){textBytes.clear();for(uint8_t b:s)textBytes.push_back(b);textBytes.push_back(uint8_t(0));}
 uint32_t word(uint32_t a){uint32_t v;if(a>=Bfn&&uint64_t(a)+4<=uint64_t(Bfn)+bfn.size())v=be32(bfn,a-Bfn);else{auto i=words.find(a);assert(i!=words.end());v=i->second;}trace.push_back({1,{a,v}});return v;}
 uint16_t half(uint32_t a){assert(a>=Bfn&&uint64_t(a)+2<=uint64_t(Bfn)+bfn.size());auto v=be16(bfn,a-Bfn);trace.push_back({2,{a,v}});return v;}
 uint8_t byte(uint32_t a){uint8_t v;if(a>=Bfn&&uint64_t(a)<uint64_t(Bfn)+bfn.size())v=bfn[a-Bfn];else v=bytes.at(a);trace.push_back({3,{a,v}});return v;}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.byte=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->byte(at);};a.memory.user=this;a.memory.word=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->word(at);};a.memory.half=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->half(at);};a.memory.storeWord=[](void*p,uint32_t at,uint32_t v){auto&f=*static_cast<Fixture*>(p);f.words[at]=v;f.trace.push_back({5,{at,v}});};return a;}
 bool operation(uint32_t fn,std::vector<uint32_t>a){Event event{fn,std::move(a)};if(!pending){pending=event;trace.push_back(event);ops.push_back(event);if(onOp)onOp(*this,event);}else assert(*pending==event);if(probe){assert(!cursor.reset());assert(!run());}if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 static void append(std::vector<uint32_t>&out,const BattleActMatrix&m){for(auto f:m)out.push_back(bits(f));}
 SourceMatrixFontAtomicGlyphServices atomic(){SourceMatrixFontAtomicGlyphServices a;a.invoking=&atomicInvoking;a.font=&font;a.actors=actors();a.user=this;a.lifetime=owner;a.current=[](void*p){return static_cast<Fixture*>(p)->alive;};a.color=[](void*p)->std::optional<uint32_t>{return static_cast<Fixture*>(p)->tev;};a.admit=[](void*p,const WindowFontQuad&){return static_cast<Fixture*>(p)->admit;};a.random=[](void*p,uint32_t bound){assert(bound==64);return ++static_cast<Fixture*>(p)->randoms%64;};a.emit=[](void*p,const WindowFontQuad&q){auto&f=*static_cast<Fixture*>(p);++f.emissions;f.quads.push_back(q);};return a;}
 SourceScalarFontServices services(){SourceScalarFontServices out;auto&s=out.common;s.actors=actors();s.user=this;s.lifetime=owner;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.nullByte=[](void*p,uint32_t at)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);assert(at==0);if(!f.nullZero)return {};if(!f.operation(0xf0000004,{at}))return {};return 0;};s.camera=[](void*p)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x8000f2e4,{}))return {};return Cam;};
  s.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&f=*static_cast<Fixture*>(p);std::vector<uint32_t>a;append(a,q.left);append(a,q.right);for(auto v:q.xyz)a.push_back(bits(v));if(!f.operation(q.function,a))return false;BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);return math.matrix(nullptr,q);};
  s.loadMatrix=[](void*p,const BattleActMatrix&m,uint32_t index){auto&f=*static_cast<Fixture*>(p);std::vector<uint32_t>a{index};append(a,m);if(!f.operation(0x802ba2fc,a))return false;f.loaded.push_back(m);return true;};s.tevColor=[](void*p,uint32_t index,uint32_t rgba){auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x802b8ab8,{index,rgba}))return false;f.tev=rgba;return true;};
  s.glyph=[](void*p,const SourceMatrixFontGlyph&q){auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x80074720,{q.glyph,q.sourceCode,uint32_t(q.x),uint32_t(q.y)}))return false;if(f.atomicGlyph&&!drawSourceMatrixFontAtomicGlyph(q,f.atomic()))return false;f.glyphs.push_back(q);return true;};
  out.fma=[](void*p,float a,float b,float c)->std::optional<float>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0xf0000001,{bits(a),bits(b),bits(c)}))return {};return std::fma(a,b,c);};return out;
 }
 bool run(){return resumeSourceScalarFont(x,y,text,services(),cursor);}
};
using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup={}){Fixture base;if(setup)setup(base);assert(base.run());for(unsigned gate=0;gate<base.ops.size();++gate){Fixture f;if(setup)setup(f);f.hold=int(gate);f.probe=true;assert(!f.run());auto trace=f.trace;auto memory=f.words;auto s=f.services();s.common.glyph=nullptr;assert(!resumeSourceScalarFont(f.x,f.y,f.text,s,f.cursor));auto copy=f.text;assert(!resumeSourceScalarFont(f.x,f.y,copy,f.services(),f.cursor));assert(!f.run()&&f.trace==trace&&f.words==memory);assert(f.run()&&f.trace==base.trace&&f.words==base.words&&f.emissions==base.emissions&&f.randoms==base.randoms);}}
}
int main(){
 retained();retained([](auto&f){f.words[0x8041e9a4]=1;});retained([](auto&f){f.words[0x80420430]=bits(20);});retained([](auto&f){f.setText("A<red x y>B");});retained([](auto&f){f.setText("");});
 {Fixture f;assert(f.run()&&f.glyphs.size()==2&&f.randoms==4&&f.loaded.size()==2);assert(f.loaded[0][3]==3&&f.loaded[0][7]==4&&f.loaded[0][0]==.8f);auto width=f.font.glyphs[f.font.mapping['A']].width;assert(f.loaded[1][3]==std::fma(.8f,float(width+2),3.f));}
 {Fixture f;f.words[0x80420430]=bits(20);assert(f.run());auto width=f.font.glyphs[f.font.mapping['A']].width;assert(f.loaded[0][3]==std::fma(.8f,.5f*(20-float(width)),3.f));assert(f.loaded[1][3]==std::fma(.8f,.5f*(20-float(f.font.glyphs[f.font.mapping['B']].width)),std::fma(20.f,.8f,3.f)));}
 {Fixture f;f.setText("A");f.words[0x8041e9a4]=1;assert(f.run()&&f.glyphs.size()==5&&f.randoms==10&&f.words[0x8041e9ac]==0x123456ff);assert(f.glyphs[0].x==-2&&f.glyphs[1].x==2&&f.glyphs[2].y==-2&&f.glyphs[3].y==2&&f.glyphs[4].x==0);}
 // Scalar tags skip all commands; unlike matrix font no R/newline exception.
 for(const char*text:{"A<foo>B","A<R>B","A</r>B","A<red x y>B","A<>B"}){Fixture f;f.setText(text);assert(f.run()&&f.glyphs.size()==2&&f.glyphs[1].sourceCode=='B');}
 {Fixture f;f.textBytes={uint8_t('A'),std::nullopt};assert(!f.run()&&f.glyphs.size()==1&&f.cursor.pending()==0x80077338);f.textBytes[1]=0;assert(f.run());}
 {Fixture f;f.setText("<"+std::string(128,'x'));assert(!f.run()&&f.cursor.pending()==0x80077338&&f.glyphs.empty());f.nullZero=true;assert(f.run()&&f.glyphs.empty());}
 // Captured scale matrix, fresh pen scale and fresh per-glyph camera view.
 {Fixture f;bool first=true;f.onOp=[&](auto&v,const Event&e){if(e.fn==0x80074720&&first){first=false;v.words[0x8041e9b0]=bits(1.5f);v.words[Fixture::Cam+0x128]=bits(7);}};assert(f.run());auto width=f.font.glyphs[f.font.mapping['A']].width;assert(f.loaded[0][0]==.8f&&f.loaded[1][0]==.8f&&f.loaded[1][3]==std::fma(1.5f,float(width+2),3.f)+7);unsigned cameras=0;for(auto&e:f.ops)cameras+=e.fn==0x8000f2e4;assert(cameras==1);}
 // Restore captured color with freshly reloaded alpha after four edge children.
 {Fixture f;f.setText("A");f.words[0x8041e9a4]=1;f.onOp=[](auto&v,const Event&e){if(e.fn==0x80074720&&e.a[2]==0&&e.a[3]==2){v.bytes[0x8041e9a8]=128;v.words[0x8041e9ac]=0;}};assert(f.run()&&f.words[0x8041e9ac]==0x123456ff&&f.tev==0x12345680);}
 // Actual single LHZ after language-sensitive lead classification; no two-byte substitute.
 {Fixture f;f.words[Fixture::Gp+0x16c]=0;f.words[0x8041601c]=2;f.atomicGlyph=false;f.setText(std::string("\x81\x40",2));assert(f.run()&&f.halfReads==1&&f.lastHalf==0x8140);for(auto&e:f.ops)if(e.fn==0xf0000002)assert(e.a[0]!=1);}
 {Fixture base;base.words[Fixture::Gp+0x16c]=0;base.words[0x8041601c]=2;base.atomicGlyph=false;base.setText(std::string("\x81\x40",2));assert(base.run());int at=-1;for(unsigned i=0;i<base.ops.size();++i)if(base.ops[i].fn==0xf0000003)at=int(i);assert(at>=0);Fixture f;f.words[Fixture::Gp+0x16c]=0;f.words[0x8041601c]=2;f.atomicGlyph=false;f.setText(std::string("\x81\x40",2));f.hold=at;assert(!f.run()&&f.halfReads==0);f.textBytes[1]=0x41;assert(!f.run()&&f.halfReads==0);assert(f.run()&&f.halfReads==1&&f.lastHalf==0x8141);}
 {Fixture f;f.words[Fixture::Gp+0x16c]=0;f.words[0x8041601c]=2;f.atomicGlyph=false;f.setText(std::string("\x81\x40",2));f.text.half=nullptr;assert(!f.run()&&f.cursor.pending()==0x800770a0&&f.halfReads==0&&f.glyphs.empty());}
 // A plain newline uses source mapping; scalar has no matrix newline advance.
 {Fixture f;f.atomicGlyph=false;f.setText("A\nB");assert(f.run());for(const auto&m:f.loaded)assert(m[7]==4);}
 {Fixture f;f.admit=false;assert(!f.run()&&f.randoms==0&&f.emissions==0);f.admit=true;assert(f.run()&&f.emissions==2);}
 {Fixture f;f.hold=0;assert(!f.run());f.alive=false;assert(!f.run()&&f.cursor.stale());auto trace=f.trace;f.alive=true;assert(!f.run()&&f.trace==trace);}
}

#include <memory>
#include "source_string_font.hpp"
#include "battle_audience_aux_callback.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
using namespace ttyd;
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
uint32_t be32(const std::vector<uint8_t>&b,size_t a){assert(a+4<=b.size());return uint32_t(b[a])<<24|uint32_t(b[a+1])<<16|uint32_t(b[a+2])<<8|b[a+3];}
uint16_t be16(const std::vector<uint8_t>&b,size_t a){assert(a+2<=b.size());return uint16_t(uint16_t(b[a])<<8|b[a+1]);}
struct Event{uint32_t fn;std::vector<uint32_t>a;bool operator==(const Event&o)const{return fn==o.fn&&a==o.a;}};
struct Fixture{
 static constexpr uint32_t Cam=0x10000,Gp=0x20000,Bfn=0x30000;
 std::vector<uint8_t>bfn;MessageFont font;EventScriptAssets originals;
 std::map<uint32_t,uint32_t>words;std::map<uint32_t,uint8_t>bytes;
 std::vector<std::optional<uint8_t>>textBytes;std::shared_ptr<const void>owner=std::make_shared<int>(7);
 SourceMatrixFontText text;SourceStringFontCursor cursor;BattleActMatrix base{1,0,0,3,0,1,0,4,0,0,1,0};
 std::vector<Event>trace,ops;std::optional<Event>pending;std::vector<WindowFontQuad>quads;std::vector<SourceMatrixFontGlyph>glyphs;
 std::function<void(Fixture&,const Event&)>onOp;int hold=-1,textHold=-1;unsigned failures=0,textFailures=0,randoms=0,emissions=0;bool alive=true,probe=false,admit=true,atomicRunning=false;uint32_t tev=0x123456ff;
 Fixture(){assert(font.load("romfs/font.pmfont")&&originals.load("romfs/trio-attack-events.pmevt"));
  std::ifstream meta("romfs/font.json");std::string json((std::istreambuf_iterator<char>(meta)),{});auto at=json.find("source_offset");assert(at!=std::string::npos);at=json.find(':',at)+1;const auto offset=std::stoull(json.substr(at));
  std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);iso.seekg(std::streamoff(offset));bfn.resize(32);iso.read(reinterpret_cast<char*>(bfn.data()),32);assert(iso&&bfn[0]=='F'&&bfn[1]=='O');const auto size=be32(bfn,8);assert(size>=32&&size<1000000);bfn.resize(size);iso.seekg(std::streamoff(offset));iso.read(reinterpret_cast<char*>(bfn.data()),size);assert(iso);
  uint32_t block=32,info=0,gly=0;for(unsigned i=0;i<be32(bfn,12);++i){const auto tag=be32(bfn,block);if(tag==0x494e4631)info=block;if(tag==0x474c5931)gly=block;if(tag==0x57494431&&be16(bfn,block+8)==0){bfn[block+13]=7;bfn[block+13]=9;}block+=be32(bfn,block+4);}assert(info&&gly);
  words={{0x80415fd0,Gp},{Gp+0x16c,1},{0x8041e978,Bfn},{0x8041e9a0,Bfn+info},{0x8041e99c,Bfn+gly},{0x8041e9a4,0},{0x8041e9ac,0x123456ff}};bytes[0x8041e9a8]=255;for(unsigned i=0;i<12;++i)words[Cam+0x11c+4*i]=bits(i==0||i==5||i==10?1.f:0.f);
  text.user=this;text.lifetime=owner;text.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto&f=*static_cast<Fixture*>(p);if(int(a)==f.textHold&&f.textFailures++<2)return {};if(a>=f.textBytes.size()||!f.textBytes[a])return {};const auto v=*f.textBytes[a];f.trace.push_back({4,{a,v}});return v;};text.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&f=*static_cast<Fixture*>(p);if(uint64_t(a)+1>=f.textBytes.size()||!f.textBytes[a]||!f.textBytes[a+1])return {};const auto v=uint16_t(uint16_t(*f.textBytes[a])<<8|*f.textBytes[a+1]);f.trace.push_back({6,{a,v}});return v;};setText("AB");
 }
 void setText(const std::string&s){textBytes.clear();for(uint8_t b:s)textBytes.push_back(b);textBytes.push_back(uint8_t(0));}
 uint32_t word(uint32_t a){uint32_t v;if(a>=Bfn&&uint64_t(a)+4<=uint64_t(Bfn)+bfn.size())v=be32(bfn,a-Bfn);else{const auto i=words.find(a);if(i!=words.end())v=i->second;else assert(originals.readWord(a,v));}trace.push_back({1,{a,v}});return v;}
 uint16_t half(uint32_t a){assert(a>=Bfn&&uint64_t(a)+2<=uint64_t(Bfn)+bfn.size());const auto v=be16(bfn,a-Bfn);trace.push_back({2,{a,v}});return v;}
 uint8_t byte(uint32_t a){uint8_t v;if(a>=Bfn&&uint64_t(a)<uint64_t(Bfn)+bfn.size())v=bfn[a-Bfn];else v=bytes.at(a);trace.push_back({3,{a,v}});return v;}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.byte=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->byte(at);};a.memory.user=this;a.memory.word=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->word(at);};a.memory.half=[](void*p,uint32_t at){return static_cast<Fixture*>(p)->half(at);};a.memory.storeWord=[](void*p,uint32_t at,uint32_t v){auto&f=*static_cast<Fixture*>(p);f.words[at]=v;f.trace.push_back({5,{at,v}});};return a;}
 bool operation(uint32_t fn,std::vector<uint32_t>a){Event op{fn,std::move(a)};if(!pending){pending=op;trace.push_back(op);ops.push_back(op);if(onOp)onOp(*this,op);}else assert(*pending==op);if(probe){assert(!cursor.reset());assert(!run());}if(int(ops.size()-1)==hold&&failures++<2)return false;pending.reset();return true;}
 static void append(std::vector<uint32_t>&a,const BattleActMatrix&m){for(float v:m)a.push_back(bits(v));}
 SourceMatrixFontAtomicGlyphServices atomic(){SourceMatrixFontAtomicGlyphServices a;a.font=&font;a.actors=actors();a.invoking=&atomicRunning;a.user=this;a.lifetime=owner;a.current=[](void*p){return static_cast<Fixture*>(p)->alive;};a.color=[](void*p)->std::optional<uint32_t>{return static_cast<Fixture*>(p)->tev;};a.admit=[](void*p,const WindowFontQuad&){return static_cast<Fixture*>(p)->admit;};a.random=[](void*p,uint32_t bound){auto&f=*static_cast<Fixture*>(p);assert(bound==64);++f.randoms;return uint32_t(f.randoms%64);};a.emit=[](void*p,const WindowFontQuad&q){auto&f=*static_cast<Fixture*>(p);++f.emissions;f.quads.push_back(q);};return a;}
 SourceMatrixFontServices commonServices(){SourceMatrixFontServices s;s.actors=actors();s.user=this;s.lifetime=owner;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.camera=[](void*p)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x8000f2e4,{}))return {};return Cam;};s.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&f=*static_cast<Fixture*>(p);std::vector<uint32_t>a;append(a,q.left);append(a,q.right);if(!f.operation(q.function,a))return false;BattleAudienceAuxServices m;bindAudienceAuxMathAdapter(m);return m.matrix(nullptr,q);};s.loadMatrix=[](void*p,const BattleActMatrix&m,uint32_t index){auto&f=*static_cast<Fixture*>(p);std::vector<uint32_t>a{index};append(a,m);return f.operation(0x802ba2fc,a);};s.tevColor=[](void*p,uint32_t i,uint32_t v){auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x802b8ab8,{i,v}))return false;f.tev=v;return true;};s.truncate=[](void*p,float v)->std::optional<int32_t>{auto&f=*static_cast<Fixture*>(p);if(!f.operation(0xf0000001,{bits(v)}))return {};if(!std::isfinite(v)||double(v)<-2147483648.0||double(v)>=2147483648.0)return {};return int32_t(v);};s.glyph=[](void*p,const SourceMatrixFontGlyph&q){auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x80074720,{q.glyph,q.sourceCode,uint32_t(q.x),uint32_t(q.y)}))return false;f.glyphs.push_back(q);return drawSourceMatrixFontAtomicGlyph(q,f.atomic());};return s;}
 uint32_t width=17;
 SourceStringFontServices services(){SourceStringFontServices s;s.common=commonServices();s.width=[](void*p,const SourceMatrixFontText&t,uint32_t line)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);assert(t.user==f.text.user&&t.lifetime==f.owner&&line==0);if(!f.operation(0x8007554c,{line}))return {};return f.width;};return s;}
 bool run(){return resumeSourceStringFont(base,text,services(),cursor);}
};

using Setup=std::function<void(Fixture&)>;
void retained(const Setup&setup={}){Fixture expected;if(setup)setup(expected);assert(expected.run());for(unsigned gate=0;gate<expected.ops.size();++gate){Fixture f;if(setup)setup(f);f.hold=int(gate);f.probe=true;assert(!f.run());auto trace=f.trace;auto memory=f.words;auto s=f.services();s.width=nullptr;assert(!resumeSourceStringFont(f.base,f.text,s,f.cursor));s=f.services();s.common.actors.audienceLifecycle.user=&expected;assert(!resumeSourceStringFont(f.base,f.text,s,f.cursor));auto copied=f.text;assert(!resumeSourceStringFont(f.base,copied,f.services(),f.cursor));auto copiedBase=f.base;assert(!resumeSourceStringFont(copiedBase,f.text,f.services(),f.cursor));assert(!f.run()&&f.trace==trace&&f.words==memory);assert(f.run()&&f.trace==expected.trace&&f.words==expected.words&&f.randoms==expected.randoms&&f.emissions==expected.emissions);}}
int main(){
 retained();retained([](auto&f){f.words[0x8041e9a4]=1;});retained([](auto&f){f.setText("");});
 Fixture centered;assert(centered.run()&&centered.glyphs.size()==2&&centered.glyphs[0].x==-8);auto width=centered.font.glyphs[centered.font.mapping['A']].width;assert(centered.glyphs[1].x==int32_t(-8.5f+width+2));assert(centered.randoms==4);
 Fixture wrapped;wrapped.width=0x10001;assert(wrapped.run()&&wrapped.glyphs[0].x==0); // Source consumes only low16 width bits.
 Fixture edge;edge.width=65535;edge.setText("A");edge.words[0x8041e9a4]=1;assert(edge.run()&&edge.glyphs[0].x==32767&&edge.glyphs[1].x==-32765&&edge.glyphs[4].x==-32767);
 Fixture outlined;outlined.setText("A");outlined.words[0x8041e9a4]=1;assert(outlined.run()&&outlined.glyphs.size()==5&&outlined.randoms==10);assert(outlined.glyphs[0].x==-10&&outlined.glyphs[1].x==-6&&outlined.glyphs[2].x==-8&&outlined.glyphs[2].y==-2&&outlined.glyphs[3].y==2&&outlined.glyphs[4].x==-8);assert(outlined.words[0x8041e9ac]==0x123456ff);
 Fixture unknown;unknown.textBytes={uint8_t('A'),std::nullopt};assert(!unknown.run()&&unknown.emissions==1);auto trace=unknown.trace;assert(!unknown.run()&&unknown.trace==trace);unknown.textBytes[1]=uint8_t(0);assert(unknown.run()&&unknown.emissions==1);
 Fixture widthHold;auto services=widthHold.services();services.width=nullptr;assert(!resumeSourceStringFont(widthHold.base,widthHold.text,services,widthHold.cursor)&&widthHold.cursor.pending()==0x8007554c&&widthHold.emissions==0);assert(!widthHold.run()); // Service identity cannot change under a held invocation.
 Fixture late;late.onOp=[](auto&f,const Event&e){if(e.fn==0x8007554c)f.base[3]=11;};assert(late.run());for(const auto&e:late.ops)if(e.fn==0x8029dd64)assert(e.a[15]==bits(11.f));
 Fixture alpha;alpha.setText("A");alpha.words[0x8041e9a4]=1;alpha.onOp=[](auto&f,const Event&e){if(e.fn==0x80074720&&e.a[3]==2)f.bytes[0x8041e9a8]=128;};assert(alpha.run()&&alpha.quads.size()==5&&(alpha.quads[4].rgba&255)==128&&alpha.words[0x8041e9ac]==0x123456ff);
 Fixture lateSpacing;bool first=true;lateSpacing.onOp=[&](auto&f,const Event&e){if(e.fn==0x80074720&&first){first=false;f.words[0x80416020]=4;}};assert(lateSpacing.run()&&lateSpacing.glyphs[1].x==int32_t(-8.5f+width+4));
 Fixture stale;stale.hold=0;assert(!stale.run());stale.alive=false;assert(!stale.run()&&stale.cursor.stale());trace=stale.trace;stale.alive=true;assert(!stale.run()&&stale.trace==trace);
 // This source function maps raw control-looking bytes instead of parsing.
 Fixture raw;raw.setText("<\n");auto rawServices=raw.services();rawServices.common.glyph=[](void*p,const SourceMatrixFontGlyph&q){auto&f=*static_cast<Fixture*>(p);if(!f.operation(0x80074720,{q.glyph,q.sourceCode,uint32_t(q.x),uint32_t(q.y)}))return false;f.glyphs.push_back(q);return true;};assert(resumeSourceStringFont(raw.base,raw.text,rawServices,raw.cursor)&&raw.glyphs.size()==2&&raw.glyphs[0].sourceCode=='<'&&raw.glyphs[1].sourceCode=='\n');
 Fixture multibyte;multibyte.setText(std::string("\x81\x40",2));multibyte.words[Fixture::Gp+0x16c]=0;multibyte.words[0x8041601c]=2;auto multiServices=multibyte.services();multiServices.common.glyph=rawServices.common.glyph;assert(resumeSourceStringFont(multibyte.base,multibyte.text,multiServices,multibyte.cursor)&&multibyte.glyphs.size()==1&&multibyte.glyphs[0].sourceCode==0x8140);
 for(unsigned i=0;i<3;++i){Fixture waiting;waiting.textHold=int(i);assert(!waiting.run());assert(!waiting.run());assert(waiting.run());assert(waiting.glyphs.size()==2&&waiting.randoms==4);}
 std::cout<<"Centered StringFont whole parent, real BFN glyphs and every child wait PASS\n";
}

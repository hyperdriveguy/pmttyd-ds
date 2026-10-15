#include "battle_font_gpu.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <limits>
using namespace ttyd;
namespace {
void require(bool value,int line){if(!value){std::cerr<<"battle font GPU check failed at "<<line<<'\n';std::abort();}}
#define CHECK(...) require((__VA_ARGS__),__LINE__)
uint32_t word(const std::vector<uint8_t>&b,uint32_t a){CHECK(uint64_t(a)+4<=b.size());return uint32_t(b[a])<<24|uint32_t(b[a+1])<<16|uint32_t(b[a+2])<<8|b[a+3];}
uint16_t half(const std::vector<uint8_t>&b,uint32_t a){CHECK(uint64_t(a)+2<=b.size());return uint16_t(uint16_t(b[a])<<8|b[a+1]);}
BattleClassSurfaceImport image(){return {reinterpret_cast<void*>(uintptr_t(0x1234)),512,256,0,1,512,256,false,{0,1,1,0}};}
struct Fixture {
 static constexpr uint32_t Bfn=0x300000;
 MessageFont font;BattleFontGpuResource resource;BattleFontGlyphSink sink;
 std::vector<uint8_t> bfn;std::map<uint32_t,uint32_t> words;
 std::shared_ptr<const void> resources=std::make_shared<int>(4);
 bool source=true,allocation=true,ordinary=false,reenter=false,retireProfile=false;
 unsigned randoms=0;
 BattleFontRasterView view;BattleActMatrix loaded{1,0,0,17,0,1,0,23,0,0,1,9};
 Fixture(){
  CHECK(font.load("romfs/font.pmfont"));std::ifstream meta("romfs/font.json");const std::string text((std::istreambuf_iterator<char>(meta)),{});const auto a=text.find("source_offset");CHECK(a!=std::string::npos);const auto offset=std::stoull(text.substr(text.find(':',a)+1));
  std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);CHECK(bool(iso));bfn.resize(32);iso.seekg(std::streamoff(offset));iso.read(reinterpret_cast<char*>(bfn.data()),32);CHECK(bool(iso));const auto n=word(bfn,8);CHECK(n==66560);bfn.resize(n);iso.seekg(std::streamoff(offset));iso.read(reinterpret_cast<char*>(bfn.data()),n);CHECK(bool(iso));
  uint32_t gly=0,info=0;for(uint32_t i=0,at=32;i<word(bfn,12);++i){const auto tag=word(bfn,at);if(tag==0x494e4631)info=at;if(tag==0x474c5931)gly=at;if(tag==0x57494431&&half(bfn,at+8)==0){bfn[at+13]=7;bfn[at+13]=9;}at+=word(bfn,at+4);}CHECK(gly&&info);
  words={{0x8041e978,Bfn},{0x8041e99c,Bfn+gly},{0x8041e9a0,Bfn+info}};
  CHECK(resource.adopt(font,image()));view.viewport={0,0,240,400};view.scissor=view.viewport;for(unsigned i=0;i<4;++i)view.projection[i][i]=float(i==3?1:i+2);
 }
 BattleFontGlyphBinding binding(size_t capacity=2){
  BattleFontGlyphBinding b;b.font=resource.lease();b.resources=resources;b.raster=view;b.capacity=capacity;
  b.actors.user=this;b.actors.memory.user=this;
  b.actors.memory.word=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);if(a>=Bfn&&uint64_t(a)+4<=uint64_t(Bfn)+f.bfn.size())return word(f.bfn,a-Bfn);return f.words.at(a);};
  b.actors.memory.half=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);CHECK(a>=Bfn);return half(f.bfn,a-Bfn);};
  b.actors.byte=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);CHECK(a>=Bfn&&uint64_t(a)<uint64_t(Bfn)+f.bfn.size());return f.bfn[a-Bfn];};
  b.sourceCurrent=[this]{return source;};b.publicationCurrent=[this]{return allocation;};
  b.ordinaryProfile=[this]{if(retireProfile)sink.retire();return ordinary;};
  b.random=[this](uint32_t bound){CHECK(bound==64);++randoms;if(reenter){CHECK(!sink.glyph(glyph('A')));CHECK(!sink.loadMatrix(loaded,0));CHECK(!sink.tevColor(1,0));CHECK(!sink.publish());}return randoms%bound;};return b;
 }
 SourceMatrixFontGlyph glyph(uint8_t code,int32_t x=0,int32_t y=0)const{return {font.mapping[code],code,x,y};}
};
void residency(){
 Fixture f;const auto old=f.resource.lease();CHECK(old.current());CHECK(old.font.get()!=&f.font);auto copy=old;copy.font=std::make_shared<MessageFont>(f.font);CHECK(!copy.current());copy=old;copy.texture=reinterpret_cast<void*>(uintptr_t(0x5678));CHECK(!copy.current());
 CHECK(!f.resource.adopt(f.font,image()));f.resource.retire();CHECK(!old.current());CHECK(f.resource.adopt(f.font,image()));CHECK(!f.resource.accepts(old)&&f.resource.lease().generation!=old.generation);
 for(unsigned index=0;index<9;++index){auto i=image();switch(index){case 0:i.identity=nullptr;break;case 1:i.width=256;break;case 2:i.height=512;break;case 3:i.format=4;break;case 4:i.count=2;break;case 5:i.subWidth=511;break;case 6:i.subHeight=255;break;case 7:i.rotated=true;break;case 8:i.uv[1]=0;break;}BattleFontGpuResource r;CHECK(!r.adopt(f.font,i)&&!r.lease().current());}
 auto bad=f.font;bad.glyphs[0].height=21;BattleFontGpuResource r;CHECK(!r.adopt(bad,image()));bad=f.font;bad.mapping[0]=162;CHECK(!r.adopt(bad,image()));
 BattleFontLease final;{BattleFontGpuResource temporary;CHECK(temporary.adopt(f.font,image()));final=temporary.lease();}CHECK(!final.current());
}
void glyphCapture(){
 Fixture f;CHECK(f.sink.bind(f.binding()));CHECK(!f.sink.bind(f.binding()));CHECK(!f.sink.glyph(f.glyph('A'))&&f.randoms==0);
 CHECK(f.sink.loadMatrix(f.loaded,0));CHECK(!f.sink.loadMatrix(f.loaded,1));CHECK(!f.sink.tevColor(0,0));CHECK(f.sink.tevColor(1,0x12345680));CHECK(!f.sink.glyph(f.glyph('A'))&&f.randoms==0);
 f.ordinary=true;f.reenter=true;CHECK(f.sink.glyph(f.glyph('A',-5,12))&&f.randoms==2);f.loaded[3]=99;CHECK(f.sink.loadMatrix(f.loaded,0));CHECK(f.sink.tevColor(1,0xabcdef00));CHECK(f.sink.glyph(f.glyph('B',8,-20))&&f.randoms==4);CHECK(!f.sink.glyph(f.glyph('A'))&&f.randoms==4);
 CHECK(f.sink.publish());CHECK(!f.sink.publish());const auto p=f.sink.snapshot();CHECK(p&&p->size()==2);CHECK((*p)[0].matrix[3]==17&&(*p)[1].matrix[3]==99);CHECK((*p)[0].quad.rgba==0x12345680&&(*p)[1].quad.rgba==0xabcdef00);CHECK((*p)[0].quad.left==-5&&(*p)[0].quad.top==10);CHECK((*p)[1].quad.left==8&&(*p)[1].quad.top==-22);
 const auto m=battleFontGlyphTransform((*p)[0]);CHECK(m[0][3]==34&&m[1][3]==69&&m[2][3]==36&&m[3][3]==1);
 f.source=false;CHECK(f.sink.snapshot()==p);f.allocation=false;CHECK(!f.sink.snapshot());f.allocation=true;CHECK(!f.sink.snapshot());CHECK(validBattleFontGlyphPacket((*p)[0]));f.resource.retire();CHECK(!validBattleFontGlyphPacket((*p)[0]));
}
void lifetimeAndAdmission(){
 Fixture bad;auto b=bad.binding();b.ordinaryProfile={};CHECK(!bad.sink.bind(b));b=bad.binding();b.raster.scissor.width=241;CHECK(!bad.sink.bind(b));b=bad.binding();b.capacity=0;CHECK(!bad.sink.bind(b));
 Fixture f;CHECK(f.sink.bind(f.binding()));auto m=f.loaded;m[0]=std::numeric_limits<float>::quiet_NaN();CHECK(!f.sink.loadMatrix(m,0));CHECK(f.sink.loadMatrix(f.loaded,0));CHECK(f.sink.tevColor(1,0xffffffff));f.ordinary=true;f.retireProfile=true;CHECK(!f.sink.glyph(f.glyph('A'))&&f.randoms==0&&!f.sink.active());CHECK(!f.sink.bind(f.binding()));
 Fixture stale;CHECK(stale.sink.bind(stale.binding()));CHECK(stale.sink.loadMatrix(stale.loaded,0));CHECK(stale.sink.tevColor(1,0));stale.ordinary=true;stale.source=false;CHECK(!stale.sink.glyph(stale.glyph('A'))&&stale.randoms==0);stale.source=true;CHECK(!stale.sink.glyph(stale.glyph('A')));
 Fixture mismatch;CHECK(mismatch.sink.bind(mismatch.binding()));CHECK(mismatch.sink.loadMatrix(mismatch.loaded,0));CHECK(mismatch.sink.tevColor(1,0));mismatch.ordinary=true;const auto actual=mismatch.binding();const auto width=sourceMatrixFontWidth(mismatch.font.mapping['A'],actual.actors);mismatch.bfn.at(width-Fixture::Bfn+1)^=1;CHECK(!mismatch.sink.glyph(mismatch.glyph('A'))&&mismatch.randoms==0);
 Fixture invisible;CHECK(invisible.sink.bind(invisible.binding(1)));CHECK(invisible.sink.loadMatrix(invisible.loaded,0));CHECK(invisible.sink.tevColor(1,0x12345600));invisible.ordinary=true;CHECK(invisible.sink.glyph(invisible.glyph(' ',30000,30000))&&invisible.randoms==2);CHECK(invisible.sink.publish()&&invisible.sink.snapshot()->size()==1);
}
}
void finiteGeometry(){
 Fixture f;CHECK(f.sink.bind(f.binding()));CHECK(f.sink.loadMatrix(f.loaded,0));CHECK(f.sink.tevColor(1,0xffffffff));f.ordinary=true;CHECK(f.sink.glyph(f.glyph('A')));CHECK(f.sink.publish());const auto original=f.sink.snapshot()->front();
 auto p=original;const float maximum=std::numeric_limits<float>::max();
 p.matrix[0]=maximum;p.raster.projection[0][0]=maximum;CHECK(!validBattleFontGlyphPacket(p)); // finite products compose to infinity
 p=original;for(auto&row:p.raster.projection)row={};for(unsigned i=0;i<4;++i)p.raster.projection[i][i]=1;
 p.matrix[0]=maximum;p.quad.left=2;CHECK(!validBattleFontGlyphPacket(p)); // finite uniform, overflowing actual clip X
 p=original;for(auto&row:p.raster.projection)row={};p.raster.projection[0]={1,1,-1,0};p.raster.projection[3][3]=1;p.matrix={maximum,0,0,0,maximum,0,0,0,maximum,0,0,0};CHECK(!validBattleFontGlyphPacket(p)); // composed intermediate sum overflow
 p=original;for(auto&row:p.raster.projection)row={};for(unsigned i=0;i<4;++i)p.raster.projection[i][i]=1;
 p.matrix={maximum,maximum,0,-maximum,0,0,0,0,0,0,0,0};p.quad.left=p.quad.top=1;CHECK(!validBattleFontGlyphPacket(p)); // clip intermediate sum overflow before cancellation
 p=original;p.matrix={};CHECK(validBattleFontGlyphPacket(p)); // actual singular/zero-area transformed geometry
 p=original;p.quad.right=p.quad.left;p.quad.bottom=p.quad.top;CHECK(validBattleFontGlyphPacket(p));
 p=original;p.quad.left=30000;p.quad.right=30010;p.quad.top=30000;p.quad.bottom=29978;p.quad.rgba=0;CHECK(validBattleFontGlyphPacket(p));
 for(unsigned mode=0;mode<2;++mode){
  Fixture blocked;for(auto&row:blocked.view.projection)row={};for(unsigned i=0;i<4;++i)blocked.view.projection[i][i]=1;
  blocked.loaded[0]=mode?maximum:2;if(!mode)blocked.view.projection[0][0]=maximum;
  CHECK(blocked.sink.bind(blocked.binding()));CHECK(blocked.sink.loadMatrix(blocked.loaded,0));CHECK(blocked.sink.tevColor(1,0xffffffff));blocked.ordinary=true;
  CHECK(!blocked.sink.glyph(blocked.glyph('A',2,0))&&blocked.randoms==0);CHECK(blocked.sink.publish()&&blocked.sink.snapshot()->empty());
 }
 Fixture accepted;CHECK(accepted.sink.bind(accepted.binding(3)));CHECK(accepted.sink.loadMatrix(accepted.loaded,0));CHECK(accepted.sink.tevColor(1,0));accepted.ordinary=true;
 CHECK(accepted.sink.glyph(accepted.glyph(' ',30000,30000))&&accepted.randoms==2);
 CHECK(accepted.sink.loadMatrix({},0));CHECK(accepted.sink.glyph(accepted.glyph('A'))&&accepted.randoms==4);
 CHECK(accepted.sink.loadMatrix(accepted.loaded,0));CHECK(accepted.sink.glyph(accepted.glyph('B',32767,32767))&&accepted.randoms==6);
 CHECK(accepted.sink.publish()&&accepted.sink.snapshot()->size()==3);
}
void orderedHooks(){
 Fixture f;auto b=f.binding();unsigned waits=2,emits=0;std::optional<BattleFontGlyphPacket> reserved;
 b.orderedUser=&f;
 b.orderedAdmit=+[](void*,const BattleFontGlyphPacket&)->bool{return true;};
 Fixture missing;auto incomplete=missing.binding();incomplete.orderedAdmit=b.orderedAdmit;CHECK(!missing.sink.bind(incomplete));
 struct Ordered{Fixture*fixture;unsigned*waits,*emits;std::optional<BattleFontGlyphPacket>*reserved;} ordered{&f,&waits,&emits,&reserved};
 b.orderedUser=&ordered;b.orderedAdmit=[](void*p,const BattleFontGlyphPacket&q){auto&s=*static_cast<Ordered*>(p);CHECK(s.fixture->randoms==0);if(*s.waits){--*s.waits;return false;}*s.reserved=q;return true;};
 b.orderedEmit=[](void*p,const BattleFontGlyphPacket&q){auto&s=*static_cast<Ordered*>(p);CHECK(s.fixture->randoms==2&&s.reserved->has_value());CHECK(q.quad.rgba==(**s.reserved).quad.rgba);CHECK(!s.fixture->sink.publish());++*s.emits;};
 CHECK(f.sink.bind(b));CHECK(f.sink.loadMatrix(f.loaded,0));CHECK(f.sink.tevColor(1,0x12345600));f.ordinary=true;
 CHECK(!f.sink.glyph(f.glyph('A'))&&f.randoms==0&&emits==0);
 CHECK(!f.sink.glyph(f.glyph('A'))&&f.randoms==0&&emits==0);
 CHECK(f.sink.glyph(f.glyph('A'))&&f.randoms==2&&emits==1);
 CHECK(f.sink.publish()&&f.sink.snapshot()->size()==1);
}
int main(){residency();glyphCapture();lifetimeAndAdmission();finiteGeometry();orderedHooks();}

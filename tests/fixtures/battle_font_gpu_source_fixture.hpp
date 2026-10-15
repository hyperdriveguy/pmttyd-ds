#pragma once
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
}

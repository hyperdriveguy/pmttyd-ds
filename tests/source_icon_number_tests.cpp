#include "source_icon_number.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
using namespace ttyd;
namespace {
std::vector<uint8_t> file(const char* p){std::ifstream f(p,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
const std::vector<uint8_t>& packData(){static const auto data=file("romfs/harbor-tutorial-events.pmevt");return data;}
const std::vector<uint8_t>& tplData(){static const auto data=file("romfs/source-icons/icon.tpl");return data;}
const std::vector<uint8_t>& directoryData(){static const auto data=file("romfs/source-icons/icon.bin");return data;}
uint32_t be(const std::vector<uint8_t>& v,size_t o){return uint32_t(v.at(o))<<24|uint32_t(v.at(o+1))<<16|uint32_t(v.at(o+2))<<8|v.at(o+3);}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
 const std::vector<uint8_t>& pack=packData();
 const std::vector<uint8_t>& directory=directoryData();
 CameraViewMatrix base{{{1,0,0,10},{0,1,0,20},{0,0,1,30}}};uint32_t rgba=0x12345678;
 std::vector<uint32_t> halfReads;std::vector<SourceMatrixIcon> icons;std::vector<BattleActMatrix> transforms;
 std::vector<BattleAudienceMatrixRequest> math;
 int stop=-1;unsigned completed=0;uint32_t halfHold=0;bool halfRelease=false;bool release=false,late=false,useGX=false;
 SourceIconGXServices gx;std::vector<SourceIconGXCall> gxCalls;std::vector<SourceIconGXQuad> gxQuads;
 static constexpr uint32_t D=0x10000000,C=0x20000000;
 std::optional<uint32_t> source(uint32_t address){
  const auto n=be(pack,8);for(unsigned i=0;i<n;++i){const auto a=be(pack,12+12*i),size=be(pack,16+12*i),off=be(pack,20+12*i);
   if(address>=a && uint64_t(address)-a+4<=size)return be(pack,off+address-a);}
  return {};
 }
 bool effect(){if(int(completed)==stop && !release)return false;++completed;return true;}
 SourceIconNumberServices services(){SourceIconNumberServices s;auto& i=s.icons;i.user=this;
  i.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);
   if(a==0x8041e634 || a==0x8041e638)return D;
   if(a==0x8041e63c || a==0x8041e630)return 1;
   if(a>=C+0x11c && a<C+0x14c && !(a%4)){const auto n=(a-C-0x11c)/4;return bits(n==0 || n==5 || n==10?1.f:0.f);}
   return f.source(a);};
  i.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& f=*static_cast<Fixture*>(u);f.halfReads.push_back(a);if(a==f.halfHold && !f.halfRelease)return {};
   if(a>=D && uint64_t(a)-D+2<=f.directory.size()){const auto o=a-D;return uint16_t(uint16_t(f.directory[o])<<8|f.directory[o+1]);}
   const auto word=f.source(a&~3u);return word?std::optional<uint16_t>(uint16_t(*word>>((a&2)?0:16))):std::nullopt;};
  i.currentCamera=[](void*)->std::optional<uint32_t>{return C;};
  i.matrix=[](void* u,BattleAudienceMatrixRequest& q){auto& f=*static_cast<Fixture*>(u);if(!f.effect())return false;
   BattleAudienceAuxServices sdk;bindAudienceAuxMathAdapter(sdk);assert(sdk.matrix(nullptr,q));f.math.push_back(q);
   if(f.late && q.function==0x8029e230)f.base[0][3]+=1;
   if(f.late && q.function==0x8029dd64)f.rgba+=1;
   return true;};
  i.draw=[](void* u,const BattleActMatrix& m,const SourceMatrixIcon& icon){auto& f=*static_cast<Fixture*>(u);if(!f.effect())return false;
   f.icons.push_back(icon);f.transforms.push_back(m);return true;};
  if(useGX){gx={};gx.user=this;gx.word=i.word;gx.half=i.half;gx.matrix=i.matrix;
   gx.call=[](void* u,const SourceIconGXCall& q)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(u);
    if(!f.effect())return {};
    f.gxCalls.push_back(q);
    if(q.function==0x8000f2fc){assert(q.integers[0]==8);return C;}
    if(q.function==0x8000f2e4)return C;
    if(q.function==0x80037a80){assert(q.integers[0]==1 && q.integers[1]<be(tplData(),4));return q.integers[1]+1;}
    return 0;};
   gx.quad=[](void* u,const SourceIconGXQuad& q){auto& f=*static_cast<Fixture*>(u);if(!f.effect())return false;f.gxQuads.push_back(q);return true;};
   i.draw=nullptr;i.gx=&gx;
  }
  return s;
 }
 bool run(int32_t n,uint32_t mode,SourceIconNumberCursor& c,int32_t& width){return resumeSourceIconNumber(base,n,mode,rgba,services(),c,width);}
};
}
int main(){
 for(uint32_t mode:{0u,1u,2u,0xffffffffu})for(int32_t number:{0,1,9,10,99,100,32767,99999,100000,100010,123456,2147483647}){
  Fixture baseline;SourceIconNumberCursor c;int32_t width=-1;assert(baseline.run(number,mode,c,width));assert(!c.active());
  const unsigned count=number>=10000?5:number>=1000?4:number>=100?3:number>=10?2:1;
  assert(width==int32_t(count*(mode?16:20)) && baseline.icons.size()==count);
  int32_t remainder=number%100000;
  for(unsigned j=0;j<count;++j){const auto digit=remainder%10;remainder/=10;
   assert(baseline.icons[j].id==uint16_t(481+2*digit+(mode?1:0)));
   assert(baseline.icons[j].flags==16 && baseline.icons[j].rgba==0x12345678);
   assert(baseline.transforms[j][3]==10-float(j)*(mode?16:20));
   assert(baseline.transforms[j][7]==20 && baseline.transforms[j][11]==30);
  }
  for(unsigned stop=0;stop<baseline.completed;++stop){
   Fixture held;held.stop=int(stop);SourceIconNumberCursor cursor;int32_t out=777;
   assert(!held.run(number,mode,cursor,out) && out==777 && cursor.active());
   const auto done=held.completed;assert(!held.run(number,mode,cursor,out) && held.completed==done);
   auto otherBase=held.base;auto color=held.rgba;auto services=held.services();
   assert(!resumeSourceIconNumber(otherBase,number,mode,held.rgba,services,cursor,out));
   assert(!resumeSourceIconNumber(held.base,number,mode,color,services,cursor,out));
   auto wrong=services;wrong.icons.user=&baseline;
   assert(!resumeSourceIconNumber(held.base,number,mode,held.rgba,wrong,cursor,out));
   assert(held.completed==done);held.release=true;assert(held.run(number,mode,cursor,out));
   assert(out==width && held.icons.size()==baseline.icons.size() && held.transforms==baseline.transforms);
   for(unsigned j=0;j<count;++j)assert(held.icons[j].id==baseline.icons[j].id && held.icons[j].rgba==baseline.icons[j].rgba);
  }
 }
 // Signed source table indexing is preserved; no synthetic minus or abs ID.
 Fixture negative;SourceIconNumberCursor nc;int32_t nw=42;
 const bool negativeComplete=negative.run(-1,0,nc,nw);
 assert(!negative.halfReads.empty() && negative.halfReads[0]==0x80415082);
 if(negativeComplete){assert(nw==20 && negative.icons.size()==1);
  assert(negative.icons[0].id==uint16_t(*negative.source(0x80415080)));}
 else assert(nw==42);
 Fixture late;late.late=true;SourceIconNumberCursor lc;int32_t lw=0;assert(late.run(12,1,lc,lw));
 assert(late.icons[0].rgba==0x12345679 && late.icons[1].rgba==0x1234567b);
 assert(late.transforms[0][3]==11 && late.transforms[1][3]==-4);
 // Exercise the real retained iconGX child, including every acknowledged GX
 // setter/SDK/quad boundary. The host sink captures packets rather than drawing.
 Fixture raster;raster.useGX=true;SourceIconNumberCursor gc;int32_t gw=0;
 assert(raster.run(59,1,gc,gw) && gw==32 && raster.gxQuads.size()==2);
 for(unsigned stop=0;stop<raster.completed;++stop){
  Fixture held;held.useGX=true;held.stop=int(stop);SourceIconNumberCursor cursor;int32_t width=-1;
  assert(!held.run(59,1,cursor,width));const auto effects=held.completed;
  assert(!held.run(59,1,cursor,width) && held.completed==effects);
  held.release=true;assert(held.run(59,1,cursor,width));
  assert(width==32 && held.gxCalls.size()==raster.gxCalls.size() && held.gxQuads.size()==2);
  for(size_t j=0;j<raster.gxCalls.size();++j)assert(held.gxCalls[j].function==raster.gxCalls[j].function && held.gxCalls[j].integers==raster.gxCalls[j].integers);
  for(unsigned j=0;j<2;++j)assert(held.gxQuads[j].positions==raster.gxQuads[j].positions && held.gxQuads[j].uv==raster.gxQuads[j].uv);
 }
 Fixture baseHold;baseHold.stop=1;SourceIconNumberCursor bc;int32_t bw=-1;
 assert(!baseHold.run(12,1,bc,bw));baseHold.base[0][3]=100;baseHold.release=true;
 assert(baseHold.run(12,1,bc,bw));assert(baseHold.transforms[0][3]==10 && baseHold.transforms[1][3]==84);
 Fixture colorHold;colorHold.halfHold=0x8041509c;SourceIconNumberCursor cc;int32_t cw=-1;
 assert(!colorHold.run(12,1,cc,cw));colorHold.rgba=0xaabbccdd;colorHold.halfRelease=true;
 assert(colorHold.run(12,1,cc,cw));assert(colorHold.icons[0].rgba==0x12345678 && colorHold.icons[1].rgba==0xaabbccdd);
 Fixture absent;auto missing=absent.services();missing.icons.word=nullptr;SourceIconNumberCursor ac;int32_t aw=99;
 assert(!resumeSourceIconNumber(absent.base,1,1,absent.rgba,missing,ac,aw) && aw==99 && absent.completed==0);
 std::cout<<"Original Number digit/mode/transform/color/width, actual pack resources and retained child effects passed\n";
}

#include "battle_enemy_hp_gpu.hpp"
#include "battle_enemy_hp_raster.hpp"
#include "file_texture_relocation.hpp"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Import {
 unsigned imports=0,releases=0;uint32_t width=128,height=32,format=0;bool available=true;
 BattleEnemyHpTextureServices services(){return {this,[](void* p,const char* path)->std::optional<BattleEnemyHpTextureAdmission>{auto& f=*static_cast<Import*>(p);++f.imports;assert(std::string(path).find("battle-enemy-hp-surfaces.t3x")!=std::string::npos);if(!f.available)return {};return BattleEnemyHpTextureAdmission{p,f.width,f.height,f.format};},[](void* p,const void* id){auto& f=*static_cast<Import*>(p);assert(p==id);++f.releases;}};}
};
struct Fixture {
 static constexpr uint32_t Palette=0x87100000,B=0x84000000,Camera=0x90000;
 Import importer;BattleEnemyHpGpuResource resource;BattleEnemyHpGpuBatch batch;
 std::vector<uint8_t> raw;std::map<uint32_t,uint32_t> words;
 bool sinkReady=true,cameraReady=true,textureReady=true;unsigned sinkCalls=0;
 uint32_t word(uint32_t a)const{if(a>=Palette && uint64_t(a)+4<=uint64_t(Palette)+raw.size()){const auto o=a-Palette;return uint32_t(raw[o])<<24|uint32_t(raw[o+1])<<16|uint32_t(raw[o+2])<<8|raw[o+3];}return words.at(a);}
 BattleEnemyHpMemoryReader reader(){return {this,[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if((a>=Palette && uint64_t(a)+4<=uint64_t(Palette)+f.raw.size()) || f.words.count(a))return f.word(a);return {};}};}
 BattleEnemyHpQueryServices queries(){return {this,[](void* p)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);return f.cameraReady?std::optional<uint32_t>(Camera):std::nullopt;},[](void* p,uint32_t palette,uint32_t index)->std::optional<AnimationTPLImage>{auto& f=*static_cast<Fixture*>(p);return f.textureReady?readBattleEnemyHpTexture(f.reader(),palette,index):std::nullopt;}};}
 BattleEnemyHpQuadSink sink(){return {this,[](void* p,const BattleEnemyHpGpuQuad& q){auto& f=*static_cast<Fixture*>(p);++f.sinkCalls;if(!f.sinkReady)return false;f.batch.quads.push_back(q);return true;}};}
 Fixture(const char* root,const char* tpl){
   assert(resource.load(root,importer.services()));std::ifstream in(tpl,std::ios::binary);raw={std::istreambuf_iterator<char>(in),{}};assert(raw.size()==217920);
   words={{0x8041eb90,B},{B+0x163f8,0x80000},{0x800a0,0x81000},{0x81000,Palette},
     {0x80422234,bits(.5f)},{0x80422240,bits(0)},{0x8042224c,bits(1)},
     {0x802ee400,0x43300000},{0x802ee404,0}};
   BattleActMatrix view{1,0,0,10,0,1,0,20,0,0,1,30};for(unsigned i=0;i<12;++i)words[Camera+0x11c+4*i]=bits(view[i]);
   FileTextureRelocationServices s;s.memory.user=this;s.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
   s.memory.storeWord=[](void* p,uint32_t a,uint32_t n){auto& r=static_cast<Fixture*>(p)->raw;const auto o=a-Palette;assert(o+4<=r.size());for(unsigned j=0;j<4;++j)r[o+j]=uint8_t(n>>(24-8*j));};
   s.memory.storeByte=[](void* p,uint32_t a,uint8_t n){static_cast<Fixture*>(p)->raw.at(a-Palette)=n;};s.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->raw.at(a-Palette);};
   FileTextureRelocationCursor cursor;assert(resumePackedFileTextureRelocation(Palette,s,cursor));
 }
 BattleTexturePlaneServices services(BattleEnemyHpGpuProvider& provider){
   BattleTexturePlaneServices s;s.user=&provider;s.actors.memory.user=this;s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
   s.request=[](void* p,const BattleTexturePlaneRequest& q){return static_cast<BattleEnemyHpGpuProvider*>(p)->request(q);};
   BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);s.matrix=math.matrix;return s;
 }
 bool run(BattleEnemyHpRasterRequest& q,BattleEnemyHpGpuProvider& provider,BattleEnemyHpRasterCursor& c){return resumeBattleEnemyHpRaster(q,services(provider),c);}
};
BattleEnemyHpRasterRequest init(){BattleEnemyHpRasterRequest q;q.function=0x80103b48;return q;}
BattleEnemyHpRasterRequest plane(uint32_t index){BattleEnemyHpRasterRequest q;q.function=0x801032c8;q.integers={index,0x12345678,0};q.reals={3,4,5,.5f,2};return q;}
}
int main(int argc,char** argv){
 const char* root=argc>1?argv[1]:"romfs";const char* tpl=argc>2?argv[2]:"romfs/battle-class-common.tpl";
 Import imp;BattleEnemyHpGpuResource r;auto services=imp.services();assert(!r.load("/no/hp",services) && !imp.imports);
 services.release=nullptr;assert(!r.load(root,services) && !imp.imports);services=imp.services();imp.available=false;assert(!r.load(root,services));imp.available=true;imp.width=256;assert(!r.load(root,services) && imp.releases==1);imp.width=128;imp.format=14;assert(!r.load(root,services) && imp.releases==2);imp.format=0;assert(r.load(root,services));assert(!r.surface(16) && r.surface(17) && r.surface(18) && !r.surface(19));assert(!r.load(root,services));
 Fixture f(root,tpl);auto image=readBattleEnemyHpTexture(f.reader(),Fixture::Palette,17);assert(image && image->data==Fixture::Palette+0xb100 && image->width==40 && image->height==16);
 assert(!readBattleEnemyHpTexture(f.reader(),Fixture::Palette+4,17));assert(!readBattleEnemyHpTexture(f.reader(),Fixture::Palette,16));
 const auto old=f.raw[0x578+35];f.raw[0x578+35]=0;assert(!readBattleEnemyHpTexture(f.reader(),Fixture::Palette,17));f.raw[0x578+35]=old;
 const auto width=f.raw[0x578+3];f.raw[0x578+3]=41;assert(!readBattleEnemyHpTexture(f.reader(),Fixture::Palette,17));f.raw[0x578+3]=width;
 f.words[0x81000]=0;assert(!readBattleEnemyHpTexture(f.reader(),Fixture::Palette,17));f.words[0x81000]=Fixture::Palette;
 BattleEnemyHpGpuProvider p(f.resource,f.queries(),f.sink());
 BattleTexturePlaneRequest begin;begin.function=BattleTexturePlaneCall::Begin;begin.integers={128,0,4};assert(!p.request(begin));
 auto initial=init();BattleEnemyHpRasterCursor ic;assert(f.run(initial,p,ic));
 auto background=plane(17);BattleEnemyHpRasterCursor bc;f.cameraReady=false;assert(!f.run(background,p,bc) && f.batch.quads.empty());f.cameraReady=true;f.textureReady=false;assert(!f.run(background,p,bc) && f.batch.quads.empty());f.textureReady=true;
 // Only the final lane may publish. Sink holds cannot duplicate any quad or
 // consume the final lane. The source child retains that exact retry operand.
 f.sinkReady=false;assert(!f.run(background,p,bc));assert(f.batch.quads.empty() && f.sinkCalls==1);
 assert(!p.request(begin));BattleTexturePlaneRequest wrong;wrong.kind=BattleTexturePlaneKind::FifoByte;wrong.integers={255};assert(!p.request(wrong));assert(f.batch.quads.empty());
 assert(!f.run(background,p,bc) && f.sinkCalls==2);f.sinkReady=true;assert(f.run(background,p,bc));assert(f.batch.quads.size()==1);
 const auto& first=f.batch.quads[0];assert(first.index==17 && first.material==BattleEnemyHpMaterial::TexturedModulate && first.vertices[0].x==3 && first.vertices[0].y==40 && first.vertices[0].z==35 && first.vertices[0].rgba==0x12345678);
 auto secondInit=init();BattleEnemyHpRasterCursor si;assert(f.run(secondInit,p,si));
 BattleEnemyHpRasterRequest load;load.function=0x802ba2fc;load.integers={0};load.matrix=CameraViewMatrix{{{{1,0,0,0}},{{0,1,0,0}},{{0,0,1,0}}}};BattleEnemyHpRasterCursor lc;assert(f.run(load,p,lc));
 BattleEnemyHpRasterRequest solid;solid.function=0x80103918;solid.integers={0,0,0,128};solid.reals={1,2,3,-4};BattleEnemyHpRasterCursor sc;assert(f.run(solid,p,sc));assert(f.batch.quads.size()==2 && f.batch.quads[1].material==BattleEnemyHpMaterial::UntexturedRaster && !f.batch.quads[1].texture && f.batch.quads[1].index==0 && f.batch.quads[1].vertices[3].y==-4);
 auto lastInit=init();BattleEnemyHpRasterCursor li;assert(f.run(lastInit,p,li));auto fill=plane(18);BattleEnemyHpRasterCursor fc;assert(f.run(fill,p,fc));assert(f.batch.quads.size()==3 && f.batch.quads[2].index==18);
 assert(validateBattleEnemyHpBatch(f.batch,&f.resource));auto immutable=f.batch;f.batch.quads.clear();assert(immutable.quads.size()==3 && validateBattleEnemyHpBatch(immutable,&f.resource));
 auto invalid=immutable;invalid.quads.back().generation.reset();assert(!validateBattleEnemyHpBatch(invalid,&f.resource));invalid=immutable;invalid.quads[1].texture=f.resource.identity();assert(!validateBattleEnemyHpBatch(invalid,&f.resource));invalid=immutable;invalid.quads[0].vertices[0].u=std::numeric_limits<float>::quiet_NaN();assert(!validateBattleEnemyHpBatch(invalid,&f.resource));assert(!validateBattleEnemyHpBatch(immutable,nullptr));
 f.resource.retire();assert(!validateBattleEnemyHpBatch(immutable,&f.resource));assert(!p.request(begin));assert(f.resource.load(root,f.importer.services()));assert(!validateBattleEnemyHpBatch(immutable,&f.resource));assert(!p.request(begin));f.resource.retire();r.retire();
}

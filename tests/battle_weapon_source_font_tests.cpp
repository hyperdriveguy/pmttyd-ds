#include "battle_weapon_source_font.hpp"
#include "battle_weapon_source_memory.hpp"
#include "battle_audience_aux_callback.hpp"
#include "battle_timing_a.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
using namespace ttyd;
namespace {
constexpr uint32_t Camera=0x60031000,Text=0x71001000;
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float v){assert(std::isfinite(v)&&double(v)>=-2147483648.&&double(v)<2147483648.);return int32_t(v);};return p;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>startup;std::shared_ptr<EventGpLanguageAlias>alias;
 std::shared_ptr<EventNativeRuntime>vm=std::make_shared<EventNativeRuntime>(0x81100000,platform());std::shared_ptr<BattleWeaponSourceMemory>memory;
 MessageFont font;BattleFontGpuResource resource;std::unique_ptr<BattleWeaponSourceFont>owner;
 std::shared_ptr<SourceGxSetterRouting>routing;BattleAudienceAuxServices sdk;
 bool live=true,reserveReady=true,matrixReady=true,textureReady=true,reenter=false;
 int holdAt=-1;unsigned holds=0;std::vector<uint32_t>trace;
 bool permit(uint32_t operation){if(holdAt==int(trace.size())&&holds++<2)return false;trace.push_back(operation);return true;}BattleRandom random{1};unsigned rng=0,reserves=0,matrices=0;std::optional<BattleFontGlyphPacket>reserved;std::vector<BattleFontGlyphPacket>packets;
 Fixture(){
  process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());startup=NativeUiStartup::create(process,"romfs");assert(startup&&startup->resumeFont());uint32_t gp;assert(process->process().vm().memory.readWord(0x80415fd0,gp));
  alias=std::make_shared<EventGpLanguageAlias>(process->process().vm().gpAddresses,gp,startup->lifetime(),[u=startup]{return u->current();});memory=BattleWeaponSourceMemory::create(*vm,startup,alias,vm,[this]{return live;},"romfs/source-ui-startup/memory.pmevt");assert(memory);
  assert(vm->memory.registerRam(Text,{'A','B',0}));
  // Explicit TEST camera input, no CameraMain/queue/normal battle receipt.
  assert(vm->memory.registerRam(Camera,std::vector<uint8_t>(0x160)));
  const BattleActMatrix view{1,0,0,17,0,1,0,23,0,0,1,9};for(unsigned i=0;i<12;++i){uint32_t bits;std::memcpy(&bits,&view[i],4);assert(vm->memory.writeWord(Camera+0x11c+4*i,bits));}
  assert(font.load("romfs/font.pmfont"));BattleClassSurfaceImport image{this,512,256,0,1,512,256,false,{0,1,1,0}};assert(resource.adopt(font,image));bindAudienceAuxMathAdapter(sdk);
  routing=std::make_shared<SourceGxSetterRouting>(startup->gxHistory(),startup->lifetime(),[this]{return live;});
  BattleWeaponSourceFontBinding b;b.actors=memory->actors();b.font=resource.lease();b.resources=memory;b.capacity=128;b.sourceCurrent=[this]{return live&&memory->current()&&startup->language()==1;};b.publicationCurrent=[this]{return live&&resource.lease().current();};
  b.byte=[this](uint32_t a)->std::optional<uint8_t>{return permit(1)?memory->byte(a):std::nullopt;};b.half=[this](uint32_t a)->std::optional<uint16_t>{return permit(2)?memory->half(a):std::nullopt;};b.camera=[this]()->std::optional<uint32_t>{return permit(3)?std::optional<uint32_t>(Camera):std::nullopt;};
  b.matrix=[this](BattleAudienceMatrixRequest&q){++matrices;if(reenter&&owner){BattleWeaponSourceFontCall nested;nested.function=0x80077b4c;assert(!owner->request(nested));}return matrixReady&&permit(q.function)&&sdk.matrix(sdk.user,q);};
  b.truncate=[this](float v)->std::optional<int32_t>{if(!permit(4)||!std::isfinite(v)||double(v)<-2147483648.||double(v)>=2147483648.)return {};return int32_t(v);};b.fma=[this](float a,float c,float d)->std::optional<float>{if(!permit(5))return {};const auto v=std::fma(a,c,d);return std::isfinite(v)?std::optional<float>(v):std::nullopt;};
  b.staticTextureCurrent=[this](uint32_t address,const BattleFontLease&l){const auto object=startup->fontObject();return textureReady&&object&&object->object==address&&object->width==512&&object->height==256&&resource.accepts(l);};
  b.ordinaryCurrent=[this]{return bool(routing->capture());};b.random=[this](uint32_t bound){assert(bound==64&&reserved);++rng;return uint32_t(random.bounded(int32_t(bound)));};b.gx=routing;
  b.raster.viewport={0,0,240,400};b.raster.scissor=b.raster.viewport;for(unsigned i=0;i<4;++i)b.raster.projection[i][i]=1;
  b.orderedUser=this;b.reserve=[](void*p,const BattleFontGlyphPacket&q){auto&f=*static_cast<Fixture*>(p);++f.reserves;if(!f.reserveReady||!f.permit(6))return false;assert(!f.reserved&&validBattleFontGlyphPacket(q));f.reserved=q;return true;};b.commit=[](void*p,const BattleFontGlyphPacket&q){auto&f=*static_cast<Fixture*>(p);assert(f.reserved&&f.reserved->quad.u0==q.quad.u0&&f.reserved->quad.v0==q.quad.v0);f.packets.push_back(q);f.reserved.reset();};
  owner=BattleWeaponSourceFont::create(std::move(b));assert(owner);
 }
 BattleWeaponSourceFontCall call(uint32_t function){BattleWeaponSourceFontCall q;q.function=function;if(function==0x80075528)q.integers={Text};return q;}
 void start(){auto q=call(0x80077b4c);assert(owner->request(q));}
 void color(){auto q=call(0x800777e0);q.integers={0x12345680};assert(owner->request(q));}
 BattleWeaponSourceFontCall draw(uint32_t function){auto q=call(function);if(function==0x800773d0)q.reals={12,15};else q.matrix=BattleActMatrix{1,0,0,2,0,1,0,3,0,0,1,4};q.integers={Text};return q;}
};
void everySourceBoundaryTwice(){
 auto sequence=[](Fixture&f){for(uint32_t fn:{0x80075528u,0x80077b4cu,0x800777e0u,0x80076a10u,0x80076cdcu,0x800773d0u}){auto q=fn==0x80076a10||fn==0x80076cdc||fn==0x800773d0?f.draw(fn):f.call(fn);if(fn==0x800777e0)q.integers={0x12345680};bool done=false;for(unsigned attempt=0;attempt<4&&!done;++attempt)done=bool(f.owner->request(q));assert(done);}};
 Fixture baseline;sequence(baseline);const auto expected=baseline.trace;
 for(unsigned at=0;at<expected.size();++at){Fixture f;f.holdAt=int(at);sequence(f);assert(f.holds==3&&f.trace==expected&&f.rng==baseline.rng&&f.packets.size()==baseline.packets.size());for(unsigned i=0;i<f.packets.size();++i){const auto&a=f.packets[i];const auto&b=baseline.packets[i];assert(a.matrix==b.matrix&&a.quad.left==b.quad.left&&a.quad.top==b.quad.top&&a.quad.u0==b.quad.u0&&a.quad.v0==b.quad.v0&&a.quad.rgba==b.quad.rgba);}}
}
void heldTexturePrefix(){Fixture f;f.textureReady=false;auto q=f.call(0x80077b4c);assert(!f.owner->request(q)&&f.rng==0);const auto count=f.matrices;assert(f.memory->store(0x8041e9ac,4,0x55667788));assert(!f.owner->request(q)&&f.matrices==count&&f.memory->word(0x8041e9ac)==0x55667788);f.textureReady=true;assert(f.owner->request(q)&&f.memory->word(0x8041e9ac)==0x55667788);}
void fullOriginalChain(){Fixture f;const auto width=f.owner->request(f.call(0x80075528));assert(width&&*width&&f.rng==0);f.start();f.color();auto q=f.draw(0x80076a10);assert(f.owner->request(q));assert(f.packets.size()==2&&f.rng==4);for(const auto&p:f.packets)assert(validBattleFontGlyphPacket(p));
 const auto before=f.packets.size();q=f.draw(0x80076cdc);assert(f.owner->request(q));assert(f.packets.size()==before+2&&f.rng==8);
 q=f.draw(0x800773d0);assert(f.owner->request(q));assert(f.packets.size()==6&&f.rng==12);
}
void genuineShadowState(){Fixture f;f.start();f.color();assert(f.owner->request(f.call(0x80077ab4)));auto q=f.draw(0x80076a10);q.integers.clear();q.privateText=std::array<uint8_t,2>{'A',0};assert(f.owner->request(q)&&f.packets.size()==5&&f.rng==10);assert(f.memory->word(0x8041e9ac)==0x12345680&&f.packets.back().quad.rgba==0x12345680);assert(f.owner->request(f.call(0x80077aa8)));assert(f.memory->word(0x8041e9a4)==0);}
void holdsAndPrivateBytes(){Fixture f;f.reenter=true;f.matrixReady=false;auto start=f.call(0x80077b4c);assert(!f.owner->request(start));const auto pc=f.owner->pending();assert(pc&&!f.owner->request(start)&&f.owner->pending()==pc&&f.rng==0);f.matrixReady=true;assert(f.owner->request(start));f.color();auto q=f.draw(0x80076cdc);q.integers.clear();q.privateText=std::array<uint8_t,2>{'3',0};f.reserveReady=false;assert(!f.owner->request(q)&&f.rng==0&&f.packets.empty());const auto matrices=f.matrices;assert(!f.owner->request(q)&&f.rng==0&&f.matrices==matrices);auto changed=q;changed.privateText=std::array<uint8_t,2>{'4',0};assert(!f.owner->request(changed));f.reserveReady=true;assert(f.owner->request(q)&&f.packets.size()==1&&f.rng==2);
 q=f.draw(0x80076a10);q.integers={0x72000000};assert(!f.owner->request(q)&&f.rng==2);assert(f.vm->memory.registerRam(0x72000000,{'A',0}));assert(f.owner->request(q)&&f.rng==4);
 auto cells=std::make_shared<std::array<std::optional<uint8_t>,2>>();(*cells)[0]='A';SourceMatrixFontText text;text.user=cells.get();text.lifetime=cells;text.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{const auto&v=*static_cast<std::array<std::optional<uint8_t>,2>*>(p);return a<v.size()?v[a]:std::nullopt;};
 q=f.draw(0x80076cdc);q.integers.clear();q.sourceText=text;const auto prior=f.rng;assert(!f.owner->request(q)&&f.rng==prior+2);const auto prefix=f.packets.size();assert(!f.owner->request(q)&&f.rng==prior+2&&f.packets.size()==prefix);(*cells)[1]=0;assert(f.owner->request(q)&&f.rng==prior+2&&f.packets.size()==prefix);
 f.live=false;assert(!f.owner->request(f.call(0x80077b4c)));f.live=true;assert(!f.owner->request(f.call(0x80077b4c)));
}
}
int main(){everySourceBoundaryTwice();heldTexturePrefix();fullOriginalChain();genuineShadowState();holdsAndPrivateBytes();return 0;}

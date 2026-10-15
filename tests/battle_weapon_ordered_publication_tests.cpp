#include "battle_weapon_ordered_publication.hpp"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <limits>
#include <fstream>
#include <map>
#include <cmath>
#include <cstring>
#include "source_palette_texture_object.hpp"
using namespace ttyd;
namespace {
struct Fixture {
 BattleWeaponOrderedPublication owner;BattleFontGpuResource font;BattleSubmenuSurfaceGpuResource submenu;
 std::shared_ptr<const uint8_t> allocation=std::make_shared<const uint8_t>(1);
 std::shared_ptr<const void> resources=std::make_shared<const int>(2);
 std::shared_ptr<const uint8_t> iconGeneration=std::make_shared<const uint8_t>(3);
 std::filesystem::path dir;
 bool alive=true,executing=true,complete=false,ready=false,iconAvailable=true,reenter=false,retireOnReady=false;
 unsigned draws=0;std::vector<size_t> order;
 Fixture(){
  MessageFont original;assert(original.load("romfs/font.pmfont"));assert(font.adopt(original,{this,512,256,0,1,512,256,false,{0,1,1,0}}));
  dir=std::filesystem::temp_directory_path()/("pm-weapon-ordered-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));std::filesystem::create_directory(dir);
  FILE*f=std::fopen((dir/"battle-submenu-surfaces.bin").c_str(),"wb");assert(f);uint32_t count=2;std::fwrite("PMBSUB01",8,1,f);std::fwrite(&count,4,1,f);
  for(unsigned i=0;i<2;++i){uint32_t id=i+2;unsigned x=i?22:2,w=i?8:16;BattleClassRegion r{w,24,float(x)/32,1-2.f/32,float(x+w)/32,1-26.f/32};std::fwrite(&id,4,1,f);std::fwrite(&r,sizeof(r),1,f);}std::fclose(f);
  BattleClassSurfaceTextureServices gpu;gpu.user=this;gpu.import=[](void*p,uint32_t,const char*)->std::optional<BattleClassSurfaceImport>{return BattleClassSurfaceImport{p,32,32,0,1,32,32,false,{0,1,1,0}};};gpu.release=[](void*,uint32_t,const void*){};assert(submenu.load(dir.string().c_str(),gpu));
 }
 ~Fixture(){submenu.retire();font.retire();std::filesystem::remove_all(dir);}
 BattleWeaponOrderedBinding binding(size_t capacity=8){
  BattleWeaponOrderedBinding b;b.allocationOwner=allocation;b.sourceResourceOwner=b.resources=resources;b.allocation=0x85600200;b.generation=7;b.capacity=capacity;b.font=&font;b.submenu=&submenu;b.user=this;
  b.current=[](void*p,uint32_t a,uint64_t g){return static_cast<Fixture*>(p)->alive&&a==0x85600200&&g==7;};
  b.sourceCurrent=[](void*p,uint64_t pass){return static_cast<Fixture*>(p)->executing&&(pass==1||pass==2);};
  b.sourceComplete=[](void*p,uint64_t){return static_cast<Fixture*>(p)->complete;};
  b.icon=[](void*p,uint32_t image)->std::optional<BattleEnemyHpIconResidency>{auto&f=*static_cast<Fixture*>(p);if(f.reenter){assert(!f.owner.begin(1));assert(!f.owner.appendIcon(1,f.icon()));assert(!f.owner.seal(1));}if(!f.iconAvailable)return {};return BattleEnemyHpIconResidency{p,f.iconGeneration,image,16,20,32,32,0};};
  b.ready=[](void*p,const BattleWeaponOrderedBatch&){auto&f=*static_cast<Fixture*>(p);if(f.retireOnReady)f.owner.retire();return f.ready;};
  b.draw=[](void*p,const BattleWeaponOrderedBatch&batch){auto&f=*static_cast<Fixture*>(p);++f.draws;for(const auto&q:batch.packets)f.order.push_back(q.index());};return b;
 }
 BattleFontGlyphPacket glyph(){BattleFontGlyphPacket p;p.font=font.lease();p.matrix={1,0,0,0,0,1,0,0,0,0,1,0};p.quad.left=2;p.quad.right=10;p.quad.top=12;p.quad.bottom=-10;p.quad.u1=8;p.quad.v1=22;p.quad.rgba=0x12345600;p.raster.viewport=p.raster.scissor={0,0,240,400};for(unsigned i=0;i<4;++i)p.raster.projection[i][i]=1;return p;}
 RetainedIconPacket icon(){RetainedIconPacket q;q.image=334;q.camera=8;q.viewport=q.scissor={0,0,240,400};for(unsigned i=0;i<4;++i)q.transform[i][i]=1;return q;}
 BattleSubmenuPacketPtr panel(){auto p=std::make_shared<BattleSubmenuPacket>();p->surface=submenu.lease();p->camera.lifetime=allocation;p->camera.user=this;p->camera.current=[](void*p,const BattleSubmenuCameraProof&){return static_cast<Fixture*>(p)->alive;};p->camera.generation=7;p->camera.pass=1;p->camera.camera=8;p->camera.viewport={0,0,608,480,0,1};p->camera.scissor={0,0,608,480};for(unsigned i=0;i<4;++i)p->camera.projection[i][i]=1;
  auto&t=p->originalTexture;t.index=2;t.width=16;t.height=24;t.format=14;t.data=0x80010000;t.imageHeader=0x80008000;t.minFilter=t.magFilter=1;p->loadedObject[2]=(23u<<10)|15;p->loadedObject[5]=14;p->sourceResourceOwner=resources;p->sourceResourceUser=this;p->sourceResourceCurrent=[](void*p){return static_cast<Fixture*>(p)->alive;};p->matrix={1,0,0,0,0,1,0,0,0,0,1,0};p->positions={{{-8,12,0},{8,12,0},{8,-12,0},{-8,-12,0}}};p->sourceUV={{{0,0},{1,0},{1,1},{0,1}}};return p;
 }
 void start(size_t capacity=8){assert(owner.bind(binding(capacity)));assert(!owner.bind(binding(capacity)));assert(owner.begin(1));assert(owner.begin(1));}
 void mixed(){const auto g=glyph();assert(owner.reserveFont(1,g));assert(owner.reserveFont(1,g));assert(!owner.appendIcon(1,icon()));assert(!owner.seal(1));owner.commitFont(1,g);assert(owner.appendSubmenu(1,panel()));assert(owner.appendIcon(1,icon()));assert(owner.reserveFont(1,g));owner.commitFont(1,g);}
};
void ordered(){
 Fixture f;f.start();f.mixed();assert(!f.owner.seal(1));f.complete=true;const auto batch=f.owner.seal(1);assert(batch&&batch->packets.size()==4);assert(f.owner.seal(1)==batch);assert(f.owner.retains(334));
 assert(!f.owner.submit(1,batch));assert(!f.owner.submit(1,batch));assert(f.draws==0);f.ready=true;assert(f.owner.submit(1,batch));assert(f.owner.submit(1,batch));assert(f.draws==1&&(f.order==std::vector<size_t>{0,1,2,0}));
 const auto services=f.owner.submission();std::shared_ptr<const void> opaque=batch;assert(services.current(services.user,opaque));assert(!services.submit(services.user,2,opaque));
 f.executing=false;assert(f.owner.validate(batch));assert(services.current(services.user,opaque));assert(!f.owner.appendIcon(1,f.icon())); // camera callback finished; copied publication survives
 auto forged=std::make_shared<BattleWeaponOrderedBatch>(*batch);assert(!f.owner.validate(forged));auto alias=std::shared_ptr<const BattleWeaponOrderedBatch>(std::make_shared<int>(1),batch.get());assert(!f.owner.validate(alias));assert(!f.owner.submit(1,alias));f.executing=true;assert(f.owner.begin(2));assert(!f.owner.validate(batch));assert(!services.current(services.user,opaque));assert(!f.owner.appendIcon(1,f.icon()));
}
void rejection(){
 Fixture missing;auto b=missing.binding();b.sourceComplete=nullptr;assert(!missing.owner.bind(b));missing.start(1);const auto g=missing.glyph();assert(missing.owner.reserveFont(1,g));auto changed=g;changed.quad.left++;assert(!missing.owner.reserveFont(1,changed));changed=g;changed.matrix[1]=-0.f;assert(!missing.owner.reserveFont(1,changed));changed=g;changed.quad.pitchX=-0.f;assert(!missing.owner.reserveFont(1,changed));changed=g;changed.quad.pitchY=-0.f;assert(!missing.owner.reserveFont(1,changed));missing.owner.commitFont(1,g);assert(!missing.owner.appendIcon(1,missing.icon()));assert(!missing.owner.reserveFont(1,g));
 Fixture stale;stale.start();stale.mixed();stale.complete=true;auto batch=stale.owner.seal(1);assert(batch);stale.iconGeneration=std::make_shared<const uint8_t>(4);stale.ready=true;assert(!stale.owner.submit(1,batch)&&stale.draws==0);
 Fixture late;late.start();late.mixed();late.complete=true;batch=late.owner.seal(1);late.submenu.retire();assert(!late.owner.submit(1,batch)&&late.draws==0);
 Fixture font; font.start();font.mixed();font.complete=true;batch=font.owner.seal(1);font.font.retire();assert(!font.owner.validate(batch));
 Fixture allocation;allocation.start();allocation.mixed();allocation.complete=true;batch=allocation.owner.seal(1);allocation.alive=false;assert(!allocation.owner.validate(batch));allocation.alive=true;assert(!allocation.owner.validate(batch));
 Fixture recurse;recurse.start();recurse.reenter=true;assert(recurse.owner.appendIcon(1,recurse.icon()));recurse.complete=true;batch=recurse.owner.seal(1);assert(batch);recurse.ready=true;recurse.retireOnReady=true;assert(!recurse.owner.submit(1,batch)&&recurse.draws==0);
 Fixture unknown;unknown.start();unknown.iconAvailable=false;assert(!unknown.owner.appendIcon(1,unknown.icon()));unknown.iconAvailable=true;assert(unknown.owner.appendIcon(1,unknown.icon()));unknown.complete=true;batch=unknown.owner.seal(1);assert(batch&&batch->packets.size()==1);
 Fixture overflow;overflow.start();auto icon=overflow.icon();icon.transform[0][0]=std::numeric_limits<float>::max();icon.quad.positions[0][0]=2;assert(!overflow.owner.appendIcon(1,icon));auto huge=std::make_shared<BattleSubmenuPacket>(*overflow.panel());huge->camera.projection[0][0]=std::numeric_limits<float>::max();assert(!overflow.owner.appendSubmenu(1,huge));
 Fixture wrong;wrong.start();auto p=std::make_shared<BattleSubmenuPacket>(*wrong.panel());p->camera.pass=2;assert(!wrong.owner.appendSubmenu(1,p));p=std::make_shared<BattleSubmenuPacket>(*wrong.panel());p->sourceResourceOwner=std::make_shared<int>(6);assert(!wrong.owner.appendSubmenu(1,p));
}

uint32_t windowBe(const std::vector<uint8_t>&v,unsigned a){assert(a+4<=v.size());return uint32_t(v[a])<<24|uint32_t(v[a+1])<<16|uint32_t(v[a+2])<<8|v[a+3];}
std::vector<uint8_t> actualWindowNativeAtlas(){
 std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);std::vector<uint8_t>b(321984);iso.seekg(947396944);iso.read(reinterpret_cast<char*>(b.data()),b.size());assert(iso&&windowBe(b,0)==0x20af30);const auto h=windowBe(b,windowBe(b,8)),start=windowBe(b,h+8);
 std::array<std::array<uint8_t,4>,72*72>rgba{};auto rgb=[](unsigned n){return std::array<uint8_t,4>{uint8_t((n>>11)*255/31),uint8_t((n>>5&63)*255/63),uint8_t((n&31)*255/31),255};};
 for(unsigned by=0;by<72;by+=8)for(unsigned bx=0;bx<72;bx+=8)for(unsigned sub=0;sub<4;++sub){const unsigned at=start+((by/8)*9+bx/8)*32+sub*8;const auto a=uint16_t(b[at]<<8|b[at+1]),z=uint16_t(b[at+2]<<8|b[at+3]);std::array<std::array<uint8_t,4>,4>c{rgb(a),rgb(z),{}, {}};for(unsigned k=0;k<3;++k){c[2][k]=uint8_t(a>z?(2*c[0][k]+c[1][k])/3:(c[0][k]+c[1][k])/2);c[3][k]=a>z?uint8_t((c[0][k]+2*c[1][k])/3):0;}c[2][3]=255;c[3][3]=a>z?255:0;for(unsigned y=0;y<4;++y)for(unsigned x=0;x<4;++x)rgba[(by+(sub/2)*4+y)*72+bx+(sub%2)*4+x]=c[b[at+4+y]>>(6-2*x)&3];}
 std::vector<uint8_t>out(128*128*4);for(unsigned y=0;y<128;++y)for(unsigned x=0;x<128;++x){unsigned m=0;for(unsigned k=0;k<3;++k)m|=((x>>k&1)<<(2*k))|((y>>k&1)<<(2*k+1));auto at=4*(((y/8)*16+x/8)*64+m);for(unsigned k=0;k<4;++k)out[at+3-k]=rgba[std::min(y,71u)*72+std::min(x,71u)][k];}return out;
}
// Explicit isolated window-source fixture. Actual readonly CMPR/TPL/SDK bytes
// and complete TEX5+GX5/Window source bodies execute; whole globalwindowSetup,
// original scheduler/normalbattle and physical GPU ownership are not asserted.
struct WindowFixture {
 Fixture& mixed;BattleWindowSurfaceResource resource;
 std::shared_ptr<const void>windowOwner=std::make_shared<const int>(80);
 SourcePrivateTextureObject object;std::map<uint32_t,uint8_t>memory;
 std::unique_ptr<BattleWindowPacketProvider>provider;WindowPrimitiveServices source;WindowPrimitiveCursor cursor;
 BattleWindowTextureObject texture;bool sourceAlive=true,textureReady=true,matrixReady=true,ordinaryGx=true;
 unsigned sourceCalls=0,sinkCalls=0;uint32_t color=0x123456cf;
 static constexpr uint32_t Palette=0x8a000000,Object=0x8b000000;
 WindowFixture(Fixture&f):mixed(f){
  auto atlas=actualWindowNativeAtlas();assert(resource.adopt({this,128,128,0,1,128,128,false,{0,1,1,0}},atlas.data(),atlas.size()));
  assert(object.bind(Object,windowOwner,this,[](void*p){return static_cast<WindowFixture*>(p)->sourceAlive;}));
  std::ifstream iso("/home/carson/games/roms/G8ME01.iso",std::ios::binary);assert(iso);
  auto read=[&](uint32_t a,unsigned n){std::vector<uint8_t>v(n);iso.seekg(a);iso.read(reinterpret_cast<char*>(v.data()),n);assert(iso);return v;};
  auto be=[](const std::vector<uint8_t>&v,unsigned a){assert(a+4<=v.size());return uint32_t(v[a])<<24|uint32_t(v[a+1])<<16|uint32_t(v[a+2])<<8|v[a+3];};
  const auto tpl=read(947396944,321984);const auto table=be(tpl,8),header=be(tpl,table);put(Palette+8,4,Palette+table);put(Palette+table,4,Palette+header);for(unsigned i=0;i<36;++i)memory[Palette+header+i]=tpl[header+i];put(Palette+header+8,4,Palette+be(tpl,header+8));
  const auto dol=be(read(0x420,4),0);const auto h=read(dol,256);
  for(auto span:{std::pair<uint32_t,unsigned>{0x803c4e84,60},{0x8041e558,8},{0x80429440,40},{0x802c0140,8},{0x80420528,4},{0x80420538,20}}){bool found=false;for(unsigned i=0;i<18;++i){const auto base=be(h,0x48+4*i),size=be(h,0x90+4*i);if(span.first>=base&&uint64_t(span.first)+span.second<=uint64_t(base)+size){assert(!found);found=true;const auto v=read(dol+be(h,4*i)+span.first-base,span.second);for(unsigned j=0;j<span.second;++j)memory[span.first+j]=v[j];}}assert(found);}
  SourcePaletteTextureObjectServices s;s.gx.user=this;s.gx.lifetime=windowOwner;s.gx.current=[](void*p){return static_cast<WindowFixture*>(p)->sourceAlive;};
  s.gx.word=[](void*p,uint32_t a){return static_cast<WindowFixture*>(p)->get(a,4);};
  s.gx.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{const auto v=static_cast<WindowFixture*>(p)->get(a,1);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
  s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{const auto v=static_cast<WindowFixture*>(p)->get(a,2);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;};
  s.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&f=*static_cast<WindowFixture*>(p);const auto hi=f.get(a,4),lo=f.get(a+4,4);return hi&&lo?std::optional<uint64_t>(uint64_t(*hi)<<32|*lo):std::nullopt;};
  s.subtractSingle=[](void*,uint64_t a,uint64_t b)->std::optional<uint32_t>{double x,y;std::memcpy(&x,&a,8);std::memcpy(&y,&b,8);if(!std::isfinite(x)||!std::isfinite(y))return {};volatile float v=float(x-y);const float captured=v;uint32_t bits;std::memcpy(&bits,&captured,4);return bits;};
  s.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){return static_cast<WindowFixture*>(p)->put(a,n,v);};
  s.gx.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<WindowFixture*>(p);if(!f.object.owns(a,32))return {};for(unsigned i=0;i<8;++i)if(!f.put(a+4*i,4,0))return {};return a;};
  s.gx.truncate=[](void*,float x)->std::optional<int32_t>{if(!std::isfinite(x)||double(x)<-2147483648.0||double(x)>=2147483648.0)return {};return int32_t(x);};
  SourcePaletteTextureObjectCursor cpu;assert(resumeSourcePaletteTextureObject(Palette,Object,0,s,cpu));const auto words=object.words();assert(words);
  texture.address=Object;texture.bytes=*words;texture.lifetime=windowOwner;texture.user=this;texture.current=[](void*p){return static_cast<WindowFixture*>(p)->sourceAlive;};auto&i=texture.image;i.index=0;i.imageHeader=Palette+header;i.data=Palette+be(tpl,header+8);i.height=uint16_t(tpl[header]<<8|tpl[header+1]);i.width=uint16_t(tpl[header+2]<<8|tpl[header+3]);i.format=be(tpl,header+4);i.wrapS=be(tpl,header+12);i.wrapT=be(tpl,header+16);i.minFilter=be(tpl,header+20);i.magFilter=be(tpl,header+24);auto bias=be(tpl,header+28);std::memcpy(&i.bias,&bias,4);i.edge=tpl[header+32];i.minLOD=tpl[header+33];i.maxLOD=tpl[header+34];
  BattleSubmenuCameraProof camera;camera.lifetime=mixed.allocation;camera.user=this;camera.current=[](void*p,const BattleSubmenuCameraProof&){return static_cast<WindowFixture*>(p)->mixed.alive;};camera.generation=7;camera.pass=1;camera.camera=8;for(unsigned r=0;r<4;++r)camera.projection[r][r]=1;camera.viewport={0,0,608,480,0,1};camera.scissor={0,0,608,480};
  BattleWindowQueryServices query;query.user=this;query.lifetime=windowOwner;query.current=[](void*p){auto&f=*static_cast<WindowFixture*>(p);return f.mixed.alive&&f.mixed.executing;};query.camera=[](void*)->std::optional<uint32_t>{return 0x1000;};query.texture=[](void*p,uint32_t address)->std::optional<BattleWindowTextureObject>{auto&f=*static_cast<WindowFixture*>(p);if(!f.textureReady||address!=Object)return {};return f.texture;};
  BattleWindowPacketSink sink;sink.user=this;sink.append=[](void*p,BattleWindowPacketPtr q){auto&f=*static_cast<WindowFixture*>(p);++f.sinkCalls;return f.mixed.owner.appendWindow(1,std::move(q));};provider=std::make_unique<BattleWindowPacketProvider>(resource,camera,query,sink);
  // Isolated primitive entry condition, not a manufactured globalSetup claim.
  put(0x8041ea1c,4,1);put(0x8041ea24,4,Object);for(unsigned n=0;n<12;++n){float v=n%5==0?1.f:0.f;uint32_t bits;std::memcpy(&bits,&v,4);put(0x111c+4*n,4,bits);}
  source.user=this;source.current=query.current;source.word=s.gx.word;source.request=[](void*p,const WindowPrimitiveRequest&q){auto&f=*static_cast<WindowFixture*>(p);++f.sourceCalls;if(!f.ordinaryGx&&q.function!=0x8000f2e4)return std::optional<uint32_t>{};return f.provider->request(q);};source.matrix=[](void*p,BattleAudienceMatrixRequest&q){if(!static_cast<WindowFixture*>(p)->matrixReady)return false;q.output={};if(q.function==0x8029dd04){for(unsigned n=0;n<12;++n)q.output[n]=n%5==0?1.f:0.f;return true;}if(q.function==0x8029dd64){for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c){for(unsigned k=0;k<3;++k)q.output[4*r+c]+=q.left[4*r+k]*q.right[4*k+c];if(c==3)q.output[4*r+c]+=q.left[4*r+3];}return true;}return false;};
 }
 std::optional<uint32_t>get(uint32_t a,uint32_t n){if(object.overlaps(a,n))return object.read(a,n);uint32_t v=0;for(uint32_t i=0;i<n;++i){const auto it=memory.find(a+i);if(it==memory.end())return {};v=(v<<8)|it->second;}return v;}
 bool put(uint32_t a,uint32_t n,uint32_t v){if(object.overlaps(a,n))return object.store(a,n,v);for(uint32_t i=0;i<n;++i)memory[a+i]=uint8_t(v>>(8*(n-i-1)));return true;}
 void bind(size_t capacity=8,bool actualWindow=true){auto b=mixed.binding(capacity);if(actualWindow)b.window=&resource;assert(mixed.owner.bind(b));assert(mixed.owner.begin(1));}
 bool run(){return resumeOriginalWindowPrimitive(0,color,10,20,400,50,10,source,cursor);}
};
void originalWindowFirst(){
 Fixture f;WindowFixture w(f);w.bind();assert(w.run()&&w.sinkCalls==1);f.mixed();f.complete=true;const auto batch=f.owner.seal(1);assert(batch&&batch->packets.size()==5);const auto window=std::get<BattleWindowPacketPtr>(batch->packets[0]);assert(window&&window->source.lifetime!=batch->sourceResourceOwner&&window->source.bytes==w.texture.bytes);assert(!f.owner.submit(1,batch)&&!f.owner.submit(1,batch)&&f.draws==0);f.ready=true;assert(f.owner.submit(1,batch)&&f.owner.submit(1,batch)&&f.draws==1);assert((f.order==std::vector<size_t>{3,0,1,2,0}));f.executing=false;assert(f.owner.validate(batch));w.sourceAlive=false;assert(!f.owner.validate(batch));
 Fixture hold;WindowFixture late(hold);late.bind();const auto glyph=hold.glyph();assert(hold.owner.reserveFont(1,glyph));assert(!late.run()&&late.provider->partial());const auto calls=late.sourceCalls;assert(!late.run()&&late.sourceCalls==calls+1&&late.sinkCalls==2);late.color=0;hold.owner.commitFont(1,glyph);assert(late.run()&&late.sinkCalls==3);hold.complete=true;const auto resumed=hold.owner.seal(1);assert(resumed&&resumed->packets.size()==2&&std::get<BattleWindowPacketPtr>(resumed->packets[1])->rgba==0x123456cf);
 Fixture absent;WindowFixture unavailable(absent);unavailable.bind(8,false);assert(!unavailable.run()&&unavailable.provider->partial());assert(!absent.owner.seal(1));
 Fixture missing;WindowFixture query(missing);query.bind();query.textureReady=false;assert(!query.run()&&query.cursor.pending()==0x802b7880&&query.sinkCalls==0);query.textureReady=true;assert(query.run()&&query.sinkCalls==1);missing.complete=true;auto p=missing.owner.seal(1);assert(p);query.resource.retire();missing.ready=true;assert(!missing.owner.submit(1,p)&&missing.draws==0);
 Fixture math;WindowFixture sdk(math);sdk.bind();sdk.matrixReady=false;assert(!sdk.run()&&sdk.cursor.pending()==0x8029dd04&&sdk.sinkCalls==0);sdk.matrixReady=true;assert(sdk.run());
 Fixture gx;WindowFixture state(gx);state.bind();state.ordinaryGx=false;assert(!state.run()&&state.cursor.pending()==0x802b5c8c&&state.sinkCalls==0);state.ordinaryGx=true;assert(state.run());
 Fixture data;WindowFixture unknown(data);unknown.bind();unknown.memory.erase(0x80420538);assert(!unknown.run()&&unknown.cursor.pending()==0x80420538&&unknown.sinkCalls==0);
 Fixture capacity;WindowFixture one(capacity);one.bind(1);assert(one.run());assert(!capacity.owner.reserveFont(1,capacity.glyph()));
}
}
int main(){ordered();rejection();originalWindowFirst();return 0;}

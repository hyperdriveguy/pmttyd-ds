#include "battle_class_surface_packets.hpp"
#include "battle_class_raster_continuation.hpp"
#include "source_palette_texture_object.hpp"
#include "battle_weapon_ordered_publication.hpp"
#include "battle_weapon_native_raster_plan.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <limits>
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
  for(auto span:{std::pair<uint32_t,unsigned>{0x803c4e84,60},{0x8041e558,8},{0x80429440,40},{0x802c0140,8},{0x80422200,0x400},{0x802ef650,0x100}}){bool found=false;for(unsigned i=0;i<18;++i){const auto base=be(h,0x48+4*i),size=be(h,0x90+4*i);if(span.first>=base&&uint64_t(span.first)+span.second<=uint64_t(base)+size){assert(!found);found=true;const auto v=read(dol+be(h,4*i)+span.first-base,span.second);for(unsigned j=0;j<span.second;++j)bytes[span.first+j]=v[j];}}assert(found);}
  // CPU construction reads only headers; actual pixel pointers are relocated
  // faithfully but neither the source helper nor this test samples pixels.
  const auto tpl=read(942647408,217920);assert(be(tpl,0)==0x0020af30&&be(tpl,4)==97);
  const uint32_t base=0x88000000,table=be(tpl,8);
  auto put=[&](uint32_t a,uint32_t value){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(value>>(24-8*i));};
  for(unsigned j=0;j<12+97*8;++j){bytes[base+j]=tpl[j];}put(base+8,base+table);
  for(unsigned index=0;index<4;++index){const auto header=be(tpl,table+8*index);put(base+table+8*index,base+header);for(unsigned j=0;j<36;++j)bytes[base+header+j]=tpl[header+j];put(base+header+8,base+be(tpl,header+8));bytes[base+header+35]=1;}
  const auto window=read(947396944,321984);assert(be(window,0)==0x0020af30);const uint32_t wb=0x89000000,wt=be(window,8),wh=be(window,wt);put(wb+8,wb+wt);put(wb+wt,wb+wh);for(unsigned j=0;j<36;++j)bytes[wb+wh+j]=window[wh+j];put(wb+wh+8,wb+be(window,wh+8));
 }
};
const Original&original(){static Original v;return v;}
using Event=std::array<uint64_t,4>;
struct TextureFixture {
 std::map<uint32_t,uint8_t> memory=original().bytes;std::vector<Event>trace;
 std::shared_ptr<const void> lifetime=std::make_shared<int>(1);SourcePrivateTextureObject output;
 SourcePaletteTextureObjectCursor cursor;SourcePaletteTextureObjectServices s;
 uint32_t palette=0x88000000,object=0x90000000,index=2;bool alive=true,reenter=false,failClear=false,partialClear=false;
 size_t holdAt=size_t(-1);unsigned attempts=0,clears=0,clearPosition=0,clearStores=0;bool clearStarted=false;
 TextureFixture(uint32_t objectBase=0x90000000,uint32_t textureIndex=2):object(objectBase),index(textureIndex){assert(output.bind(object,lifetime,this,[](void*p){return static_cast<TextureFixture*>(p)->alive;}));s.gx.user=this;s.gx.lifetime=lifetime;s.gx.current=[](void*p){return static_cast<TextureFixture*>(p)->alive;};
  s.gx.word=[](void*p,uint32_t a)->std::optional<uint32_t>{return static_cast<TextureFixture*>(p)->read(a,4,1);};
  s.gx.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{const auto v=static_cast<TextureFixture*>(p)->read(a,1,2);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
  s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{const auto v=static_cast<TextureFixture*>(p)->read(a,2,3);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;};
  s.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&f=*static_cast<TextureFixture*>(p);if(!f.ready())return {};const auto hi=f.get(a,4),lo=f.get(a+4,4);if(!hi||!lo)return {};const auto n=uint64_t(*hi)<<32|*lo;f.trace.push_back({4,a,8,n});return n;};
  s.subtractSingle=[](void*p,uint64_t a,uint64_t b)->std::optional<uint32_t>{auto&f=*static_cast<TextureFixture*>(p);if(!f.ready())return {};const auto x=real(a),y=real(b);if(!std::isfinite(x)||!std::isfinite(y))return {};volatile float v=float(x-y);f.trace.push_back({5,a,b,bits(v)});return bits(v);};
  s.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t value){auto&f=*static_cast<TextureFixture*>(p);if(!f.ready())return false;f.trace.push_back({6,a,n,value});return f.put(a,n,value);};
  s.gx.truncate=[](void*p,float x)->std::optional<int32_t>{auto&f=*static_cast<TextureFixture*>(p);if(!f.ready()||!std::isfinite(x)||double(x)<-2147483648.0||double(x)>=2147483648.0)return {};f.trace.push_back({7,bits(x),0,0});return int32_t(x);};
  s.gx.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<TextureFixture*>(p);if(f.failClear||!f.ready())return {};if(f.reenter){assert(!f.cursor.reset());assert(!f.resume());}if(!f.clearStarted){f.clearStarted=true;++f.clears;f.trace.push_back({8,a,32,0});}while(f.clearPosition<32){assert(f.put(a+f.clearPosition,1,0));++f.clearPosition;++f.clearStores;if(f.partialClear&&f.clearPosition==16)return {};}return 0;};
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
void metadata(const std::string&root){
 auto write=[&](const char*name,const char*magic,unsigned count,bool icons){FILE*f=std::fopen((root+name).c_str(),"wb");assert(f);std::fwrite(magic,8,1,f);std::fwrite(&count,4,1,f);for(unsigned i=0;i<count;++i){uint32_t id=icons?334+i:i;std::fwrite(&id,4,1,f);if(icons){uint32_t loop=0,frames=1,delay=0;for(auto n:{loop,frames,delay})std::fwrite(&n,4,1,f);}const unsigned x=icons?(i%4)*52+2:i?46:2,y=icons?(i/4)*52+2:2,w=icons?48:i?128:40,h=icons?48:i?80:40,ah=icons?256:128;BattleClassRegion r{w,h,float(x)/256,1-float(y)/ah,float(x+w)/256,1-float(y+h)/ah};std::fwrite(&r,sizeof(r),1,f);}std::fclose(f);};
 write("/battle-class-surfaces.bin","PMBCLS01",2,false);write("/battle-class-icons.bin","PMICON1\0",10,true);
}
struct Fixture {
 TextureFixture texture;std::array<std::unique_ptr<TextureFixture>,2>objects;
 BattleClassSurfaceGpuResource resource;std::filesystem::path dir;
 std::shared_ptr<const void>life=std::make_shared<int>(3);bool live=true,executing=true,objectLive=true,ordinary=true,reuseOutput=false;
 BattleSubmenuCameraProof camera;BattleClassSurfaceQueryServices query;BattleClassSurfacePacketSink sink;
 std::unique_ptr<BattleClassSurfacePacketProvider>provider;std::unique_ptr<BattleClassSurfaceRaster>raster;
 BattleSubmenuSourceServices source;BattleClassRasterServices services;BattleClassRasterCursor circle,bubble;BattleActMatrix base=identity();
 std::vector<BattleClassSurfacePacketPtr>packets;std::vector<uint32_t>trace;
 std::map<uint32_t,uint32_t>memory;size_t holdAt=size_t(-1);unsigned waits=0,sinkHolds=0,sinkCalls=0,blocked=0;bool reenter=false;std::function<bool(BattleClassSurfacePacketPtr)>append;
 static constexpr uint32_t B=0x10000,R=0x20000,W=0x30000,C=0x40000;
 Fixture(){
  for(unsigned i=0;i<2;++i){objects[i]=std::make_unique<TextureFixture>(0x90000000+0x100*i,i);objects[i]->run();}
  dir=std::filesystem::temp_directory_path()/("pm-class-packets-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));std::filesystem::create_directory(dir);metadata(dir.string());
  BattleClassSurfaceTextureServices gpu;gpu.user=this;gpu.import=[](void*p,uint32_t i,const char*)->std::optional<BattleClassSurfaceImport>{auto&f=*static_cast<Fixture*>(p);return BattleClassSurfaceImport{i?static_cast<const void*>(&f.objects):static_cast<const void*>(&f.resource),256,i?256u:128u,0,1,256,i?256u:128u,false,{0,1,1,0}};};gpu.release=[](void*,uint32_t,const void*){};assert(resource.load(dir.c_str(),gpu));
  camera.lifetime=life;camera.user=this;camera.current=[](void*p,const BattleSubmenuCameraProof&v){return static_cast<Fixture*>(p)->live&&v.pass==7&&v.generation==9;};camera.pass=7;camera.generation=9;camera.camera=8;camera.viewport={0,0,608,480,0,1};camera.scissor={0,0,608,480};for(unsigned i=0;i<4;++i)camera.projection[i][i]=1;
  query.user=this;query.lifetime=life;query.sourceResourceOwner=life;query.current=[](void*p){auto&f=*static_cast<Fixture*>(p);return f.live&&f.executing;};query.sourceResourceCurrent=[](void*p){return static_cast<Fixture*>(p)->live;};query.ordinaryCurrent=[](void*p){return static_cast<Fixture*>(p)->ordinary;};
  query.texture=[](void*p,uint32_t palette,uint32_t index)->std::optional<BattleClassSurfaceTextureObject>{auto&f=*static_cast<Fixture*>(p);if(palette!=0x88000000||index>1)return {};auto&o=*f.objects[f.reuseOutput?0:index];if(o.index!=index){o.index=index;o.clearStarted=false;o.clearPosition=0;o.run();}const auto words=o.output.words();if(!words)return {};const auto table=f.texture.word(palette+8),header=f.texture.word(table+8*index);AnimationTPLImage image;image.index=index;image.width=index?128:40;image.height=index?80:40;image.format=0;image.imageHeader=header;image.data=f.texture.word(header+8);image.minFilter=image.magFilter=1;BattleClassSurfaceTextureObject out;out.handle=o.object;out.image=image;out.bytes=words;out.lifetime=f.life;out.user=p;out.current=[](void*p){return static_cast<Fixture*>(p)->objectLive;};
  out.witness=[](void*p,const BattleClassSurfaceTextureObject&w){auto&f=*static_cast<Fixture*>(p);const auto index=w.image.index;if(!f.objectLive||index>1)return false;const auto&o=*f.objects[f.reuseOutput?0:index];if(o.index!=index)return false;const auto words=o.output.words();const auto header=f.texture.word(f.texture.word(0x88000008)+8*index);return w.handle==o.object&&w.bytes&&words&&*w.bytes==*words&&w.image.imageHeader==header&&w.image.data==f.texture.word(header+8)&&w.image.width==(index?128:40)&&w.image.height==(index?80:40)&&w.image.format==0;};return out;};
  sink.user=this;sink.append=[](void*p,BattleClassSurfacePacketPtr packet){auto&f=*static_cast<Fixture*>(p);++f.sinkCalls;if(f.reenter){f.reenter=false;BattleClassRasterRequest q;q.function=BattleClassRasterService::Init2D;assert(!f.raster->request(q));}if(f.sinkHolds){--f.sinkHolds;return false;}if(f.append&&!f.append(packet))return false;f.packets.push_back(std::move(packet));return true;};
  provider=std::make_unique<BattleClassSurfacePacketProvider>(resource,camera,query,sink);
  source.user=this;source.lifetime=life;source.current=query.current;
  source.word=[](void*p,uint32_t at)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.wait())return {};const auto value=f.texture.get(at,4);if(value)f.trace.push_back(at);return value;};
  source.request=[](void*p,const BattleTexturePlaneRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.wait())return {};const auto value=f.provider->request(q);if(value)f.trace.push_back(q.kind==BattleTexturePlaneKind::Call?uint32_t(q.function):0xcc008000);return value;};
  raster=std::make_unique<BattleClassSurfaceRaster>(source,*provider);
  memory[0x8041eb90]=B;memory[B+0x163f8]=0x50000;memory[0x500a0]=0x51000;memory[0x51000]=0x88000000;memory[R+4]=1;memory[R+8]=0x1234;memory[R+0xc]=334;memory[W+0x1c]=0;
  for(unsigned i=0;i<12;++i)memory[C+0x11c+4*i]=bits(i%5==0?1:0);
  services.user=this;services.memory.memory.user=this;services.memory.memory.word=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);const auto i=f.memory.find(a);if(i!=f.memory.end())return i->second;const auto v=f.texture.get(a,4);if(!v)std::cerr<<"unknown source word "<<std::hex<<a<<"\n";assert(v);return *v;};services.memory.memory.half=[](void*p,uint32_t a){return uint16_t(static_cast<Fixture*>(p)->memory.at(a));};
  services.request=[](void*p,const BattleClassRasterRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(q.function==BattleClassRasterService::Camera)return C;const auto n=f.raster->request(q);if(!n&&(q.function==BattleClassRasterService::Icon||q.function==BattleClassRasterService::IconColor))++f.blocked;return n;};
  services.truncate=[](void*,float x)->std::optional<int32_t>{return int32_t(x);};services.identity=[](void*)->std::optional<BattleActMatrix>{return identity();};
  services.math.scale=[](void*,float x,float y,float z)->std::optional<BattleActMatrix>{auto m=identity();m[0]=x;m[5]=y;m[10]=z;return m;};services.math.translation=[](void*,const std::array<float,3>&v)->std::optional<BattleActMatrix>{auto m=identity();m[3]=v[0];m[7]=v[1];m[11]=v[2];return m;};
  services.math.rotation=[](void*,float a,uint32_t)->std::optional<BattleActMatrix>{auto m=identity();m[0]=m[5]=std::cos(a);m[1]=-std::sin(a);m[4]=std::sin(a);return m;};services.math.concat=[](void*,const BattleActMatrix&a,const BattleActMatrix&b)->std::optional<BattleActMatrix>{BattleActMatrix out{};for(unsigned r=0;r<3;++r)for(unsigned c=0;c<4;++c){float n=c==3?a[4*r+3]:0;for(unsigned k=0;k<3;++k)n+=a[4*r+k]*b[4*k+c];out[4*r+c]=n;}return out;};
 }
 ~Fixture(){raster.reset();provider.reset();resource.retire();std::filesystem::remove_all(dir);}
 static BattleActMatrix identity(){BattleActMatrix m{};m[0]=m[5]=m[10]=1;return m;}
 bool wait(){return trace.size()==holdAt&&waits++<2;}
 bool circleOnce(){return resumePackedBattleClassCircle(R,base,{0,0,0},0,services,circle);}
 void reachIcon(){for(unsigned i=0;i<10&&!blocked;++i)assert(!circleOnce());if(!blocked||packets.size()!=8)std::cerr<<"pending "<<std::hex<<raster->pending()<<" packets "<<std::dec<<packets.size()<<" source "<<trace.size()<<"\n";assert(blocked&&packets.size()==8);}
};
void actualCircle(){Fixture f;f.reachIcon();assert(f.trace.size()>300);for(const auto&p:f.packets){assert(validateBattleClassSurfacePacket(*p,f.resource));assert(p->object.handle==0x90000000&&p->object.image.index==0&&p->rgb==BattleClassSurfaceRGB::MaterialRaster);assert(p->material==0xe1e1e1ff);}const auto trace=f.trace;const auto first=f.packets[0];assert(first->rgba[0]==0x00000080);assert(f.packets[4]->rgba[0]==0xc0c0c0ff);
 // Repeated real missing icon never replays the eight successful surface quads.
 for(unsigned i=0;i<3;++i){assert(!f.circleOnce());}assert(f.trace==trace&&f.packets.size()==8);
 for(size_t at=0;at<trace.size();++at){Fixture held;held.holdAt=at;held.reachIcon();assert(held.trace==trace&&held.waits==3);}
 Fixture reference;const auto base=Fixture::identity();assert(!drawPackedBattleClassCircle(Fixture::R,base,{0,0,0},0,reference.services));assert(reference.blocked&&reference.trace==trace&&reference.packets.size()==8);
 Fixture tint;tint.memory[0x8042240c]=0x12345600;tint.reachIcon();assert(tint.packets[0]->material==0x12345600&&tint.packets[0]->rgba[0]==0x00000080);
 Fixture held;held.sinkHolds=2;held.reenter=true;held.reachIcon();assert(held.sinkCalls==10);const auto before=held.packets;held.objectLive=false;assert(validateBattleClassSurfacePacket(*before[0],held.resource));held.objectLive=true;held.resource.retire();assert(!validateBattleClassSurfacePacket(*before[0],held.resource));
 auto invalid=*first;invalid.object.handle=1;assert(!validateBattleClassSurfacePacket(invalid,f.resource));invalid=*first;(*invalid.object.bytes)[0]^=1;assert(!validateBattleClassSurfacePacket(invalid,f.resource));invalid=*first;invalid.object.witness=nullptr;assert(!validateBattleClassSurfacePacket(invalid,f.resource));invalid=*first;invalid.rgba.fill(0);invalid.matrix.fill(0);assert(validateBattleClassSurfacePacket(invalid,f.resource));invalid=*first;invalid.matrix[0]=std::numeric_limits<float>::max();invalid.positions[0][0]=2;assert(!validateBattleClassSurfacePacket(invalid,f.resource));invalid=*first;invalid.camera.projection[0][0]=std::numeric_limits<float>::max();invalid.positions[0][0]=2;assert(!validateBattleClassSurfacePacket(invalid,f.resource));
}
void actualHeaderQuery(){
 Fixture f;BattleSubmenuMemoryReader reader{&f,[](void*p,uint32_t at)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);const auto i=f.memory.find(at);if(i!=f.memory.end())return i->second;return f.texture.get(at,4);}};
 for(unsigned i=0;i<2;++i){const auto image=readBattleClassSurfaceTexture(reader,0x88000000,i);assert(image&&image->format==0&&image->index==i&&image->width==(i?128:40));}
 assert(!readBattleClassSurfaceTexture(reader,0x88000000,2));assert(!readBattleClassSurfaceTexture(reader,0x88000004,0));
 const auto header=f.texture.word(f.texture.word(0x88000008));f.texture.memory.erase(header+20);assert(!readBattleClassSurfaceTexture(reader,0x88000000,0));
}

void actualTemporaryReuse(){
 Fixture f;f.reuseOutput=true;f.reachIcon();const auto circle=f.packets.front();const auto circleWords=circle->object.bytes;assert(circleWords&&circle->object.handle==0x90000000);
 // Separate isolated original leaf invocation: genuine second TEX/GX body
 // overwrites SAME actual32B private output while earlier loaded data is held.
 // No opaque Icon ack or whole MainIcon/Weapon completion is asserted.
 assert(!resumePackedBattleClassBubble(Fixture::W,Fixture::R,f.base,0,f.services,f.bubble));assert(f.packets.size()==10&&f.packets.back()->object.handle==circle->object.handle&&f.packets.back()->object.image.index==1&&f.packets.back()->object.bytes!=circleWords);
 for(const auto&p:f.packets)assert(validateBattleClassSurfacePacket(*p,f.resource));
 f.objects[0]->output.retire();for(const auto&p:f.packets)assert(validateBattleClassSurfacePacket(*p,f.resource));
 f.live=false;for(const auto&p:f.packets)assert(!validateBattleClassSurfacePacket(*p,f.resource));
}
void actualBubbleAndFrontiers(){
 Fixture bubble;const auto base=Fixture::identity();assert(!resumePackedBattleClassBubble(Fixture::W,Fixture::R,base,0,bubble.services,bubble.bubble));assert(bubble.blocked&&bubble.packets.size()==2);
 for(const auto&p:bubble.packets){assert(p->object.image.index==1&&p->object.handle==0x90000100&&validateBattleClassSurfacePacket(*p,bubble.resource));}
 Fixture gx;gx.ordinary=false;assert(!gx.circleOnce()&&gx.packets.empty()&&gx.trace.empty());gx.ordinary=true;gx.reachIcon();
 Fixture lost;lost.holdAt=1;assert(!lost.circleOnce()&&lost.trace.size()==1);lost.holdAt=size_t(-1);lost.ordinary=false;assert(!lost.circleOnce());lost.ordinary=true;assert(!lost.circleOnce()&&lost.packets.empty()&&lost.trace.size()==1);
 Fixture missing;missing.objects[0]->output.retire();assert(!missing.circleOnce()&&missing.raster->pending()==0x80037a80&&missing.packets.empty());const auto prefix=missing.trace;assert(!missing.circleOnce()&&missing.trace==prefix);
}
// The following TEST parent is a typed publication/geometry harness. It does
// not claim original WeaponFrame completion, original glyph RNG, or native GPU
// submission. The class prefix above executes original source/TEX/GX children.
void orderedAndPrepared(){
 Fixture f;BattleFontGpuResource font;BattleSubmenuSurfaceGpuResource submenu;
 MessageFont originalFont;assert(originalFont.load("romfs/font.pmfont"));assert(font.adopt(originalFont,{&font,512,256,0,1,512,256,false,{0,1,1,0}}));
 auto*file=std::fopen((f.dir/"battle-submenu-surfaces.bin").c_str(),"wb");assert(file);uint32_t count=2;std::fwrite("PMBSUB01",8,1,file);std::fwrite(&count,4,1,file);for(unsigned i=0;i<2;++i){uint32_t index=i+2;unsigned x=i?22:2,w=i?8:16;BattleClassRegion r{w,24,float(x)/32,1-2.f/32,float(x+w)/32,1-26.f/32};std::fwrite(&index,4,1,file);std::fwrite(&r,sizeof(r),1,file);}std::fclose(file);
 BattleClassSurfaceTextureServices gpu;gpu.user=&submenu;gpu.import=[](void*p,uint32_t,const char*)->std::optional<BattleClassSurfaceImport>{return BattleClassSurfaceImport{p,32,32,0,1,32,32,false,{0,1,1,0}};};gpu.release=[](void*,uint32_t,const void*){};assert(submenu.load(f.dir.string().c_str(),gpu));
 struct Context{Fixture*f;bool testComplete=false,ready=false;unsigned draws=0;std::vector<size_t>order;std::shared_ptr<const uint8_t>allocation=std::make_shared<const uint8_t>(1),icons=std::make_shared<const uint8_t>(2);};Context c{};c.f=&f;
 BattleWeaponOrderedPublication ordered;BattleWeaponOrderedBinding binding;binding.allocationOwner=c.allocation;binding.sourceResourceOwner=f.life;binding.resources=f.life;binding.allocation=Fixture::W;binding.generation=9;binding.capacity=16;binding.user=&c;binding.font=&font;binding.submenu=&submenu;binding.classes=&f.resource;
 binding.current=[](void*p,uint32_t w,uint64_t g){return static_cast<Context*>(p)->f->live&&w==Fixture::W&&g==9;};binding.sourceCurrent=[](void*p,uint64_t pass){auto&c=*static_cast<Context*>(p);return c.f->live&&c.f->executing&&pass==7;};binding.sourceComplete=[](void*p,uint64_t){return static_cast<Context*>(p)->testComplete;};binding.icon=[](void*p,uint32_t image)->std::optional<BattleEnemyHpIconResidency>{auto&c=*static_cast<Context*>(p);return BattleEnemyHpIconResidency{p,c.icons,image,16,20,32,32,0};};binding.ready=[](void*p,const BattleWeaponOrderedBatch&){return static_cast<Context*>(p)->ready;};binding.draw=[](void*p,const BattleWeaponOrderedBatch&b){auto&c=*static_cast<Context*>(p);++c.draws;for(const auto&q:b.packets)c.order.push_back(q.index());};
 assert(ordered.bind(binding)&&ordered.begin(7));f.append=[&](BattleClassSurfacePacketPtr p){return ordered.appendClassSurface(7,std::move(p));};
 BattleFontGlyphPacket glyph;glyph.font=font.lease();glyph.matrix=Fixture::identity();glyph.quad.right=8;glyph.quad.bottom=20;glyph.quad.u1=8;glyph.quad.v1=20;glyph.quad.rgba=0x12345600;glyph.raster.viewport=glyph.raster.scissor={0,0,240,400};for(unsigned i=0;i<4;++i)glyph.raster.projection[i][i]=1;
 assert(ordered.reserveFont(7,glyph));assert(!f.circleOnce()&&f.provider->partial()&&f.packets.empty());const auto prefix=f.trace;assert(!f.circleOnce()&&f.trace==prefix);ordered.commitFont(7,glyph);f.reachIcon();assert(!ordered.seal(7));
 assert(ordered.reserveFont(7,glyph));ordered.commitFont(7,glyph);
 RetainedIconPacket icon;icon.image=334;icon.camera=8;icon.viewport=icon.scissor={0,0,240,400};for(unsigned i=0;i<4;++i)icon.transform[i][i]=1;assert(ordered.appendIcon(7,icon));
 c.testComplete=true;const auto batch=ordered.seal(7);assert(batch&&batch->packets.size()==11);assert(batch->packets[0].index()==0&&batch->packets[1].index()==4&&batch->packets[8].index()==4&&batch->packets[9].index()==0&&batch->packets[10].index()==2);
 const BattleWeaponRasterResources resources{&font,&submenu,nullptr,&f.resource};const auto plan=prepareBattleWeaponRasterGeometry(&c,batch,resources);assert(plan&&plan->primitives().size()==11);
 const auto&first=plan->primitives()[1];assert(first.kind==BattleWeaponRasterKind::ClassSurface&&first.texture==f.resource.lease().textures[0]&&first.classRGB==BattleClassSurfaceRGB::MaterialRaster&&first.classMaterial==0xe1e1e1ff&&first.vertices[0].rgba==0x00000080); // C0 alpha never pre-multiplies raster alpha
 const auto originalTrace=f.trace;assert(!ordered.submit(7,batch)&&!ordered.submit(7,batch)&&!c.draws);c.ready=true;assert(ordered.submit(7,batch)&&ordered.submit(7,batch)&&c.draws==1&&c.order.size()==11);assert(f.trace==originalTrace);
 f.executing=false;assert(ordered.validate(batch)&&plan->current(&c,resources));f.objectLive=false;assert(ordered.validate(batch)&&plan->current(&c,resources));f.live=false;assert(!ordered.validate(batch)&&!plan->current(&c,resources));
 // Absence is a genuine hold before native plan allocation, never a selector
 // alias to the submenu atlas. Source and resident lifetime failures differ.
 Fixture absent;absent.reachIcon();auto manual=std::make_shared<BattleWeaponOrderedBatch>();manual->allocationOwner=c.allocation;manual->sourceResourceOwner=absent.life;manual->allocation=Fixture::W;manual->generation=9;manual->pass=7;manual->packets.push_back(absent.packets[0]);auto missing=resources;missing.classes=nullptr;assert(!prepareBattleWeaponRasterGeometry(&c,manual,missing));
 auto present=resources;present.classes=&absent.resource;const auto valid=prepareBattleWeaponRasterGeometry(&c,manual,present);assert(valid);absent.resource.retire();assert(!valid->current(&c,present));
}

}
int main(){actualCircle();actualHeaderQuery();actualTemporaryReuse();actualBubbleAndFrontiers();orderedAndPrepared();std::cout<<"actual class surface source packet tests PASS\n";return 0;}

#include "battle_submenu_packets.hpp"
#include "source_gx_setter_routing.hpp"
#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <map>
#include <limits>
using namespace ttyd;
static uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
struct Fixture {
 std::string root;std::filesystem::path dir;BattleSubmenuSurfaceGpuResource resource;
 std::shared_ptr<int> lifetime=std::make_shared<int>(1);bool live=true,executing=true,resourceLive=true,recurse=false,unknownObject=false,reuseHandle=false;int holds=0,calls=0;bool sinkReenter=false;
 BattleSubmenuCameraProof camera;BattleSubmenuQueryServices query;BattleSubmenuPacketSink sink;
 std::unique_ptr<BattleSubmenuPacketProvider> provider;BattleSubmenuSourceServices source;BattleSubmenuSourceCursor init,quad;
 std::vector<BattleSubmenuPacketPtr> packets;std::vector<uint32_t> trace;int holdAt=-1,waits=0;
 std::map<uint32_t,uint32_t> words{{0x80422240,bits(0)},{0x8042224c,bits(1)}};
 std::shared_ptr<SourceGxInheritedState>gx;std::unique_ptr<SourceGxSetterRouting>gxRoute;unsigned captures=0;bool captureHold=false;
 Fixture(bool withGx=false){
 dir=std::filesystem::temp_directory_path()/("pm-submenu-packets-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));std::filesystem::create_directory(dir);root=dir.string();
 FILE*f=std::fopen((root+"/battle-submenu-surfaces.bin").c_str(),"wb");assert(f);uint32_t count=2;std::fwrite("PMBSUB01",8,1,f);std::fwrite(&count,4,1,f);for(unsigned i=0;i<2;++i){uint32_t id=i+2;unsigned x=i?22:2,w=i?8:16;BattleClassRegion r{w,24,float(x)/32,1-2.f/32,float(x+w)/32,1-26.f/32};std::fwrite(&id,4,1,f);std::fwrite(&r,sizeof(r),1,f);}std::fclose(f);
 BattleClassSurfaceTextureServices gpu;gpu.user=this;gpu.import=[](void*p,uint32_t,const char*)->std::optional<BattleClassSurfaceImport>{return BattleClassSurfaceImport{p,32,32,0,1,32,32,false,{0,1,1,0}};};gpu.release=[](void*,uint32_t,const void*){};assert(resource.load(root.c_str(),gpu));
 camera.lifetime=lifetime;camera.user=this;camera.current=[](void*p,const BattleSubmenuCameraProof&v){return static_cast<Fixture*>(p)->live&&v.pass==1&&v.generation==4;};camera.pass=1;camera.generation=4;camera.camera=8;camera.viewport={0,0,608,480,0,1};camera.scissor={0,0,608,480};for(unsigned i=0;i<4;++i)camera.projection[i][i]=1;
 query.user=this;query.lifetime=lifetime;query.current=[](void*p){const auto&f=*static_cast<Fixture*>(p);return f.live&&f.executing&&(!f.gxRoute||f.gxRoute->compatibleNow());};query.sourceResourceCurrent=[](void*p){return static_cast<Fixture*>(p)->resourceLive;};query.camera=[](void*,std::optional<uint32_t>)->std::optional<uint32_t>{return 0x1000;};query.texture=[](void*p,uint32_t palette,uint32_t i)->std::optional<BattleSubmenuTextureObject>{if(palette!=0x8000||i<2||i>3)return {};AnimationTPLImage r;r.index=i;r.width=i==2?16:8;r.height=24;r.format=14;r.data=0x8400;r.imageHeader=0x8100;r.minFilter=r.magFilter=1;auto&f=*static_cast<Fixture*>(p);BattleSubmenuTextureObject object;object.handle=f.reuseHandle?0x5000:0x5000+i*0x100;object.image=r;object.bytes=std::array<uint32_t,8>{0x1234,0,(23u<<10)|(uint32_t(r.width)-1),0,0,14,0,0};if(f.unknownObject)object.bytes.reset();object.lifetime=f.lifetime;object.user=p;object.current=[](void*p){return static_cast<Fixture*>(p)->resourceLive;};return object;};
 sink.user=this;sink.append=[](void*p,BattleSubmenuPacketPtr packet){auto&f=*static_cast<Fixture*>(p);++f.calls;if(f.sinkReenter){f.sinkReenter=false;BattleTexturePlaneRequest q;q.function=BattleTexturePlaneCall::Begin;q.integers={128,0,4};assert(!f.provider->request(q));}if(f.holds>0){--f.holds;return false;}f.packets.push_back(std::move(packet));return true;};
 if(withGx){gx=std::make_shared<SourceGxInheritedState>();assert(gx->bind({lifetime,this,[](void*p){return static_cast<Fixture*>(p)->live;},[](void*){return true;}})&&gx->initializeCold());gxRoute=std::make_unique<SourceGxSetterRouting>(gx,lifetime,[this]{return live&&executing;});query.ordinaryCapture=[](void*p)->std::shared_ptr<const SourceGxInheritedSnapshot>{auto&f=*static_cast<Fixture*>(p);++f.captures;return f.captureHold?nullptr:f.gxRoute->capture();};}
 provider=std::make_unique<BattleSubmenuPacketProvider>(resource,camera,query,sink);
 source.user=this;source.lifetime=lifetime;source.current=query.current;source.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.wait())return {};auto it=f.words.find(a);if(it==f.words.end())return {};f.trace.push_back(a);return it->second;};
 source.request=[](void*p,const BattleTexturePlaneRequest&q)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);if(f.recurse){f.recurse=false;assert(!resumeBattleSubmenuInit(f.source,f.init));assert(!f.init.reset());}if(f.wait())return {};auto v=f.provider->request(q);if(v){f.trace.push_back(q.kind==BattleTexturePlaneKind::Call?uint32_t(q.function):0xcc008000);if(f.gxRoute)assert(f.gxRoute->accepted(q)!=SourceGxObserved::Unavailable);}return v;};
 }
 ~Fixture(){provider.reset();resource.retire();std::filesystem::remove_all(dir);}
 bool wait(){return holdAt==int(trace.size())&&waits++<2;}
 void initialize(){for(unsigned i=0;i<5;++i)if(resumeBattleSubmenuInit(source,init))return;assert(false);}
 void select(uint32_t index){BattleTexturePlaneRequest q;q.function=BattleTexturePlaneCall::LoadMatrix;q.integers={0};q.matrix=BattleActMatrix{2,0,0,5,0,3,0,-7,0,0,1,9};assert(provider->request(q));q={};q.function=BattleTexturePlaneCall::TextureObject;q.integers={0x8000,index};auto v=provider->request(q);assert(v&&*v==(reuseHandle?0x5000:0x5000+index*0x100));q.function=BattleTexturePlaneCall::LoadTexture;q.integers={*v,0};assert(provider->request(q));}
 bool once(){return resumeBattleSubmenuQuad({-8,12,8,-12},{0x123,2,3,128},source,quad);}
 void draw(){for(unsigned i=0;i<5;++i)if(once())return;assert(false);}
};
void perLoadOrdinary(){
 Fixture f(true);f.initialize();BattleTexturePlaneRequest q;q.function=BattleTexturePlaneCall::LoadMatrix;q.integers={0};q.matrix=BattleActMatrix{2,0,0,5,0,3,0,-7,0,0,1,9};assert(f.provider->request(q));q={};q.function=BattleTexturePlaneCall::TextureObject;q.integers={0x8000,2};const auto object=f.provider->request(q);assert(object);q.function=BattleTexturePlaneCall::LoadTexture;q.integers={*object,0};f.captureHold=true;assert(!f.provider->request(q)&&!f.provider->request(q)&&f.captures==2&&f.packets.empty());f.captureHold=false;assert(f.provider->request(q)&&f.captures==3);f.draw();assert(f.packets.size()==1);const auto p=f.packets[0];assert(p->ordinarySnapshot&&validateBattleSubmenuPacket(*p,f.resource));
 SourceGxInheritedCall swap;swap.function=0x802b8c98;swap.count=3;swap.integers={0,0,1};assert(f.gxRoute->accepted(swap)==SourceGxObserved::Recorded);assert(!f.gxRoute->capture()&&validateBattleSubmenuPacket(*p,f.resource));f.executing=false;assert(validateBattleSubmenuPacket(*p,f.resource));f.gx->retire();assert(!validateBattleSubmenuPacket(*p,f.resource));
}
int main(){
 perLoadOrdinary();
 Fixture base;base.initialize();base.select(2);base.draw();assert(base.packets.size()==1);const auto p=base.packets.front();assert(validateBattleSubmenuBatch(base.packets,base.resource));assert((p->positions[0]==std::array<float,3>{-8,12,0}));assert(p->rgba[0]==0x23020380);assert((p->sourceUV[2]==std::array<float,2>{1,1}));const auto vertices=battleSubmenuVertices(*p);assert(vertices&&(*vertices)[0].x==-11&&(*vertices)[0].y==29&&(*vertices)[0].z==9);const auto trace=base.trace;
 for(unsigned i=0;i<trace.size();++i){Fixture f;f.holdAt=int(i);f.initialize();f.select(2);f.draw();assert(f.trace==trace&&f.waits==3&&f.packets.size()==1);}
 Fixture unknown;unknown.initialize();unknown.unknownObject=true;BattleTexturePlaneRequest absent;absent.function=BattleTexturePlaneCall::TextureObject;absent.integers={0x8000,2};assert(!unknown.provider->request(absent));
 Fixture reuse;reuse.reuseHandle=true;reuse.initialize();reuse.select(2);reuse.draw();reuse.select(3);reuse.draw();assert(reuse.packets.size()==2&&reuse.packets[0]->originalTexture.index==2&&reuse.packets[1]->originalTexture.index==3);assert(validateBattleSubmenuBatch(reuse.packets,reuse.resource));
 Fixture held;held.initialize();held.select(3);held.holds=2;held.sinkReenter=true;assert(!held.once());const auto before=held.trace;assert(!held.once());assert(held.trace==before);assert(held.once());assert(held.calls==3&&held.packets.size()==1&&held.packets[0]->originalTexture.index==3);
 Fixture late;late.initialize();late.select(2);late.holdAt=int(late.trace.size()+4);assert(!late.once());late.words[0x80422240]=bits(.25f);late.draw();assert(late.packets[0]->positions[0][2]==.25f);assert(late.packets[0]->sourceUV[0][0]==0);

 // Feed the COMPLETE existing retained TexturePlane2 into the same sink.
 Fixture plane;plane.initialize();plane.words[0x8041eb90]=0x2000;plane.words[0x2000+0x163f8]=0x3000;plane.words[0x3000+0xa0]=0x4000;plane.words[0x4000]=0x8000;
 plane.words[0x80422234]=bits(.5f);plane.words[0x802ee400]=0x43300000;plane.words[0x802ee404]=0;
 for(unsigned i=0;i<12;++i)plane.words[0x1000+0x11c+4*i]=bits(i%5==0?1.f:0.f);
 BattleTexturePlaneServices ts;ts.user=&plane;ts.actors.memory.user=&plane;ts.actors.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->words.at(a);};ts.request=plane.source.request;
 ts.matrix=[](void*,BattleAudienceMatrixRequest&q){if(q.function==0x8029dd30){q.output=q.left;return true;}assert(q.function==0x8029dd64);for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col){float value=col==3?q.left[4*row+3]:0;for(unsigned k=0;k<3;++k)value+=q.left[4*row+k]*q.right[4*k+col];q.output[4*row+col]=value;}return true;};
 BattleTexturePlaneCursor tc;const BattleActMatrix transform{1,0,0,4,0,1,0,5,0,0,1,6};const uint32_t rgba=0x11223344;plane.holds=2;
 assert(!resumePackedBattleTexturePlane2(transform,2,rgba,ts,tc));const auto planePrefix=plane.trace;assert(!resumePackedBattleTexturePlane2(transform,2,rgba,ts,tc));assert(plane.trace==planePrefix);assert(resumePackedBattleTexturePlane2(transform,2,rgba,ts,tc));assert(plane.packets.size()==1);assert((plane.packets[0]->positions[0]==std::array<float,3>{-8,12,0}));assert(plane.packets[0]->rgba[3]==rgba);assert(validateBattleSubmenuBatch(plane.packets,plane.resource));
 // Actual relocated packed common-bank chain, not a guessed index lookup.
 std::map<uint32_t,uint32_t> memory{{0x8041eb90,0x1000},{0x1000+0x163f8,0x2000},{0x2000+0xa0,0x3000},{0x3000,0x10000},{0x10000,0x0020af30},{0x10004,97},{0x10008,0x1000c},{0x1000c+16,0x1035c},{0x1000c+20,0},{0x1035c,(24u<<16)|16},{0x10360,14},{0x10364,0x11000},{0x10368,0},{0x1036c,0},{0x10370,1},{0x10374,1},{0x10378,bits(0)},{0x1037c,1}};
 BattleSubmenuMemoryReader reader{&memory,[](void*p,uint32_t a)->std::optional<uint32_t>{const auto&m=*static_cast<std::map<uint32_t,uint32_t>*>(p);auto i=m.find(a);return i==m.end()?std::nullopt:std::optional<uint32_t>(i->second);}};
 auto image=readBattleSubmenuTexture(reader,0x10000,2);assert(image&&image->width==16&&image->format==14);assert(!readBattleSubmenuTexture(reader,0x10001,2));assert(!readBattleSubmenuTexture(reader,0x10000,4));memory.erase(0x10370);assert(!readBattleSubmenuTexture(reader,0x10000,2));

 Fixture bad;BattleTexturePlaneRequest q;q.function=BattleTexturePlaneCall::Begin;q.integers={128,0,4};assert(!bad.provider->request(q));q.function=BattleTexturePlaneCall::Cull;q.integers={2};assert(!bad.provider->request(q));bad.recurse=true;bad.initialize();bad.select(2);bad.draw();
 auto copied=*bad.packets[0];copied.sourceUV[0][0]=2;assert(!validateBattleSubmenuPacket(copied,bad.resource));copied=*bad.packets[0];copied.matrix[0]=std::numeric_limits<float>::infinity();assert(!validateBattleSubmenuPacket(copied,bad.resource));copied=*bad.packets[0];copied.camera.pass=2;assert(!validateBattleSubmenuPacket(copied,bad.resource));copied=*bad.packets[0];copied.camera.viewport[2]=100;assert(!validateBattleSubmenuPacket(copied,bad.resource));
 auto other=std::make_shared<BattleSubmenuPacket>(*bad.packets[0]);other->camera.projection[0][0]=2;assert(validateBattleSubmenuPacket(*other,bad.resource));assert(!validateBattleSubmenuBatch({bad.packets[0],other},bad.resource));
 bad.resourceLive=false;assert(!validateBattleSubmenuBatch(bad.packets,bad.resource));bad.resourceLive=true;
 base.executing=false;assert(validateBattleSubmenuBatch(base.packets,base.resource));
 bad.live=false;assert(!validateBattleSubmenuBatch(bad.packets,bad.resource));
 // The actual bank identity and pinned callback holder need not be the same
 // allocation. A published packet keeps its userdata alive after the active
 // producer/provider is retired; actual bank retirement still rejects it.
 {Fixture f;
  struct Proof {std::shared_ptr<bool>bank;BattleSubmenuTextureObject object;};
  auto bank=std::make_shared<bool>(true);auto owner=std::make_shared<Proof>();owner->bank=bank;owner->object=*f.query.texture(&f,0x8000,2);
  std::weak_ptr<Proof>weak=owner;f.query.user=owner.get();f.query.lifetime=owner;f.query.sourceResourceOwner=bank;
  f.query.current=[](void*p){return *static_cast<Proof*>(p)->bank;};f.query.sourceResourceCurrent=f.query.current;
  f.query.texture=[](void*p,uint32_t palette,uint32_t index)->std::optional<BattleSubmenuTextureObject>{return palette==0x8000&&index==2?std::optional<BattleSubmenuTextureObject>(static_cast<Proof*>(p)->object):std::nullopt;};
  f.provider=std::make_unique<BattleSubmenuPacketProvider>(f.resource,f.camera,f.query,f.sink);f.initialize();f.select(2);f.draw();
  const auto packet=f.packets.front();assert(packet->sourceResourceOwner==bank&&packet->sourceProofOwner==owner);
  f.provider.reset();f.query={};owner.reset();assert(!weak.expired()&&validateBattleSubmenuPacket(*packet,f.resource));
  *bank=false;assert(!validateBattleSubmenuPacket(*packet,f.resource));
 }
 base.resource.retire();assert(!validateBattleSubmenuBatch(base.packets,base.resource));
 return 0;
}

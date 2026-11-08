#include "platform.hpp"
#include "native_harbor_star_point_presentation.hpp"
#include <cmath>
namespace ttyd {
namespace {
constexpr unsigned VerticesPerParticle=6;
bool sameIcon(const MessageIcon&a,const MessageIcon&b){
 if(a.id!=b.id||a.loop!=b.loop||a.frames.size()!=b.frames.size())return false;
 for(size_t i=0;i<a.frames.size();++i){const auto&x=a.frames[i];const auto&y=b.frames[i];
  if(x.delay!=y.delay||x.width!=y.width||x.height!=y.height||x.u0!=y.u0||x.v0!=y.v0||x.u1!=y.u1||x.v1!=y.v1)return false;
 }return true;
}
bool sameSource(const NativeHarborStarPointSource&a,const NativeHarborStarPointSource&b){
 return a.user==b.user&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.current==b.current&&a.randomRaw==b.randomRaw&&a.math==b.math;
}
bool sameRequest(const NativeHarborStarPointRequest&a,const NativeHarborStarPointRequest&b){return a.subtype==b.subtype&&a.amount==b.amount&&a.position==b.position;}
}
struct Renderer::NativeHarborStarPoint:std::enable_shared_from_this<NativeHarborStarPoint>{
 Renderer*renderer=nullptr;bool alive=true,invoking=false;
 std::weak_ptr<const void>runtime;
 const HarborTutorial*facade=nullptr;NativeFacadeRegistration::Proof facadeProof;
 struct Epoch{bool resident=true;};std::shared_ptr<Epoch>epoch=std::make_shared<Epoch>();
 Tex3DS_Texture imported=nullptr;const void*pixels=nullptr;MessageIcon icon;uint32_t rgba=0;
 std::unique_ptr<NativeHarborStarPointVisual>visual;
 struct Slot{Vertex*vertices=nullptr;unsigned capacity=0,count=0;uint64_t id=0;};
 std::array<Slot,NativeHarborStarPointVisual::Capacity>slots{};
 struct Pending{size_t slot;NativeHarborStarPointRequest request;NativeHarborStarPointSource source;};
 std::optional<Pending>pending;std::optional<NativeHarborStarPointGeometry>geometry;uint64_t geometryTick=0;
 bool current()const{
  if(!alive||!epoch->resident||!facadeProof.current()||!facade||!renderer||runtime.expired())return false;
  const auto captured=runtime.lock(),actualRuntime=facade->nativeGameplayLifetime().lock();
  if(!facade->nativeGameplayFacadeProof().current()||!captured||captured!=actualRuntime||captured.owner_before(actualRuntime)||actualRuntime.owner_before(captured))return false;
  const auto&r=*renderer;const auto*actual=messageIcon(NativeHarborStarPointVisual::IconId);
  return r.c3dReady&&r.c2dReady&&r.programReady&&r.transformLoc>=0&&r.tutorialReady&&
   r.messageIconTexture.imported==imported&&r.messageIconTexture.texture.data==pixels&&pixels&&actual&&sameIcon(*actual,icon)&&
   validNativeHarborStarPointIcon(icon,r.messageIconTexture.texture.width,r.messageIconTexture.texture.height);
 }
 std::optional<uint64_t>spawn(const NativeHarborStarPointRequest&q,const NativeHarborStarPointSource&s){
  if(invoking||!current()||renderer->harborSourceFrameActive||!s.lifetime||!s.current||!s.randomRaw||!s.math)return {};
  invoking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{invoking};
  if(!s.current(s.user)||!current())return {};
  if(pending){if(!sameRequest(q,pending->request)||!sameSource(s,pending->source))return {};}
  else {
   if(q.subtype!=0||q.amount<0||size_t(q.amount)>NativeHarborStarPointVisual::MaximumParticles)return {};
   for(float value:q.position)if(!std::isfinite(value))return {};
   size_t index=0;while(index<slots.size()&&visual->instances()[index].id)++index;if(index==slots.size())return {};
   auto&slot=slots[index];const auto required=unsigned(q.amount)*VerticesPerParticle;
   if(required>slot.capacity){
    // Only an inactive slot is replaced. Its preceding GPU use must finish;
    // other active effects retain their own allocations on a failed growth.
    C3D_FrameSync();if(slot.vertices)linearFree(slot.vertices);slot={};
    if(required){slot.vertices=static_cast<Vertex*>(linearAlloc(sizeof(Vertex)*required));if(!slot.vertices)return {};slot.capacity=required;}
   }
   pending=Pending{index,q,s};
  }
  // Resource/buffer admission is complete BEFORE the first original RNG call.
  const auto id=visual->spawn(q,s);if(!id||!current())return {};
  const auto index=pending->slot;if(visual->instances()[index].id!=*id)return {};
  slots[index].id=*id;slots[index].count=0;pending.reset();return id;
 }
 bool advance(uint64_t visit){
  if(invoking||!current()||renderer->harborSourceFrameActive)return false;
  invoking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{invoking};
  return visual->advance(visit)&&current();
 }
};
bool Renderer::prepareNativeHarborStarPoint(Game&g){
 if(!tutorialReady||!g.harborTutorial.running()||!c3dReady||!programReady||!messageIconTexture.imported)return false;
 const auto runtime=g.harborTutorial.nativeGameplayLifetime().lock();const auto proof=g.harborTutorial.nativeGameplayFacadeProof();if(!runtime||!proof.current())return false;
 if(nativeHarborStarPoint){const auto old=nativeHarborStarPoint->runtime.lock();if(old!=runtime||old.owner_before(runtime)||runtime.owner_before(old))retireNativeHarborStarPoint();}
 if(!nativeHarborStarPoint){
  const auto*icon=messageIcon(NativeHarborStarPointVisual::IconId);
  if(!icon||!validNativeHarborStarPointIcon(*icon,messageIconTexture.texture.width,messageIconTexture.texture.height))return false;
  // Ordinary source iconDispGx reads this actual initialized DATA color.
  constexpr uint32_t SourceDefaultIconColor=0x8041f864;uint32_t rgba=0;
  if(!g.harborTutorial.readPackedWord(SourceDefaultIconColor,rgba)||(rgba&255)!=255)return false;
  NativeHarborStarPointData data;if(!data.load("romfs:/harbor-star-point/memory.pmevt"))return false;
  auto c=std::make_shared<NativeHarborStarPoint>();c->renderer=this;c->runtime=runtime;c->facade=&g.harborTutorial;c->facadeProof=proof;c->imported=messageIconTexture.imported;c->pixels=messageIconTexture.texture.data;c->icon=*icon;c->rgba=rgba;
  const std::weak_ptr<NativeHarborStarPoint>weak=c;
  c->visual=std::make_unique<NativeHarborStarPointVisual>(data,c->epoch,[weak]{const auto p=weak.lock();return p&&p->current();});
  NativeHarborStarPointConsumer consumer;consumer.user=c.get();consumer.lifetime=c;
  consumer.current=[](void*p){return static_cast<NativeHarborStarPoint*>(p)->current();};
  consumer.spawn=[](void*p,const NativeHarborStarPointRequest&q,const NativeHarborStarPointSource&s){return static_cast<NativeHarborStarPoint*>(p)->spawn(q,s);};
  consumer.advance=[](void*p,uint64_t tick){return static_cast<NativeHarborStarPoint*>(p)->advance(tick);};
  if(!c->current()||!g.harborTutorial.bindNativeStarPoint(consumer)){c->alive=false;c->epoch->resident=false;c->renderer=nullptr;return false;}
  nativeHarborStarPoint=std::move(c);
 }
 return nativeHarborStarPoint->current();
}
void Renderer::drawNativeHarborStarPoint(const Game&g,bool lower){
 const auto c=nativeHarborStarPoint;if(!c||!c->current())return;
 const auto runtime=g.harborTutorial.nativeGameplayLifetime().lock(),actual=c->runtime.lock();
 if(!runtime||runtime!=actual||runtime.owner_before(actual)||actual.owner_before(runtime))return;
 BattleSceneMatrix matrix;for(unsigned row=0;row<4;++row){const auto&r=worldTransform.r[row];matrix[row]={r.x,r.y,r.z,r.w};}
 // Both views preflight all clip vertices; only the upper view updates buffers.
 const auto geometry=nativeHarborStarPointGeometry(c->visual->instances(),c->icon,g.ticks,c->rgba,matrix);if(!geometry)return;
 if(!lower){
  for(size_t i=0;i<c->slots.size();++i){const auto&slot=c->slots[i];const auto range=geometry->ranges[i];if(range[1]&&(!slot.vertices||range[1]>slot.capacity||slot.id!=c->visual->instances()[i].id))return;}
  for(size_t i=0;i<c->slots.size();++i){auto&slot=c->slots[i];const auto range=geometry->ranges[i];slot.count=range[1];
   for(unsigned n=0;n<range[1];++n){const auto&v=geometry->vertices[range[0]+n];slot.vertices[n]={v.x,v.y,v.z,v.u,v.v,v.color[0],v.color[1],v.color[2],v.color[3]};}
   if(slot.count)GSPGPU_FlushDataCache(slot.vertices,slot.count*sizeof(Vertex));
  }
  c->geometry=*geometry;c->geometryTick=g.ticks;
 }else if(!c->geometry||c->geometryTick!=g.ticks||geometry->ranges!=c->geometry->ranges)return;
 bool any=false;for(const auto&slot:c->slots)any|=slot.count!=0;if(!any)return;
 C2D_Flush();C3D_BindProgram(&program);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
 auto*attributes=C3D_GetAttrInfo();AttrInfo_Init(attributes);AttrInfo_AddLoader(attributes,0,GPU_FLOAT,3);AttrInfo_AddLoader(attributes,1,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,2,GPU_FLOAT,4);
 for(unsigned stage=0;stage<6;++stage)C3D_TexEnvInit(C3D_GetTexEnv(stage));
 auto*tev=C3D_GetTexEnv(0);C3D_TexEnvSrc(tev,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(tev,C3D_Both,GPU_MODULATE);
 C3D_CullFace(GPU_CULL_NONE);C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITE_ALL);C3D_AlphaTest(true,GPU_GEQUAL,128);
 C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_ONE,GPU_ZERO,GPU_ONE,GPU_ZERO);
 C3D_TexBind(0,&messageIconTexture.texture);parkUnusedSamplers();
 for(const auto&slot:c->slots)if(slot.count){auto*buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,slot.vertices,sizeof(Vertex),3,0x210);C3D_DrawArrays(GPU_TRIANGLES,0,slot.count);}
}
bool Renderer::nativeHarborStarPointMatches(const std::shared_ptr<const void>&runtime)const{
 if(!nativeHarborStarPoint)return true;const auto current=nativeHarborStarPoint->runtime.lock();
 return !nativeHarborStarPoint->invoking&&runtime&&current==runtime&&!runtime.owner_before(current)&&!current.owner_before(runtime);
}
void Renderer::retireNativeHarborStarPoint(){
 auto c=std::move(nativeHarborStarPoint);if(!c)return;
 c->alive=false;c->epoch->resident=false;c->visual->retire();
 if(c3dReady)C3D_FrameSync();
 for(auto&slot:c->slots){if(slot.vertices)linearFree(slot.vertices);slot={};}
 c->geometry.reset();c->pending.reset();c->renderer=nullptr;
}
}

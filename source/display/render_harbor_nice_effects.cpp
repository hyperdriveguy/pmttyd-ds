#include "platform.hpp"
#include "native_harbor_nice_visual.hpp"
#include "native_ui_startup.hpp"
#include <cmath>
namespace ttyd {
// A genuine native visual allocation/consumer, separate from original effEntry
// and its camera4 queue. Original A_1 geometry, atlas, scale and fade operands
// feed the tutorial's documented native world-camera presentation policy.
struct Renderer::NativeHarborNice:std::enable_shared_from_this<NativeHarborNice> {
 Renderer*renderer=nullptr;bool alive=true;
 std::weak_ptr<const void>runtime;std::shared_ptr<NativeUiStartup>startup;
 std::unique_ptr<NativeHarborNiceVisual>visual;Vertex*vertices=nullptr;
 std::array<std::array<unsigned,2>,NativeHarborNiceVisual::InstanceCapacity>ranges{};
 bool current()const{
  if(!alive||!renderer||!vertices||runtime.expired()||!startup||!startup->current())return false;
  const auto language=startup->language();if(!language||*language!=1)return false;
  const auto&r=*renderer;
  if(!r.c3dReady||r.transformLoc<0||!r.tutorialStartupResources)return false;
  for(unsigned i=3;i<8;++i){const auto*bank=r.tutorialStartupResources->resource(i);if(!bank||!r.tutorialStartupTextures[i].imported||!r.tutorialStartupResources->resourceReady(bank->sourceKey,1))return false;}
  return true;
 }
};
bool Renderer::prepareNativeHarborNice(Game&g){
 if(!tutorialReady||!g.harborTutorial.running()||!tutorialStartupResources)return false;
 const auto runtime=g.harborTutorial.nativeGameplayLifetime().lock();const auto startup=g.nativeUiStartup();
 if(!runtime||!startup||!startup->current())return false;
 if(nativeHarborNice&&nativeHarborNice->runtime.lock()!=runtime)retireNativeHarborNice();
 if(!nativeHarborNice){
  auto context=std::make_shared<NativeHarborNice>();context->renderer=this;context->runtime=runtime;context->startup=startup;
  constexpr size_t VertexCapacity=NativeHarborNiceVisual::InstanceCapacity*NativeHarborNiceVisual::VerticesPerInstance;
  context->vertices=static_cast<Vertex*>(linearAlloc(VertexCapacity*sizeof(Vertex)));
  if(!context->vertices)return false;
  if(!context->current()){linearFree(context->vertices);context->vertices=nullptr;return false;}
  const std::weak_ptr<NativeHarborNice>weak=context;
  context->visual=std::make_unique<NativeHarborNiceVisual>(*tutorialStartupResources,std::make_shared<const uint8_t>(0),[weak]{auto c=weak.lock();return c&&c->current();});
  NativeHarborNiceConsumer consumer;consumer.user=context.get();consumer.lifetime=context;
  consumer.current=[](void*p){return static_cast<NativeHarborNice*>(p)->current();};
  consumer.spawn=[](void*p,const NativeHarborNiceVisualRequest&q){auto&c=*static_cast<NativeHarborNice*>(p);return c.current()?c.visual->spawn(q):std::optional<uint64_t>{};};
  consumer.scale=[](void*p,uint64_t id,float scale){auto&c=*static_cast<NativeHarborNice*>(p);return c.current()&&c.visual->scale(id,scale);};
  if(!g.harborTutorial.bindNativeNice(startup,consumer)){context->alive=false;linearFree(context->vertices);context->vertices=nullptr;context->renderer=nullptr;return false;}
  nativeHarborNice=std::move(context);
 }
 return nativeHarborNice->current()&&nativeHarborNice->visual->advance(g.harborTutorial.now);
}
void Renderer::drawNativeHarborNice(const Game&g,bool lower){
 const auto context=nativeHarborNice;
 if(!context||!context->current()||context->runtime.lock()!=g.harborTutorial.nativeGameplayLifetime().lock())return;
 auto&c=*context;const auto&instances=c.visual->instances();
 if(!lower){
  // Validate the entire frame before uploading any instance. Top and lower
  // replay the SAME vertex memory; the second view changes only projection.
  for(const auto&i:instances)if(i.id&&i.scaled){
   if(i.sample.vertices.size()>NativeHarborNiceVisual::VerticesPerInstance)return;
   const float scale=i.scale*i.request.modelScale*.01f;if(!std::isfinite(scale))return;
   for(const auto&v:i.sample.vertices)if(!std::isfinite(v.x*scale+i.request.effect.position[0]*.01f)||!std::isfinite(v.y*scale+i.request.effect.position[1]*.01f)||!std::isfinite(v.z*scale+i.request.effect.position[2]*.01f))return;
  }
  c.ranges={};
  for(size_t n=0;n<instances.size();++n){const auto&i=instances[n];if(!i.id||!i.scaled)continue;
   const auto*bank=tutorialStartupResources->resource(3+i.request.effect.type);if(!bank)return;
   const size_t first=n*NativeHarborNiceVisual::VerticesPerInstance;const float scale=i.scale*i.request.modelScale*.01f;
   for(size_t j=0;j<i.sample.vertices.size();++j){const auto&v=i.sample.vertices[j];const auto color=i.sample.colors.empty()?ActorColor{{1,1,1,1}}:i.sample.colors[j];
    c.vertices[first+j]={i.request.effect.position[0]*.01f+v.x*scale,i.request.effect.position[1]*.01f+v.y*scale,i.request.effect.position[2]*.01f+v.z*scale,v.u,1-(1-v.v)*bank->uvScale,color[0],color[1],color[2],color[3]*i.alpha};}
   c.ranges[n]={unsigned(first),unsigned(i.sample.vertices.size())};
   if(c.ranges[n][1])GSPGPU_FlushDataCache(c.vertices+first,c.ranges[n][1]*sizeof(Vertex));
  }
 }
 C3D_BindProgram(&program);C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER,transformLoc,&worldTransform);
 auto*attributes=C3D_GetAttrInfo();AttrInfo_Init(attributes);AttrInfo_AddLoader(attributes,0,GPU_FLOAT,3);AttrInfo_AddLoader(attributes,1,GPU_FLOAT,2);AttrInfo_AddLoader(attributes,2,GPU_FLOAT,4);
 auto*buffers=C3D_GetBufInfo();BufInfo_Init(buffers);BufInfo_Add(buffers,c.vertices,sizeof(Vertex),3,0x210);
 for(int stage=0;stage<6;++stage)C3D_TexEnvInit(C3D_GetTexEnv(stage));
 auto*environment=C3D_GetTexEnv(0);C3D_TexEnvSrc(environment,C3D_Both,GPU_TEXTURE0,GPU_PRIMARY_COLOR,GPU_PRIMARY_COLOR);C3D_TexEnvFunc(environment,C3D_Both,GPU_MODULATE);
 constexpr GPU_CULLMODE Culls[]={GPU_CULL_FRONT_CCW,GPU_CULL_BACK_CCW,GPU_CULL_NONE,GPU_CULL_NONE};
 for(size_t n=0;n<instances.size();++n){const auto&i=instances[n];const auto range=c.ranges[n];if(!i.id||!i.scaled||!range[1])continue;
  C3D_TexBind(0,&tutorialStartupTextures[3+i.request.effect.type].texture);
  // The source Nice draw requests passes1,2,3 in that order. Retain original
  // sample batch culling/pass metadata rather than flattening opaque/translucent.
  for(unsigned pass=1;pass<=3;++pass)for(const auto&batch:i.sample.batches){if(batch.pass!=pass||!batch.count||batch.cull==2)continue;
   C3D_CullFace(Culls[batch.cull]);C3D_DepthTest(true,GPU_GEQUAL,GPU_WRITE_COLOR);
   C3D_AlphaTest(pass==2,GPU_GEQUAL,128);
   // Native fading must modulate all authored passes, including opaque paper.
   C3D_AlphaBlend(GPU_BLEND_ADD,GPU_BLEND_ADD,GPU_SRC_ALPHA,GPU_ONE_MINUS_SRC_ALPHA,GPU_ONE,GPU_ONE_MINUS_SRC_ALPHA);
   C3D_DrawArrays(GPU_TRIANGLES,range[0]+batch.start,batch.count);
  }
 }
}
void Renderer::retireNativeHarborNice(){
 auto context=std::move(nativeHarborNice);if(!context)return;
 context->alive=false;context->visual->retire();
 if(c3dReady)C3D_FrameSync();
 if(context->vertices)linearFree(context->vertices);
 context->vertices=nullptr;context->renderer=nullptr;
 // Runtime may still pin the retired callback context: it now rejects before
 // touching Renderer, startup atlases or released GPU memory. No late free.
}
}

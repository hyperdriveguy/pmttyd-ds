#include "platform.hpp"
#include "map_material_color.hpp"
namespace ttyd {
// One stable native context per actual Trio allocation. Runtime's explicit
// lifetime lease keeps its tables alive across holds; release marks callbacks
// unavailable before freeing GPU resources or touching Renderer lifetime.
struct Renderer::EnemyHpBinding {
 Renderer* renderer=nullptr;const Game* game=nullptr;
 std::shared_ptr<const uint8_t> allocationOwner;
 uint64_t allocation=0,pass=0;bool nativeAlive=true,publicationBound=false;
 NativeBattleEnemyHpGpu gpu;
 BattleEnemyHpOrderedPresentation ordered;
 BattleEnemyHpOrderedHpSink sink;
 std::unique_ptr<BattleEnemyHpGpuProvider> provider;
 SourceIconGXServices gx;
 // Long proof compares BOTH numeric allocation and pinned incarnation token.
 // Short invocation additionally requires this captured pass and actual camera.
 bool current()const{return nativeAlive && renderer && game && allocationOwner && game->trioAttack.enemyHpAllocationOwner()==allocationOwner && game->trioAttack.enemyHpAllocationCurrent(allocation);}
 bool invocation()const{return current() && pass && renderer->orderedHpPresentation==&ordered && renderer->orderedHpCapturePass==pass && bool(game->trioAttack.enemyHpCamera());}
 void initialize(){
   gx.user=this;
   gx.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& c=*static_cast<EnemyHpBinding*>(u);if(!c.current())return {};
     // Original icon storage gets first refusal; actual runtime VM/resource
     // reads provide the fallback, rather than a synthesized camera/descriptor.
     const auto resource=c.renderer->sourceIcons.word(a);return resource?resource:c.game->trioAttack.enemyHpResourceWord(a);};
   gx.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& c=*static_cast<EnemyHpBinding*>(u);if(!c.current())return {};
     // Existing map-work adapter token 0x85000000 is a synthetic guest
     // identity; halfword +4 exposes actual MapMaterialGroup flags.
     if(a==0x85000004){const auto* group=c.game->trioAttack.materialGroup();return group?std::optional<uint16_t>(group->flags):std::nullopt;}
     return c.renderer->sourceIcons.half(a);};
   gx.matrix=[](void* u,BattleAudienceMatrixRequest& q){auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation() && c.game->trioAttack.enemyHpMatrix(q);};
   gx.call=[](void* u,const SourceIconGXCall& q)->std::optional<uint32_t>{auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation()?c.renderer->sourceIconCall(*c.game,q):std::nullopt;};
   gx.quad=[](void* u,const SourceIconGXQuad& q){auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation() && c.renderer->captureSourceIcon(*c.game,q);};
   BattleEnemyHpQueryServices query;query.user=this;
   query.currentCamera=[](void* u)->std::optional<uint32_t>{auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation()?c.game->trioAttack.enemyHpCamera():std::nullopt;};
   query.texture=[](void* u,uint32_t palette,uint32_t index)->std::optional<AnimationTPLImage>{auto& c=*static_cast<EnemyHpBinding*>(u);if(!c.invocation())return {};
     BattleEnemyHpMemoryReader reader{u,[](void* v,uint32_t a)->std::optional<uint32_t>{auto& c=*static_cast<EnemyHpBinding*>(v);return c.invocation()?c.game->trioAttack.enemyHpResourceWord(a):std::nullopt;}};
     return readBattleEnemyHpTexture(reader,palette,index);};
   BattleEnemyHpQuadSink target{this,[](void* u,const BattleEnemyHpGpuQuad& q){auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation() && c.sink.services().append(c.sink.services().user,q);}};
   provider=std::make_unique<BattleEnemyHpGpuProvider>(gpu.resource(),query,target);
 }
 // Bind publication once per actual allocation; held source retries reopen
 // capture for the same pass without resetting already accepted mixed entries.
 bool begin(uint64_t actual,uint64_t sourcePass){
   if(!current() || actual!=allocation || !sourcePass)return false;
   if(!publicationBound){
     BattleEnemyHpPublicationServices s;s.user=this;s.hp=&gpu.resource();
     s.current=[](void* u,uint64_t a){const auto& c=*static_cast<EnemyHpBinding*>(u);return c.current() && a==c.allocation;};
     s.complete=[](void* u,uint64_t p){const auto& c=*static_cast<EnemyHpBinding*>(u);return c.current() && c.game->trioAttack.enemyHpFrameComplete(p);};
     s.icon=[](void* u,uint32_t image)->std::optional<BattleEnemyHpIconResidency>{const auto& c=*static_cast<EnemyHpBinding*>(u);return c.current()?c.renderer->enemyHpIconResidency(image):std::nullopt;};
     s.view=[](void* u)->std::optional<BattleEnemyHpRasterView>{const auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation()?c.renderer->enemyHpRasterView(*c.game):std::nullopt;};
     if(!ordered.bind(allocation,s))return false;
     publicationBound=true;
   }
   if(!ordered.begin(sourcePass))return false;
   pass=sourcePass;sink={&ordered,sourcePass};renderer->bindEnemyHpPresentation(&ordered);
   return renderer->captureEnemyHpPresentation(sourcePass);
 }
 TrioEnemyHpNativeServices services(){
   TrioEnemyHpNativeServices s;s.user=this;
   s.number.icons.user=this;s.number.icons.word=gx.word;s.number.icons.half=gx.half;s.number.icons.matrix=gx.matrix;
   s.number.icons.currentCamera=[](void* u)->std::optional<uint32_t>{auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation()?c.game->trioAttack.enemyHpCamera():std::nullopt;};
   s.number.icons.gx=&gx;
   s.raster.user=this;s.raster.matrix=gx.matrix;
   s.raster.request=[](void* u,const BattleTexturePlaneRequest& q)->std::optional<uint32_t>{auto& c=*static_cast<EnemyHpBinding*>(u);return c.invocation() && c.provider?c.provider->request(q):std::nullopt;};
   s.begin=[](void* u,uint64_t a,uint64_t p){return static_cast<EnemyHpBinding*>(u)->begin(a,p);};
   s.publish=[](void* u,uint64_t a,uint64_t p){auto& c=*static_cast<EnemyHpBinding*>(u);
     if(!c.invocation() || a!=c.allocation || p!=c.pass || !c.game->trioAttack.enemyHpFrameComplete(p) || !c.ordered.publish(p))return false;
     // Stop interception after whole source completion/publication, retaining
     // immutable packets and icon residency pins for later presentation.
     c.renderer->captureEnemyHpPresentation(0);return true;};
   return s;
 }
};
// Out-of-frame preparation only. Reuse requires the same Game, allocation
// value AND incarnation token; unavailable original resources remain a hold.
bool Renderer::prepareEnemyHp(const Game& game){
 const auto allocation=game.trioAttack.enemyHpAllocation();
 const auto allocationOwner=game.trioAttack.enemyHpAllocationOwner();
 if(!game.trioBattleActive || !allocation || !allocationOwner){releaseEnemyHp();return false;}
 if(enemyHpBinding && enemyHpBinding->game==&game && enemyHpBinding->allocation==*allocation && enemyHpBinding->allocationOwner==allocationOwner && enemyHpBinding->current())return enemyHpBinding->gpu.resource().ready();
 releaseEnemyHp();
 if(!c3dReady || !programReady || !fontTexture.imported)return false;
 auto candidate=std::make_shared<EnemyHpBinding>();candidate->renderer=this;candidate->game=&game;candidate->allocation=*allocation;candidate->allocationOwner=allocationOwner;
 if(!candidate->gpu.load("romfs:")){frontFailure="Original enemy HP resources unavailable";return false;}
 candidate->initialize();enemyHpBinding=std::move(candidate);bindEnemyHpPresentation(&enemyHpBinding->ordered);return true;
}
void Renderer::releaseEnemyHp(){
 if(!enemyHpBinding)return;
 // Mark retained contexts unavailable before detaching capture, discarding
 // providers/publication and releasing native textures while C3D remains live.
 auto old=std::move(enemyHpBinding);old->nativeAlive=false;
 if(orderedHpPresentation==&old->ordered)bindEnemyHpPresentation(nullptr);
 old->ordered.retire();old->provider.reset();old->gpu.release();
}
bool Renderer::resumeEnemyHp(const Game& game,uint32_t camera,uint32_t userdata){
 const auto state=enemyHpBinding;
 if(!state || state->game!=&game || !state->current() || camera!=8 || !state->gpu.resource().ready())return false;
 // Begin runs once at first source admission. A held retry must reenable the
 // same exact pass only around this actual queued HP invocation.
 if(state->pass && state->ordered.buildingPass()==state->pass && !captureEnemyHpPresentation(state->pass))return false;
 // Runtime holds this stable table context through its explicit lifetime lease.
 auto services=state->services();services.lifetime=state;
 const bool completed=game.trioAttack.resumeEnemyHpDraw(camera,userdata,services);
 // Every return from the actual source invocation disables interception, even
 // on a hold. The next retry explicitly reenables its unchanged building pass.
 captureEnemyHpPresentation(0);return completed;
}
bool Renderer::drawEnemyHp(const Game& game){
 const auto state=enemyHpBinding;
 if(!state || state->game!=&game || !state->current() || !state->ordered.snapshot())return false;
 return drawEnemyHpOrdered(state->ordered,state->gpu);
}
// Observational counters only: no source stepping, publication validation or
// GPU admission. An empty completed pass is not evidence of visible HP pixels.
Renderer::EnemyHpMetrics Renderer::enemyHpMetrics(const Game& game)const{
 EnemyHpMetrics result;const auto state=enemyHpBinding;
 if(!state || state->game!=&game)return result;
 result.prepared=state->nativeAlive && state->gpu.resource().ready();
 result.allocation=state->allocation;result.allocationCurrent=state->current();result.sourcePass=state->pass;
 result.sourceComplete=result.allocationCurrent && state->pass && game.trioAttack.enemyHpFrameComplete(state->pass);
 const auto publication=state->ordered.snapshot();if(!publication)return result;
 result.publishedPass=publication->pass;result.packets=publication->packets.size();
 for(const auto& packet:publication->packets){if(std::holds_alternative<BattleEnemyHpIconPacket>(packet))++result.glyphs;else ++result.barQuads;}
 return result;
}
}

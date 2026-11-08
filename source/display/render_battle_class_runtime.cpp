#include "platform.hpp"
#include "battle_class_native_footers.hpp"
#include "trio_class_menu_frame_binding.hpp"
#include <map>
namespace ttyd {
// Actual Renderer/VM ownership is stable through this retained queued callback.
// Explicit retirement breaks frame's context pin before any GPU resource free.
struct Renderer::ClassMenuBinding {
  Renderer* renderer=nullptr;const Game* game=nullptr;
  TrioClassMenuFrameBinding source;
  std::shared_ptr<HarborSourceMenuFactory> harbor;
  std::shared_ptr<const uint8_t> battleOwner,footerGeneration;
  BattleClassSurfaceLease surface;
  const BattleClassFooterGpuResource* footerResource=nullptr;
  const void* fontTexture=nullptr;const void* iconTexture=nullptr;
  BattleFontLease font;
  std::map<uint32_t,MessageIconFrame> iconFrames;
  uint64_t sourceTick=0;bool nativeAlive=true,sourcePending=false;
  BattleClassNativeFrame frame;
  std::unique_ptr<BattleClassNativeNumeric> numeric;
  std::unique_ptr<BattleClassNativeFooters> footers;
  // Ready requires exact native generations/import identities, not merely
  // equivalent asset bytes or reused texture addresses. No lazy repair here.
  bool current() const {
    if(!nativeAlive||!renderer||!game||!font.current()||
       renderer->battleFontLease().generation!=font.generation||!source.native.allocationCurrent||
       !source.native.allocationCurrent()||!surface.current()||
       !renderer->fontTexture.imported||renderer->fontTexture.imported!=fontTexture||
       !renderer->messageIconTexture.imported||renderer->messageIconTexture.imported!=iconTexture||
       !renderer->classFooterGpu||&renderer->classFooterGpu->resource()!=footerResource||
       !footerResource->ready()||footerResource->generation()!=footerGeneration)return false;
    const auto actual=renderer->classMenuSurfaceLease();
    return actual&&actual->generation==surface.generation&&actual->textures==surface.textures;
  }
  bool initialize(const std::shared_ptr<ClassMenuBinding>& owner){
    // Provider closures use weak self references. frame.bind resources below
    // deliberately pins owner until explicit frame retirement breaks the cycle.
    const std::weak_ptr<ClassMenuBinding> weak=owner;
    const auto live=[weak]{auto p=weak.lock();return p&&p->current();};
    BattleClassNativeNumericFooters children;children.user=this;
    children.party=[](void* p){auto& c=*static_cast<ClassMenuBinding*>(p);return c.current()&&c.footers&&c.footers->party();};
    children.pinch=[](void* p){auto& c=*static_cast<ClassMenuBinding*>(p);return c.current()&&c.footers&&c.footers->pinch();};
    numeric=std::make_unique<BattleClassNativeNumeric>(frame,source.native.actors,
        source.native.addresses.base,source.resourceWord,live,children);
    // Sound is intentionally not acknowledged by a vector append. A source
    // footer arm requiring its genuine sound provider remains a retained wait.
    footers=std::make_unique<BattleClassNativeFooters>(frame,*numeric,*footerResource,
        source.native.actors,source.resourceWord,live);
    BattleClassBackend backend;backend.assets=surface.assets.get();backend.surfaceLease=surface;
    backend.fontLease=font;backend.font=font.font.get();backend.label=source.label;backend.random=source.random;
    backend.messageIcon=[weak](uint32_t id)->const MessageIconFrame*{
      auto p=weak.lock();if(!p||!p->current())return nullptr;
      const auto found=p->iconFrames.find(id);if(found!=p->iconFrames.end())return &found->second;
      const auto* icon=messageIcon(id);if(!icon||icon->frames.empty())return nullptr;
      // Copy the actual original frame at this simulation tick. A held draw
      // keeps this tick and copied metadata; no render-frame animation clock.
      // std::map nodes keep returned frame addresses stable across later inserts.
      return &p->iconFrames.emplace(id,icon->frame(p->sourceTick)).first->second;
    };
    auto binding=source.native;binding.allocationCurrent=live;
    auto display=source.display;display.draw=numeric->draw();
    return frame.bind(std::move(binding),owner,display,numeric->raster(),backend);
  }
};
bool Renderer::prepareClassMenuFrame(const Game& game){
  const bool harbor=!game.trioBattleActive&&game.harborTutorial.running();
  const auto factory=harbor?game.harborTutorial.sourceMenuFactory():nullptr;
  if(!game.trioBattleActive&&!factory){releaseClassMenuFrame();return false;}
  if(classMenuBinding&&classMenuBinding->game==&game&&classMenuBinding->harbor==factory){
    if(classMenuBinding->current())return true;
    // A suspended source invocation may not restart its prefix under a new UI
    // or texture allocation. Only actual VM retirement cancels this context.
    // While the same battle incarnation survives, retain this pending attempt;
    // rebuilding with fresh resources would restart accepted source prefixes.
    if(classMenuBinding->sourcePending&&(factory?classMenuBinding->harbor==factory:classMenuBinding->battleOwner==game.trioAttack.enemyHpAllocationOwner()))return false;
  }
  releaseClassMenuFrame();
  const auto source=factory?factory->classBinding({}):game.trioAttack.classMenuFrameBinding({});
  if(!source)return false;
  if(!prepareClassMenuDrawing()||!fontTexture.imported||!messageIconTexture.imported)return false;
  const auto surface=classMenuSurfaceLease();const auto* footer=classFooterResources();
  if(!surface||!surface->current()||!footer||!footer->ready()||harborMessageFont().glyphs.empty())return false;
  auto candidate=std::make_shared<ClassMenuBinding>();candidate->renderer=this;candidate->game=&game;
  candidate->source=*source;candidate->harbor=factory;candidate->battleOwner=factory?source->native.allocationOwner:game.trioAttack.enemyHpAllocationOwner();
  candidate->surface=*surface;candidate->footerResource=footer;candidate->footerGeneration=footer->generation();
  candidate->fontTexture=fontTexture.imported;candidate->iconTexture=messageIconTexture.imported;
  candidate->font=battleFontLease();
  if(!candidate->current()||!candidate->initialize(candidate))return false;
  classMenuBinding=std::move(candidate);return true;
}
void Renderer::releaseClassMenuFrame(){
  if(!classMenuBinding)return;
  auto old=std::move(classMenuBinding);old->nativeAlive=false;
  // A source provider may retire this context recursively. The actual queued
  // invocation/frame pin keeps its numeric/footer userdata alive until return.
  old->frame.retire();
}
bool Renderer::resumeClassMenu(const Game& game,uint32_t camera,uint32_t userdata){
  const auto context=classMenuBinding;
  if(!context||context->harbor||context->game!=&game||!context->current()||camera!=8)return false;
  // Pin the simulation tick and original icon frame copies for one retained
  // attempt, including source/GPU holds; rendering frames do not advance them.
  if(!context->sourcePending){context->sourceTick=game.ticks;context->iconFrames.clear();context->sourcePending=true;}
  BattleClassNativeSubmission submission{context.get(),[](void* p,const BattleClassBackend& batch){
    auto& c=*static_cast<ClassMenuBinding*>(p);
    if(!c.current())return false;
    // No draw is attempted before the actual frame submission phase is ready.
    // The native frame retains source completion and its camera lease on false.
    if(!c.renderer->classMenuSubmitReady){c.renderer->classMenuSubmissionWaiting=true;return false;}
    return c.renderer->drawClassMenu(batch);
  }};
  const bool completed=game.trioAttack.resumeClassMenuFrame(camera,userdata,context->frame,submission);
  if(completed)context->sourcePending=false;
  return completed;
}
bool Renderer::resumeHarborClassMenu(const Game&game,const HarborSourceDisplayInvocation&q){
  const auto context=classMenuBinding;const auto factory=game.harborTutorial.sourceMenuFactory();
  if(!harborSourceFrameActive||game.trioBattleActive||!context||!factory||context->harbor!=factory||context->game!=&game||!context->current())return false;
  if(!context->sourcePending){context->sourceTick=game.ticks;context->iconFrames.clear();context->sourcePending=true;}
  BattleClassNativeSubmission submission{context.get(),[](void*p,const BattleClassBackend&batch){auto&c=*static_cast<ClassMenuBinding*>(p);if(!c.current()||!c.renderer->harborSourceFrameActive)return false;if(!c.renderer->classMenuSubmitReady){c.renderer->classMenuSubmissionWaiting=true;return false;}return c.renderer->drawClassMenu(batch);}};
  const bool done=factory->resumeClass(q,context->frame,submission);if(done)context->sourcePending=false;return done;
}
}

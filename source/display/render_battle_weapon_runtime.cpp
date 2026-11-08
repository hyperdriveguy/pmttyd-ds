#include "platform.hpp"
#include "battle_weapon_native_numeric.hpp"
#include "battle_weapon_source_memory.hpp"
#include "battle_weapon_source_textures.hpp"
#include "source_gx_setter_routing.hpp"
#include "trio_weapon_menu_frame_binding.hpp"
#include "battle_weapon_cost_continuation.hpp"
#include "battle_class_observable_draw.hpp"
#include "battle_class_surface_packets.hpp"
#include "battle_sub_icon_raster.hpp"
#include "battle_weapon_cursor.hpp"
#include "battle_weapon_help.hpp"
#include "battle_weapon_window_bridge.hpp"
#include "battle_weapon_source_font.hpp"
#include "battle_packed_pouch.hpp"
namespace ttyd {
// This concrete queued route preserves unbound native children as source waits.
// It does not register a callback, activate battle Main or manufacture GX history.
struct Renderer::WeaponMenuBinding {
 struct Proof {
  TrioWeaponMenuFrameBinding source;std::shared_ptr<BattleWeaponSourceMemory>memory;
  std::shared_ptr<BattleWeaponSourceTextures>textures;
  std::shared_ptr<NativeUiStartup>startup;
  std::shared_ptr<const SourceGxInheritedSnapshot>fontOrdinary;
  BattleFontLease font;BattleClassSurfaceLease classes;BattleSubmenuSurfaceLease submenu;
  std::weak_ptr<WeaponMenuBinding>context;bool alive=true;mutable bool invalid=false,checking=false;
  bool current()const{
   if(checking||invalid)return false;
   checking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{checking};
   const bool ok=alive&&memory&&memory->current()&&font.current()&&classes.current()&&submenu.current()&&source.native.allocationCurrent&&source.native.allocationCurrent()&&source.native.sourceResourceCurrent&&source.native.sourceResourceCurrent();
   if(!ok||(fontOrdinary&&!fontOrdinary->current()))invalid=true;
   return !invalid;
  }
 };
 Renderer*renderer=nullptr;const Game*game=nullptr;std::shared_ptr<Proof>proof;
 std::shared_ptr<const uint8_t>battleOwner;
 BattleWeaponNativeFrame frame;std::unique_ptr<BattleWeaponNativeNumeric>numeric;
 std::shared_ptr<BattleWeaponOrderedPublication>ordered;
 std::shared_ptr<const BattleWeaponOrderedBatch>sealed;
 std::shared_ptr<const BattleWeaponRasterPlan>plan;
 PackedWeaponCostCursor cost;
 BattleClassObservableDrawCursor classes;
 BattleSubIconCursor subIcons;BattleSubIconRasterCursor subRaster;
 BattleWeaponCursor cursor;BattleWeaponHelpCursor help;
 BattleWindowSurfaceResource absentWindowSurface;std::unique_ptr<BattleWeaponWindowBridge>windowChild;
 std::unique_ptr<BattleWeaponSourceFont>fontChild;
 struct HelpText {std::weak_ptr<WeaponMenuBinding>context;const BattleWeaponHelpBytes*bytes=nullptr;uint32_t offset=0;};
 std::shared_ptr<HelpText>helpText;
 std::unique_ptr<BattleClassSurfacePacketProvider>classPackets;
 std::unique_ptr<BattleClassSurfaceRaster>classRaster;
 std::shared_ptr<SourceGxSetterRouting>gx;
 std::unique_ptr<BattleSubmenuPacketProvider>submenuPackets;
 BattleSubmenuSourceServices submenuSource;BattleSubmenuSourceCursor submenuInit,submenuQuad;
 uint64_t pass=0,completedPass=0;bool pending=false,alive=true;
 bool current()const{return alive&&renderer&&game&&proof&&proof->current();}
 bool sourceCurrent()const{return current()&&frame.camera();}
 static std::shared_ptr<WeaponMenuBinding>lock(void*p){return static_cast<Proof*>(p)->context.lock();}
 bool begin(uint64_t actualPass){
  if(!current()||!frame.camera()||!actualPass)return false;
  if(pass==actualPass)return ordered->begin(pass);
  // Only a genuinely completed previous source/submission reaches a new pass.
  if(pass&&completedPass!=pass)return false;
  const auto view=frame.camera()->view();if(!view)return false;
  pass=actualPass;sealed.reset();plan.reset();windowChild.reset();fontChild.reset();helpText.reset();
  BattleSubmenuCameraProof camera;camera.lifetime=proof;camera.user=proof.get();camera.generation=proof->source.native.generation;camera.pass=pass;camera.camera=8;
  camera.projection=view->matrices.projection;camera.viewport=view->viewport;
  for(unsigned i=0;i<4;++i)camera.scissor[i]=view->scissor[i];
  camera.current=[](void*p,const BattleSubmenuCameraProof&q){auto&f=*static_cast<Proof*>(p);return f.current()&&q.generation==f.source.native.generation&&q.camera==8;};
  BattleClassSurfaceQueryServices query;query.user=proof.get();query.lifetime=proof;query.sourceResourceOwner=proof->source.native.sourceResourceOwner;
  query.current=[](void*p){auto c=lock(p);return c&&c->sourceCurrent()&&c->gx&&c->gx->compatibleNow();};
  query.ordinaryCurrent=[](void*p){auto c=lock(p);return c&&c->gx&&c->gx->compatibleNow();};
  query.ordinaryCapture=[](void*p){auto c=lock(p);return c&&c->gx?c->gx->capture():nullptr;};
  query.sourceResourceCurrent=[](void*p){auto&f=*static_cast<Proof*>(p);return f.textures&&f.textures->current();};
  query.texture=[](void*p,uint32_t palette,uint32_t index){auto&f=*static_cast<Proof*>(p);return f.textures?f.textures->texture(palette,index):std::nullopt;};
  query.camera=[](void*p,std::optional<uint32_t>i)->std::optional<uint32_t>{auto c=lock(p);return c?c->numeric->camera(i):std::nullopt;};
  // Per-Load immutable inherited evidence comes from actual process history;
  // neither the native atlas nor the TEX constructor supplies that history.
  BattleClassSurfacePacketSink sink;sink.user=proof.get();sink.append=[](void*p,BattleClassSurfacePacketPtr q){auto c=lock(p);return c&&c->sourceCurrent()&&c->ordered->appendClassSurface(c->pass,std::move(q));};
  classPackets=std::make_unique<BattleClassSurfacePacketProvider>(renderer->classSurfaceGpu->resource(),camera,query,sink);
  BattleSubmenuSourceServices source;source.user=proof.get();source.lifetime=proof;source.current=[](void*p){auto c=lock(p);return c&&c->sourceCurrent();};
  source.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto c=lock(p);return c?c->numeric->word(a):std::nullopt;};
  source.request=[](void*p,const BattleTexturePlaneRequest&q)->std::optional<uint32_t>{auto c=lock(p);if(!c||!c->sourceCurrent())return {};const auto result=c->classPackets->request(q);if(result&&q.kind==BattleTexturePlaneKind::Call&&q.function==BattleTexturePlaneCall::TextureObject){if(q.integers.size()!=2||!c->proof->textures->acknowledge(q.integers[0],q.integers[1]))return {};}if(result)c->gx->accepted(q);return result;};
  classRaster=std::make_unique<BattleClassSurfaceRaster>(source,*classPackets);
  BattleSubmenuQueryServices sub;sub.user=proof.get();sub.lifetime=proof;sub.sourceResourceOwner=proof->source.native.sourceResourceOwner;
  sub.current=query.current;sub.sourceResourceCurrent=query.sourceResourceCurrent;sub.camera=query.camera;sub.ordinaryCapture=query.ordinaryCapture;
  sub.texture=[](void*p,uint32_t palette,uint32_t index)->std::optional<BattleSubmenuTextureObject>{auto&f=*static_cast<Proof*>(p);const auto v=f.textures?f.textures->texture(palette,index):std::nullopt;return v?std::optional<BattleSubmenuTextureObject>(*v):std::nullopt;};
  BattleSubmenuPacketSink subSink;subSink.user=proof.get();subSink.append=[](void*p,BattleSubmenuPacketPtr q){auto c=lock(p);return c&&c->sourceCurrent()&&c->ordered->appendSubmenu(c->pass,std::move(q));};
  submenuPackets=std::make_unique<BattleSubmenuPacketProvider>(*renderer->weaponMenuSurfaceResources(),camera,sub,subSink);
  submenuSource=source;submenuSource.request=[](void*p,const BattleTexturePlaneRequest&q)->std::optional<uint32_t>{auto c=lock(p);if(!c||!c->sourceCurrent())return {};const auto result=c->submenuPackets->request(q);if(result&&q.kind==BattleTexturePlaneKind::Call&&q.function==BattleTexturePlaneCall::TextureObject){if(q.integers.size()!=2||!c->proof->textures->acknowledge(q.integers[0],q.integers[1]))return {};}if(result)c->gx->accepted(q);return result;};
  return ordered->begin(pass);
 }
 static bool fontFunction(uint32_t f){switch(f){case 0x80077b4c:case 0x800777e0:case 0x80077844:case 0x80077758:case 0x80075528:case 0x80076a10:case 0x80076cdc:case 0x800773d0:case 0x80077aa8:case 0x80077ab4:return true;default:return false;}}
 BattleActorServices fontActors(){
  auto a=proof->memory->actors();a.user=this;a.memory.user=this;
  a.byte=[](void*p,uint32_t address){const auto v=static_cast<WeaponMenuBinding*>(p)->proof->memory->byte(address);if(!v)std::abort();return *v;};
  a.memory.word=[](void*p,uint32_t address){const auto v=static_cast<WeaponMenuBinding*>(p)->numeric->word(address);if(!v)std::abort();return *v;};
  a.memory.half=[](void*p,uint32_t address){const auto v=static_cast<WeaponMenuBinding*>(p)->proof->memory->half(address);if(!v)std::abort();return *v;};
  a.memory.storeWord=[](void*p,uint32_t address,uint32_t value){if(!static_cast<WeaponMenuBinding*>(p)->proof->memory->store(address,4,value))std::abort();};
  a.memory.storeHalf=[](void*p,uint32_t address,uint16_t value){if(!static_cast<WeaponMenuBinding*>(p)->proof->memory->store(address,2,value))std::abort();};
  a.memory.storeByte=[](void*p,uint32_t address,uint8_t value){if(!static_cast<WeaponMenuBinding*>(p)->proof->memory->store(address,1,value))std::abort();};return a;
 }
 static constexpr size_t WeaponMenuGlyphCapacity=4096;
 static constexpr uint32_t OriginalUsLanguage=1;
 static constexpr uint32_t OriginalFontWidth=512,OriginalFontHeight=256;
 std::optional<uint32_t>fontRequest(const BattleWeaponSourceFontCall&q){
  if(!fontFunction(q.function)||!sourceCurrent()||!proof->startup||proof->startup->language()!=OriginalUsLanguage)return {};
  if(!fontChild){
   const auto raster=renderer->battleFontRasterView(*game);if(!raster||!proof->source.random)return {};
   BattleWeaponSourceFontBinding b;b.actors=fontActors();b.font=proof->font;b.raster=*raster;b.resources=proof;b.capacity=WeaponMenuGlyphCapacity;b.gx=gx;
   const auto weak=proof->context;b.sourceCurrent=[weak]{auto c=weak.lock();return c&&c->sourceCurrent()&&c->proof->startup->language()==OriginalUsLanguage;};b.publicationCurrent=[p=proof]{return p->current();};
   b.byte=[m=proof->memory](uint32_t a){return m->byte(a);};b.half=[m=proof->memory](uint32_t a){return m->half(a);};
   b.camera=[weak]()->std::optional<uint32_t>{auto c=weak.lock();return c?c->numeric->camera():std::nullopt;};b.matrix=[weak](BattleAudienceMatrixRequest&q){auto c=weak.lock();return c&&c->numeric->matrix(q);};
   b.truncate=[weak](float v)->std::optional<int32_t>{auto c=weak.lock();return c?c->numeric->truncate(v):std::nullopt;};b.fma=[weak](float a,float d,float e)->std::optional<float>{auto c=weak.lock();return c?c->numeric->fma(a,d,e):std::nullopt;};
   b.staticTextureCurrent=[p=proof](uint32_t address,const BattleFontLease&font){const auto object=p->startup->fontObject();return p->current()&&object&&object->object==address&&object->width==OriginalFontWidth&&object->height==OriginalFontHeight&&font.current()&&font.generation==p->font.generation&&font.font==p->font.font&&font.texture==p->font.texture;};
   b.ordinaryCurrent=[weak]{auto c=weak.lock();if(!c||!c->sourceCurrent())return false;const auto captured=c->gx->capture();if(!captured)return false;if(!c->proof->fontOrdinary)c->proof->fontOrdinary=captured;return c->proof->fontOrdinary->current();};
   b.random=proof->source.random;b.orderedUser=proof.get();b.reserve=[](void*p,const BattleFontGlyphPacket&q){auto c=lock(p);return c&&c->sourceCurrent()&&c->ordered->reserveFont(c->pass,q);};
   b.commit=[](void*p,const BattleFontGlyphPacket&q){const auto c=lock(p);if(!c)std::abort();c->ordered->commitFont(c->pass,q);};
   fontChild=BattleWeaponSourceFont::create(std::move(b));if(!fontChild)return {};
  }
  return fontChild->request(q);
 }
 std::optional<uint32_t>helpChild(const BattleWeaponHelpCall&q){
  if(q.function==0x8007b44c)return windowHelp(q);
  if(q.function==0x80081b8c&&q.integers.size()==1&&q.reals.empty()&&!q.privateColor&&!q.privateText&&!q.guestText){if(!sourceCurrent()||!proof->source.messageLookup)return {};return proof->source.messageLookup(q.integers[0]);}
  if(!fontFunction(q.function))return {};
  BattleWeaponSourceFontCall f;f.function=q.function;f.integers=q.integers;f.reals=q.reals;
  if(q.function==0x800773d0){
   if(q.privateText){
    if(q.guestText)return {};
    if(!helpText||helpText->bytes!=q.privateText||helpText->offset!=q.privateOffset){helpText=std::make_shared<HelpText>();helpText->context=proof->context;helpText->bytes=q.privateText;helpText->offset=q.privateOffset;}
    SourceMatrixFontText t;t.user=helpText.get();t.lifetime=helpText;
    t.byte=[](void*p,uint32_t offset)->std::optional<uint8_t>{const auto&t=*static_cast<HelpText*>(p);const auto c=t.context.lock();if(!c||!c->sourceCurrent()||uint64_t(t.offset)+offset>UINT32_MAX)return {};return t.bytes->byte(t.offset+offset);};
    t.half=[](void*p,uint32_t offset)->std::optional<uint16_t>{const auto&t=*static_cast<HelpText*>(p);const auto c=t.context.lock();if(!c||!c->sourceCurrent()||uint64_t(t.offset)+offset+2>0x100000000ULL)return {};const auto a=t.bytes->byte(t.offset+offset),b=t.bytes->byte(t.offset+offset+1);return a&&b?std::optional<uint16_t>(uint16_t(uint16_t(*a)<<8|*b)):std::nullopt;};f.sourceText=std::move(t);
   }else if(q.guestText)f.integers={*q.guestText};else return {};
  }
  const auto result=fontRequest(f);if(result)helpText.reset();return result;
 }
 std::optional<uint32_t>subRasterCall(const BattleSubIconRasterCall&q){
  if(!sourceCurrent())return {};
  if(fontFunction(q.function)){BattleWeaponSourceFontCall f;f.function=q.function;f.integers=q.integers;f.reals=q.reals;f.matrix=q.matrix;f.privateText=q.privateText;return fontRequest(f);}
  if(q.function==0x8000f2fc&&q.integers==std::vector<uint32_t>{8}&&q.reals.empty()&&!q.matrix&&!q.privateText){const auto n=numeric->raster();return n.request(n.user,{BattleClassRasterService::Camera,{8},{},{}});}
  if(q.function==0x80103a14&&q.integers.empty()&&q.reals.empty()&&!q.matrix&&!q.privateText)return resumeBattleSubmenuInit(submenuSource,submenuInit)?std::optional<uint32_t>(0):std::nullopt;
  if(q.function==0x801037c4&&q.integers.size()==4&&q.reals.size()==4&&!q.matrix&&!q.privateText){std::array<float,4>xy;std::array<uint32_t,4>color;for(unsigned i=0;i<4;++i){xy[i]=q.reals[i];color[i]=q.integers[i];}return resumeBattleSubmenuQuad(xy,color,submenuSource,submenuQuad)?std::optional<uint32_t>(0):std::nullopt;}
  if(q.privateText)return {};
  // These are the reached original low GX/TEX children of SubIcon, not a
  // whole Init/Quad acknowledgement. Unsupported font/icon owners still hold.
  BattleTexturePlaneRequest low;low.function=static_cast<BattleTexturePlaneCall>(q.function);low.integers=q.integers;low.reals=q.reals;low.matrix=q.matrix;
  return submenuSource.request(submenuSource.user,low);
 }
 std::optional<uint32_t>windowHelp(const BattleWeaponHelpCall&q){
  if(!sourceCurrent()||q.function!=0x8007b44c)return {};
  if(!windowChild){
   const auto view=frame.camera()->view();const auto address=numeric->camera(8);
   if(!view||!address||!proof->startup||!gx)return {};
   BattleWeaponWindowBinding b;b.startup=proof->startup;b.resources=proof;b.gx=gx;b.cameraAddress=*address;
   b.camera.lifetime=proof;b.camera.user=proof.get();b.camera.generation=proof->source.native.generation;b.camera.pass=pass;b.camera.camera=8;
   b.camera.projection=view->matrices.projection;b.camera.viewport=view->viewport;for(unsigned i=0;i<4;++i)b.camera.scissor[i]=view->scissor[i];
   b.camera.current=[](void*p,const BattleSubmenuCameraProof&q){auto&f=*static_cast<Proof*>(p);return f.current()&&q.generation==f.source.native.generation&&q.camera==8;};
   const std::weak_ptr<WeaponMenuBinding>weak=proof->context;
   b.sourceCurrent=[weak]{auto c=weak.lock();return c&&c->sourceCurrent();};
   b.longCurrent=[p=proof]{return p->current();};
   b.currentCamera=[weak]()->std::optional<uint32_t>{auto c=weak.lock();return c?c->numeric->camera():std::nullopt;};
   b.word=[weak](uint32_t a)->std::optional<uint32_t>{auto c=weak.lock();return c?c->numeric->word(a):std::nullopt;};
   b.matrix=[weak](BattleAudienceMatrixRequest&q){auto c=weak.lock();return c&&c->numeric->matrix(q);};
   b.append=[weak](BattleWindowPacketPtr p){auto c=weak.lock();return c&&c->sourceCurrent()&&c->ordered->appendWindow(c->pass,std::move(p));};
   // A real sourceReady0 invocation must not depend on native atlas residency.
   // Existing native GPU owner, when present, remains stable through this pass.
   const auto&surface=renderer->windowPrimitiveGpu?renderer->windowPrimitiveGpu->resource():absentWindowSurface;
   windowChild=std::make_unique<BattleWeaponWindowBridge>(surface,std::move(b));
  }
  return windowChild->request(q);
 }
 std::optional<uint32_t>child(const BattleWeaponDrawRequest&q){
  if(!sourceCurrent())return {};
  if(q.function==0x80126968&&q.count==2){
   PackedWeaponCostServices s;s.user=this;s.lifetime=proof;s.current=[](void*p){return static_cast<WeaponMenuBinding*>(p)->sourceCurrent();};
   s.word=[](void*p,uint32_t a){return static_cast<WeaponMenuBinding*>(p)->proof->memory->word(a);};s.byte=[](void*p,uint32_t a){return static_cast<WeaponMenuBinding*>(p)->proof->memory->byte(a);};
   s.equippedCount=[](void*p,uint32_t item)->std::optional<uint32_t>{auto&c=*static_cast<WeaponMenuBinding*>(p);const auto pouch=c.proof->memory->word(0x8041eb00);if(!pouch)return {};return uint32_t(packedPouchEquippedBadgeCount(*pouch,int32_t(item),c.proof->source.native.actors.memory));};
   // Reached original pow/double-fctiwz domains still require their actual
   // admitted numeric providers. The ordinary source free-cost arm needs none.
   return resumePackedWeaponCost(q.arguments[0],q.arguments[1],s,cost);
  }
  if(q.function==0x80119abc&&q.count==2&&q.output){
   BattleClassDrawContinuationServices s;s.raster=numeric->raster();s.raster.user=this;
   s.raster.request=[](void*p,const BattleClassRasterRequest&r)->std::optional<uint32_t>{auto&c=*static_cast<WeaponMenuBinding*>(p);if(!c.sourceCurrent())return {};if(r.function==BattleClassRasterService::Camera){const auto n=c.numeric->raster();return n.request(n.user,r);}if(fontFunction(uint32_t(r.function))){BattleWeaponSourceFontCall f;f.function=uint32_t(r.function);f.integers=r.integers;f.reals=r.reals;f.matrix=r.matrix;return c.fontRequest(f);}return c.classRaster->request(r);};
   s.raster.truncate=[](void*p,float v){return static_cast<WeaponMenuBinding*>(p)->numeric->truncate(v);};s.raster.identity=[](void*p){return static_cast<WeaponMenuBinding*>(p)->numeric->identity();};
   // Actual source font children above own their retained globals/GX/glyphs.
   // Missing icon children remain real source waits until their owner binds.
   return resumeBattleClassObservableDraw(q.arguments[0],q.arguments[1],q.output->classes,s,classes)?std::optional<uint32_t>(0):std::nullopt;
  }
  if(q.function==0x80119024&&q.count==3&&q.output){
   BattleSubIconServices s;s.math=numeric->draw();s.user=this;s.current=[](void*p){return static_cast<WeaponMenuBinding*>(p)->sourceCurrent();};s.truncate=[](void*p,float v){return static_cast<WeaponMenuBinding*>(p)->numeric->truncate(v);};s.fma=[](void*p,float a,float b,float d){return static_cast<WeaponMenuBinding*>(p)->numeric->fma(a,b,d);};
   s.request=[](void*p,const BattleSubIconRasterRequest&r)->std::optional<uint32_t>{auto&c=*static_cast<WeaponMenuBinding*>(p);BattleSubIconRasterServices n;n.actors=c.numeric->raster().memory;n.actors.user=c.proof->source.native.actors.user;n.actors.byte=c.proof->source.native.actors.byte;n.user=&c;n.lifetime=c.proof;n.current=[](void*p){return static_cast<WeaponMenuBinding*>(p)->sourceCurrent();};n.request=[](void*p,const BattleSubIconRasterCall&r)->std::optional<uint32_t>{return static_cast<WeaponMenuBinding*>(p)->subRasterCall(r);};n.matrix=[](void*p,BattleAudienceMatrixRequest&r){return static_cast<WeaponMenuBinding*>(p)->numeric->matrix(r);};n.truncate=[](void*p,float v){return static_cast<WeaponMenuBinding*>(p)->numeric->truncate(v);};return resumePackedBattleSubIconRaster(r,n,c.subRaster)?std::optional<uint32_t>(0):std::nullopt;};
   return resumePackedBattleSubIcon(q.arguments[0],*q.output,q.arguments[1],q.arguments[2],s,subIcons)?std::optional<uint32_t>(0):std::nullopt;
  }
  if(q.function==0x80117a6c&&q.count==0&&q.cursorWindow){
   BattleWeaponCursorServices s;s.actors=proof->source.native.actors;s.lifetime=proof;s.user=this;s.current=[](void*p){return static_cast<WeaponMenuBinding*>(p)->sourceCurrent();};s.interpolate=[](void*p,int32_t m,int32_t step,int32_t d,float a,float b){auto n=static_cast<WeaponMenuBinding*>(p)->numeric->draw();return n.interpolate(n.user,m,step,d,a,b);};s.fma=[](void*p,float a,float b,float d){return static_cast<WeaponMenuBinding*>(p)->numeric->fma(a,b,d);};
   s.icons.user=this;s.icons.word=[](void*p,uint32_t a){return static_cast<WeaponMenuBinding*>(p)->numeric->word(a);};s.icons.half=[](void*p,uint32_t a){return static_cast<WeaponMenuBinding*>(p)->proof->memory->half(a);};s.icons.currentCamera=[](void*p){return static_cast<WeaponMenuBinding*>(p)->numeric->camera();};s.icons.matrix=[](void*p,BattleAudienceMatrixRequest&r){return static_cast<WeaponMenuBinding*>(p)->numeric->matrix(r);};
   return resumePackedBattleWeaponCursor(*q.cursorWindow,s,cursor)?std::optional<uint32_t>(0):std::nullopt;
  }
  if(q.function==0x80117c1c&&q.count==1&&q.x&&q.y){
   BattleWeaponHelpServices s;s.actors=proof->source.native.actors;s.lifetime=proof;s.user=this;s.current=[](void*p){return static_cast<WeaponMenuBinding*>(p)->sourceCurrent();};s.byte=[](void*p,uint32_t a){return static_cast<WeaponMenuBinding*>(p)->proof->memory->byte(a);};
   s.request=[](void*p,const BattleWeaponHelpCall&r)->std::optional<uint32_t>{return static_cast<WeaponMenuBinding*>(p)->helpChild(r);};
   return resumePackedBattleWeaponHelp(q.arguments[0],*q.x,*q.y,s,help)?std::optional<uint32_t>(0):std::nullopt;
  }
  return {};
 }
 bool initialize(const std::shared_ptr<WeaponMenuBinding>&self){
  proof->context=self;const std::weak_ptr<WeaponMenuBinding>weak=self;
  numeric=std::make_unique<BattleWeaponNativeNumeric>(frame,proof->source.native.actors,proof->source.native.addresses.base,[m=proof->memory](uint32_t a){return m->word(a);},[weak]{auto c=weak.lock();return c&&c->sourceCurrent();});
  proof->textures=BattleWeaponSourceTextures::create(proof->source,proof->memory,0x60018000,[weak]{auto c=weak.lock();return c&&c->sourceCurrent();},[weak](float v)->std::optional<int32_t>{auto c=weak.lock();return c?c->numeric->truncate(v):std::nullopt;});
  if(!proof->textures)return false;
  const auto history=proof->startup?proof->startup->gxHistory():nullptr;if(!history)return false;
  gx=std::make_shared<SourceGxSetterRouting>(history,proof,[weak]{auto c=weak.lock();return c&&c->sourceCurrent();});
  ordered=std::make_shared<BattleWeaponOrderedPublication>();BattleWeaponOrderedBinding b;b.allocationOwner=proof->source.native.allocationOwner;b.sourceResourceOwner=proof->source.native.sourceResourceOwner;b.resources=proof;b.allocation=proof->source.native.allocation;b.generation=proof->source.native.generation;b.capacity=4096;b.user=proof.get();b.font=&renderer->battleFontGpu;b.submenu=renderer->weaponMenuSurfaceResources();b.window=renderer->windowPrimitiveSurfaceResources();b.classes=&renderer->classSurfaceGpu->resource();
  b.current=[](void*p,uint32_t a,uint64_t g){auto&f=*static_cast<Proof*>(p);return a==f.source.native.allocation&&g==f.source.native.generation&&f.current();};
  b.sourceCurrent=[](void*p,uint64_t pass){auto c=lock(p);return c&&c->pass==pass&&c->sourceCurrent();};b.sourceComplete=[](void*p,uint64_t pass){auto c=lock(p);return c&&c->frame.sourceComplete(pass);};
  b.icon=[](void*p,uint32_t image)->std::optional<BattleEnemyHpIconResidency>{auto c=lock(p);return c&&c->current()?c->renderer->enemyHpIconResidency(image):std::nullopt;};
  b.ready=[](void*p,const BattleWeaponOrderedBatch&q){auto c=lock(p);if(!c||!c->sourceCurrent()||c->sealed.get()!=&q)return false;if(!c->plan)c->plan=c->renderer->prepareWeaponOrderedRaster(c->sealed);return c->plan&&c->renderer->canDrawWeaponOrderedRaster(*c->plan);};
  b.tryDraw=[](void*p,const BattleWeaponOrderedBatch&q){auto c=lock(p);if(!c||!c->sourceCurrent()||c->sealed.get()!=&q||!c->plan)return false;if(!c->renderer->weaponMenuSubmitReady){c->renderer->weaponMenuSubmissionWaiting=true;return false;}return c->renderer->submitWeaponOrderedRaster(*c->plan);};
  if(!ordered->bind(b))return false;
  auto s=numeric->weapon();s.user=this;s.current=[](void*p){return static_cast<WeaponMenuBinding*>(p)->sourceCurrent();};s.request=[](void*p,const BattleWeaponDrawRequest&q){return static_cast<WeaponMenuBinding*>(p)->child(q);};s.truncate=[](void*p,float v){return static_cast<WeaponMenuBinding*>(p)->numeric->truncate(v);};s.fma=[](void*p,float a,float b,float d){return static_cast<WeaponMenuBinding*>(p)->numeric->fma(a,b,d);};s.identity=[](void*p){return static_cast<WeaponMenuBinding*>(p)->numeric->identity();};
  auto submit=ordered->submission();submit.user=proof.get();submit.begin=[](void*p,uint64_t pass){auto c=lock(p);return c&&c->begin(pass);};submit.seal=[](void*p,uint64_t pass)->std::shared_ptr<const void>{auto c=lock(p);if(!c)return {};c->sealed=c->ordered->seal(pass);return c->sealed;};submit.submit=[](void*p,uint64_t pass,const std::shared_ptr<const void>&batch){auto c=lock(p);return c&&c->sealed&&batch.get()==c->sealed.get()&&!batch.owner_before(c->sealed)&&!c->sealed.owner_before(batch)&&c->ordered->submit(pass,c->sealed);};submit.current=[](void*p,const std::shared_ptr<const void>&batch){auto c=lock(p);return c&&c->sealed&&batch.get()==c->sealed.get()&&!batch.owner_before(c->sealed)&&!c->sealed.owner_before(batch)&&c->ordered->validate(c->sealed);};
  return frame.bind(proof->source.native,self,s,submit);
 }
};
bool Renderer::prepareWeaponMenuFrame(const Game&game){
 const auto source=game.trioAttack.weaponMenuFrameBinding({});
 if(weaponMenuBinding&&weaponMenuBinding->game==&game){
  if(game.trioBattleActive&&weaponMenuBinding->current())return true;
  if(weaponMenuBinding->pending&&(!source||source->native.allocationOwner==weaponMenuBinding->proof->source.native.allocationOwner))return false;
 }
 releaseWeaponMenuFrame();
 if(!game.trioBattleActive||!source||weaponMenuAbandonedAllocation==source->native.allocationOwner)return false;
 weaponMenuAbandonedAllocation.reset();
 const auto startup=game.nativeUiStartup();if(!startup)return false;
 if(!prepareClassMenuDrawing()||!prepareWeaponMenuSurfaces())return false;
 const auto classes=classMenuSurfaceLease();const auto submenu=weaponMenuSurfaceLease();const auto font=battleFontLease();if(!classes||!submenu||!font.current())return false;
 const auto memory=game.trioAttack.weaponMenuSourceMemory(startup,"romfs:/source-ui-startup/memory.pmevt");if(!memory)return false;
 auto c=std::make_shared<WeaponMenuBinding>();c->renderer=this;c->game=&game;c->proof=std::make_shared<WeaponMenuBinding::Proof>();c->proof->source=*source;c->proof->memory=memory;c->proof->startup=startup;c->proof->font=font;c->proof->classes=*classes;c->proof->submenu=*submenu;c->battleOwner=game.trioAttack.enemyHpAllocationOwner();
 if(!c->current()||!c->initialize(c))return false;
 weaponMenuBinding=std::move(c);return true;
}
void Renderer::releaseWeaponMenuFrame(){if(!weaponMenuBinding)return;auto old=std::move(weaponMenuBinding);if(old->pending)weaponMenuAbandonedAllocation=old->proof->source.native.allocationOwner;old->alive=false;old->proof->alive=false;if(old->ordered)old->ordered->retire();old->frame.retire();}
bool Renderer::resumeWeaponMenu(const Game&game,uint32_t camera,uint32_t userdata){const auto c=weaponMenuBinding;if(!c||c->game!=&game||camera!=8||!c->current())return false;c->pending=true;const bool complete=game.trioAttack.resumeWeaponMenuFrame(camera,userdata,c->frame);if(complete){c->completedPass=c->pass;c->pending=false;}return complete;}
}

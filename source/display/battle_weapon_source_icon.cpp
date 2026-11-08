#include "battle_weapon_source_icon.hpp"
#include <cmath>
#include <algorithm>
#include <cstring>
#include <limits>
namespace ttyd {namespace {
constexpr uint32_t UiCamera=8,QuadPrimitive=128,QuadVertices=4;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const BattleAudienceIconRequest&a,const BattleAudienceIconRequest&b){return a.function==b.function&&a.flags==b.flags&&a.icon==b.icon&&a.rgba==b.rgba&&std::memcmp(a.matrix.data(),b.matrix.data(),sizeof(a.matrix))==0;}
bool finite(const BattleActMatrix&m){for(auto v:m)if(!std::isfinite(v))return false;return true;}
bool args(const SourceIconGXCall&q,std::initializer_list<uint32_t>v){std::array<uint32_t,8>a{};std::copy(v.begin(),v.end(),a.begin());return q.integers==a;}
}
struct BattleWeaponSourceIcon::State {
 BattleWeaponSourceIconBinding b;std::unique_ptr<BattleWeaponIconTextures>textures;
 SourceIconGXServices raw;SourceIconMatrixServices services;SourceIconMatrixCursor matrixCursor;
 SourceIconGXCursor gxCursor;std::optional<SourceMatrixIcon>record;BattleActMatrix recordMatrix{};bool drawing=false;
 std::optional<BattleAudienceIconRequest>outer;
 NativeIconProgram program;std::optional<BattleActMatrix>loadedMatrix;
 std::optional<BattleSubmenuTextureObject>loadedTexture;
 std::shared_ptr<const SourceGxInheritedSnapshot>ordinary;
 std::optional<NativeIconPlan>material;std::optional<uint32_t>pendingImage;
 std::optional<bool>blend,beforeAlpha,alphaReject,depth,depthWrite;
 bool matrixSelected=false,evidenceRetained=false,begin=false,quadCommitted=false,callback=false;
 mutable bool invalid=false,checking=false;uint32_t pending=0;
 bool live()const{if(invalid||checking)return false;Guard guard(checking);
  const bool ok=b.textures.process&&b.textures.process->current()&&b.textures.lifetime&&b.textures.longCurrent&&b.textures.longCurrent()&&b.camera.lifetime&&b.camera.current&&b.camera.current(b.camera.user,b.camera);
  if(!ok)invalid=true;
  return ok;
 }
 bool source()const{return live()&&b.textures.sourceCurrent&&b.textures.sourceCurrent();}
 std::optional<uint32_t>word(uint32_t a){if(!source())return {};auto v=b.textures.process->owns(a,4)?b.textures.process->word(a):b.word(a);return source()?v:std::nullopt;}
 std::optional<uint16_t>half(uint32_t a){if(!source())return {};auto v=b.textures.process->owns(a,2)?b.textures.process->half(a):b.half(a);return source()?v:std::nullopt;}
 bool pipeline(const SourceIconGXCall&q){
  switch(q.function){
  case 0x802b932c:if(!args(q,{0,4,5,0})&&!args(q,{1,4,5,0}))return false;blend=q.integers[0]!=0;return true;
  case 0x802b940c:if(!args(q,{0})&&!args(q,{1}))return false;beforeAlpha=q.integers[0]!=0;return true;
  case 0x802b8d60:if(args(q,{7,0,0,7,0}))alphaReject=false;else if(args(q,{6,128,1,0,0}))alphaReject=true;else return false;return true;
  case 0x802b93d8:if(!args(q,{1,3,0})&&!args(q,{1,3,1})&&!args(q,{0,3,0}))return false;depth=q.integers[0]!=0;depthWrite=q.integers[2]!=0;return true;
  default:return false;
  }
 }
 std::optional<uint32_t>call(const SourceIconGXCall&q){
  pending=q.function;if(!source())return {};
  switch(q.function){
  case 0x8000f2fc:if(!args(q,{UiCamera}))return {};return b.cameraAddress(UiCamera);
  case 0x8000f2e4:if(!args(q,{}))return {};return b.cameraAddress({});
  case 0x802ba2fc:if(!args(q,{})||!finite(q.matrix))return {};loadedMatrix=q.matrix;break;
  case 0x802ba39c:if(!args(q,{0}))return {};matrixSelected=true;break;
  case 0x80037a80:{if(!args(q,{q.integers[0],q.integers[1]}))return {};auto v=textures->construct(q.integers[0],q.integers[1]);if(v){loadedTexture.reset();ordinary.reset();evidenceRetained=begin=quadCommitted=false;material.reset();}return v;}
  case 0x802b7544:return textures->lod(q)?std::optional<uint32_t>(0):std::nullopt;
  case 0x802b7880:{if(!args(q,{b.textures.privateAddress,0}))return {};
   if(!loadedTexture){loadedTexture=textures->snapshot(q.integers[0]);if(!loadedTexture)return {};}
   if(!ordinary){ordinary=b.gx->capture(record&&(record->flags&32)?3:1);if(!ordinary)return {};}
   if(!evidenceRetained){if(!b.retainOrdinary(ordinary))return {};evidenceRetained=true;}
   pendingImage=loadedTexture->image.index;
   if(!b.pin->markPending(*pendingImage))return {};
   const auto resident=b.residency(*pendingImage);if(!resident||!validBattleEnemyHpIconResidency(*resident)||resident->image!=*pendingImage||resident->width!=loadedTexture->image.width||resident->height!=loadedTexture->image.height)return {};
   break;}
  case 0x802b76c4:case 0x802b76d4:if(!args(q,{b.textures.privateAddress}))return {};return textures->dimension(q.function,q.integers[0]);
  case 0x802b5a6c:if(!args(q,{QuadPrimitive,0,QuadVertices})||!loadedMatrix||!matrixSelected||!loadedTexture||!ordinary||!ordinary->current()||!blend||!beforeAlpha||!alphaReject||!depth||!depthWrite)return {};
   material=program.plan();if(!material)return {};begin=true;break;
  // These whole linked parents are never silently treated as GX setters.
  case 0x800208c0:case 0x800217c8:case 0x8002b438:case 0x8001d968:return {};
  default:if(!pipeline(q)&&!program.request(q))return {};break;
  }
  // Concrete finite setters above have mutated their real logical owner.
  // Observation is admission metadata only, never the reason to acknowledge.
  b.gx->accepted(q);return 0;
 }
 bool quad(const SourceIconGXQuad&q){
  pending=0x800197f8;if(!source()||!begin||!loadedMatrix||!loadedTexture||!material||!ordinary||!ordinary->current()||!loadedTexture->current||!loadedTexture->current(loadedTexture->user))return false;
  if(quadCommitted)return true;
  const auto viewport=nativeCameraRect(b.camera.viewport[0],b.camera.viewport[1],b.camera.viewport[2],b.camera.viewport[3]);
  const auto scissor=nativeCameraRect(float(b.camera.scissor[0]),float(b.camera.scissor[1]),float(b.camera.scissor[2]),float(b.camera.scissor[3]));
  const auto near=b.camera.viewport[4],far=b.camera.viewport[5];if(!viewport||!scissor||!std::isfinite(near)||!std::isfinite(far)||near<0||far>1||near>far)return false;
  CameraViewMatrix affine{};for(unsigned i=0;i<3;++i)for(unsigned j=0;j<4;++j)affine[i][j]=(*loadedMatrix)[4*i+j];
  RetainedIconPacket packet;packet.quad=q;packet.material=*material;packet.transform=nativeCameraTransform(b.camera.projection,affine);packet.viewport=*viewport;packet.scissor=*scissor;packet.depthScale=near-far;packet.depthOffset=1-far;packet.camera=UiCamera;packet.image=loadedTexture->image.index;
  // capture() proved actual source ColorUpdate1/AlphaUpdate1. Every remaining
  // pipeline lane below comes from this original IconGX's reached setters.
  packet.pipeline.color=packet.pipeline.alpha=true;packet.pipeline.sourceBlend=*blend?4:1;packet.pipeline.destinationBlend=*blend?5:0;packet.pipeline.logicOr=false;packet.pipeline.beforeAlpha=*beforeAlpha;packet.pipeline.alphaReject=*alphaReject;packet.pipeline.depth=*depth;packet.pipeline.depthWrite=*depthWrite;packet.pipeline.compare=3;
  if(!validBattleEnemyHpIconPacket(packet))return false;
  // Conservative finite native clip domain: no GPU multiplication/addition
  // prefix may overflow, even where a later cancellation would be finite.
  for(const auto&p:q.positions)for(const auto&row:packet.transform){double bound=std::abs(double(row[3]));for(unsigned j=0;j<3;++j)bound+=std::abs(double(row[j])*p[j]);if(!std::isfinite(bound)||bound>std::numeric_limits<float>::max())return false;}
  if(!b.append(packet))return false;
  quadCommitted=true;
  // append is the atomic source quad owner. Cache acknowledgment removes only
  // intent already retained by that actual building/sealed publication.
  b.pin->acknowledge(packet.image);textures->finish(loadedTexture->handle);pendingImage.reset();return true;
 }
};
BattleWeaponSourceIcon::BattleWeaponSourceIcon()=default;BattleWeaponSourceIcon::~BattleWeaponSourceIcon()=default;
std::unique_ptr<BattleWeaponSourceIcon>BattleWeaponSourceIcon::create(BattleWeaponSourceIconBinding b){
 if(!b.gx||!b.pin||!b.word||!b.half||!b.cameraAddress||!b.matrix||!b.residency||!b.append||!b.retainOrdinary||b.camera.camera!=UiCamera||!b.camera.generation||!b.camera.pass)return {};
 auto p=std::unique_ptr<BattleWeaponSourceIcon>(new BattleWeaponSourceIcon);p->state_=std::make_unique<State>();auto&s=*p->state_;s.b=std::move(b);if(!s.live())return {};s.textures=BattleWeaponIconTextures::create(s.b.textures);if(!s.textures)return {};
 auto&r=s.raw;r.user=&s;
 r.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(s.callback)return {};Guard guard(s.callback);return s.word(a);};
 r.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto&s=*static_cast<State*>(p);if(s.callback)return {};Guard guard(s.callback);return s.half(a);};
 r.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&s=*static_cast<State*>(p);if(s.callback||!s.source())return false;Guard guard(s.callback);const bool result=s.b.matrix(q);return s.source()&&result;};
 r.call=[](void*p,const SourceIconGXCall&q)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(s.callback||!s.drawing||!s.record)return {};Guard guard(s.callback);return s.call(q);};
 r.quad=[](void*p,const SourceIconGXQuad&q){auto&s=*static_cast<State*>(p);if(s.callback||!s.drawing||!s.record)return false;Guard guard(s.callback);return s.quad(q);};
 auto&v=s.services;v.user=&s;v.word=r.word;v.half=r.half;v.matrix=r.matrix;v.gx=&r;
 v.draw=[](void*p,const BattleActMatrix&m,const SourceMatrixIcon&i){auto&s=*static_cast<State*>(p);if(s.callback||s.drawing||!s.source())return false;Guard guard(s.drawing);
  if(s.record){const auto&a=*s.record;if(a.flags!=i.flags||a.id!=i.id||a.width!=i.width||a.height!=i.height||a.texture!=i.texture||a.delay!=i.delay||a.rgba!=i.rgba||std::memcmp(s.recordMatrix.data(),m.data(),sizeof(m)))return false;}
  else{s.record=i;s.recordMatrix=m;}
  if(!resumeSourceIconGX(s.recordMatrix,*s.record,s.raw,s.gxCursor))return false;
  s.record.reset();s.pending=0;return true;
 };
 v.currentCamera=[](void*p)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(s.callback||!s.source())return {};Guard guard(s.callback);const auto result=s.b.cameraAddress({});return s.source()?result:std::nullopt;};
 return p;
}
bool BattleWeaponSourceIcon::resumeMatrix(const BattleAudienceIconRequest&q){if(invoking_||!state_||state_->callback||state_->drawing)return false;Guard guard(invoking_);auto&s=*state_;if(!s.source())return false;
 if(s.outer&&!same(*s.outer,q))return false;
 if(!s.outer)s.outer=q;
 if(!resumeSourceIconMatrix(*s.outer,s.services,s.matrixCursor))return false;
 s.outer.reset();s.pending=0;return true;
}
const SourceIconMatrixServices&BattleWeaponSourceIcon::matrixServices()const{return state_->services;}
bool BattleWeaponSourceIcon::current()const{if(invoking_||!state_||state_->callback||state_->drawing)return false;return state_->live();}
uint32_t BattleWeaponSourceIcon::pending()const{if(!state_)return 0;const auto&s=*state_;if(s.textures->pending())return s.textures->pending();return s.gxCursor.active?s.gxCursor.pending:s.matrixCursor.active?s.matrixCursor.pending:s.pending;}
std::optional<uint32_t>BattleWeaponSourceIcon::pendingImage()const{return state_?state_->pendingImage:std::nullopt;}
}

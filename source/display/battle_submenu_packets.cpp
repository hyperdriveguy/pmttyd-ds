#include "battle_submenu_packets.hpp"
#include "native_camera_draw.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
std::optional<AnimationTPLImage>
readBattleSubmenuTexture(const BattleSubmenuMemoryReader &m,
                             uint32_t palette, uint32_t index) {
  if (!m.word || index < 2 || index > 3)
    return {};
  const auto read = [&](uint32_t a) { return m.word(m.user, a); };
  const auto battle = read(0x8041eb90);
  if (!battle)
    return {};
  const auto descriptor = read(*battle + 0x163f8);
  if (!descriptor || !*descriptor)
    return {};
  const auto holder = read(*descriptor + 0xa0);
  if (!holder || !*holder)
    return {};
  const auto actual = read(*holder);
  if (!actual || *actual != palette)
    return {};
  const uint64_t end = uint64_t(palette) + 217920;
  if (end > 0x100000000ULL)
    return {};
  const auto span = [&](uint32_t a, uint32_t n) {
    return a >= palette && uint64_t(a) + n <= end;
  };
  const auto magic = read(palette), count = read(palette + 4),
             table = read(palette + 8);
  if (!magic || *magic != 0x0020af30 || !count || *count != 97 || !table ||
      !span(*table, 97 * 8))
    return {};
  const auto image = read(*table + 8 * index),
             clut = read(*table + 8 * index + 4);
  if (!image || !span(*image, 36) || !clut || *clut)
    return {};
  const auto dimensions = read(*image), format = read(*image + 4),
             data = read(*image + 8), wrapS = read(*image + 12),
             wrapT = read(*image + 16), min = read(*image + 20),
             mag = read(*image + 24), bias = read(*image + 28),
             flags = read(*image + 32);
  if (!dimensions || !format || *format != 14 || !data || !wrapS || !wrapT ||
      !min || !mag || !bias || !flags)
    return {};
  AnimationTPLImage result;
  result.index = index;
  result.imageHeader = *image;
  result.height = uint16_t(*dimensions >> 16);
  result.width = uint16_t(*dimensions);
  const auto bytes = ((uint32_t(result.width) + 7) / 8) *
                     ((uint32_t(result.height) + 7) / 8) * 32;
  if (!result.width || !result.height || !span(*data, bytes) || *wrapS > 2 ||
      *wrapT > 2 || uint8_t(*flags) != 1)
    return {};
  result.format = *format;
  result.data = *data;
  result.wrapS = *wrapS;
  result.wrapT = *wrapT;
  result.minFilter = *min;
  result.magFilter = *mag;
  const auto bits = *bias;
  std::memcpy(&result.bias, &bits, 4);
  result.edge = uint8_t(*flags >> 24);
  result.minLOD = uint8_t(*flags >> 16);
  result.maxLOD = uint8_t(*flags >> 8);
  if (!std::isfinite(result.bias) || result.minLOD > result.maxLOD)
    return {};
  return result;
}

namespace {
bool cameraCurrent(const BattleSubmenuCameraProof&p){return p.lifetime&&p.current&&p.generation&&p.pass&&p.camera==8&&p.current(p.user,p);}
bool finite(const BattleActMatrix&m){for(float v:m)if(!std::isfinite(v))return false;return true;}
bool image(const AnimationTPLImage&i,uint32_t index){return i.index==index&&i.width==(index==2?16:8)&&i.height==24&&i.format==14&&i.data&&i.imageHeader&&i.wrapS==0&&i.wrapT==0&&i.minFilter==1&&i.magFilter==1&&i.bias==0&&i.edge==0&&i.minLOD==0&&i.maxLOD==0;}
bool objectImage(const std::array<uint32_t,8>&b,const AnimationTPLImage&i){return (b[2]&1023)+1==i.width&&((b[2]>>10)&1023)+1==i.height&&b[5]==i.format;}
bool sameCamera(const BattleSubmenuCameraProof&a,const BattleSubmenuCameraProof&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.user==b.user&&a.current==b.current&&a.generation==b.generation&&a.pass==b.pass&&a.camera==b.camera&&!std::memcmp(&a.projection,&b.projection,sizeof(a.projection))&&!std::memcmp(a.viewport.data(),b.viewport.data(),sizeof(a.viewport))&&a.scissor==b.scissor;}
bool sameLane(const BattleTexturePlaneRequest&a,const BattleTexturePlaneRequest&b){return a.kind==b.kind&&a.integers==b.integers&&a.reals.size()==b.reals.size()&&!std::memcmp(a.reals.data(),b.reals.data(),a.reals.size()*sizeof(float))&&!a.matrix&&!b.matrix;}
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
}
std::optional<std::array<BattleClassVertex,4>> battleSubmenuVertices(const BattleSubmenuPacket&p){
 const auto* r=p.surface.surface(p.originalTexture.index);if(!r||!finite(p.matrix))return {};
 std::array<BattleClassVertex,4> result;
 for(unsigned i=0;i<4;++i){const auto&a=p.positions[i];const auto&uv=p.sourceUV[i];for(float x:a)if(!std::isfinite(x))return {};for(float x:uv)if(!std::isfinite(x)||x<0||x>1)return {};
  auto&v=result[i];v.x=p.matrix[0]*a[0]+p.matrix[1]*a[1]+p.matrix[2]*a[2]+p.matrix[3];v.y=p.matrix[4]*a[0]+p.matrix[5]*a[1]+p.matrix[6]*a[2]+p.matrix[7];v.z=p.matrix[8]*a[0]+p.matrix[9]*a[1]+p.matrix[10]*a[2]+p.matrix[11];
  v.u=r->u0+(r->u1-r->u0)*uv[0];v.v=r->v0+(r->v1-r->v0)*uv[1];v.rgba=p.rgba[i];if(!std::isfinite(v.x)||!std::isfinite(v.y)||!std::isfinite(v.z))return {};
 }
 return result;
}
bool validateBattleSubmenuPacket(const BattleSubmenuPacket&p,const BattleSubmenuSurfaceGpuResource&r){
 if(p.ordinarySnapshot&&(p.ordinarySnapshot->stages()!=1||!p.ordinarySnapshot->current()))return false;
 if(!p.sourceResourceOwner||!p.sourceResourceCurrent||!p.sourceResourceCurrent(p.sourceResourceUser)||!r.accepts(p.surface)||!cameraCurrent(p.camera)||!finite(p.matrix)||!objectImage(p.loadedObject,p.originalTexture)||!image(p.originalTexture,p.originalTexture.index)||!p.surface.surface(p.originalTexture.index))return false;
 for(const auto&row:p.camera.projection)for(float v:row)if(!std::isfinite(v))return false;
 const auto&vp=p.camera.viewport;const auto&sc=p.camera.scissor;
 if(!nativeCameraRect(vp[0],vp[1],vp[2],vp[3])||!std::isfinite(vp[4])||!std::isfinite(vp[5])||vp[4]<0||vp[5]>1||vp[4]>vp[5])return false;
 if(sc[0]>608||sc[1]>480||sc[2]>608||sc[3]>480||!nativeCameraRect(float(sc[0]),float(sc[1]),float(sc[2]),float(sc[3])))return false;
 for(unsigned i=0;i<4;++i){for(float v:p.positions[i])if(!std::isfinite(v))return false;for(float v:p.sourceUV[i])if(!std::isfinite(v)||v<0||v>1)return false;}
 return bool(battleSubmenuVertices(p))&&cameraCurrent(p.camera)&&p.sourceResourceCurrent(p.sourceResourceUser)&&p.surface.current()&&(!p.ordinarySnapshot||p.ordinarySnapshot->current());
}
bool validateBattleSubmenuBatch(const std::vector<BattleSubmenuPacketPtr>&batch,const BattleSubmenuSurfaceGpuResource&r){
 for(const auto&p:batch)if(!p||!validateBattleSubmenuPacket(*p,r)||!sameCamera(p->camera,batch.front()->camera)||p->sourceResourceOwner.get()!=batch.front()->sourceResourceOwner.get()||p->sourceResourceOwner.owner_before(batch.front()->sourceResourceOwner)||batch.front()->sourceResourceOwner.owner_before(p->sourceResourceOwner)||p->sourceResourceUser!=batch.front()->sourceResourceUser||p->sourceResourceCurrent!=batch.front()->sourceResourceCurrent)return false;
 return true;
}
BattleSubmenuPacketProvider::BattleSubmenuPacketProvider(const BattleSubmenuSurfaceGpuResource&r,BattleSubmenuCameraProof p,BattleSubmenuQueryServices q,BattleSubmenuPacketSink sink):resource_(r),lease_(r.lease()),camera_(std::move(p)),query_(std::move(q)),sink_(sink){}
bool BattleSubmenuPacketProvider::current(){if(poisoned_)return false;if(!resource_.accepts(lease_)||!cameraCurrent(camera_)||!query_.lifetime||!query_.current||!query_.current(query_.user)||!query_.sourceResourceCurrent||!query_.sourceResourceCurrent(query_.user)){poisoned_=true;return false;}return true;}
std::optional<uint32_t> BattleSubmenuPacketProvider::request(const BattleTexturePlaneRequest&q){
 if(invoking_)return {};
 Guard guard(invoking_);if(!current())return {};
 using F=BattleTexturePlaneCall;using K=BattleTexturePlaneKind;
 if(q.kind!=K::Call){
  if(!lanes_)return {};
  const unsigned ordinal=lanes_-1,vertex=ordinal/9,lane=ordinal%9;
  auto p=positions_;auto uv=uv_;auto colors=rgba_;
  if(lane>=3&&lane<=6){if(q.kind!=K::FifoByte||q.integers.size()!=1||q.integers[0]>255||!q.reals.empty()||q.matrix)return {};const unsigned shift=24-8*(lane-3);colors[vertex]=(colors[vertex]&~(255u<<shift))|(q.integers[0]<<shift);}
  else{if(q.kind!=K::FifoFloat||q.reals.size()!=1||!q.integers.empty()||q.matrix||!std::isfinite(q.reals[0]))return {};if(lane<3)p[vertex][lane]=q.reals[0];else{if(q.reals[0]<0||q.reals[0]>1)return {};uv[vertex][lane-7]=q.reals[0];}}
  if(ordinal==35){
   if(!pending_){BattleSubmenuPacket packet;packet.surface=lease_;packet.camera=camera_;packet.originalTexture=loaded_->image;packet.loadedObject=*loaded_->bytes;packet.sourceResourceOwner=query_.sourceResourceOwner?query_.sourceResourceOwner:query_.lifetime;packet.sourceProofOwner=query_.lifetime;packet.sourceResourceUser=query_.user;packet.sourceResourceCurrent=query_.sourceResourceCurrent;packet.ordinarySnapshot=loadedOrdinary_;packet.matrix=*matrix_;packet.positions=p;packet.sourceUV=uv;packet.rgba=colors;pending_=std::make_shared<const BattleSubmenuPacket>(std::move(packet));pendingLane_=q;}
   if(!sameLane(q,*pendingLane_)||!sink_.append||!validateBattleSubmenuPacket(*pending_,resource_))return {};
   const bool accepted=sink_.append(sink_.user,pending_);
   if(!current()||!accepted)return {};
   pending_.reset();pendingLane_.reset();lanes_=0;
  }else ++lanes_;
  positions_=p;uv_=uv;rgba_=colors;return 0;
 }
 if(lanes_||!q.reals.empty())return {};
 const auto&n=q.integers;
 if(q.function==F::CurrentCamera){if(!n.empty()||q.matrix||!query_.camera)return {};auto v=query_.camera(query_.user,{});return current()?v:std::nullopt;}
 if(q.function==F::TextureObject){
  if(n.size()!=2||n[1]<2||n[1]>3||q.matrix||!query_.texture)return {};
  auto v=query_.texture(query_.user,n[0],n[1]);if(!current()||!v||!v->bytes||!v->lifetime||!v->current||!v->current(v->user)||!image(v->image,n[1])||!objectImage(*v->bytes,v->image)||!current())return {};
  for(auto&old:objects_)if(old&&old->handle==v->handle)old.reset();
  objects_[n[1]-2]=*v;return v->handle;
 }
 if(q.function==F::LoadTexture){
  if(n.size()!=2||n[1]||q.matrix)return {};
  for(unsigned i=0;i<2;++i)if(objects_[i]&&objects_[i]->handle==n[0]&&objects_[i]->current(objects_[i]->user)&&current()){
   std::shared_ptr<const SourceGxInheritedSnapshot>ordinary;
   if(query_.ordinaryCapture){ordinary=query_.ordinaryCapture(query_.user);if(!ordinary||ordinary->stages()!=1||!ordinary->current()||!current())return {};}
   loaded_=*objects_[i];loadedOrdinary_=std::move(ordinary);selected_=i+2;return 0;}
  return {};
 }
 if(q.function==F::TextureFormat||q.function==F::TextureWidth||q.function==F::TextureHeight){
  if(n.size()!=1||q.matrix)return {};
  for(const auto&o:objects_)if(o&&o->handle==n[0]&&o->current(o->user)&&current()){const auto&b=*o->bytes;return q.function==F::TextureFormat?b[5]:q.function==F::TextureWidth?(b[2]&1023)+1:((b[2]>>10)&1023)+1;}
  return {};
 }
 if(q.function==F::LoadMatrix){if(n!=std::vector<uint32_t>{0}||!q.matrix||!finite(*q.matrix))return {};matrix_=*q.matrix;return 0;}
 if(q.matrix)return {};
 if(q.function==F::Begin){if(n!=std::vector<uint32_t>{128,0,4}||state_!=0x7fff||descriptors_!=14||formats_!=14||!matrix_||!selected_||!sink_.append)return {};lanes_=1;positions_={};uv_={};rgba_={};return 0;}
 uint32_t flag=0;bool valid=false;
 switch(q.function){
 case F::Cull:flag=1;valid=n==std::vector<uint32_t>{0};break;
 case F::DepthLocation:flag=2;valid=n==std::vector<uint32_t>{1};break;
 case F::AlphaCompare:flag=4;valid=n==std::vector<uint32_t>{7,0,0,7,0};break;
 case F::BlendMode:flag=8;valid=n==std::vector<uint32_t>{1,4,5,5}||n==std::vector<uint32_t>{1,4,5,7};break;
 case F::DepthMode:flag=16;valid=n==std::vector<uint32_t>{1,7,0};break;
 case F::ClearDescriptors:valid=n.empty();if(valid){descriptors_=formats_=0;}break;
 case F::Descriptor:valid=n.size()==2&&(n[0]==9||n[0]==11||n[0]==13)&&n[1]==1;if(valid)descriptors_|=1u<<((n[0]-7)/2);break;
 case F::AttributeFormat:valid=n==std::vector<uint32_t>{0,9,1,4,0}||n==std::vector<uint32_t>{0,11,1,5,0}||n==std::vector<uint32_t>{0,13,1,4,0};if(valid)formats_|=1u<<((n[1]-7)/2);break;
 case F::Channels:flag=32;valid=n==std::vector<uint32_t>{1};break;
 case F::ChannelControl:flag=64;valid=n==std::vector<uint32_t>{4,0,1,1,0,2,2}||n==std::vector<uint32_t>{4,0,0,1,0,2,2};break;
 case F::TextureGenerators:flag=128;valid=n==std::vector<uint32_t>{1};break;
 case F::TextureGenerator:flag=256;valid=n==std::vector<uint32_t>{0,1,4,60,0,125};break;
 case F::TevStages:flag=512;valid=n==std::vector<uint32_t>{1};break;
 case F::ColorOp:flag=1024;valid=n==std::vector<uint32_t>{0,0,0,0,1,0};break;
 case F::AlphaOp:flag=2048;valid=n==std::vector<uint32_t>{0,0,0,0,1,0};break;
 case F::AlphaIn:flag=4096;valid=n==std::vector<uint32_t>{0,7,5,4,7};break;
 case F::Order:flag=8192;valid=n==std::vector<uint32_t>{0,0,0,4};break;
 case F::ColorIn:flag=16384;valid=n==std::vector<uint32_t>{0,15,10,8,15};break;
 case F::TevOp:valid=n==std::vector<uint32_t>{0,0};if(valid)state_|=1024|2048|4096|8192|16384;break;
 default:break;
 }
 if(!valid)return {};
 state_|=flag;return 0;
}
}

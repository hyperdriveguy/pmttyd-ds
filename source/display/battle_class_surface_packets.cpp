#include "battle_class_surface_packets.hpp"
#include "native_camera_draw.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
std::optional<AnimationTPLImage>
readBattleClassSurfaceTexture(const BattleSubmenuMemoryReader &m,
                             uint32_t palette, uint32_t index) {
  if (!m.word || index > 1)
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
  if (!dimensions || !format || *format != 0 || !data || !wrapS || !wrapT ||
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
struct Guard{bool&v;explicit Guard(bool&x):v(x){v=true;}~Guard(){v=false;}};
bool cameraCurrent(const BattleSubmenuCameraProof&p){return p.lifetime&&p.current&&p.generation&&p.pass&&p.camera==8&&p.current(p.user,p);}
bool finite(const BattleActMatrix&m){for(float v:m)if(!std::isfinite(v))return false;return true;}
bool image(const AnimationTPLImage&i,uint32_t index){return index<2&&i.index==index&&i.width==(index?128:40)&&i.height==(index?80:40)&&i.format==0&&i.data&&i.imageHeader&&i.wrapS==0&&i.wrapT==0&&i.minFilter==1&&i.magFilter==1&&i.bias==0&&i.edge==0&&i.minLOD==0&&i.maxLOD==0&&!i.paletteHeader&&!i.paletteData&&!i.paletteFormat&&!i.paletteCount;}
bool objectImage(const std::array<uint32_t,8>&b,const AnimationTPLImage&i){return (b[2]&1023)+1==i.width&&((b[2]>>10)&1023)+1==i.height&&b[5]==i.format;}
bool sameLane(const BattleTexturePlaneRequest&a,const BattleTexturePlaneRequest&b){return a.kind==b.kind&&a.integers==b.integers&&a.reals.size()==b.reals.size()&&(a.reals.empty()||!std::memcmp(a.reals.data(),b.reals.data(),a.reals.size()*sizeof(float)))&&!a.matrix&&!b.matrix;}
// Conservative binary32 product/sum checks precede all packet admission. They
// preserve finite offscreen/singular/transparent geometry, not visibility culls.
bool transform(const float*m,const std::array<float,3>&v,float&out){float sum=0;for(unsigned k=0;k<4;++k){const volatile float product=m[k]*(k==3?1:v[k]);if(!std::isfinite(product))return false;const volatile float next=sum+product;if(!std::isfinite(next))return false;sum=next;}out=sum;return true;}
}
class BattleClassSurfaceObjectSnapshot {
 uint32_t handle_;AnimationTPLImage image_;std::array<uint32_t,8>bytes_;
 std::shared_ptr<const void>bank_,identity_;void*user_;bool(*current_)(void*);bool(*ordinary_)(void*);
 static bool imageEqual(const AnimationTPLImage&a,const AnimationTPLImage&b){
  return a.index==b.index&&a.imageHeader==b.imageHeader&&a.paletteHeader==b.paletteHeader&&a.data==b.data&&a.format==b.format&&a.wrapS==b.wrapS&&a.wrapT==b.wrapT&&a.minFilter==b.minFilter&&a.magFilter==b.magFilter&&a.paletteData==b.paletteData&&a.paletteFormat==b.paletteFormat&&a.width==b.width&&a.height==b.height&&a.paletteCount==b.paletteCount&&!std::memcmp(&a.bias,&b.bias,4)&&a.edge==b.edge&&a.minLOD==b.minLOD&&a.maxLOD==b.maxLOD;
 }
 static bool live(void*p){const auto&s=*static_cast<const BattleClassSurfaceObjectSnapshot*>(p);return s.bank_&&s.identity_&&s.current_&&s.current_(s.user_);}
 static bool witness(void*p,const BattleClassSurfaceTextureObject&o){return static_cast<const BattleClassSurfaceObjectSnapshot*>(p)->accepts(o);}
 BattleClassSurfaceObjectSnapshot(const BattleClassSurfaceTextureObject&o,const BattleClassSurfaceQueryServices&q):handle_(o.handle),image_(o.image),bytes_(*o.bytes),bank_(q.lifetime),identity_(q.sourceResourceOwner),user_(q.user),current_(q.sourceResourceCurrent),ordinary_(q.ordinaryCurrent){}
 friend class BattleClassSurfacePacketProvider;
public:
 BattleClassSurfaceTextureObject captured(const std::shared_ptr<const BattleClassSurfaceObjectSnapshot>&owner)const{
  BattleClassSurfaceTextureObject o;o.handle=handle_;o.image=image_;o.bytes=bytes_;o.lifetime=owner;o.user=const_cast<BattleClassSurfaceObjectSnapshot*>(this);o.current=&live;o.witness=&witness;return o;
 }
 bool owns(const BattleClassSurfacePacket&p)const{
  return p.sourceResourceOwner==identity_&&!p.sourceResourceOwner.owner_before(identity_)&&!identity_.owner_before(p.sourceResourceOwner)&&p.sourceProofOwner==bank_&&!p.sourceProofOwner.owner_before(bank_)&&!bank_.owner_before(p.sourceProofOwner)&&p.sourceResourceUser==user_&&p.sourceResourceCurrent==current_&&p.ordinaryCurrent==ordinary_;
 }
 bool accepts(const BattleClassSurfaceTextureObject&o)const{
  // Compare identity/canonical copied values BEFORE invoking any supplied raw
  // object callback. No original temporary byte read is part of this proof.
  return o.handle==handle_&&o.bytes&&*o.bytes==bytes_&&imageEqual(o.image,image_)&&o.lifetime.get()==this&&o.user==this&&o.current==&live&&o.witness==&witness&&live(const_cast<BattleClassSurfaceObjectSnapshot*>(this));
 }
};
std::optional<std::array<BattleClassVertex,4>> battleClassSurfaceVertices(const BattleClassSurfacePacket&p){
 if(!p.surface.assets||!finite(p.matrix))return {};
 const auto*r=p.surface.assets->surface(p.object.image.index);if(!r)return {};
 std::array<BattleClassVertex,4>out;
 for(unsigned i=0;i<4;++i){const auto&xyz=p.positions[i];const auto&uv=p.sourceUV[i];for(float v:xyz)if(!std::isfinite(v))return {};for(float v:uv)if(!std::isfinite(v)||v<0||v>1)return {};
  auto&o=out[i];if(!transform(p.matrix.data(),xyz,o.x)||!transform(p.matrix.data()+4,xyz,o.y)||!transform(p.matrix.data()+8,xyz,o.z))return {};
  o.u=r->u0+(r->u1-r->u0)*uv[0];o.v=r->v0+(r->v1-r->v0)*uv[1];o.rgba=p.rgba[i];if(!std::isfinite(o.u)||!std::isfinite(o.v))return {};
  const CameraViewMatrix identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};const auto projection=nativeCameraTransform(p.camera.projection,identity);
  const std::array<float,3>native{o.x,o.y,o.z};for(const auto&row:projection){float ignored;if(!transform(row.data(),native,ignored))return {};}
 }
 return out;
}
bool validateBattleClassSurfacePacket(const BattleClassSurfacePacket&p,const BattleClassSurfaceGpuResource&r){
 const auto&o=p.object;
 if(!p.objectSnapshot||!p.objectSnapshot->owns(p))return false;
 if(!p.sourceResourceOwner||!p.sourceProofOwner||!p.sourceResourceCurrent||!p.sourceResourceCurrent(p.sourceResourceUser)||!p.ordinaryCurrent||!p.ordinaryCurrent(p.sourceResourceUser)||!r.accepts(p.surface)||!cameraCurrent(p.camera)||!p.objectSnapshot||o.lifetime.owner_before(p.objectSnapshot)||p.objectSnapshot.owner_before(o.lifetime)||!p.objectSnapshot->accepts(o)||!o.bytes||!image(o.image,o.image.index)||!objectImage(*o.bytes,o.image))return false;
 if(p.rgb!=BattleClassSurfaceRGB::TextureRaster&&p.rgb!=BattleClassSurfaceRGB::MaterialRaster)return false;
 for(const auto&row:p.camera.projection)for(float v:row)if(!std::isfinite(v))return false;
 const auto&v=p.camera.viewport;const auto&s=p.camera.scissor;
 if(!nativeCameraRect(v[0],v[1],v[2],v[3])||!std::isfinite(v[4])||!std::isfinite(v[5])||v[4]<0||v[5]>1||v[4]>v[5]||s[0]>608||s[1]>480||s[2]>608||s[3]>480||!nativeCameraRect(float(s[0]),float(s[1]),float(s[2]),float(s[3])))return false;
 return bool(battleClassSurfaceVertices(p))&&p.objectSnapshot->accepts(o)&&cameraCurrent(p.camera)&&p.sourceResourceCurrent(p.sourceResourceUser)&&p.ordinaryCurrent(p.sourceResourceUser)&&r.accepts(p.surface);
}
BattleClassSurfacePacketProvider::BattleClassSurfacePacketProvider(const BattleClassSurfaceGpuResource&r,BattleSubmenuCameraProof p,BattleClassSurfaceQueryServices q,BattleClassSurfacePacketSink sink):resource_(r),lease_(r.lease()),camera_(std::move(p)),query_(std::move(q)),sink_(sink){}
bool BattleClassSurfacePacketProvider::current(){
 if(poisoned_)return false;
 if(!resource_.accepts(lease_)||!cameraCurrent(camera_)||!query_.lifetime||!query_.sourceResourceOwner||!query_.current||!query_.current(query_.user)||!query_.sourceResourceCurrent||!query_.sourceResourceCurrent(query_.user)){poisoned_=true;return false;}
 if(!query_.ordinaryCurrent||!query_.ordinaryCurrent(query_.user)){if(ordinaryKnown_)poisoned_=true;return false;}
 ordinaryKnown_=true;return true;
}
std::optional<uint32_t> BattleClassSurfacePacketProvider::request(const BattleTexturePlaneRequest&q){
 if(invoking_)return {};
 Guard guard(invoking_);if(!current())return {};
 using F=BattleTexturePlaneCall;using K=BattleTexturePlaneKind;
 if(q.kind!=K::Call){
  if(!lanes_)return {};
  if(!loaded_||!loadedSnapshot_||!loadedSnapshot_->accepts(*loaded_)){poisoned_=true;return {};}
  const unsigned ordinal=lanes_-1,vertex=ordinal/9,lane=ordinal%9;
  auto p=positions_;auto uv=uv_;auto colors=rgba_;
  if(lane>=3&&lane<=6){if(q.kind!=K::FifoByte||q.integers.size()!=1||q.integers[0]>255||!q.reals.empty()||q.matrix)return {};const unsigned shift=24-8*(lane-3);colors[vertex]=(colors[vertex]&~(255u<<shift))|(q.integers[0]<<shift);}
  else{if(q.kind!=K::FifoFloat||q.reals.size()!=1||!q.integers.empty()||q.matrix||!std::isfinite(q.reals[0]))return {};if(lane<3)p[vertex][lane]=q.reals[0];else{if(q.reals[0]<0||q.reals[0]>1)return {};uv[vertex][lane-7]=q.reals[0];}}
  if(ordinal==35){
   if(!pending_){BattleClassSurfacePacket packet;packet.surface=lease_;packet.camera=camera_;packet.object=*loaded_;packet.objectSnapshot=loadedSnapshot_;packet.rgb=rgb_;packet.material=material_.value_or(0xffffffff);packet.sourceResourceOwner=query_.sourceResourceOwner;packet.sourceProofOwner=query_.lifetime;packet.sourceResourceUser=query_.user;packet.sourceResourceCurrent=query_.sourceResourceCurrent;packet.ordinaryCurrent=query_.ordinaryCurrent;packet.matrix=*matrix_;packet.positions=p;packet.sourceUV=uv;packet.rgba=colors;pending_=std::make_shared<const BattleClassSurfacePacket>(std::move(packet));pendingLane_=q;}
   if(!sameLane(q,*pendingLane_)||!sink_.append||!validateBattleClassSurfacePacket(*pending_,resource_))return {};
   const bool accepted=sink_.append(sink_.user,pending_);
   if(!current()||!accepted)return {};
   if(!validateBattleClassSurfacePacket(*pending_,resource_)){poisoned_=true;return {};}
   pending_.reset();pendingLane_.reset();lanes_=0;
  }else ++lanes_;
  positions_=p;uv_=uv;rgba_=colors;return 0;
 }
 if(lanes_||!q.reals.empty())return {};
 const auto&n=q.integers;
 if(q.function==F::CurrentCamera){if(!n.empty()||q.matrix||!query_.camera)return {};auto v=query_.camera(query_.user,{});return current()?v:std::nullopt;}
 if(q.function==F::TextureObject){
  if(n.size()!=2||n[1]>1||q.matrix||!query_.texture)return {};
  auto v=query_.texture(query_.user,n[0],n[1]);if(!current()||!v||!v->bytes||!v->lifetime||!v->current||!v->current(v->user)||!v->witness||!v->witness(v->user,*v)||!image(v->image,n[1])||!v->handle||!objectImage(*v->bytes,v->image)||!current())return {};
  for(auto&old:objects_)if(old&&old->handle==v->handle)old.reset();
  objects_[n[1]]=*v;return v->handle;
 }
 if(q.function==F::LoadTexture){
  if(n.size()!=2||n[1]||q.matrix)return {};
  for(unsigned i=0;i<2;++i)if(objects_[i]&&objects_[i]->handle==n[0]&&objects_[i]->current(objects_[i]->user)&&objects_[i]->witness(objects_[i]->user,*objects_[i])&&current()){
   const auto snapshot=std::shared_ptr<const BattleClassSurfaceObjectSnapshot>(new BattleClassSurfaceObjectSnapshot(*objects_[i],query_));
   const auto captured=snapshot->captured(snapshot);if(!snapshot->accepts(captured)||!current())return {};
   loaded_=captured;loadedSnapshot_=snapshot;return 0;
  }
  return {};
 }
 if(q.function==F::TextureFormat||q.function==F::TextureWidth||q.function==F::TextureHeight){
  if(n.size()!=1||q.matrix)return {};
  for(const auto&o:objects_)if(o&&o->handle==n[0]&&o->current(o->user)&&o->witness(o->user,*o)&&current()){const auto&b=*o->bytes;return q.function==F::TextureFormat?b[5]:q.function==F::TextureWidth?(b[2]&1023)+1:((b[2]>>10)&1023)+1;}
  return {};
 }
 if(q.function==F::LoadMatrix){if(n!=std::vector<uint32_t>{0}||!q.matrix||!finite(*q.matrix))return {};matrix_=*q.matrix;return 0;}
 if(q.matrix)return {};
 if(uint32_t(q.function)==0x802b8ab8){if(n.size()!=2||n[0]!=1)return {};material_=n[1];return 0;}
 if(q.function==F::Begin){if(n!=std::vector<uint32_t>{128,0,4}||state_!=0x7fff||descriptors_!=14||formats_!=14||!matrix_||!loaded_||(rgb_==BattleClassSurfaceRGB::MaterialRaster&&!material_)||!sink_.append)return {};if(!loadedSnapshot_||!loadedSnapshot_->accepts(*loaded_)){poisoned_=true;return {};}lanes_=1;positions_={};uv_={};rgba_={};return 0;}
 uint32_t flag=0;bool valid=false;
 switch(q.function){
 case F::Cull:flag=1;valid=n==std::vector<uint32_t>{0};break;
 case F::DepthLocation:flag=2;valid=n==std::vector<uint32_t>{1};break;
 case F::AlphaCompare:flag=4;valid=n==std::vector<uint32_t>{7,0,0,7,0};break;
 case F::BlendMode:flag=8;valid=n==std::vector<uint32_t>{1,4,5,5};break;
 case F::DepthMode:flag=16;valid=n==std::vector<uint32_t>{1,7,0};break;
 case F::ClearDescriptors:valid=n.empty();if(valid){descriptors_=formats_=0;}break;
 case F::Descriptor:valid=n.size()==2&&(n[0]==9||n[0]==11||n[0]==13)&&n[1]==1;if(valid)descriptors_|=1u<<((n[0]-7)/2);break;
 case F::AttributeFormat:valid=n==std::vector<uint32_t>{0,9,1,4,0}||n==std::vector<uint32_t>{0,11,1,5,0}||n==std::vector<uint32_t>{0,13,1,4,0};if(valid)formats_|=1u<<((n[1]-7)/2);break;
 case F::Channels:flag=32;valid=n==std::vector<uint32_t>{1};break;
 case F::ChannelControl:flag=64;valid=n==std::vector<uint32_t>{4,0,1,1,0,2,2};break;
 case F::TextureGenerators:flag=128;valid=n==std::vector<uint32_t>{1};break;
 case F::TextureGenerator:flag=256;valid=n==std::vector<uint32_t>{0,1,4,60,0,125};break;
 case F::TevStages:flag=512;valid=n==std::vector<uint32_t>{1};break;
 case F::ColorOp:flag=1024;valid=n==std::vector<uint32_t>{0,0,0,0,1,0};break;
 case F::AlphaOp:flag=2048;valid=n==std::vector<uint32_t>{0,0,0,0,1,0};break;
 case F::AlphaIn:flag=4096;valid=n==std::vector<uint32_t>{0,7,5,4,7};break;
 case F::Order:flag=8192;valid=n==std::vector<uint32_t>{0,0,0,4};break;
 case F::ColorIn:flag=16384;valid=n==std::vector<uint32_t>{0,15,10,8,15}||n==std::vector<uint32_t>{0,15,10,2,15};if(valid)rgb_=n[3]==2?BattleClassSurfaceRGB::MaterialRaster:BattleClassSurfaceRGB::TextureRaster;break;
 default:break;
 }
 if(!valid)return {};
 state_|=flag;return 0;
}
}

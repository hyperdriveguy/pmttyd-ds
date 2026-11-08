#include "battle_weapon_icon_textures.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {namespace {
constexpr uint32_t ObjectBytes=32,ImageDescriptorBytes=36,AllDescriptorFields=(1u<<13)-1;
constexpr uint32_t TextureDimensionBits=10,TextureDimensionMask=(1u<<TextureDimensionBits)-1;
constexpr uint32_t IconPaletteGlobal=0x8041e630,TextureWidth=0x802b76c4,TextureHeight=0x802b76d4;
struct Guard{bool&value;explicit Guard(bool&v):value(v){value=true;}~Guard(){value=false;}};
struct LoadedProof {
 std::shared_ptr<NativeSourceIconProcess>process;std::shared_ptr<const void>owner;
 std::function<bool()>current;bool invalid=false,checking=false;
 static bool check(void*p){auto&c=*static_cast<LoadedProof*>(p);if(c.invalid||c.checking)return false;Guard guard(c.checking);if(!c.process||!c.owner||!c.process->current()||!c.current||!c.current())c.invalid=true;return !c.invalid;}
};
}
struct BattleWeaponIconTextures::State {
 BattleWeaponIconTextureBinding b;SourcePrivateTextureObject object;
 SourcePaletteTextureObjectServices services;SourcePaletteTextureObjectCursor tex;
 SourceGxTextureObjectCursor lodCursor;std::optional<SourceGxTextureObjectInput>lodInput;
 uint32_t palette=0,index=0,table=0,observed=0,clearPosition=0;
 bool held=false,complete=false,lodComplete=false;mutable bool invalid=false,checking=false;
 AnimationTPLImage image{};
 bool live()const{if(invalid||checking)return false;Guard guard(checking);
  const bool ok=b.process&&b.process->current()&&b.lifetime&&b.longCurrent&&b.longCurrent()&&b.unmapped&&b.unmapped(b.privateAddress,ObjectBytes)&&!b.process->owns(b.privateAddress,ObjectBytes);
  if(!ok)invalid=true;
  return ok;
 }
 bool source()const{return live()&&b.sourceCurrent&&b.sourceCurrent();}
 void observe(uint32_t site,uint32_t v){switch(site){
 case 0x80037aa0:table=v;break;
 case 0x80037aa4:image.imageHeader=v;observed|=1;break;
 case 0x80037ac4:image.data=v;observed|=2;break;
 case 0x80037acc:image.width=uint16_t(v);observed|=4;break;
 case 0x80037ad0:image.height=uint16_t(v);observed|=8;break;
 case 0x80037ad4:image.format=v;observed|=16;break;
 case 0x80037ad8:image.wrapS=v;observed|=32;break;
 case 0x80037adc:image.wrapT=v;observed|=64;break;
 case 0x80037af4:image.minLOD=uint8_t(v);observed|=128;break;
 case 0x80037afc:image.maxLOD=uint8_t(v);observed|=256;break;
 case 0x80037b20:image.minFilter=v;observed|=512;break;
 case 0x80037b28:image.magFilter=v;observed|=1024;break;
 case 0x80037b30:std::memcpy(&image.bias,&v,4);observed|=2048;break;
 case 0x80037b38:image.edge=uint8_t(v);observed|=4096;break;
 default:break;
 }}
 std::optional<uint32_t>read(uint32_t a,uint32_t n){if(!source())return {};std::optional<uint32_t>v;
  if(object.overlaps(a,n))v=object.read(a,n);
  else if(n==4)v=b.process->word(a);
  else if(n==2){auto x=b.process->half(a);if(x)v=*x;}
  else if(n==1){auto x=b.process->byte(a);if(x)v=*x;}
  if(!source())return {};
  if(v)observe(tex.pending(),*v);
  return v;
 }
 bool descriptor()const{
  if(observed!=AllDescriptorFields||!complete||!image.width||!image.height||!std::isfinite(image.bias)||image.wrapS>2||image.wrapT>2||image.minLOD>image.maxLOD)return false;
  const auto assets=b.process->assets();if(!assets)return false;
  const uint64_t end=uint64_t(palette)+assets->tpl()->bytes().size();
  const auto inside=[&](uint32_t a,uint64_t n){return a>=palette&&uint64_t(a)+n<=end;};
  if(!inside(image.imageHeader,ImageDescriptorBytes)||!inside(table,uint64_t(index+1)*8))return false;
  const auto p=b.process->word(table+8*index+4);if(!p||*p)return false; // Actual613-image bank has no TLUT.
  uint64_t pixels=0;
  switch(image.format){case 0:pixels=((uint64_t(image.width)+7)/8)*((uint64_t(image.height)+7)/8)*32;break;
  case 2:pixels=((uint64_t(image.width)+7)/8)*((uint64_t(image.height)+3)/4)*32;break;
  case 5:pixels=((uint64_t(image.width)+3)/4)*((uint64_t(image.height)+3)/4)*32;break;
  case 14:pixels=((uint64_t(image.width)+7)/8)*((uint64_t(image.height)+7)/8)*32;break;
  default:return false;}
  return inside(image.data,pixels);
 }
};
BattleWeaponIconTextures::BattleWeaponIconTextures()=default;BattleWeaponIconTextures::~BattleWeaponIconTextures()=default;
std::unique_ptr<BattleWeaponIconTextures>BattleWeaponIconTextures::create(BattleWeaponIconTextureBinding b){
 if(!b.process||!b.lifetime||!b.longCurrent||!b.sourceCurrent||!b.unmapped||!b.truncate||!b.privateAddress||uint64_t(b.privateAddress)+ObjectBytes>0x100000000ULL)return {};
 auto p=std::unique_ptr<BattleWeaponIconTextures>(new BattleWeaponIconTextures);p->state_=std::make_unique<State>();auto&s=*p->state_;s.b=std::move(b);if(!s.live())return {};
 if(!s.object.bind(s.b.privateAddress,s.b.lifetime,&s,[](void*p){return static_cast<State*>(p)->source();}))return {};
 auto&g=s.services.gx;g.user=&s;g.lifetime=s.b.lifetime;g.current=[](void*p){return static_cast<State*>(p)->source();};
 g.word=[](void*p,uint32_t a){return static_cast<State*>(p)->read(a,4);};
 g.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{auto v=static_cast<State*>(p)->read(a,1);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
 s.services.half=[](void*p,uint32_t a)->std::optional<uint16_t>{auto v=static_cast<State*>(p)->read(a,2);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;};
 g.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto&s=*static_cast<State*>(p);return s.source()&&s.object.store(a,n,v)&&s.source();};
 g.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(!s.source()||a!=s.b.privateAddress)return {};while(s.clearPosition<ObjectBytes){if(!s.object.store(a+s.clearPosition,1,0)||!s.source())return {};++s.clearPosition;}return a;};
 g.truncate=[](void*p,float v)->std::optional<int32_t>{auto&s=*static_cast<State*>(p);if(!s.source())return {};auto result=s.b.truncate(v);return s.source()?result:std::nullopt;};
 s.services.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&s=*static_cast<State*>(p);if(!s.source()||a>UINT32_MAX-4)return {};auto hi=s.b.process->word(a),lo=s.b.process->word(a+4);if(!hi||!lo||!s.source())return {};return uint64_t(*hi)<<32|*lo;};
 s.services.subtractSingle=[](void*p,uint64_t a,uint64_t b)->std::optional<uint32_t>{auto&s=*static_cast<State*>(p);if(!s.source()||b!=0x4330000000000000ULL||(a&0xffffffffffffff00ULL)!=0x4330000000000000ULL)return {};
  // Exact finite original unsigned-byte bias conversion. All256 integer
  // results are exactly representable in binary32; no general FPSCR policy.
  const float value=float(uint8_t(a));uint32_t bits;std::memcpy(&bits,&value,4);return bits;};
 return p;
}
std::optional<uint32_t>BattleWeaponIconTextures::construct(uint32_t palette,uint32_t index){
 if(invoking_||!state_)return {};
 Guard guard(invoking_);auto&s=*state_;if(!s.source())return {};
 const auto actual=s.b.process->word(IconPaletteGlobal);if(!actual||*actual!=palette)return {};
 // Explicit native resource domain; the original TEX body itself has no clamp.
 const auto count=s.b.process->word(palette+4);if(!count||index>=*count)return {};
 if(s.held&&(s.palette!=palette||s.index!=index))return {};
 if(!s.held){s.held=true;s.complete=s.lodComplete=false;s.palette=palette;s.index=index;s.clearPosition=s.observed=0;s.image={};s.image.index=index;s.lodInput.reset();}
 if(!s.complete){if(!resumeSourcePaletteTextureObject(palette,s.b.privateAddress,index,s.services,s.tex)||!s.source())return {};s.complete=true;}
 return s.descriptor()?std::optional<uint32_t>(s.b.privateAddress):std::nullopt;
}
bool BattleWeaponIconTextures::lod(const SourceIconGXCall&q){
 if(invoking_||!state_)return false;
 Guard guard(invoking_);auto&s=*state_;
 if(!s.source()||!s.held||!s.complete||q.function!=0x802b7544||q.integers[0]!=s.b.privateAddress||q.integers[6]||q.integers[7]||q.reals[3]!=0)return false;
 SourceGxTextureObjectInput input;input.integers=q.integers;for(unsigned i=0;i<3;++i)std::memcpy(&input.reals[i],&q.reals[i],4);
 if(s.lodInput&&(s.lodInput->integers!=input.integers||s.lodInput->reals!=input.reals))return false;
 if(!s.lodInput)s.lodInput=input;
 if(!s.lodComplete){if(!resumeSourceGxTextureObject(q.function,input,s.services.gx,s.lodCursor)||!s.source())return false;s.lodComplete=true;}return true;
}
std::optional<BattleSubmenuTextureObject>BattleWeaponIconTextures::snapshot(uint32_t handle)const{
 if(invoking_||!state_)return {};
 Guard guard(invoking_);auto&s=*state_;if(handle!=s.b.privateAddress||!s.source()||!s.held||!s.complete||!s.lodComplete||!s.descriptor())return {};
 const auto bytes=s.object.words();if(!bytes||!s.source())return {};
 auto proof=std::make_shared<LoadedProof>();proof->process=s.b.process;proof->owner=s.b.lifetime;proof->current=s.b.longCurrent;
 BattleSubmenuTextureObject result;result.handle=handle;result.image=s.image;result.bytes=*bytes;result.lifetime=proof;result.user=proof.get();result.current=LoadedProof::check;return result;
}
std::optional<uint32_t>BattleWeaponIconTextures::dimension(uint32_t function,uint32_t handle)const{
 if(invoking_||!state_)return {};
 Guard guard(invoking_);auto&s=*state_;if(!s.source()||handle!=s.b.privateAddress||!s.held||!s.complete||(function!=TextureWidth&&function!=TextureHeight))return {};
 const auto word=s.object.read(handle+8,4);if(!word||!s.source())return {};return (function==TextureWidth?(*word&TextureDimensionMask):((*word>>TextureDimensionBits)&TextureDimensionMask))+1;
}
bool BattleWeaponIconTextures::finish(uint32_t handle){
 if(invoking_||!state_)return false;
 Guard guard(invoking_);auto&s=*state_;if(!s.source()||handle!=s.b.privateAddress||!s.held||!s.complete||!s.lodComplete)return false;
 s.held=s.complete=s.lodComplete=false;s.lodInput.reset();return true;
}
bool BattleWeaponIconTextures::current()const{if(invoking_||!state_)return false;
 Guard guard(invoking_);return state_->live();}
uint32_t BattleWeaponIconTextures::pending()const{if(!state_)return 0;auto&s=*state_;return s.tex.active()?(s.tex.childPending()?s.tex.childPending():s.tex.pending()):s.lodCursor.pending();}
}

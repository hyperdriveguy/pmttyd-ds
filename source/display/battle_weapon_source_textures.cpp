#include "battle_weapon_source_textures.hpp"
#include <cstring>
namespace ttyd {namespace {
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
// Descriptor identity compares bias by binary32 bits, including signed zero.
bool same(const AnimationTPLImage&a,const AnimationTPLImage&b){
 return a.index==b.index&&a.imageHeader==b.imageHeader&&a.paletteHeader==b.paletteHeader&&a.data==b.data&&a.format==b.format&&a.wrapS==b.wrapS&&a.wrapT==b.wrapT&&a.minFilter==b.minFilter&&a.magFilter==b.magFilter&&a.paletteData==b.paletteData&&a.paletteFormat==b.paletteFormat&&a.width==b.width&&a.height==b.height&&a.paletteCount==b.paletteCount&&std::memcmp(&a.bias,&b.bias,4)==0&&a.edge==b.edge&&a.minLOD==b.minLOD&&a.maxLOD==b.maxLOD;
}
}
// Long proof covers the real UI/common bank and exclusion of ALL 32 private
// output bytes from actual source owners. It is independent of the short callback.
bool BattleWeaponSourceTextures::longCurrent()const{
 if(invalid_)return false;
 const bool ok=memory_&&memory_->current()&&!memory_->owns(address_,32)&&source_.native.sourceResourceOwner&&source_.native.allocationCurrent&&source_.native.allocationCurrent()&&source_.native.sourceResourceCurrent&&source_.native.sourceResourceCurrent()&&source_.native.addresses.unmapped&&source_.native.addresses.unmapped(address_,32);
 if(!ok)invalid_=true;
 return ok;
}
bool BattleWeaponSourceTextures::current()const{if(invoking_)return false;Guard g(invoking_);return longCurrent();}
bool BattleWeaponSourceTextures::sourceCurrent()const{return longCurrent()&&sourceCurrent_&&sourceCurrent_();}
uint32_t BattleWeaponSourceTextures::pending()const{return cursor_.childPending()?cursor_.childPending():cursor_.pending();}
std::shared_ptr<BattleWeaponSourceTextures>BattleWeaponSourceTextures::create(TrioWeaponMenuFrameBinding source,std::shared_ptr<BattleWeaponSourceMemory>memory,uint32_t address,std::function<bool()>current,std::function<std::optional<int32_t>(float)>truncate){
 // Thirteen native camera slots use 0x400-byte stride. This 32-byte SDK
 // output must not overlap that reservation or wrap across the 2^32 boundary.
 const uint64_t cameraBegin=source.native.addresses.base,cameraEnd=cameraBegin+13*0x400;
 if(!memory||!current||!truncate||!address||uint64_t(address)+32>0x100000000ULL||(uint64_t(address)<cameraEnd&&cameraBegin<uint64_t(address)+32))return {};
 auto p=std::shared_ptr<BattleWeaponSourceTextures>(new BattleWeaponSourceTextures);p->source_=std::move(source);p->memory_=std::move(memory);p->address_=address;p->sourceCurrent_=std::move(current);p->truncate_=std::move(truncate);
 if(!p->current())return {};
 auto&s=p->services_;s.gx.user=p.get();s.gx.lifetime=p->memory_;s.gx.current=[](void*p){return static_cast<BattleWeaponSourceTextures*>(p)->sourceCurrent();};
 s.gx.word=[](void*p,uint32_t a){return static_cast<BattleWeaponSourceTextures*>(p)->read(a,4);};
 s.gx.byte=[](void*p,uint32_t a)->std::optional<uint8_t>{const auto v=static_cast<BattleWeaponSourceTextures*>(p)->read(a,1);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;};
 s.half=[](void*p,uint32_t a)->std::optional<uint16_t>{const auto v=static_cast<BattleWeaponSourceTextures*>(p)->read(a,2);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;};
 s.gx.store=[](void*p,uint32_t a,uint32_t n,uint32_t v){auto&t=*static_cast<BattleWeaponSourceTextures*>(p);return t.sourceCurrent()&&t.object_.store(a,n,v)&&t.sourceCurrent();};
 // Whole source memset is retained byte by byte. Unknown private output bytes
 // become known only through actual clear/stores, not at native allocation time.
 s.gx.clear=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&t=*static_cast<BattleWeaponSourceTextures*>(p);if(!t.sourceCurrent()||a!=t.address_)return {};while(t.clearPosition_<32){if(!t.object_.store(a+t.clearPosition_,1,0)||!t.sourceCurrent())return {};++t.clearPosition_;}return a;};
 s.gx.truncate=[](void*p,float v)->std::optional<int32_t>{auto&t=*static_cast<BattleWeaponSourceTextures*>(p);if(!t.sourceCurrent())return {};const auto n=t.truncate_(v);return t.sourceCurrent()?n:std::nullopt;};
 // Original double words join high/low big-endian lanes; protect the second
 // word against guest-address wrap before issuing its actual memory read.
 s.doubleWord=[](void*p,uint32_t a)->std::optional<uint64_t>{auto&t=*static_cast<BattleWeaponSourceTextures*>(p);if(!t.sourceCurrent())return {};const auto hi=t.memory_->word(a),lo=a<=UINT32_MAX-4?t.memory_->word(a+4):std::nullopt;if(!hi||!lo||!t.sourceCurrent())return {};return uint64_t(*hi)<<32|*lo;};
 // Exact admitted unsigned-byte conversion: binary64 (2^52 + U8) - 2^52.
 // 0x4330000000000000 is that bias; the mask forbids every other input domain.
 // Returning float(U8) bits is exact here, not a generic double/FPSCR emulator.
 s.subtractSingle=[](void*p,uint64_t a,uint64_t b)->std::optional<uint32_t>{auto&t=*static_cast<BattleWeaponSourceTextures*>(p);if(!t.sourceCurrent()||b!=0x4330000000000000ULL||(a&0xffffffffffffff00ULL)!=0x4330000000000000ULL)return {};const float v=float(uint8_t(a));uint32_t bits;std::memcpy(&bits,&v,4);return bits;};
 return p;
}
// These are original TEX read PCs, not field offsets. Capture values at the
// actual reads, including post-GXInit LOD/filter reloads; do not reread headers
// later to invent the constructor operands. Bits 0..12 cover thirteen fields.
void BattleWeaponSourceTextures::observe(uint32_t site,uint32_t v){
 switch(site){
 case 0x80037aa0:table_=v;break;
 case 0x80037aa4:image_.imageHeader=v;observed_|=1;break;
 case 0x80037ac4:image_.data=v;observed_|=2;break;
 case 0x80037acc:image_.width=uint16_t(v);observed_|=4;break;
 case 0x80037ad0:image_.height=uint16_t(v);observed_|=8;break;
 case 0x80037ad4:image_.format=v;observed_|=16;break;
 case 0x80037ad8:image_.wrapS=v;observed_|=32;break;
 case 0x80037adc:image_.wrapT=v;observed_|=64;break;
 case 0x80037af4:image_.minLOD=uint8_t(v);observed_|=128;break;
 case 0x80037afc:image_.maxLOD=uint8_t(v);observed_|=256;break;
 case 0x80037b20:image_.minFilter=v;observed_|=512;break;
 case 0x80037b28:image_.magFilter=v;observed_|=1024;break;
 case 0x80037b30:std::memcpy(&image_.bias,&v,4);observed_|=2048;break;
 case 0x80037b38:image_.edge=uint8_t(v);observed_|=4096;break;
 default:break;
 }
}
std::optional<uint32_t>BattleWeaponSourceTextures::read(uint32_t a,uint32_t n){
 if(!sourceCurrent())return {};
 std::optional<uint32_t>v;
 // Private object intersections get first refusal, including unknown cells and
 // partial reads; they cannot leak through into source-memory fallback.
 if(object_.overlaps(a,n))v=object_.read(a,n);
 else if(n==1){const auto x=memory_->byte(a);if(x)v=*x;}
 else if(n==2){const auto x=memory_->half(a);if(x)v=*x;}
 else if(n==4)v=memory_->word(a);
 if(!sourceCurrent())return {};
 if(v)observe(cursor_.pending(),*v);
 return v;
}
// Witness exact output address, canonical callback/control-block identity, all
// known words and observed metadata before downstream LoadTexture snapshots it.
bool BattleWeaponSourceTextures::witness(const BattleClassSurfaceTextureObject&v)const{
 const auto actual=shared_from_this();
 if(!sourceCurrent()||v.handle!=address_||v.user!=this||v.lifetime.get()!=this||v.lifetime.owner_before(actual)||actual.owner_before(v.lifetime)||v.current!=objectCurrent||v.witness!=objectWitness||!v.bytes||!same(image_,v.image))return false;
 const auto words=object_.words();return words&&*words==*v.bytes&&sourceCurrent();
}
bool BattleWeaponSourceTextures::objectCurrent(void*p){return static_cast<BattleWeaponSourceTextures*>(p)->current();}
bool BattleWeaponSourceTextures::objectWitness(void*p,const BattleClassSurfaceTextureObject&v){auto&t=*static_cast<BattleWeaponSourceTextures*>(p);if(t.invoking_)return false;Guard g(t.invoking_);return t.witness(v);}
std::optional<BattleClassSurfaceTextureObject>BattleWeaponSourceTextures::texture(uint32_t palette,uint32_t index){
 if(invoking_)return {};
 Guard g(invoking_);if(!sourceCurrent()||index>3)return {};
 // Genuine common bank: B+0x163f8 FileEntry -> +0xa0 holder -> palette.
 // Indices 0..3 cover class I4 and submenu CMPR; no native atlas substitutes here.
 const auto descriptor=memory_->word(source_.native.battle+0x163f8);const auto cell=descriptor?memory_->word(*descriptor+0xa0):std::nullopt;const auto actual=cell?memory_->word(*cell):std::nullopt;
 if(!actual||*actual!=palette)return {};
 // A held constructor fixes palette/index. A completed result is reused until
 // explicit consumer acknowledgment, without replaying clear/stores/math.
 if(held_&&(palette_!=palette||index_!=index))return {};
 if(!held_){held_=true;completed_=false;palette_=palette;index_=index;observed_=clearPosition_=0;image_={};image_.index=index;}
 if(!bound_){if(!object_.bind(address_,memory_,this,[](void*p){return static_cast<BattleWeaponSourceTextures*>(p)->sourceCurrent();}))return {};bound_=true;}
 if(!completed_){if(!resumeSourcePaletteTextureObject(palette,address_,index,services_,cursor_)||!sourceCurrent())return {};completed_=true;}
 // 8191 = 0x1fff: all thirteen required metadata reads were observed.
 if(observed_!=8191)return {};
 // Additional native resource admission, not an extra original TEX read:
 // these four surfaces have no TLUT. Never invent unread palette metadata.
 // Each TPL descriptor is 8 bytes: image header then CLUT pointer (+4).
 const uint64_t at=uint64_t(table_)+(uint64_t(index)<<3)+4;if(at>UINT32_MAX)return {};
 const auto paletteHeader=memory_->word(uint32_t(at));if(!paletteHeader||*paletteHeader)return {};
 const auto words=object_.words();if(!words||!sourceCurrent())return {};
 BattleClassSurfaceTextureObject v;v.handle=address_;v.image=image_;v.bytes=*words;v.lifetime=shared_from_this();v.user=this;
 v.current=objectCurrent;v.witness=objectWitness;return v;
}
// Release the completed result only after actual downstream acceptance. The
// next original TEX call may then reuse this same private object storage.
bool BattleWeaponSourceTextures::acknowledge(uint32_t palette,uint32_t index){
 if(invoking_)return false;
 Guard g(invoking_);if(!sourceCurrent()||!held_||!completed_||palette_!=palette||index_!=index)return false;
 held_=completed_=false;return true;
}
}

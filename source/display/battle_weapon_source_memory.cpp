#include "battle_weapon_source_memory.hpp"
#include <cstdlib>
namespace ttyd {namespace {
// Nonempty 32-bit guest spans, checked with wider arithmetic before routing.
bool extent(uint32_t a,uint32_t n){return n&&uint64_t(a)+n<=0x100000000ULL;}
struct Guard{bool&b;explicit Guard(bool&v):b(v){b=true;}~Guard(){b=false;}};
// FNV-1a64 offset basis/prime over exact original bytes, wrapping modulo 2^64.
// Each expected value below pins one independent immutable literal span.
bool span(const EventScriptAssets&s,uint32_t a,uint32_t n,uint64_t expected){uint64_t hash=14695981039346656037ULL;for(uint32_t i=0;i<n;++i){uint8_t b;if(!s.readByte(a+i,b))return false;hash=(hash^b)*1099511628211ULL;}return hash==expected;}
}
std::shared_ptr<BattleWeaponSourceMemory>BattleWeaponSourceMemory::create(EventNativeRuntime&vm,
 std::shared_ptr<NativeUiStartup>startup,std::shared_ptr<EventGpLanguageAlias>language,
 std::shared_ptr<const void>lifetime,std::function<bool()>current,const char*image){
 if(!startup||!language||!lifetime||!current||!current()||!startup->current()||!startup->fontObject()||!language->current())return {};
 auto result=std::shared_ptr<BattleWeaponSourceMemory>(new BattleWeaponSourceMemory);
 result->vm_=&vm;result->startup_=std::move(startup);result->language_=std::move(language);result->lifetime_=std::move(lifetime);result->current_=std::move(current);result->font_=result->startup_->fontMemory();
 // The alias must come from the actual completed startup process. Global
 // 0x80415fd0 selects destination GP; its +0x16c language lane shares that
 // source owner instead of receiving a copied private language value.
 const auto process=result->startup_->fieldProcess();
 if(!result->font_||!process||!result->language_->sourceIs(process->process().vm().gpAddresses)||!vm.memory.readWord(0x80415fd0,result->gp_)||!result->gp_||!result->constants_.load(image)||
  // Source literals: 0x802c2b30/144 bytes, 0x804203e4/96 bytes, and
  // original SDK min-filter table 0x8041e558/8 bytes, each with its own hash.
  !span(result->constants_,0x802c2b30,144,0xa7bc9710cf961686ULL)||!span(result->constants_,0x804203e4,96,0x38fa0130cc650a0eULL)||!span(result->constants_,0x8041e558,8,0x8263db999b8acaebULL)||
  (!vm.gpAddresses.languageAliasIs(result->gp_,result->language_)&&!vm.gpAddresses.bindLanguageAlias(result->gp_,result->language_))||!result->current())return {};
 return result;
}
// Actual VM, startup/font owner, GP identity and installed language alias
// remain authoritative. Observed loss is sticky; recreating values cannot revive it.
bool BattleWeaponSourceMemory::valid()const{
 if(invalid_)return false;
 if(!vm_||!font_||!startup_||!language_||!lifetime_||!current_||!current_()||!startup_->current()||startup_->fontMemory()!=font_||!language_->current()){invalid_=true;return false;}
 uint32_t gp=0,language=0;const auto actual=startup_->language();
 const bool ok=actual&&vm_->gpAddresses.languageAliasIs(gp_,language_)&&vm_->memory.readWord(0x80415fd0,gp)&&gp==gp_&&vm_->memory.readWord(gp+0x16c,language)&&language==*actual;
 if(!ok)invalid_=true;
 return ok;
}
bool BattleWeaponSourceMemory::current()const{
 if(invoking_)return false;
 Guard guard(invoking_);if(!valid()){invalid_=true;return false;}return true;
}
bool BattleWeaponSourceMemory::constant(uint32_t a,uint32_t n)const{
 if(!extent(a,n))return false;
 return (a>=0x802c2b30&&uint64_t(a)+n<=0x802c2bc0)||(a>=0x804203e4&&uint64_t(a)+n<=0x80420444)||(a>=0x8041e558&&uint64_t(a)+n<=0x8041e560);
}
// Ownership means ANY overlap, not successful readability of every byte.
// Private-output reservations must reject unknown tails and partial intersections.
bool BattleWeaponSourceMemory::owns(uint32_t a,uint32_t n)const{
 if(!extent(a,n)||!vm_||!font_)return false;
 if((startup_&&startup_->ownsSourceResource(a,n))||font_->owns(a,n)||!vm_->memory.unmappedRange(a,n))return true;
 for(const auto&span:std::array<std::pair<uint32_t,uint32_t>,3>{{{0x802c2b30,144},{0x804203e4,96},{0x8041e558,8}}})if(uint64_t(a)<uint64_t(span.first)+span.second&&uint64_t(span.first)<uint64_t(a)+n)return true;
 return false;
}
std::optional<uint32_t>BattleWeaponSourceMemory::read(uint32_t a,uint32_t n)const{
 if(!extent(a,n)||!valid())return {};
 std::optional<uint32_t>value;
 // Entire intersecting startup allocations, including unknown tails, have first
 // refusal. A partial overlap cannot obtain bytes from the destination VM.
 if(startup_->ownsSourceResource(a,n)){
  if(n==1){const auto v=startup_->sourceResourceByte(a);if(v)value=*v;}
  if(n==2){const auto v=startup_->sourceResourceHalf(a);if(v)value=*v;}
  if(n==4)value=startup_->sourceResourceWord(a);
 // Any actual VM intersection gets first refusal, even when its semantic
 // reader cannot supply the requested value; only fully unmapped spans fall back.
 }else if(!vm_->memory.unmappedRange(a,n)){
  if(n==1){uint8_t v;if(vm_->memory.readByte(a,v))value=v;}
  if(n==2){uint16_t v;if(vm_->memory.readHalf(a,v))value=v;}
  if(n==4){uint32_t v;if(vm_->memory.readWord(a,v))value=v;}
 // Immutable literals are assembled big-endian only when the WHOLE read
 // fits one admitted span. They do not create writable source RAM.
 }else if(constant(a,n)){
  uint32_t v=0;bool ok=true;for(uint32_t i=0;i<n;++i){uint8_t b;if(!constants_.readByte(a+i,b)){ok=false;break;}v=v<<8|b;}if(ok)value=v;
 }
 return valid()?value:std::nullopt;
}
std::optional<uint8_t>BattleWeaponSourceMemory::byte(uint32_t a)const{if(invoking_)return {};Guard g(invoking_);const auto v=read(a,1);return v?std::optional<uint8_t>(uint8_t(*v)):std::nullopt;}
std::optional<uint16_t>BattleWeaponSourceMemory::half(uint32_t a)const{if(invoking_)return {};Guard g(invoking_);const auto v=read(a,2);return v?std::optional<uint16_t>(uint16_t(*v)):std::nullopt;}
std::optional<uint32_t>BattleWeaponSourceMemory::word(uint32_t a)const{if(invoking_)return {};Guard g(invoking_);return read(a,4);}
// Only widths 1/2/4 are source store operations. Font bytes and actual VM
// mappings are mutable; immutable fallback literals have no write destination.
bool BattleWeaponSourceMemory::store(uint32_t a,uint32_t n,uint32_t value){
 if(invoking_||!extent(a,n)||(n!=1&&n!=2&&n!=4))return false;
 Guard g(invoking_);if(!valid())return false;
 bool ok=false;
 if(font_->owns(a,n)){
  // Preflight every actual byte before any halfword write. There are no
  // callbacks/yields between these original byte-lane stores.
  for(uint32_t i=0;i<n;++i)if(!font_->byte(a+i))return false;
  if(n==4)ok=font_->storeWord(a,value);
  else{ok=true;for(uint32_t i=0;i<n;++i)ok=font_->storeByte(a+i,uint8_t(value>>(8*(n-i-1))))&&ok;}
 }else if(!vm_->memory.unmappedRange(a,n)){
  if(n==1)ok=vm_->memory.writeByte(a,uint8_t(value));
  if(n==2)ok=vm_->memory.writeHalf(a,uint16_t(value));
  if(n==4)ok=vm_->memory.writeWord(a,value);
 }
 return valid()&&ok;
}
BattleActorServices BattleWeaponSourceMemory::actors(){
 BattleActorServices a;a.user=this;a.memory.user=this;
 // Total source bodies require their complete actual finite domain admitted
 // first. An unknown cell is a programming-domain violation, never source0.
 a.byte=[](void*p,uint32_t at){const auto v=static_cast<BattleWeaponSourceMemory*>(p)->byte(at);if(!v)std::abort();return *v;};
 a.memory.word=[](void*p,uint32_t at){const auto v=static_cast<BattleWeaponSourceMemory*>(p)->word(at);if(!v)std::abort();return *v;};
 a.memory.half=[](void*p,uint32_t at){const auto v=static_cast<BattleWeaponSourceMemory*>(p)->half(at);if(!v)std::abort();return *v;};
 a.memory.storeWord=[](void*p,uint32_t at,uint32_t v){if(!static_cast<BattleWeaponSourceMemory*>(p)->store(at,4,v))std::abort();};
 a.memory.storeHalf=[](void*p,uint32_t at,uint16_t v){if(!static_cast<BattleWeaponSourceMemory*>(p)->store(at,2,v))std::abort();};
 a.memory.storeByte=[](void*p,uint32_t at,uint8_t v){if(!static_cast<BattleWeaponSourceMemory*>(p)->store(at,1,v))std::abort();};return a;
}
}

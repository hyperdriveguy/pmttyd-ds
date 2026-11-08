#include "native_field_mario_battle_transfer.hpp"
#include <cmath>
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t Saved=0x8041e8f4,PartyPointers=0x8041ead8;
constexpr uint32_t PartyBase=0x85800000,PartyStride=0x200;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
}
NativeFieldMarioBattleTransfer::NativeFieldMarioBattleTransfer(NativeFieldMarioBattleTransferBinding b):binding_(std::move(b)){}
std::unique_ptr<NativeFieldMarioBattleTransfer>NativeFieldMarioBattleTransfer::create(NativeFieldMarioBattleTransferBinding b){
 if(!b.lifetime||!b.host.current()||!b.facade.current()||!b.current||!b.mario||!b.parties||!b.presentation||!b.movement||!b.motion||!b.position||!b.storePosition)return {};
 auto result=std::unique_ptr<NativeFieldMarioBattleTransfer>(new NativeFieldMarioBattleTransfer(std::move(b)));if(!result->current())return {};return result;
}
bool NativeFieldMarioBattleTransfer::current(){
 if(stale_||checking_)return false;
 Guard guard(checking_);
 if(!binding_.host.current()||!binding_.facade.current()){stale_=true;return false;}
 if(!binding_.current(binding_.user)){stale_=true;return false;}return true;
}
FieldPartyActor*NativeFieldMarioBattleTransfer::actor(uint32_t address,unsigned&offset){
 if(address<PartyBase)return nullptr;
 const auto delta=address-PartyBase,index=delta/PartyStride;offset=delta%PartyStride;
 if(index>=2||offset>=0x188)return nullptr;
 return binding_.parties->actors[index].get();
}
std::optional<int32_t>NativeFieldMarioBattleTransfer::partyId(){
 if(!current())return {};
 for(const auto raw:binding_.mario->partyIndices){const int32_t index=raw<128?int32_t(raw):int32_t(raw)-256;
  if(index<0)continue;
  if(index>=2)return {};
  const auto*p=binding_.parties->actors[unsigned(index)].get();if(p&&!(p->word(0)&8))return index;
 }return -1;
}
std::optional<uint32_t>NativeFieldMarioBattleTransfer::word(uint32_t a){
 if(!current())return {};
 if(a==0x8041e900)return FieldMarioOwnerAddress;
 if(a==FieldMarioOwnerAddress)return binding_.mario->flags;
 if(a>=Saved&&a<=Saved+8&&(a-Saved)%4==0)return saved_[(a-Saved)/4];
 if(a>=FieldMarioOwnerAddress+0x8c&&a<=FieldMarioOwnerAddress+0x94&&(a-FieldMarioOwnerAddress-0x8c)%4==0)return binding_.position(binding_.user,(a-FieldMarioOwnerAddress-0x8c)/4);
 if(a==PartyPointers||a==PartyPointers+4){const auto i=(a-PartyPointers)/4;return binding_.parties->actors[i]?PartyBase+i*PartyStride:0;}
 unsigned offset=0;auto*p=actor(a,offset);if(p&&offset%4==0&&offset+4<=p->bytes.size())return p->word(offset);
 return {};
}
std::optional<uint16_t>NativeFieldMarioBattleTransfer::half(uint32_t a){if(!current())return {};if(a==FieldMarioOwnerAddress+0x2e)return binding_.motion(binding_.user);return {};}
std::optional<uint8_t>NativeFieldMarioBattleTransfer::byte(uint32_t a){if(!current())return {};unsigned offset=0;auto*p=actor(a,offset);if(p)return p->bytes[offset];return {};}
bool NativeFieldMarioBattleTransfer::store(uint32_t a,uint32_t value){
 if(!current())return false;
 if(a==FieldMarioOwnerAddress){binding_.mario->flags=value;return true;}
 if(a>=Saved&&a<=Saved+8&&(a-Saved)%4==0){saved_[(a-Saved)/4]=value;return true;}
 if(a>=FieldMarioOwnerAddress+0x8c&&a<=FieldMarioOwnerAddress+0x94&&(a-FieldMarioOwnerAddress-0x8c)%4==0)return binding_.storePosition(binding_.user,(a-FieldMarioOwnerAddress-0x8c)/4,value);
 unsigned offset=0;auto*p=actor(a,offset);if(p&&offset%4==0&&offset+4<=p->bytes.size()){p->word(offset,value);return true;}return false;
}
bool NativeFieldMarioBattleTransfer::child(FieldMarioBattleTransferRequest&q){
 if(!current())return false;
 switch(q.function){
 case 0x800965fc:{
  // Existing native ordinary-domain source direction adaptation. Source's
  // special main00100000 branch is not silently approximated here.
  auto&p=*binding_.presentation;auto&m=*binding_.movement;
  if((binding_.mario->flags&0x100000)||!std::isfinite(p.display)||!std::isfinite(p.offset))return false;
  const float snapped=p.display>=90&&p.display<=270?180.f:0.f;
  const float base=(270.f-snapped)+p.offset;
  const float quantized=std::fma(1000.f,base,base>=0?.5f:-.5f);
  if(!std::isfinite(base)||!std::isfinite(quantized)||double(quantized)<-2147483648.0||double(quantized)>=2147483648.0)return false;
  const float heading=p.adjustMoveDirection();
  const float radians=heading*3.14159265358979323846f/180.f;
  m.headingX=std::sin(radians);m.headingZ=-std::cos(radians);return true;
 }
 case 0x80141050:{auto id=partyId();if(!id)return false;q.result=uint32_t(*id);return true;}
 case 0x800bf1b4:{const auto raw=q.arguments[0];if(raw&0x80000000u){q.result=0;return true;}if(raw>=2)return false;q.result=binding_.parties->actors[raw]?PartyBase+raw*PartyStride:0;return true;}
 default:return binding_.child&&binding_.child(binding_.user,q);
 }
}
FieldMarioBattleTransferServices NativeFieldMarioBattleTransfer::services(){FieldMarioBattleTransferServices s;s.lifetime=binding_.lifetime;s.user=this;
 s.current=[](void*p){return static_cast<NativeFieldMarioBattleTransfer*>(p)->current();};
 s.word=[](void*p,uint32_t a){return static_cast<NativeFieldMarioBattleTransfer*>(p)->word(a);};
 s.half=[](void*p,uint32_t a){return static_cast<NativeFieldMarioBattleTransfer*>(p)->half(a);};
 s.byte=[](void*p,uint32_t a){return static_cast<NativeFieldMarioBattleTransfer*>(p)->byte(a);};
 s.store=[](void*p,uint32_t a,uint32_t v){return static_cast<NativeFieldMarioBattleTransfer*>(p)->store(a,v);};
 s.call=[](void*p,FieldMarioBattleTransferRequest&q){return static_cast<NativeFieldMarioBattleTransfer*>(p)->child(q);};return s;
}
bool NativeFieldMarioBattleTransfer::call(BattleSequenceCall&q){
 if(invoking_||!current())return false;
 Guard guard(invoking_);
 if(q.function==MarioFieldBattlePrepare)return prepare_.resume(services());
 if(q.function==MarioFieldBattlePost)return post_.resume(services());
 switch(q.function){
 case 0x8005c2c4:q.result=uint32_t(fieldMarioControlOff(*binding_.mario));return true;
 case 0x8005c290:q.result=uint32_t(fieldMarioControlOn(*binding_.mario));return true;
 case 0x800bed44:binding_.parties->control(false);return true;
 case 0x800bed0c:binding_.parties->control(true);return true;
 case 0x80141050:case 0x800bf1b4:{FieldMarioBattleTransferRequest r;r.function=q.function;r.arguments[0]=q.args[0];if(!child(r)||!r.result)return false;q.result=*r.result;return true;}
 default:return false;
 }
}
uint32_t NativeFieldMarioBattleTransfer::pending()const{return post_.pending()?post_.pending():prepare_.pending();}
}

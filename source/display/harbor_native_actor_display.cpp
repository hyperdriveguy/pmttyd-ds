#include "harbor_native_actor_display.hpp"
namespace ttyd {
namespace {
constexpr uint32_t UnitAttributes=0x104,UnitParts=0x14,PartNext=0,
 PartAttributes=0x1ac,BodyPart=1,SuppressOrdinaryRegistration=0x01000000;
}
std::optional<HarborNativeBodyDisplayAttributes>harborNativeBodyDisplayAttributes(uint32_t unit,const HarborNativeBodyDisplayServices&s){
 if(!unit||!s.lifetime||!s.current||!s.word||!s.current(s.user))return {};
 auto read=[&](uint32_t address)->std::optional<uint32_t>{
  if(!s.current(s.user))return {};const auto value=s.word(s.user,address);
  if(!s.current(s.user))return {};return value;
 };
 const auto attributes=read(unit+UnitAttributes),head=read(unit+UnitParts);
 if(!attributes||!head)return {};
 uint32_t part=*head;
 for(unsigned index=0;part&&index<HarborNativeMaximumBodyParts;++index){
  const auto flags=read(part+PartAttributes);if(!flags)return {};
  // Same body identity test as actual BtlUnit_GetBodyPartsId80128c98:
  // part attributes mask1 chooses the primary body, not a guessed part index.
  if(*flags&BodyPart)return HarborNativeBodyDisplayAttributes{*attributes,*flags};
  const auto next=read(part+PartNext);if(!next)return {};part=*next;
 }
 return {};
}
bool harborNativeBodyDrawAllowed(const HarborNativeBodyDisplayAttributes&a){
 return !(a.unit&SuppressOrdinaryRegistration)&&!(a.part&SuppressOrdinaryRegistration);
}
}

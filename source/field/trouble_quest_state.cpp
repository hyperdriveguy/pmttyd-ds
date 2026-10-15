#include "trouble_quest_state.hpp"
#include "field_actors.hpp"
#include "trouble_quest_services.hpp"
namespace ttyd {
// Reject unsupported/duplicate flag bits and unknown nonzero bytes before
// importing a save snapshot. This validates representation, not complete knowledge.
bool TroubleQuestExtraState::valid() const {
 if((known&~Mask) || (values&~known) || (byteKnown&~0x3fu))return false;
 for(unsigned i=0;i<bytes.size();++i)if(!(byteKnown&(1u<<i)) && bytes[i])return false;
 return true;
}
// Canonical5376/5405/5406/5407 bypass the extra bank. Out-of-domain or
// missing known bit returns nullopt; false is an actual observed clear flag.
std::optional<bool> troubleQuestFlag(const FieldActorTalkState& s,int32_t flag) {
 switch(flag){case 5376:return s.flag5376;case 5405:return s.flag5405;
  case 5406:return s.flag5406;case 5407:return s.flag5407;}
 if(flag<5374 || flag>5421)return {};
 const uint64_t bit=uint64_t(1)<<(flag-5374);
 if(!(s.trouble.known&bit))return {};
 return (s.trouble.values&bit)!=0;
}
bool setTroubleQuestFlag(FieldActorTalkState& s,int32_t flag,bool value) {
 switch(flag){case 5376:s.flag5376=value;return true;case 5405:s.flag5405=value;return true;
  case 5406:s.flag5406=value;return true;case 5407:s.flag5407=value;return true;}
 if(flag<5374 || flag>5421)return false;
 const uint64_t bit=uint64_t(1)<<(flag-5374);s.trouble.known|=bit;
 if(value)s.trouble.values|=bit;else s.trouble.values&=~bit;
 return true;
}
// Quest1420 uses its existing optional; 1469..1474 use the six-byte bank.
// Setter narrows value toU8 (source wrapping), not saturation.
std::optional<uint8_t> troubleQuestByte(const FieldActorTalkState& s,int32_t byte) {
 if(byte==1420)return s.quest1420;
 if(byte<1469 || byte>1474 || !(s.trouble.byteKnown&(1u<<(byte-1469))))return {};
 return s.trouble.bytes[byte-1469];
}
bool setTroubleQuestByte(FieldActorTalkState& s,int32_t byte,int32_t value) {
 if(byte==1420){s.quest1420=uint8_t(value);return true;}
 if(byte<1469 || byte>1474)return false;
 const auto i=unsigned(byte-1469);s.trouble.byteKnown|=uint8_t(1u<<i);s.trouble.bytes[i]=uint8_t(value);return true;
}
// Fresh source swInit only: establish all supported bits/bytes as known zero.
// Never call merely because an older save omitted this extension.
void initializeTroubleQuestState(FieldActorTalkState& s) {
 s.trouble={};s.trouble.known=TroubleQuestExtraState::Mask;s.trouble.byteKnown=0x3f;
 s.quest1420=0;s.flag5376=false;s.flag5405=false;s.flag5406=false;s.flag5407=false;
}
TroubleQuestServices troubleQuestSwitchServices(FieldActorTalkState& s) {
 TroubleQuestServices services;services.user=&s;
 services.clearFlag=[](void* p,int32_t flag){setTroubleQuestFlag(*static_cast<FieldActorTalkState*>(p),flag,false);};
 services.setByte=[](void* p,int32_t byte,uint8_t value){setTroubleQuestByte(*static_cast<FieldActorTalkState*>(p),byte,value);};
 return services;
}
}

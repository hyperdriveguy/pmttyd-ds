#pragma once
#include "event_manager.hpp"
#include "battle_status.hpp"
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct SourceBattleVoiceServices {
  void* user=nullptr;
  std::shared_ptr<const void> lifetime;
  bool(*current)(void*)=nullptr;
  std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
  std::optional<int32_t>(*value)(void*,EventEntry&,int32_t)=nullptr;
  std::optional<int32_t>(*translate)(void*,EventEntry&,int32_t)=nullptr;
  std::optional<uint32_t>(*unit)(void*,uint32_t,int32_t)=nullptr;
  std::optional<int32_t>(*random)(void*,int32_t)=nullptr;
  std::optional<std::array<float,3>>(*position)(void*,uint32_t)=nullptr;
  std::optional<int32_t>(*checkStatus)(void*,uint32_t,int32_t)=nullptr;
  std::optional<StatusValue>(*getStatus)(void*,uint32_t,int32_t)=nullptr;
  std::optional<int32_t>(*on)(void*,uint32_t,const std::array<float,3>&)=nullptr;
  bool(*pitch)(void*,int32_t,int32_t)=nullptr;
  std::optional<uint8_t>(*volume)(void*,int32_t)=nullptr;
  bool(*setVolume)(void*,int32_t,uint32_t)=nullptr;
};
struct SourceBattleVoiceState;
class SourceBattleVoiceCursor {
public:
  bool active()const{return bool(state_);}
  uint32_t pending()const;
private:
  std::shared_ptr<SourceBattleVoiceState> state_;
  friend bool resumeSourceBattleVoice(EventEntry&,const SourceBattleVoiceServices&,SourceBattleVoiceCursor&,int32_t&);
  friend bool resumeSourceBattleUnitSound(uint32_t,uint32_t,uint32_t,int32_t,const SourceBattleVoiceServices&,SourceBattleVoiceCursor&,int32_t&);
};
// Complete original80104508/d8 +8012619c/5c +80126088/114 local chain.
// On must admit a real original pending slot or original failure handle; a trace
// vector/sample submission is not source completion. result is the EVT return2.
bool resumeSourceBattleVoice(EventEntry&,const SourceBattleVoiceServices&,SourceBattleVoiceCursor&,int32_t& result);
// Whole unit-position wrapper and positional child, for arbitrary source volume
// word/belong. A separate cursor is required for each original invocation.
bool resumeSourceBattleUnitSound(uint32_t unit,uint32_t key,uint32_t rawVolume,int32_t belong,const SourceBattleVoiceServices&,SourceBattleVoiceCursor&,int32_t& handle);
}

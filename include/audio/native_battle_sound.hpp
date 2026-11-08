#pragma once
#include "source_battle_voice.hpp"
#include "positional_sfx_registry.hpp"
#include <memory>
namespace ttyd {
struct EventNativeRuntime;class BattleRandom;
struct NativeBattleSoundBinding {
  EventNativeRuntime* vm=nullptr;BattleRandom* random=nullptr;
  std::shared_ptr<PositionalSfxRegistry> registry;
  std::shared_ptr<const void> lifetime;void* user=nullptr;
  bool(*current)(void*)=nullptr;
};
// Finite serialized native binding. It owns real public PENDING slots, not NDSP
// voices. Started-engine On/steal domains need their actual backend and hold.
class NativeBattleSound {
public:
 explicit NativeBattleSound(const NativeBattleSoundBinding&);
 ~NativeBattleSound();
 NativeBattleSound(const NativeBattleSound&)=delete;
 NativeBattleSound& operator=(const NativeBattleSound&)=delete;
 std::optional<int32_t> voice(EventEntry&);
 std::optional<int32_t> menu(uint32_t key);
 // Same atomic real source On owner with an actual captured source position.
 std::optional<int32_t> position(uint32_t key,const std::array<float,3>&);
 uint32_t pending()const;
 bool usesRegistry(const PositionalSfxRegistry*)const;
 bool usesRuntime(const EventNativeRuntime*)const;
private:
 struct State;std::unique_ptr<State> state_;
};
}

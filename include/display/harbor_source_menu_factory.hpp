#pragma once
#include "battle_menu_file_cache.hpp"
#include "battle_vm_message_lookup.hpp"
#include "battle_weapon_source_memory.hpp"
#include "harbor_source_display_pass.hpp"
#include "trio_class_menu_frame_binding.hpp"
#include "trio_weapon_menu_frame_binding.hpp"
namespace ttyd {
// These existing binding record types contain generic original source/frame
// services; using them does not create or impersonate a Trio runtime.
struct HarborSourceMenuFactoryBinding {
  EventNativeRuntime *vm = nullptr;
  uint32_t battle = 0;
  std::weak_ptr<const void> runtime;
  std::weak_ptr<HarborSourceDisplayPass> display;
  std::shared_ptr<BattleMenuFileCache> files;
  std::shared_ptr<const MessageAssets> messages;
  // Actual B/allocation/process proof, with no public facade pointer capture.
  std::function<bool()> current;
  std::function<std::shared_ptr<const uint8_t>(uint32_t)> uiLifetime;
  std::function<uint32_t(uint32_t)> random;
  std::vector<std::pair<std::string, uint32_t>> existingMessages;
};
class HarborSourceMenuFactory {
public:
  static std::shared_ptr<HarborSourceMenuFactory>
      create(HarborSourceMenuFactoryBinding);
  bool current() const;
  // Resume only an already sealed actual source display pass; never register,
  // activate or generate a camera receipt. Runtime is pinned for the whole call.
  bool resumeDisplay() const;
  std::optional<TrioClassMenuFrameBinding>
  classBinding(const BattleActDrawServices &) const;
  std::optional<TrioWeaponMenuFrameBinding>
  weaponBinding(const BattleWeaponDrawServices &) const;
  std::shared_ptr<BattleWeaponSourceMemory>
  weaponMemory(std::shared_ptr<NativeUiStartup>, const char *image);
  bool resumeClass(const HarborSourceDisplayInvocation &,
                   BattleClassNativeFrame &,
                   BattleClassNativeSubmission = {}) const;
  bool resumeWeapon(const HarborSourceDisplayInvocation &,
                    BattleWeaponNativeFrame &) const;

private:
  HarborSourceMenuFactory() = default;
  struct State;
  std::shared_ptr<State> state_;
};
} // namespace ttyd

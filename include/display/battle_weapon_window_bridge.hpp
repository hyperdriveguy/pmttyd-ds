#pragma once
#include "battle_weapon_help.hpp"
#include "battle_window_packets.hpp"
#include "native_ui_startup.hpp"
#include "source_gx_setter_routing.hpp"
#include <functional>
namespace ttyd {
struct BattleWeaponWindowBinding {
  std::shared_ptr<NativeUiStartup> startup;
  std::shared_ptr<const void> resources;
  BattleSubmenuCameraProof camera;
  uint32_t cameraAddress =
      0; // Actual admitted camera8 address, never a fallback.
  std::shared_ptr<SourceGxSetterRouting> gx;
  // The short predicates/readers are actual executing camera8/source owners.
  // word must preserve owned unknown ranges; it supplies no fallback literals.
  std::function<bool()> sourceCurrent, longCurrent;
  std::function<std::optional<uint32_t>()> currentCamera;
  std::function<std::optional<uint32_t>(uint32_t)> word;
  std::function<bool(BattleAudienceMatrixRequest &)> matrix;
  std::function<bool(BattleWindowPacketPtr)> append;
};
// Actual source Help window child, whole original primitive and exact packet
// provider. Preparation only: the queued parent must seal/preflight/submit its
// complete ordered publication before acknowledging the display callback.
class BattleWeaponWindowBridge {
public:
  BattleWeaponWindowBridge(const BattleWindowSurfaceResource &,
                           BattleWeaponWindowBinding);
  ~BattleWeaponWindowBridge();
  BattleWeaponWindowBridge(const BattleWeaponWindowBridge &) = delete;
  BattleWeaponWindowBridge &
  operator=(const BattleWeaponWindowBridge &) = delete;
  std::optional<uint32_t> request(const BattleWeaponHelpCall &);
  uint32_t pending() const;

private:
  struct State;
  std::unique_ptr<State> state_;
};
} // namespace ttyd

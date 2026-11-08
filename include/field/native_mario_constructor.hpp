#pragma once
#include "native_ui_startup.hpp"
#include "prologue_pouch.hpp"
#include "source_mario_constructor.hpp"
namespace ttyd {
// Fresh source constructor over the actual animation/UI/SDK/file process.
// Cold admission refuses existing Mario/pouch/party/history BSS; never rerun
// Init against a loaded pouch or replace the typed subset with guessed RAM.
class NativeMarioConstructor {
public:
  static std::shared_ptr<NativeMarioConstructor>
  create(std::shared_ptr<NativeUiStartup>, const char *root);
  ~NativeMarioConstructor();
  bool
  initialize(unsigned visits = 65536); // Native cold-work budget, not ticks.
  // Whole raw marioPoseInit over this same constructor-owned source record.
  // Each source Entry runs once; typed role views may then adopt its handles.
  bool initializePoses(std::array<std::shared_ptr<const BattlePoseResource>, 3>);
  bool posesReady() const;
  std::array<int32_t, 3> poseHandles() const;
  bool current() const;
  bool ready() const;
  bool busy() const;
  uint32_t pending() const;
  uint32_t marioAddress() const;
  uint32_t pouchAddress() const;
  std::shared_ptr<NativeUiStartup> ui() const;
  // Explicit native checkpoint handoff AFTER genuine fresh source Init. Copy
  // packed state into the same live pouch, preserving its allocation/identity.
  // This is not source card-load/field-return acknowledgment or
  // reinitialization.
  bool adoptPouchSnapshot(const ProloguePouch &);
  bool capturePouchSnapshot(ProloguePouch &) const;

private:
  NativeMarioConstructor();
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
};
} // namespace ttyd

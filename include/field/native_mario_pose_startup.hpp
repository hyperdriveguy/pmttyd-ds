#pragma once
#include "native_field_animation.hpp"
#include "source_mario_pose_startup.hpp"
namespace ttyd {
// Borrows an already initialized, backed raw Mario owner in this animation VM.
// This does not run marioInit, install a guessed Mario pointer, or reinterpret
// the typed FieldMarioBattleState/token as a complete source2f8-byte record.
class NativeMarioPoseStartup {
public:
  static std::unique_ptr<NativeMarioPoseStartup>
      bind(std::shared_ptr<NativeFieldAnimation>,
           std::array<std::shared_ptr<const BattlePoseResource>, 3>);
  bool resume();
  bool current() const;
  bool ready() const { return current() && cursor_.complete; }
  bool pending() const { return cursor_.active; }
  uint32_t pendingAddress() const { return cursor_.pending; }
  uint32_t marioAddress() const { return mario_; }

private:
  NativeMarioPoseStartup() = default;
  bool pin(uint32_t, uint32_t);
  BattleSequenceEntryServices services();
  std::shared_ptr<NativeFieldAnimation> animation_;
  std::array<std::shared_ptr<const BattlePoseResource>, 3> resources_;
  std::unique_ptr<EventAddressSpace> identity_;
  std::vector<std::pair<uint32_t, uint32_t>> spans_;
  SourceMarioPoseStartupCursor cursor_;
  uint32_t mario_ = 0;
  bool invoking_ = false;
};
} // namespace ttyd

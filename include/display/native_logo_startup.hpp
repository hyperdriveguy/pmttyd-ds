#pragma once
#include "native_ui_startup.hpp"
#include "source_logo_startup.hpp"
namespace ttyd {
// Retained real platform/texture/Mario child provider. It must acknowledge each
// whole requested child; no default reset/video/pose success is supplied.
// Each child retains accepted effects when post-call currentness defers
// admission.
struct NativeLogoChildren {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  bool (*call)(void *, BattleSequenceCall &) = nullptr;
};
class NativeLogoStartup {
public:
  static std::unique_ptr<NativeLogoStartup>
  bind(std::shared_ptr<NativeUiStartup>, const char *root);
  bool configureChildren(
      NativeLogoChildren); // One provider, retained across holds.
  bool initialize();
  bool exit();
  bool current() const;
  bool initialized() const { return current() && init_.complete; }
  bool exited() const { return current() && exit_.complete; }
  bool pending() const { return init_.active || exit_.active; }
  uint32_t pendingAddress() const {
    return exit_.active ? exit_.pending : init_.pending;
  }
  uint32_t fileHandle() const { return handle_; }

private:
  NativeLogoStartup() = default;
  BattleSequenceEntryServices services();
  bool child(BattleSequenceCall &);
  std::shared_ptr<NativeUiStartup> ui_;
  std::shared_ptr<NativeFieldAnimation> animation_;
  std::unique_ptr<EventAddressSpace> identity_;
  NativeLogoChildren children_;
  SourceLogoStartupCursor init_, exit_;
  uint32_t handle_ = 0;
  bool invoking_ = false, formatted_ = false;
  std::string filename_;
};
} // namespace ttyd

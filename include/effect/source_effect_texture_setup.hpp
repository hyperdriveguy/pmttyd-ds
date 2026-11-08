#pragma once
#include "source_window_texture_setup.hpp"
namespace ttyd {
// Shared source texture primitives/path ABI; this does not borrow window state.
using SourceEffectTextureSetupServices = SourceWindowTextureSetupServices;
// Whole effTexSetup and its mandatory effTexSetupN64 child. Acknowledgment
// means source job admission, not completion of the independent DVD callbacks.
class SourceEffectTextureSetupCursor {
public:
  SourceEffectTextureSetupCursor();
  ~SourceEffectTextureSetupCursor();
  SourceEffectTextureSetupCursor(const SourceEffectTextureSetupCursor &) =
      delete;
  SourceEffectTextureSetupCursor &
  operator=(const SourceEffectTextureSetupCursor &) = delete;
  bool active() const;
  bool stale() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool
  resumeSourceEffectTextureEntry(uint32_t, uint32_t,
                                 const SourceEffectTextureSetupServices &,
                                 SourceEffectTextureSetupCursor &);
};
bool resumeSourceEffectTextureSetup(const SourceEffectTextureSetupServices &,
                                    SourceEffectTextureSetupCursor &);
bool resumeSourceEffectTextureCompletion(
    uint32_t secondArgument, const SourceEffectTextureSetupServices &,
    SourceEffectTextureSetupCursor &);
bool resumeSourceEffectTextureN64Completion(
    uint32_t secondArgument, const SourceEffectTextureSetupServices &,
    SourceEffectTextureSetupCursor &);
} // namespace ttyd

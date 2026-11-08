#pragma once
#include "source_window_texture_setup.hpp"
namespace ttyd {
using SourceFadeStartupServices = SourceWindowTextureSetupServices;
// Whole original fadeInit, fadeTexSetup and independent DVD completion.
// Source initialization's scalar constants must be original finite binary32.
class SourceFadeStartupCursor {
public:
  SourceFadeStartupCursor();
  ~SourceFadeStartupCursor();
  SourceFadeStartupCursor(const SourceFadeStartupCursor &) = delete;
  SourceFadeStartupCursor &operator=(const SourceFadeStartupCursor &) = delete;
  bool active() const;
  bool stale() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool resumeSourceFadeEntry(uint32_t, uint32_t,
                                    const SourceFadeStartupServices &,
                                    SourceFadeStartupCursor &);
};
bool resumeSourceFadeInit(const SourceFadeStartupServices &,
                          SourceFadeStartupCursor &);
bool resumeSourceFadeTextures(const SourceFadeStartupServices &,
                              SourceFadeStartupCursor &);
bool resumeSourceFadeCompletion(uint32_t secondArgument,
                                const SourceFadeStartupServices &,
                                SourceFadeStartupCursor &);
} // namespace ttyd

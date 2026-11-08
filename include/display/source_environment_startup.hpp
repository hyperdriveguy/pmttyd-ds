#pragma once
#include "source_window_texture_setup.hpp"
namespace ttyd {
using SourceEnvironmentStartupServices = SourceWindowTextureSetupServices;
// Whole distinct envInit, envTexSetup and independent DVD callback. Setup
// returns on job admission; it preserves the original prior readiness word.
class SourceEnvironmentStartupCursor {
public:
  SourceEnvironmentStartupCursor();
  ~SourceEnvironmentStartupCursor();
  SourceEnvironmentStartupCursor(const SourceEnvironmentStartupCursor &) =
      delete;
  SourceEnvironmentStartupCursor &
  operator=(const SourceEnvironmentStartupCursor &) = delete;
  bool active() const;
  bool stale() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool
  resumeSourceEnvironmentEntry(uint32_t, uint32_t,
                               const SourceEnvironmentStartupServices &,
                               SourceEnvironmentStartupCursor &);
};
bool resumeSourceEnvironmentInit(const SourceEnvironmentStartupServices &,
                                 SourceEnvironmentStartupCursor &);
bool resumeSourceEnvironmentTextures(const SourceEnvironmentStartupServices &,
                                     SourceEnvironmentStartupCursor &);
bool resumeSourceEnvironmentCompletion(uint32_t secondArgument,
                                       const SourceEnvironmentStartupServices &,
                                       SourceEnvironmentStartupCursor &);
} // namespace ttyd

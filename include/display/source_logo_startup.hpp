#pragma once
#include "battle_sequence_entry.hpp"
namespace ttyd {
// Whole original seq_logoInit/Exit. Platform OS/VI/controller calls remain
// real children: unavailable providers hold after retained earlier effects.
struct SourceLogoStartupCursor {
  bool active = false, complete = false, invoking = false, queued = false;
  uint32_t entry = 0, phase = 0, pending = 0, work = 0, root = 0, handle = 0,
           value = 0, gp = 0;
  uint32_t mode = 0, workValue = 0, workReadSite = 0, workStoreSite = 0;
  bool terminalVideo = false, extraRetrace = false;
  BattleSequenceCall call;
};
bool initializeSourceLogo(const BattleSequenceEntryServices &,
                          SourceLogoStartupCursor &);
bool exitSourceLogo(const BattleSequenceEntryServices &,
                    SourceLogoStartupCursor &);
} // namespace ttyd

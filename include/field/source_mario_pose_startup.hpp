#pragma once
#include "battle_sequence_entry.hpp"
namespace ttyd {
// One whole original marioPoseInit8005ba50/98 invocation. Keep memory/callee
// owners alive across holds; accepted reads, Entries and stores never replay.
struct SourceMarioPoseStartupCursor {
  bool active = false, complete = false, invoking = false, queued = false;
  uint32_t phase = 0, pending = 0, mario = 0, selection = 0, index = 0,
           table = 0, name = 0, result = 0;
  BattleSequenceCall call;
};
bool initializeSourceMarioPoses(const BattleSequenceEntryServices &,
                                SourceMarioPoseStartupCursor &);
} // namespace ttyd

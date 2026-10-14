#pragma once
#include "game.hpp"
#include <array>
#include <string>
namespace ttyd {
// Native v8/v9 header + packed original pouch + checksum; not a card record.
using SaveBytes=std::array<uint8_t,0x600>;
// Invalid input encodes an all-zero buffer, which decodeSave rejects. The
// byte array is always largest-version sized; file writes choose the version size.
SaveBytes encodeSave(const Progress& progress);
// Immutable queued-write/import snapshot; live pouch ownership stays separate.
Progress snapshotSaveProgress(const Progress& progress);
// Structural checks only; world collision/arrival validation belongs to runtime.
// Failure does not replace progress. Legacy saves retain unknown optional switches.
bool decodeSave(const SaveBytes& bytes, Progress& progress);
enum class LoadResult { Missing, Loaded, Recovered, Invalid };
LoadResult loadSave(const std::string& path, Progress& progress);
bool writeSave(const std::string& path, const Progress& progress);
}

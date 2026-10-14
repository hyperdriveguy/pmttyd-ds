#pragma once
#include "game.hpp"
#include <array>
#include <string>
namespace ttyd {
// Native v8 header + packed original pouch + checksum; not a card record.
using SaveBytes=std::array<uint8_t,0x600>;
SaveBytes encodeSave(const Progress& progress);
bool decodeSave(const SaveBytes& bytes, Progress& progress);
enum class LoadResult { Missing, Loaded, Recovered, Invalid };
LoadResult loadSave(const std::string& path, Progress& progress);
bool writeSave(const std::string& path, const Progress& progress);
}

#pragma once
#include "game.hpp"
#include <array>
#include <string>
namespace ttyd {
using SaveBytes=std::array<uint8_t,32>;
SaveBytes encodeSave(const Progress& progress);
bool decodeSave(const SaveBytes& bytes, Progress& progress);
enum class LoadResult { Missing, Loaded, Recovered, Invalid };
LoadResult loadSave(const std::string& path, Progress& progress);
bool writeSave(const std::string& path, const Progress& progress);
}

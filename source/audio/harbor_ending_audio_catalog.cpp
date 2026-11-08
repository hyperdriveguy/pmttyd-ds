#include "harbor_ending_audio_catalog.hpp"
#include <array>
namespace ttyd {
namespace {
struct Record {
  uint32_t row, descriptor;
  const char *name;
  const char *file;
};
constexpr std::array<Record, HarborEndingPcmCount> Records{
    {{225, 0x200002c1, "SFX_VOICE_MARIO_RELIEF1", "mario-voice-relief1.wav"},
     {226, 0x200002c2, "SFX_VOICE_MARIO_RELIEF2", "mario-voice-relief2.wav"},
     {2616, 0x20000367, "SFX_STAR_POINT_GET1", "star-point-get1.wav"}}};
const Record *record(uint32_t row) {
  for (const auto &r : Records)
    if (r.row == row)
      return &r;
  return nullptr;
}
} // namespace
const char *harborEndingPcmFile(uint32_t row) {
  const auto *r = record(row);
  return r ? r->file : nullptr;
}
const char *harborEndingPcmName(uint32_t row) {
  const auto *r = record(row);
  return r ? r->name : nullptr;
}
std::optional<uint32_t> harborEndingPcmDescriptor(uint32_t row) {
  const auto *r = record(row);
  return r ? std::optional<uint32_t>(r->descriptor) : std::nullopt;
}
std::optional<unsigned> harborEndingPcmIndex(uint32_t row) {
  for (unsigned i = 0; i < Records.size(); ++i)
    if (Records[i].row == row)
      return i;
  return {};
}
} // namespace ttyd

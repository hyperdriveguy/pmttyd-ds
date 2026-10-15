#pragma once
#include "field_npc_inline_norm.hpp"
#include <array>
namespace ttyd {
struct FieldNpcDetectionRequest {
  uint32_t function = 0;
  uint32_t argument = 0; // event ID for GetPtrID, actor for Interrupt
  std::array<uint32_t, 3>
      point{}; // semantic source stack XYZ bits, never guest pointers
  float width = 0;
};
struct FieldNpcDetectionResponse {
  uint32_t word = 0;
  float power = 0;
};
struct FieldNpcDetectionServices {
  FieldNpcNormServices norm;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  bool (*request)(void *, const FieldNpcDetectionRequest &,
                  FieldNpcDetectionResponse &) = nullptr; // norm.memory.user
};
enum class FieldNpcDetectionFrontier { Pending, Mario, Display };
// Source8004a160..8004a4e4 and reached common interrupt8004acc4..accc.
// Mario and Display are unfinished frontiers, NEVER full Main completion.
struct FieldNpcDetectionCursor {
  bool active = false;
  FieldNpcDetectionServices owner;
  uint32_t actor = 0, phase = 0, event = 0, script = 0, value = 0, kind = 0,
           sample = 0, trigger = 0, xExtent = 0, zExtent = 0;
  uint8_t eventClass = 0, flags = 0;
  float x = 0, z = 0, current = 0, height = 0, half = 0, zero = 0, squared = 0,
        distance = 0, extent = 0;
  FieldNpcNormCursor normCursor;
  FieldNpcDetectionRequest call;
  FieldNpcDetectionResponse response;
};
FieldNpcDetectionFrontier
advanceFieldNpcMainDetection(uint32_t actor, const FieldNpcDetectionServices &,
                             FieldNpcDetectionCursor &);
} // namespace ttyd

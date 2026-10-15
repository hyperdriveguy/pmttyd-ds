#pragma once
#include "field_npc_main_display.hpp"
namespace ttyd {
struct FieldNpcTailRequest {
  uint32_t function = 0;
  std::array<uint32_t,3> args{};
  std::array<float,2> scalars{};
  // Copied source score XYZ words, not Mario aliases or a guest stack address.
  std::array<uint32_t,3> point{};
};
struct FieldNpcTailResponse { uint32_t word = 0; float scalar = 0; };
struct FieldNpcTailServices {
  FieldNpcClockServices memory;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  bool (*storeByte)(void *, uint32_t, uint8_t) = nullptr;
  bool (*request)(void *, const FieldNpcTailRequest &, FieldNpcTailResponse &) = nullptr;
  bool (*queue)(void *, const FieldNpcDisplayRequest &, FieldNpcDisplayResponse &) = nullptr;
};
// field is ORIGINAL F used for final54d clear; secondField is a later GP168
// capture used for Kpa bookkeeping. Do not merge them. Counter/score words are
// interpreted signed at source comparisons. Terminal91 stays active/effect-free.
struct FieldNpcTailCursor {
  bool active = false;
  FieldNpcTailServices owner;
  uint32_t phase = 0, gp = 0, field = 0, effect = 0, secondField = 0, mario = 0,
           value = 0, count = 0, jump = 0, fire = 0, score = 0;
  float from = 0, to = 0, zero = 0, interpolated = 0, height = 0;
  FieldNpcTailRequest call;
  FieldNpcTailResponse response;
  FieldNpcDisplayRequest draw;
  FieldNpcDisplayResponse drawResponse;
};
// Complete source8004b1d0..8004b3e4. True only after final ORIGINAL F54d byte
// clear; caller must already have completed actor traversal. Every unavailable
// primitive is no-effect. Keep VM/field/Mario/queue/pass owners through holds.
bool advanceFieldNpcMainTail(const FieldNpcTailServices &, FieldNpcTailCursor &);
} // namespace ttyd

#pragma once
#include "field_npc_clock.hpp"
#include <array>
namespace ttyd {
struct FieldNpcMainEntryRequest {
  uint32_t function = 0;
  std::array<uint32_t, 2> args{};
};
// Empty/false primitives and requests have no effects. Successful requests
// publish once; stable mappings and service owners outlive the retained cursor.
struct FieldNpcMainEntryServices {
  FieldNpcClockServices clock;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr; // clock.user
  bool (*storeHalf)(void *, uint32_t, uint16_t) = nullptr;
  bool (*request)(void *, const FieldNpcMainEntryRequest &,
                  uint32_t &) = nullptr;
  // No-effect native storage admission, not an original callee. Captured source
  // bank/count/array must not be normalized or clamped. Negative count skips.
  bool (*bank)(void *, uint32_t, uint32_t, uint32_t) = nullptr;
};
enum class FieldNpcMainFrontier { Pending, Movement, FieldTail };
// Retained source parent registers through80049e8c exclusive. Movement and tail
// are unfinished frontiers, NEVER success or permission to skip source work.
struct FieldNpcMainEntryCursor {
  bool active = false;
  FieldNpcMainEntryServices owner;
  uint32_t phase = 0, gp = 0, bank = 0, array = 0, count = 0, index = 0,
           actor = 0, flags = 0, value = 0, state = 0, pose = 0, mario = 0,
           mainCount = 0, response = 0;
  float factor = 0, height = 0;
  uint16_t timer = 0;
  FieldNpcMainEntryRequest call;
  FieldNpcClockCursor clock;
};
FieldNpcMainFrontier advanceFieldNpcMainEntry(const FieldNpcMainEntryServices &,
                                              FieldNpcMainEntryCursor &);
} // namespace ttyd

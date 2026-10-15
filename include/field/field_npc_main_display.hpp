#pragma once
#include "field_npc_clock.hpp"
#include <array>
namespace ttyd {
// Semantic locals replace source stack addresses, never guest pointers.
struct FieldNpcDisplayRequest {
  uint32_t function = 0;
  std::array<uint32_t, 4> args{};
  std::array<float, 3> point{};
  uint32_t color = 0;
  float depth = 0;
};
struct FieldNpcDisplayResponse {
  uint32_t word = 0;
  std::array<float, 3> point{};
};
struct FieldNpcDisplayServices {
  FieldNpcClockServices memory;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  // False has no effects. Camera/matrix/queue providers retain the SAME source
  // producer pass, mappings and queue generation across holds and dispatch.
  // Matrix point request args[1] is actual actor+8c; projection uses point
  // in-place. Material color is the COPIED word, not a guest stack pointer.
  bool (*request)(void *, const FieldNpcDisplayRequest &,
                  FieldNpcDisplayResponse &) = nullptr;
};
enum class FieldNpcDisplayFrontier { Pending, Shadow, SeparatePose, History };
// One prefix:projected point/depth and chosen callback/class survive holds.
// Actor/providers must match; terminal Shadow/SeparatePose/History retains
// captures for the suffix. Queue registration is separate from draw execution.
struct FieldNpcDisplayCursor {
  bool active = false;
  FieldNpcDisplayServices owner;
  uint32_t actor = 0, phase = 0, value = 0, colorIndex = 0, drawClass = 0,
           callback = 0;
  std::array<float, 3> projected{};
  float low = 0, high = 0, scale = 0, zero = 0, depth = 0;
  FieldNpcDisplayRequest call;
  FieldNpcDisplayResponse response;
};
// Source8004accc..8004aeac only. All three non-Pending results are caller-owned
// boundaries. No Main completion, actor advance, history or callback dispatch.
FieldNpcDisplayFrontier advanceFieldNpcMainDisplay(
    uint32_t actor, const FieldNpcDisplayServices &, FieldNpcDisplayCursor &);
} // namespace ttyd

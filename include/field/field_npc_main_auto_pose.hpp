#pragma once
#include "field_npc_clock.hpp"
#include <array>
namespace ttyd {
struct FieldNpcAutoPoseRequest {
  uint32_t function = 0;
  std::array<uint32_t, 3> args{};
};
// False/empty primitives have no effects; successful requests publish once.
// Numeric seed is an explicit primitive, not a replacement for the source norm.
struct FieldNpcAutoPoseServices {
  FieldNpcClockServices memory; // word/store/user only
  std::optional<uint64_t> (*doubleWord)(void *, uint32_t) = nullptr;
  bool (*estimate)(void *, double, double &) = nullptr;
  bool (*request)(void *, const FieldNpcAutoPoseRequest &) = nullptr;
};
struct FieldNpcAutoPoseCursor {
  bool active = false;
  FieldNpcAutoPoseServices owner;
  uint32_t actor = 0, phase = 0, value = 0, descriptor = 0, selectedPhase = 0,
           nameOffset = 0;
  float dx = 0, dz = 0, factor = 0, dt = 0, zero = 0, norm = 0, speed = 0,
        idle = 0, threshold = 0;
  double seed = 0, half = 0, three = 0;
  FieldNpcAutoPoseRequest call;
};
// Complete source80049fbc..8004a160 only. True reaches unfinished Detection;
// never completes Main, advances actors or publishes previous-position history.
bool advanceFieldNpcMainAutoPose(uint32_t actor, float deltaX, float deltaZ,
                                 float retainedFactor,
                                 const FieldNpcAutoPoseServices &,
                                 FieldNpcAutoPoseCursor &);
// Explicit native seed adapter, NOT Gekko frsqrte/FPSCR equivalence. Caller
// must opt in. The source three double Newton rounds still execute in the core.
bool nativeFieldNpcReciprocalSqrtEstimate(void *, double, double &);
} // namespace ttyd

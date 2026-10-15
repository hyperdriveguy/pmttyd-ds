#pragma once
#include "field_npc_main_display.hpp"
namespace ttyd {
struct FieldNpcShadowRequest {
  uint32_t function = 0;
  std::array<uint32_t, 4> args{};
  // Scalar child ABI: normalize/sin use0; shadowEntry XYZ/radius use0..3;
  // hitCheckFilter origin/direction use0..5 and initialized length uses6.
  std::array<float, 7> scalars{};
  std::array<float, 3> point{}; // copied Vivian local, never actor alias
};
struct FieldNpcShadowResponse {
  uint32_t word = 0;
  float scalar = 0;
  // Seven individually addressable hit outputs, r4..r10. Only slot3 has an
  // initialized input (request scalar6). Do not zero-fill original scratch.
  std::array<std::optional<float>, 7> hit{};
};
struct FieldNpcShadowServices {
  FieldNpcClockServices memory;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  bool (*request)(void *, const FieldNpcShadowRequest &,
                  FieldNpcShadowResponse &) = nullptr;
  bool (*queue)(void *, const FieldNpcDisplayRequest &,
                FieldNpcDisplayResponse &) = nullptr;
};
struct FieldNpcShadowCursor {
  bool active = false;
  FieldNpcShadowServices owner;
  FieldNpcDisplayFrontier start = FieldNpcDisplayFrontier::Pending;
  uint32_t actor = 0, depthBits = 0, phase = 0, flags = 0, pointer = 0,
           handle = 0, token = 0, callback = 0, drawClass = 0;
  std::array<float, 3> root{}, offset{}, scale{};
  float depth = 0, angle = 0, sign = 0, divisor = 0, width = 0,
        factor = 0, radius = 0, zero = 0, one = 0, minusOne = 0;
  FieldNpcShadowRequest call;
  FieldNpcShadowResponse response;
  FieldNpcDisplayRequest draw;
  FieldNpcDisplayResponse drawResponse;
};
// Complete source aeac..b10c, including blur and separate-pose registrations.
// True reaches late history ONLY. Pending prefix is never admitted. Same
// provider pass/mappings/queue and borrowed actor live through deferred draw;
// false/empty primitives have no effects, successful ones publish once.
bool advanceFieldNpcMainShadow(uint32_t actor, FieldNpcDisplayFrontier start,
    float retainedDepth, const FieldNpcShadowServices &, FieldNpcShadowCursor &);
} // namespace ttyd

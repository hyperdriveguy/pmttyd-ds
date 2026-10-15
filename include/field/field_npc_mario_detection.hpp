#pragma once
#include "field_npc_inline_norm.hpp"
#include <array>
namespace ttyd {
// Function/args are original identities/raw words; scalars are semantic source
// local values. Mario lookup and eligibility lookup are separate child requests.
struct FieldNpcMarioRequest {
  uint32_t function = 0;
  std::array<uint32_t, 2> args{};
  std::array<float, 4> scalars{};
};
struct FieldNpcMarioResponse {
  uint32_t word = 0;
  float scalar = 0;
};
struct FieldNpcMarioServices {
  FieldNpcNormServices norm;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  // Atomic source paired XY load, both words or neither.
  std::optional<std::array<uint32_t, 2>> (*pair)(void *, uint32_t) = nullptr;
  bool (*request)(void *, const FieldNpcMarioRequest &,
                  FieldNpcMarioResponse &) = nullptr;
};
// One actor/class visit. mario retains position-source pointer; admissionMario
// separately retains eligibility pointer. Reset normCursor only between distinct
// norm graphs; terminal912 stays active. Input/provider identity must match.
struct FieldNpcMarioCursor {
  bool active = false;
  FieldNpcMarioServices owner;
  uint32_t actor = 0, phase = 0, mario = 0, admissionMario = 0, value = 0,
           gp = 0, trigger = 0;
  uint8_t eventClass = 0;
  std::array<float, 3> player{}, root{}, previous{}, current{}, change{};
  std::array<uint32_t, 4> radii{};
  float dx = 0, dy = 0, dz = 0, zero = 0, squared = 0, distance = 0, facing = 0,
        threshold = 0, range = 0, halfWidth = 0, angle = 0, x = 0, z = 0,
        extent = 0, scale = 0;
  double doubleScale = 0;
  FieldNpcNormCursor normCursor;
  FieldNpcMarioRequest call;
  FieldNpcMarioResponse response;
};
// Complete source8004a4e4..8004accc. True reaches caller-owned Display only;
// never completes Main, advances the actor or publishes history.
// Empty/false primitives have no effects; retain owner/mappings/actor/class.
bool advanceFieldNpcMarioDetection(uint32_t actor, uint8_t capturedEventClass,
                                   const FieldNpcMarioServices &,
                                   FieldNpcMarioCursor &);
} // namespace ttyd

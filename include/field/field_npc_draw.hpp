#pragma once
#include "field_npc_matrix.hpp"
namespace ttyd {
// Source call payloads use original function identities and copied locals.
// args are source integer registers r3..; scalars are f1..; matrix is the
// retained mtx_setup result, never a guest stack pointer. atan2 returns double.
struct FieldNpcDrawRequest {
  uint32_t function = 0;
  std::array<uint32_t, 3> args{};
  std::array<float, 2> scalars{};
  BattleActMatrix matrix{};
  uint32_t color = 0;
};
struct FieldNpcDrawResponse {
  uint32_t word = 0;
  double number = 0;
};
struct FieldNpcDrawServices {
  FieldNpcMatrixServices transform;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  bool (*request)(void *, const FieldNpcDrawRequest &,
                  FieldNpcDrawResponse &) = nullptr;
};
struct FieldNpcDrawCursor {
  bool active = false, pending = false, xlu = false, rear = false;
  FieldNpcDrawServices owner;
  uint32_t actor = 0, callback = 0, phase = 0, value = 0, color = 0, handle = 0,
           record = 0, pose = 0, name = 0, camera = 0, index = 0, pointer = 0,
           pass = 0;
  float incoming = 0, angle = 0, literal = 0;
  std::array<float, 4> cameraValues{};
  FieldNpcMatrixCursor transform;
  BattleActMatrix matrix{};
  FieldNpcDrawRequest call;
  FieldNpcDrawResponse response;
};
// Complete local npcDisp8004c50c / npcDisp_xlu8004c7e0, including darkening,
// facing-name swap and source Main/draw order. Other callbacks hold untouched.
// False/empty primitives have no effects. Retain VM/pose/camera/current queue
// record, generation and bindings across holds; acknowledged work never
// repeats. Completion resets the cursor; this does not activate or schedule
// Game rendering.
bool advanceFieldNpcDraw(uint32_t callback, uint32_t actor,
                         const FieldNpcDrawServices &, FieldNpcDrawCursor &);
} // namespace ttyd

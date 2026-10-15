#pragma once
#include "field_npc_blur_trig.hpp"
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
  // Offscreen bbox arguments r4..r7: left/top/right/bottom, copied u16.
  std::array<uint16_t, 4> bounds{};
};
struct FieldNpcDrawResponse {
  uint32_t word = 0;
  double number = 0;
  std::array<uint16_t, 4> bounds{};
};
struct FieldNpcDrawServices {
  FieldNpcMatrixServices transform;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  bool (*request)(void *, const FieldNpcDrawRequest &,
                  FieldNpcDrawResponse &) = nullptr;
};
struct FieldNpcDrawCursor {
  bool active = false, pending = false, xlu = false, rear = false,
       offscreen = false;
  FieldNpcDrawServices owner;
  uint32_t actor = 0, callback = 0, phase = 0, value = 0, color = 0, handle = 0,
           record = 0, pose = 0, name = 0, camera = 0, index = 0, pointer = 0,
           pass = 0, cameraId = 0, savedFlags = 0, history = 0,
           alphaCounter = 0;
  float incoming = 0, angle = 0, literal = 0, offsetX = 0, offsetZ = 0,
        trigValue = 0;
  std::array<uint16_t, 4> bounds{};
  FieldNpcBlurTrigCursor trig;
  std::array<float, 4> cameraValues{};
  FieldNpcMatrixCursor transform;
  BattleActMatrix matrix{};
  FieldNpcDrawRequest call;
  FieldNpcDrawResponse response;
};
// Complete local ordinary/xlu, offscreen ordinary/xlu and blur callbacks.
// Camera argument is retained r3 for blur; other variants ignore it.
// GPU sync/bounds, camera and Main/draw providers are required, never no-ops.
// False/empty primitives have no effects. Retain VM/pose/camera/current queue
// record, generation and bindings across holds; acknowledged work never
// repeats. Completion resets the cursor; this does not activate or schedule
// Game rendering.
bool advanceFieldNpcDraw(uint32_t callback, uint32_t actor,
                         const FieldNpcDrawServices &, FieldNpcDrawCursor &,
                         uint32_t camera = 0);
} // namespace ttyd

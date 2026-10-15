#pragma once
#include "event_memory_opcodes.hpp"
#include "source_icon_gx.hpp"
namespace ttyd {
struct RecoveryNumberRequest {
  BattleActMatrix matrix{};
  int32_t style = 0, amount = 0;
  uint32_t alpha = 255, mode = 1;
};
struct RecoveryDrawServices {
  EventMemoryServices memory;
  void *user = nullptr;
  uint8_t (*byte)(void *, uint32_t) = nullptr;
  float (*multiply)(void *, float, float) = nullptr;
  bool (*matrix)(void *, BattleAudienceMatrixRequest &) = nullptr;
  // Original camera/format/strlen/GX identities; returns an opaque token for
  // texture or stack-string construction, scalar for queries, any value for
  // void.
  std::optional<uint32_t> (*call)(void *, const SourceIconGXCall &) = nullptr;
  bool (*quad)(void *, const SourceIconGXQuad &) = nullptr;
  bool (*number)(void *, const RecoveryNumberRequest &) = nullptr;
};
// operation0 idle,1 Disp,2 number Disp2;phase/step pin reached SDK/GX
// requests. Full used provider table/camera/effect identity must match retries.
// Success resets. Matrix/geometry values replace guest stack temporaries only.
struct RecoveryDrawCursor {
  uint32_t operation = 0, phase = 0, step = 0, cameraId = 0, effect = 0,
           payload = 0, camera = 0, firstCamera = 0, particle = 0, index = 0,
           texture = 0, result = 0, string = 0, length = 0;
  int32_t kind = 0, amount = 0;
  bool callReady = false, mathReady = false, geometryReady = false;
  RecoveryDrawServices owner;
  std::array<BattleActMatrix, 4> matrices{};
  BattleAudienceMatrixRequest math;
  SourceIconGXCall request;
  SourceIconGXQuad geometry;
  RecoveryNumberRequest number;
};
// Whole original effRecoveryDisp801422f4/684 and Disp2 801420fc/1f8.
// SDK math, texture/GX submission, and effNumberGX remain explicit owners.
// Direct FIFO quads use atomic acknowledgement; false consumes no geometry.
// Queued callback execution still needs actual texture/number/GX lifetime.
// Preparation or immutable resource loading cannot acknowledge those owners.
bool resumeRecoveryEffectDraw(int32_t camera, uint32_t effect,
                              const RecoveryDrawServices &,
                              RecoveryDrawCursor &);
bool resumeRecoveryEffectNumberDraw(int32_t camera, uint32_t effect,
                                    const RecoveryDrawServices &,
                                    RecoveryDrawCursor &);
} // namespace ttyd

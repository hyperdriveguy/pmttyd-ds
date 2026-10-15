#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
#include <optional>
namespace ttyd {
enum class RecoveryBinary { Add, Subtract, Multiply, Divide };
// Callbacks acknowledge execution only after ownership/side effects are real.
// Numeric binary callbacks are synchronous, pure binary32 operations; external
// math callbacks may suspend. No default host transcendental substitutes exist.
struct RecoveryEffectServices {
  EventMemoryServices memory;
  void *user = nullptr;
  uint8_t (*byte)(void *, uint32_t) = nullptr;
  std::optional<uint32_t> (*effectEntry)(void *) = nullptr;
  std::optional<uint32_t> (*heapAllocate)(void *, int32_t, uint32_t) = nullptr;
  std::optional<uint32_t> (*mayaAllocate)(void *, uint32_t) = nullptr;
  bool (*mayaPlay)(void *, uint32_t) = nullptr;
  bool (*mayaCalc)(void *, uint32_t) = nullptr;
  bool (*mayaDelete)(void *, uint32_t) = nullptr;
  uint32_t (*tick)(void *) = nullptr; // Original fresh gp->1c load.
  float (*binary)(void *, RecoveryBinary, float, float) = nullptr;
  std::optional<float> (*sine)(void *, float) = nullptr;
  std::optional<float> (*cosine)(void *, float) = nullptr;
  std::optional<float> (*interpolate)(void *, int32_t, int32_t, int32_t, float,
                                      float) = nullptr;
  std::optional<float> (*distance)(void *, float, float, float,
                                   float) = nullptr;
  std::optional<int32_t> (*random)(void *) = nullptr;
  bool (*sfx)(void *, uint32_t, const std::array<float, 3> &) = nullptr;
  bool (*stardust)(void *, int32_t, const std::array<float, 3> &, int32_t,
                   int32_t, float, float) = nullptr;
  bool (*effectDelete)(void *, uint32_t) = nullptr;
  std::optional<float> (*depth)(void *, const std::array<float, 3> &) = nullptr;
  bool (*display)(void *, int32_t, int32_t, uint32_t, uint32_t,
                  float) = nullptr;
};
struct RecoveryEntryCursor {
  bool active = false;
  uint32_t phase = 0, effect = 0, payload = 0, maya = 0, sound = 0,
           heapBytes = 0;
  int32_t kind = 0, amount = 0, requestedAmount = 0;
  EventMemoryServices owner;
  void *serviceOwner = nullptr;
  uint8_t (*byteReader)(void *, uint32_t) = nullptr;
  std::array<float, 3> position{}, requestPosition{};
};
struct RecoveryMainCursor {
  bool active = false;
  uint32_t phase = 0, effect = 0, payload = 0, particle = 0, maya = 0;
  uint32_t remaining = 0, sound = 0;
  int32_t kind = 0, amount = 0, index = 0, completed = 0, counter = 0;
  bool launched = false;
  float targetY = 0, angle = 0, radius = 0, value = 0, depth = 0;
  EventMemoryServices owner;
  void *serviceOwner = nullptr;
  uint8_t (*byteReader)(void *, uint32_t) = nullptr;
  std::array<float, 3> position{}, soundPosition{};
  std::array<float, 5> operands{};
};
// Whole Entry/Main paths for source kinds 0..5,7,8. Kind6 requires the distinct
// Main2 owner and is explicitly unavailable before allocation; never remapped.
bool recoveryEffectKindSupported(int32_t);
std::optional<uint32_t> resumeRecoveryEffectEntry(
    int32_t kind, int32_t amount, const std::array<float, 3> &,
    const RecoveryEffectServices &, RecoveryEntryCursor &);
// true means this driver invocation returned, including original deletion.
// A held service keeps all earlier source stores/calls acknowledged exactly
// once.
bool resumeRecoveryEffectMain(uint32_t effect, const RecoveryEffectServices &,
                              RecoveryMainCursor &);
} // namespace ttyd

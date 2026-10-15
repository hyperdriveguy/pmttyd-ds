#pragma once
#include "event_memory_opcodes.hpp"
#include <cstdint>

namespace ttyd {
// Original addresses are packed guest tokens. A false service result retains
// this call; successful calls must not be repeated by the owning caller.
struct EffectMayaAnimationServices {
  EventMemoryServices memory;
  void *user = nullptr;
  bool (*allocate)(void *, int32_t heap, uint32_t bytes,
                   uint32_t &result) = nullptr;
  bool (*clear)(void *, uint32_t address, uint32_t bytes) = nullptr;
  bool (*release)(void *, int32_t heap, uint32_t address) = nullptr;
  bool (*divideUnsigned)(void *, uint64_t numerator, uint64_t denominator,
                         uint64_t &) = nullptr;
  bool (*convertUnsigned)(void *, uint64_t, float &) = nullptr;
  bool (*matrix)(void *, uint32_t animation, uint32_t channel,
                 uint32_t matrixOut, uint32_t indexOut) = nullptr;
};
// operation0 idle,1 allocate,2 play,3 calculate,4 delete;phase retains child
// completion. Reject changed operation/argument/providers while held. Completion
// resets;raw memory services are synchronous mapped-domain preconditions.
struct EffectMayaAnimationCursor {
  uint8_t operation = 0, phase = 0;
  uint32_t argument = 0, result = 0, bytes = 0, gp = 0, index = 0, table = 0,
           channel = 0;
  uint64_t numerator = 0, denominator = 0, quotient = 0, elapsed = 0;
  float converted = 0;
  EffectMayaAnimationServices owner;
};
// Native retention wraps original synchronous bodies;not original event waits.
// Descriptor+8 channel table;count*34hex+18hex heap3 bytes. Animation+4 frame,
// +8/+c start-ms high/low,+10 descriptor,+14 channel outputs(stride34hex).
bool resumeEffectMayaAnimAllocate(uint32_t descriptor,
                                  const EffectMayaAnimationServices &,
                                  EffectMayaAnimationCursor &,
                                  uint32_t &result);
bool resumeEffectMayaAnimPlay(uint32_t animation,
                              const EffectMayaAnimationServices &,
                              EffectMayaAnimationCursor &);
bool resumeEffectMayaAnimCalculate(uint32_t animation,
                                   const EffectMayaAnimationServices &,
                                   EffectMayaAnimationCursor &);
bool resumeEffectMayaAnimDelete(uint32_t animation,
                                const EffectMayaAnimationServices &,
                                EffectMayaAnimationCursor &);
// Pure CPU leaves matching __div2u80264ad8 and __cvt_sll_flt80264f24.
// The latter converts UNSIGNED bits through binary64, then rounds to binary32.
bool effectMayaDivideUnsigned(void *, uint64_t, uint64_t, uint64_t &);
bool effectMayaConvertUnsigned(void *, uint64_t, float &);
} // namespace ttyd

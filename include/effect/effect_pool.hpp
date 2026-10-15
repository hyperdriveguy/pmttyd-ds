#pragma once
#include "event_memory_opcodes.hpp"
#include <optional>

namespace ttyd {
// These callbacks acknowledge complete execution, not request submission.
// nullopt/false retains the exact source call and operands; async result zero
// is an executed source return and finishes Main without publishing a tag.
struct EffectPoolServices {
  EventMemoryServices memory;
  void *user = nullptr;
  std::optional<uint32_t> (*heapAllocate)(void *, int32_t, uint32_t) = nullptr;
  bool (*clear)(void *, uint32_t, uint32_t) = nullptr;
  bool (*heapFree)(void *, int32_t, uint32_t) = nullptr;
  bool (*update)(void *, uint32_t callback, uint32_t effect) = nullptr;
  bool (*fileFree)(void *, uint32_t) = nullptr;
  std::optional<int32_t> (*fileAsync)(void *, int32_t, uint32_t, uint32_t,
                                    uint32_t, uint32_t, uint32_t) = nullptr;
  std::optional<uint32_t> (*fileAllocate)(void *, int32_t, uint32_t, uint32_t,
                                       uint32_t, uint32_t) = nullptr;
};
struct EffectPoolCursorOwner {
  bool active = false;
  uint32_t phase = 0;
  EventMemoryServices memory;
  void *serviceOwner = nullptr;
};
struct EffectMainCursor : EffectPoolCursorOwner {
  uint32_t count = 0, index = 0, record = 0, callback = 0, handle = 0,
           name = 0;
};
struct EffectInitCursor : EffectPoolCursorOwner {
  uint32_t bytes = 0, address = 0;
};
struct EffectReleaseCursor : EffectPoolCursorOwner {
  uint32_t context = 0, count = 0, index = 0, record = 0, payload = 0;
};
// Whole effEntry8005ec68. Exact-zero free predicate, partial initialization;
// exhaustion intentionally requests the original one-past record. Mapped pool
// capacity/valid guest storage remain the provider's explicit preconditions.
std::optional<uint32_t> effectPoolEntry(const EffectPoolServices &);
bool resumeEffectMain(const EffectPoolServices &, EffectMainCursor &);
// Whole effInit8005ef20 including effInit64801d7a20.
bool resumeEffectInit(const EffectPoolServices &, EffectInitCursor &);
bool resumeEffectAutoRelease(uint32_t context, const EffectPoolServices &,
                             EffectReleaseCursor &);
} // namespace ttyd

#pragma once
#include "field_npc_actor_event.hpp"
namespace ttyd {
// Original callee identity and captured ABI inputs. Floats are source-unit
// operands; sine/cosine return double in number. Sound uses
// ID,root,255,255,0,8.
struct FieldNpcWanderRequest {
  uint32_t function = 0;
  std::array<uint32_t, 6> words{};
  std::array<float, 4> floats{};
};
struct FieldNpcWanderResponse {
  uint32_t word = 0;
  double number = 0;
};
struct FieldNpcWanderEventServices {
  FieldNpcActorEventServices actor;
  bool (*half)(void *, uint32_t, uint16_t &) = nullptr;
  void (*storeHalf)(void *, uint32_t, uint16_t) = nullptr;
  // No effects on false; captured inputs survive retry. Successful effectful
  // requests run exactly once. Source providers may mutate admitted aliases.
  bool (*request)(void *, const FieldNpcWanderRequest &,
                  FieldNpcWanderResponse &) = nullptr;
  // Full stable readable/writable work domain, not a fabricated work default.
  bool (*work)(void *, uint32_t) = nullptr;
};
struct FieldNpcWanderEventCursor {
  bool initialized = false, pending = false;
  void *owner = nullptr;
  EventEntry *entry = nullptr;
  uint32_t id = 0, function = 0, address = 0, args = 0, stage = 0, name = 0,
           actor = 0, mario = 0, work = 0, flags = 0, mask = 0;
  unsigned argument = 1;
  std::array<float, 4> values{};
  float distance = 0, step = 0;
  FieldNpcWanderRequest call;
  FieldNpcWanderResponse response;
};
bool fieldNpcWanderEventFunction(uint32_t);
// Native false provider returns0 retaining cursor. Genuine main return0 clears
// cursor so the next source invocation reloads name/Mario/work and services.
int32_t fieldNpcWanderEvent(uint32_t, EventEntry &, bool first,
                            uint32_t entryAddress, FieldNpcWanderEventCursor &,
                            const FieldNpcWanderEventServices &);
} // namespace ttyd

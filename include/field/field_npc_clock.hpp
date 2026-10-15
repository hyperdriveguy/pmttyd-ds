#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
// Original npcMain clock region80049b88..49dc0. All successful primitive reads,
// stores and numeric requests remain captured across unavailable-provider
// holds. False/empty has no effects; retain mappings and owner until
// completion.
struct FieldNpcClockServices {
  void *user = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  bool (*store)(void *, uint32_t, uint32_t) = nullptr;
  // Raw signed64 argument/result bits, positive32-bit divisor. Source __div2i.
  bool (*divide)(void *, uint64_t, uint32_t, uint64_t &) = nullptr;
  // Source __cvt_sll_flt: signed64 -> rounded double -> rounded float.
  bool (*convert)(void *, uint64_t, float &) = nullptr;
};
struct FieldNpcClockCursor {
  bool active = false;
  FieldNpcClockServices owner;
  uint32_t actor = 0, phase = 0, gp = 0, lo = 0, hi = 0, otherLo = 0,
           otherHi = 0, bus = 0, divisor = 0, index = 0;
  uint64_t number = 0, quotient = 0;
  float converted = 0, seconds = 0;
};
// Caller owns earlier active/query/pose/state/timer work and later Main work.
// This returns true only after this complete clock region; it is not npcMain.
bool updateFieldNpcClock(uint32_t actor, const FieldNpcClockServices &,
                         FieldNpcClockCursor &);
bool nativeFieldNpcClockDivide(void *, uint64_t, uint32_t, uint64_t &);
bool nativeFieldNpcClockConvert(void *, uint64_t, float &);
} // namespace ttyd

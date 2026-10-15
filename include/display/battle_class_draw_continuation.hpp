#pragma once
#include "battle_class_raster_continuation.hpp"
#include <memory>
namespace ttyd {
struct BattleClassDrawContinuationServices {
  BattleClassRasterServices raster;
  void* fontUser=nullptr;
  // Required explicit FontString owner. It must retain any acknowledged glyph/
  // SDK prefix until this same call completes. nullopt suspends; optional0 may
  // acknowledge a source void return. An unretained backend FontString callback
  // cannot be installed here merely because it handles ordinary short labels.
  std::optional<uint32_t> (*retainedFontString)(
      void*,const BattleClassRasterRequest&)=nullptr;
};
class BattleClassDrawContinuationCursor {
public:
  BattleClassDrawContinuationCursor();
  ~BattleClassDrawContinuationCursor();
  BattleClassDrawContinuationCursor(const BattleClassDrawContinuationCursor&)=delete;
  BattleClassDrawContinuationCursor& operator=(const BattleClassDrawContinuationCursor&)=delete;
  bool active() const;
  // Abandons native continuation state only; does not roll back raster effects
  // or retire caller-owned font/resources. Use only at actual owner retirement.
  void reset();
private:
  struct State;
  std::unique_ptr<State> state_;
  friend bool resumeBattleClassDrawIcons(uint32_t,uint32_t,
      const std::vector<BattleActDrawSlot>&,
      const BattleClassDrawContinuationServices&,
      BattleClassDrawContinuationCursor&);
};
// Compose retained DrawMainIcon and whole retained circle/bubble invocations.
// Borrowed slots must retain object identity/lifetime through all waits; other
// services and owners must remain identical. Geometry/resources are supplied by
// caller raster providers, not synthesized here. FontString always uses the
// explicit retained owner above; an absent owner is an unavailable frontier,
// never forwarded to a possibly partially emitting raw raster request.
// Does not begin/clear/check/publish/admit a native batch. Caller validates its
// actual backend after complete success and publishes only its complete pass.
bool resumeBattleClassDrawIcons(uint32_t work,uint32_t rows,
    const std::vector<BattleActDrawSlot>&,
    const BattleClassDrawContinuationServices&,
    BattleClassDrawContinuationCursor&);
}

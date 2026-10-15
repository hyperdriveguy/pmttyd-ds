#pragma once
#include "battle_class_native_numeric.hpp"
#include "battle_class_footer_backend.hpp"
#include "battle_class_footer_provider.hpp"
#include <functional>
namespace ttyd {
// Concrete original footer providers. The actual frame, numeric adapter,
// relocated common bank and GPU resource outlive this stable context. A missing
// sound owner is a real retained frontier, never a successful recording.
class BattleClassNativeFooters {
public:
  BattleClassNativeFooters(BattleClassNativeFrame&,BattleClassNativeNumeric&,
      const BattleClassFooterGpuResource&,BattleActorServices,
      std::function<std::optional<uint32_t>(uint32_t)> resourceWord,
      std::function<bool()> current,
      std::function<bool(uint32_t)> sound={});
  BattleClassNativeFooters(const BattleClassNativeFooters&)=delete;
  BattleClassNativeFooters& operator=(const BattleClassNativeFooters&)=delete;
  bool party();
  bool pinch();
private:
  BattleClassNativeFrame& frame_;
  BattleClassNativeNumeric& numeric_;
  BattleActorServices actors_;
  std::function<std::optional<uint32_t>(uint32_t)> resourceWord_;
  std::function<bool()> current_;
  std::function<bool(uint32_t)> sound_;
  BattleClassFooterServices footer_;
  BattleClassFooterCursor party_,pinch_,button_;
  BattleTexturePlaneServices plane_;
  BattleTexturePlaneCursor planeCursor_;
  std::unique_ptr<BattleClassFooterProvider> provider_;
  bool invoking_=false;
  bool current() const;
  std::optional<uint32_t> raster(const BattleClassRasterRequest&);
};
}

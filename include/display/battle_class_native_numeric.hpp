#pragma once
#include "battle_class_native_frame.hpp"
namespace ttyd {
// Concrete source math/camera adapter, no GX/font/footer acknowledgements.
// This stable noncopyable context and its actual VM outlive every held child.
// Ordinary source total-memory reads require the native VM's asserted mapped
// domain. Optional queries never invent a value for an unavailable address.
struct BattleClassNativeNumericFooters {
  void* user=nullptr;
  bool (*party)(void*)=nullptr;
  bool (*pinch)(void*)=nullptr;
};
class BattleClassNativeNumeric {
public:
  BattleClassNativeNumeric(BattleClassNativeFrame&,BattleActorServices,
      uint32_t reservedCameraBase,
      std::function<std::optional<uint32_t>(uint32_t)> sourceWord,
      std::function<bool()> current,BattleClassNativeNumericFooters={});
  BattleClassNativeNumeric(const BattleClassNativeNumeric&)=delete;
  BattleClassNativeNumeric& operator=(const BattleClassNativeNumeric&)=delete;
  BattleActDrawServices draw();
  BattleClassRasterServices raster();
  std::optional<uint32_t> word(uint32_t) const;
  std::optional<uint32_t> camera(std::optional<uint32_t> index={}) const;
  std::optional<std::array<float,3>> project(uint32_t) const;
private:
  BattleClassNativeFrame& frame_;
  BattleActorServices actors_;
  const BattleClassNativeNumericFooters footers_;
  uint32_t base_=0,capturedCamera_=0;
  std::array<uint32_t,12> view_{};
  bool captured_=false;
  std::function<std::optional<uint32_t>(uint32_t)> sourceWord_;
  std::function<bool()> current_;
  bool current() const;
  bool owns(uint32_t) const;
  bool matrix(BattleAudienceMatrixRequest&) const;
  std::optional<uint32_t> captureCamera(uint32_t);
  uint32_t totalWord(uint32_t) const;
};
}

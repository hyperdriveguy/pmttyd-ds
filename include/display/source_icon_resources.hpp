#pragma once
#include "animation_texture.hpp"
#include "source_icon_gx.hpp"
namespace ttyd {
class EventGpAddresses;
std::optional<uint32_t>
sourceIconCameraIdentity(const CameraManager *,
                         std::optional<uint32_t> index = {});
std::optional<uint32_t> sourceIconRuntimeWord(const CameraManager *,
                                              const EventGpAddresses &,
                                              uint32_t address);

class SourceIconResources {
public:
  static constexpr uint32_t Directory = 0x83000000, Palette = 0x83100000;
  bool load(const char *root);
  bool ready() const { return bool(textures_); }
  uint32_t count() const { return textures_ ? textures_->count() : 0; }
  std::optional<AnimationTPLImage> image(uint32_t index) const {
    return textures_ ? textures_->image(index) : std::nullopt;
  }
  std::optional<uint32_t> word(uint32_t) const;
  std::optional<uint16_t> half(uint32_t) const;
  std::optional<uint32_t> request(const SourceIconGXCall &, void *,
                                  bool (*bind)(void *, uint32_t));

private:
  std::vector<uint8_t> directory_, constants_;
  std::shared_ptr<const AnimationTextureResource> textures_;
  std::vector<uint8_t> objects_;
};
} // namespace ttyd

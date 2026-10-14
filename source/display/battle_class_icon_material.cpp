#include "battle_class_icon_material.hpp"
namespace ttyd {
uint32_t disabledClassIconColor(uint32_t texture, uint32_t record) {
  const auto mean = (((texture >> 24) & 255) + ((texture >> 16) & 255) +
                     ((texture >> 8) & 255)) /
                    3;
  const auto alpha = (texture & 255) * (record & 255) / 255;
  return (mean << 24) | (mean << 16) | (mean << 8) | alpha;
}
} // namespace ttyd

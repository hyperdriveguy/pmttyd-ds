#pragma once
#include <cstdint>
#include <cstdio>
#include <optional>
#include <vector>
namespace ttyd {
// Immutable original-address resource views. Loading data does not admit an
// effect, animation evaluator, texture, sound, driver or display callback.
class RecoveryEffectResources {
public:
  bool load(const char *path);
  bool read(FILE *);
  std::optional<uint8_t> byte(uint32_t address) const;
  std::optional<uint16_t> half(uint32_t address) const;
  std::optional<uint32_t> word(uint32_t address) const;
  struct Span {
    uint32_t address;
    std::vector<uint8_t> bytes;
  };
  const std::vector<Span> &spans() const { return spans_; }

private:
  const uint8_t *find(uint32_t address, uint32_t size) const;
  std::vector<Span> spans_;
};
} // namespace ttyd

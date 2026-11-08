#pragma once
#include "positional_sfx_registry.hpp"
#include <functional>
#include <memory>
namespace ttyd {
// Finite native primary-sample presentation, not psndMain or MusyX execution.
struct MarioActionVoiceSample {
  const int16_t *data = nullptr;
  uint32_t frames = 0, rate = 0;
  std::shared_ptr<const void> memory;
};
enum class MarioActionVoicePcmStatus { Playing, Done };
struct MarioActionVoicePcmServices {
  std::shared_ptr<PositionalSfxRegistry> registry;
  std::shared_ptr<const void> runtime, backend;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  bool (*owns)(void *, unsigned slot,
               const std::shared_ptr<const uint8_t> &birth) = nullptr;
  std::optional<MarioActionVoiceSample> (*sample)(void *,
                                                  uint32_t row) = nullptr;
  // Native callbacks are atomic: false means no queue/mix/stop effect occurred.
  bool (*queue)(void *, unsigned voice, const MarioActionVoiceSample &,
                uint8_t volume, uint8_t pan) = nullptr;
  std::optional<MarioActionVoicePcmStatus> (*status)(void *,
                                                     unsigned voice) = nullptr;
  bool (*mix)(void *, unsigned voice, uint8_t volume, uint8_t pan) = nullptr;
  bool (*stop)(void *, unsigned voice) = nullptr;
};
const char *marioActionVoicePcmFile(uint32_t originalRow);
const char *marioActionVoicePcmName(uint32_t originalRow);
std::optional<uint32_t> marioActionVoicePcmDescriptor(uint32_t originalRow);
// Four stable records pin actual bank, nonwrapping birth token, runtime and PCM
// allocation through real backend retirement. Source handles alone are unsafe.
class MarioActionVoicePcm {
public:
  static constexpr unsigned VoiceCount = 4, SourceSlotCount = 40;
  static_assert(
      std::tuple_size<decltype(PositionalSfxRegistry::slots)>::value ==
          SourceSlotCount,
      "original public-bank capacity");
  bool bind(const MarioActionVoicePcmServices &);
  bool service(bool enabled);
  // Stop actual channels first. Source pending slots stay for genuine owner
  // End.
  bool retire(const std::shared_ptr<const void> &actualRuntime);
  bool bound() const { return bool(services_.registry); }
  bool owns(const std::shared_ptr<const void> &runtime) const {
    return services_.runtime == runtime && bound();
  }
  unsigned playing() const;

private:
  struct Voice {
    std::shared_ptr<const uint8_t> birth;
    MarioActionVoiceSample sample;
    unsigned slot = 0;
    int32_t key = -1, handle = -1;
    uint16_t generation = 0;
  };
  MarioActionVoicePcmServices services_;
  std::array<Voice, VoiceCount> voices_{};
  // One actual queue per source birth, including interrupted unsupported
  // domains.
  std::array<std::shared_ptr<const uint8_t>, SourceSlotCount> handled_{};
  bool invoking_ = false;
  bool current() const;
  bool same(const Voice &) const;
};
} // namespace ttyd

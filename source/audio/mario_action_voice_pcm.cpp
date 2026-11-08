#include "mario_action_voice_pcm.hpp"
#include "harbor_ending_audio_catalog.hpp"
namespace ttyd {
namespace {
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
constexpr uint32_t KeyIndexMask = 0x1fff, PendingFlag = 1,
                   NativeVoiceRate = 22050, MaximumSampleFrames = 65536;
} // namespace
const char *marioActionVoicePcmFile(uint32_t row) {
  switch (row) {
  case 165:
    return "mario-voice-ac1-1.wav";
  case 166:
    return "mario-voice-ac1-2.wav";
  case 167:
    return "mario-voice-ac1-3.wav";
  case 168:
    return "mario-voice-ac1-4.wav";
  case 169:
    return "mario-voice-ac1-5.wav";
  case 171:
    return "mario-voice-ac2-1.wav";
  case 172:
    return "mario-voice-ac2-2.wav";
  case 173:
    return "mario-voice-ac2-3.wav";
  case 174:
    return "mario-voice-ac2-4.wav";
  case 175:
    return "mario-voice-ac2-5.wav";
  case 177:
    return "mario-voice-ac3-1.wav";
  case 178:
    return "mario-voice-ac3-2.wav";
  case 179:
    return "mario-voice-ac3-3.wav";
  case 180:
    return "mario-voice-ac3-4.wav";
  case 181:
    return "mario-voice-ac3-5.wav";
  default:
    return harborEndingPcmFile(row);
  }
}
const char *marioActionVoicePcmName(uint32_t row) {
  switch (row) {
  case 165:
    return "SFX_VOICE_MARIO_AC1_1";
  case 166:
    return "SFX_VOICE_MARIO_AC1_2";
  case 167:
    return "SFX_VOICE_MARIO_AC1_3";
  case 168:
    return "SFX_VOICE_MARIO_AC1_4";
  case 169:
    return "SFX_VOICE_MARIO_AC1_5";
  case 171:
    return "SFX_VOICE_MARIO_AC2_1";
  case 172:
    return "SFX_VOICE_MARIO_AC2_2";
  case 173:
    return "SFX_VOICE_MARIO_AC2_3";
  case 174:
    return "SFX_VOICE_MARIO_AC2_4";
  case 175:
    return "SFX_VOICE_MARIO_AC2_5";
  case 177:
    return "SFX_VOICE_MARIO_AC3_1";
  case 178:
    return "SFX_VOICE_MARIO_AC3_2";
  case 179:
    return "SFX_VOICE_MARIO_AC3_3";
  case 180:
    return "SFX_VOICE_MARIO_AC3_4";
  case 181:
    return "SFX_VOICE_MARIO_AC3_5";
  default:
    return harborEndingPcmName(row);
  }
}
std::optional<uint32_t> marioActionVoicePcmDescriptor(uint32_t row) {
  constexpr uint32_t AuditedEffectBits = 0x20000000;
  if (row >= 165 && row <= 169)
    return AuditedEffectBits | (654 + row - 165);
  if (row >= 171 && row <= 175)
    return AuditedEffectBits | (659 + row - 171);
  if (row >= 177 && row <= 181)
    return AuditedEffectBits | (664 + row - 177);
  return harborEndingPcmDescriptor(row);
}
bool MarioActionVoicePcm::current() const {
  return services_.registry && services_.runtime && services_.backend &&
         services_.current && services_.current(services_.user);
}
bool MarioActionVoicePcm::bind(const MarioActionVoicePcmServices &s) {
  if (invoking_ || bound() || !s.registry || !s.runtime || !s.backend ||
      !s.current || !s.owns || !s.sample || !s.queue || !s.status || !s.mix ||
      !s.stop)
    return false;
  Guard guard(invoking_);
  if (!s.current(s.user))
    return false;
  services_ = s;
  if (!current()) {
    services_ = {};
    return false;
  }
  return true;
}
bool MarioActionVoicePcm::same(const Voice &v) const {
  if (!v.birth || !services_.registry ||
      v.slot >= services_.registry->slots.size() ||
      services_.registry->slotLifetime(v.slot) != v.birth ||
      !services_.owns(services_.user, v.slot, v.birth))
    return false;
  const auto &slot = services_.registry->slots[v.slot];
  return slot.key == v.key && slot.generation == v.generation &&
         slot.engine == -1 && (slot.flags & PendingFlag);
}
unsigned MarioActionVoicePcm::playing() const {
  unsigned n = 0;
  for (const auto &v : voices_)
    n += bool(v.birth);
  return n;
}
bool MarioActionVoicePcm::service(bool enabled) {
  if (invoking_ || !bound())
    return false;
  Guard guard(invoking_);
  if (!current())
    return false;
  bool available = true;
  for (unsigned slot = 0; slot < handled_.size(); ++slot)
    if (handled_[slot] != services_.registry->slotLifetime(slot))
      handled_[slot].reset();
  for (unsigned index = 0; index < VoiceCount; ++index) {
    auto &voice = voices_[index];
    if (!voice.birth)
      continue;
    if (!same(voice)) {
      if (!services_.stop(services_.user, index))
        return false;
      voice = {};
      continue;
    }
    if (!current())
      return false;
    const auto state = services_.status(services_.user, index);
    if (!state) {
      available = false;
      continue;
    }
    if (!current())
      return false;
    // A status provider may expose a source Off/reuse boundary. Never Off a new
    // birth.
    if (!same(voice)) {
      if (!services_.stop(services_.user, index))
        return false;
      voice = {};
      continue;
    }
    if (*state == MarioActionVoicePcmStatus::Done) {
      if (!services_.stop(services_.user, index))
        return false;
      if (!current())
        return false;
      if (same(voice) && !services_.registry->off(voice.handle, {}))
        return false;
      voice = {};
      continue;
    }
    const auto &slot = services_.registry->slots[voice.slot];
    if (slot.volume > 127 || slot.pan > 127) {
      if (!services_.stop(services_.user, index))
        return false;
      voice = {};
      available = false;
      continue;
    }
    if (!services_.mix(services_.user, index, enabled ? slot.volume : 0,
                       slot.pan))
      available = false;
    if (!current())
      return false;
  }
  for (unsigned slotIndex = 0; slotIndex < services_.registry->slots.size();
       ++slotIndex) {
    if (!current())
      return false;
    const auto &source = services_.registry->slots[slotIndex];
    const auto birth = services_.registry->slotLifetime(slotIndex);
    if (!birth || handled_[slotIndex] == birth ||
        !services_.owns(services_.user, slotIndex, birth) || source.key == -1 ||
        source.engine != -1 || !(source.flags & PendingFlag))
      continue;
    const auto row = uint32_t(source.key) & KeyIndexMask;
    if (!marioActionVoicePcmFile(row))
      continue;
    bool existing = false;
    for (const auto &v : voices_)
      if (v.birth == birth) {
        existing = true;
        break;
      }
    if (existing)
      continue;
    unsigned index = 0;
    while (index < VoiceCount && voices_[index].birth)
      ++index;
    if (index == VoiceCount)
      break;
    if (source.volume > 127 || source.pan > 127 || source.delay ||
        source.retriggerDelay) {
      available = false;
      continue;
    }
    Voice capture;
    capture.birth = birth;
    capture.slot = slotIndex;
    capture.key = source.key;
    capture.generation = source.generation;
    capture.handle = int32_t((uint32_t(source.generation) << 8) | slotIndex);
    const auto sample = services_.sample(services_.user, row);
    if (!current())
      return false;
    if (!sample || !sample->memory || !sample->data || !sample->frames ||
        sample->frames > MaximumSampleFrames ||
        sample->rate != (row == HarborStarPointPcmRow ? HarborStarPointPcmRate : NativeVoiceRate)) {
      available = false;
      continue;
    }
    if (!same(capture))
      continue;
    capture.sample = *sample;
    // Queue is the final atomic backend operation. Record publication cannot
    // fail.
    const auto volume = services_.registry->slots[slotIndex].volume,
               pan = services_.registry->slots[slotIndex].pan;
    if (volume > 127 || pan > 127) {
      available = false;
      continue;
    }
    if (!services_.queue(services_.user, index, *sample, enabled ? volume : 0,
                         pan)) {
      available = false;
      continue;
    }
    handled_[slotIndex] = birth;
    voices_[index] = std::move(capture);
    if (!current())
      return false;
  }
  return available;
}
bool MarioActionVoicePcm::retire(
    const std::shared_ptr<const void> &actualRuntime) {
  if (invoking_ || !bound() || services_.runtime != actualRuntime)
    return false;
  Guard guard(invoking_);
  // Retirement does not require source-current admission: it removes actual DSP
  // borrows. False retains successful earlier stops and all remaining
  // ownership.
  for (unsigned index = 0; index < VoiceCount; ++index)
    if (voices_[index].birth) {
      if (!services_.stop(services_.user, index))
        return false;
      voices_[index] = {};
    }
  handled_ = {};
  services_ = {};
  return true;
}
} // namespace ttyd

#include "audio_policy.hpp"
#include "mario_action_voice_pcm.hpp"
#include "native_battle_sound.hpp"
#include "platform.hpp"
#include <algorithm>
#include <cstdio>
namespace ttyd {
namespace {
constexpr unsigned FirstVoiceChannel = 10;
constexpr uint32_t MaximumResidentFrames = 256 * 1024,
                   MaximumSampleFrames = 65536, VoiceRate = 22050;
struct SampleMemory {
  int16_t *data = nullptr;
  uint32_t frames = 0;
  ~SampleMemory() {
    if (data)
      linearFree(data);
  }
};
struct VoiceBackend {
  bool active = false;
  uint32_t residentFrames = 0;
  unsigned queued = 0, completed = 0, stopped = 0;
  std::shared_ptr<PositionalSfxRegistry> registry;
  std::shared_ptr<NativeBattleSound> sound;
  std::array<std::shared_ptr<SampleMemory>, 15> samples{};
  std::array<bool, 15> attempted{};
  std::array<ndspWaveBuf, MarioActionVoicePcm::VoiceCount> waves{};
  static std::optional<unsigned> index(uint32_t row) {
    if (row >= 165 && row <= 169)
      return row - 165;
    if (row >= 171 && row <= 175)
      return row - 171 + 5;
    if (row >= 177 && row <= 181)
      return row - 177 + 10;
    return {};
  }
  std::optional<MarioActionVoiceSample> sample(uint32_t row) {
    const auto i = index(row);
    if (!i)
      return {};
    const char *name = marioActionVoicePcmFile(row);
    const char *sourceName = marioActionVoicePcmName(row);
    const auto descriptor = marioActionVoicePcmDescriptor(row);
    if (!name || !sourceName || !descriptor || !sound)
      return {};
    const auto actualName = sound->sourceRowName(row);
    const auto actualDescriptor = sound->sourceRowDescriptor(row);
    if (!actualName || !actualDescriptor || *actualName != sourceName ||
        *actualDescriptor != *descriptor)
      return {};
    auto &clip = samples[*i];
    if (!clip) {
      if (attempted[*i])
        return {};
      attempted[*i] = true;
      char path[96];
      std::snprintf(path, sizeof(path), "romfs:/%s", name);
      FILE *file = std::fopen(path, "rb");
      if (!file)
        return {};
      unsigned char header[44]{};
      PcmHeader pcm;
      bool valid =
          std::fread(header, 1, sizeof(header), file) == sizeof(header) &&
          pcmHeader(header, pcm) && pcm.channels == 1 &&
          pcm.rate == VoiceRate && pcm.frames &&
          pcm.frames <= MaximumSampleFrames &&
          pcm.frames <= MaximumResidentFrames - residentFrames;
      auto loaded = std::make_shared<SampleMemory>();
      if (valid) {
        loaded->frames = pcm.frames;
        loaded->data =
            static_cast<int16_t *>(linearAlloc(pcm.frames * sizeof(int16_t)));
        valid = loaded->data &&
                std::fread(loaded->data, sizeof(int16_t), pcm.frames, file) ==
                    pcm.frames &&
                std::fgetc(file) == EOF;
      }
      std::fclose(file);
      if (!valid)
        return {};
      DSP_FlushDataCache(loaded->data, loaded->frames * sizeof(int16_t));
      residentFrames += loaded->frames;
      clip = std::move(loaded);
    }
    return MarioActionVoiceSample{clip->data, clip->frames, VoiceRate, clip};
  }
  static void mix(unsigned voice, uint8_t volume, uint8_t pan) {
    const float gain = float(volume) / 127.f, balance = float(pan) / 127.f;
    float output[12]{};
    output[0] = gain * (balance <= .5f ? 1.f : 2 * (1 - balance));
    output[1] = gain * (balance >= .5f ? 1.f : 2 * balance);
    ndspChnSetMix(FirstVoiceChannel + voice, output);
  }
};
} // namespace
struct Audio::HarborVoicePcmState {
  std::shared_ptr<VoiceBackend> backend = std::make_shared<VoiceBackend>();
  MarioActionVoicePcm player;
  std::shared_ptr<const void> runtime;
  std::weak_ptr<const void> retiredRuntime;
};
void Audio::updateHarborVoicePcm(const Game &game) {
  if (!ready)
    return;
  const auto runtime = game.harborTutorial.running()
                           ? game.harborTutorial.nativeGameplayLifetime().lock()
                           : nullptr;
  const auto sound =
      runtime ? game.harborTutorial.nativeGameplaySound() : nullptr;
  if (!runtime || !sound || !game.publicSfx ||
      !sound->usesRegistry(game.publicSfx.get())) {
    stopHarborVoicePcm();
    return;
  }
  if (!harborVoicePcm)
    harborVoicePcm = std::make_shared<HarborVoicePcmState>();
  auto &state = *harborVoicePcm;
  // Completion retirement belongs to this exact incarnation. A later service
  // visit cannot reacquire its channels while the field handoff is still held.
  if (state.retiredRuntime.lock() == runtime)
    return;
  if (state.runtime != runtime || state.backend->registry != game.publicSfx ||
      state.backend->sound != sound) {
    if (state.runtime && !state.player.retire(state.runtime))
      return;
    // Genuine exclusive handoff: clear house queue BEFORE claiming10..13.
    // Its subsequent nullptr updates see queuedVoices0 and cannot clear us.
    if (!openingHousePcm.update(nullptr, true))
      return;
    state.runtime = runtime;
    state.retiredRuntime.reset();
    auto &backend = *state.backend;
    backend.registry = game.publicSfx;
    backend.sound = sound;
    backend.active = true;
    MarioActionVoicePcmServices services;
    services.registry = game.publicSfx;
    services.runtime = runtime;
    services.backend = state.backend;
    services.user = &backend;
    services.current = [](void *p) {
      const auto &b = *static_cast<VoiceBackend *>(p);
      return b.active && b.registry && b.sound &&
             b.sound->usesRegistry(b.registry.get());
    };
    services.owns = [](void *p, unsigned slot,
                       const std::shared_ptr<const uint8_t> &birth) {
      const auto &b = *static_cast<VoiceBackend *>(p);
      return b.active && b.sound &&
             b.sound->ownsPending(b.registry.get(), slot, birth);
    };
    services.sample = [](void *p, uint32_t row) {
      return static_cast<VoiceBackend *>(p)->sample(row);
    };
    services.queue = [](void *p, unsigned voice,
                        const MarioActionVoiceSample &sample, uint8_t volume,
                        uint8_t pan) {
      auto &b = *static_cast<VoiceBackend *>(p);
      if (!b.active || voice >= b.waves.size() || !sample.memory ||
          !sample.data || !sample.frames || sample.rate != VoiceRate)
        return false;
      auto &wave = b.waves[voice];
      if (wave.status != NDSP_WBUF_FREE)
        return false;
      wave = {};
      wave.data_pcm16 = const_cast<int16_t *>(sample.data);
      wave.nsamples = sample.frames;
      wave.looping = false;
      const unsigned channel = FirstVoiceChannel + voice;
      ndspChnSetInterp(channel, NDSP_INTERP_LINEAR);
      ndspChnSetRate(channel, VoiceRate);
      ndspChnSetFormat(channel, NDSP_FORMAT_MONO_PCM16);
      VoiceBackend::mix(voice, volume, pan);
      ndspChnWaveBufAdd(channel, &wave);
      ++b.queued;
      return true;
    };
    services.status =
        [](void *p,
           unsigned voice) -> std::optional<MarioActionVoicePcmStatus> {
      auto &b = *static_cast<VoiceBackend *>(p);
      if (!b.active || voice >= b.waves.size())
        return {};
      const auto status = b.waves[voice].status;
      if (status == NDSP_WBUF_DONE) {
        ++b.completed;
        return MarioActionVoicePcmStatus::Done;
      }
      if (status == NDSP_WBUF_QUEUED || status == NDSP_WBUF_PLAYING)
        return MarioActionVoicePcmStatus::Playing;
      return {};
    };
    services.mix = [](void *p, unsigned voice, uint8_t volume, uint8_t pan) {
      auto &b = *static_cast<VoiceBackend *>(p);
      if (!b.active || voice >= b.waves.size())
        return false;
      VoiceBackend::mix(voice, volume, pan);
      return true;
    };
    services.stop = [](void *p, unsigned voice) {
      auto &b = *static_cast<VoiceBackend *>(p);
      if (!b.active || voice >= b.waves.size())
        return false;
      ndspChnWaveBufClear(FirstVoiceChannel + voice);
      b.waves[voice] = {};
      ++b.stopped;
      return true;
    };
    if (!state.player.bind(services)) {
      backend.active = false;
      state.runtime.reset();
      return;
    }
  }
  // Muting queues real PCM at zero mix. Only real NDSP completion can Off it.
  state.player.service(game.progress.sound);
}
bool Audio::retireHarborVoicePcm(const std::shared_ptr<const void> &runtime) {
  if (!runtime)
    return false;
  if (!harborVoicePcm)
    return true;
  auto &state = *harborVoicePcm;
  if (!state.runtime)
    return state.retiredRuntime.lock() == runtime;
  if (state.runtime != runtime)
    return false;
  if (!state.player.retire(runtime))
    return false;
  state.backend->active = false;
  state.backend->sound.reset();
  state.backend->registry.reset();
  state.retiredRuntime = runtime;
  state.runtime.reset();
  return true;
}
bool Audio::harborBattleAudioRetired(
    const std::shared_ptr<const void> &runtime) const {
  return runtime && harborVoicePcm && harborVoicePcm->retiredRuntime.lock() == runtime;
}
bool Audio::retireHarborBattleAudio(
    const std::shared_ptr<const void> &runtime) {
  if (!runtime)
    return false;
  // A foreign/never-bound track12 owner cannot be certified from its number.
  if (!harborVoicePcm)
    return musicTrack != 12;
  const auto &state = *harborVoicePcm;
  if (state.runtime != runtime && state.retiredRuntime.lock() != runtime)
    return false;
  if (!retireHarborVoicePcm(runtime))
    return false;
  if (ready) {
    LightLock_Lock(&musicMutex);
    for (unsigned slot = 0; slot < 3; ++slot)
      if (music[slot].track == 12)
        stopMusic(slot);
    if (musicTrack == 12)
      musicTrack = -1;
    LightLock_Unlock(&musicMutex);
  }
  return true;
}
void Audio::stopHarborVoicePcm() {
  if (harborVoicePcm && harborVoicePcm->runtime)
    retireHarborVoicePcm(harborVoicePcm->runtime);
}
void Audio::releaseHarborVoicePcm() { harborVoicePcm.reset(); }
} // namespace ttyd

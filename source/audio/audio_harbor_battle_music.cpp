#include "audio_policy.hpp"
#include "harbor_battle_music_owner.hpp"
#include "platform.hpp"
#include <cstdio>
namespace ttyd {
struct Audio::HarborBattleMusicState {
  HarborBattleMusicOwner owner;
  std::shared_ptr<const void> ownerRuntime;
  std::shared_ptr<const void> lifetime = std::make_shared<unsigned char>(0);
  bool alive = true, poisoned = false;
  HarborBattleMusicServices services;
};
bool Audio::beginHarborOwnedMusic(const HarborBattleMusicBinding &binding,
                                  bool victory, bool enabled) {
  if (!ready || !binding.runtime)
    return false;
  if (!harborBattleMusic)
    harborBattleMusic = std::make_shared<HarborBattleMusicState>();
  auto &s = *harborBattleMusic;
  if (!s.alive || s.poisoned || s.owner.busy())
    return false;
  if (s.owner.active() && !s.owner.owns(binding.runtime))
    return false;
  if (!s.owner.active() && !s.owner.owns(binding.runtime) &&
      s.services.lifetime) {
    s.owner = HarborBattleMusicOwner{};
  }
  if (!s.services.lifetime) {
    auto &services = s.services;
    services.lifetime = s.lifetime;
    services.user = this;
    services.current = [](void *p) {
      const auto &a = *static_cast<Audio *>(p);
      // Backend lifetime stays available for real cleanup after an admission
      // failure; poisoned prevents new starts, not hardware retirement.
      return a.ready && a.harborBattleMusic && a.harborBattleMusic->alive;
    };
    services.start = [](void *p, int32_t track,
                        bool on) -> std::optional<HarborBattleMusicLease> {
      auto &a = *static_cast<Audio *>(p);
      if (!a.ready || !a.harborBattleMusic || !a.harborBattleMusic->alive ||
          a.harborBattleMusic->poisoned)
        return {};
      if (track != HarborBattleMusicOwner::BattleTrack &&
          track != HarborBattleMusicOwner::VictoryTrack)
        return {};
      // Immutable RomFS preflight precedes clearing ANY actual previous stream.
      FILE *file = std::fopen(musicAssetPath(track, a.switchSoundtrack), "rb");
      if (!file)
        return {};
      unsigned char header[44];
      PcmHeader pcm;
      bool valid =
          std::fread(header, 1, sizeof(header), file) == sizeof(header) &&
          pcmHeader(header, pcm) && pcm.frames &&
          (pcm.channels == 1 || pcm.channels == 2) && pcm.rate;
      if (valid) {
        valid = std::fseek(file, 0, SEEK_END) == 0;
        if (valid) {
          const long size = std::ftell(file);
          valid = size >= 0 && uint64_t(size) >= 44ull + uint64_t(pcm.frames) *
                                                             pcm.channels *
                                                             sizeof(int16_t);
        }
      }
      std::fclose(file);
      if (!valid)
        return {};
      LightLock_Lock(&a.musicMutex);
      if (a.musicSlot >= 3 ||
          (!a.music[a.musicSlot].nativeGeneration ||
           a.music[a.musicSlot].nativeGeneration >= UINT64_MAX - 1)) {
        LightLock_Unlock(&a.musicMutex);
        return {};
      }
      for (unsigned i = 0; i < 3; ++i)
        a.stopMusic(i);
      a.startMusic(a.musicSlot, track, on ? 127 : 0);
      const auto &stream = a.music[a.musicSlot];
      if (!stream.file || !stream.nativeGeneration) {
        // This exact serialized attempt owns the selected slot. Clear any
        // native prefix before poisoning; never adopt it as a successful lease.
        a.stopMusic(a.musicSlot);
        a.musicTrack = -1;
        a.harborBattleMusic->poisoned = true;
        LightLock_Unlock(&a.musicMutex);
        return {};
      }
      a.musicTrack = track;
      HarborBattleMusicLease lease{a.musicSlot, stream.nativeGeneration, track};
      LightLock_Unlock(&a.musicMutex);
      return lease;
    };
    services.valid = [](void *p, const HarborBattleMusicLease &q) {
      auto &a = *static_cast<Audio *>(p);
      LightLock_Lock(&a.musicMutex);
      const bool valid = a.ready && q.slot < 3 && q.generation &&
                         a.music[q.slot].nativeGeneration == q.generation &&
                         a.music[q.slot].track == q.track;
      LightLock_Unlock(&a.musicMutex);
      return valid;
    };
    services.mix = [](void *p, const HarborBattleMusicLease &q, bool on) {
      auto &a = *static_cast<Audio *>(p);
      LightLock_Lock(&a.musicMutex);
      const bool valid = q.slot < 3 && q.generation &&
                         a.music[q.slot].nativeGeneration == q.generation &&
                         a.music[q.slot].track == q.track;
      if (valid)
        a.musicMix(q.slot, on ? 127 : 0);
      LightLock_Unlock(&a.musicMutex);
      return valid;
    };
    services.stop = [](void *p, const HarborBattleMusicLease &q) {
      auto &a = *static_cast<Audio *>(p);
      if (!a.ready)
        return false;
      LightLock_Lock(&a.musicMutex);
      if (q.slot < 3 && q.generation &&
          a.music[q.slot].nativeGeneration == q.generation &&
          a.music[q.slot].track == q.track) {
        a.stopMusic(q.slot);
        if (a.musicTrack == q.track)
          a.musicTrack = -1;
      }
      LightLock_Unlock(&a.musicMutex);
      return true;
    };
  }
  s.ownerRuntime = binding.runtime;
  return s.owner.begin(binding, s.services, victory, enabled);
}
bool Audio::beginHarborBattleMusic(const HarborBattleMusicBinding &b,
                                   bool enabled) {
  return beginHarborOwnedMusic(b, false, enabled);
}
bool Audio::beginHarborVictoryMusic(const HarborBattleMusicBinding &b,
                                    bool enabled) {
  return beginHarborOwnedMusic(b, true, enabled);
}
bool Audio::updateHarborVictoryMusic(const std::shared_ptr<const void> &runtime,
                                     bool enabled) {
  return harborBattleMusic && harborBattleMusic->owner.victory() &&
         harborBattleMusic->owner.update(runtime, enabled);
}
bool Audio::serviceHarborOwnedMusic(bool enabled) {
  if (!harborBattleMusic || !harborBattleMusic->owner.active())
    return false;
  harborBattleMusic->owner.update(harborBattleMusic->ownerRuntime, enabled);
  return true;
}
bool Audio::canRetireHarborOwnedMusic(
    const std::shared_ptr<const void> &runtime) {
  if (!runtime)
    return false;
  if (!ready)
    return !harborBattleMusic || harborBattleMusic->owner.retired(runtime);
  if (harborBattleMusic && (harborBattleMusic->owner.busy() ||
                            !harborBattleMusic->owner.owns(runtime)))
    return false;
  LightLock_Lock(&musicMutex);
  bool valid = true;
  for (unsigned i = 0; i < 3; ++i)
    if (music[i].track == HarborBattleMusicOwner::BattleTrack ||
        music[i].track == HarborBattleMusicOwner::VictoryTrack) {
      const HarborBattleMusicLease lease{i, music[i].nativeGeneration,
                                         music[i].track};
      if (!harborBattleMusic || !harborBattleMusic->owner.protects(lease))
        valid = false;
    }
  LightLock_Unlock(&musicMutex);
  return valid;
}
bool Audio::harborOwnedMusicRetired(
    const std::shared_ptr<const void> &runtime) const {
  return !harborBattleMusic || harborBattleMusic->owner.retired(runtime);
}
bool Audio::retireHarborOwnedMusic(const std::shared_ptr<const void> &runtime) {
  if (!harborBattleMusic)
    return true;
  if (!harborBattleMusic->owner.retire(runtime))
    return false;
  harborBattleMusic->ownerRuntime.reset();
  return true;
}
void Audio::releaseHarborOwnedMusic() {
  if (harborBattleMusic) {
    harborBattleMusic->alive = false;
    harborBattleMusic.reset();
  }
}
// Native lifecycle service for BOTH admitted battle12 and victory2. A
// same-runtime retired receipt means handled silence, never queue replay.
bool Audio::updateHarborBattleMusic(const std::shared_ptr<const void>& runtime,bool enabled){
 if(!harborBattleMusic)return false;
 if(harborBattleMusic->owner.retired(runtime))return true;
 return harborBattleMusic->owner.update(runtime,enabled);
}
bool Audio::stopHarborVictoryMusic(const std::shared_ptr<const void>& runtime){
 if(!runtime||!harborBattleMusic||!harborBattleMusic->owner.owns(runtime))return false;
 if(harborBattleMusic->owner.retired(runtime))return true;
 if(!harborBattleMusic->owner.victory()||!canRetireHarborOwnedMusic(runtime))return false;
 // Finite native immediate-stop contribution at source BGMOff201, not
 // the original envelope/driver. Actual public-slot PCM voices keep running.
 return retireHarborOwnedMusic(runtime);
}
} // namespace ttyd

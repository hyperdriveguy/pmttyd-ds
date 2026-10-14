#pragma once
#include "game.hpp"
#include <cstdint>
#include <cstring>
namespace ttyd {
enum class AudioCue : int { None=-1, Confirm, Jump, Hit, Success, Guard };
inline AudioCue audioCue(const Game& game) {
    if(game.sound==Sound::None || game.scene==Scene::Victory || game.scene==Scene::Defeat || game.scene==Scene::FieldDefeat) return AudioCue::None;
    switch(game.sound) {
    case Sound::Confirm:return AudioCue::Confirm;
    case Sound::Jump:return AudioCue::Jump;
    case Sound::Hit:return AudioCue::Hit;
    case Sound::Success:return game.scene==Scene::Battle && game.phase==Phase::EnemyImpact ? AudioCue::Guard : AudioCue::Success;
    default:return AudioCue::None;
    }
}
inline int audioTrack(Scene scene,bool enabled) {
    if(!enabled || scene==Scene::Defeat || scene==Scene::FieldDefeat)return -1;
    return scene==Scene::Battle?1:scene==Scene::Victory?2:0;
}
// This is a native presentation adapter, not recovered Switch event routing.
inline int soundtrackTrack(Scene scene,bool enabled,bool harborDanger,int professorMusic,
                           uint32_t room,bool thinking,bool switchSoundtrack) {
    int track=audioTrack(scene,enabled);
    if(!enabled)return -1;
    const bool field=scene==Scene::Explore || scene==Scene::Dialogue;
    if(field) {
        if(switchSoundtrack) {
            constexpr int districts[]={0,13,14,15,16,11};
            if(room<6)track=districts[room];
        } else if(room==5)track=11;
        if(harborDanger)track=3;
    }
    if(switchSoundtrack && scene==Scene::Battle && thinking)track=17;
    if(professorMusic)track=professorMusic<0?-1:10;
    return track;
}
inline const char* musicAssetPath(int track,bool switchSoundtrack) {
    if(track==18)return "romfs:/field-item-get-key-fanfare.wav"; // Original row217 in both soundtrack modes.
    constexpr const char* original[]={
        "romfs:/rogueport.wav","romfs:/battle.wav","romfs:/victory.wav",
        "romfs:/harbor-danger.wav","romfs:/title.wav","romfs:/demo-book.wav",
        "romfs:/demo-market.wav","romfs:/opening-house.wav","romfs:/opening-voyage.wav",
        "romfs:/opening-sailing.wav","romfs:/professor-music.wav",
        "romfs:/underground-music.wav","romfs:/harbor-tutorial-music.wav"};
    constexpr const char* remake[]={
        "romfs:/soundtrack-switch/rogueport.wav","romfs:/soundtrack-switch/battle.wav",
        "romfs:/soundtrack-switch/victory.wav","romfs:/soundtrack-switch/harbor-danger.wav",
        "romfs:/soundtrack-switch/title.wav","romfs:/soundtrack-switch/demo-book.wav",
        "romfs:/soundtrack-switch/demo-market.wav","romfs:/soundtrack-switch/opening-house.wav",
        "romfs:/soundtrack-switch/opening-voyage.wav","romfs:/soundtrack-switch/opening-sailing.wav",
        "romfs:/soundtrack-switch/professor-music.wav","romfs:/soundtrack-switch/underground-music.wav",
        "romfs:/soundtrack-switch/harbor-tutorial-music.wav","romfs:/soundtrack-switch/rogueport-harbor.wav",
        "romfs:/soundtrack-switch/rogueport-east.wav","romfs:/soundtrack-switch/rogueport-west.wav",
        "romfs:/soundtrack-switch/rogueport-station.wav","romfs:/soundtrack-switch/battle-think.wav"};
    if(track<0 || track>=18)return nullptr;
    // The original US book waits retain a looping BGM owner until Off. The
    // Switch arrangement is a shorter nonlooped cue and exhausts before that
    // fade request. Use the original loop rather than inventing loop points.
    if(track==5)return original[5];
    if(switchSoundtrack)return remake[track];
    if(track==17)return original[1];
    return original[track<13?track:0];
}
struct PcmHeader {uint32_t rate=0,channels=0,frames=0;};
inline bool pcmHeader(const unsigned char* p,PcmHeader& out) {
    auto u16=[p](int i)->uint32_t {return p[i]|uint32_t(p[i+1])<<8;};
    auto u32=[p](int i)->uint32_t {return p[i]|uint32_t(p[i+1])<<8|uint32_t(p[i+2])<<16|uint32_t(p[i+3])<<24;};
    if(std::memcmp(p,"RIFF",4) || std::memcmp(p+8,"WAVEfmt ",8) || std::memcmp(p+36,"data",4)
        || u32(16)!=16 || u16(20)!=1 || u16(34)!=16)return false;
    uint32_t channels=u16(22),rate=u32(24),bytes=u32(40);
    if(channels<1 || channels>2 || rate<8000 || rate>48000 || !bytes || bytes%(channels*2)
        || u16(32)!=channels*2 || u32(28)!=rate*channels*2 || uint64_t(u32(4))!=uint64_t(bytes)+36)return false;
    out={rate,channels,bytes/(channels*2)};return true;
}
}

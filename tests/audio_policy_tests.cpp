#include "audio_policy.hpp"
#include <cassert>
#include <array>
#include <cstring>
using namespace ttyd;
int main() {
    Game game;game.scene=Scene::Battle;game.phase=Phase::EnemyImpact;game.sound=Sound::Success;
    assert(audioCue(game)==AudioCue::Guard);
    game.phase=Phase::Impact;assert(audioCue(game)==AudioCue::Success);
    game.scene=Scene::Victory;assert(audioCue(game)==AudioCue::None);
    assert(audioTrack(Scene::Dialogue,true)==audioTrack(Scene::Explore,true));
    assert(audioTrack(Scene::Battle,true)==1);assert(audioTrack(Scene::Victory,true)==2);
    assert(audioTrack(Scene::Defeat,true)==-1);assert(audioTrack(Scene::FieldDefeat,true)==-1);
    assert(audioTrack(Scene::Battle,false)==-1);
    std::array<unsigned char,44> raw{};
    auto u16=[&](int o,unsigned v){raw[o]=v;raw[o+1]=v>>8;};
    auto u32=[&](int o,unsigned v){for(int n=0;n<4;++n)raw[o+n]=v>>(8*n);};
    std::memcpy(raw.data(),"RIFF",4);std::memcpy(raw.data()+8,"WAVEfmt ",8);std::memcpy(raw.data()+36,"data",4);
    u32(4,44);u32(16,16);u16(20,1);u16(22,1);u32(24,22050);u32(28,44100);u16(32,2);u16(34,16);u32(40,8);
    PcmHeader pcm;assert(pcmHeader(raw.data(),pcm));assert(pcm.frames==4 && pcm.channels==1 && pcm.rate==22050);
    u32(40,7);assert(!pcmHeader(raw.data(),pcm));u32(40,8);
    u16(20,3);assert(!pcmHeader(raw.data(),pcm));u16(20,1);
    u32(28,22050);assert(!pcmHeader(raw.data(),pcm));u32(28,44100);
    u32(4,43);assert(!pcmHeader(raw.data(),pcm));
}

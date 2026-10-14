#include "platform.hpp"
#include "audio_policy.hpp"
#include <cstdio>
namespace ttyd {
namespace {
struct Lock {LightLock& mutex;explicit Lock(LightLock& value):mutex(value){LightLock_Lock(&mutex);}~Lock(){LightLock_Unlock(&mutex);}};
constexpr int KeyItemTrack=18;
}
bool Audio::keyItemFanfareReady() {
    if(!ready)return false;
    Lock lock(musicMutex);
    if(keyItemResourceAttempted)return keyItemResourceReady;
    keyItemResourceAttempted=true;
    FILE* file=std::fopen(musicAssetPath(KeyItemTrack,false),"rb");
    if(!file)return false;
    unsigned char header[44]{};PcmHeader pcm;
    bool ok=std::fread(header,1,44,file)==44 && pcmHeader(header,pcm) &&
        pcm.rate==32000 && pcm.channels==2 && pcm.frames==83888;
    if(ok)ok=std::fseek(file,0,SEEK_END)==0 && std::ftell(file)==44+83888*4;
    std::fclose(file);
    // The source COMMON restart marker is nonlooping. Reject stale sidecars.
    FILE* loop=std::fopen("romfs:/field-item-get-key-fanfare.wav.loop","rb");
    if(loop){std::fclose(loop);ok=false;}
    keyItemResourceReady=ok;return ok;
}
bool Audio::prepareFieldItemGetAudio() {
    if(!keyItemFanfareReady())return false;
    Lock lock(musicMutex);retireKeyItemFanfare();
    if(keyItemSlot>=0 || keyItemTailSlot>=0)return false;
    auto& stream=music[musicSlot];
    // Muted presentation still owns the source field stream/logical clock.
    if(!stream.file && musicTrack==-1 && fieldItemMuted){startMusic(musicSlot,0,70);if(stream.file)musicTrack=0;}
    if(!stream.file || stream.track!=0 || stream.channels!=2 || stream.rate!=32000)return false;
    bool free=false;for(const auto& candidate:music)free|=!candidate.file;if(!free)return false;
    if(itemGetFieldSlot>=0)return itemGetFieldSlot==int(musicSlot) && itemGetFieldTrack==0;
    itemGetFieldSlot=int(musicSlot);itemGetFieldTrack=0;itemGetFieldEnvelope.bind(70,100,false);return true;
}
void Audio::retireKeyItemFanfare() {
    const int slots[2]={keyItemSlot,keyItemTailSlot};
    for(int slot:slots){
        if(slot<0)continue;
        auto& stream=music[unsigned(slot)];
        if(!stream.file){stopMusic(unsigned(slot));continue;}
        if(!stream.ended)continue;
        bool retained=false;
        for(const auto& buffer:stream.buffers)
            retained|=buffer.status==NDSP_WBUF_QUEUED || buffer.status==NDSP_WBUF_PLAYING;
        if(!retained)stopMusic(unsigned(slot));
    }
}
bool Audio::beginKeyItemFanfare(int32_t gpRate) {
    if(gpRate<=0 || !keyItemFanfareReady())return false;
    Lock lock(musicMutex);retireKeyItemFanfare();
    if(keyItemSlot>=0 || keyItemTailSlot>=0)return false; // Fresh group/clean-tail domain; replacement unbound.
    unsigned slot=0;while(slot<3 && music[slot].file)++slot;
    if(slot==3)return false; // Never evict another retained group/tail to fabricate admission.
    startMusic(slot,KeyItemTrack,80);
    if(!music[slot].file)return false;
    keyItemSlot=int(slot);keyItemEnvelope.bind(80);
    // Source row217/type2 pan64 publishes80 to each hard-panned channel.
    // sndStreamMix uses original1/127; no native .65 adapter factor here.
    // Source On_f_d invokes all admitted groups before returning.
    return itemGetBgmMain(gpRate);
}
int32_t Audio::keyItemFanfareCheck() {
    if(!ready)return -1;
    Lock lock(musicMutex);retireKeyItemFanfare();return keyItemSlot<0?-1:0;
}
void Audio::releaseKeyItemFanfare() {
    if(!ready)return;
    Lock lock(musicMutex);
    if(keyItemSlot>=0)stopMusic(unsigned(keyItemSlot));
    if(keyItemTailSlot>=0)stopMusic(unsigned(keyItemTailSlot));
}
bool Audio::fadeKeyItemFanfare(int32_t milliseconds,int32_t gpRate) {
    if(!ready)return false;
    const auto duration=fieldBgmDuration(milliseconds,gpRate);if(!duration)return false;
    Lock lock(musicMutex);retireKeyItemFanfare();
    return keyItemSlot<0 || keyItemEnvelope.off201(*duration);
}
bool Audio::itemGetBgmMain(int32_t gpRate) {
    if(itemGetFieldSlot>=0){
        const auto publication=itemGetFieldEnvelope.main();if(!publication)return false;
        if(publication->published)musicMix(unsigned(itemGetFieldSlot),publication->volume);
    }
    if(keyItemSlot>=0){
        const auto publication=keyItemEnvelope.main();if(!publication)return false;
        if(publication->published)musicMix(unsigned(keyItemSlot),publication->volume);
        if(publication->detach){
            if(!keyItemTail.begin(float(publication->volume),publication->tailMilliseconds,gpRate))return false;
            keyItemTailSlot=keyItemSlot;keyItemSlot=-1;
        }
    }
    return true;
}
bool Audio::tickKeyItemAudio(int32_t gpRate) {
    if(!ready || gpRate<=0)return false;
    Lock lock(musicMutex);retireKeyItemFanfare();
    if(!itemGetBgmMain(gpRate))return false;
    // Source outer order: BGMMain publishes/detaches, then SoundSSMain.
    // The first detached subtraction can happen in this same admitted call.
    if(keyItemTailSlot>=0){
        const auto publication=keyItemTail.main();
        if(publication.published)musicMix(unsigned(keyItemTailSlot),publication.volume);
        if(publication.stop)stopMusic(unsigned(keyItemTailSlot));
    }
    return true;
}
bool Audio::retainItemGetFieldMusic(uint8_t base,int8_t global,bool bypass) {
    if(!ready)return false;
    Lock lock(musicMutex);
    const auto& stream=music[musicSlot];
    if(!stream.file || stream.channels!=2 || stream.track==KeyItemTrack)return false;
    if(itemGetFieldSlot>=0)return itemGetFieldSlot==int(musicSlot) && itemGetFieldTrack==stream.track;
    itemGetFieldSlot=int(musicSlot);itemGetFieldTrack=stream.track;
    itemGetFieldEnvelope.bind(base,global,bypass);return true;
}
bool Audio::itemGetFieldMusicSilent() {
    if(!ready)return false;
    Lock lock(musicMutex);return itemGetFieldSlot>=0 && itemGetFieldEnvelope.silent();
}
bool Audio::muteItemGetFieldMusic(int32_t gpRate) {
    if(!ready)return false;
    const auto duration=fieldBgmDuration(1000,gpRate);if(!duration)return false;
    Lock lock(musicMutex);
    return itemGetFieldSlot>=0 && itemGetFieldEnvelope.mute3800(*duration);
}
bool Audio::resumeItemGetFieldMusic(int32_t gpRate) {
    if(!ready)return false;
    const auto duration=fieldBgmDuration(1000,gpRate);if(!duration)return false;
    Lock lock(musicMutex);retireKeyItemFanfare();
    if(itemGetFieldSlot<0 || !itemGetFieldEnvelope.resumeA0(*duration))return false;
    // No fopen/seek/startMusic: source NULLcontinue retains the authored cursor.
    return itemGetBgmMain(gpRate);
}

}

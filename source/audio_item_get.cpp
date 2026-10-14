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
void Audio::retireKeyItemFanfare() {
    if(keyItemSlot<0)return;
    auto& stream=music[unsigned(keyItemSlot)];
    if(!stream.file){keyItemSlot=-1;return;}
    if(!stream.ended)return;
    for(const auto& buffer:stream.buffers)
        if(buffer.status==NDSP_WBUF_QUEUED || buffer.status==NDSP_WBUF_PLAYING)return;
    stopMusic(unsigned(keyItemSlot));
}
bool Audio::beginKeyItemFanfare() {
    if(!keyItemFanfareReady())return false;
    Lock lock(musicMutex);retireKeyItemFanfare();
    if(keyItemSlot>=0)return false; // Fresh source group; replacement owner unbound.
    unsigned slot=0;while(slot<3 && music[slot].file)++slot;
    if(slot==3)return false; // Never evict another retained group/tail to fabricate admission.
    startMusic(slot,KeyItemTrack,80);
    if(!music[slot].file)return false;
    keyItemSlot=int(slot);
    // Source row217/type2 pan64 publishes80 to each hard-panned channel.
    // sndStreamMix uses original1/127; no native .65 adapter factor here.
    // startMusic publishes this gain before submitting the first buffer.
    return true;
}
int32_t Audio::keyItemFanfareCheck() {
    if(!ready)return -1;
    Lock lock(musicMutex);retireKeyItemFanfare();return keyItemSlot<0?-1:0;
}
void Audio::releaseKeyItemFanfare() {
    if(!ready)return;
    Lock lock(musicMutex);if(keyItemSlot>=0)stopMusic(unsigned(keyItemSlot));
}
}

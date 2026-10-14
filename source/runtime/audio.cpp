#include "platform.hpp"
#include "audio_policy.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t StreamFrames=4096;
struct MusicLock {LightLock& lock;explicit MusicLock(LightLock& value):lock(value){LightLock_Lock(&lock);}~MusicLock(){LightLock_Unlock(&lock);}};
constexpr unsigned MusicChannels[3]={1,0,18};
constexpr const char* CueNames[]={"confirm","jump","hit","success","guard","save-block","goomba-step-left","goomba-step-right"};
constexpr const char* OriginalCueNames[]={"SFX_CURSOR_OK1","SFX_MARIO_JUMP1","SFX_MARIO_DAMAGE1","SFX_AC_OK1","SFX_MARIO_GUARD1","SFX_MOBJ_BLOCK_SAVE1","SFX_FLD_NPC_KURI_MOVE1L","SFX_FLD_NPC_KURI_MOVE1R"};
bool readHeader(FILE* file,PcmHeader& pcm) {
    unsigned char bytes[44]{};
    return std::fread(bytes,1,44,file)==44 && pcmHeader(bytes,pcm);
}
}
bool Audio::frontendTrackReady(int track) {
    // Source title waits BGM resource admission. Playback availability and the
    // user's mute setting do not determine whether the original asset exists.
    if(track!=4)return ready && music[musicSlot].file && music[musicSlot].track==track;
    if(titleResourceAttempted)return titleResourceReady;
    titleResourceAttempted=true;
    FILE* file=std::fopen(musicAssetPath(4,switchSoundtrack),"rb");
    if(!file)return false;
    PcmHeader pcm;
    if(readHeader(file,pcm) && pcm.frames && std::fseek(file,0,SEEK_END)==0){
        const long length=std::ftell(file);
        titleResourceReady=length>=0 && uint64_t(length)>=44ull+uint64_t(pcm.frames)*pcm.channels*2;
    }
    std::fclose(file);return titleResourceReady;
}
bool Audio::init() {
    LightLock_Init(&musicMutex);LightEvent_Init(&musicWake,RESET_ONESHOT);
    // User preference: original ISO music. Converted Switch arrangements stay optional library assets.
    switchSoundtrack=false;
    if(R_FAILED(ndspInit())) return false;
    ready=true;ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    streamData=(int16_t*)linearAlloc(StreamFrames*2*3*3*2);
    if(!streamData) {shutdown();return false;}
    for(unsigned slot=0;slot<3;++slot)for(int n=0;n<3;++n)music[slot].buffers[n].data_pcm16=streamData+(slot*3+n)*StreamFrames*2;
    for(int n=0;n<8;++n) {
        char path[96];std::snprintf(path,sizeof(path),"romfs:/%s.wav",CueNames[n]);
        FILE* file=std::fopen(path,"rb");
        if(!file) {shutdown();return false;}
        PcmHeader pcm;
        if(!readHeader(file,pcm) || pcm.channels!=1 || pcm.frames>512*1024) {
            std::fclose(file);shutdown();return false;
        }
        Clip& clip=clips[n];clip.samples=pcm.frames;
        clip.data=(int16_t*)linearAlloc(pcm.frames*2);
        if(!clip.data || std::fread(clip.data,2,pcm.frames,file)!=pcm.frames) {
            std::fclose(file);shutdown();return false;
        }
        std::fclose(file);
        clip.wave.data_pcm16=clip.data;clip.wave.nsamples=clip.samples;clip.wave.looping=false;
        DSP_FlushDataCache(clip.data,pcm.frames*2);
        ndspChnSetInterp(n+2,NDSP_INTERP_LINEAR);ndspChnSetRate(n+2,pcm.rate);
        ndspChnSetFormat(n+2,NDSP_FORMAT_MONO_PCM16);
        // Conservative harness mix. Jump's muted original table entry is
        // deliberately overridden for this primary-sample fallback.
        constexpr float gains[]={80.f/127,0.65f,100.f/127,110.f/127,100.f/127,100.f/127,45.f/127,45.f/127};
        float mix[12]{};mix[0]=mix[1]=gains[n]*.65f;ndspChnSetMix(n+2,mix);
    }
    musicWorkerRun.store(true,std::memory_order_release);
    musicThread=threadCreate([](void* owner){
        auto* audio=static_cast<Audio*>(owner);
        while(audio->musicWorkerRun.load(std::memory_order_acquire)){
            LightEvent_Wait(&audio->musicWake);
            if(audio->musicWorkerRun.load(std::memory_order_acquire))audio->serviceMusic();
        }
    },this,16*1024,0x28,-2,false);
    if(!musicThread){shutdown();return false;}
    // NDSP's sound thread must never block on file IO or a main-thread lock.
    // Its callback only wakes the separate, bounded PCM refill worker.
    ndspSetCallback([](void* owner){LightEvent_Signal(&static_cast<Audio*>(owner)->musicWake);},this);
    return true;
}
void Audio::play(const Game& game) {
    if(!ready || !game.progress.sound)return;
    if(game.saveObjects)for(auto id:game.saveObjects->sounds)if(id==431) {
        auto& clip=clips[5];ndspChnWaveBufClear(7);clip.wave.status=NDSP_WBUF_FREE;ndspChnWaveBufAdd(7,&clip.wave);
    }
    for(const auto& request:game.fieldNpcSounds()) {
        for(int index=0;index<8;++index)if(request.key==OriginalCueNames[index]) {
            auto& clip=clips[index];ndspChnWaveBufClear(index+2);clip.wave.status=NDSP_WBUF_FREE;ndspChnWaveBufAdd(index+2,&clip.wave);break;
        }
    }
    int cue=static_cast<int>(audioCue(game));
    if(cue<0 || cue>=5)return;
    Clip& clip=clips[cue];
    ndspChnWaveBufClear(cue+2);clip.wave.status=NDSP_WBUF_FREE;
    ndspChnWaveBufAdd(cue+2,&clip.wave);
}
void Audio::stopMusic(unsigned slot) {
    ndspChnWaveBufClear(MusicChannels[slot]);
    if(keyItemSlot==int(slot)){keyItemSlot=-1;keyItemEnvelope.backendVacant();}
    if(keyItemTailSlot==int(slot)){keyItemTailSlot=-1;keyItemTail.clear();}
    if(itemGetFieldSlot==int(slot)){itemGetFieldSlot=itemGetFieldTrack=-1;itemGetFieldEnvelope.backendVacant();}
    auto& stream=music[slot];
    if(stream.file){std::fclose(stream.file);stream.file=nullptr;}
    stream.track=-1;stream.ended=false;
    for(auto& buffer:stream.buffers)buffer.status=NDSP_WBUF_FREE;
}
void Audio::musicMix(unsigned slot,uint8_t volume) {
    music[slot].nativeVolume=volume;
    const float gain=(music[slot].track==18 || itemGetFieldSlot==int(slot))?float(volume)*.007874015718698502f:.65f*float(volume)/127.f;
    float mix[12]{};mix[0]=mix[1]=fieldItemMuted?0:gain;
    ndspChnSetMix(MusicChannels[slot],mix);
}
void Audio::switchTrack(int track,bool preservePrevious) {
    MusicLock guard(musicMutex);
    if(preservePrevious){
        unsigned candidate=0;while(candidate<3 && music[candidate].file)++candidate;
        if(candidate==3)return;
        musicSlot=candidate;
    } else for(unsigned n=0;n<3;++n)stopMusic(n);
    startMusic(musicSlot,track);musicTrack=track;
}
void Audio::startMusic(unsigned slot,int track,uint8_t volume){
    auto& stream=music[slot];
    stream.track=track;stream.position=0;stream.ended=false;stream.loop=false;
    const char* asset=musicAssetPath(track,switchSoundtrack);
    if(!asset)return;
    char path[128];stream.file=std::fopen(asset,"rb");
    if(!stream.file)return;
    PcmHeader pcm;
    if(!readHeader(stream.file,pcm)) {std::fclose(stream.file);stream.file=nullptr;return;}
    stream.channels=pcm.channels;stream.rate=pcm.rate;stream.loopStart=0;stream.loopEnd=pcm.frames;
    std::snprintf(path,sizeof(path),"%s.loop",asset);FILE* loop=std::fopen(path,"rb");
    if(loop) {
        uint32_t points[2];
        if(std::fread(points,4,2,loop)==2 && points[0]<points[1] && points[1]<=stream.loopEnd) {
            stream.loopStart=points[0];stream.loopEnd=points[1];stream.loop=true;
        }
        std::fclose(loop);
    }
    const unsigned channel=MusicChannels[slot];
    ndspChnSetInterp(channel,NDSP_INTERP_LINEAR);ndspChnSetRate(channel,stream.rate);
    ndspChnSetFormat(channel,stream.channels==2?NDSP_FORMAT_STEREO_PCM16:NDSP_FORMAT_MONO_PCM16);
    musicMix(slot,volume);
    for(auto& buffer:stream.buffers){buffer.status=NDSP_WBUF_FREE;fill(stream,slot,buffer);}
}
void Audio::fill(MusicStream& stream,unsigned slot,ndspWaveBuf& buffer) {
    if(!stream.file || stream.ended)return;
    uint32_t filled=0;
    while(filled<StreamFrames) {
        if(stream.position>=stream.loopEnd) {
            if(!stream.loop) {stream.ended=true;break;}
            stream.position=stream.loopStart;
            if(std::fseek(stream.file,44+uint64_t(stream.position)*stream.channels*2,SEEK_SET)) {stream.ended=true;break;}
        }
        uint32_t frames=std::min(StreamFrames-filled,stream.loopEnd-stream.position);
        size_t read=std::fread(buffer.data_pcm16+filled*stream.channels,stream.channels*2,frames,stream.file);
        filled+=read;stream.position+=read;
        if(read!=frames) {stream.ended=true;break;}
    }
    if(!filled)return;
    buffer.nsamples=filled;buffer.looping=false;
    DSP_FlushDataCache(buffer.data_pcm16,filled*stream.channels*2);ndspChnWaveBufAdd(MusicChannels[slot],&buffer);
}
void Audio::serviceMusic(){MusicLock guard(musicMutex);if(!ready || !streamData)return;for(unsigned slot=0;slot<3;++slot)for(auto& buffer:music[slot].buffers)if(buffer.status==NDSP_WBUF_DONE)fill(music[slot],slot,buffer);retireKeyItemFanfare();}
void Audio::update(Scene scene,bool enabled,bool harborDanger,int professorMusic,uint32_t room,bool thinking) {
    if(!ready)return;
    int wanted=soundtrackTrack(scene,enabled,harborDanger,professorMusic,room,thinking,switchSoundtrack);
    bool retainedItem=false;
    {MusicLock lock(musicMutex);retainedItem=(scene==Scene::Explore || scene==Scene::Dialogue) &&
        (keyItemSlot>=0 || keyItemTailSlot>=0 || itemGetFieldSlot>=0);}
    if(wanted!=musicTrack && !retainedItem)switchTrack(wanted);
    if(!enabled)for(int n=0;n<8;++n)ndspChnWaveBufClear(n+2);
    if(!musicThread)serviceMusic();
}
void Audio::updateFrontend(int track,bool enabled) {
    if(!ready)return;
    if(!enabled)track=-1;
    if(track!=musicTrack)switchTrack(track);
    if(!enabled)for(int n=0;n<8;++n)ndspChnWaveBufClear(n+2);
    if(!musicThread)serviceMusic();
}
void Audio::frontendMusic(const std::array<int,3>& tracks,const std::array<uint8_t,3>& volumes){
    // Keep each admitted PCM owner at its fixed address until its own source
    // publication retires it. No queued ndspWaveBuf is copied or reassigned.
    {MusicLock guard(musicMutex);
    for(unsigned n=0;n<3;++n)if(music[n].file && std::find(tracks.begin(),tracks.end(),music[n].track)==tracks.end())stopMusic(n);
    for(unsigned wanted=0;wanted<3;++wanted){
        if(tracks[wanted]<0)continue;
        unsigned slot=0;while(slot<3 && !(music[slot].file && music[slot].track==tracks[wanted]))++slot;
        if(slot==3){slot=0;while(slot<3 && music[slot].file)++slot;if(slot==3)continue;startMusic(slot,tracks[wanted],volumes[wanted]);}
        musicMix(slot,volumes[wanted]);if(wanted==0){musicSlot=slot;musicTrack=tracks[0];}
    }
    }
    if(!musicThread)serviceMusic();
}
void Audio::updateFrontend(const OpeningDemo& demo,bool enabled){
    if(!ready)return;
    if(!enabled){updateFrontend(-1,false);return;}
    const auto snapshot=demo.bgm.snapshot();
    frontendMusic({{snapshot.primaryTrack,snapshot.tailActive?snapshot.tailTrack:-1,-1}},{{snapshot.primaryVolume,snapshot.tailVolume,0}});
}
void Audio::updateFrontend(const OpeningTitle& title,bool enabled){
    if(!ready)return;
    if(!enabled){updateFrontend(-1,false);return;}
    const auto snapshot=title.bgm.snapshot();frontendMusic(snapshot.tracks,snapshot.volumes);
}
void Audio::shutdown() {
    fieldSfxBackend.clear();fieldMarioOwner=nullptr;
    if(ready)ndspSetCallback(nullptr,nullptr);
    musicWorkerRun.store(false,std::memory_order_release);
    if(musicThread){LightEvent_Signal(&musicWake);threadJoin(musicThread,U64_MAX);threadFree(musicThread);musicThread=nullptr;}
    openingHousePcm.shutdown();
    if(ready)ndspChnWaveBufClear(19);
    if(openingMapSound.data)linearFree(openingMapSound.data);
    openingMapSound={};openingMapSoundRate=0;openingMapSoundAttempted=false;
    if(ready) {
        for(int n=0;n<=9;++n)ndspChnWaveBufClear(n);
        ndspChnWaveBufClear(18);
        ndspSetCallback(nullptr,nullptr);
        {MusicLock guard(musicMutex);for(unsigned slot=0;slot<3;++slot)stopMusic(slot);ready=false;}
        ndspExit();
    }
    for(auto& stream:music)if(stream.file){std::fclose(stream.file);stream.file=nullptr;}
    if(streamData) {linearFree(streamData);streamData=nullptr;}
    for(auto& clip:clips) {
        if(clip.data)linearFree(clip.data);
        clip={};
    }
    keyItemSlot=keyItemTailSlot=itemGetFieldSlot=itemGetFieldTrack=-1;keyItemEnvelope={};keyItemTail={};itemGetFieldEnvelope={};keyItemResourceAttempted=keyItemResourceReady=false;
    musicTrack=-1;
}
}

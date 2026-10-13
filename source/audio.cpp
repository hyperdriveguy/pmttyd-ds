#include "platform.hpp"
#include "audio_policy.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t StreamFrames=2048;
constexpr const char* CueNames[]={"confirm","jump","hit","success","guard","save-block","goomba-step-left","goomba-step-right"};
constexpr const char* OriginalCueNames[]={"SFX_CURSOR_OK1","SFX_MARIO_JUMP1","SFX_MARIO_DAMAGE1","SFX_AC_OK1","SFX_MARIO_GUARD1","SFX_MOBJ_BLOCK_SAVE1","SFX_FLD_NPC_KURI_MOVE1L","SFX_FLD_NPC_KURI_MOVE1R"};
bool readHeader(FILE* file,PcmHeader& pcm) {
    unsigned char bytes[44]{};
    return std::fread(bytes,1,44,file)==44 && pcmHeader(bytes,pcm);
}
}
bool Audio::init() {
    if(R_FAILED(ndspInit())) return false;
    ready=true;ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    streamData=(int16_t*)linearAlloc(StreamFrames*2*2*3*2);
    if(!streamData) {shutdown();return false;}
    for(unsigned slot=0;slot<2;++slot)for(int n=0;n<3;++n)music[slot].buffers[n].data_pcm16=streamData+(slot*3+n)*StreamFrames*2;
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
    ndspChnWaveBufClear(slot?0:1);
    auto& stream=music[slot];
    if(stream.file){std::fclose(stream.file);stream.file=nullptr;}
    stream.track=-1;stream.ended=false;
    for(auto& buffer:stream.buffers)buffer.status=NDSP_WBUF_FREE;
}
void Audio::musicMix(unsigned slot,uint8_t volume) {
    float mix[12]{};mix[0]=mix[1]=.65f*float(volume)/127.f;
    ndspChnSetMix(slot?0:1,mix);
}
void Audio::switchTrack(int track,bool preservePrevious) {
    if(preservePrevious){musicSlot=1-musicSlot;stopMusic(musicSlot);}
    else {stopMusic(0);stopMusic(1);}
    auto& stream=music[musicSlot];
    musicTrack=stream.track=track;stream.position=0;stream.ended=false;stream.loop=false;
    const char* names[]={"rogueport","battle","victory","harbor-danger","title","demo-book","demo-market","opening-house","opening-voyage","opening-sailing","professor-music","underground-music","harbor-tutorial-music"};
    if(track<0 || track>=int(sizeof(names)/sizeof(names[0])))return;
    char path[96];std::snprintf(path,sizeof(path),"romfs:/%s.wav",names[track]);stream.file=std::fopen(path,"rb");
    if(!stream.file)return;
    PcmHeader pcm;
    if(!readHeader(stream.file,pcm)) {std::fclose(stream.file);stream.file=nullptr;return;}
    stream.channels=pcm.channels;stream.rate=pcm.rate;stream.loopStart=0;stream.loopEnd=pcm.frames;
    std::snprintf(path,sizeof(path),"romfs:/%s.wav.loop",names[track]);FILE* loop=std::fopen(path,"rb");
    if(loop) {
        uint32_t points[2];
        if(std::fread(points,4,2,loop)==2 && points[0]<points[1] && points[1]<=stream.loopEnd) {
            stream.loopStart=points[0];stream.loopEnd=points[1];stream.loop=true;
        }
        std::fclose(loop);
    }
    const unsigned channel=musicSlot?0:1;
    ndspChnSetInterp(channel,NDSP_INTERP_LINEAR);ndspChnSetRate(channel,stream.rate);
    ndspChnSetFormat(channel,stream.channels==2?NDSP_FORMAT_STEREO_PCM16:NDSP_FORMAT_MONO_PCM16);
    musicMix(musicSlot,127);
    for(auto& buffer:stream.buffers){buffer.status=NDSP_WBUF_FREE;fill(stream,musicSlot,buffer);}
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
    DSP_FlushDataCache(buffer.data_pcm16,filled*stream.channels*2);ndspChnWaveBufAdd(slot?0:1,&buffer);
}
void Audio::serviceMusic(){for(unsigned slot=0;slot<2;++slot)for(auto& buffer:music[slot].buffers)if(buffer.status==NDSP_WBUF_DONE)fill(music[slot],slot,buffer);}
void Audio::update(Scene scene,bool enabled,bool harborDanger,int professorMusic,uint32_t room) {
    if(!ready)return;
    int wanted=enabled && harborDanger && (scene==Scene::Explore || scene==Scene::Dialogue)?3:audioTrack(scene,enabled);
    if(enabled && room==5 && (scene==Scene::Explore || scene==Scene::Dialogue))wanted=11;
    if(enabled && professorMusic)wanted=professorMusic<0?-1:10;
    if(wanted!=musicTrack)switchTrack(wanted);
    if(!enabled)for(int n=0;n<8;++n)ndspChnWaveBufClear(n+2);
    serviceMusic();
}
void Audio::updateFrontend(int track,bool enabled) {
    if(!ready)return;
    if(!enabled)track=-1;
    if(track!=musicTrack)switchTrack(track);
    if(!enabled)for(int n=0;n<8;++n)ndspChnWaveBufClear(n+2);
    serviceMusic();
}
void Audio::updateFrontend(const OpeningDemo& demo,bool enabled) {
    if(!ready)return;
    if(!enabled){updateFrontend(-1,false);return;}
    const auto snapshot=demo.bgm.snapshot();
    if(snapshot.primaryTrack!=musicTrack){
        const bool retain=snapshot.tailActive && music[musicSlot].file && musicTrack==snapshot.tailTrack;
        switchTrack(snapshot.primaryTrack,retain);
    }
    musicMix(musicSlot,snapshot.primaryVolume);
    const unsigned tail=1-musicSlot;
    if(snapshot.tailActive && music[tail].track==snapshot.tailTrack)musicMix(tail,snapshot.tailVolume);
    else if(music[tail].file)stopMusic(tail);
    serviceMusic();
}
void Audio::shutdown() {
    if(ready) {
        for(int n=0;n<=9;++n)ndspChnWaveBufClear(n);
        ndspExit();ready=false;
    }
    for(auto& stream:music)if(stream.file){std::fclose(stream.file);stream.file=nullptr;}
    if(streamData) {linearFree(streamData);streamData=nullptr;}
    for(auto& clip:clips) {
        if(clip.data)linearFree(clip.data);
        clip={};
    }
    musicTrack=-1;
}
}

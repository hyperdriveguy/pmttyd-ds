#include "opening_house_pcm.hpp"
#include "audio_policy.hpp"
#include <cstdio>
namespace ttyd {
bool OpeningHousePcmPlayer::load(){
    if(ready)return true;
    auto fail=[&](){for(auto& clip:clips){if(clip.data)linearFree(clip.data);clip={};}return false;};
    const char* paths[]={"flight","postbox","door-open","door-shut"};
    for(unsigned i=0;i<4;++i){const auto* metadata=openingHouseCue(i);if(!metadata)return fail();
        char path[96];std::snprintf(path,sizeof(path),"romfs:/opening-house-%s.wav",paths[i]);FILE* f=std::fopen(path,"rb");
        if(!f)return fail();
        unsigned char header[44]{};PcmHeader pcm;
        bool ok=std::fread(header,44,1,f)==1 && pcmHeader(header,pcm) && pcm.channels==1 && pcm.rate==metadata->rate && pcm.frames==metadata->frames && pcm.frames<=512*1024;
        auto& clip=clips[i];if(ok){clip.data=static_cast<int16_t*>(linearAlloc(pcm.frames*2));ok=clip.data && std::fread(clip.data,2,pcm.frames,f)==pcm.frames;}
        std::fclose(f);if(!ok)return fail();clip.rate=pcm.rate;clip.frames=pcm.frames;DSP_FlushDataCache(clip.data,pcm.frames*2);
    }
    ready=true;return true;
}
void OpeningHousePcmPlayer::clear(){
    for(unsigned i=0;i<voices.size();++i)if(queuedVoices&(1u<<i)){
        ndspChnWaveBufClear(10+i);voices[i]={};
    }
    queuedVoices=0;
}
void OpeningHousePcmPlayer::shutdown(){clear();for(auto& clip:clips){if(clip.data)linearFree(clip.data);clip={};}voices={};ready=false;owner=nullptr;cursor=0;}
bool OpeningHousePcmPlayer::play(const OpeningHouseAudioRequest& request){
    if(request.operation!=OpeningHouseAudioRequest::Operation::Sfx3D || request.cue<0 || request.cue>=4 || request.control!=0 || !request.positionResolved)return false;
    if(!load())return false;
    const auto* meta=openingHouseCue(unsigned(request.cue));if(!meta)return false;
    for(unsigned i=0;i<voices.size();++i)if(voices[i].status==NDSP_WBUF_FREE || voices[i].status==NDSP_WBUF_DONE){
        auto& voice=voices[i];voice={};voice.data_pcm16=clips[request.cue].data;voice.nsamples=clips[request.cue].frames;voice.looping=false;
        const unsigned channel=10+i;ndspChnWaveBufClear(channel);ndspChnSetInterp(channel,NDSP_INTERP_LINEAR);ndspChnSetRate(channel,clips[request.cue].rate);ndspChnSetFormat(channel,NDSP_FORMAT_MONO_PCM16);
        // Preserve source table volume/pan. Full calc3D listener/distance/surround,
        // MusyX pitch/envelope/modulation/priority and ENV RNG remain unbound.
        const float volume=float(meta->parameters[0])/127.f,pan=float(meta->parameters[1])/127.f;
        float mix[12]{};mix[0]=volume*(pan<=.5f?1.f:2*(1-pan));mix[1]=volume*(pan>=.5f?1.f:2*pan);
        ndspChnSetMix(channel,mix);ndspChnWaveBufAdd(channel,&voice);queuedVoices|=uint8_t(1u<<i);++queued;return true;
    }
    return false; // Explicit bounded native voice admission failure, never an acknowledgement.
}
bool OpeningHousePcmPlayer::update(const OpeningHouseSounds* sounds,bool enabled){
    if(!sounds){clear();owner=nullptr;cursor=0;return true;}
    if(owner!=sounds || epoch!=sounds->beginTick || cursor>sounds->requests.size()){clear();owner=sounds;epoch=sounds->beginTick;cursor=0;}
    if(!enabled){clear();cursor=sounds->requests.size();return true;}
    bool ok=true;
    for(;cursor<sounds->requests.size();++cursor){const auto& request=sounds->requests[cursor];
        if(request.operation==OpeningHouseAudioRequest::Operation::Sfx3D){if(!play(request)){++failures;ok=false;}}
        else ++unboundRequests; // Retained in source owner; no successful ENV/BGM-scope consumer asserted.
    }
    return ok;
}
}

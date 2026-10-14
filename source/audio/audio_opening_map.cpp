#include "platform.hpp"
#include "audio_policy.hpp"
namespace ttyd {
void Audio::updateFrontend(const OpeningSequence& opening,bool enabled){
    if(!ready)return;
    updateOpeningMapTurn(opening,enabled);
    if(!enabled){updateFrontend(-1,false);return;}
    if(!opening.openingMusicOwned()){updateFrontend(-1,true);return;}
    const auto state=opening.mapBgm.snapshot();frontendMusic(state.tracks,state.volumes);
}
void Audio::updateOpeningMapTurn(const OpeningSequence& opening,bool enabled){
    if(!enabled || !opening.mapTransition.entryRequested){
        ndspChnWaveBufClear(19);openingMapSoundAttempted=false;return;
    }
    if(!opening.mapTransition.entryAdmitted || openingMapSoundAttempted)return;
    openingMapSoundAttempted=true;
    if(!openingMapSound.data){
        FILE* f=std::fopen("romfs:/opening-map-turn.wav","rb");if(!f)return;
        unsigned char header[44]{};PcmHeader pcm;
        bool ok=std::fread(header,44,1,f)==1 && pcmHeader(header,pcm) && pcm.channels==1 && pcm.frames<=512*1024;
        if(ok){openingMapSound.data=static_cast<int16_t*>(linearAlloc(pcm.frames*2));ok=openingMapSound.data && std::fread(openingMapSound.data,2,pcm.frames,f)==pcm.frames;}
        std::fclose(f);
        if(!ok){if(openingMapSound.data)linearFree(openingMapSound.data);openingMapSound={};return;}
        openingMapSound.samples=pcm.frames;openingMapSoundRate=pcm.rate;DSP_FlushDataCache(openingMapSound.data,pcm.frames*2);
    }
    // Source869 volume100/pan64. Dedicated channel19 avoids three BGM streams,
    // eight field effects and eight house voices; no original macro parity claim.
    ndspChnWaveBufClear(19);ndspChnSetInterp(19,NDSP_INTERP_LINEAR);
    ndspChnSetRate(19,openingMapSoundRate);ndspChnSetFormat(19,NDSP_FORMAT_MONO_PCM16);
    const float volume=100/127.f,pan=64/127.f;float mix[12]{};
    mix[0]=volume*2*(1-pan);mix[1]=volume;ndspChnSetMix(19,mix);
    auto& wave=openingMapSound.wave;wave={};wave.data_pcm16=openingMapSound.data;wave.nsamples=openingMapSound.samples;
    ndspChnWaveBufAdd(19,&wave);
}
}

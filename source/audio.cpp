#include "platform.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t StreamFrames=2048;
uint32_t le32(const unsigned char* p) { return p[0]|uint32_t(p[1])<<8|uint32_t(p[2])<<16|uint32_t(p[3])<<24; }
}
bool Audio::init() {
    if(R_FAILED(ndspInit())) return false;
    ready=true;ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    streamData=(int16_t*)linearAlloc(StreamFrames*2*2*3);
    if(!streamData) {shutdown();return false;}
    for(int n=0;n<3;++n) stream[n].data_pcm16=streamData+n*StreamFrames*2;
    return true;
}
void Audio::play(Sound) {
    // Preserve original MusyX banks offline. Event macro playback is pending;
    // synthetic replacements are intentionally absent from this build.
}
void Audio::switchTrack(int track) {
    ndspChnWaveBufClear(1);
    if(music) {std::fclose(music);music=nullptr;}
    musicTrack=track;musicPosition=0;
    if(track<0)return;
    const char* names[]={"rogueport","battle","victory"};
    char path[96];std::snprintf(path,sizeof(path),"romfs:/%s.wav",names[track]);music=std::fopen(path,"rb");
    if(!music)return;
    unsigned char header[44]{};
    if(std::fread(header,1,44,music)!=44 || std::memcmp(header,"RIFF",4) || std::memcmp(header+8,"WAVEfmt ",8) || std::memcmp(header+36,"data",4)) {std::fclose(music);music=nullptr;return;}
    musicChannels=header[22];musicRate=le32(header+24);loopStart=0;loopEnd=le32(header+40)/(musicChannels*2);
    if(musicChannels<1 || musicChannels>2 || !loopEnd || musicRate<8000 || musicRate>48000) {std::fclose(music);music=nullptr;return;}
    std::snprintf(path,sizeof(path),"romfs:/%s.wav.loop",names[track]);FILE* loop=std::fopen(path,"rb");
    if(loop) {uint32_t points[2];if(std::fread(points,4,2,loop)==2 && points[0]<points[1] && points[1]<=loopEnd) {loopStart=points[0];loopEnd=points[1];}std::fclose(loop);}
    ndspChnSetInterp(1,NDSP_INTERP_LINEAR);ndspChnSetRate(1,musicRate);ndspChnSetFormat(1,musicChannels==2?NDSP_FORMAT_STEREO_PCM16:NDSP_FORMAT_MONO_PCM16);
    float mix[12]{};mix[0]=mix[1]=.65f;ndspChnSetMix(1,mix);
    for(auto& b:stream) {b.status=NDSP_WBUF_FREE;fill(b);}
}
void Audio::fill(ndspWaveBuf& buffer) {
    if(!music)return;
    uint32_t filled=0;
    while(filled<StreamFrames) {
        if(musicPosition>=loopEnd) {musicPosition=loopStart;std::fseek(music,44+musicPosition*musicChannels*2,SEEK_SET);}
        const uint32_t frames=std::min(StreamFrames-filled,loopEnd-musicPosition);
        size_t read=std::fread(buffer.data_pcm16+filled*musicChannels,musicChannels*2,frames,music);
        if(read!=frames) {std::memset(buffer.data_pcm16+filled*musicChannels,0,(StreamFrames-filled)*musicChannels*2);break;}
        filled+=frames;musicPosition+=frames;
    }
    buffer.nsamples=StreamFrames;buffer.looping=false;
    DSP_FlushDataCache(buffer.data_pcm16,StreamFrames*musicChannels*2);ndspChnWaveBufAdd(1,&buffer);
}
void Audio::update(Scene scene,bool enabled) {
    if(!ready)return;
    int wanted=enabled?(scene==Scene::Battle?1:scene==Scene::Victory?2:0):-1;
    if(wanted!=musicTrack)switchTrack(wanted);
    for(auto& b:stream) if(b.status==NDSP_WBUF_DONE)fill(b);
}
void Audio::shutdown() {
    if(ready) {ndspChnWaveBufClear(1);ndspExit();ready=false;}
    if(music) {std::fclose(music);music=nullptr;}
    if(streamData) {linearFree(streamData);streamData=nullptr;}
}
}

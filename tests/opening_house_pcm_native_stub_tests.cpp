#include "opening_house_pcm.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <unistd.h>

namespace {
std::array<unsigned,24> clears{};
std::array<ndspWaveBuf*,24> channels{};
std::array<float,24> rates{};
std::array<std::array<float,12>,24> mixes{};
unsigned allocations=0,frees=0,adds=0;
bool failAllocation=false;
std::array<ttyd::OpeningHouseCue,4> cues{};
unsigned clearCount(){unsigned count=0;for(auto n:clears)count+=n;return count;}
ttyd::OpeningHouseAudioRequest request(int cue=0){
    ttyd::OpeningHouseAudioRequest out;out.cue=cue;out.positionResolved=true;return out;
}
void writePcm(const char* name){
    unsigned char h[44]{};
    auto word=[&](unsigned offset,uint32_t value){for(unsigned i=0;i<4;++i)h[offset+i]=uint8_t(value>>(i*8));};
    std::memcpy(h,"RIFF",4);word(4,40);std::memcpy(h+8,"WAVEfmt ",8);word(16,16);
    h[20]=1;h[22]=1;word(24,16000);word(28,32000);h[32]=2;h[34]=16;
    std::memcpy(h+36,"data",4);word(40,4);
    FILE* file=std::fopen(name,"wb");assert(file);
    const int16_t samples[]={123,-456};assert(std::fwrite(h,1,44,file)==44);
    assert(std::fwrite(samples,2,2,file)==2);assert(!std::fclose(file));
}
}
void* linearAlloc(size_t size){if(failAllocation)return nullptr;++allocations;return std::malloc(size);}
void linearFree(void* data){++frees;std::free(data);}
void DSP_FlushDataCache(const void*,size_t){}
void ndspChnWaveBufClear(int channel){++clears.at(channel);if(channels[channel])channels[channel]->status=NDSP_WBUF_FREE;channels[channel]=nullptr;}
void ndspChnSetInterp(int,int){}
void ndspChnSetRate(int channel,float rate){rates.at(channel)=rate;}
void ndspChnSetFormat(int,int){}
void ndspChnSetMix(int channel,float* mix){std::memcpy(mixes.at(channel).data(),mix,12*sizeof(float));}
void ndspChnWaveBufAdd(int channel,ndspWaveBuf* wave){assert(!channels.at(channel));channels[channel]=wave;wave->status=NDSP_WBUF_QUEUED;++adds;}
namespace ttyd {const OpeningHouseCue* openingHouseCue(unsigned cue){return cue<4?&cues[cue]:nullptr;}}

int main(){
    const auto oldDirectory=std::filesystem::current_path();char path[]="/tmp/ttyd-house-pcm-XXXXXX";
    const char* temp=mkdtemp(path);assert(temp);std::filesystem::current_path(temp);std::filesystem::create_directory("romfs:");
    const char* names[]={"flight","postbox","door-open","door-shut"};
    for(unsigned i=0;i<4;++i){cues[i].rate=16000;cues[i].frames=2;cues[i].parameters[0]=100;cues[i].parameters[1]=64;
        writePcm((std::string("romfs:/opening-house-")+names[i]+".wav").c_str());}
    ttyd::OpeningHousePcmPlayer player;ttyd::OpeningHouseSounds sounds;
    // Empty/invalid/unbound requests never acquire a channel or clear idle ones.
    assert(player.update(nullptr,true));assert(player.update(&sounds,true));
    sounds.requests.push_back(request(-1));assert(!player.update(&sounds,true));assert(player.failures==1);
    auto unbound=request();unbound.operation=ttyd::OpeningHouseAudioRequest::Operation::EnvironmentOn;
    sounds.requests.push_back(unbound);assert(player.update(&sounds,true));assert(player.unboundRequests==1);
    assert(clearCount()==0 && adds==0);
    // Allocation failure remains a consumed request; subsequent requests retry.
    failAllocation=true;sounds.requests.push_back(request());assert(!player.update(&sounds,true));
    assert(player.failures==2 && adds==0 && clearCount()==0);failAllocation=false;
    sounds.requests.push_back(request());assert(player.update(&sounds,true));assert(allocations==4 && adds==1);
    assert(channels[10]->nsamples==2 && channels[10]->data_pcm16[0]==123 && channels[10]->data_pcm16[1]==-456);
    assert(!channels[10]->looping && rates[10]==16000);
    assert(mixes[10][0]==100/127.f*(2*(1-64/127.f)) && mixes[10][1]==100/127.f);
    // DONE voices retain ownership until reset/reuse; all eight voices still admit.
    channels[10]->status=NDSP_WBUF_DONE;sounds.requests.push_back(request(1));
    assert(player.update(&sounds,true));assert(adds==2 && allocations==4);
    for(unsigned i=1;i<8;++i)sounds.requests.push_back(request(i%4));
    assert(player.update(&sounds,true));assert(adds==9);
    sounds.requests.push_back(request());assert(!player.update(&sounds,true));assert(player.failures==3);
    const auto beforeMute=clearCount();assert(player.update(&sounds,false));assert(clearCount()==beforeMute+8);
    for(unsigned i=0;i<100;++i)assert(player.update(&sounds,false));
    assert(clearCount()==beforeMute+8);
    // Muted requests stay consumed, while a fresh request reuses retained PCM.
    sounds.requests.push_back(request());assert(player.update(&sounds,false));assert(player.update(&sounds,true));assert(adds==9);
    sounds.requests.push_back(request());assert(player.update(&sounds,true));assert(adds==10 && allocations==4);
    const auto beforeExit=clearCount();assert(player.update(nullptr,true));assert(clearCount()==beforeExit+1);
    for(unsigned i=0;i<100;++i)assert(player.update(nullptr,true));
    assert(clearCount()==beforeExit+1 && frees==0);
    // Source restart and cursor rewind still retire voices before replaying.
    sounds.requests={request()};assert(player.update(&sounds,true));const auto beforeRestart=clearCount();
    ++sounds.beginTick;assert(player.update(&sounds,true));assert(clearCount()==beforeRestart+2);
    sounds.requests.clear();const auto beforeRewind=clearCount();assert(player.update(&sounds,true));assert(clearCount()==beforeRewind+1);
    sounds.requests.push_back(request());assert(player.update(&sounds,true));
    const auto beforeShutdown=clearCount();player.shutdown();assert(clearCount()==beforeShutdown+1 && frees==4);
    player.shutdown();assert(clearCount()==beforeShutdown+1 && frees==4);
    std::filesystem::current_path(oldDirectory);std::filesystem::remove_all(temp);
}

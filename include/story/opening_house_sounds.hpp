#pragma once
#include <array>
#include <cstdint>
#include <vector>
namespace ttyd {
struct OpeningHouseCue {uint32_t tableIndex=0,flags=0,fxId=0,objectId=0,sampleId=0,rate=0,frames=0;std::array<uint8_t,8> parameters{};std::array<char,32> name{};};
static_assert(sizeof(OpeningHouseCue)==68,"Original sound metadata layout");
bool loadOpeningHouseSoundAssets(const char* root);
const OpeningHouseCue* openingHouseCue(unsigned cue);
struct OpeningHouseAudioRequest {
    enum class Operation {Sfx3D,EnvironmentOn,EnvironmentOff,EnvironmentLpf,BgmScope};
    Operation operation=Operation::Sfx3D;
    uint32_t command=0,control=0;uint64_t tick=0;
    int32_t cue=-1,argument=0;
    const char* name=nullptr;
    std::array<float,3> position{};
    bool positionResolved=false;
};
class OpeningHouseSounds {
public:
    std::vector<OpeningHouseAudioRequest> requests;
    bool flightStarted=false,departurePublished=false,flightFinished=false;
    uint64_t beginTick=0;
    void begin(uint64_t tick);
    void startFlight(uint64_t tick);
    void depart(){departurePublished=true;}
    void advance(uint64_t tick,const std::array<float,3>& sourceNpcPosition);
    void postbox(uint64_t tick,uint32_t command,float parameter);
    void door(uint64_t tick,uint32_t command,float parameter,bool open);
    void interior(uint64_t tick);
private:
    enum class Phase {Delay,Initial,Holding,Final,Complete};
    Phase phase=Phase::Delay;unsigned count=0;uint64_t deadline=0;
    void sfx(unsigned,uint32_t,uint64_t,const std::array<float,3>&,bool resolved);
};
}

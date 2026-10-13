#pragma once
#include "message_playback.hpp"
#include "field_scripted_jump.hpp"
#include <array>
#include <memory>
#include <vector>
namespace ttyd {
bool loadHarborEncounterAssets(const char* root);
struct HarborEncounterActor {
    std::array<float,3> position{};float yaw=90;uint32_t flags=0;
    bool moving=false;std::string pose="M_S_1";uint64_t poseAt=0;
};
struct HarborEncounterCamera {std::array<float,3> eye{},at{};int duration=0,curve=11;};
struct HarborEncounterRequest {uint32_t function=0,command=0;uint64_t time=0;std::vector<int32_t> operands;};
struct HarborEncounterRuntime;
class HarborEncounter {
public:
    bool triggered=false,finished=false,failed=false,admitted=false;
    bool controlEnabled=true,battleRequested=false;
    int keyDepth=0,battleSetup=-1,hp=10;int32_t story=4;
    uint32_t speaker=0,pendingService=0;uint64_t now=0;
    std::array<HarborEncounterActor,5> actors{}; // Mario,Goombella,Crump,A,B.
    std::array<float,3> speakerRoot{};
    MessagePlayback message;
    std::vector<std::string> messageOrder;
    std::vector<HarborEncounterCamera> cameras;
    std::vector<HarborEncounterRequest> requests;
    bool begin(uint64_t time,std::array<float,3> mario,int hp,bool resume);
    void tick(uint64_t time,uint32_t keys,std::optional<float> floor={});
    bool running() const{return triggered && !finished;}
    bool locked() const{return running() && (keyDepth!=0 || !controlEnabled);}
private:
    std::shared_ptr<HarborEncounterRuntime> runtime_;
};
}

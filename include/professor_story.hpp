#pragma once
#include "message_playback.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
bool loadProfessorStoryAssets(const char* root);
struct ProfessorSceneActor {
    std::array<float,3> position{};
    float yaw=0;std::string pose="S_1";
    bool hidden=false,moving=false;
    uint64_t poseAt=0;
};
struct ProfessorCameraRequest {
    std::array<float,3> eye{},at{};int duration=0,curve=11;bool off=false;
};
struct ProfessorServiceRequest {uint32_t function=0,command=0;uint64_t time=0;std::vector<int32_t> rawOperands;};
struct ProfessorRuntime;
// Scoped bridge around the recovered event interpreter, not a flattened story.
// All actor coordinates and request operands retain original engine units.
class ProfessorStory {
public:
    bool triggered=false,finished=false,failed=false,keysEnabled=true,temporaryProfessor=false;
    int32_t story=8;unsigned speaker=0;uint64_t now=0;
    int music=0;
    std::array<float,3> itemPosition{};
    float itemScale=1;bool itemVisible=false;
    std::array<ProfessorSceneActor,3> actors{}; // Mario,Goombella,Frankly.
    std::array<float,3> speakerRoot{};
    MessagePlayback message;
    std::vector<ProfessorCameraRequest> cameras;
    std::vector<ProfessorServiceRequest> presentationRequests;
    std::vector<std::string> messageOrder;
    bool begin(uint64_t milliseconds,std::array<float,3> mario,std::array<float,3> goombella);
    void tick(uint64_t milliseconds,uint32_t keys,bool doorBusy);
    bool locked() const{return triggered && !finished;}
private:
    std::shared_ptr<ProfessorRuntime> runtime_;
};
}

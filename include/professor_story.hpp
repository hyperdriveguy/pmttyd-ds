#pragma once
#include "message_playback.hpp"
#include "message_selection.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
bool loadProfessorStoryAssets(const char* root);
const std::vector<uint32_t>& professorFenceCollisionNodes();
struct ProfessorSceneActor {
    std::array<float,3> position{};
    float yaw=0;std::string pose="S_1";
    bool hidden=false,moving=false;
    bool jumping=false;
    uint32_t flags=0;
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
    bool fenceStarted=false,fenceHidden=false,fenceVisible=false,fenceCollisionOff=false;
    bool captureRequested=false;
    mutable bool captureReady=false; // Offscreen backend publishes the previous-frame rectangle.
    mutable std::array<int32_t,4> captureBounds{};
    std::string fencePose="Z_1";
    uint64_t fencePoseAt=0;
    uint32_t pendingService=0;
    bool goombellaNpc=false;
    bool professorDeparted=false;
    int battleSetup=-1;
    MessageSelection selection;
    std::vector<std::string> selectionOrder;
    std::vector<int> selectionResults;
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
    bool begin(uint64_t milliseconds,std::array<float,3> mario,std::array<float,3> goombella,bool departed=false);
    bool beginFence(uint64_t milliseconds,std::array<float,3> mario,std::array<float,3> goombella);
    bool beginLecture(uint64_t milliseconds,std::array<float,3> mario,std::array<float,3> goombella,bool departed=false);
    void restoreProfessor(bool departed);
    void tick(uint64_t milliseconds,uint32_t keys,bool doorBusy,const MessageSelectionInput& selectionInput={});
    bool running() const{return triggered && !finished;}
    bool locked() const{return running() && !(fenceStarted && story>=10 && keysEnabled);}
private:
    std::shared_ptr<ProfessorRuntime> runtime_;
};
}

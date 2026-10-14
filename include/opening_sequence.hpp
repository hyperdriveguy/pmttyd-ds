#pragma once
#include "message_assets.hpp"
#include "message_playback.hpp"
#include "opening_house_scene.hpp"
#include "opening_heading.hpp"
#include "opening_heading_display.hpp"
#include "opening_wake.hpp"
#include "camera_road_assets.hpp"
#include "opening_harbor_scene.hpp"
#include "opening_voyage_scene.hpp"
namespace ttyd {
enum class OpeningPart { House, Letter, HouseReturn, MapDisplay, Voyage, Prologue, Wake, Arrival, Complete };
// Sequencing adapter: retains original IDs/tokens and explicit source speakers.
// Scene choreography is provided separately; this does not run REL bytecode.
class OpeningSequence {
public:
    OpeningPart part=OpeningPart::House;
    MessagePlayback message;
    OpeningHouseScene house;
    OpeningHeading heading;
    OpeningHeadingDisplay headingDisplay;
    OpeningWake wake;
    CameraPoint wakeEye{},wakeAt{};
    mutable OpeningHarborScene harbor;
    mutable OpeningVoyageScene voyage;
    unsigned index=0;
    int32_t story=0;
    float globalWork8=0; // Explicit native fresh global work word, preserved until supplied callbacks write it.
    uint32_t speaker=0;
    bool failed=false;
    std::string failure;
    uint64_t now=0;
    void bindSceneServices(){sceneServicesBound=true;}
    void publishSceneAdmission(OpeningPart owner) const{if(owner==part)sceneReadyNotification=true;}
    void publishSceneFailure(const std::string& reason) const{sceneReadyNotification=false;sceneAdmitted=false;sceneFailure=reason;}
    bool load(const char* root);
    void begin(uint64_t time);
    void tick(uint64_t time,uint32_t keys);
    bool complete() const{return part==OpeningPart::Complete;}
    const char* map() const;
    int music() const;
private:
    CameraRoadAsset wakeCamera;
    MessageResource global;
    std::array<MessageResource,3> rooms;
    bool ready=false;
    const char* find(const char*) const;
    MessageFont font;
    bool waiting=false;
    uint64_t deadline=0,enteredAt=0;
    bool sceneServicesBound=false;
    mutable bool sceneAdmitted=false;
    mutable bool sceneReadyNotification=false;
    mutable std::string sceneFailure;
    bool enter(OpeningPart,uint64_t);
    bool print(const char*,uint32_t);
};
}

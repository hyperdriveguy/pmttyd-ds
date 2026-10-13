#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>
#include "message_assets.hpp"
#include "message_playback.hpp"
namespace ttyd {
struct FieldActorModel {std::string asset;float uvScale=1;};
struct FieldActorRecord {
    uint32_t key=0,room=0,model=0,interiorRequired=0,interiorForbidden=0,storyGate=0,kind=0; // 0 NPC, 1 map object.
    int32_t storyLow=0,storyHigh=0;
    std::array<float,3> position{}; // Original engine XYZ, scaled only by renderer.
    float yaw=0,scale=2; // Original model scalar; objects own distinct scales.
    std::string pose,talkPrefix; // Empty when the recovered talk event is unbound.
    bool visible(uint32_t currentRoom,std::optional<int32_t> story,uint32_t interior) const {
        return room==currentRoom && (interior&interiorRequired)==interiorRequired && !(interior&interiorForbidden) &&
            (!storyGate || (story && *story>=storyLow && *story<=storyHigh));
    }
};
std::string fieldActorTalkKey(const FieldActorRecord&,int32_t story);
// PMNPC001 is a local container. It describes recovered static init subsets;
// scripted actor movement/flags and the original NPC driver are separate owners.
class FieldActorAssets {
public:
    bool load(const char* path);
    bool read(FILE* file);
    const std::vector<FieldActorModel>& models() const {return models_;}
    const std::vector<FieldActorRecord>& actors() const {return actors_;}
private:
    std::vector<FieldActorModel> models_;
    std::vector<FieldActorRecord> actors_;
};
bool loadFieldActorCatalog(const char* root);
const FieldActorAssets& fieldActorCatalog();
// Source simple CASE_LESS talk family only. Native admission/proximity and
// rendering are adapters; complex event graphs need their own service owners.
class FieldActorTalk {
public:
    MessagePlayback message;
    FieldActorRecord actor;
    bool begin(const FieldActorRecord&,int32_t story,const MessageFont&,uint64_t now);
private:
    MessageAssets messages;
};
}

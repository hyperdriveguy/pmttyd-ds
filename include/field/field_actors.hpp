#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>
#include "trouble_quest_state.hpp"
#include "message_assets.hpp"
#include "message_playback.hpp"
namespace ttyd {
struct FieldActorModel {
    std::string asset,originalName;float uvScale=1;
    uint32_t drawMask=0; // Original animation resource+c4; 0 for legacy unbound metadata.
    uint32_t hitMode=0; // 0 unbound, 1 original mobjHitEntry mode0/default object scale.
    std::array<float,6> hitBounds{}; // Original model minXYZ,maxXYZ.
};
struct FieldActorTalkRule {
    uint32_t comparison=0; // 0 less, 1 equal, 2 greater/equal, 3 unconditional.
    int32_t story=0;
    std::string message;
};
struct FieldActorWander {
    uint32_t enabled=0,flags=0;float radius=0,speed=0,runThreshold=0;uint32_t stepPeriod=0;
    std::string idle,walk,run,soundLeft,soundRight;
};
// Partial knowledge of original saved switches. Unknown is distinct from zero.
struct FieldActorTalkState {
    std::optional<uint8_t> quest1420;
    std::optional<bool> flag5405,flag5406,flag5407,flag5376;
    TroubleQuestExtraState trouble;
};
// Live original inventory observations, not cached/persisted mushroom quantities.
struct FieldActorTalkInventory {std::optional<int32_t> item95,item107;};
struct FieldActorRecord {
    uint32_t key=0,room=0,model=0,interiorRequired=0,interiorForbidden=0,storyGate=0,kind=0; // 0 NPC, 1 map object.
    uint32_t systemForbidden=0;
    int32_t storyLow=0,storyHigh=0;
    std::array<float,3> position{}; // Original engine XYZ, scaled only by renderer.
    float yaw=0,scale=2; // Original model scalar; objects own distinct scales.
    std::string pose,talkPrefix,sourceName; // Empty when the recovered talk event is unbound.
    float npcWidth=0,npcHeight=0; // Source tribe dimensions, independent of draw scalar.
    std::string talkPose;
    uint32_t npcFlags=0; // Source Entry/setup/constructor flags; legacy rows leave metadata unbound.
    std::vector<FieldActorTalkRule> talkRules;
    uint32_t sourceTalkPresent=0;
    FieldActorWander wander;
    bool talkRulesBound=false; // PMNPC007 source-order rules; empty means event not bound.
    bool visible(uint32_t currentRoom,std::optional<int32_t> story,uint32_t interior,uint32_t system=0) const {
        return room==currentRoom && (interior&interiorRequired)==interiorRequired && !(interior&interiorForbidden) &&
            !(system&systemForbidden) && (!storyGate || (story && (storyGate==2?*story!=storyLow:*story>=storyLow && *story<=storyHigh)));
    }
};
std::string fieldActorTalkKey(const FieldActorRecord&,int32_t story,const FieldActorTalkState& state={},const FieldActorTalkInventory& items={});
bool fieldActorTalkAvailable(const FieldActorRecord&,int32_t story,const FieldActorTalkState& state={},const FieldActorTalkInventory& items={});
// PMNPC001..008 are local containers. They describe recovered static init subsets;
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
uint64_t fieldActorCatalogGeneration();
class NpcAngleTable;
const NpcAngleTable& fieldNpcAngles();
// Source simple CASE_LESS talk family only. Native admission/proximity and
// rendering are adapters; complex event graphs need their own service owners.
class FieldActorTalk {
public:
    MessagePlayback message;
    FieldActorRecord actor;
    bool begin(const FieldActorRecord&,int32_t story,const MessageFont&,uint64_t now,const FieldActorTalkState& state={},const FieldActorTalkInventory& items={});
    bool canPrint(const FieldActorRecord&,const char* id,const MessageFont&,uint64_t now) const;
    bool print(const FieldActorRecord&,const char* id,const MessageFont&,uint64_t now);
private:
    MessageAssets messages;
};
}

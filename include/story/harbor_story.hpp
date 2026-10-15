#pragma once
#include "message_assets.hpp"
#include "message_playback.hpp"
#include "npc_presentation.hpp"
#include <array>
namespace ttyd {
bool loadHarborStoryAssets(const char* root);
const MessageFont& harborMessageFont();
// Four source actors: Goombella, Crump, X-Naut A/B; key is a gor REL
// speaker identity, never a host pointer. Coordinates are original engine units.
// Movement deadlines are milliseconds; flags retain original NPC bits.
struct HarborActor {
    uint32_t key=0;float x=0,y=15,z=-300,targetX=0,speed=0;
    uint64_t moveAt=0;int displacement=0;bool pending=false,moving=false;
    float display=0,yaw=270;uint32_t flags=0x40000623;
    // Field owner supplies speed per simulation visit and previous root; this
    // scene's x/speed driver alone does not derive the pose-selection speed.
    float actualSpeed=0;std::array<float,3> previousRoot{};bool hasPreviousRoot=false;
    NpcPoseClock poseClock;NpcPresentation presentation;bool presentationReady=false;
};
// Bounded native adapter for original mac_0_016..021, preceding the separate
// HarborEncounter VM. stage is native sequencing, not a recovered event opcode.
// Completion releases its input lock; it does not complete the Crump battle.
class HarborStory {
public:
    bool enabled=false,triggered=false,finished=false,failed=false;
    unsigned stage=0;uint32_t speaker=0;uint64_t now=0,deadline=0;
    std::array<float,3> speakerRoot{}; // Print-entry anchor, not a live actor reference.
    std::array<HarborActor,4> actors{};
    MessagePlayback message;
    void reset(bool available);
    bool begin(uint64_t now);
    void tick(uint64_t now,uint32_t keys);
    void preparePresentation(const NpcAngleTable&,const NpcCameraService&,const MessagePlayback*,uint32_t speaker,uint64_t time);
    bool locked() const{return triggered && !finished;}
private:
    bool print(int id,unsigned actor);
    void move(unsigned actor,int delta,float speed,unsigned delay);
    void advanceActors(uint64_t delta);
};
}

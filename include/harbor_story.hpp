#pragma once
#include "message_assets.hpp"
#include "message_playback.hpp"
#include <array>
namespace ttyd {
bool loadHarborStoryAssets(const char* root);
const MessageFont& harborMessageFont();
struct HarborActor {
    uint32_t key=0;float x=0,y=15,z=-300,targetX=0,speed=0;
    uint64_t moveAt=0;int displacement=0;bool pending=false,moving=false;
    float display=0;
};
class HarborStory {
public:
    bool enabled=false,triggered=false,finished=false,failed=false;
    unsigned stage=0;uint32_t speaker=0;uint64_t now=0,deadline=0;
    std::array<HarborActor,4> actors{};
    MessagePlayback message;
    void reset(bool available);
    bool begin(uint64_t now);
    void tick(uint64_t now,uint32_t keys);
    bool locked() const{return triggered && !finished;}
private:
    bool print(int id,unsigned actor);
    void move(unsigned actor,int delta,float speed,unsigned delay);
    void advanceActors(uint64_t delta);
};
}

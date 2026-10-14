#pragma once
#include "actor_animation.hpp"
#include <array>
#include <functional>
namespace ttyd {
// Actual US context1 source-bank/surface admission. This never acknowledges a
// field capture, pose playback, timing delay or battle completion.
class BattleStartupResources {
public:
    struct Pose {std::string name;uint32_t loop=0;float start=0,end=0;uint32_t keys=0;bool supported=false;};
    struct Resource {
        std::string name,textureOwner;uint32_t sourceKey=0,textures=0;
        bool needsCapture=false;float uvScale=1;
        std::vector<uint8_t> originalBank;
        std::vector<Pose> descriptors;
        ActorAnimationPack animations;
    };
    using TextureAdmission=std::function<bool(unsigned,const std::string&)>;
    bool load(const char* root);
    bool admitTexture(unsigned index,const TextureAdmission&);
    void releaseTexture(unsigned index){if(index<9)textureAdmitted[index]=false;}
    void clear(){*this=BattleStartupResources{};}
    const Resource* resource(unsigned index) const{return loaded && index<9?&resources[index]:nullptr;}
    bool paperReady(uint32_t sourceKey,int32_t context) const;
    bool niceReady(int32_t mode) const;
    bool resourceReady(uint32_t sourceKey,int32_t context) const;
    bool ready() const{return loaded;}
private:
    std::string root;
    std::array<Resource,9> resources;
    std::array<bool,9> textureAdmitted{};
    bool loaded=false;
};
}

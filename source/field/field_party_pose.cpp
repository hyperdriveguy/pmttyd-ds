#include "field_party_pose.hpp"
namespace ttyd {
namespace {std::array<std::shared_ptr<const BattlePoseResource>,4> resources;}
bool loadFieldPlayerPoseAssets(const char* root){
 if(!root)return false;
 std::array<std::shared_ptr<const BattlePoseResource>,4> next;
 constexpr const char* names[]={"a_mario","a_mario_r","e_mario","c_pkr"};
 for(unsigned i=0;i<4;++i){const auto path=std::string(root)+"/field-player-"+names[i];next[i]=loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());if(!next[i] || !next[i]->draw || !next[i]->textures || next[i]->modelName!=names[i])return false;}
 resources=std::move(next);return true;
}
std::array<std::shared_ptr<const BattlePoseResource>,4> fieldPlayerPoseResources(){return resources;}
}

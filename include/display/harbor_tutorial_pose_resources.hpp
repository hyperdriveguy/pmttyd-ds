#pragma once
#include "battle_pose_pool.hpp"
#include <array>
namespace ttyd {
// Strong CPU resource catalog for the actual source UnitEntry model names.
// No pose allocation, animation change, source readiness or GPU admission occurs.
class HarborTutorialPoseResources {
public:
    static std::shared_ptr<const HarborTutorialPoseResources> load(const char* root);
    static std::shared_ptr<const HarborTutorialPoseResources> admit(
        std::shared_ptr<const BattlePoseResource> mario,
        std::shared_ptr<const BattlePoseResource> crump,
        std::shared_ptr<const BattlePoseResource> helper);
    std::shared_ptr<const BattlePoseResource> find(const std::string& sourceModel) const;
private:
    std::array<std::shared_ptr<const BattlePoseResource>,3> resources_;
};
}

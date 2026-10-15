#pragma once
#include "battle_pose_pool.hpp"
namespace ttyd {
// One immutable source model shared by original tik enemy kinds. Admission of
// CPU/raw resources does not register a pose, allocate source buffers or admit GX.
class TrioEnemyPoseResources {
public:
    static std::shared_ptr<const TrioEnemyPoseResources> load(const char* directory,const char* pack);
    // Checks the complete supplied source directory and CPU/raw backing once.
    // The runtime still owns actual pose generations and draw admission; this
    // catalog performs no live allocation/resource-generation revalidation.
    static std::shared_ptr<const TrioEnemyPoseResources> admit(std::shared_ptr<const BattlePoseResource>);
    // Call with model/initialName read from the actual kind/part/type28 table.
    // Unknown names remain unavailable; never choose by flattened visual index.
    std::shared_ptr<const BattlePoseResource> find(const std::string& model,const std::string& initialName) const;
    const std::shared_ptr<const BattlePoseResource>& resource() const{return resource_;}
private:
    std::shared_ptr<const BattlePoseResource> resource_;
};
}

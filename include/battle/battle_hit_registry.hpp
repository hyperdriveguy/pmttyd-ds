#pragma once
#include "shadow_collision.hpp"
#include <string>
#include <vector>
namespace ttyd {
struct BattleHitTriangle {
    std::array<std::array<float,3>,3> vertices{},edges{};
    std::array<float,3> normal{};
};
struct BattleHitObject {
    std::string name;
    uint32_t map=0,node=0,parts=0,attributes=0;
    uint16_t flags=0,cull=0;
    std::array<float,3> center{};
    float radius=0;
    std::optional<uint32_t> mobjFlags;
    std::vector<BattleHitTriangle> triangles;
};
struct BattleHitWinner {uint32_t object=0,triangle=0;float distance=0;};
// Independent selected battle-map HitObj owner. Import is an explicit initial
// registry snapshot, not field groundAt/B.floor or inferred runtime history.
class BattleHitRegistry {
public:
    bool load(const char*);
    void invalidate(){known_=false;}
    bool known()const{return known_;}
    const BattleHitObject* object(uint32_t index)const{return index<objects_.size()?&objects_[index]:nullptr;}
    uint32_t count()const{return uint32_t(objects_.size());}
    bool flags(uint32_t index,uint16_t flags);
    bool attributes(uint32_t index,uint32_t attributes);
    bool mobjFlags(uint32_t index,std::optional<uint32_t> flags);
    void publish(); // Source hitMain admission/order; no implicit rebuild in ray.
    ShadowRayResult ray(const ShadowCollisionRay&,ShadowCollisionHit&,BattleHitWinner* winner=nullptr) const;
private:
    bool known_=false;
    std::vector<BattleHitObject> objects_;
    std::vector<uint32_t> active_;
};
}

#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
namespace ttyd {
// Typed native file handles resolve to owned, validated original TPL payloads.
// These are not replicas of the GameCube fileAllocf pointer layout.
class BattleAudienceResources {
public:
    bool load(const char* root);
    std::optional<uint32_t> handle(const std::string& path) const;
    const std::vector<uint8_t>* payload(uint32_t handle) const;
private:
    std::vector<uint8_t> normal_,luigi_;
};
}

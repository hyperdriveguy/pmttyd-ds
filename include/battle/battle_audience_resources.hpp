#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <array>
// Native handles1/2 identify owned normal/Luigi TPLs, not fileAllocf objects.
// Raw BE TPL validation and private LE atlas admission are separate operations;
// atlas readiness alone cannot establish population, animation or guest handles.
namespace ttyd {
// Typed native file handles resolve to owned, validated original TPL payloads.
// These are not replicas of the GameCube fileAllocf pointer layout.
class BattleAudienceResources {
public:
    struct Frame { uint32_t width, height; std::array<float,4> uv; };
    bool load(const char* root);
    // Optional native atlas catalog; does not select animation or allocate actors.
    bool loadFrames(const char* root);
    const Frame* frame(uint32_t handle, uint32_t originalIndex) const;
    std::optional<uint32_t> handle(const std::string& path) const;
    const std::vector<uint8_t>* payload(uint32_t handle) const;
private:
    std::vector<uint8_t> normal_,luigi_;
    std::array<Frame,224> frames_{};
    bool framesReady_=false;
};
}

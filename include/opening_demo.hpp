#pragma once
#include "message_assets.hpp"
#include "message_playback.hpp"
namespace ttyd {
// Original book narration before the title. Market continuation is separate.
class OpeningDemo {
public:
    MessagePlayback message;
    unsigned narration=0;
    uint32_t speaker=0;
    uint64_t now=0;
    bool failed=false,bookComplete=false,finished=false;
    bool load(const char* root);
    void begin(uint64_t now);
    void tick(uint64_t now,uint32_t keys=0);
    unsigned image() const;
private:
    MessageAssets assets;
    MessageFont font;
    uint64_t deadline=0;
    bool print(uint64_t now);
};
}

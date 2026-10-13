#pragma once
#include "message_assets.hpp"
#include "message_playback.hpp"
#include "opening_demo_scene.hpp"
#include "opening_book_scene.hpp"
#include "opening_bgm_scene.hpp"
namespace ttyd {
// Original book narration before the title. Market continuation is separate.
class OpeningDemo {
public:
    MessagePlayback message;
    OpeningDemoScene market;
    OpeningBookScene book;
    OpeningBgmScene bgm;
    unsigned narration=0;
    uint32_t speaker=0;
    uint64_t now=0;
    bool failed=false,bookComplete=false,finished=false,waitingForTitle=false;
    bool bookOpening=false;
    bool paperServicesBound=false; // Renderer registers only after original assets load.
    float bookRotation=0;
    bool load(const char* root);
    void begin(uint64_t now);
    void tick(uint64_t now,uint32_t keys=0);
    unsigned image() const;
    int music() const{return bgm.snapshot().primaryTrack;}
private:
    MessageAssets assets;
    MessageFont font;
    uint64_t deadline=0;
    int waitingPage=-1;

    bool print(uint64_t now);
};
}

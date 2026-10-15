#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
// Bounded original dmo book providers. Source clocks are adapted to simulation
// ticks; page completion must be published after rendering the original pose.
class OpeningBookScene {
public:
    // Four captured papers: old images8..11, depths6/5/4/3. Capture and final
    // evaluated draw are separate admissions; active alone proves neither.
    struct Page {bool active=false,capturePending=false;unsigned image=0;float depth=0;uint64_t start=0;bool drawnComplete=false;};
    std::array<Page,4> pages{};
    float scale=1,rotation=0,fade=0,offsetX=0,offsetY=0;
    float finalPaperScale=1,finalPaperOffsetX=0,finalPaperOffsetY=0;
    bool adjusting=false,adjusted=false;
    unsigned image=0;
    bool opening=false;
    void begin(uint64_t tick);
    void advance(uint64_t tick);
    bool scaleComplete() const{return scale==1.5f;}
    // 780 simulation ticks is the bounded source13-second narration gate,
    // separate from the scale clamp at720 ticks and rendering readiness.
    bool thirdMessageReady() const{return now>=start+780;}
    void open();
    void startAdjust();
    bool beginPage(unsigned index);
    float pageFrame(unsigned index) const;
    void captureComplete(unsigned index);
    // Renderer publishes only after actual final sample submission/completion.
    // A timer or capture acknowledgement alone cannot satisfy page completion.
    void presented(unsigned index,float evaluatedFrame);
    bool pageComplete(unsigned index) const;
    void releasePages();
private:
    uint64_t start=0,now=0,openingStart=0,adjustStart=0;
};
}

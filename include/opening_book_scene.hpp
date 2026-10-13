#pragma once
#include <array>
#include <cstdint>
namespace ttyd {
// Bounded original dmo book providers. Source clocks are adapted to simulation
// ticks; page completion must be published after rendering the original pose.
class OpeningBookScene {
public:
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
    bool thirdMessageReady() const{return now>=start+780;}
    void open();
    void startAdjust();
    bool beginPage(unsigned index);
    float pageFrame(unsigned index) const;
    void captureComplete(unsigned index);
    void presented(unsigned index,float evaluatedFrame);
    bool pageComplete(unsigned index) const;
    void releasePages();
private:
    uint64_t start=0,now=0,openingStart=0,adjustStart=0;
};
}

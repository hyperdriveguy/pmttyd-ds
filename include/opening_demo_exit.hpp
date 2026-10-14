#pragma once
#include <cstdint>
namespace ttyd {
// Original buttonCheck + demo mapChange type17 override. Original entry
// demo.tpl admission and type9 final presentation own the input gate.
class OpeningDemoExit {
public:
    bool entryAdmitted=false,entryFinished=false,requested=false,driverFinished=false,opaquePresented=false;
    bool parentExitRequest=false;
    uint32_t buttonMask=0x1100,configuredType=17,configuredMilliseconds=500;
    uint32_t actualType=10,actualMilliseconds=2000,sourceColor=0xf0f0f0ff,sequence=3;
    const char* destination="title";
    float progress=0;
    void begin(uint64_t tick){*this={};now=tick;}
    // demo_init requests type9/500 after actual original demo.tpl admission.
    void admitEntry(){if(!entryAdmitted){entryAdmitted=true;entryLastAdvance=now;}}
    uint8_t entryAlpha() const{return uint8_t(int(255.f*entryProgress));}
    void entryPresented(uint8_t alpha){if(entryAdmitted && entryDriverFinished && alpha==0)entryFinished=true;}
    bool entryDriverFinished=false;
    uint32_t entryType=9,entryMilliseconds=500,entryColor=0x000000ff;
    float entryProgress=1;
    bool request(uint32_t triggers,bool finalMainWaiting){
        if(!entryFinished || requested || !(triggers&buttonMask))return false;
        requested=true;parentExitRequest=finalMainWaiting;
        // buttonCheck publishes500 first; final demo_evt can additionally
        // publish2000 before mapChange unconditionally selects2000.
        configuredMilliseconds=finalMainWaiting?2000:500;lastAdvance=now;return true;
    }
    void advance(uint64_t tick){
        if(tick<now)return;
        now=tick;
        if(entryAdmitted && !entryDriverFinished){
            while(entryLastAdvance<now && !entryDriverFinished){
                ++entryLastAdvance;
                entryElapsedMilliseconds=entryElapsedMilliseconds+1000.f/60.f;
                entryProgress=1.f-entryElapsedMilliseconds/float(entryMilliseconds);
                if(entryProgress<0){entryProgress=0;entryDriverFinished=true;}
            }
        }
        if(!requested || driverFinished)return;
        while(lastAdvance<now && !driverFinished){
            ++lastAdvance;
            elapsedMilliseconds=elapsedMilliseconds+1000.f/60.f;
            progress=elapsedMilliseconds/float(actualMilliseconds);
            // Source fadeMain sets finish only after ratio>1, not at equality.
            if(progress>1){progress=1;driverFinished=true;}
        }
    }
    uint8_t alpha() const{return uint8_t(int(255.f*progress));}
    void presented(uint8_t alpha){if(requested && driverFinished && alpha==255)opaquePresented=true;}
    bool finished() const{return requested && driverFinished && opaquePresented;}
private:
    uint64_t now=0,entryLastAdvance=0,lastAdvance=0;
    float elapsedMilliseconds=0,entryElapsedMilliseconds=0;
};
}

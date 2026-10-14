#pragma once
#include "field_camera_events.hpp"
#include "opening_title_bgm.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
namespace ttyd {
// Ordinary demo -> title path from original titleMain. Explicit native resource
// and final-pose presentation providers own readiness; no elapsed-only ack.
class OpeningTitle {
public:
    OpeningTitleBgm bgm;
    uint32_t state=0,curtainType=34;
    bool musicReady=false,resourcesReady=false,curtainReady=false,failed=false;
    bool filesRequested=false,demoRequested=false,complete=false;
    uint32_t counter=0,curtainCounter=0,promptCounter=0,idleCounter=0;
    int32_t initialHideCounter=23;
    bool narrowFast=true,whiteActive=false;
    float paperScale=3,logoX=0,logoY=1000,copyrightX=0,copyrightY=-1000,promptAmplitude=0;
    float whiteProgress=1,blackProgress=0;bool whiteDriverFinished=false,blackDriverFinished=false,blackPresented=false;
    bool curtainFinalPresented=false;float curtainLoops=0;
    void begin(){*this={};}
    void admitMusic(bool actual){musicReady=actual;}
    void admitResources(){resourcesReady=true;}
    void admitCurtain(uint32_t type){if(type==curtainType)curtainReady=true;}
    float curtainFrame() const{return curtainType==34?0.f:float(std::min<uint32_t>(curtainCounter,curtainType==33?160:curtainType==63?80:60));}
    bool curtainDriverFinished() const{return curtainReady && (curtainType==34?curtainCounter==(narrowFast?90u:180u) && curtainLoops>=1:curtainLoops>=1);}
    void presented(uint32_t type,float frame,float scale){
        if(type!=curtainType || !curtainDriverFinished() || !std::isfinite(frame) || !std::isfinite(scale))return;
        const float end=type==34?0.f:type==33?160.f:type==63?80.f:60.f;
        if(frame>=end && (type!=34 || scale==1))curtainFinalPresented=true;
    }
    uint8_t blackAlpha() const{return uint8_t(int(255.f*blackProgress));}
    void presentedBlack(uint8_t alpha){if(state==21 && blackDriverFinished && alpha==255)blackPresented=true;}
    uint8_t whiteAlpha() const{return uint8_t(int(255.f*whiteProgress));}
    uint8_t promptAlpha() const{
        const float phase=(3.141592025756836f*float(promptCounter))/120.f;
        const float value=promptAmplitude*(459.f*float(std::sin(double(phase))));
        return uint8_t(std::clamp(int(value),0,255));
    }
    bool inputReady() const{return state==3 && counter>60;}
    void tick(uint32_t triggers=0){
        if(failed || complete)return;
        bgm.tick();
        ++promptCounter; // Native 60Hz adapter for original disp2 draw counter.
        if(whiteActive && !whiteDriverFinished){
            whiteElapsed=whiteElapsed+1000.f/60.f;whiteProgress=1-whiteElapsed/500.f;
            if(whiteProgress<0){whiteProgress=0;whiteDriverFinished=true;}
        }
        if(state==21 && !blackDriverFinished){
            blackElapsed=blackElapsed+1000.f/60.f;blackProgress=blackElapsed/1000.f;
            if(blackProgress>1){blackProgress=1;blackDriverFinished=true;}
        }
        if(state && curtainReady && !curtainFinalPresented){
            ++curtainCounter;
            if(curtainType==34){curtainCounter=std::min<uint32_t>(curtainCounter,narrowFast?90:180);paperScale=*cameraInterpolation(12,curtainCounter,narrowFast?90:180,narrowFast?1.5f:3.f,1);curtainLoops=float(curtainCounter);}
            else curtainLoops=float(curtainCounter)/float(curtainType==33?160:curtainType==63?80:60);
        }
        switch(state){
        case 0:
            if(!musicReady || !resourcesReady)return;
            state=1;return; // Original21/0 white and34/0 source requests.
        case 1:
            --initialHideCounter;
            if(!curtainFinalPresented)return;
            state=2;whiteProgress=1;whiteElapsed=0;whiteDriverFinished=false;counter=0;
            // Original20/500 white-in overlaps the logo interpolation.
            whiteActive=true;return;
        case 2:
            logoX=copyrightX=0;
            logoY=*cameraInterpolation(10,counter,60,300,120);
            copyrightY=*cameraInterpolation(11,counter,60,-300,-170);
            ++counter;if(counter>60){state=3;counter=0;promptCounter=0;}return;
        case 3:
            ++counter;if(counter<=60)return;
            promptAmplitude=std::min(1.f,promptAmplitude+.1f);
            if(triggers&0x1100){filesRequested=true;state=10;return;}
            ++idleCounter;if(idleCounter>19*60){demoRequested=true;state=20;}return;
        case 20:
            bgm.offForIdle();requestCurtain(33);counter=0;blackElapsed=blackProgress=0;blackDriverFinished=blackPresented=false;state=21;return;
        case 21:
            logoY=*cameraInterpolation(11,counter,60,120,400);
            copyrightY=*cameraInterpolation(11,counter,60,-170,-400);
            counter=std::min<uint32_t>(counter+1,60);
            promptAmplitude=std::max(0.f,promptAmplitude-.1f);
            if(blackPresented)complete=true;
            return;
        case 10:
            requestCurtain(63);state=11;return;
        case 11:
            if(!curtainFinalPresented)return;
            requestCurtain(26);state=12;return;
        case 12:
            if(curtainFinalPresented)complete=true;
            return;
        }
    }
private:
    float whiteElapsed=0,blackElapsed=0;
    void requestCurtain(uint32_t type){curtainType=type;curtainReady=false;curtainCounter=0;curtainLoops=0;curtainFinalPresented=false;paperScale=1;}
};
}

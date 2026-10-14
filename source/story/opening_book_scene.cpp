#include "opening_book_scene.hpp"
#include "field_camera_events.hpp"
#include <algorithm>
#include <cmath>
namespace ttyd {
void OpeningBookScene::begin(uint64_t tick){*this={};start=now=tick;scale=1;advance(tick);}
void OpeningBookScene::advance(uint64_t tick){
    if(tick<now)return;
    now=tick;
    // Original demo_scale computes integer milliseconds from elapsed OSTime,
    // blends600/1500 over12000, divides1000, then clamps at1.5.
    const uint64_t elapsed=now-start;
    const float milliseconds=float(std::min<uint64_t>(elapsed,720)*1000/60);
    const float q=milliseconds/12000.f;
    if(!adjusting && !adjusted)scale=std::min(1.5f,std::fma(600.f,1-q,1500.f*q)/1000.f);
    if(opening){
        const auto elapsedOpen=std::min<uint64_t>(now-openingStart,120);
        rotation=*cameraInterpolation(11,int32_t(elapsedOpen),120,0,1800)/10.f;
        if(elapsedOpen==120){rotation=180;opening=false;}
    }
    if(adjusting){
        const uint64_t elapsedAdjust=std::min<uint64_t>(now-adjustStart,180);
        // Source linear integer0..1000 over3000ms, then LW0/=1000.
        const auto milliseconds=elapsedAdjust*1000/60;
        fade=float(milliseconds*1000/3000)/1000.f;
        const float wave=float(std::sin(double(1.5707963705062866f*fade)));
        scale=std::fma(1.5f,wave,1.5f);offsetY=-105.f*wave;
        finalPaperScale=scale/1.5f;finalPaperOffsetX=offsetX/10.f;finalPaperOffsetY=offsetY/10.f;
        if(elapsedAdjust==180){adjusting=false;adjusted=true;releasePages();}
    }
}
void OpeningBookScene::open(){opening=true;openingStart=now;rotation=0;image=8;}
void OpeningBookScene::startAdjust(){if(adjusting || adjusted)return;adjusting=true;adjustStart=now;fade=0;finalPaperScale=scale/1.5f;}
bool OpeningBookScene::beginPage(unsigned index){
    if(index>=4 || pages[index].active)return false;
    // next increments the source background before its paper starts; the old
    // capture remains alive until the following page animation completes.
    pages[index]={true,true,8+index,6.f-float(index),now,false};
    return true;
}
float OpeningBookScene::pageFrame(unsigned index) const {
    if(index>=4 || !pages[index].active || pages[index].capturePending)return 0;
    return float(std::min<uint64_t>(now-pages[index].start,180));
}
void OpeningBookScene::captureComplete(unsigned index){if(index<4 && pages[index].active && pages[index].capturePending){pages[index].capturePending=false;pages[index].start=now;image=9+index;}}
void OpeningBookScene::presented(unsigned index,float frame){
    if(index>=4 || !pages[index].active || pages[index].capturePending || !std::isfinite(frame) || frame<180 || pageFrame(index)<180)return;
    pages[index].drawnComplete=true;
    if(index)pages[index-1]={};
}
bool OpeningBookScene::pageComplete(unsigned index) const{return index<4 && pages[index].active && !pages[index].capturePending && pages[index].drawnComplete;}
void OpeningBookScene::releasePages(){pages={};}
}

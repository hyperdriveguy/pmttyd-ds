#include "opening_demo.hpp"
#include <cstdio>
namespace ttyd {
bool OpeningDemo::load(const char* root){if(!root)return false;const std::string b=root;return assets.loadGlobal((b+"/messages/global.txt").c_str()) && assets.loadRoom((b+"/messages/dmo_00.txt").c_str()) && font.load((b+"/font.pmfont").c_str());}
void OpeningDemo::begin(uint64_t now){this->now=now;narration=0;speaker=0;failed=false;bookComplete=false;finished=false;message={};deadline=now+4000;}
bool OpeningDemo::print(uint64_t now){
    static constexpr uint32_t speakers[]={0,0x2040,0,0x2048,0x2040,0x2048,0x2040,0x2040,0x2048,0x2040,0x2048,0x2040,0x2040};
    speaker=narration>24?speakers[narration-25]:0;
    char key[24];if(narration<=24)std::snprintf(key,sizeof(key),"op_book_%02u",narration);else std::snprintf(key,sizeof(key),"op_%02u",narration-25);if(!message.open(key,assets.find(key),font,now)){failed=true;return false;}return true;}
void OpeningDemo::tick(uint64_t now,uint32_t keys){
    if(failed || finished || now<this->now)return;
    this->now=now;
    if(message.active()){
        message.tick(now,bookComplete?keys:0);if(message.active())return;
        deadline=now;
        // Source explicit waits after page turns. Paper animation time remains
        // a native presentation adapter rather than an original-runtime claim.
        if(narration==2 || narration==4 || narration==8 || narration==12 || narration==16 || narration==21 || narration==24)deadline+=1000;
    }
    if(now<deadline)return;
    if(narration==37){finished=true;return;}
    if(narration==24)bookComplete=true;
    ++narration;print(now);
}
unsigned OpeningDemo::image() const {
    if(narration<5)return 0;
    if(narration<=8)return 8;
    if(narration<=12)return 9;
    if(narration<=16)return 10;
    if(narration<=21)return 11;
    return 12;
}
}

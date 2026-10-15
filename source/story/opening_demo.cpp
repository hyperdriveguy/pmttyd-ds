#include "opening_demo.hpp"
#include <cstdio>
#include <algorithm>
namespace ttyd {
bool OpeningDemo::load(const char* root){if(!root)return false;const std::string b=root;return assets.loadGlobal((b+"/messages/global.txt").c_str()) && assets.loadRoom((b+"/messages/dmo_00.txt").c_str()) && font.load((b+"/font.pmfont").c_str()) && loadDemoChestEffectAssets(root) && market.loadFacing((b+"/npc-angle.bin").c_str());}
// Start a fresh narration/scene/exit owner. 4000ms is the source narration
// brother wait; scene clocks are explicit floor(ms*60/1000) simulation adapters,
// not rendering frames or proof of the complete original concurrent scheduler.
void OpeningDemo::begin(uint64_t now){this->now=now;narration=0;speaker=0;failed=false;bookComplete=false;finished=false;waitingForTitle=false;bookOpening=false;bookRotation=0;waitingPage=-1;market.reset();book.begin(now*60/1000);bgm.begin(now*60/1000);exit.begin(now*60/1000);message={};deadline=now+4000;}
bool OpeningDemo::print(uint64_t now){
    // Book has no actor token; market op_00..12 retains the original13-entry
    // speaker order (0/0x2040/0x2048). Keep exact message IDs and attribution.
    static constexpr uint32_t speakers[]={0,0x2040,0,0x2048,0x2040,0x2048,0x2040,0x2040,0x2048,0x2040,0x2048,0x2040,0x2040};
    speaker=narration>24?speakers[narration-25]:0;
    char key[24];if(narration<=24)std::snprintf(key,sizeof(key),"op_book_%02u",narration);else std::snprintf(key,sizeof(key),"op_%02u",narration-25);if(!message.open(key,assets.find(key),font,now)){failed=true;return false;}return true;}
void OpeningDemo::tick(uint64_t now,uint32_t keys){
    if(failed || finished || now<this->now)return;
    this->now=now;
    // An exit REQUEST is not completed exit. Current adapter continues book,
    // market/effect/message clocks below until actual opaque presentation makes
    // exit.finished() true; bgm also advances before that completion gate.
    bgm.advance(now*60/1000);exit.advance(now*60/1000);
    exit.request(keys,waitingForTitle);
    if(waitingForTitle)exit.requestAutomatic();
    if(exit.finished()){finished=true;return;}
    book.advance(now*60/1000);bookOpening=book.opening;bookRotation=book.rotation;
    if(bookComplete)market.advance(now*60/1000);
    if(market.effects.failed){failed=true;return;}
    if(message.active()){
        message.tick(now,bookComplete?keys:0);if(message.active())return;
        deadline=now;
        // Source explicit waits after page turns. Paper animation time remains
        // a native presentation adapter rather than an original-runtime claim.
        if(bookComplete){market.messageComplete(narration-25);market.advance(now*60/1000);}
        else if(narration==24){book.startAdjust();bgm.transition();bookComplete=true;market.begin(now*60/1000);}
        else if(narration==4){book.open();bookOpening=true;deadline+=3000;}
        // Registered original paper renderer owns capture/final-pose admission
        // after book08/12/16/21. Unregistered mode keeps a provisional1000ms
        // static-page wait and must not be called source animation completion.
        else if(paperServicesBound && (narration==8 || narration==12 || narration==16 || narration==21)){
            waitingPage=narration==8?0:narration==12?1:narration==16?2:3;
            if(!book.beginPage(unsigned(waitingPage))){failed=true;return;}
        }
        else if(narration==8 || narration==12 || narration==16 || narration==21)deadline+=1000;
    }
    // Only actual pageComplete releases this wait, then add original1000ms.
    // Third book line additionally waits boot+780 adapter ticks (13000ms).
    // Market message gates come from authored requests; final op_12 leaves its
    // source title/automatic-exit owner running rather than inventing another line.
    if(waitingPage>=0){if(!book.pageComplete(unsigned(waitingPage)))return;waitingPage=-1;deadline=now+1000;}
    if(now<deadline || (!bookComplete && narration==2 && !book.thirdMessageReady()))return;
    if(bookComplete){waitingForTitle=market.waitingForTitle;if(narration==37 || !market.messageReady(narration-24))return;}
    ++narration;print(now);
}
// Source TPL identities: cover/closed image0, illustrations8..12. Bound paper
// mode retains old background until capture acknowledgement; static fallback
// selects by narration ranges and does not prove original paper lifecycle.
unsigned OpeningDemo::image() const {
    if(paperServicesBound && narration>=5 && narration<=24)return book.image;
    if(narration<5)return narration==4 && !message.active()?8:0;
    if(narration<=8)return 8;
    if(narration<=12)return 9;
    if(narration<=16)return 10;
    if(narration<=21)return 11;
    return 12;
}
}

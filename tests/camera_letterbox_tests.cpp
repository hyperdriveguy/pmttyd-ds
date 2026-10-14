#include "camera_letterbox.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Provider {
    uintptr_t party=17;bool available=true,soundReady=false;unsigned parties=0,sounds=0;
    uint32_t bero=0,partyFlags=0;int32_t key=0,ctrl=0,rate=60;std::vector<int32_t> sequences{2};unsigned sequenceAt=0;
    std::vector<LetterboxQuery> reads;
    static std::optional<uintptr_t> get(void* p){auto& s=*static_cast<Provider*>(p);++s.parties;return s.party;}
    static std::optional<int32_t> query(void* p,LetterboxQuery q,uintptr_t party){
        auto& s=*static_cast<Provider*>(p);assert(party==s.party);s.reads.push_back(q);if(!s.available)return {};
        switch(q){case LetterboxQuery::BeroExec:return int32_t(s.bero);case LetterboxQuery::Sequence:return s.sequences.at(s.sequenceAt++);
        case LetterboxQuery::MarioKey:return s.key;case LetterboxQuery::MarioCtrl:return s.ctrl;case LetterboxQuery::PartyFlags:return int32_t(s.partyFlags);case LetterboxQuery::Rate:return s.rate;}
        return {};
    }
    static bool sound(void* p,bool set,uint16_t mask){auto& s=*static_cast<Provider*>(p);++s.sounds;assert(mask==0x20);assert(set== (s.sequences[0]==2));return s.soundReady;}
    CameraLetterboxServices services(){return {this,get,query,sound};}
};
CameraState camera(){CameraManager b;b.initializeBoot(608,480);return *b.camera(8);}
int main(){
    auto c=camera();DisplayQueue q;CameraLetterboxCursor cursor;Provider p;p.available=false;
    assert(!updateCameraLetterbox(c,q,p.services(),cursor) && cursor.pending==0x8007ac40 && p.parties==1);
    p.available=true;assert(!updateCameraLetterbox(c,q,p.services(),cursor) && cursor.pending==0x800dc110);
    assert(c.publication.flags&0x100);assert(c.letterboxAlpha==0 && c.letterboxTop==0 && q.count()==0);
    const auto reads=p.reads;p.soundReady=true;assert(updateCameraLetterbox(c,q,p.services(),cursor));
    assert(p.reads==reads && p.parties==1 && c.letterboxAlpha==255 && c.letterboxTop==3 && c.letterboxBottom==3);
    assert(q.count()==1 && q.record(0)->camera==8 && q.record(0)->drawClass==3 && q.record(0)->callback==0x8000e564 && q.record(0)->userdata==0);
    // Already enabled: no reinitialization; original stored draw-camera remains.
    c.publication.flags|=0x200;c.drawCamera=12;c.letterboxTop=48;c.letterboxBottom=43;
    Provider locked;locked.soundReady=true;assert(updateCameraLetterbox(c,q,locked.services(),cursor));
    assert(locked.reads.empty() && locked.sounds==0 && locked.parties==1 && c.letterboxTop==49 && c.letterboxBottom==44 && q.record(1)->camera==12);
    c.letterboxTop=32767;c.letterboxBottom=32767;assert(updateCameraLetterbox(c,q,locked.services(),cursor));
    assert(c.letterboxTop==-32766 && c.letterboxBottom==-32766); // Narrow signed store precedes cap.
    c.publication.flags|=0x400;assert(updateCameraLetterbox(c,q,locked.services(),cursor));
    assert(!(c.publication.flags&0x400) && c.letterboxTop==49 && c.letterboxBottom==44);
    // Disable re-queries the sequence; sequence3 preserves the prior enabled state.
    c.publication.flags=0x100;Provider preserve;preserve.sequences={4,3};
    assert(updateCameraLetterbox(c,q,preserve.services(),cursor));assert(preserve.sequenceAt==2 && preserve.sounds==0 && (c.publication.flags&0x100));
    // Disable flag store happens before the unavailable clear sound call.
    Provider disable;disable.sequences={4,4};c.letterboxAlpha=255;
    assert(!updateCameraLetterbox(c,q,disable.services(),cursor) && cursor.pending==0x800dc0f8 && !(c.publication.flags&0x100));
    disable.soundReady=true;assert(updateCameraLetterbox(c,q,disable.services(),cursor));assert(c.letterboxAlpha==247 && disable.sequenceAt==2);
    // Bero busy bypasses decisions, but disabled animation and registration remain.
    Provider busy;busy.bero=1;c.letterboxAlpha=4;const auto count=q.count();
    assert(updateCameraLetterbox(c,q,busy.services(),cursor));assert(c.letterboxAlpha==0 && q.count()==count+1 && busy.sequenceAt==0);
    c.letterboxAlpha=-32768;assert(updateCameraLetterbox(c,q,busy.services(),cursor));assert(c.letterboxAlpha==32760);
    busy.rate=0;const auto alpha=c.letterboxAlpha;assert(!updateCameraLetterbox(c,q,busy.services(),cursor) && c.letterboxAlpha==alpha);
    busy.rate=60;assert(updateCameraLetterbox(c,q,busy.services(),cursor));assert(c.letterboxAlpha==32752);
    // Registration capacity hold cannot repeat bar increments or scalar decisions.
    DisplayQueue full;for(unsigned i=0;i<1024;++i)assert(full.enter(0,0,0,0,0)==DisplayAdmission::Registered);
    c=camera();c.publication.flags=0x300;Provider capacity;
    assert(!updateCameraLetterbox(c,full,capacity.services(),cursor) && cursor.pending==0x80010bc4 && c.letterboxTop==3);
    assert(!updateCameraLetterbox(c,full,capacity.services(),cursor) && c.letterboxTop==3 && capacity.parties==1);
    full.reinitialize();assert(updateCameraLetterbox(c,full,capacity.services(),cursor) && c.letterboxTop==3 && full.count()==1);
    // Source Key/Ctrl and live party flags choose disable; neither is an input enum.
    c=camera();Provider control;control.key=1;control.ctrl=1;control.sequences={2,3};
    assert(updateCameraLetterbox(c,q,control.services(),cursor));assert(!(c.publication.flags&0x100));
    assert(control.reads==std::vector<LetterboxQuery>({LetterboxQuery::BeroExec,LetterboxQuery::Sequence,LetterboxQuery::MarioKey,LetterboxQuery::MarioCtrl,LetterboxQuery::Sequence,LetterboxQuery::Rate}));
}

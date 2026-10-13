#include "save_block_presentation.hpp"
#include <cassert>
#include <algorithm>
#include <vector>
using namespace ttyd;
struct Run {
    SaveBlockPresentation p;MapObjectKeyControl keys;
    uint64_t tick=0,ticket=0;unsigned writes=0;
    SaveBlockSwitches snapshot;
    std::vector<std::string> windows;
    bool sawRetained=false,sawBusyWrite=false;
    void step(int choice,bool success) {
        MessageSelectionInput input;
        if(p.selection.state==1) {
            if(p.selection.current!=choice)input.buttons=4;
            else input.buttons=0x100;
        } else if(p.message.waiting() || p.message.state==1)input.buttons=0x100;
        if(p.selection.active()) {
            sawRetained=true;
            assert(p.message.stopped() && p.message.active() && !p.message.pending());
        }
        const uint64_t now=tick++*1000/60;p.tick(now,input);
        if(windows.empty() || windows.back()!=p.message.rawText())windows.push_back(p.message.rawText());
        uint64_t candidate;SaveBlockSwitches s;
        if(p.takeWriteRequest(candidate,s)) {
            ++writes;ticket=candidate;snapshot=s;
            assert(!p.takeWriteRequest(candidate,s));
            assert(!p.completeWrite(ticket+1,success));
        }
        if(ticket) {
            sawBusyWrite=true;
            // Several complete simulation updates remain pending until I/O acknowledges.
            if(tick%7==0) {
                assert(p.completeWrite(ticket,success));
                assert(!p.completeWrite(ticket,success));ticket=0;
            }
        }
    }
    void run(int choice,bool success,SaveBlockSwitches initial) {
        keys.off(); // Outer save-object callback owns the first key leaf.
        assert(p.begin(0,initial,17,keys));
        assert(!p.begin(0,initial,18,keys));
        for(unsigned i=0;i<5000 && p.active();++i)step(choice,success);
        assert(p.event.finished && !p.failed && sawRetained && keys.depth==1);
        keys.on();assert(keys.depth==0 && !(keys.marioFlags&8));
        for(unsigned i=0;i<100 && p.message.active();++i)step(choice,success);
        assert(!p.message.active() && !p.message.pending());
        assert(!p.selection.message.active() && !p.selection.message.pending());
    }
};
int main() {
    assert(loadSaveBlockPresentationAssets("romfs"));
    assert(!loadSaveBlockPresentationAssets("/nonexistent/pm-save-assets"));
    SaveBlockPresentation empty;assert(!empty.message.pending());
    Run no;no.run(1,true,{8,false,false,9});
    assert(no.writes==0 && no.p.event.switches.firstHit && no.p.event.switches.postStoryHit && no.p.event.switches.mode==9);
    assert(std::any_of(no.windows.begin(),no.windows.end(),[](const auto& s){return s.find("<scrl_auto><once_stop>")!=std::string::npos;}));
    Run yes;yes.run(0,true,{7,false,false,9});
    assert(yes.writes==1 && yes.sawBusyWrite && yes.snapshot.mode==0 && yes.snapshot.firstHit && yes.snapshot.postStoryHit);
    assert(yes.p.message.rawText().find("saved")!=std::string::npos);
    Run failure;failure.run(0,false,{8,true,true,0});
    assert(failure.writes==1 && failure.sawBusyWrite && failure.p.event.switches.story==8);
    assert(!failure.p.completeWrite(1,true));
    // Source B cancellation comes from the original selection control, independent
    // of the retained print window's key history.
    MapObjectKeyControl keys;SaveBlockPresentation cancel;
    assert(cancel.begin(0,{8,true,true,0},17,keys));
    bool pressed=false;
    for(unsigned i=0;i<5000 && cancel.active();++i) {
        MessageSelectionInput input;
        if(cancel.selection.state==1){input.buttons=0x200;pressed=true;}
        else if(cancel.message.waiting() || cancel.message.state==1)input.buttons=0x100;
        cancel.tick(uint64_t(i)*1000/60,input);
    }
    uint64_t ticket;SaveBlockSwitches snapshot;
    assert(pressed && cancel.event.finished && !cancel.failed && !cancel.takeWriteRequest(ticket,snapshot) && !keys.depth);
    MapObjectKeyControl movedKeys;SaveBlockPresentation original;
    assert(original.begin(0,{8,true,true,0},17,movedKeys));
    uint64_t now=0;ticket=0;
    for(unsigned i=0;i<5000;++i) {
        MessageSelectionInput input;
        if(original.selection.state==1 || original.message.waiting() || original.message.state==1)input.buttons=0x100;
        now=uint64_t(i)*1000/60;original.tick(now,input);
        if(original.takeWriteRequest(ticket,snapshot))break;
    }
    assert(ticket && original.active());
    auto moved=std::move(original);
    assert(!original.active() && !original.completeWrite(ticket,true));
    assert(moved.completeWrite(ticket,true));
    for(unsigned i=1;i<5000 && moved.active();++i) {
        MessageSelectionInput input;input.buttons=0x100;
        moved.tick(now+uint64_t(i)*1000/60,input);
    }
    assert(moved.event.finished && !moved.failed && movedKeys.depth==0);
}

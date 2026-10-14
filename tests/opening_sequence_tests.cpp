#include "opening_sequence.hpp"
#include "opening_demo.hpp"
#include <cassert>
#include <cstdio>
#include <vector>
#include <string>
using namespace ttyd;
int main(){
    OpeningDemo demo;assert(demo.load("romfs"));demo.begin(0);demo.exit.admitEntry();
    std::vector<std::string> ids;uint64_t now=0;bool bookOpening=false;
    for(;now<500000 && !demo.waitingForTitle;now+=17){demo.tick(now);bookOpening|=demo.bookOpening;if(demo.message.active() && (ids.empty() || ids.back()!=demo.message.id()))ids.push_back(demo.message.id());assert(!demo.failed);}
    assert(demo.waitingForTitle && !demo.finished && ids.size()==37 && bookOpening && demo.bookRotation==180);
    demo.tick(now+10000,0x200);assert(!demo.finished);
    demo.exit.entryPresented(0);assert(demo.exit.entryFinished);
    demo.tick(now+10017,0x1000);assert(!demo.finished && demo.exit.requested);
    demo.tick(now+13017);assert(!demo.finished && demo.exit.driverFinished);
    demo.exit.presented(255);demo.tick(now+13034);assert(demo.finished);
    for(unsigned i=0;i<37;++i){char key[24];if(i<24)std::snprintf(key,sizeof(key),"op_book_%02u",i+1);else std::snprintf(key,sizeof(key),"op_%02u",i-24);assert(ids[i]==key);}
    // mapChange keeps the old scene event alive until the opaque frame is
    // actually presented. A requested exit must not freeze its message owner.
    OpeningDemo exiting;assert(exiting.load("romfs"));exiting.begin(0);exiting.exit.admitEntry();
    exiting.tick(1000);exiting.exit.entryPresented(0);
    exiting.tick(1100,0x100);assert(exiting.exit.requested && exiting.narration==0);
    exiting.tick(4100);assert(!exiting.finished && exiting.exit.driverFinished);
    assert(exiting.narration==1 && exiting.message.active());
    exiting.exit.presented(255);exiting.tick(4117);assert(exiting.finished);
    OpeningSequence opening;assert(opening.load("romfs"));opening.begin(0);ids.clear();std::vector<uint32_t> speakers;
    bool voyageGate=false;
    for(now=0;now<300000 && !opening.complete();now+=17){
        const uint32_t keys=(now/17)%4==0?0x100u:0u;
        // Explicit host display providers; native renderer must publish real
        // source work/fade completions independently of these test doubles.
        opening.heading.entryComplete=opening.heading.displayReady=opening.heading.startFadeComplete=opening.heading.started=opening.heading.exitComplete=true;
        opening.tick(now,keys);assert(!opening.failed);
        if(opening.part==OpeningPart::Voyage)voyageGate=true;
        if(opening.message.active() && (ids.empty() || ids.back()!=opening.message.id())){ids.push_back(opening.message.id());speakers.push_back(opening.speaker);}
    }
    assert(opening.complete() && voyageGate && opening.story==3);
    assert((ids==std::vector<std::string>{"pro_00","pro_01","pro_02","pro_03","pro_04","pro_05","mac_0_012","mac_0_013","mac_0_013_01","mac_0_014","mac_0_015"}));
    assert((speakers==std::vector<uint32_t>{0x468,0x474,0x474,0x474,0,0x474,0,0,0,0,0}));
    OpeningSequence noInput;assert(noInput.load("romfs"));noInput.begin(0);
    for(now=0;now<120000;now+=17)noInput.tick(now,0);
    assert(noInput.part==OpeningPart::House && noInput.message.id()=="pro_00" && noInput.message.waiting());
    assert(noInput.house.messageReady(0) && noInput.message.requireTerminalInput);
    OpeningSequence gate;assert(gate.load("romfs"));gate.begin(0);
    for(now=0;now<200000 && gate.part!=OpeningPart::Voyage;now+=17)gate.tick(now,0x100);
    assert(gate.part==OpeningPart::Voyage);for(unsigned i=0;i<3000;++i){now+=17;gate.tick(now,0);}assert(gate.part==OpeningPart::Voyage);
    gate.tick(now+17,0x1000);assert(gate.part==OpeningPart::Prologue);
    OpeningSequence missing;assert(!missing.load("/missing-original-assets"));
    std::puts("37 original demo + 11 opening messages, source speakers and voyage gate passed");
}

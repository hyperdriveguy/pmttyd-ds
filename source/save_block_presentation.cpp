#include "save_block_presentation.hpp"
#include "message_assets.hpp"
#include "message_icons.hpp"
namespace ttyd {
namespace {MessageAssets messages;MessageFont font;bool ready=false;}
bool loadSaveBlockPresentationAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;MessageAssets m;MessageFont f;
    if(!m.loadGlobal((base+"/messages/global.txt").c_str()) ||
       !f.load((base+"/font.pmfont").c_str()) ||
       !loadMessageIcons((base+"/message-icons.bin").c_str()) ||
       !loadSaveBlockEventAssets(root))return false;
    messages=std::move(m);font=std::move(f);ready=true;return true;
}
bool SaveBlockPresentation::begin(uint64_t milliseconds,const SaveBlockSwitches& switches,uint32_t owner,MapObjectKeyControl& control) {
    if(!ready || active() || message.active() || selection.active() || writeWaiting)return false;
    SaveBlockEvent next;if(!next.begin(milliseconds,switches,owner))return false;
    event=std::move(next);keys=&control;now=milliseconds;failed=false;
    writeQueued=writeWaiting=writeDone=false;writeCode=0;return true;
}
void SaveBlockPresentation::tick(uint64_t milliseconds,const MessageSelectionInput& input) {
    if(milliseconds<now)return;
    now=milliseconds;
    // Window processing precedes the event's polling. A newly resumed/opened
    // window first progresses on the following simulation invocation.
    message.tick(now,input.buttons);
    if(selection.active())selection.tick(input);
    if(!active())return;
    SaveEventServices services;services.user=this;
    services.call=[](void* p,SaveEventRequest& request){return static_cast<SaveBlockPresentation*>(p)->call(request);};
    event.tick(now,services);
    if(writeQueued)writeSnapshot=event.switches; // Includes writes in this VM invocation.
}
int32_t SaveBlockPresentation::call(SaveEventRequest& request) {
    if(failed)return 0;
    auto fault=[&](){failed=true;return 0;};
    auto text=[&](){return (request.flags&1)?request.text.c_str():messages.find(request.text.c_str());};
    switch(request.kind) {
    case SaveEventCall::Print:
        if(request.first && (!text() || !message.open(request.text.c_str(),text(),font,now)))return fault();
        return message.pending()?0:2;
    case SaveEventCall::Append:
        if(request.first && (!text() || !message.append(text(),font,now)))return fault();
        return message.pending()?0:2;
    case SaveEventCall::Continue:
        if(request.first && !message.resume(now))return fault();
        return message.pending()?0:2;
    case SaveEventCall::Select:
        if(request.first && (!text() || !selection.open(request.text.c_str(),text(),font,now)))return fault();
        if(!selection.complete())return 0;
        request.result=selection.current;return 2;
    case SaveEventCall::Key:
        if(request.first){if(request.flags)keys->on();else keys->off();}return 2;
    case SaveEventCall::Header:
        // Native SD serialization combines header/data in one atomic write;
        // retain the original yielding boundary without emulating GC hardware.
        return request.first?0:2;
    case SaveEventCall::Write:
        if(request.first) {
            writeTicket=++nextTicket;writeSnapshot=event.switches;
            writeQueued=writeWaiting=true;writeDone=false;return 0;
        }
        if(!writeDone)return 0;
        writeWaiting=writeDone=false;return 2;
    case SaveEventCall::Code:request.result=writeCode;return 2;
    case SaveEventCall::Guard:request.result=0;return 2;
    case SaveEventCall::Cleanup:case SaveEventCall::Recovery:return 2;
    // This SD backend publishes only success0 or generic error-1. Other source
    // card-mode operations require a provider before those paths can execute.
    case SaveEventCall::Ipl:case SaveEventCall::Create:case SaveEventCall::Format:return fault();
    }
    return fault();
}
bool SaveBlockPresentation::takeWriteRequest(uint64_t& ticket,SaveBlockSwitches& snapshot) {
    if(!active() || !writeQueued)return false;
    ticket=writeTicket;snapshot=writeSnapshot;writeQueued=false;return true;
}
bool SaveBlockPresentation::completeWrite(uint64_t ticket,bool success) {
    if(!active() || !writeWaiting || writeQueued || writeDone || ticket!=writeTicket)return false;
    writeCode=success?0:-1;writeDone=true;return true;
}
}

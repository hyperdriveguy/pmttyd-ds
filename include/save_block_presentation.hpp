#pragma once
#include "map_object_lifecycle.hpp"
#include "message_selection.hpp"
#include "save_block_event.hpp"
namespace ttyd {
bool loadSaveBlockPresentationAssets(const char* root);
// Portable retained windows for the original event. The owning field subsystem
// supplies raw input and services the write queue outside simulation updates.
class SaveBlockPresentation {
public:
    SaveBlockPresentation()=default;
    SaveBlockPresentation(const SaveBlockPresentation&)=delete;
    SaveBlockPresentation& operator=(const SaveBlockPresentation&)=delete;
    SaveBlockPresentation(SaveBlockPresentation&&)=default;
    SaveBlockPresentation& operator=(SaveBlockPresentation&&)=default;
    bool begin(uint64_t milliseconds,const SaveBlockSwitches&,uint32_t owner,MapObjectKeyControl&);
    void tick(uint64_t milliseconds,const MessageSelectionInput&);
    bool active() const{return event.active() && !failed;}
    bool takeWriteRequest(uint64_t& ticket,SaveBlockSwitches& snapshot);
    bool completeWrite(uint64_t ticket,bool success);
    MessagePlayback message;
    MessageSelection selection;
    SaveBlockEvent event;
    bool failed=false;
private:
    int32_t call(SaveEventRequest&);
    MapObjectKeyControl* keys=nullptr;
    uint64_t now=0,nextTicket=0,writeTicket=0;
    SaveBlockSwitches writeSnapshot;
    bool writeQueued=false,writeWaiting=false,writeDone=false;
    int32_t writeCode=0;
};
}

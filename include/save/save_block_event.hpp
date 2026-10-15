#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>
namespace ttyd {
bool loadSaveBlockEventAssets(const char* root);
// Ordinal order matches the14 source function addresses in save_block_event.
// These are typed service requests; enum value is not a guest function pointer.
enum class SaveEventCall:uint32_t {Select,Continue,Append,Print,Key,Write,Ipl,Header,Create,Format,Cleanup,Guard,Recovery,Code};
struct SaveEventRequest {
    SaveEventCall kind;
    uint32_t function=0;
    bool first=false;
    int32_t flags=0;
    std::string text; // Original message ID, or literal original text when flags&1.
    int32_t result=0; // Caller supplies selection, guard or completion-code output.
    uint8_t argumentCount=0;
    std::array<int32_t,4> originalOperands{},values{}; // Preserve source speaker/coordinates and output references.
};
struct SaveEventServices {
    void* user=nullptr;
    // Original USER_FUNC return protocol:0 busy,2 complete. Platform owns
    // retained message windows, source choices and explicit asynchronous card I/O.
    int32_t (*call)(void*,SaveEventRequest&)=nullptr;
};
// Original namespaces: GSW0 story, GSW4 mode, GSF233 first hit, GSF236
// post-story hit. Distinct from event LW/LF and native checkpoint schema.
struct SaveBlockSwitches {int32_t story=0;bool firstHit=false,postStoryHit=false;uint8_t mode=0;};
struct SaveBlockEventRuntime;
// Native wrapper around an actual original-data EVT VM. Runtime owner is
// rebound after moves/before ticks. Do not copy an active instance expecting an
// independent VM: its shared_ptr runtime is shared, with one current owner.
class SaveBlockEvent {
public:
    bool begin(uint64_t milliseconds,const SaveBlockSwitches&,uint32_t owner);
    void tick(uint64_t milliseconds,const SaveEventServices&);
    bool active()const{return runtime && started && !finished && !failed;}
    bool started=false,finished=false,failed=false;
    SaveBlockSwitches switches;
private:
    std::shared_ptr<SaveBlockEventRuntime> runtime;
};
}

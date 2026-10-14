#pragma once
#include <cstdint>
#include <array>
#include <memory>
#include <string>
namespace ttyd {
bool loadSaveBlockEventAssets(const char* root);
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
struct SaveBlockSwitches {int32_t story=0;bool firstHit=false,postStoryHit=false;uint8_t mode=0;};
struct SaveBlockEventRuntime;
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

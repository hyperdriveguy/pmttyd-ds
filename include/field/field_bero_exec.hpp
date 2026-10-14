#pragma once
#include <cstdint>
#include <optional>
namespace ttyd {
struct EventEntry;
// Source startup BSS producer is0. Unknown transition history must not become0.
class FieldBeroExec {
public:
    void afterInfo(){bits_=0;known_=~0u;} // Only after actual get_info providers complete.
    void unknown(uint32_t mask=~0u){known_&=~mask;}
    void onOff(int32_t operation,uint32_t mask){
        bits_=operation==0?bits_|mask:bits_&~mask;known_|=mask;
    }
    std::optional<uint32_t> read(uint32_t mask=~0u)const{return (known_&mask)==mask?std::optional<uint32_t>(bits_&mask):std::nullopt;}
private:
    uint32_t bits_=0,known_=~0u;
};
struct FieldBeroExecEventServices {
    void* user=nullptr;
    uint32_t (*word)(void*,uint32_t)=nullptr;
    int32_t (*value)(void*,EventEntry&,int32_t)=nullptr;
    FieldBeroExec* (*owner)(void*)=nullptr;
};
// Captured args pointer; each raw read precedes its resolver, then live owner/store.
int32_t fieldBeroExecOnOffEvent(EventEntry&,const FieldBeroExecEventServices&);
}

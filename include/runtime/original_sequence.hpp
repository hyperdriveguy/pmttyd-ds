#pragma once
#include <array>
#include <cstdint>

namespace ttyd {
// Semantic view of the original 32-byte seqWork; pointers retain source tokens.
using OriginalSequenceWork = std::array<uint32_t,8>;
// -1 is source boot/unselected;field2 and battle4 are ORIGINAL table IDs,
// independent of native Scene. Requested changes do not directly set current.
// Work is source32-byte storage;checkpoint import is explicit native policy.
struct OriginalSequenceState {
    int32_t current=-1,requested=-1,previous=-1;
    uint32_t p0=0,p1=0;
    OriginalSequenceWork work{};
};
struct OriginalSequenceServices {
    void* user=nullptr;
    bool (*drawSync)(void*)=nullptr;
    // False suspends this callback. Its provider owns any transitive cursor.
    bool (*callback)(void*,uint32_t identity,OriginalSequenceWork&)=nullptr;
};
class OriginalSequence {
public:
    OriginalSequenceState state;
    void request(int32_t sequence,uint32_t p0,uint32_t p1);
    // Sync/Exit/reset/current/Init/Main stages retain their reached callback.
    // Caller keeps callback providers/work/lifetimes stable across holds;no
    // automatic service-owner identity check or transitive-child cancellation.
    bool main(const OriginalSequenceServices&);
    uint32_t pending() const{return pending_;}
    // Checkpoint/field admission adapter, not execution of original field Init.
    void importNativeField();
private:
    uint8_t stage_=0;
    uint32_t callback_=0,pending_=0;
};
// Complete seq_gameExit80007390 leaf. This does not release field resources.
void originalFieldSequenceExit(uint32_t& gpFlags);
}

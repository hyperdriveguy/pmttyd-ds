#pragma once
#include <cstdint>
#include <memory>
#include <optional>

namespace ttyd {
// A real registry/actor owner must supply these pins; a pointer comparison is
// insufficient to prove that a retired slot was not reused across a hold.
struct FieldMarioKeyPartyCapture {
    const void* actor=nullptr;
    std::shared_ptr<const void> registry, incarnation;
};
// Original function/callsite identities distinguish repeated calls to one
// getter. argument is source operand storage, not a native pointer encoding.
struct FieldMarioKeyRequest {
    uint32_t function=0,callsite=0;
    int32_t argument=0;
};
struct FieldMarioKeyServices {
    void* user=nullptr;
    bool (*current)(void*)=nullptr;
    std::optional<int32_t> (*query)(void*,const FieldMarioKeyRequest&)=nullptr;
    std::optional<FieldMarioKeyPartyCapture> (*party)(void*,int32_t)=nullptr;
    bool (*partyCurrent)(void*,const FieldMarioKeyPartyCapture&)=nullptr;
    std::optional<uint8_t> (*marioByte)(void*,uint32_t)=nullptr;
    std::optional<uint32_t> (*partyWord)(void*,const FieldMarioKeyPartyCapture&,uint32_t)=nullptr;
    std::optional<uint8_t> (*partyByte)(void*,const FieldMarioKeyPartyCapture&,uint32_t)=nullptr;
};
// One invocation caches completed result until reset. Provider-table mismatch
// holds without replacement; observed lifetime loss poisons until reset. It
// rejects recursive resume/reset from callbacks and retains actual actor pins.
class FieldMarioKeyCursor {
public:
    bool reset();
    uint32_t pending() const {return pending_;}
private:
    friend std::optional<int32_t> resumeOriginalMarioChkKey(const FieldMarioKeyServices&,FieldMarioKeyCursor&);
    FieldMarioKeyServices services_{};
    FieldMarioKeyPartyCapture party_{};
    unsigned step_=0;
    int32_t id_=0;
    uint32_t pending_=0;
    std::optional<int32_t> result_;
    bool bound_=false,invoking_=false,poisoned_=false;
};
// nullopt means the original query/read is still unavailable. Successful
// children never replay. The actual context/pins must remain alive until reset.
std::optional<int32_t> resumeOriginalMarioChkKey(const FieldMarioKeyServices&,FieldMarioKeyCursor&);
// marioChkCtrl8005c464: signed byte38 equals zero; NOT the depth itself.
std::optional<int32_t> originalMarioChkCtrl(const FieldMarioKeyServices&);
}

#pragma once
#include "effect_pool.hpp"
#include <array>
#include <memory>
namespace ttyd {
struct HitN64Services {
    EffectPoolServices pool;
    void* user=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
    std::optional<int32_t> (*randomRaw)(void*)=nullptr; // Original8026c324.
    bool (*effectDelete)(void*,uint32_t)=nullptr; // Whole effDelete8005ea30.
    std::optional<float> (*depth)(void*,const std::array<float,3>&)=nullptr;
    bool (*display)(void*,int32_t,int32_t,uint32_t,uint32_t,float)=nullptr;
};
// One source invocation with exact input FLOAT BITS/provider identity. Reentry
// and reset while invoking are refused. Reset/disposal drops continuation only;
// actual parent owns partial-record allocation/cancellation/deletion lifetime.
class HitN64EntryCursor {
public:
    HitN64EntryCursor();~HitN64EntryCursor();
    HitN64EntryCursor(const HitN64EntryCursor&)=delete;
    HitN64EntryCursor& operator=(const HitN64EntryCursor&)=delete;
    bool active()const;uint32_t pending()const;bool reset();
private:
    struct State;std::unique_ptr<State> state_;bool invoking_=false;
    friend std::optional<uint32_t> resumeHitN64Entry(int32_t,int32_t,const std::array<float,3>&,float,const HitN64Services&,HitN64EntryCursor&);
};
// One completed Main call advances simulation once. A held queue/delete child
// resumes without repeating age/lifetime writes;success resets for next visit.
class HitN64MainCursor {
public:
    HitN64MainCursor();~HitN64MainCursor();
    HitN64MainCursor(const HitN64MainCursor&)=delete;
    HitN64MainCursor& operator=(const HitN64MainCursor&)=delete;
    bool active()const;uint32_t pending()const;bool reset();
private:
    struct State;std::unique_ptr<State> state_;bool invoking_=false;
    friend bool resumeHitN64Main(uint32_t,const HitN64Services&,HitN64MainCursor&);
};
// Complete801c6344/270; wrapper8013f1b4/20 forwards every argument unchanged.
// Source effEntry and heap3 allocate are real owners, not invented guest handles.
// No source null-allocation repair exists: original unchecked stores are retained.
std::optional<uint32_t> resumeHitN64Entry(int32_t kind,int32_t amount,const std::array<float,3>& position,float scale,const HitN64Services&,HitN64EntryCursor&);
// Complete801c61bc/188. Each completed call is ONE simulation invocation.
// Actual effDelete / source depth / queued effHitDisp801c5d6c remain providers.
// A held display must not acknowledge without real queued callback ownership.
bool resumeHitN64Main(uint32_t effect,const HitN64Services&,HitN64MainCursor&);
}

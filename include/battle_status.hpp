#pragma once
#include <array>
#include <cstdint>
#include <cassert>

namespace ttyd {
struct StatusChangeInfo {
    uint8_t type=0, duration=0, strength=0, extra=0;
    uint32_t value=0, retained=0;
};
// Semantic unit fields shared by status, movement eligibility and damage.
// Status bytes retain original offsets118..135; this is not a packed unit ABI.
struct BattleStatusUnit {
    std::array<uint8_t,30> statusBytes{};
    std::array<uint8_t,5> moveBytes{}; // Original20..24.
    int16_t hp=0;
    int32_t kind=0;
    int8_t belong=0;
    uint8_t zapTap=0;
    uint32_t flags=0, statusFlags=0;
    std::array<StatusChangeInfo,6> statusChangeInfo{};
    uint8_t& statusByte(unsigned offset) {assert(offset>=0x118 && offset<=0x135);return statusBytes[offset-0x118];}
    uint8_t statusByte(unsigned offset) const {assert(offset>=0x118 && offset<=0x135);return statusBytes[offset-0x118];}
};
struct StatusValue {uint8_t duration=0,strength=0;};
StatusValue getStatus(const BattleStatusUnit&,int32_t id);
bool checkStatus(const BattleStatusUnit*,int32_t id);
int setStatus(BattleStatusUnit&,int32_t id,int32_t duration,int32_t strength);
int checkRecoveryStatus(BattleStatusUnit*,int32_t id);
void resetMoveStatus(BattleStatusUnit&);
void setMaxMoveCount(BattleStatusUnit&,int32_t count);
bool canActStatus(const BattleStatusUnit*);
bool canGuardStatus(const BattleStatusUnit*);
bool actionCommandPossible(const BattleStatusUnit&);
}

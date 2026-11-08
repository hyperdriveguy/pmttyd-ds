#pragma once
#include "event_memory_opcodes.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
// Actual original bmapLoad8002892c children. Void children leave result empty;
// archive query and smartEntry must publish their genuine source return value.
struct BattleMapLoadRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,3> arguments{};
 unsigned count=0;
 std::optional<uint32_t> result;
};
struct BattleMapLoadServices {
 EventMemoryServices memory;
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 bool (*call)(void*,BattleMapLoadRequest&)=nullptr;
};
class BattleMapLoadCursor {
public:
 struct State;
 std::shared_ptr<State> state;
 uint32_t pending()const;
 bool complete()const;
};
// Both names are actual source string addresses. Captures source r30/r31 once;
// providers, mapped memory and original process/heap lifetime stay stationary
// through holds. No map count or active-bank value is fabricated on a wait.
bool resumePackedBattleMapLoad(uint32_t baseName,uint32_t foregroundName,
 const BattleMapLoadServices&,BattleMapLoadCursor&);
}

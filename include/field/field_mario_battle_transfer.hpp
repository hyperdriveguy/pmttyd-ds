#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
constexpr uint32_t MarioFieldBattlePrepare=0x8005c3cc;
constexpr uint32_t MarioFieldBattlePost=0x8005c30c;
// An actual source child, including the dynamic party ability callback. The
// private vector is native storage for source Post's uninitialized SP08..10;
// it is never an encoded guest stack address. The child owns partial work.
struct FieldMarioBattleTransferRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,2> arguments{};
 std::optional<uint32_t> result;
 std::array<std::optional<uint32_t>,3>* privateVector=nullptr;
};
struct FieldMarioBattleTransferServices {
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 std::optional<uint32_t> (*word)(void*,uint32_t)=nullptr;
 std::optional<uint16_t> (*half)(void*,uint32_t)=nullptr;
 std::optional<uint8_t> (*byte)(void*,uint32_t)=nullptr;
 // False means no store occurred. Earlier accepted stores are retained.
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*call)(void*,FieldMarioBattleTransferRequest&)=nullptr;
};
// Whole Prepare/Post with whole partyUsePost and both-slot CtrlOff/On bodies.
// No source callback may reset/replace its backing owners during invocation.
// Completed cursors stay completed; create a new cursor for the next call.
class FieldMarioBattleTransfer {
public:
 explicit FieldMarioBattleTransfer(uint32_t originalFunction);
 ~FieldMarioBattleTransfer();
 FieldMarioBattleTransfer(const FieldMarioBattleTransfer&)=delete;
 FieldMarioBattleTransfer& operator=(const FieldMarioBattleTransfer&)=delete;
 bool resume(const FieldMarioBattleTransferServices&);
 uint32_t pending()const;
 bool complete()const;
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
};
}

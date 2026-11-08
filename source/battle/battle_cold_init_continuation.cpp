#include "battle_cold_init_continuation.hpp"

namespace ttyd {
namespace {
enum class Operation { Call, StoreWord, StoreHalf };
enum class Argument { None, Allocate, ClearWork, Work, Information, SequenceMain,
                      SequenceInit, ClearResult };
struct Instruction {
    uint32_t pc;
    Operation operation;
    uint32_t target;
    uint32_t value;
    Argument arguments = Argument::None;
};
// Store targets are work-relative, except the source global pointer cell.
// This ordered list follows the original parent; child bodies stay separately
// owned. No source scratch stack is exposed as a native guest address.
constexpr Instruction Program[] = {
    {0x800f8f20, Operation::Call, 0x800f8934, 0, Argument::Allocate},
    {0x800f8f28, Operation::StoreWord, BattleColdPointerCell, 0, Argument::Work},
    {0x800f8f38, Operation::Call, 0x800050b4, 0, Argument::ClearWork},
    {0x800f8f44, Operation::StoreWord, 0x19070, 0xffffffff},
    {0x800f8f48, Operation::StoreWord, 0x424, 0xffffffff},
    {0x800f8f4c, Operation::StoreWord, 0x2738, 0, Argument::Information},
    {0x800f8f50, Operation::Call, 0x8011a48c, 0},
    {0x800f8f54, Operation::Call, 0x80216908, 0},
    {0x800f8f58, Operation::Call, 0x801aa228, 0},
    {0x800f8f60, Operation::Call, 0x800fa62c, 0, Argument::Work},
    {0x800f8f64, Operation::Call, 0x800f7af0, 0},
    {0x800f8f68, Operation::Call, 0x80195a94, 0},
    {0x800f8f6c, Operation::Call, 0x801030f8, 0},
    {0x800f8f7c, Operation::Call, 0x800f8a08, 0, Argument::SequenceMain},
    {0x800f8f8c, Operation::Call, 0x800f8a08, 0, Argument::SequenceInit},
    {0x800f8f98, Operation::StoreWord, 0xf28, 0},
    {0x800f8fa8, Operation::StoreWord, 0x1720, 0},
    {0x800f8fac, Operation::StoreWord, 0x1bb4, 0},
    {0x800f8fb0, Operation::StoreWord, 0x1bb8, 0},
    {0x800f8fb4, Operation::StoreWord, 0x1bc0, 0},
    {0x800f8fb8, Operation::StoreWord, 0x1bc4, 0},
    {0x800f8fbc, Operation::StoreWord, 0x1bcc, 0},
    {0x800f8fc0, Operation::StoreWord, 0x1bd0, 0},
    {0x800f8fc4, Operation::StoreWord, 0x1bd8, 0},
    {0x800f8fc8, Operation::StoreWord, 0x1bdc, 0},
    {0x800f8fcc, Operation::StoreWord, 0x1be4, 0},
    {0x800f8fd0, Operation::StoreWord, 0x1be8, 0},
    {0x800f8fd4, Operation::StoreWord, 0x1bf0, 0},
    {0x800f8fd8, Operation::StoreWord, 0x1bf4, 0},
    {0x800f8fdc, Operation::StoreWord, 0x1bfc, 0},
    {0x800f8fe0, Operation::StoreWord, 0x1c00, 0},
    {0x800f8fe4, Operation::StoreWord, 0x1c08, 0},
    {0x800f8fe8, Operation::StoreWord, 0x1c0c, 0},
    {0x800f8fec, Operation::StoreWord, 0x1c14, 0},
    {0x800f8ff0, Operation::StoreWord, 0x1c18, 0},
    {0x800f8ff4, Operation::StoreWord, 0x1c20, 0},
    {0x800f8ff8, Operation::StoreWord, 0x1c24, 0},
    {0x800f8ffc, Operation::StoreWord, 0x1c78, 0},
    {0x800f9000, Operation::StoreHalf, 0, 0},
    {0x800f9004, Operation::Call, 0x800050b4, 0, Argument::ClearResult},
    {0x800f900c, Operation::StoreWord, 0xef4, 0},
    {0x800f9010, Operation::Call, 0x80129694, 0},
    {0x800f9018, Operation::Call, 0x800f7c20, 0, Argument::Work},
    {0x800f901c, Operation::Call, 0x80139780, 0},
    {0x800f9020, Operation::Call, 0x801a83f8, 0},
    {0x800f9024, Operation::Call, 0x8020507c, 0},
};
bool same(const BattleColdInitServices& a, const BattleColdInitServices& b) {
    return a.user == b.user && a.current == b.current && a.call == b.call &&
        a.lifetime == b.lifetime && !a.lifetime.owner_before(b.lifetime) &&
        !b.lifetime.owner_before(a.lifetime) &&
        a.memory.user == b.memory.user && a.memory.storeWord == b.memory.storeWord &&
        a.memory.storeHalf == b.memory.storeHalf;
}
struct Guard {
    bool& value;
    explicit Guard(bool& v) : value(v) { value = true; }
    ~Guard() { value = false; }
};
}
struct BattleColdInitCursor::State {
    BattleColdInitServices services;
    uint32_t information = 0, work = 0, pending = 0;
    size_t instruction = 0;
    bool invalid = false;
    bool current() {
        if (invalid || !services.current(services.user)) { invalid = true; return false; }
        return true;
    }
};
BattleColdInitCursor::BattleColdInitCursor() = default;
BattleColdInitCursor::~BattleColdInitCursor() = default;
bool BattleColdInitCursor::active() const { return bool(state_); }
uint32_t BattleColdInitCursor::pending() const { return state_ ? state_->pending : 0; }
bool BattleColdInitCursor::reset() {
    if (invoking_) return false;
    state_.reset();
    return true;
}
bool resumePackedBattleColdInit(uint32_t information, const BattleColdInitServices& s,
                               BattleColdInitCursor& cursor) {
    if (cursor.invoking_ || !s.lifetime || !s.current || !s.call ||
        !s.memory.storeWord || !s.memory.storeHalf) return false;
    if (cursor.state_ && (cursor.state_->information != information ||
                          !same(cursor.state_->services, s))) return false;
    Guard guard(cursor.invoking_);
    if (!cursor.state_) {
        cursor.state_ = std::make_unique<BattleColdInitCursor::State>();
        cursor.state_->services = s;
        cursor.state_->information = information;
    }
    auto& c = *cursor.state_;
    if (!c.current()) return false;
    for (; c.instruction < sizeof(Program) / sizeof(*Program); ++c.instruction) {
        const auto& op = Program[c.instruction];
        if (!c.current()) return false;
        if (op.operation == Operation::Call) {
            BattleColdInitRequest request;
            request.callsite = op.pc;
            request.function = op.target;
            switch (op.arguments) {
            case Argument::Allocate: request.arguments = {BattleColdWorkBytes, 0, 0}; request.argumentCount = 1; break;
            case Argument::ClearWork: request.arguments = {c.work, 0, BattleColdWorkBytes}; request.argumentCount = 3; break;
            case Argument::Work: request.arguments = {c.work, 0, 0}; request.argumentCount = 1; break;
            case Argument::SequenceMain: request.arguments = {c.work, 0, 0}; request.argumentCount = 3; break;
            case Argument::SequenceInit: request.arguments = {c.work, 1, 0x01000000}; request.argumentCount = 3; break;
            case Argument::ClearResult: request.arguments = {c.work + 0x19060, 0, 16}; request.argumentCount = 3; break;
            default: break;
            }
            c.pending = op.target;
            const auto result = s.call(s.user, request);
            if (!c.current() || !result) return false;
            if (op.arguments == Argument::Allocate) c.work = *result;
            c.pending = 0;
        } else {
            const auto address = op.arguments == Argument::Work ? op.target : c.work + op.target;
            const auto value = op.arguments == Argument::Work ? c.work :
                               op.arguments == Argument::Information ? information : op.value;
            if (op.operation == Operation::StoreHalf)
                s.memory.storeHalf(s.memory.user, address, uint16_t(value));
            else s.memory.storeWord(s.memory.user, address, value);
            if (!c.current()) return false;
        }
    }
    cursor.state_.reset();
    return true;
}
}

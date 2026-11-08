#include "battle_unit_setup_continuation.hpp"
#include <cstring>

namespace ttyd {
namespace {
constexpr uint32_t Entry = 0x80128fe0, Equip = 0x800f831c, Import = 0x80127134;
constexpr uint32_t SystemDefinition = 0x80416a80, MarioDefinition = 0x80416a50;
constexpr uint32_t PartnerDefinitions = 0x80363790, DefinitionBytes = 0x30;
constexpr unsigned EquippedCount = 200;
enum class Step {
    Capture, Pouch, Alliance, System, SystemEquip, Mario, MarioEquip, MarioImport,
    Party, PartyEntry, PartyHome, PartySetHome, PartySetPosition, PartyEquip,
    PartyImport, GroupBase, GroupTest, GroupEntry, GroupItem, GroupEquip,
    GroupTail, Equipment
};
int64_t signedWord(uint32_t value) {
    return value < 0x80000000 ? int64_t(value) : int64_t(value) - 0x100000000LL;
}
bool same(const BattleUnitSetupContinuationServices& a, const BattleUnitSetupContinuationServices& b) {
    const auto& x = a.actors.memory; const auto& y = b.actors.memory;
    return a.user == b.user && a.current == b.current && a.call == b.call &&
        a.lifetime == b.lifetime && !a.lifetime.owner_before(b.lifetime) &&
        !b.lifetime.owner_before(a.lifetime) && a.actors.user == b.actors.user &&
        a.actors.byte == b.actors.byte && x.user == y.user && x.word == y.word &&
        x.half == y.half && x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
        x.storeByte == y.storeByte;
}
struct Guard {
    bool& value;
    explicit Guard(bool& v) : value(v) { value = true; }
    ~Guard() { value = false; }
};
}
struct BattleUnitSetupCursor::State {
    BattleUnitSetupContinuationServices services;
    uint32_t battle = 0, npc = 0, group = 0, pouch = 0, unit = 0;
    uint32_t definition = 0, ordinal = 0, offset = 0, pending = 0;
    float homeY = 0;
    bool invalid = false;
    Step step = Step::Capture;
    std::optional<BattleUnitSetupRequest> held;
    bool current() {
        if (invalid || !services.current(services.user)) { invalid = true; return false; }
        return true;
    }
    uint32_t word(uint32_t a) const { return services.actors.memory.word(services.actors.memory.user, a); }
    void word(uint32_t a, uint32_t v) const { services.actors.memory.storeWord(services.actors.memory.user, a, v); }
    uint16_t half(uint32_t a) const { return services.actors.memory.half(services.actors.memory.user, a); }
    void half(uint32_t a, uint16_t v) const { services.actors.memory.storeHalf(services.actors.memory.user, a, v); }
    void byte(uint32_t a, uint8_t v) const { services.actors.memory.storeByte(services.actors.memory.user, a, v); }
    float real(uint32_t a) const { const auto bits = word(a); float v; std::memcpy(&v, &bits, 4); return v; }
    std::optional<BattleUnitSetupOutput> call(uint32_t pc, uint32_t function,
                                            std::array<uint32_t, 3> integers = {}) {
        if (!held) held = BattleUnitSetupRequest{function, pc, integers, {}};
        pending = function;
        if (!current()) return {};
        const auto result = services.call(services.user, *held);
        if (!current() || !result) return {};
        held.reset(); pending = 0;
        return result;
    }
    std::optional<BattleUnitSetupOutput> position(uint32_t pc, uint32_t function) {
        // Separate reached source loads for each setter. On hold the exact
        // request, including constants/private homeY, remains owned here.
        if (!held) held = BattleUnitSetupRequest{function, pc, {unit, 0, 0},
                                                {real(0x80422178), homeY, real(0x8042217c)}};
        return call(pc, function);
    }
};
BattleUnitSetupCursor::BattleUnitSetupCursor() = default;
BattleUnitSetupCursor::~BattleUnitSetupCursor() = default;
bool BattleUnitSetupCursor::active() const { return bool(state_); }
uint32_t BattleUnitSetupCursor::pending() const { return state_ ? state_->pending : 0; }
bool BattleUnitSetupCursor::reset() { if (invoking_) return false; state_.reset(); return true; }
bool resumePackedBattleUnitSetup(uint32_t b, const BattleUnitSetupContinuationServices& s,
                                BattleUnitSetupCursor& cursor) {
    const auto& m = s.actors.memory;
    if (cursor.invoking_ || !s.lifetime || !s.current || !s.call || !s.actors.byte ||
        !m.word || !m.half || !m.storeWord || !m.storeHalf || !m.storeByte) return false;
    if (cursor.state_ && (cursor.state_->battle != b || !same(cursor.state_->services, s))) return false;
    Guard guard(cursor.invoking_);
    if (!cursor.state_) {
        cursor.state_ = std::make_unique<BattleUnitSetupCursor::State>();
        cursor.state_->battle = b; cursor.state_->services = s;
    }
    auto& c = *cursor.state_;
    for (;;) {
        if (!c.current()) return false;
        switch (c.step) {
        case Step::Capture:
            c.npc = c.word(c.word(b + 0x2738) + 0xc); c.group = c.word(c.npc + 0x18);
            c.step = Step::Pouch; break;
        case Step::Pouch: {
            const auto r = c.call(0x800f8acc, 0x800d5eb0); if (!r) return false;
            c.pouch = r->value; c.step = Step::Alliance; break;
        }
        case Step::Alliance:
            c.half(b + 8, 2); c.half(b + 0x10, 1); c.half(b + 0x18, 0);
            c.byte(b + 0xa, 1); c.byte(b + 0x12, 255); c.byte(b + 0x1a, 1);
            c.word(b + 0xc, 0); c.word(b + 0x14, 0); c.word(b + 0x1c, 0);
            c.step = Step::System; break;
        case Step::System: {
            const auto r = c.call(0x800f8b0c, Entry, {SystemDefinition, 0, 0}); if (!r) return false;
            c.unit = r->value; c.step = Step::SystemEquip; break;
        }
        case Step::SystemEquip:
            if (!c.call(0x800f8b18, Equip, {c.unit, 3, 0})) return false;
            c.step = Step::Mario; break;
        case Step::Mario: {
            const auto r = c.call(0x800f8b20, Entry, {MarioDefinition, 0, 0}); if (!r) return false;
            c.unit = r->value; c.step = Step::MarioEquip; break;
        }
        case Step::MarioEquip:
            if (!c.call(0x800f8b30, Equip, {c.unit, 3, 0})) return false;
            c.step = Step::MarioImport; break;
        case Step::MarioImport:
            if (!c.call(0x800f8b38, Import, {c.unit, 0, 0})) return false;
            c.step = Step::Party; break;
        case Step::Party: {
            const auto kind = c.word(c.word(b + 0x2738) + 4);
            c.definition = PartnerDefinitions + (kind - 224) * DefinitionBytes;
            c.step = kind ? Step::PartyEntry : Step::GroupBase; break;
        }
        case Step::PartyEntry: {
            const auto r = c.call(0x800f8b60, Entry, {c.definition, 0, 0}); if (!r) return false;
            c.unit = r->value; c.step = Step::PartyHome; break;
        }
        case Step::PartyHome: {
            const auto r = c.call(0x800f8b74, 0x801289b0, {c.unit, 0, 0}); if (!r) return false;
            c.homeY = r->position[1]; c.step = Step::PartySetHome; break;
        }
        case Step::PartySetHome:
            if (!c.position(0x800f8b88, 0x801289a0)) return false;
            c.step = Step::PartySetPosition; break;
        case Step::PartySetPosition:
            if (!c.position(0x800f8b9c, 0x80128c6c)) return false;
            c.step = Step::PartyEquip; break;
        case Step::PartyEquip:
            if (!c.call(0x800f8bac, Equip, {c.unit, 5, 0})) return false;
            c.step = Step::PartyImport; break;
        case Step::PartyImport:
            if (!c.call(0x800f8bb4, Import, {c.unit, 0, 0})) return false;
            c.step = Step::GroupBase; break;
        case Step::GroupBase:
            c.definition = c.word(c.group + 4); c.ordinal = c.offset = 0;
            c.step = Step::GroupTest; break;
        case Step::GroupTest:
            c.step = signedWord(c.ordinal) < signedWord(c.word(c.group)) ? Step::GroupEntry : Step::Equipment;
            break;
        case Step::GroupEntry: {
            const auto r = c.call(0x800f8bcc, Entry, {c.definition, 0, 0}); if (!r) return false;
            c.unit = r->value; c.step = Step::GroupItem; break;
        }
        case Step::GroupItem: {
            c.byte(c.unit + 0xe, uint8_t(c.ordinal)); c.word(c.unit + 0x308, 0);
            const auto raw = s.actors.byte(s.actors.user, c.unit + 0xe);
            const auto index = raw < 128 ? uint32_t(raw) : uint32_t(raw) - 256;
            const auto held = c.word(c.npc + 0x5c + index * 4);
            if (held) {
                c.word(c.unit + 0x308, held); c.word(c.unit + 0x1c, c.word(c.unit + 0x1c) | 0x40000000);
                c.step = Step::GroupTail;
            } else {
                c.word(c.unit + 0x308, c.word(c.npc + 0x1c + c.offset));
                // Reached source argument read is distinct from the stored copy.
                c.held = BattleUnitSetupRequest{Equip, 0x800f8c24,
                    {c.unit, 1, c.word(c.npc + 0x1c + c.offset)}, {}};
                c.step = Step::GroupEquip;
            }
            break;
        }
        case Step::GroupEquip:
            if (!c.call(0x800f8c24, Equip)) return false;
            c.step = Step::GroupTail; break;
        case Step::GroupTail: {
            const auto value = c.word(c.npc + 0x3c + c.offset);
            c.definition += DefinitionBytes; ++c.ordinal; c.offset += 4;
            c.word(c.unit + 0x30c, value);
            c.half(c.unit + 0x10c, c.half(c.unit + 0x108));
            c.half(c.unit + 0x112, c.half(c.unit + 0x10e));
            c.step = Step::GroupTest; break;
        }
        case Step::Equipment:
            c.word(b + 0x420, 0xffffffff); c.word(b + 0x163f4, 0);
            for (unsigned slot = 0; slot < EquippedCount; ++slot) {
                const auto raw = c.half(c.pouch + 0x38a + 2 * slot);
                const auto item = raw < 0x8000 ? uint32_t(raw) : uint32_t(raw) - 0x10000;
                const auto index = item - 308;
                constexpr uint32_t Bits[] = {1, 2, 4, 8, 16, 128, 0, 256, 1024, 2048, 512, 4096};
                // Source case314 branches to the next slot without a store.
                if (index < 12 && Bits[index]) c.word(b + 0x163f4, c.word(b + 0x163f4) | Bits[index]);
            }
            if (!c.current()) return false;
            cursor.state_.reset(); return true;
        }
    }
}
}

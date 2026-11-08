#include "harbor_source_startup.hpp"
#include "battle_cold_storage.hpp"
#include "battle_camera_frame.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_packed_unit_equipment.hpp"
#include "battle_sequence_startup.hpp"
#include "battle_unit_resources.hpp"
#include <cstdlib>
#include <cstring>

namespace ttyd {
namespace {
void required(bool value) { if (!value) std::abort(); }
struct Guard {
    bool& value;
    explicit Guard(bool& v) : value(v) { value = true; }
    ~Guard() { value = false; }
};
}
struct HarborSourceStartup::State {
    HarborSourceStartupBinding binding;
    BattleColdInitCursor cold;
    BattleUnitSetupCursor setup;
    BattleMenuResourceCursor menu;
    std::unique_ptr<HarborSourceUnitConstruction> units;
    bool advanceUnit = false;
    std::optional<BattleColdInitRequest> heldColdChild;
    std::shared_ptr<const uint8_t> battleLifetime;
    uint32_t battle = 0, information = 0, pending = 0, displayBattle = 0;
    unsigned displayPhase = 0;
    bool begun = false, coldDone = false, setupDone = false, published = false;
    mutable bool invalid = false, checking = false;
    bool current() const {
        if (invalid || checking) return false;
        Guard guard(checking);
        if (!binding.current()) { invalid = true; return false; }
        if (!binding.pool->current()) { invalid = true; return false; }
        if (battleLifetime && binding.pool->allocationLifetime(battle) != battleLifetime) {
            invalid = true; return false;
        }
        if (published && word(BattleColdPointerCell) != battle) invalid = true;
        return !invalid;
    }
    BattleActorServices actors() const { return nativeBattleActorServices(*binding.vm); }
    uint32_t word(uint32_t at) const {
        uint32_t value; required(binding.vm->memory.readWord(at, value)); return value;
    }
    uint16_t half(uint32_t at) const {
        uint16_t value; required(binding.vm->memory.readHalf(at, value)); return value;
    }
    float real(uint32_t at) const { const auto bits = word(at); float value; std::memcpy(&value, &bits, 4); return value; }
    void real(uint32_t at, float value) {
        uint32_t bits; std::memcpy(&bits, &value, 4); required(binding.vm->memory.writeWord(at, bits));
    }
    PackedUnitResourceServices resources() {
        PackedUnitResourceServices s; s.memory = actors(); s.user = this;
        s.pouch = [](void* p) { return static_cast<State*>(p)->word(0x8041eb00); };
        s.partyIndex = [](void*, int32_t kind) { return battlePartyIndex(kind); };
        return s;
    }
    bool displayInit() {
        if (!displayPhase) { displayBattle = word(BattleColdPointerCell); displayPhase = 1; }
        if (displayPhase == 1) {
            BattleCameraRequestServices camera;
            camera.positions = nativeBattlePartPositionServices(*binding.vm);
            camera.values = camera.positions.unit.values;
            initializeBattleCamera(displayBattle, camera);
            displayPhase = 2;
        }
        if (displayPhase == 2) {
            pending = 0x80119da0;
            if (!initializePackedBattleMenuResources(binding.menu, menu)) return false;
            displayPhase = 3;
        }
        if (displayPhase == 3) {
            const auto& m = actors().memory;
            m.storeWord(m.user, displayBattle + 0xef8, word(displayBattle + 0xef8) | 2);
            displayPhase = 4;
        }
        pending = 0;
        return true;
    }
    std::optional<uint32_t> coldCall(const BattleColdInitRequest& q) {
        pending = q.function;
        if (!current()) return {};
        if (q.function == 0x800f8934) {
            const auto allocated = binding.pool->allocate(binding.pool->root(), q.arguments[0]);
            if (!allocated) return {};
            battle = *allocated;
            battleLifetime = binding.pool->allocationLifetime(battle);
            return allocated;
        }
        if (!published) {
            // Original BattleInit stores its allocation to the source global
            // before its first memset child. This owner never writes that cell.
            if (!battleLifetime || word(BattleColdPointerCell) != battle) return {};
            published = true;
        }
        const auto a = actors();
        if (const auto closed = initializePackedBattleColdStorage(q.function, q.arguments[0], a)) return closed;
        if (q.function == 0x800050b4)
            return eventFillMemory(q.arguments[0], int32_t(q.arguments[1]), q.arguments[2], a.memory);
        if (q.function == 0x800f8a08) {
            packedBattleSetSequence(q.arguments[0], q.arguments[1], q.arguments[2], a.memory);
            return 0;
        }
        if (q.function == 0x801030f8) return displayInit() ? std::optional<uint32_t>(0) : std::nullopt;
        if (!heldColdChild) heldColdChild = q;
        // Stable child identity even though the cold parent's request is a
        // read-only value transport. The underlying child owns its continuation.
        const auto result = binding.coldChild ? binding.coldChild(*heldColdChild) : std::nullopt;
        if (!current() || !result) return {};
        heldColdChild.reset();
        return result;
    }
    std::optional<BattleUnitSetupOutput> unitCall(const BattleUnitSetupRequest& q) {
        pending = q.function;
        if (!current()) return {};
        BattleUnitSetupOutput out;
        switch (q.function) {
        case 0x80128fe0: {
            if (!units) {
                HarborSourceUnitConstructionBinding entry;
                entry.vm = binding.vm; entry.pool = binding.pool; entry.battle = battle;
                entry.animation = binding.animation; entry.resources = binding.poseResources;
                entry.lifetime = binding.lifetime; entry.current = [this] { return current(); };
                units = HarborSourceUnitConstruction::create(std::move(entry));
                if (!units) return {};
            }
            // UnitSetup invokes the same group callsite repeatedly. Completion
            // of the preceding child, rather than callsite equality, separates
            // those distinct source constructor invocations.
            if (advanceUnit) { if (!units->next()) return {}; advanceUnit = false; }
            const auto value = units->resume(q.integers[0]);
            if (!value) { pending = units->pending(); return {}; }
            out.value = *value; advanceUnit = true; return out;
        }
        case 0x800d5eb0: out.value = word(0x8041eb00); return out;
        case 0x801289b0:
            for (unsigned axis = 0; axis < 3; ++axis) out.position[axis] = real(q.integers[0] + 0x30 + 4 * axis);
            return out;
        case 0x801289a0: case 0x80128c6c: {
            const auto offset = q.function == 0x801289a0 ? 0x30u : 0x3cu;
            for (unsigned axis = 0; axis < 3; ++axis) real(q.integers[0] + offset + 4 * axis, q.position[axis]);
            return out;
        }
        case 0x80127134:
            importPackedUnitResources(battle, q.integers[0], resources());
            return out;
        case 0x800f831c: {
            // Native no-effects admission for the complete source item0 arm.
            // Nonzero equipment retains its genuine external source provider.
            const auto mode = q.integers[1];
            bool noItems = true;
            if (!q.integers[0] || word(q.integers[0] + 8) == 220) {
                noItems = true; // Source null/system returns before equipOne.
            } else if (mode & 6) {
                const auto pouch = word(0x8041eb00);
                for (unsigned slot = 0; slot < 200; ++slot)
                    if (half(pouch + 0x38a + 2 * slot)) noItems = false;
            } else noItems = q.integers[2] == 0;
            if (!noItems) break;
            PackedUnitEquipmentServices equipment; equipment.resources = resources();
            equipment.equipOne = [](void*, uint32_t, int32_t, int32_t item) { required(equipPackedNoItem(item)); };
            equipPackedUnitItems(q.integers[0], int32_t(mode), int32_t(q.integers[2]), equipment);
            return out;
        }
        default: break;
        }
        const auto result = binding.unitChild ? binding.unitChild(q) : std::nullopt;
        return current() ? result : std::nullopt;
    }
};
HarborSourceStartup::HarborSourceStartup() = default;
HarborSourceStartup::~HarborSourceStartup() = default;
std::unique_ptr<HarborSourceStartup> HarborSourceStartup::create(HarborSourceStartupBinding b) {
    if (!b.vm || !b.pool || !b.lifetime || !b.current) return {};
    auto out = std::unique_ptr<HarborSourceStartup>(new HarborSourceStartup);
    out->state_ = std::make_unique<State>(); out->state_->binding = std::move(b);
    return out->current() ? std::move(out) : nullptr;
}
bool HarborSourceStartup::resumeCold(uint32_t information) {
    if (invoking_ || !current()) return false;
    auto& s = *state_;
    if (s.begun && s.information != information) return false;
    if (s.coldDone) return true;
    Guard guard(invoking_); s.begun = true; s.information = information;
    BattleColdInitServices services; services.memory = s.actors().memory;
    services.lifetime = s.binding.lifetime; services.user = &s;
    services.current = [](void* p) { return static_cast<State*>(p)->current(); };
    services.call = [](void* p, const BattleColdInitRequest& q) { return static_cast<State*>(p)->coldCall(q); };
    s.coldDone = resumePackedBattleColdInit(information, services, s.cold);
    if (s.coldDone) s.pending = 0;
    return s.coldDone;
}
bool HarborSourceStartup::resumeUnitSetup(uint32_t battle) {
    if (invoking_ || !current() || !state_->coldDone || battle != state_->battle) return false;
    auto& s = *state_; if (s.setupDone) return true;
    Guard guard(invoking_);
    BattleUnitSetupContinuationServices services; services.actors = s.actors();
    services.lifetime = s.binding.lifetime; services.user = &s;
    services.current = [](void* p) { return static_cast<State*>(p)->current(); };
    services.call = [](void* p, const BattleUnitSetupRequest& q) { return static_cast<State*>(p)->unitCall(q); };
    s.setupDone = resumePackedBattleUnitSetup(battle, services, s.setup);
    if (s.setupDone) s.pending = 0;
    return s.setupDone;
}
bool HarborSourceStartup::coldComplete() const { return state_->coldDone; }
bool HarborSourceStartup::unitSetupComplete() const { return state_->setupDone; }
uint32_t HarborSourceStartup::battleAddress() const { return state_->battle; }
uint32_t HarborSourceStartup::pending() const { return state_->pending; }
bool HarborSourceStartup::current() const { return state_ && state_->current(); }
}

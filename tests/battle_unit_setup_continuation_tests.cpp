#include "battle_unit_setup_continuation.hpp"
#include "battle_unit_setup.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Battle = 0x1000, Information = 0x20000, Npc = 0x21000;
constexpr uint32_t Group = 0x22000, Pouch = 0x23000, Definitions = 0x24000;
using Store = std::tuple<uint32_t, uint32_t, unsigned>;
struct Fixture {
    std::map<uint32_t, uint8_t> bytes;
    std::vector<Store> stores;
    std::vector<BattleUnitSetupRequest> calls;
    BattleUnitSetupCursor cursor;
    std::shared_ptr<const uint8_t> life = std::make_shared<const uint8_t>(0);
    size_t hold = SIZE_MAX;
    unsigned attempts = 0, entries = 0;
    bool reenter = false, alive = true, grow = false;
    uint32_t read(uint32_t address, unsigned size) {
        uint32_t value = 0;
        for (unsigned n = 0; n < size; ++n) value = (value << 8) | bytes[address + n];
        return value;
    }
    void write(uint32_t address, uint32_t value, unsigned size = 4) {
        stores.emplace_back(address, value, size);
        for (unsigned n = 0; n < size; ++n) bytes[address + n] = uint8_t(value >> ((size - n - 1) * 8));
    }
    Fixture(bool partner = true, int32_t count = 2) {
        write(Battle + 0x2738, Information); write(Information + 0xc, Npc);
        write(Information + 4, partner ? 224 : 0); write(Npc + 0x18, Group);
        write(Group, uint32_t(count)); write(Group + 4, Definitions);
        write(Npc + 0x1c, 8); write(Npc + 0x20, 9);
        write(Npc + 0x5c, 0); write(Npc + 0x60, 77);
        write(Npc + 0x3c, 0xabc); write(Npc + 0x40, 0xdef);
        real(0x80422178, -140); real(0x8042217c, 5);
        for (unsigned n = 0; n < 12; ++n) write(Pouch + 0x38a + 2 * n, 308 + n, 2);
        stores.clear();
    }
    void real(uint32_t address, float value) { uint32_t bits; std::memcpy(&bits, &value, 4); write(address, bits); }
    BattleActorServices actors() {
        BattleActorServices a; a.user = this; a.memory.user = this;
        a.byte = [](void* p, uint32_t at) { return uint8_t(static_cast<Fixture*>(p)->read(at, 1)); };
        a.memory.word = [](void* p, uint32_t at) { return static_cast<Fixture*>(p)->read(at, 4); };
        a.memory.half = [](void* p, uint32_t at) { return uint16_t(static_cast<Fixture*>(p)->read(at, 2)); };
        a.memory.storeWord = [](void* p, uint32_t at, uint32_t v) { static_cast<Fixture*>(p)->write(at, v); };
        a.memory.storeHalf = [](void* p, uint32_t at, uint16_t v) { static_cast<Fixture*>(p)->write(at, v, 2); };
        a.memory.storeByte = [](void* p, uint32_t at, uint8_t v) { static_cast<Fixture*>(p)->write(at, v, 1); };
        return a;
    }
    BattleUnitSetupOutput child(const BattleUnitSetupRequest& q) {
        calls.push_back(q);
        switch (q.function) {
        case 0x800d5eb0: return {Pouch, {}};
        case 0x80128fe0: {
            const auto unit = 0x30000 + 0x1000 * entries++;
            write(unit + 0x108, 11, 2); write(unit + 0x10e, 6, 2);
            if (grow && q.integers[0] == Definitions) write(Group, 2);
            return {unit, {}};
        }
        case 0x801289b0: return {0, {4, 17, 9}};
        case 0x801289a0: case 0x80128c6c:
            assert(q.position == (std::array<float, 3>{-140, 17, 5})); return {};
        case 0x800f831c: case 0x80127134: return {};
        default: assert(false); return {};
        }
    }
    BattleUnitSetupContinuationServices services() {
        BattleUnitSetupContinuationServices s; s.actors = actors(); s.lifetime = life;
        s.user = this; s.current = [](void* p) { return static_cast<Fixture*>(p)->alive; };
        s.call = [](void* p, const BattleUnitSetupRequest& q) -> std::optional<BattleUnitSetupOutput> {
            auto& f = *static_cast<Fixture*>(p);
            if (f.reenter) {
                assert(!f.cursor.reset());
                assert(!resumePackedBattleUnitSetup(Battle, f.services(), f.cursor));
            }
            if (f.calls.size() == f.hold && f.attempts++ < 2) return {};
            return f.child(q);
        }; return s;
    }
    void run() {
        for (unsigned n = 0; n < 100; ++n)
            if (resumePackedBattleUnitSetup(Battle, services(), cursor)) return;
        assert(false);
    }
    void reference() {
        BattleUnitSetupServices s; s.memory = actors(); s.user = this;
        s.storeByte = s.memory.memory.storeByte;
        s.pouch = [](void* p) { return static_cast<Fixture*>(p)->child({0x800d5eb0, 0, {}, {}}).value; };
        s.entry = [](void* p, uint32_t d) { return static_cast<Fixture*>(p)->child({0x80128fe0, 0, {d, 0, 0}, {}}).value; };
        s.equip = [](void* p, uint32_t u, int32_t mode, int32_t item) { static_cast<Fixture*>(p)->child({0x800f831c, 0, {u, uint32_t(mode), uint32_t(item)}, {}}); };
        s.pouchParameters = [](void* p, uint32_t u) { static_cast<Fixture*>(p)->child({0x80127134, 0, {u, 0, 0}, {}}); };
        s.basePosition = [](void* p, uint32_t u) { return static_cast<Fixture*>(p)->child({0x801289b0, 0, {u, 0, 0}, {}}).position; };
        s.setBasePosition = [](void* p, uint32_t u, const std::array<float, 3>& v) { static_cast<Fixture*>(p)->child({0x801289a0, 0, {u, 0, 0}, v}); };
        s.setPosition = [](void* p, uint32_t u, const std::array<float, 3>& v) { static_cast<Fixture*>(p)->child({0x80128c6c, 0, {u, 0, 0}, v}); };
        assert(packedBattleUnitSetup(Battle, s) == 1);
    }
};
void equal(const Fixture& a, const Fixture& b, bool pc) {
    assert(a.bytes == b.bytes && a.stores == b.stores && a.calls.size() == b.calls.size());
    for (size_t n = 0; n < a.calls.size(); ++n) {
        const auto& x = a.calls[n]; const auto& y = b.calls[n];
        assert(x.function == y.function && x.integers == y.integers && x.position == y.position);
        if (pc) assert(x.callsite == y.callsite);
    }
}
void differential() {
    for (bool partner : {false, true}) for (int32_t count : {-1, 0, 1, 2}) {
        Fixture a(partner, count), b(partner, count);
        a.stores.clear(); b.stores.clear(); a.reference(); b.run(); equal(a, b, false);
    }
}
void heldChildren() {
    Fixture reference; reference.run();
    for (size_t at = 0; at < reference.calls.size(); ++at) {
        Fixture f; f.hold = at; f.reenter = true; f.run();
        assert(f.attempts == 3); equal(f, reference, true);
    }
}
void sourceEdges() {
    Fixture f(false, 1); f.grow = true; f.run();
    assert(f.entries == 4); // Two fixed actors plus live-count growth to two opponents.
    assert(f.read(Battle + 0x163f4, 4) == 0x1f9f);
    size_t maskStores = 0;
    for (const auto& store : f.stores) if (std::get<0>(store) == Battle + 0x163f4) ++maskStores;
    assert(maskStores == 12); // One clear plus eleven cases;314 has no write.
    Fixture malformed; malformed.write(Information + 4, 1); malformed.run();
    bool entered = false;
    for (const auto& q : malformed.calls) if (q.callsite == 0x800f8b60) {
        assert(q.integers[0] == 0x80363790 + uint32_t(1 - 224) * 0x30); entered = true;
    }
    assert(entered);
    Fixture held; held.hold = 3;
    assert(!resumePackedBattleUnitSetup(Battle, held.services(), held.cursor));
    const auto prefix = held.stores;
    auto different = held.services(); different.lifetime = std::make_shared<const uint8_t>(0);
    assert(!resumePackedBattleUnitSetup(Battle, different, held.cursor));
    held.alive = false; assert(!resumePackedBattleUnitSetup(Battle, held.services(), held.cursor));
    held.alive = true; assert(!resumePackedBattleUnitSetup(Battle, held.services(), held.cursor));
    assert(held.stores == prefix && held.attempts == 1);
}
}
int main() {
    differential(); heldChildren(); sourceEdges();
    std::cout << "retained UnitSetup differential/holds/live group/nonzero party/314 no-write PASS\n";
}

#include "battle_cold_init_continuation.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Work = 0x84000000, Information = 0x84500020;
using Store = std::tuple<uint32_t, uint32_t, unsigned>;
struct Fixture {
    BattleColdInitCursor cursor;
    std::shared_ptr<const uint8_t> lifetime = std::make_shared<const uint8_t>(0);
    std::vector<BattleColdInitRequest> calls;
    std::vector<Store> stores;
    std::map<uint32_t, uint8_t> bytes;
    size_t hold = SIZE_MAX;
    unsigned attempts = 0;
    bool alive = true, reenter = false;
    uint32_t allocation = Work;
    void store(uint32_t address, uint32_t value, unsigned count) {
        stores.emplace_back(address, value, count);
        for (unsigned n = 0; n < count; ++n)
            bytes[address + n] = uint8_t(value >> ((count - n - 1) * 8));
    }
    BattleColdInitServices services() {
        BattleColdInitServices s;
        s.memory.user = this;
        s.memory.storeWord = [](void* p, uint32_t a, uint32_t v) { static_cast<Fixture*>(p)->store(a, v, 4); };
        s.memory.storeHalf = [](void* p, uint32_t a, uint16_t v) { static_cast<Fixture*>(p)->store(a, v, 2); };
        s.lifetime = lifetime;
        s.user = this;
        s.current = [](void* p) { return static_cast<Fixture*>(p)->alive; };
        s.call = [](void* p, const BattleColdInitRequest& q) -> std::optional<uint32_t> {
            auto& f = *static_cast<Fixture*>(p);
            if (f.reenter) {
                assert(!f.cursor.reset());
                assert(!resumePackedBattleColdInit(Information, f.services(), f.cursor));
            }
            if (f.calls.size() == f.hold && f.attempts++ < 2) return {};
            f.calls.push_back(q);
            if (q.function == 0x800f8934) {
                assert(q.argumentCount == 1 && q.arguments[0] == 0x19098);
                return f.allocation;
            }
            if (q.function == 0x800050b4) {
                assert(q.argumentCount == 3 && q.arguments[1] == 0);
                for (uint32_t n = 0; n < q.arguments[2]; ++n) f.bytes[q.arguments[0] + n] = 0;
            }
            // Void original children may complete with scalar zero.
            return 0;
        };
        return s;
    }
    void run() {
        for (unsigned n = 0; n < 100; ++n)
            if (resumePackedBattleColdInit(Information, services(), cursor)) return;
        assert(false);
    }
};
void compare(const Fixture& a, const Fixture& b) {
    assert(a.stores == b.stores && a.bytes == b.bytes && a.calls.size() == b.calls.size());
    for (size_t n = 0; n < a.calls.size(); ++n) {
        const auto& x = a.calls[n]; const auto& y = b.calls[n];
        assert(x.function == y.function && x.callsite == y.callsite &&
               x.arguments == y.arguments && x.argumentCount == y.argumentCount);
    }
}
void originalOrder() {
    Fixture f;
    f.run();
    assert(!f.cursor.active());
    const std::vector<uint32_t> expected = {
        0x800f8934, 0x800050b4, 0x8011a48c, 0x80216908, 0x801aa228,
        0x800fa62c, 0x800f7af0, 0x80195a94, 0x801030f8, 0x800f8a08,
        0x800f8a08, 0x800050b4, 0x80129694, 0x800f7c20, 0x80139780,
        0x801a83f8, 0x8020507c};
    assert(f.calls.size() == expected.size());
    for (size_t n = 0; n < expected.size(); ++n) assert(f.calls[n].function == expected[n]);
    assert(f.calls[1].arguments == (std::array<uint32_t, 3>{Work, 0, 0x19098}));
    assert(f.calls[5].argumentCount == 1 && f.calls[5].arguments[0] == Work);
    assert(f.calls[9].arguments == (std::array<uint32_t, 3>{Work, 0, 0}));
    assert(f.calls[10].arguments == (std::array<uint32_t, 3>{Work, 1, 0x01000000}));
    assert(f.calls[11].arguments == (std::array<uint32_t, 3>{Work + 0x19060, 0, 16}));
    assert(f.calls[13].argumentCount == 1 && f.calls[13].arguments[0] == Work);
    std::vector<Store> stores = {{BattleColdPointerCell, Work, 4},
        {Work + 0x19070, 0xffffffff, 4}, {Work + 0x424, 0xffffffff, 4},
        {Work + 0x2738, Information, 4}};
    for (auto offset : {0xf28, 0x1720, 0x1bb4, 0x1bb8, 0x1bc0, 0x1bc4,
            0x1bcc, 0x1bd0, 0x1bd8, 0x1bdc, 0x1be4, 0x1be8, 0x1bf0,
            0x1bf4, 0x1bfc, 0x1c00, 0x1c08, 0x1c0c, 0x1c14, 0x1c18,
            0x1c20, 0x1c24, 0x1c78}) stores.emplace_back(Work + offset, 0, 4);
    stores.emplace_back(Work, 0, 2);
    stores.emplace_back(Work + 0xef4, 0, 4);
    assert(f.stores == stores);
    // Original sentinel is just outside the second 16-byte clear.
    assert(f.bytes.at(Work + 0x19070) == 255);
}
void waits() {
    Fixture reference; reference.run();
    for (size_t at = 0; at < reference.calls.size(); ++at) {
        Fixture f; f.hold = at; f.reenter = true; f.run();
        assert(f.attempts == 3);
        compare(f, reference);
    }
}
void ownership() {
    Fixture f; f.hold = 4;
    assert(!resumePackedBattleColdInit(Information, f.services(), f.cursor));
    assert(f.cursor.pending() == 0x801aa228 && f.cursor.active());
    const auto prefix = f.stores;
    auto changed = f.services(); changed.lifetime = std::make_shared<const uint8_t>(0);
    assert(!resumePackedBattleColdInit(Information, changed, f.cursor));
    assert(!resumePackedBattleColdInit(Information + 4, f.services(), f.cursor));
    assert(f.stores == prefix && f.attempts == 1);
    f.alive = false;
    assert(!resumePackedBattleColdInit(Information, f.services(), f.cursor));
    f.alive = true;
    assert(!resumePackedBattleColdInit(Information, f.services(), f.cursor));
    assert(f.attempts == 1 && f.stores == prefix);
    assert(f.cursor.reset() && !f.cursor.active());
    Fixture nullAllocation; nullAllocation.allocation = 0; nullAllocation.run();
    assert(nullAllocation.calls[1].arguments[0] == 0);
    assert(nullAllocation.stores[0] == Store(BattleColdPointerCell, 0, 4));
}
}
int main() {
    originalOrder(); waits(); ownership();
    std::cout << "retained original cold BattleInit ordering/17 held children/lifetime PASS\n";
}

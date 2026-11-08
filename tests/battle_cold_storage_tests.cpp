#include "battle_cold_storage.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Battle = 0x1000;
struct Fixture {
    EventNativeRuntime vm{0x81000000, {}};
    std::vector<std::tuple<uint32_t, uint32_t, unsigned>> writes;
    unsigned globalReads = 0;
    bool changing = false;
    Fixture() {
        assert(vm.memory.registerRam(Battle, std::vector<uint8_t>(0x30000, 0xa5)));
        assert(vm.memory.registerRam(0x8041eb90, std::vector<uint8_t>(4)));
        assert(vm.memory.registerRam(0x80422174, std::vector<uint8_t>(4)));
        assert(vm.memory.writeWord(0x8041eb90, Battle));
    }
    uint32_t word(uint32_t a) const { uint32_t v; assert(vm.memory.readWord(a, v)); return v; }
    uint8_t byte(uint32_t a) const { uint8_t v; assert(vm.memory.readByte(a, v)); return v; }
    BattleActorServices services() {
        BattleActorServices a; auto& m = a.memory; m.user = this;
        m.word = [](void* p, uint32_t at) {
            auto& f = *static_cast<Fixture*>(p);
            if (at == 0x8041eb90) {
                const auto index = f.globalReads++;
                if (f.changing) return Battle + 0x4000 * index;
            }
            return f.word(at);
        };
        m.storeWord = [](void* p, uint32_t at, uint32_t v) {
            auto& f = *static_cast<Fixture*>(p); f.writes.emplace_back(at, v, 4); assert(f.vm.memory.writeWord(at, v));
        };
        m.storeHalf = [](void* p, uint32_t at, uint16_t v) {
            auto& f = *static_cast<Fixture*>(p); f.writes.emplace_back(at, v, 2); assert(f.vm.memory.writeHalf(at, v));
        };
        m.storeByte = [](void* p, uint32_t at, uint8_t v) {
            auto& f = *static_cast<Fixture*>(p); f.writes.emplace_back(at, v, 1); assert(f.vm.memory.writeByte(at, v));
        };
        return a;
    }
};
void pad() {
    Fixture f; assert(initializePackedBattleColdStorage(0x8011a38c, Battle, f.services()) == 0);
    for (unsigned i = 0; i < 0x1fc; ++i) {
        const bool cleared = i < 11 || (i >= 12 && i < 23) || (i >= 0x18 && i < 0x180) || i == 0x1f8 || i == 0x1f9;
        assert(f.byte(Battle + i) == (cleared ? 0 : 0xa5));
    }
    assert(f.writes.size() == 112 && f.globalReads == 0);
    assert(f.writes[0] == std::make_tuple(Battle, 0u, 2u));
    assert(f.writes[20] == std::make_tuple(Battle + 0x18, 0u, 4u));
    assert(f.writes[21] == std::make_tuple(Battle + 0x90, 0u, 4u));
    assert(f.writes[22] == std::make_tuple(Battle + 0x108, 0u, 4u));
    Fixture general; general.changing = true;
    assert(initializePackedBattleColdStorage(0x8011a48c, 0, general.services()) == 0);
    assert(general.globalReads == 4);
    for (unsigned i = 0; i < 4; ++i) assert(general.word(Battle + 0x4000 * i + 0xf2c + 0x1fc * i) == 0);
    Fixture action;
    assert(initializePackedBattleColdStorage(0x800fa62c, Battle, action.services()) == 0);
    assert(action.word(Battle + 0x1ca0) == 0 && action.word(Battle + 0x1cac) == 0 && action.word(Battle + 0x1f18) == 0xffffffff);
    assert(action.word(Battle + 0xf2c) == 0xa5a5a5a5 && action.byte(Battle + 0x1d1c + 0xb) == 0xa5);
}
void records() {
    Fixture queue; assert(initializePackedBattleColdStorage(0x800f7af0, 0, queue.services()) == 0);
    assert(queue.globalReads == 1 && queue.writes.size() == 128);
    for (unsigned i = 0; i < 64; ++i) assert(queue.word(Battle + 0x16f5c + 8 * i) == 0xffffffff && queue.word(Battle + 0x16f60 + 8 * i) == 0);
    Fixture units; assert(initializePackedBattleColdStorage(0x80129694, 0, units.services()) == 1);
    assert(units.globalReads == 1 && units.writes.size() == 64 && units.word(Battle + 0x120) == 0xa5a5a5a5);
    Fixture icons; assert(initializePackedBattleColdStorage(0x801aa228, 0, icons.services()) == 0);
    assert(icons.byte(Battle + 0x182cb) == 0xa5 && icons.byte(Battle + 0x182cc) == 0 && icons.byte(Battle + 0x18c8b) == 0 && icons.byte(Battle + 0x18c8c) == 0xa5);
    Fixture messages; assert(initializePackedBattleColdStorage(0x80195a94, 0, messages.services()) == 0);
    for (unsigned i = 0; i < 600; ++i) assert(messages.byte(Battle + 0x18da0 + i) == (i % 12 == 0 && i / 12 < 28 ? 255 : 0));
    assert(messages.byte(Battle + 0x18da0 + 600) == 0xa5);
    Fixture party; assert(initializePackedBattleColdStorage(0x800f7c20, Battle, party.services()) == 0);
    for (unsigned n = 0; n < 7; ++n) {
        const auto at = Battle + 0x162a0 + 44 * n;
        for (unsigned i = 0; i < 44; ++i) assert(party.byte(at + i) == (i == 34 || i == 35 ? 0xa5 : 0));
    }
    Fixture absent;
    assert(!initializePackedBattleColdStorage(0x801030f8, Battle, absent.services()));
    assert(absent.writes.empty() && absent.globalReads == 0);
    assert(initializePackedBattleColdStorage(0x80216908, 0, {}) == 0);
}
}
int main() { pad(); records(); std::cout << "actual cold storage/pad raw preservation/ordered banks/partial records PASS\n"; }

#include "harbor_source_startup.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_event_callbacks.hpp"
#include "fixtures/host_event_cache.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    EventNativeRuntime vm(0x81000000, {});
    test::HostEventCache publication(vm);
    EventScriptAssets original;
    assert(original.load("romfs/harbor-tutorial-events.pmevt") && vm.memory.importScripts(original));
    assert(vm.memory.registerRam(BattleColdPointerCell, std::vector<uint8_t>(4)));
    constexpr uint32_t Information = 0x85000000;
    // Test-owned field information domain: type0 is an actual source camera
    // branch. This fixture is not a native field event or original story trace.
    assert(vm.memory.registerRam(Information, std::vector<uint8_t>(16)));
    auto pool = HarborSourceMapPool::bind(vm.memory, vm.memoryBinding.services()); assert(pool);
    publication.requireBattlePoolPublication();
    auto files = std::make_shared<BattleMenuFileCache>();
    assert(files->load("romfs/battle-class-common.tpl") && files->bind(vm.memory));
    BattleMenuFileBinding menu(files, nativeBattleActorServices(vm));
    bool alive = true;
    unsigned requests = 0;
    const BattleColdInitRequest* first = nullptr;
    HarborSourceStartupBinding binding; binding.vm = &vm; binding.pool = pool.get(); binding.menu = menu.services();
    binding.lifetime = std::make_shared<const uint8_t>(0); binding.current = [&] { return alive; };
    binding.coldChild = [&](const BattleColdInitRequest& q) -> std::optional<uint32_t> {
        assert(q.function == 0x80139780 && q.callsite == 0x800f901c);
        if (first) assert(first == &q);
        first = &q;
        ++requests;
        return {}; // Real stage provider is absent; never declare it complete.
    };
    auto startup = HarborSourceStartup::create(binding); assert(startup);
    assert(!startup->resumeCold(Information) && !startup->coldComplete());
    assert(startup->pending() == 0x80139780 && requests == 1);
    const auto battle = startup->battleAddress();
    auto word = [&](uint32_t at) { uint32_t v; assert(vm.memory.readWord(at, v)); return v; };
    assert(battle == pool->root() + 32 && word(BattleColdPointerCell) == battle);
    assert(word(battle + 0x2738) == Information && word(battle + 0x19070) == 0xffffffff);
    assert(word(battle + 0x424) == 0xffffffff && word(battle + 0xf0c) == 0x01000000);
    assert(word(battle + 0x2760) == 14 && word(battle + 0xef8) == 2);
    assert(word(battle + 0x1f18) == 0xffffffff && word(battle + 0x16f5c) == 0xffffffff);
    const auto entry = word(battle + 0x163f8);
    assert(entry && (word(entry) & 0xffff) == 1);
    assert(!startup->resumeCold(Information) && requests == 2 && (word(entry) & 0xffff) == 1);
    assert(!startup->resumeCold(Information + 4) && requests == 2);
    assert(!startup->resumeUnitSetup(battle));
    alive = false; assert(!startup->resumeCold(Information));
    alive = true; assert(!startup->resumeCold(Information) && requests == 2);
    startup.reset();
    assert(files->release(entry)); // Actual resource owner, independent of parent cursor teardown.
    std::cout << "native source cold allocation/storage/camera/menu exact held-stage boundary PASS\n";
}

#include "harbor_tutorial_packed.hpp"
#include <cassert>
using namespace ttyd;
int main(){
    EventNativeRuntime vm(0x81000000,{});
    HarborTutorialPacked packed(vm);
    // No legacy actor/menu/global storage is allowed to appear as a side
    // effect of native callback configuration.
    const auto unmapped=[&]{
        assert(vm.memory.unmappedRange(0x8041eb90,4));
        assert(vm.memory.unmappedRange(HarborTutorialPacked::Battle,0x19400));
        assert(vm.memory.unmappedRange(HarborTutorialPacked::Units,0x4000));
        assert(vm.memory.unmappedRange(HarborTutorialPacked::Parts,0x4000));
        assert(vm.memory.unmappedRange(HarborSourceMapPool::ArenaAddress,HarborSourceMapPool::ArenaBytes));
        assert(vm.memory.unmappedRange(0x8041e808,20));
    };
    unmapped();assert(packed.registerEventServices());unmapped();
    for(const auto function:{0x80113f1cu,0x8010f458u,0x80105b40u,0x801101c4u,0x801100ccu})
        assert(packed.registry.contains(function));
    assert(!packed.registerEventServices());unmapped(); // Existing registry binds once.
    // Original GetHp/GetMaxHp callbacks subsequently use a relocated real
    // registry entry. Registration did not require or create fixed aliases.
    constexpr uint32_t Battle=0x84701000,Unit=0x84730000,Arguments=0x85000000;
    assert(vm.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));
    assert(vm.memory.registerRam(Battle,std::vector<uint8_t>(0x120)));
    assert(vm.memory.registerRam(Unit,std::vector<uint8_t>(0x300)));
    assert(vm.memory.registerRam(Arguments,std::vector<uint8_t>(8)));
    assert(vm.memory.writeWord(0x8041eb90,Battle) && vm.memory.writeWord(Battle+0x20+7*4,Unit));
    assert(vm.memory.writeHalf(Unit+0x10c,3) && vm.memory.writeHalf(Unit+0x108,10));
    std::array<EventEntry,1> events{};
    assert(vm.entryAddresses.registerEntries(0x82000000,events.data(),1));
    auto& event=events[0];event.owner=7;event.arguments=Arguments;
    assert(vm.memory.writeWord(Arguments,uint32_t(-2)) && vm.memory.writeWord(Arguments+4,uint32_t(-30000000)));
    int32_t result=0;
    assert(packed.registry.invoke(event,0x801101c4,1,result) && result==2 && event.locals[0]==3);
    assert(packed.registry.invoke(event,0x801100cc,0,result) && result==2 && event.locals[0]==10);
    assert(vm.memory.unmappedRange(HarborTutorialPacked::Battle,0x19400));
}

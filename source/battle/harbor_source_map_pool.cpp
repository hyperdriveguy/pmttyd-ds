#include "harbor_source_map_pool.hpp"
#include "battle_sequence_nested_pool.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_release.hpp"
#include "event_ram_access.hpp"

namespace ttyd {
namespace {
constexpr uint32_t MapGlobals = 0x8041e808;
constexpr uint32_t BattleRootCell = 0x8041e80c;
constexpr uint32_t HeaderBytes = 32;
constexpr uint32_t BattlePoolBytes = 0x64000;
void word(uint8_t* p, uint32_t value) {
    for (unsigned n = 0; n < sizeof(value); ++n) p[n] = uint8_t(value >> (24 - 8 * n));
}
bool backing(EventAddressSpace& memory, uint32_t address, const uint8_t* bytes, uint32_t size) {
    return memory.withRawRamAccess([&](EventAddressSpace::RawRamAccess& access) {
        return access.matchesBacking(address, size, bytes);
    });
}
}
HarborSourceMapPool::HarborSourceMapPool(EventAddressSpace& memory, const EventMemoryServices& services)
    : memory_(memory), services_(services), arena_(ArenaBytes) {
    // Same native cold primary-map policy as BattleSequenceStorage. This is
    // explicit host-owned storage, not a fabricated original SDK heap address.
    word(arena_.data() + 4, ArenaBytes - HeaderBytes);
    word(globals_.data(), ArenaAddress);
    word(globals_.data() + 16, ArenaBytes);
}
std::unique_ptr<HarborSourceMapPool> HarborSourceMapPool::bind(EventAddressSpace& memory,
                                                             const EventMemoryServices& services) {
    if (!services.word || !services.half || !services.storeWord || !services.storeHalf ||
        !services.storeByte || !services.flushBlock || !services.systemCall ||
        !memory.unmappedRange(ArenaAddress, ArenaBytes) ||
        !memory.unmappedRange(MapGlobals, 20)) return {};
    auto owner = std::unique_ptr<HarborSourceMapPool>(new HarborSourceMapPool(memory, services));
    if (!(owner->arenaMapped_ = memory.registerExternalRam(ArenaAddress, owner->arena_.data(), ArenaBytes))) return {};
    if (!(owner->globalsMapped_ = memory.registerExternalRam(MapGlobals, owner->globals_.data(), uint32_t(owner->globals_.size())))) return {};
    allocateEventBattleMapPool(services);
    uint32_t root = 0;
    if (!memory.readWord(BattleRootCell, root)) return {};
    owner->root_ = root;
    uint32_t size = 0;
    if (!root || !memory.readWord(root + 4, size) || size != BattlePoolBytes - HeaderBytes) return {};
    return owner;
}
HarborSourceMapPool::HarborSourceMapPool(EventAddressSpace& memory,const EventMemoryServices& services,std::shared_ptr<const BattleSequenceNestedPoolReceipt> receipt)
    : memory_(memory),services_(services),receipt_(std::move(receipt)) {}
std::unique_ptr<HarborSourceMapPool> HarborSourceMapPool::adopt(EventNativeRuntime& vm,std::shared_ptr<const BattleSequenceNestedPoolReceipt> receipt) {
    if(!receipt || !receipt->accepts(vm))return {};
    const auto services=vm.memoryBinding.services();
    if(!services.word||!services.half||!services.storeWord||!services.storeHalf||!services.storeByte||!services.flushBlock||!services.systemCall)return {};
    auto owner=std::unique_ptr<HarborSourceMapPool>(new HarborSourceMapPool(vm.memory,services,receipt));
    owner->root_=receipt->root();
    if(!owner->root_||!receipt->claim(vm))return {};
    return owner;
}
HarborSourceMapPool::~HarborSourceMapPool() {
    // The native parent retires every consumer before teardown. Never remove
    // another owner's replacement mapping at an address reused after retirement.
    allocations_.clear();
    if(receipt_)return; // Adopted storage belongs to the source process, never this facade.
    if (globalsMapped_ && backing(memory_, MapGlobals, globals_.data(), uint32_t(globals_.size())))
        memory_.unregisterExternalRam(MapGlobals);
    if (arenaMapped_ && backing(memory_, ArenaAddress, arena_.data(), ArenaBytes))
        memory_.unregisterExternalRam(ArenaAddress);
}
bool HarborSourceMapPool::current() const {
    if(receipt_)return receipt_->current();
    return globalsMapped_ && arenaMapped_ &&
        backing(memory_, MapGlobals, globals_.data(), uint32_t(globals_.size())) &&
        backing(memory_, ArenaAddress, arena_.data(), ArenaBytes);
}
uint32_t HarborSourceMapPool::root() const {
    uint32_t value = 0;
    return current() && memory_.readWord(BattleRootCell, value) && value == root_ ? value : 0;
}
std::optional<uint32_t> HarborSourceMapPool::allocate(uint32_t requestedRoot, uint32_t bytes) {
    if(invoking_)return {};
    const auto actualRoot = root();
    if (!actualRoot || requestedRoot != actualRoot) return {};
    invoking_=true;struct Invocation{bool&v;~Invocation(){v=false;}} invocation{invoking_};
    const auto address = eventMapAllocate(actualRoot, bytes, services_);
    if (address) allocations_.emplace(address, std::make_shared<const uint8_t>(0));
    return address;
}
bool HarborSourceMapPool::free(uint32_t requestedRoot, uint32_t address) {
    if(invoking_)return false;
    const auto actualRoot = root();
    if (!actualRoot || requestedRoot != actualRoot) return false;
    if (!address) return true;
    const auto it = allocations_.find(address);
    if (it == allocations_.end()) return false;
    invoking_=true;struct Invocation{bool&v;~Invocation(){v=false;}} invocation{invoking_};
    eventMapFree(actualRoot, address, services_);
    allocations_.erase(it);
    return true;
}
std::shared_ptr<const uint8_t> HarborSourceMapPool::allocationLifetime(uint32_t address) const {
    const auto it = allocations_.find(address);
    return current() && it != allocations_.end() ? it->second : nullptr;
}
}

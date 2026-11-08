#include "harbor_source_map_pool.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    struct Cache { unsigned lines = 0, barriers = 0; } cache;
    EventNativePlatform platform; platform.user = &cache;
    platform.flushBlock = [](void* p, uint32_t) { ++static_cast<Cache*>(p)->lines; };
    platform.systemCall = [](void* p) { ++static_cast<Cache*>(p)->barriers; };
    EventNativeRuntime vm(0x81000000, platform);
    auto pool = HarborSourceMapPool::bind(vm.memory, vm.memoryBinding.services());
    assert(pool && pool->current());
    assert(!HarborSourceMapPool::bind(vm.memory, vm.memoryBinding.services()));
    const auto root = pool->root();
    assert(root == HarborSourceMapPool::ArenaAddress + 32);
    assert(cache.lines == 0x64000 / 32 && cache.barriers == 1);
    auto wrong = pool->allocate(root + 32, 292); assert(!wrong);
    const auto work = pool->allocate(root, 0x19098);
    const auto ui = pool->allocate(root, 292);
    assert(work && *work == root + 32 && ui && *ui > *work);
    uint32_t value = 0;
    assert(vm.memory.readWord(*work, value) && value == 0);
    assert(vm.memory.writeWord(*ui, 0x12345678));
    const auto first = pool->allocationLifetime(*ui);
    assert(first && !pool->allocationLifetime(*ui + 4));
    assert(!pool->free(root + 32, *ui));
    assert(pool->free(root, *ui) && !pool->allocationLifetime(*ui));
    assert(!pool->free(root, *ui));
    const auto reused = pool->allocate(root, 292);
    const auto second = pool->allocationLifetime(*reused);
    assert(reused && *reused == *ui && second && second != first);
    assert(vm.memory.readWord(*reused, value) && value == 0);
    const auto zero = pool->allocate(root, 0);
    const auto nofit = pool->allocate(root, 0x64000);
    assert(zero && *zero == 0 && nofit && *nofit == 0);
    assert(pool->free(root, *reused) && pool->free(root, *work));
    assert(vm.memory.readWord(root + 4, value) && value == 0x63fe0);
    assert(vm.memory.writeWord(0x8041e80c, root + 32));
    assert(!pool->root() && !pool->allocate(root, 64));
    assert(vm.memory.writeWord(0x8041e80c, root));
    pool.reset();
    assert(!vm.memory.readWord(0x8041e80c, value));
    assert(!vm.memory.readWord(root, value));
    auto replacement = HarborSourceMapPool::bind(vm.memory, vm.memoryBinding.services());
    assert(replacement && replacement->root() == root);
    std::cout << "actual nested source map pool/allocation/free/reuse lifetime PASS\n";
}

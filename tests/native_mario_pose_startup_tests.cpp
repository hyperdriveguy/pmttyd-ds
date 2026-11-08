#include "event_heap_allocation.hpp"
#include "event_memory_opcodes.hpp"
#include "field_party_pose.hpp"
#include "native_mario_pose_startup.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace &m, uint32_t a) {
  uint32_t v;
  assert(m.readWord(a, v));
  return v;
}
EventNativePlatform platform() {
  EventNativePlatform p;
  p.flushBlock = [](void *, uint32_t) {};
  p.systemCall = [](void *) {};
  p.truncate = [](void *, float v) {
    assert(std::isfinite(v));
    return int32_t(v);
  };
  p.subtractDouble = [](void *, double a, double b) {
    volatile double v = a - b;
    return v;
  };
  p.roundDoubleFloat = [](void *, double v) {
    volatile float f = float(v);
    return f;
  };
  return p;
}
void actualSharedEntryAndBorrowedOwner() {
  assert(loadFieldPlayerPoseAssets("romfs"));
  const auto all = fieldPlayerPoseResources();
  std::array<std::shared_ptr<const BattlePoseResource>, 3> resources{
      {all[0], all[1], all[2]}};
  auto animation = NativeFieldAnimation::create(
      "romfs", NativeFileCache::create(), platform());
  assert(animation);
  assert(!NativeMarioPoseStartup::bind(animation,
                                       resources)); // No invented raw owner.
  auto &vm = animation->process().vm();
  auto &m = vm.memory;
  // TEST-ONLY caller: actual heap allocation/isolated backing, constructor
  // zero point and selector0. This is not a receipt for whole marioInit.
  constexpr uint32_t MarioBytes = 0x2f8, Global = 0x8041e900;
  const auto mario =
      eventHeapAllocate(0, MarioBytes, vm.memoryBinding.services());
  assert(mario);
  std::vector<uint8_t> backing;
  assert(m.extractRam(mario, MarioBytes, backing) &&
         m.registerRam(mario, std::move(backing)));
  assert(m.registerRam(Global, std::vector<uint8_t>(4)) &&
         m.writeWord(Global, mario));
  eventFillMemory(mario, 0, MarioBytes, vm.memoryBinding.services());
  assert(m.writeByte(mario + 0x3d, 0));
  auto owner = NativeMarioPoseStartup::bind(animation, resources);
  assert(owner && owner->current() && !owner->ready() &&
         owner->marioAddress() == mario);
  assert(owner->resume() && owner->ready() && !owner->pending());
  for (unsigned i = 0; i < 3; ++i) {
    const auto handle = word(m, mario + 0x22c + 4 * i);
    const auto *pose = animation->pool()->pose(int32_t(handle));
    assert(pose && pose->resource &&
           pose->resource->modelName == resources[i]->modelName &&
           pose->word(0xc) == 2);
  }
  const auto last = word(m, mario + 0x234);
  assert(owner->resume() && word(m, mario + 0x234) == last);
  // Backing identity is required even when all bytes are equal.
  std::vector<uint8_t> old;
  assert(m.unregisterRam(mario, &old));
  auto copied = old;
  assert(m.registerRam(mario, std::move(copied)));
  assert(!owner->current() && !owner->ready() && !owner->resume());
}
} // namespace
int main() { actualSharedEntryAndBorrowedOwner(); }

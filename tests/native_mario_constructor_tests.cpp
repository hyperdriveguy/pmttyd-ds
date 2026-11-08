#include "field_party_pose.hpp"
#include "native_mario_constructor.hpp"
#include "native_mario_pose_startup.hpp"
#include <cassert>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace &m, uint32_t a) {
  uint32_t v;
  assert(m.readWord(a, v));
  return v;
}
uint16_t half(EventAddressSpace &m, uint32_t a) {
  uint16_t v;
  assert(m.readHalf(a, v));
  return v;
}
uint8_t byte(EventAddressSpace &m, uint32_t a) {
  uint8_t v;
  assert(m.readByte(a, v));
  return v;
}
EventNativePlatform platform() {
  EventNativePlatform p;
  p.flushBlock = [](void *, uint32_t) {};
  p.systemCall = [](void *) {};
  p.truncate = [](void *, float v) { return int32_t(v); };
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
void actualConstructorAndPoseOwner() {
  assert(loadFieldPlayerPoseAssets("romfs"));
  auto animation = NativeFieldAnimation::create(
      "romfs", NativeFileCache::create(), platform());
  assert(animation);
  auto ui = NativeUiStartup::create(animation, "romfs");
  assert(ui && ui->resumeFont());
  auto owner = NativeMarioConstructor::create(ui, "romfs");
  assert(owner && owner->current() && !owner->ready());
  assert(!owner->sharedPouch());
  auto &m = animation->process().vm().memory;
  // Budget yields retain an actual partially constructed owner, never allocate
  // another Mario/pouch or restart the source parent.
  assert(!owner->initialize(8) && owner->busy());
  uint32_t gp = 0;
  assert(m.readWord(0x80415fd0, gp) && m.writeWord(gp + 0x16c, 2));
  assert(!owner->initialize() && owner->pending() == 0x800d5e8c);
  const auto heldMario = owner->marioAddress(),
             heldPouch = owner->pouchAddress();
  assert(heldMario && heldPouch && !owner->initialize() &&
         owner->marioAddress() == heldMario &&
         owner->pouchAddress() == heldPouch);
  assert(m.writeWord(gp + 0x16c, 1));
  unsigned resumes = 0;
  while (!owner->ready()) {
    assert(++resumes < 10000);
    owner->initialize(64);
  }
  const auto mario = owner->marioAddress(), pouch = owner->pouchAddress();
  assert(mario && pouch && mario != pouch && word(m, 0x8041e900) == mario &&
         word(m, 0x8041eb00) == pouch);
  assert(word(m, mario + 0x27c) == 0xffffffff &&
         word(m, mario + 0x280) == 0xffffffff);
  assert(word(m, mario + 0x22c) == 0xffffffff &&
         word(m, mario + 0x230) == 0xffffffff &&
         word(m, mario + 0x234) == 0xffffffff);
  assert(byte(m, mario + 0x245) == 255 && byte(m, mario + 0x246) == 255 &&
         word(m, mario + 0x134) == 4);
  assert(word(m, mario + 0x228) == 0x3f800000 &&
         word(m, mario + 0x188) == 0x40100000 &&
         word(m, mario + 0x1c0) == 0x41a00000);
  assert(word(m, mario + 0x18) == 0x8041ff98 && word(m, mario + 0xc) == 0x1001);
  assert(word(m, 0x8041ead8) == 0 && word(m, 0x8041eadc) == 0 &&
         word(m, 0x803dc248) == 0xffffffff);
  assert(half(m, pouch + 0x70) == 10 && half(m, pouch + 0x74) == 5 &&
         half(m, pouch + 0xa0) == 9 && byte(m, pouch + 0x99) == 1);
  const auto shared = owner->sharedPouch();
  assert(shared && m.hasRamBacking(pouch, shared->bytes.data(), shared->bytes.size()));
  shared->half(0x78, 321);
  assert(half(m, pouch + 0x78) == 321);
  assert(m.writeHalf(pouch + 0x70, 8) && shared->half(0x70) == 8);
  ProloguePouch snapshot;
  assert(owner->capturePouchSnapshot(snapshot));
  snapshot.half(0x70, 3);
  snapshot.half(0x78, 456);
  assert(owner->adoptPouchSnapshot(snapshot) && word(m, 0x8041eb00) == pouch &&
         half(m, pouch + 0x70) == 3 && half(m, pouch + 0x78) == 456);
  assert(owner->initialize() && owner->marioAddress() == mario &&
         owner->pouchAddress() == pouch && half(m, pouch + 0x70) == 3);
  // Window extracts later heap fragments. Actual isolated record backing and
  // all original field contents must survive (the prior Fade failure domain).
  assert(ui->resumeWindow() && owner->current());
  assert(animation->files()->advanceAsync(UINT32_MAX));
  const auto resources = fieldPlayerPoseResources();
  assert(owner->initializePoses({resources[0], resources[1], resources[2]}) && owner->posesReady());
  const auto handles=owner->poseHandles();
  assert(owner->initializePoses({resources[0], resources[1], resources[2]}) && owner->poseHandles()==handles);
  assert(!animation->pool()->pose(3)); // Completed wrapper must not repeat Entries.
  for (unsigned i = 0; i < 3; ++i)
    assert(animation->pool()->pose(int32_t(word(m, mario + 0x22c + 4 * i))));
  assert(
      !NativeMarioConstructor::create(ui, "romfs")); // No reset of live owners.
  std::vector<uint8_t> old;
  assert(m.unregisterRam(mario, &old));
  auto copied = old;
  assert(m.registerRam(mario, std::move(copied)));
  assert(!owner->current() && !owner->ready() && !owner->initialize() &&
         !owner->posesReady());
}
} // namespace
void pouchRetirement(bool foreign) {
  auto animation = NativeFieldAnimation::create("romfs", NativeFileCache::create(), platform());
  assert(animation);
  auto ui = NativeUiStartup::create(animation, "romfs");
  assert(ui && ui->resumeFont());
  auto owner = NativeMarioConstructor::create(ui, "romfs");
  assert(owner && owner->initialize());
  const auto shared = owner->sharedPouch();
  const auto address = owner->pouchAddress();
  assert(shared && address);
  shared->half(0x70, 6);
  auto &m = animation->process().vm().memory;
  if (foreign) {
    assert(m.unregisterExternalRam(address, shared->bytes.data()));
    std::vector<uint8_t> replacement(shared->bytes.begin(), shared->bytes.end());
    assert(m.registerRam(address, std::move(replacement)));
    assert(!owner->current() && !owner->sharedPouch());
  }
  owner.reset();
  uint16_t value = 0;
  assert(m.readHalf(address + 0x70, value) == foreign);
  if (foreign) assert(value == 6);
  // A retained typed snapshot remains safe after its source view is retired.
  assert(shared->half(0x70) == 6);
}
int main() { actualConstructorAndPoseOwner(); pouchRetirement(false); pouchRetirement(true); }

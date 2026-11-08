#include "battle_class_menu_messages.hpp"
#include "harbor_source_menu_factory.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
namespace {
// TEST-only admitted battle storage. Real VM/file/message owners are used;
// no assertion here claims original field initialization or native GPU work.
struct Runtime {
  EventNativeRuntime vm{0x81000000, {}};
  std::shared_ptr<HarborSourceMenuFactory> factory;
  std::shared_ptr<HarborSourceDisplayPass> display;
  std::shared_ptr<const uint8_t> token = std::make_shared<const uint8_t>(1);
  bool alive = true;
};
struct Fixture {
  static constexpr uint32_t B = 0x84000000, W = 0x84610000, D = 0x84510000,
                            C = D + 0x100, P = D + 0x200, Key = D + 0x300;
  std::shared_ptr<Runtime> r = std::make_shared<Runtime>();
  std::shared_ptr<BattleMenuFileCache> files =
      std::make_shared<BattleMenuFileCache>();
  std::shared_ptr<MessageAssets> messages = std::make_shared<MessageAssets>();
  std::shared_ptr<CameraManager> cameras = std::make_shared<CameraManager>();
  std::shared_ptr<int> receipt = std::make_shared<int>(1);
  uint32_t descriptor = 0, palette = 0;
  Fixture() {
    assert(r->vm.memory.registerRam(0x8041eb90, std::vector<uint8_t>(4)));
    assert(r->vm.memory.registerRam(B, std::vector<uint8_t>(0x20000)));
    assert(r->vm.memory.registerRam(W, std::vector<uint8_t>(0x1000)));
    assert(r->vm.memory.registerRam(D, std::vector<uint8_t>(0x1000)));
    assert(r->vm.memory.writeWord(0x8041eb90, B));
    assert(r->vm.memory.writeWord(B + 0x1c78, W));
    assert(files->load("romfs/battle-class-common.tpl"));
    assert(files->bind(r->vm.memory));
    const auto entry = files->acquire("./battle/common/battle_common.tpl", 4);
    assert(entry && *entry);
    descriptor = *entry;
    uint32_t holder = 0;
    assert(r->vm.memory.readWord(descriptor + 0xa0, holder));
    assert(r->vm.memory.readWord(holder, palette));
    assert(r->vm.memory.writeWord(B + 0x163f8, descriptor));
    assert(messages->loadGlobal("romfs/messages/global.txt"));
    assert(messages->loadRoom("romfs/messages/gor_00.txt"));
    const char *key = originalBattleClassMenuMessageKeys()[0].id;
    for (uint32_t i = 0;; ++i) {
      assert(r->vm.memory.writeByte(Key + i, uint8_t(key[i])));
      if (!key[i])
        break;
    }
    cameras->initializeBoot(608, 480);
    HarborSourceDisplayBinding d;
    d.battle = B;
    d.generation = 17;
    d.runtime = r;
    d.cameras = cameras;
    d.cameraReceipt = {receipt, [this](const CameraManager &bank) {
                         return &bank == cameras.get();
                       }};
    const std::weak_ptr<Runtime> weak = r;
    d.current = [weak] {
      auto r = weak.lock();
      return r && r->alive;
    };
    d.registerCommands = [](DisplayQueue &q, BattleFooterCursor &) {
      return q.enter(8, 0, 0x80121ce8, B, 500) == DisplayAdmission::Registered;
    };
    d.main.callback = [](void *, uint32_t, uint32_t, CameraState &) {
      return true;
    };
    d.main.lookAt = [](void *, const CameraState &c) {
      return cameraLookAtAdapter(c.eye, c.at, c.up);
    };
    d.draw.letterbox = [](void *, DisplayQueue &) { return true; };
    d.draw.request = [](void *, const CameraDrawRequest &) { return true; };
    d.draw.display.state = [](void *, const DisplayStateRequest &) {
      return true;
    };
    d.callback = [weak](const HarborSourceDisplayInvocation &q) {
      auto r = weak.lock();
      assert(r && r->factory);
      auto b = r->factory->weaponBinding({});
      assert(b);
      assert(b->native.producerCurrent({b->native.generation, q.pass}));
      assert(!b->native.producerCurrent({17, q.pass}));
      assert(!b->native.producerCurrent({b->native.generation, q.pass + 1}));
      assert(!r->factory->resumeDisplay()); // actual source callback cannot recursively drive its parent
      BattleWeaponNativeFrame unbound;
      assert(!r->factory->resumeWeapon(q, unbound));
      return true;
    };
    auto display = HarborSourceDisplayPass::create(std::move(d));
    r->display = std::shared_ptr<HarborSourceDisplayPass>(std::move(display));
    assert(r->display);
    HarborSourceMenuFactoryBinding b;
    b.vm = &r->vm;
    b.battle = B;
    b.runtime = r;
    b.display = r->display;
    b.files = files;
    b.messages = messages;
    b.current = [weak] {
      auto r = weak.lock();
      return r && r->alive;
    };
    b.uiLifetime = [weak](uint32_t w) {
      auto r = weak.lock();
      return r && w == W ? r->token : nullptr;
    };
    b.random = [](uint32_t bound) -> uint32_t {
      assert(bound);
      return bound - 1;
    };
    r->factory = HarborSourceMenuFactory::create(std::move(b));
    assert(r->factory && r->factory->current());
  }
};
void actualBindings() {
  Fixture f;
  auto a = f.r->factory->classBinding({});
  auto b = f.r->factory->weaponBinding({});
  assert(a && b);
  assert(a->native.cameras == f.cameras && b->native.battle == Fixture::B);
  assert(b->native.sourceResourceCurrent());
  assert(a->random(5) == 4);
  const auto text = b->messageLookup(Fixture::Key);
  assert(text && *text);
  assert(b->messageLookup(Fixture::Key) == text);
  assert(a->label(*text));
  assert(f.r->vm.memory.writeWord(Fixture::B + 0xa0, 0xffffffff));
  assert(b->native.sourceResourceCurrent());
  assert(f.r->vm.memory.writeWord(Fixture::B + 0x163f8, f.descriptor + 4));
  assert(!b->native.sourceResourceCurrent());
  assert(f.r->vm.memory.writeWord(Fixture::B + 0x163f8, f.descriptor));
  assert(b->native.sourceResourceCurrent());
  const auto oldToken = f.r->token;
  f.r->token = std::make_shared<const uint8_t>(2);
  assert(!a->native.allocationCurrent());
  assert(!b->native.allocationCurrent());
  auto fresh = f.r->factory->weaponBinding({});
  assert(fresh && fresh->native.generation != b->native.generation);
  assert(!f.r->factory->resumeDisplay()); // no source registration/seal inferred
  assert(f.r->display->begin(1));
  assert(f.r->display->registerCommands());
  assert(f.r->display->sealRegistrations());
  assert(f.r->factory->resumeDisplay());
  assert(!fresh->native.producerCurrent({fresh->native.generation, 1}));
  assert(f.files->release(f.descriptor));
  assert(!fresh->native
              .sourceResourceCurrent()); // actual source FileEntry state/refs
                                         // retire, B pointer remains

  auto factory = f.r->factory;
  f.r.reset();
  assert(!factory->current());
  assert(!fresh->native.allocationCurrent());
  assert(!fresh->native.sourceResourceCurrent());
  assert(!fresh->messageLookup(Fixture::Key));
}
void retiredRuntime() {
  Fixture f;
  auto source = f.r->factory->weaponBinding({});
  assert(source);
  f.r->alive = false;
  assert(!source->native.allocationCurrent());
  f.r->alive = true;
  assert(!f.r->factory->current());
}
} // namespace
int main() {
  actualBindings();
  retiredRuntime();
  std::cout << "Harbor source menu factory tests passed\n";
  return 0;
}

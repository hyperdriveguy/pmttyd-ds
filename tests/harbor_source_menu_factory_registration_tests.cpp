// Compile this TU with the exact journaled Harbor.cpp on its private include
// search path. All facade code and storage have one consistent ABI; no Game
// object or legacy archive Harbor implementation is executed.
#include "harbor_tutorial.cpp"
#include <cassert>
#include <iostream>
namespace ttyd {
struct HarborSourceMenuFactoryFixture {
  static std::shared_ptr<HarborTutorialRuntime>
  retain(const HarborTutorial &s) {
    return s.runtime_;
  }
  static void attach(HarborTutorial &s) {
    auto r = s.runtime_;
    assert(r);
    auto cameras = std::make_shared<CameraManager>();
    cameras->initializeBoot(608, 480);
    HarborSourceDisplayBinding b;
    b.battle = r->packed->battleAddress();
    b.generation = 77;
    b.runtime = r;
    b.cameras = cameras;
    // TEST parent receipt only: no source initializer or normal native GPU
    // claim.
    b.cameraReceipt = {cameras, [weak = std::weak_ptr<CameraManager>(cameras)](
                                    const CameraManager &bank) {
                         return weak.lock().get() == &bank;
                       }};
    b.current = [weak = std::weak_ptr<HarborTutorialRuntime>(r)] {
      return !weak.expired();
    };
    b.registerCommands = [](DisplayQueue &, BattleFooterCursor &) {
      return false;
    }; // real source child unbound, never acknowledged
    auto pass = HarborSourceDisplayPass::create(std::move(b));
    assert(pass);
    assert(s.setHarborSourceDisplayPass(
        std::shared_ptr<HarborSourceDisplayPass>(std::move(pass))));
  }
};
} // namespace ttyd
using namespace ttyd;
int main() {
  assert(loadHarborTutorialAssets("romfs"));
  std::shared_ptr<HarborSourceMenuFactory> retained;
  std::shared_ptr<HarborTutorialRuntime> runtime;
  {
    HarborTutorial scene;
    assert(scene.beginFresh(0, 10, 10, 0));
    HarborSourceMenuFactoryFixture::attach(scene);
    runtime = HarborSourceMenuFactoryFixture::retain(scene);
    retained = scene.sourceMenuFactory();
    assert(retained && retained->current());
  }
  assert(runtime);
  assert(!retained->current()); // retained VM alone is not facade ownership
  {
    HarborTutorial scene;
    assert(scene.beginFresh(0, 10, 10, 0));
    HarborSourceMenuFactoryFixture::attach(scene);
    auto oldRuntime = HarborSourceMenuFactoryFixture::retain(scene);
    auto old = scene.sourceMenuFactory();
    assert(old && old->current());
    assert(!scene.beginFresh(1, 10, 10, 0));
    assert(old->current()); // active original begin is rejected without
                            // replacement
    scene = HarborTutorial{};
    assert(!old->current());
    assert(scene.beginFresh(1, 10, 10, 0));
    assert(oldRuntime);
  }
  {
    HarborTutorial first;
    assert(first.beginFresh(0, 10, 10, 0));
    HarborSourceMenuFactoryFixture::attach(first);
    auto oldRuntime = HarborSourceMenuFactoryFixture::retain(first);
    auto factory = first.sourceMenuFactory();
    assert(factory && factory->current());
    {
      HarborTutorial copy = first;
      assert(copy.sourceMenuFactory() == factory);
      first = HarborTutorial{};
      assert(first.beginFresh(1, 10, 10, 0));
      assert(
          factory
              ->current()); // copy still owns that actual runtime registration
      HarborTutorial moved = std::move(copy);
      assert(moved.sourceMenuFactory() == factory);
      assert(factory->current());
      HarborTutorial replacement;
      assert(replacement.beginFresh(2, 10, 10, 0));
      moved = replacement; // release the final facade slot of oldRuntime
    }
    assert(oldRuntime);
    assert(!factory->current());
  }
  std::cout << "Harbor actual facade menu registration tests passed\n";
  return 0;
}

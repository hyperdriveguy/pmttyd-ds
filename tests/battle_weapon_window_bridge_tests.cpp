#include "battle_weapon_camera_bridge.hpp"
#include "battle_weapon_ordered_publication.hpp"
#include "battle_weapon_window_bridge.hpp"
#include "fixtures/battle_weapon_source_draw_fixture.hpp"
#include "fixtures/window_native_atlas_fixture.hpp"
#include <iostream>
#include <memory>
using namespace ttyd;
namespace {
EventNativePlatform platform() {
  EventNativePlatform p;
  p.flushBlock = [](void *, uint32_t) {};
  p.systemCall = [](void *) {};
  p.truncate = [](void *, float x) {
    assert(std::isfinite(x) && double(x) >= -2147483648.0 &&
           double(x) < 2147483648.0);
    return int32_t(x);
  };
  p.subtractDouble = [](void *, double a, double b) {
    volatile double v = a - b;
    return v;
  };
  p.roundDoubleFloat = [](void *, double d) {
    volatile float f = float(d);
    return f;
  };
  return p;
}
struct Fixture {
  std::shared_ptr<NativeUiStartup> ui;
  std::shared_ptr<const int> life = std::make_shared<const int>(1);
  std::shared_ptr<CameraManager> bank = std::make_shared<CameraManager>();
  WeaponSourceFixture actor;
  BattleWeaponCameraBridge camera;
  CameraMainCursor main;
  CameraDrawCursor draw;
  DisplayQueue queue;
  BattleWeaponOrderedPublication ordered;
  BattleFontGpuResource unusedFont;
  BattleSubmenuSurfaceGpuResource unusedSubmenu;
  std::shared_ptr<const uint8_t> allocation =
      std::make_shared<const uint8_t>(0);
  BattleWindowSurfaceResource atlas;
  std::vector<uint8_t> pixels = WindowNativeFixture::actualNativeAtlas();
  std::shared_ptr<SourceGxSetterRouting> gx;
  std::unique_ptr<BattleWeaponWindowBridge> bridge;
  BattleWeaponHelpCall call;
  std::vector<BattleWindowPacketPtr> packets;
  bool alive = true, holdSink = false, prepared = false, wrongCamera = false,
       holdView = false;
  std::optional<uint32_t> blockedWord;
  unsigned cameras = 0, viewReads = 0, sinks = 0, mathCalls = 0;
  uint64_t pass = 1;
  static constexpr uint32_t Base = 0x60014000;
  explicit Fixture(bool complete = true, bool imported = true) {
    auto process = NativeFieldAnimation::create(
        "romfs", NativeFileCache::create(), platform());
    assert(process && process->ready());
    ui = NativeUiStartup::create(process, "romfs");
    assert(ui && ui->resumeFont());
    if (complete)
      assert(ui->resumeWindow() && ui->windowObject(50));
    bank->initializeBoot(608, 480);
    // Explicit camera fixture policy: unrelated boot callback owners are
    // absent, so those records have no callback. Camera8 uses real
    // Main/LookAt/Draw and actual invoking80121ce8 queue admission; no
    // fabricated view words/projection.
    for (uint32_t i = 0; i < 13; ++i)
      bank->camera(i)->callback = 0;
    CameraMainServices m;
    m.lookAt = [](void *, const CameraState &c) {
      return cameraLookAtAdapter(c.eye, c.at, c.up);
    };
    assert(bank->main(false, m, main));
    assert(camera.bind(bank, 17, actor.actors(),
                       {Base, [](uint32_t, uint32_t) { return true; }},
                       [this](BattleClassCameraEpoch e) {
                         return alive && e.generation == 17 && e.pass == pass;
                       }));
    gx = std::make_shared<SourceGxSetterRouting>(
        ui->gxHistory(), life, [this] { return alive && camera.available(); });
    if (imported) {
      BattleClassSurfaceImport facts{this, 128, 128,   0,           1,
                                     128,  128, false, {0, 1, 1, 0}};
      assert(atlas.adopt(facts, pixels.data(), pixels.size()));
    }
    call.function = 0x8007b44c;
    call.callsite = 0x80117c64;
    call.integers = {0};
    call.reals = {-250, 100, 500, 60, 10};
    call.privateColor = 0x123456cf;
    assert(queue.enter(8, 0, 0x80121ce8, WeaponSourceFixture::B, 500) ==
           DisplayAdmission::Registered);
  }
  bool callback() {
    assert(camera.begin({17, pass}, main, draw, queue, WeaponSourceFixture::B));
    if (!bridge) {
      const auto view = camera.view();
      assert(view);
      BattleWeaponOrderedBinding orderedBinding;
      orderedBinding.allocationOwner = allocation;
      orderedBinding.sourceResourceOwner = life;
      orderedBinding.resources = life;
      orderedBinding.allocation = WeaponSourceFixture::W;
      orderedBinding.generation = 17;
      orderedBinding.capacity = 16;
      orderedBinding.user = this;
      orderedBinding.current = [](void *p, uint32_t a, uint64_t g) {
        auto &f = *static_cast<Fixture *>(p);
        return f.alive && f.ui->current() && a == WeaponSourceFixture::W &&
               g == 17;
      };
      orderedBinding.sourceCurrent = [](void *p, uint64_t pass) {
        auto &f = *static_cast<Fixture *>(p);
        return pass == 1 && f.alive && f.camera.available();
      };
      orderedBinding.sourceComplete = [](void *, uint64_t) {
        return false;
      }; // Whole Weapon parent is deliberately unsubmitted in this source-child
         // test.
      orderedBinding.font = &unusedFont;
      orderedBinding.submenu = &unusedSubmenu;
      orderedBinding.window = &atlas;
      orderedBinding.icon =
          [](void *, uint32_t) -> std::optional<BattleEnemyHpIconResidency> {
        return {};
      };
      orderedBinding.ready = [](void *, const BattleWeaponOrderedBatch &) {
        return false;
      };
      orderedBinding.draw = [](void *, const BattleWeaponOrderedBatch &) {
        assert(false);
      };
      assert(ordered.bind(std::move(orderedBinding)) && ordered.begin(pass));
      BattleWeaponWindowBinding b;
      b.startup = ui;
      b.resources = life;
      b.gx = gx;
      b.camera.lifetime = life;
      b.camera.user = this;
      b.camera.current = [](void *p, const auto &q) {
        return static_cast<Fixture *>(p)->alive && q.generation == 17 &&
               q.pass == 1;
      };
      b.camera.generation = 17;
      b.camera.pass = 1;
      b.camera.camera = 8;
      b.camera.projection = view->matrices.projection;
      b.camera.viewport = view->viewport;
      for (unsigned i = 0; i < 4; ++i)
        b.camera.scissor[i] = view->scissor[i];
      b.cameraAddress = *camera.camera(8);
      b.sourceCurrent = [this] { return alive && camera.available(); };
      b.longCurrent = [this] { return alive; };
      b.currentCamera = [this]() -> std::optional<uint32_t> {
        ++cameras;
        return camera.camera(wrongCamera ? std::optional<uint32_t>(4)
                                         : std::nullopt);
      };
      b.word = [this](uint32_t a) -> std::optional<uint32_t> {
        if (camera.owns(a)) {
          ++viewReads;
          if (holdView)
            return {};
          return camera.word(a);
        }
        if (blockedWord && *blockedWord == a)
          return {};
        uint32_t v;
        return actor.originals.readWord(a, v) ? std::optional<uint32_t>(v)
                                              : std::nullopt;
      };
      b.matrix = [this](BattleAudienceMatrixRequest &q) {
        ++mathCalls;
        return camera.matrix(q);
      };
      b.append = [this](BattleWindowPacketPtr p) {
        ++sinks;
        assert(!bridge->request(call));
        if (holdSink)
          return false;
        if (!ordered.appendWindow(pass, p))
          return false;
        packets.push_back(std::move(p));
        return true;
      };
      bridge = std::make_unique<BattleWeaponWindowBridge>(atlas, std::move(b));
    }
    if (!prepared)
      prepared = bridge->request(call).has_value();
    // Preparation is not a host GPU submission. Leave the actual outer callback
    // held; normal runtime finishes only after its whole ordered GPU admission.
    return false;
  }
  void run() {
    CameraDrawServices s;
    s.user = this;
    s.letterbox = [](void *, DisplayQueue &) { return true; };
    s.request = [](void *, const CameraDrawRequest &) { return true; };
    s.display.user = this;
    s.display.state = [](void *, const DisplayStateRequest &) { return true; };
    s.display.callback = [](void *p, uint32_t c, uint32_t f, uint32_t b) {
      assert(c == 8 && f == 0x80121ce8 && b == WeaponSourceFixture::B);
      return static_cast<Fixture *>(p)->callback();
    };
    assert(!bank->draw(queue, s, draw));
  }
};
void actualSourceAndHold() {
  Fixture f;
  f.holdSink = true;
  f.run();
  assert(!f.prepared && f.packets.empty() && f.sinks == 1 && f.cameras == 1 &&
         f.viewReads == 12 && f.mathCalls == 2);
  f.run();
  assert(f.sinks == 2 && f.cameras == 1 && f.viewReads == 12 &&
         f.mathCalls == 2);
  f.call.privateColor = 0;
  f.holdSink = false;
  f.run();
  assert(f.prepared && f.packets.size() == 1 &&
         f.packets[0]->rgba == 0x123456cf &&
         validateBattleWindowPacket(*f.packets[0], f.atlas));
  const auto p = f.packets[0];
  auto gx = f.ui->gxHistory();
  SourceGxInheritedCall changed;
  changed.function = 0x802b9380;
  changed.count = 1;
  changed.integers[0] = 0;
  assert(gx->apply(changed) && validateBattleWindowPacket(*p, f.atlas));
  const auto o = f.ui->windowObject(0);
  assert(o);
  auto m = f.ui->windowMemory();
  assert(m->storeByte(o->address, o->bytes[0] ^ 1) &&
         !validateBattleWindowPacket(*p, f.atlas));
  assert(m->storeByte(o->address, o->bytes[0]) &&
         validateBattleWindowPacket(*p, f.atlas));
  f.ui->retire();
  assert(!validateBattleWindowPacket(*p, f.atlas));
}
void genuineUnknownObjectsAndHistory() {
  Fixture object;
  auto o = object.ui->windowObject(0);
  assert(o);
  auto memory = object.ui->windowMemory();
  assert(memory->storeByte(o->address, o->bytes[0] ^ 1));
  object.run();
  assert(!object.prepared && object.bridge->pending() == 0x802b7880 &&
         object.cameras == 1 && object.viewReads == 12 &&
         object.mathCalls == 2 && object.packets.empty());
  assert(memory->storeByte(o->address, o->bytes[0]));
  object.run();
  assert(object.prepared && object.cameras == 1 && object.viewReads == 12 &&
         object.mathCalls == 2);
  Fixture unknown;
  unknown.blockedWord = 0x80420540;
  unknown.run();
  assert(!unknown.prepared && unknown.packets.empty() &&
         unknown.bridge->pending() == 0x80420540);
  unknown.run();
  assert(unknown.cameras == 1 && unknown.viewReads == 12 &&
         unknown.mathCalls == 2);
  unknown.blockedWord.reset();
  unknown.run();
  assert(unknown.prepared && unknown.packets.size() == 1);
  Fixture gx;
  assert(gx.ui->gxHistory()->invalidate());
  gx.run();
  assert(!gx.prepared && gx.packets.empty() &&
         gx.bridge->pending() == 0x802b7880);
  gx.run();
  assert(gx.cameras == 1 && gx.viewReads == 12 && gx.mathCalls == 2);
}
void realReadyZero() {
  Fixture f(false, false);
  assert(!f.ui->beginWindow() ||
         !f.ui->windowReady()); // setup job admitted; actual callback not
                                // delivered
  f.run();
  assert(f.prepared && f.packets.empty() && f.cameras == 1 && !f.viewReads &&
         !f.mathCalls && !f.sinks);
}
void unknownAndStale() {
  Fixture f;
  f.holdView = true;
  f.run();
  assert(!f.prepared && f.cameras == 1 && f.viewReads == 1 && f.mathCalls == 1);
  f.run();
  assert(f.cameras == 1 && f.viewReads == 2 && f.mathCalls == 1);
  f.holdView = false;
  f.run();
  assert(f.prepared && f.packets.size() == 1 && f.cameras == 1 &&
         f.mathCalls == 2);
  Fixture stale;
  stale.holdSink = true;
  stale.run();
  stale.atlas.retire();
  stale.holdSink = false;
  stale.run();
  assert(!stale.prepared && stale.packets.empty());
  Fixture wrong;
  wrong.wrongCamera = true;
  wrong.run();
  assert(!wrong.prepared && wrong.bridge->pending() == 0x8000f2e4 &&
         wrong.packets.empty());
}
} // namespace
int main() {
  actualSourceAndHold();
  genuineUnknownObjectsAndHistory();
  realReadyZero();
  unknownAndStale();
  std::cout << "actual Window startup/GX/camera/ordered preparation bridge "
               "tests PASS\n";
}

#include "battle_enemy_hp_camera_bridge.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float f) {
  uint32_t n;
  std::memcpy(&n, &f, 4);
  return n;
}
struct Fixture {
  std::shared_ptr<CameraManager> bank = std::make_shared<CameraManager>();
  CameraMainCursor main;
  CameraDrawCursor draw;
  DisplayQueue queue;
  BattleEnemyHpCameraBridge hp;
  std::map<uint32_t, uint32_t> words{
      {0x80422244, bits(304)}, {0x80422248, bits(240)}, {0x80422240, bits(0)}};
  uint64_t generation = 9, producer = 0;
  uint32_t callback = 0x8011fde0;
  bool admitted = true, unmapped = true, finish = false, expect = true;
  unsigned reads = 0, invocations = 0;
  static constexpr uint32_t Base = 0x60000000, B = 0x12340000;
  Fixture() { bank->initializeBoot(608, 480); }
  BattleClassCameraEpoch epoch() const { return {generation, producer}; }
  void produce() {
    CameraMainServices s;
    s.callback = [](void *, uint32_t, uint32_t, CameraState &) { return true; };
    s.lookAt = [](void *, const CameraState &c) {
      return cameraLookAtAdapter(c.eye, c.at, c.up);
    };
    assert(bank->main(false, s, main));
    ++producer;
  }
  void bind() {
    BattleActorServices a;
    a.memory.user = this;
    a.memory.word = [](void *u, uint32_t address) {
      auto &f = *static_cast<Fixture *>(u);
      ++f.reads;
      return f.words.at(address);
    };
    assert(hp.bind(bank, generation, a,
                   {Base,
                    [this](uint32_t a, uint32_t n) {
                      return unmapped && a == Base && n == 13 * 0x400;
                    }},
                   [this](BattleClassCameraEpoch e) {
                     return admitted && e.generation == generation &&
                            e.pass == producer;
                   }));
  }
  bool dispatch(bool enqueue = true) {
    if (enqueue)
      assert(queue.enter(8, 0, callback, B, 902) ==
             DisplayAdmission::Registered);
    CameraDrawServices s;
    s.user = this;
    s.letterbox = [](void *, DisplayQueue &) { return true; };
    s.request = [](void *, const CameraDrawRequest &) { return true; };
    s.display.user = this;
    s.display.state = [](void *, const DisplayStateRequest &) { return true; };
    s.display.callback = [](void *u, uint32_t camera, uint32_t callback,
                            uint32_t battle) {
      auto &f = *static_cast<Fixture *>(u);
      ++f.invocations;
      assert(camera == 8 && callback == f.callback && battle == B);
      const bool ready = f.hp.begin(f.epoch(), f.main, f.draw, f.queue, battle);
      assert(ready == f.expect);
      return ready && f.finish && f.hp.finish(f.epoch());
    };
    return bank->draw(queue, s, draw);
  }
  void ready() {
    produce();
    bind();
    assert(!dispatch());
    assert(hp.available());
  }
};
void typedEntry() {
  struct Typed {
    BattleScreenPoint input{1, 2, 3};
    std::vector<uint32_t> order;
    bool camera = true, view = true, project = true;
  } f;
  BattleScreenPointServices s;
  s.user = &f;
  s.memory.user = &f;
  s.camera = [](void *u, uint32_t id) -> std::optional<uint32_t> {
    auto &f = *static_cast<Typed *>(u);
    f.order.push_back(id);
    f.input = {2, 3, 4};
    return f.camera ? std::optional<uint32_t>(0x5000) : std::nullopt;
  };
  s.view = [](void *u, uint32_t a, const BattleScreenPoint &in,
              BattleScreenPoint &out) {
    auto &f = *static_cast<Typed *>(u);
    f.order.push_back(a);
    assert((in == BattleScreenPoint{2, 3, 4}));
    f.input = {100, 200, 300};
    if (!f.view)
      return false;
    out = in;
    return true;
  };
  s.project = [](void *u, uint32_t a, const BattleScreenPoint &in,
                 BattleScreenPoint &out) {
    auto &f = *static_cast<Typed *>(u);
    f.order.push_back(a);
    if (!f.project)
      return false;
    out = in;
    return true;
  };
  s.memory.word = [](void *u, uint32_t a) {
    static_cast<Typed *>(u)->order.push_back(a);
    return bits(a == 0x80422244 ? 12.f : a == 0x80422248 ? -7.f : 9.f);
  };
  BattleScreenPoint out{8, 8, 8};
  assert(packedBattleScreenPoint(f.input, out, s));
  assert((out == BattleScreenPoint{24, -21, 9}));
  assert((f.order == std::vector<uint32_t>{4, 0x511c, 0x515c, 0x80422244,
                                           0x80422248, 0x80422240}));
  for (unsigned i = 0; i < 3; ++i) {
    f = {};
    f.camera = i != 0;
    f.view = i != 1;
    f.project = i != 2;
    out = {8, 8, 8};
    assert(!packedBattleScreenPoint(f.input, out, s));
    assert((out == BattleScreenPoint{8, 8, 8}));
  }
}
} // namespace
int main() {
  typedEntry();
  Fixture f;
  f.ready();
  assert(f.hp.camera() == Fixture::Base + 8 * 0x400 &&
         f.hp.camera(4) == Fixture::Base + 4 * 0x400);
  assert(f.hp.word(Fixture::Base + 8 * 0x400 + 0x148) ==
         bits(f.bank->camera(8)->matrices.view[2][3]));
  const BattleScreenPoint xyz{100, 50, 0};
  const auto projected = f.hp.project(xyz);
  assert(projected);
  const auto expected =
      cameraDepthProject(f.bank->camera(4)->matrices,
                         cameraDepthView(f.bank->camera(4)->matrices, xyz));
  assert((*projected ==
          BattleScreenPoint{304 * expected[0], 240 * expected[1], 0}));
  f.finish = true;
  assert(f.dispatch(false));
  assert(!f.hp.available());
  f.produce();
  assert(f.dispatch());
  Fixture wrong;
  wrong.callback = 0x8012210c;
  wrong.expect = false;
  wrong.produce();
  wrong.bind();
  assert(!wrong.dispatch());
  assert(!wrong.hp.available());
  Fixture boot;
  boot.expect = false;
  boot.bind();
  assert(!boot.dispatch());
  assert(!boot.hp.available());
  for (unsigned mutation = 0; mutation < 5; ++mutation) {
    Fixture stale;
    stale.ready();
    const auto reads = stale.reads;
    if (mutation == 0)
      ++stale.generation;
    if (mutation == 1)
      ++stale.producer;
    if (mutation == 2)
      stale.unmapped = false;
    if (mutation == 3)
      stale.bank->camera(8)->matrices.view[2][3] += 1;
    if (mutation == 4) {
      const auto epoch = stale.queue.resetEpoch();
      const auto *record = stale.queue.current();
      stale.queue.reinitialize();
      assert(stale.queue.resetEpoch() != epoch);
      assert(stale.queue.enter(8, 0, 0x8011fde0, Fixture::B, 902) ==
             DisplayAdmission::Registered);
      stale.queue.sort();
      assert(stale.queue.current() == record);
    }
    assert(!stale.hp.available());
    assert(!stale.hp.project(xyz));
    assert(stale.reads == reads && stale.hp.owns(Fixture::Base));
  }
  Fixture classOwner;
  classOwner.produce();
  BattleClassCameraBridge klass;
  BattleActorServices memory;
  memory.memory.user = &classOwner;
  memory.memory.word = [](void *u, uint32_t a) {
    return static_cast<Fixture *>(u)->words.at(a);
  };
  assert(klass.bind(classOwner.bank, 9, memory,
                    {Fixture::Base, [](uint32_t, uint32_t) { return true; }},
                    [&](BattleClassCameraEpoch e) {
                      return e.generation == 9 && e.pass == classOwner.producer;
                    }));
  assert(classOwner.queue.enter(8, 0, 0x8012210c, Fixture::B, 500) ==
         DisplayAdmission::Registered);
  CameraDrawServices classDraw;
  classDraw.letterbox = [](void *, DisplayQueue &) { return true; };
  classDraw.request = [](void *, const CameraDrawRequest &) { return true; };
  struct ClassContext {
    Fixture &f;
    BattleClassCameraBridge &bridge;
  } context{classOwner, klass};
  classDraw.display.user = &context;
  classDraw.display.state = [](void *, const DisplayStateRequest &) {
    return true;
  };
  classDraw.display.callback = [](void *u, uint32_t, uint32_t, uint32_t b) {
    auto &c = *static_cast<ClassContext *>(u);
    assert(c.bridge.begin(c.f.epoch(), c.f.main, c.f.draw, c.f.queue, b));
    return false;
  };
  assert(!classOwner.bank->draw(classOwner.queue, classDraw, classOwner.draw));
  assert(klass.available());
  classOwner.queue.reinitialize();
  assert(classOwner.queue.enter(8, 0, 0x8012210c, Fixture::B, 500) ==
         DisplayAdmission::Registered);
  classOwner.queue.sort();
  assert(!klass.available());
  // Metadata does not alter original dispatch's captured callback semantics.
  DisplayQueue q;
  DisplayDispatchCursor d;
  unsigned calls = 0;
  DisplayDispatchServices s;
  s.user = &calls;
  s.state = [](void *, const DisplayStateRequest &) { return true; };
  s.callback = [](void *u, uint32_t, uint32_t, uint32_t) {
    return ++*static_cast<unsigned *>(u) > 1;
  };
  assert(q.resetEpoch() == 1);
  assert(q.enter(8, 0, 0x8011fde0, Fixture::B, 902) ==
         DisplayAdmission::Registered);
  q.sort();
  assert(!q.dispatch(8, s, d));
  const auto *current = q.current();
  q.reinitialize();
  assert(q.resetEpoch() == 2 && q.current() == current && q.count() == 0);
  assert(q.dispatch(8, s, d) && calls == 2);
  Fixture invalid;
  invalid.ready();
  BattleScreenPoint bad = xyz;
  bad[0] = std::numeric_limits<float>::infinity();
  assert(!invalid.hp.project(bad));
  std::cout << "Actual HP camera callback, typed source projection, shared "
               "epochs and reset/repopulate rejection passed\n";
}

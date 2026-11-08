#include "harbor_source_display_pass.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
// TEST-only source-memory and camera-provider fixture. This does not claim the
// original field-battle parent has executed or that any native GPU was used.
struct Fixture {
  static constexpr uint32_t Battle = 0x100000, Unit = 0x200000;
  std::shared_ptr<int> runtime = std::make_shared<int>(1),
                       receipt = std::make_shared<int>(2);
  std::shared_ptr<CameraManager> cameras = std::make_shared<CameraManager>();
  std::unique_ptr<HarborSourceDisplayPass> owner;
  std::map<uint32_t, uint32_t> words;
  BattleCommandPalette palette;
  std::vector<std::string> trace;
  int stop = -1, failures = 0;
  unsigned accepted = 0, childPhase = 0, registrations = 0;
  bool live = true, receiptCurrent = true, reenter = false,
       retireAtCallback = false;
  Fixture() {
    cameras->initializeBoot(608, 480);
    words[Battle + 0x1720] = 1;
    for (unsigned slot = 0; slot < 64; ++slot)
      words[Battle + 0x20 + 4 * slot] = 0;
    words[Battle + 0x20] = Unit;
    words[0x8041eb90] = Battle;
    words[Unit + 8] = 35;
    words[Unit + 0x1c] = 0;
    words[Battle + 0x163f4] = 0;
    words[Battle + 0x163d4 + 4] = 0;
  }
  bool event(std::string value) {
    if (int(accepted) == stop && failures++ < 2)
      return false;
    trace.push_back(std::move(value));
    ++accepted;
    return true;
  }
  HarborSourceDisplayBinding binding() {
    HarborSourceDisplayBinding b;
    b.battle = Battle;
    b.generation = 7;
    b.runtime = runtime;
    b.cameras = cameras;
    b.cameraReceipt = {receipt, [this](const CameraManager &bank) {
                         return receiptCurrent && &bank == cameras.get();
                       }};
    b.current = [this] { return live; };
    b.registerCommands = [this](DisplayQueue &q, BattleFooterCursor &cursor) {
      ++registrations;
      BattleActorServices a;
      a.memory.user = this;
      a.memory.word = [](void *u, uint32_t address) {
        return static_cast<Fixture *>(u)->words.at(address);
      };
      return registerBattleCommandDisplay(Battle, palette, q, cursor, a);
    };
    b.main.user = this;
    b.main.callback = [](void *u, uint32_t fn, uint32_t index, CameraState &) {
      return static_cast<Fixture *>(u)->event("main:" + std::to_string(fn) +
                                              ":" + std::to_string(index));
    };
    b.main.lookAt =
        [](void *u, const CameraState &c) -> std::optional<CameraViewMatrix> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("look"))
        return {};
      return cameraLookAtAdapter(c.eye, c.at, c.up);
    };
    b.draw.user = this;
    b.draw.display.user = this;
    b.draw.letterbox = [](void *u, DisplayQueue &) {
      return static_cast<Fixture *>(u)->event("letterbox");
    };
    b.draw.request = [](void *u, const CameraDrawRequest &q) {
      return static_cast<Fixture *>(u)->event(
          "draw:" + std::to_string(q.index) + ":" +
          std::to_string(unsigned(q.kind)));
    };
    b.draw.display.state = [](void *u, const DisplayStateRequest &q) {
      return static_cast<Fixture *>(u)->event("state:" +
                                              std::to_string(unsigned(q.kind)));
    };
    b.hp.user = this;
    b.hp.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      assert(f.cameras->currentIndex() == 8);
      if (!f.event("word:" + std::to_string(a)))
        return {};
      auto i = f.words.find(a);
      return i == f.words.end() ? std::nullopt
                                : std::optional<uint32_t>(i->second);
    };
    b.hp.status = [](void *u, uint32_t unit,
                     int32_t status) -> std::optional<int32_t> {
      assert(unit == Unit && status == 27);
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("status"))
        return {};
      return 0;
    };
    b.monosiri.user = this;
    b.monosiri.switchFlag = [](void *u, int32_t id) -> std::optional<int32_t> {
      assert(id == 35 + 4474);
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("saved-switch"))
        return {};
      return 0;
    };
    b.callback = [this](const HarborSourceDisplayInvocation &call) {
      assert(call.callback == 0x8012210c && call.battle == Battle &&
             call.camera == 8);
      assert(call.draw.active && call.draw.stage == 8 &&
             call.draw.display.invoking);
      assert(owner->cameraMainCurrent({call.generation, call.pass}));
      if (reenter) {
        assert(!owner->resume());
        assert(!owner->begin(2));
        assert(!owner->registerCommands());
        assert(!owner->registrationQueue());
        assert(!owner->sealRegistrations());
      }
      if (retireAtCallback) {
        owner->retire();
        return true;
      }
      while (childPhase < 2) {
        if (!event("class-child:" + std::to_string(childPhase)))
          return false;
        ++childPhase;
      }
      return true;
    };
    return b;
  }
  void start() {
    owner = HarborSourceDisplayPass::create(binding());
    assert(owner);
    assert(owner->begin(1));
    assert(owner->registerCommands());
    assert(owner->registerCommands());
    assert(registrations == 1);
    assert(owner->registrationQueue()->count() == 2);
    assert(owner->sealRegistrations());
  }
  void finish() {
    for (unsigned i = 0; i < 1000 && !owner->resume(); ++i) {
      assert(owner->current());
      assert(!owner->complete(1));
      assert(!owner->begin(2));
    }
    assert(owner->complete(1));
    assert(!owner->cameraMainCurrent({7, 1}));
  }
};
std::vector<std::string> directSourceTrace() {
  Fixture f;
  auto b = f.binding();
  DisplayQueue queue;
  BattleFooterCursor registration;
  assert(b.registerCommands(queue, registration));
  CameraMainCursor main;
  assert(f.cameras->main(false, b.main, main));
  struct Direct {
    Fixture &fixture;
    BattleEnemyHpDrawCursor hp;
    BattleMonosiriCursor monosiri;
  } direct{f, {}, {}};
  CameraDrawServices draw = b.draw;
  draw.display.user = &direct;
  draw.display.state = [](void *u, const DisplayStateRequest &q) {
    return static_cast<Direct *>(u)->fixture.event(
        "state:" + std::to_string(unsigned(q.kind)));
  };
  draw.display.callback = [](void *u, uint32_t camera, uint32_t fn,
                             uint32_t battle) {
    auto &d = *static_cast<Direct *>(u);
    assert(camera == 8 && battle == Fixture::Battle);
    if (fn != 0x8011fde0) {
      assert(fn == 0x8012210c);
      while (d.fixture.childPhase < 2) {
        assert(d.fixture.event("class-child:" +
                               std::to_string(d.fixture.childPhase)));
        ++d.fixture.childPhase;
      }
      return true;
    }
    auto hp = d.fixture.binding().hp;
    hp.user = &d;
    hp.word = [](void *p, uint32_t address) {
      auto &f = static_cast<Direct *>(p)->fixture;
      return f.binding().hp.word(&f, address);
    };
    hp.status = [](void *p, uint32_t unit, int32_t id) {
      auto &f = static_cast<Direct *>(p)->fixture;
      return f.binding().hp.status(&f, unit, id);
    };
    hp.camera = [](void *p, uint32_t index) {
      auto &f = static_cast<Direct *>(p)->fixture;
      return index == 8 && f.cameras->currentIndex() == index;
    };
    hp.monosiri = [](void *p, uint32_t unit) {
      auto &d = *static_cast<Direct *>(p);
      auto services = d.fixture.binding().monosiri;
      services.word = d.fixture.binding().hp.word;
      return checkBattleMonosiri(unit, services, d.monosiri);
    };
    return drawBattleEnemyHp(battle, hp, d.hp);
  };
  CameraDrawCursor cursor;
  assert(f.cameras->draw(queue, draw, cursor));
  assert(queue.count() == 0);
  return f.trace;
}
void exactHolds() {
  Fixture base;
  base.start();
  base.finish();
  assert(base.palette.rgba[1] == 193);
  auto trace = base.trace;
  assert(trace == directSourceTrace());
  for (unsigned stop = 0; stop < base.accepted; ++stop) {
    Fixture f;
    f.stop = int(stop);
    f.start();
    f.finish();
    assert(f.trace == trace);
    assert(f.childPhase == 2);
    assert(f.palette.rgba[1] == 193);
    assert(f.registrations == 1);
  }
  // Completed repeated resume does not rerun source reads or callbacks.
  const auto size = base.trace.size();
  assert(base.owner->resume());
  assert(base.trace.size() == size);
  assert(!base.owner->begin(1));
  assert(base.owner->begin(2));
  assert(base.owner->registrationQueue()->count() == 0);
}
void admissionAndLifetimes() {
  Fixture missing;
  auto binding = missing.binding();
  binding.cameraReceipt = {};
  missing.owner = HarborSourceDisplayPass::create(binding);
  assert(missing.owner);
  assert(!missing.owner->begin(1));
  assert(missing.trace.empty());
  assert(missing.owner->admitCameraReceipt(missing.binding().cameraReceipt));
  assert(!missing.owner->admitCameraReceipt(missing.binding().cameraReceipt));
  assert(missing.owner->begin(1));
  assert(missing.owner->registerCommands());
  assert(missing.owner->sealRegistrations());
  missing.finish();
  Fixture f;
  f.start();
  f.stop = 0;
  assert(!f.owner->resume());
  f.runtime.reset();
  assert(!f.owner->resume());
  assert(!f.owner->current());
  assert(f.trace.empty());
  Fixture replaced;
  replaced.start();
  replaced.stop = 0;
  assert(!replaced.owner->resume());
  replaced.receiptCurrent = false;
  assert(!replaced.owner->resume());
  replaced.receiptCurrent = true;
  assert(!replaced.owner->current());
  Fixture recurse;
  recurse.reenter = true;
  recurse.start();
  recurse.finish();
  Fixture retired;
  retired.retireAtCallback = true;
  retired.start();
  assert(!retired.owner->resume());
  assert(!retired.owner->current());
  assert(!retired.owner->complete(1));
  Fixture reset;
  reset.owner = HarborSourceDisplayPass::create(reset.binding());
  assert(reset.owner->begin(1));
  assert(reset.owner->registerCommands());
  auto *q = reset.owner->registrationQueue();
  assert(reset.owner->sealRegistrations());
  reset.stop = 0;
  assert(!reset.owner->resume());
  q->reinitialize();
  assert(q->enter(8, 0, 0x8012210c, Fixture::Battle, 500) ==
         DisplayAdmission::Registered);
  assert(!reset.owner->resume());
  assert(!reset.owner->current());
}
void destroyedProviderContext() {
  std::unique_ptr<HarborSourceDisplayPass> retained;
  {
    auto fixture = std::make_unique<Fixture>();
    fixture->start();
    fixture->stop = 0;
    assert(!fixture->owner->resume());
    retained = std::move(fixture->owner);
    // All callbacks still hold raw pointers into this TEST context. The actual
    // weak runtime must be checked before any of those pointers is used.
  }
  assert(!retained->resume());
  assert(!retained->current());
  assert(!retained->registerCommands());
  assert(!retained->registrationQueue());
}
void actualHpFrontiers() {
  Fixture unknown;
  unknown.words.erase(Fixture::Battle + 0x163d4 + 4);
  unknown.start();
  assert(!unknown.owner->resume());
  assert(unknown.childPhase == 0);
  const auto trace = unknown.trace;
  assert(!unknown.owner->resume());
  assert(unknown.trace.size() == trace.size() + 1); // exact missing word only
  unknown.words[Fixture::Battle + 0x163d4 + 4] = 0;
  unknown.finish();
  // Genuine known Monosiri true must reach missing position/GPU work and hold;
  // never use untattled completion or native overlay as an HP acknowledgement.
  Fixture known;
  known.words[Fixture::Battle + 0x163f4] = 2;
  known.words[Fixture::Unit + 0x104] = 0;
  known.words[Fixture::Unit + 0x1c] = 1;
  known.start();
  assert(!known.owner->resume());
  assert(known.childPhase == 0);
}
} // namespace
int main() {
  exactHolds();
  admissionAndLifetimes();
  actualHpFrontiers();
  destroyedProviderContext();
  std::cout << "Harbor retained source display pass tests passed\n";
  return 0;
}

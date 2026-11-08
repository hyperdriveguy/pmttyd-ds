#include "harbor_source_display_pass.hpp"
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t EnemyHpCallback = 0x8011fde0;
constexpr uint32_t UiCameraIndex = 8;
constexpr uint32_t SourceCameraCount = 13;
struct Guard {
  bool &v;
  explicit Guard(bool &x) : v(x) { v = true; }
  ~Guard() { v = false; }
};
} // namespace
struct HarborSourceDisplayPass::State {
  enum class Phase { Idle, Collect, Main, Draw, Done };
  HarborSourceDisplayBinding binding;
  Phase phase = Phase::Idle;
  DisplayQueue queue;
  BattleFooterCursor registration;
  CameraMainCursor main;
  CameraDrawCursor draw;
  BattleEnemyHpDrawCursor hp;
  BattleMonosiriCursor monosiri;
  HarborSourceDisplayPass *owner = nullptr;
  uint64_t pass = 0, queueEpoch = 0;
  bool commands = false, retired = false, invalid = false;
  uint32_t pending = 0;
  bool valid() { return owner->current(); }
  template <class F> bool effect(F f) {
    if (!valid())
      return false;
    const bool result = f();
    return valid() && result;
  }
  static State &self(void *u) { return *static_cast<State *>(u); }
  static bool mainCallback(void *u, uint32_t fn, uint32_t index,
                           CameraState &c) {
    auto &s = self(u);
    s.pending = fn;
    return s.effect([&] {
      return s.binding.main.callback &&
             s.binding.main.callback(s.binding.main.user, fn, index, c);
    });
  }
  static std::optional<CameraViewMatrix> lookAt(void *u, const CameraState &c) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.main.lookAt
                 ? s.binding.main.lookAt(s.binding.main.user, c)
                 : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static bool zoom(void *u, CameraState &c) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.main.fieldZoom &&
             s.binding.main.fieldZoom(s.binding.main.user, c);
    });
  }
  static bool letterbox(void *u, DisplayQueue &q) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.draw.letterbox &&
             s.binding.draw.letterbox(s.binding.draw.user, q);
    });
  }
  static bool nearFar(void *u, CameraState &c) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.draw.nearFar &&
             s.binding.draw.nearFar(s.binding.draw.user, c);
    });
  }
  static bool request(void *u, const CameraDrawRequest &q) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.draw.request &&
             s.binding.draw.request(s.binding.draw.user, q);
    });
  }
  static bool displayState(void *u, const DisplayStateRequest &q) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.draw.display.state &&
             s.binding.draw.display.state(s.binding.draw.display.user, q);
    });
  }
  static std::optional<uint32_t> word(void *u, uint32_t a) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.hp.word ? s.binding.hp.word(s.binding.hp.user, a)
                               : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static std::optional<int32_t> switchFlag(void *u, int32_t id) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.monosiri.switchFlag
                 ? s.binding.monosiri.switchFlag(s.binding.monosiri.user, id)
                 : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static std::optional<int32_t> checkMonosiri(void *u, uint32_t unit) {
    auto &s = self(u);
    BattleMonosiriServices services{u, word, switchFlag};
    return checkBattleMonosiri(unit, services, s.monosiri);
  }
  static bool hpCamera(void *u, uint32_t index) {
    auto &s = self(u);
    return s.valid() && index == UiCameraIndex &&
           s.binding.cameras->currentIndex() == index &&
           s.draw.index == index && s.draw.stage == 8 &&
           s.binding.cameras->camera(index)->viewPublished;
  }
  static bool dispatch(void *u, uint32_t camera, uint32_t fn, uint32_t battle) {
    auto &s = self(u);
    if (!s.valid())
      return false;
    s.pending = fn;
    if (fn == EnemyHpCallback) {
      if (camera != UiCameraIndex || battle != s.binding.battle)
        return false;
      auto hp = s.binding.hp;
      hp.camera = hpCamera;
      hp.monosiri = checkMonosiri;
      // Existing leaf callbacks retain their original userdata; only composed
      // camera/Monosiri need this stable owner. Forward other leaves below.
      hp.user = &s;
      hp.word = word;
      hp.half = half;
      hp.status = status;
      hp.screenPoint = screen;
      hp.interpolate = interpolate;
      hp.truncate = truncate;
      hp.number = number;
      hp.bar = bar;
      const bool done = drawBattleEnemyHp(battle, hp, s.hp);
      s.pending = done ? 0 : s.hp.pending;
      return s.valid() && done;
    }
    if (!s.binding.callback)
      return false;
    return s.effect([&] {
      return s.binding.callback({s.binding.generation, s.pass, camera, fn,
                                 battle, s.main, s.draw, s.queue});
    });
  }
  static std::optional<uint16_t> half(void *u, uint32_t a) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.hp.half ? s.binding.hp.half(s.binding.hp.user, a)
                               : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static std::optional<int32_t> status(void *u, uint32_t a, int32_t id) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.hp.status ? s.binding.hp.status(s.binding.hp.user, a, id)
                                 : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static bool screen(void *u, const std::array<float, 3> &a,
                     std::array<float, 3> &b) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.hp.screenPoint &&
             s.binding.hp.screenPoint(s.binding.hp.user, a, b);
    });
  }
  static std::optional<float> interpolate(void *u, int32_t m, int32_t t,
                                          int32_t d, float a, float b) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.hp.interpolate
                 ? s.binding.hp.interpolate(s.binding.hp.user, m, t, d, a, b)
                 : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static std::optional<int32_t> truncate(void *u, float x) {
    auto &s = self(u);
    if (!s.valid())
      return {};
    auto v = s.binding.hp.truncate ? s.binding.hp.truncate(s.binding.hp.user, x)
                                   : std::nullopt;
    return s.valid() ? v : std::nullopt;
  }
  static bool number(void *u, const CameraViewMatrix &m, int32_t n, uint32_t a,
                     uint32_t b) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.hp.number &&
             s.binding.hp.number(s.binding.hp.user, m, n, a, b);
    });
  }
  static bool bar(void *u, const std::array<float, 3> &p, uint32_t unit,
                  uint32_t color) {
    auto &s = self(u);
    return s.effect([&] {
      return s.binding.hp.bar &&
             s.binding.hp.bar(s.binding.hp.user, p, unit, color);
    });
  }
};
HarborSourceDisplayPass::HarborSourceDisplayPass() = default;
HarborSourceDisplayPass::~HarborSourceDisplayPass() = default;
std::unique_ptr<HarborSourceDisplayPass>
HarborSourceDisplayPass::create(HarborSourceDisplayBinding b) {
  if (!b.battle || !b.generation || !b.cameras || !b.current ||
      !b.registerCommands || b.runtime.expired() || b.draw.display.callback ||
      b.hp.camera || b.hp.monosiri)
    return {};
  auto out =
      std::unique_ptr<HarborSourceDisplayPass>(new HarborSourceDisplayPass);
  out->state_ = std::make_unique<State>();
  out->state_->binding = std::move(b);
  out->state_->owner = out.get();
  return out;
}
bool HarborSourceDisplayPass::current() const {
  if (!state_ || checking_)
    return false;
  Guard check(checking_);
  auto &s = *state_;
  if (s.retired || s.invalid)
    return false;
  auto runtime = s.binding.runtime.lock();
  if (!runtime || !s.binding.current()) {
    s.invalid = true;
    return false;
  }
  // Missing actual field-source receipt is an unavailable frontier, not stale.
  if (!s.binding.cameraReceipt.lifetime || !s.binding.cameraReceipt.current)
    return false;
  if (!s.binding.cameraReceipt.current(*s.binding.cameras)) {
    s.invalid = true;
    return false;
  }
  if (s.phase != State::Phase::Idle && s.phase != State::Phase::Done &&
      s.queue.resetEpoch() != s.queueEpoch) {
    s.invalid = true;
    return false;
  }
  return !s.retired;
}
bool HarborSourceDisplayPass::admitCameraReceipt(
    HarborBattleCameraReceipt receipt) {
  if (invoking_ || checking_ || !state_ || !receipt.lifetime ||
      !receipt.current)
    return false;
  Guard guard(invoking_);
  auto &s = *state_;
  auto runtime = s.binding.runtime.lock();
  if (!runtime || s.retired || s.invalid || s.binding.cameraReceipt.lifetime ||
      s.binding.cameraReceipt.current || !s.binding.current())
    return false;
  if (!receipt.current(*s.binding.cameras) || s.retired)
    return false;
  s.binding.cameraReceipt = std::move(receipt);
  return true;
}
bool HarborSourceDisplayPass::begin(uint64_t pass) {
  if (invoking_ || !state_ || !pass || pass <= state_->pass)
    return false;
  Guard g(invoking_);
  auto runtime = state_->binding.runtime.lock();
  if (!runtime || !current())
    return false;
  auto &s = *state_;
  if (s.phase != State::Phase::Idle && s.phase != State::Phase::Done)
    return false;
  if (s.queue.count() != 0) {
    s.invalid = true;
    return false;
  }
  s.pass = pass;
  s.phase = State::Phase::Collect;
  s.queueEpoch = s.queue.resetEpoch();
  s.commands = false;
  s.registration = {};
  s.main = {};
  s.draw = {};
  s.hp = {};
  s.monosiri = {};
  s.pending = 0;
  return true;
}
bool HarborSourceDisplayPass::registerCommands() {
  if (invoking_ || !state_)
    return false;
  Guard g(invoking_);
  auto runtime = state_->binding.runtime.lock();
  if (!runtime || !current())
    return false;
  auto &s = *state_;
  if (s.phase != State::Phase::Collect)
    return false;
  if (s.commands)
    return true;
  s.commands = s.binding.registerCommands(s.queue, s.registration);
  return current() && s.commands;
}
DisplayQueue *HarborSourceDisplayPass::registrationQueue() {
  return !invoking_ && current() && state_->phase == State::Phase::Collect
             ? &state_->queue
             : nullptr;
}
bool HarborSourceDisplayPass::sealRegistrations() {
  if (invoking_ || !current() || state_->phase != State::Phase::Collect ||
      !state_->commands || state_->registration.active)
    return false;
  state_->phase = State::Phase::Main;
  return true;
}
bool HarborSourceDisplayPass::resume() {
  if (invoking_ || !state_)
    return false;
  Guard g(invoking_);
  auto runtime = state_->binding.runtime.lock();
  if (!runtime || !current())
    return false;
  auto &s = *state_;
  if (s.phase == State::Phase::Done)
    return true;
  if (s.phase == State::Phase::Main) {
    CameraMainServices services{&s, State::mainCallback, State::lookAt,
                                State::zoom};
    if (!s.binding.cameras->main(false, services, s.main) || !current())
      return false;
    s.phase = State::Phase::Draw;
  }
  if (s.phase != State::Phase::Draw)
    return false;
  CameraDrawServices services{&s,
                              State::letterbox,
                              State::nearFar,
                              State::request,
                              {&s, State::displayState, State::dispatch}};
  if (!s.binding.cameras->draw(s.queue, services, s.draw))
    return false;
  // Source Draw resets the queue LAST; accept that one completed reset, never
  // compare it against the held epoch as though it were external replacement.
  s.phase = State::Phase::Done;
  s.pending = 0;
  return current();
}
bool HarborSourceDisplayPass::complete(uint64_t p) const {
  return current() && p == state_->pass && state_->phase == State::Phase::Done;
}
bool HarborSourceDisplayPass::cameraMainCurrent(
    BattleClassCameraEpoch e) const {
  return current() && e.generation == state_->binding.generation &&
         e.pass == state_->pass && state_->phase == State::Phase::Draw &&
         !state_->main.active && state_->main.index == SourceCameraCount;
}
uint32_t HarborSourceDisplayPass::pending() const {
  return state_ ? state_->pending : 0;
}
uint64_t HarborSourceDisplayPass::pass() const {
  return state_ ? state_->pass : 0;
}
void HarborSourceDisplayPass::retire() {
  if (state_)
    state_->retired = true;
}
} // namespace ttyd

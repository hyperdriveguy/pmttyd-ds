#include "battle_enemy_hp_frame.hpp"
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float f) {
  uint32_t n;
  std::memcpy(&n, &f, 4);
  return n;
}
} // namespace
struct BattleEnemyHpFrame::State {
  uint64_t allocation = 0, lastPass = 0, completed = 0, pass = 0, queueEpoch = 0;
  BattleEnemyHpFrameServices source;
  std::function<bool()> current;
  bool invalid = false, held = false, running = false, retireRequested = false;
  const DisplayDispatchCursor *dispatch = nullptr;
  const DisplayQueue *queue = nullptr;
  const DisplayRecord *record = nullptr;
  DisplayRecord captured;
  uint32_t dispatchIndex = 0;
  QueuedBattleEnemyHpServices linked;
  QueuedBattleEnemyHpCursor parent;
  SourceIconNumberServices number;
  SourceIconGXServices actualGx, gx;
  SourceIconNumberCursor numberCursor;
  BattleTexturePlaneServices raster;
  BattleEnemyHpRasterCursor rasterCursor;
  bool numberActive = false;
  const CameraViewMatrix *numberBase = nullptr;
  int32_t numberValue = 0, numberWidth = 0;
  uint32_t numberMode = 0, numberColor = 0;
  bool live() {
    if (invalid || retireRequested)
      return false;
    if (!current || !current()) {
      invalid = true;
      return false;
    }
    return true;
  }
  bool matches() const {
    if (!dispatch || !queue || !queueEpoch || queue->resetEpoch()!=queueEpoch)
      return false;
    const auto &d = *dispatch;
    const auto *r = queue->current();
    const bool valid =
        d.active && d.invoking && d.camera == 8 && d.callback == 0x8011fde0 &&
        d.userdata == captured.userdata && d.index == dispatchIndex &&
        r == record && r && r->camera == captured.camera &&
        r->drawClass == captured.drawClass && r->padding == captured.padding &&
        bits(r->key) == bits(captured.key) &&
        r->callback == captured.callback && r->userdata == captured.userdata;
    return valid;
  }
  bool invocation() {
    if (!live() || !held)
      return false;
    const bool valid = matches();
    if (!valid)
      invalid = true;
    return valid;
  }
  template <class T> std::optional<T> checked(std::optional<T> result) {
    return invocation() ? result : std::nullopt;
  }
  bool checked(bool result) { return invocation() && result; }
  void link() {
    linked.parent.user = this;
    linked.parent.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.parent;
      return c.checked(s.word ? s.word(s.user, a) : std::nullopt);
    };
    linked.parent.half = [](void *u, uint32_t a) -> std::optional<uint16_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.parent;
      return c.checked(s.half ? s.half(s.user, a) : std::nullopt);
    };
    linked.parent.camera = [](void *u, uint32_t a) {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return false;
      auto &s = c.source.parent;
      return c.checked(s.camera && s.camera(s.user, a));
    };
    linked.parent.status = [](void *u, uint32_t a,
                              int32_t b) -> std::optional<int32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.parent;
      return c.checked(s.status ? s.status(s.user, a, b) : std::nullopt);
    };
    linked.parent.monosiri = [](void *u, uint32_t a) -> std::optional<int32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.parent;
      return c.checked(s.monosiri ? s.monosiri(s.user, a) : std::nullopt);
    };
    linked.parent.screenPoint = [](void *u, const std::array<float, 3> &a,
                                   std::array<float, 3> &b) {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return false;
      auto &s = c.source.parent;
      std::array<float, 3> out;
      const bool ok = s.screenPoint && s.screenPoint(s.user, a, out);
      if (!c.checked(ok))
        return false;
      b = out;
      return true;
    };
    linked.parent.interpolate = [](void *u, int32_t a, int32_t b, int32_t d,
                                   float x, float y) -> std::optional<float> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.parent;
      return c.checked(s.interpolate ? s.interpolate(s.user, a, b, d, x, y)
                                     : std::nullopt);
    };
    linked.parent.truncate = [](void *u, float f) -> std::optional<int32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.parent;
      return c.checked(s.truncate ? s.truncate(s.user, f) : std::nullopt);
    };
    linked.parent.number = [](void *u, const CameraViewMatrix &m, int32_t n,
                              uint32_t mode, uint32_t rgba) {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return false;
      if (!c.numberActive) {
        c.numberActive = true;
        c.numberBase = &m;
        c.numberValue = n;
        c.numberMode = mode;
        c.numberColor = rgba;
      } else if (c.numberBase != &m || c.numberValue != n ||
                 c.numberMode != mode || c.numberColor != rgba) {
        c.invalid = true;
        return false;
      }
      if (!resumeSourceIconNumber(m, n, mode, c.numberColor, c.number,
                                  c.numberCursor, c.numberWidth))
        return false;
      if (!c.invocation())
        return false;
      c.numberActive = false;
      c.numberBase = nullptr;
      return true;
    };
    linked.bar.actors = source.bar.actors;
    linked.bar.user = this;
    linked.bar.camera = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.bar;
      return c.checked(s.camera ? s.camera(s.user, a) : std::nullopt);
    };
    linked.bar.request = [](void *u, const BattleEnemyHpRasterRequest &q) {
      auto &c = *static_cast<State *>(u);
      return c.invocation() &&
             c.checked(resumeBattleEnemyHpRaster(q, c.raster, c.rasterCursor));
    };
    number.icons.user = this;
    number.icons.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.number.icons;
      return c.checked(s.word ? s.word(s.user, a) : std::nullopt);
    };
    number.icons.half = [](void *u, uint32_t a) -> std::optional<uint16_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.number.icons;
      return c.checked(s.half ? s.half(s.user, a) : std::nullopt);
    };
    number.icons.currentCamera = [](void *u) -> std::optional<uint32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.number.icons;
      return c.checked(s.currentCamera ? s.currentCamera(s.user)
                                       : std::nullopt);
    };
    number.icons.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return false;
      auto &s = c.source.number.icons;
      return c.checked(s.matrix && s.matrix(s.user, q));
    };
    if (source.number.icons.draw)
      number.icons.draw = [](void *u, const BattleActMatrix &m,
                             const SourceMatrixIcon &i) {
        auto &c = *static_cast<State *>(u);
        if (!c.invocation())
          return false;
        auto &s = c.source.number.icons;
        return c.checked(s.draw && s.draw(s.user, m, i));
      };
    if (source.number.icons.gx) {
      actualGx = *source.number.icons.gx;
      gx.user = this;
      gx.word = [](void *u, uint32_t a) -> std::optional<uint32_t> {
        auto &c = *static_cast<State *>(u);
        if (!c.invocation())
          return {};
        auto &s = c.actualGx;
        return c.checked(s.word ? s.word(s.user, a) : std::nullopt);
      };
      gx.half = [](void *u, uint32_t a) -> std::optional<uint16_t> {
        auto &c = *static_cast<State *>(u);
        if (!c.invocation())
          return {};
        auto &s = c.actualGx;
        return c.checked(s.half ? s.half(s.user, a) : std::nullopt);
      };
      gx.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
        auto &c = *static_cast<State *>(u);
        if (!c.invocation())
          return false;
        auto &s = c.actualGx;
        return c.checked(s.matrix && s.matrix(s.user, q));
      };
      gx.call = [](void *u,
                   const SourceIconGXCall &q) -> std::optional<uint32_t> {
        auto &c = *static_cast<State *>(u);
        if (!c.invocation())
          return {};
        auto &s = c.actualGx;
        return c.checked(s.call ? s.call(s.user, q) : std::nullopt);
      };
      gx.quad = [](void *u, const SourceIconGXQuad &q) {
        auto &c = *static_cast<State *>(u);
        if (!c.invocation())
          return false;
        auto &s = c.actualGx;
        return c.checked(s.quad && s.quad(s.user, q));
      };
      number.icons.gx = &gx;
    }
    raster.actors = source.raster.actors;
    raster.user = this;
    raster.request =
        [](void *u,
           const BattleTexturePlaneRequest &q) -> std::optional<uint32_t> {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return {};
      auto &s = c.source.raster;
      return c.checked(s.request ? s.request(s.user, q) : std::nullopt);
    };
    raster.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &c = *static_cast<State *>(u);
      if (!c.invocation())
        return false;
      auto &s = c.source.raster;
      return c.checked(s.matrix && s.matrix(s.user, q));
    };
  }
};
BattleEnemyHpFrame::~BattleEnemyHpFrame() { retire(); }
bool BattleEnemyHpFrame::bind(uint64_t allocation,
                              BattleEnemyHpFrameServices source,
                              std::function<bool()> current) {
  if (state_ || !allocation || !current || source.parent.number ||
      source.parent.bar || source.bar.request)
    return false;
  state_ = new State;
  state_->allocation = allocation;
  state_->source = source;
  state_->current = std::move(current);
  state_->link();
  return true;
}
bool BattleEnemyHpFrame::resume(uint64_t pass,
                                const DisplayDispatchCursor &dispatch,
                                const DisplayQueue &queue) {
  if (!state_ || state_->running || !pass)
    return false;
  auto &c = *state_;
  if (!c.live())
    return false;
  if (c.held) {
    if (c.pass != pass || c.dispatch != &dispatch || c.queue != &queue)
      return false;
  } else {
    if (pass == c.completed)
      return c.dispatch == &dispatch && c.queue == &queue && c.matches();
    if (pass <= c.lastPass || !dispatch.active || !dispatch.invoking ||
        dispatch.callback != 0x8011fde0 || dispatch.camera != 8)
      return false;
    const auto *record = queue.current();
    if (!queue.resetEpoch() || !record || record->camera != 8 || record->callback != 0x8011fde0 ||
        record->userdata != dispatch.userdata)
      return false;
    c.pass = c.lastPass = pass;
    c.dispatch = &dispatch;
    c.queue = &queue;
    c.record = record;
    c.captured = *record;
    c.dispatchIndex = dispatch.index;
    c.queueEpoch = queue.resetEpoch();
    c.held = true;
  }
  if (!c.invocation())
    return false;
  c.running = true;
  const bool done = resumeQueuedBattleEnemyHp(dispatch, c.linked, c.parent);
  const bool valid = c.invocation();
  c.running = false;
  if (c.retireRequested) {
    delete state_;
    state_ = nullptr;
    return false;
  }
  if (!done || !valid)
    return false;
  c.held = false;
  c.completed = pass;
  return true;
}
bool BattleEnemyHpFrame::complete(uint64_t pass) const {
  return state_ && !state_->held && state_->completed == pass && pass &&
         state_->live();
}
bool BattleEnemyHpFrame::active() const { return state_ && state_->held; }
uint32_t BattleEnemyHpFrame::pending() const {
  return !state_                         ? 0
         : state_->rasterCursor.active() ? state_->rasterCursor.pending()
         : state_->numberActive          ? 0x80019120
                                         : state_->parent.pending();
}
void BattleEnemyHpFrame::retire() {
  if (!state_)
    return;
  if (state_->running) {
    state_->invalid = true;
    state_->retireRequested = true;
    return;
  }
  delete state_;
  state_ = nullptr;
}
} // namespace ttyd

#include "battle_enemy_hp_presentation.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float add(float a, float b) {
  volatile float out = a + b;
  return out;
}
float sub(float a, float b) {
  volatile float out = a - b;
  return out;
}
float real(uint32_t n) {
  float v;
  std::memcpy(&v, &n, 4);
  return v;
}
bool same(const BattleEnemyHpBarServices &a,
          const BattleEnemyHpBarServices &b) {
  const auto &x = a.actors.memory;
  const auto &y = b.actors.memory;
  return a.user == b.user && a.camera == b.camera && a.request == b.request &&
         a.actors.user == b.actors.user && a.actors.byte == b.actors.byte &&
         x.user == y.user && x.word == y.word && x.half == y.half;
}
bool same(const BattleEnemyHpDrawServices &a,
          const BattleEnemyHpDrawServices &b) {
  return a.user == b.user && a.word == b.word && a.half == b.half &&
         a.camera == b.camera && a.status == b.status &&
         a.monosiri == b.monosiri && a.screenPoint == b.screenPoint &&
         a.interpolate == b.interpolate && a.truncate == b.truncate &&
         a.number == b.number && a.bar == b.bar;
}
} // namespace
struct BattleEnemyHpBarCursor::State {
  BattleEnemyHpBarServices owner;
  std::array<float, 3> *position = nullptr;
  const uint32_t *rgba = nullptr;
  uint32_t unit = 0, camera = 0, phase = 0, pending = 0, step = 0;
  bool queued = false;
  CameraViewMatrix matrix{};
  BattleEnemyHpRasterRequest request;
};
BattleEnemyHpBarCursor::BattleEnemyHpBarCursor() = default;
BattleEnemyHpBarCursor::~BattleEnemyHpBarCursor() = default;
bool BattleEnemyHpBarCursor::active() const { return bool(state_); }
uint32_t BattleEnemyHpBarCursor::pending() const {
  return state_ ? state_->pending : 0;
}
void BattleEnemyHpBarCursor::reset() { state_.reset(); }
bool resumeBattleEnemyHpBar(std::array<float, 3> &position, uint32_t unit,
                            const uint32_t &rgba,
                            const BattleEnemyHpBarServices &s,
                            BattleEnemyHpBarCursor &cursor) {
  if (cursor.state_) {
    const auto &c = *cursor.state_;
    if (c.position != &position || c.rgba != &rgba || c.unit != unit ||
        !same(c.owner, s))
      return false;
  } else {
    if (!s.actors.memory.word || !s.actors.memory.half)
      return false;
    cursor.state_ = std::make_unique<BattleEnemyHpBarCursor::State>();
    cursor.state_->owner = s;
    cursor.state_->position = &position;
    cursor.state_->rgba = &rgba;
    cursor.state_->unit = unit;
  }
  auto &c = *cursor.state_;
  const auto &m = s.actors.memory;
  const auto word = [&](uint32_t a) { return m.word(m.user, a); };
  const auto f = [&](uint32_t a) { return real(word(a)); };
  const auto send = [&](uint32_t function, auto build) {
    if (!c.queued) {
      c.request = build();
      c.request.function = function;
      c.queued = true;
    }
    c.pending = function;
    if (!s.request || !s.request(s.user, c.request))
      return false;
    c.queued = false;
    return true;
  };
  const auto matrix = [&]() {
    BattleEnemyHpRasterRequest q;
    for (unsigned row = 0; row < 3; ++row)
      for (unsigned col = 0; col < 4; ++col)
        c.matrix[row][col] = f(c.camera + 0x11c + 16 * row + 4 * col);
    q.integers = {0};
    q.matrix = c.matrix;
    return q;
  };
  for (;;)
    switch (c.phase) {
    case 0: {
      c.pending = 0x8000f2fc;
      if (!s.camera)
        return false;
      const auto v = s.camera(s.user, 8);
      if (!v)
        return false;
      c.camera = *v;
      c.phase = 1;
      break;
    }
    case 1:
      if (!send(0x80103b48, [] { return BattleEnemyHpRasterRequest{}; }))
        return false;
      c.phase = 2;
      break;
    case 2:
      if (!send(0x802ba2fc, matrix))
        return false;
      c.phase = 3;
      break;
    case 3:
      if (!send(0x801032c8, [&] {
            // Preserve source ordered reads/arithmetic, including the private X
            // store.
            const float x0 = position[0], ten = f(0x804225cc),
                        one = f(0x804225dc);
            const float x = sub(x0, ten), nineteen = f(0x804225d0);
            const auto color = rgba;
            const float y0 = position[1], three = f(0x804225d4);
            const float center = add(nineteen, x);
            position[0] = x;
            const float y = sub(y0, three), z = f(0x804225d8);
            BattleEnemyHpRasterRequest q;
            q.integers = {17, color, 0};
            q.reals = {center, y, z, one, one};
            return q;
          }))
        return false;
      c.phase = 4;
      break;
    case 4:
      if (m.half(m.user, unit + 0x1fc) == 1) {
        const auto raw = m.half(m.user, unit + 0x200);
        const int32_t count = raw < 32768 ? int32_t(raw) : int32_t(raw) - 65536;
        c.phase = count > 0 ? 5 : 8;
      } else
        c.phase = 9;
      break;
    case 5:
      if (!send(0x80103b48, [] { return BattleEnemyHpRasterRequest{}; }))
        return false;
      c.phase = 6;
      break;
    case 6:
      if (!send(0x802ba2fc, matrix))
        return false;
      c.phase = 7;
      break;
    case 7:
      if (!send(0x80103918, [&] {
            const float two = f(0x804225e0), y = position[1],
                        one = f(0x804225dc), x = position[0];
            const float top = add(two, y), eight = f(0x804225e8),
                        left = add(one, x), width = f(0x804225e4),
                        ratio = f(unit + 0x214), bottom = sub(top, eight);
            const auto alpha = uint8_t(rgba);
            const float right = std::fma(width, ratio, left);
            BattleEnemyHpRasterRequest q;
            q.integers = {0, 0, 0, alpha};
            q.reals = {left, top, right, bottom};
            return q;
          }))
        return false;
      c.phase = 10;
      break;
    case 10:
      if (!send(0x80103b48, [] { return BattleEnemyHpRasterRequest{}; }))
        return false;
      c.phase = 11;
      break;
    case 11:
      if (!send(0x802ba2fc, matrix))
        return false;
      c.phase = 8;
      break;
    case 8:
    case 9: {
      const bool modeOne = c.phase == 8;
      if (!send(0x801032c8, [&] {
            BattleEnemyHpRasterRequest q;
            uint32_t color;
            float ratio, one, nineteen, x, remainder, width, y, three, z;
            if (modeOne) {
              color = rgba;
              one = f(0x804225dc);
              nineteen = f(0x804225d0);
              ratio = f(unit + 0x210);
              x = position[0];
              remainder = sub(one, ratio);
              width = f(0x804225ec);
              x = add(nineteen, x);
              y = position[1];
              three = f(0x804225d4);
              x = std::fma(-width, remainder, x);
              z = f(0x804225d8);
              y = sub(y, three);
            } else {
              ratio = f(unit + 0x210);
              one = f(0x804225dc);
              nineteen = f(0x804225d0);
              x = position[0];
              remainder = sub(one, ratio);
              color = rgba;
              x = add(nineteen, x);
              width = f(0x804225ec);
              y = position[1];
              three = f(0x804225d4);
              x = std::fma(-width, remainder, x);
              y = sub(y, three);
              z = f(0x804225d8);
            }
            q.integers = {18, color, 0};
            q.reals = {x, y, z, ratio, one};
            return q;
          }))
        return false;
      cursor.reset();
      return true;
    }
    default:
      return false;
    }
}
struct QueuedBattleEnemyHpCursor::State {
  QueuedBattleEnemyHpServices owner;
  BattleEnemyHpDrawServices bridge;
  BattleEnemyHpDrawCursor parent;
  BattleEnemyHpBarCursor bar;
  std::array<float, 3> barPoint{};
  uint32_t barColor = 0, camera = 0, battle = 0;
};
QueuedBattleEnemyHpCursor::QueuedBattleEnemyHpCursor() = default;
QueuedBattleEnemyHpCursor::~QueuedBattleEnemyHpCursor() = default;
bool QueuedBattleEnemyHpCursor::active() const { return bool(state_); }
uint32_t QueuedBattleEnemyHpCursor::pending() const {
  return state_ ? (state_->bar.active() ? state_->bar.pending()
                                        : state_->parent.pending)
                : 0;
}
void QueuedBattleEnemyHpCursor::reset() { state_.reset(); }
bool resumeQueuedBattleEnemyHp(const DisplayDispatchCursor &invocation,
                               const QueuedBattleEnemyHpServices &source,
                               QueuedBattleEnemyHpCursor &cursor) {
  if (!invocation.active || !invocation.invoking ||
      invocation.callback != 0x8011fde0 || invocation.camera != 8)
    return false;
  if (cursor.state_) {
    const auto &c = *cursor.state_;
    if (c.camera != invocation.camera || c.battle != invocation.userdata ||
        !same(c.owner.parent, source.parent) || !same(c.owner.bar, source.bar))
      return false;
  } else {
    cursor.state_ = std::make_unique<QueuedBattleEnemyHpCursor::State>();
    auto &c = *cursor.state_;
    c.owner = source;
    c.camera = invocation.camera;
    c.battle = invocation.userdata;
    c.bridge.user = &c;
    using S = QueuedBattleEnemyHpCursor::State;
    c.bridge.word = [](void *p, uint32_t a) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.word ? s.word(s.user, a) : std::nullopt;
    };
    c.bridge.half = [](void *p, uint32_t a) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.half ? s.half(s.user, a) : std::nullopt;
    };
    c.bridge.camera = [](void *p, uint32_t a) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.camera && s.camera(s.user, a);
    };
    c.bridge.status = [](void *p, uint32_t a, int32_t b) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.status ? s.status(s.user, a, b) : std::nullopt;
    };
    c.bridge.monosiri = [](void *p, uint32_t a) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.monosiri ? s.monosiri(s.user, a) : std::nullopt;
    };
    c.bridge.screenPoint = [](void *p, const std::array<float, 3> &a,
                              std::array<float, 3> &b) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.screenPoint && s.screenPoint(s.user, a, b);
    };
    c.bridge.interpolate = [](void *p, int32_t a, int32_t b, int32_t d, float x,
                              float y) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.interpolate ? s.interpolate(s.user, a, b, d, x, y)
                           : std::nullopt;
    };
    c.bridge.truncate = [](void *p, float v) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.truncate ? s.truncate(s.user, v) : std::nullopt;
    };
    c.bridge.number = [](void *p, const CameraViewMatrix &a, int32_t n,
                         uint32_t mode, uint32_t color) {
      auto &s = static_cast<S *>(p)->owner.parent;
      return s.number && s.number(s.user, a, n, mode, color);
    };
    c.bridge.bar = [](void *p, const std::array<float, 3> &point, uint32_t unit,
                      uint32_t color) {
      auto &c = *static_cast<S *>(p);
      if (!c.bar.active()) {
        c.barPoint = point;
        c.barColor = color;
      }
      return resumeBattleEnemyHpBar(c.barPoint, unit, c.barColor, c.owner.bar,
                                    c.bar);
    };
  }
  auto &c = *cursor.state_;
  if (!drawBattleEnemyHp(c.battle, c.bridge, c.parent))
    return false;
  cursor.reset();
  return true;
}
} // namespace ttyd

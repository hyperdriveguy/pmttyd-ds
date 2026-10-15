#include "battle_enemy_hp_presentation.hpp"
#include <cassert>
#include <cstring>
#include <map>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t U = 0x300000, B = 0x100000, Cam = 0x400000;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  std::vector<uint32_t> reads;
  std::vector<BattleEnemyHpRasterRequest> calls;
  unsigned accepted = 0, target = ~0u, waits = 0, cameras = 0, screens = 0,
           numbers = 0;
  int32_t number = 0;
  bool numberReady = true, fillReady = true;
  void floating(uint32_t a, float v) {
    uint32_t n;
    std::memcpy(&n, &v, 4);
    words[a] = n;
  }
  Fixture() {
    for (auto [a, v] : std::map<uint32_t, float>{{0x804225cc, 10},
                                                 {0x804225d0, 19},
                                                 {0x804225d4, 3},
                                                 {0x804225d8, 0},
                                                 {0x804225dc, 1},
                                                 {0x804225e0, 2},
                                                 {0x804225e4, 34},
                                                 {0x804225e8, 8},
                                                 {0x804225ec, 20}})
      floating(a, v);
    floating(U + 0x210, .5f);
    floating(U + 0x214, .75f);
    for (unsigned row = 0; row < 3; ++row)
      for (unsigned col = 0; col < 4; ++col)
        floating(Cam + 0x11c + 16 * row + 4 * col, row == col ? 1 : 0);
    words[B + 0x20] = U;
    words[U + 0x1c] = 1;
    floating(U + 0x3c, 50);
    floating(U + 0x40, 80);
    floating(U + 0x44, 9);
    halves[U + 0x10c] = 5;
    halves[U + 0x108] = 10;
  }
  bool pause() {
    if (accepted == target && waits++ < 2)
      return true;
    ++accepted;
    return false;
  }
  BattleEnemyHpBarServices bar() {
    BattleEnemyHpBarServices s;
    s.user = this;
    s.actors.memory.user = this;
    s.actors.memory.word = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.words[a];
    };
    s.actors.memory.half = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.halves[a];
    };
    s.camera = [](void *p, uint32_t i) -> std::optional<uint32_t> {
      assert(i == 8);
      auto &f = *static_cast<Fixture *>(p);
      if (f.pause())
        return {};
      ++f.cameras;
      return Cam;
    };
    s.request = [](void *p, const BattleEnemyHpRasterRequest &r) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.pause())
        return false;
      if (r.function == 0x801032c8 && r.integers[0] == 18 && !f.fillReady)
        return false;
      f.calls.push_back(r);
      return true;
    };
    return s;
  }
  QueuedBattleEnemyHpServices queued() {
    QueuedBattleEnemyHpServices s;
    s.bar = bar();
    s.parent.user = this;
    s.parent.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.parent.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      return static_cast<Fixture *>(p)->halves[a];
    };
    s.parent.camera = [](void *, uint32_t i) {
      assert(i == 8);
      return true;
    };
    s.parent.status = [](void *, uint32_t u,
                         int32_t id) -> std::optional<int32_t> {
      assert(u == U && id == 27);
      return 0;
    };
    s.parent.monosiri = [](void *, uint32_t u) -> std::optional<int32_t> {
      assert(u == U);
      return 1;
    };
    s.parent.screenPoint = [](void *p, const std::array<float, 3> &in,
                              std::array<float, 3> &out) {
      ++static_cast<Fixture *>(p)->screens;
      out = in;
      return true;
    };
    s.parent.number = [](void *p, const CameraViewMatrix &m, int32_t n,
                         uint32_t mode, uint32_t color) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.numbers;
      assert(mode == 1 && color == 0xffffffff && m[0][3] == 70 &&
             m[1][3] == 48);
      if (!f.numberReady)
        return false;
      f.number = n;
      return true;
    };
    return s;
  }
};
void equal(const Fixture &a, const Fixture &b) {
  assert(a.words == b.words && a.halves == b.halves && a.reads == b.reads &&
         a.cameras == b.cameras && a.calls.size() == b.calls.size());
  for (unsigned i = 0; i < a.calls.size(); ++i) {
    const auto &x = a.calls[i];
    const auto &y = b.calls[i];
    assert(x.function == y.function && x.integers == y.integers &&
           x.reals == y.reals && x.matrix == y.matrix);
  }
}
int main() {
  for (unsigned mode : {0u, 1u, 2u})
    for (uint16_t timer : {0u, 2u, 0x8000u}) {
      Fixture baseline;
      baseline.halves[Fixture::U + 0x1fc] = mode;
      baseline.halves[Fixture::U + 0x200] = timer;
      std::array<float, 3> point{50, 80, 9};
      uint32_t rgba = 0xaabbcc88;
      BattleEnemyHpBarCursor cursor;
      assert(resumeBattleEnemyHpBar(point, Fixture::U, rgba, baseline.bar(),
                                    cursor));
      assert((point == std::array<float, 3>{40, 80, 9}) && !cursor.active());
      assert(baseline.calls[2].function == 0x801032c8 &&
             baseline.calls[2].integers ==
                 std::vector<uint32_t>({17, rgba, 0}));
      assert(baseline.calls[2].reals == std::vector<float>({59, 77, 0, 1, 1}));
      assert(baseline.calls.back().integers ==
             std::vector<uint32_t>({18, rgba, 0}));
      assert(baseline.calls.back().reals ==
             std::vector<float>({49, 77, 0, .5f, 1}));
      if (mode == 1 && timer == 2) {
        assert(baseline.calls.size() == 9);
        assert(baseline.calls[5].function == 0x80103918 &&
               baseline.calls[5].integers ==
                   std::vector<uint32_t>({0, 0, 0, 0x88}));
        assert(baseline.calls[5].reals ==
               std::vector<float>({41, 82, 66.5f, 74}));
      } else
        assert(baseline.calls.size() == 4);
      for (unsigned target = 0; target < baseline.accepted; ++target) {
        Fixture held;
        held.target = target;
        held.halves = baseline.halves;
        std::array<float, 3> p{50, 80, 9};
        BattleEnemyHpBarCursor c;
        assert(!resumeBattleEnemyHpBar(p, Fixture::U, rgba, held.bar(), c));
        auto wrong = held.bar();
        wrong.user = &baseline;
        const auto reads = held.reads.size();
        assert(!resumeBattleEnemyHpBar(p, Fixture::U, rgba, wrong, c));
        assert(!resumeBattleEnemyHpBar(p, Fixture::U + 1, rgba, held.bar(), c));
        auto other = p;
        assert(!resumeBattleEnemyHpBar(other, Fixture::U, rgba, held.bar(), c));
        assert(held.reads.size() == reads);
        assert(!resumeBattleEnemyHpBar(p, Fixture::U, rgba, held.bar(), c));
        assert(resumeBattleEnemyHpBar(p, Fixture::U, rgba, held.bar(), c));
        equal(baseline, held);
      }
    }
  // Background color/coordinates are retained, later gauge/color reads are
  // live.
  Fixture live;
  live.target = 3;
  std::array<float, 3> point{50, 80, 9};
  uint32_t color = 0x11223344;
  BattleEnemyHpBarCursor bar;
  assert(!resumeBattleEnemyHpBar(point, Fixture::U, color, live.bar(), bar));
  assert(point[0] == 40);
  color = 0xaabbccdd;
  live.floating(Fixture::U + 0x210, .25f);
  assert(!resumeBattleEnemyHpBar(point, Fixture::U, color, live.bar(), bar));
  assert(resumeBattleEnemyHpBar(point, Fixture::U, color, live.bar(), bar));
  assert(live.calls[2].integers[1] == 0x11223344 &&
         live.calls.back().integers[1] == color);
  assert(live.calls.back().reals == std::vector<float>({44, 77, 0, .25f, 1}));
  // Actual invoking record admission; missing mode1 number owner cannot
  // complete.
  Fixture queued;
  queued.numberReady = false;
  QueuedBattleEnemyHpCursor q;
  DisplayDispatchCursor invocation;
  invocation.active = true;
  invocation.invoking = true;
  invocation.callback = 0x8011fde0;
  invocation.camera = 8;
  invocation.userdata = Fixture::B;
  auto wrong = invocation;
  wrong.callback = 1;
  assert(!resumeQueuedBattleEnemyHp(wrong, queued.queued(), q) && !q.active());
  assert(!resumeQueuedBattleEnemyHp(invocation, queued.queued(), q) &&
         q.pending() == 0x80019120 && queued.screens == 1 &&
         queued.calls.empty());
  queued.halves[Fixture::U + 0x10c] = 999;
  queued.numberReady = true;
  queued.fillReady = false;
  assert(!resumeQueuedBattleEnemyHp(invocation, queued.queued(), q) &&
         q.pending() == 0x801032c8 && queued.number == 5 &&
         queued.screens == 1);
  const auto prefix = queued.calls.size();
  wrong = invocation;
  wrong.userdata++;
  assert(!resumeQueuedBattleEnemyHp(wrong, queued.queued(), q) &&
         queued.calls.size() == prefix);
  assert(!resumeQueuedBattleEnemyHp(invocation, queued.queued(), q) &&
         queued.calls.size() == prefix);
  queued.fillReady = true;
  assert(resumeQueuedBattleEnemyHp(invocation, queued.queued(), q) &&
         !q.active());
  assert(queued.screens == 1 && queued.numbers == 2 && queued.number == 5 &&
         queued.calls.back().reals[3] == .5f);
}

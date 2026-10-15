#include "battle_enemy_hp_frame.hpp"
#include "battle_audience_aux_callback.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace ttyd;
namespace {
uint32_t be(const std::vector<uint8_t> &b, size_t n) {
  return uint32_t(b.at(n)) << 24 | uint32_t(b.at(n + 1)) << 16 |
         uint32_t(b.at(n + 2)) << 8 | b.at(n + 3);
}
const std::vector<uint8_t> &data(bool directory) {
  const auto load = [](const char *p) {
    std::ifstream f(p, std::ios::binary);
    assert(f);
    return std::vector<uint8_t>{std::istreambuf_iterator<char>(f), {}};
  };
  static const auto pack = load("romfs/harbor-tutorial-events.pmevt"),
                    icons = load("romfs/source-icons/icon.bin");
  return directory ? icons : pack;
}
uint32_t bits(float f) {
  uint32_t n;
  std::memcpy(&n, &f, 4);
  return n;
}
struct Fixture {
  static constexpr uint32_t B = 0x100000, U = 0x200000, C = 0x300000,
                            D = 0x400000, Descriptor = 0x500000,
                            File = 0x600000, Palette = 0x700000;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint16_t> halves;
  BattleEnemyHpFrame frame;
  DisplayQueue queue;
  DisplayDispatchCursor dispatch;
  std::vector<std::string> trace, ordered;
  bool solid = false, useGX = false;
  SourceIconGXServices gx;
  std::vector<uint32_t> numeralColors;
  std::vector<uint16_t> numerals;
  unsigned accepted = 0, wordReads = 0, halfReads = 0;
  int stop = -1, invalidateAt = -1, retireAt = -1, reenterAt = -1;
  bool released = false, current = true, allowStatus = true, done = false;
  uint64_t pass = 1;
  Fixture() {
    for (unsigned i = 0; i < 64; ++i)
      words[B + 0x20 + 4 * i] = 0;
    words[B + 0x20] = U;
    words[0x8041eb90] = B;
    words[B + 0x163f4] = 2;
    words[B + 0x1720] = 0;
    words[B + 0xef4] = 0;
    words[U + 8] = 1;
    words[U + 0x1c] = 1;
    words[U + 0x104] = 0;
    words[U + 0x3c] = bits(50);
    words[U + 0x40] = bits(80);
    words[U + 0x44] = bits(9);
    words[U + 0x210] = bits(.5f);
    words[U + 0x214] = bits(.75f);
    halves[U + 0x10c] = 59;
    halves[U + 0x108] = 80;
    halves[U + 0xd4] = halves[U + 0xd6] = 0;
    halves[U + 0x1fe] = 0;
    halves[U + 0x1fc] = 1;
    halves[U + 0x200] = 2;
    words[B + 0x163f8] = Descriptor;
    words[Descriptor + 0xa0] = File;
    words[File] = Palette;
    words[0x8041e634] = words[0x8041e638] = D;
    words[0x8041e63c] = words[0x8041e630] = 1;
    for (unsigned i = 0; i < 12; ++i)
      words[C + 0x11c + 4 * i] = bits(i == 0 || i == 5 || i == 10 ? 1.f : 0.f);
  }
  std::optional<uint32_t> word(uint32_t a) {
    ++wordReads;
    const auto it = words.find(a);
    if (it != words.end())
      return it->second;
    const auto &b = data(false);
    for (unsigned i = 0; i < be(b, 8); ++i) {
      const auto base = be(b, 12 + 12 * i), size = be(b, 16 + 12 * i),
                 off = be(b, 20 + 12 * i);
      if (a >= base && uint64_t(a) - base + 4 <= size)
        return be(b, off + a - base);
    }
    return {};
  }
  std::optional<uint16_t> half(uint32_t a) {
    ++halfReads;
    const auto it = halves.find(a);
    if (it != halves.end())
      return it->second;
    if (a >= D && uint64_t(a) - D + 2 <= data(true).size()) {
      const auto &b = data(true);
      return uint16_t(uint16_t(b[a - D]) << 8 | b[a - D + 1]);
    }
    const auto n = word(a & ~3u);
    return n ? std::optional<uint16_t>(uint16_t(*n >> ((a & 2) ? 0 : 16)))
             : std::nullopt;
  }
  bool event(std::string name) {
    if (int(accepted) == stop && !released)
      return false;
    const auto ordinal = accepted++;
    trace.push_back(std::move(name));
    if (int(ordinal) == reenterAt)
      assert(!frame.resume(pass, dispatch, queue));
    if (int(ordinal) == invalidateAt)
      current = false;
    if (int(ordinal) == retireAt)
      frame.retire();
    return true;
  }
  BattleEnemyHpFrameServices services() {
    BattleEnemyHpFrameServices s;
    auto &p = s.parent;
    p.user = this;
    p.word = [](void *u, uint32_t a) {
      return static_cast<Fixture *>(u)->word(a);
    };
    p.half = [](void *u, uint32_t a) {
      return static_cast<Fixture *>(u)->half(a);
    };
    p.camera = [](void *u, uint32_t i) {
      assert(i == 8);
      return static_cast<Fixture *>(u)->event("parent-camera");
    };
    p.status = [](void *u, uint32_t unit,
                  int32_t status) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(u);
      assert((unit == U || unit == U + 0x1000) && status == 27);
      if (!f.allowStatus || !f.event("status"))
        return {};
      return 0;
    };
    p.monosiri = [](void *u, uint32_t unit) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("tattle"))
        return {};
      BattleMonosiriServices source;
      source.user = &f;
      source.word = [](void *v, uint32_t a) {
        return static_cast<Fixture *>(v)->word(a);
      };
      BattleMonosiriCursor cursor;
      return checkBattleMonosiri(unit, source, cursor);
    };
    p.screenPoint = [](void *u, const std::array<float, 3> &in,
                       std::array<float, 3> &out) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("projection"))
        return false;
      out = {in[0], in[1], 0};
      return true;
    };
    p.interpolate = [](void *u, int32_t, int32_t, int32_t, float,
                       float) -> std::optional<float> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("interpolate"))
        return {};
      return 128;
    };
    p.truncate = [](void *u, float f) -> std::optional<int32_t> {
      auto &owner = *static_cast<Fixture *>(u);
      if (!owner.event("truncate"))
        return {};
      return int32_t(std::trunc(f));
    };
    s.bar.user = this;
    s.bar.actors.memory.user = this;
    s.bar.actors.memory.word = [](void *u, uint32_t a) {
      const auto n = static_cast<Fixture *>(u)->word(a);
      assert(n);
      return *n;
    };
    s.bar.actors.memory.half = [](void *u, uint32_t a) {
      const auto n = static_cast<Fixture *>(u)->half(a);
      assert(n);
      return *n;
    };
    s.bar.camera = [](void *u, uint32_t i) -> std::optional<uint32_t> {
      assert(i == 8);
      return static_cast<Fixture *>(u)->event("bar-camera")
                 ? std::optional<uint32_t>(C)
                 : std::nullopt;
    };
    auto &icons = s.number.icons;
    icons.user = this;
    icons.word = p.word;
    icons.half = p.half;
    icons.currentCamera = [](void *u) -> std::optional<uint32_t> {
      return static_cast<Fixture *>(u)->event("number-camera")
                 ? std::optional<uint32_t>(C)
                 : std::nullopt;
    };
    icons.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("number-math" + std::to_string(q.function)))
        return false;
      BattleAudienceAuxServices math;
      bindAudienceAuxMathAdapter(math);
      return math.matrix(nullptr, q);
    };
    icons.draw = [](void *u, const BattleActMatrix &,
                    const SourceMatrixIcon &icon) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("number-draw"))
        return false;
      f.ordered.push_back("N");
      f.numerals.push_back(icon.id);
      f.numeralColors.push_back(icon.rgba);
      return true;
    };
    if (useGX) {
      gx = {};
      gx.user = this;
      gx.word = p.word;
      gx.half = p.half;
      gx.matrix = icons.matrix;
      gx.call = [](void *u,
                   const SourceIconGXCall &q) -> std::optional<uint32_t> {
        auto &f = *static_cast<Fixture *>(u);
        if (!f.event("iconGX" + std::to_string(q.function)))
          return {};
        if (q.function == 0x8000f2fc || q.function == 0x8000f2e4)
          return C;
        if (q.function == 0x80037a80) {
          assert(q.integers[0] == 1 && q.integers[1] < 613);
          return q.integers[1] + 1;
        }
        return 0;
      };
      gx.quad = [](void *u, const SourceIconGXQuad &) {
        auto &f = *static_cast<Fixture *>(u);
        if (!f.event("icon-quad"))
          return false;
        f.ordered.push_back("N");
        return true;
      };
      icons.draw = nullptr;
      icons.gx = &gx;
    }
    s.raster.actors = s.bar.actors;
    s.raster.user = this;
    s.raster.matrix = [](void *u, BattleAudienceMatrixRequest &q) {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("bar-math" + std::to_string(q.function)))
        return false;
      BattleAudienceAuxServices math;
      bindAudienceAuxMathAdapter(math);
      return math.matrix(nullptr, q);
    };
    s.raster.request =
        [](void *u,
           const BattleTexturePlaneRequest &q) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      if (!f.event("raster" + std::to_string(unsigned(q.kind)) + ":" +
                   std::to_string(uint32_t(q.function))))
        return {};
      if (q.kind != BattleTexturePlaneKind::Call)
        return 0;
      if (q.function == BattleTexturePlaneCall::TextureGenerators)
        f.solid = q.integers == std::vector<uint32_t>{0};
      if (q.function == BattleTexturePlaneCall::Begin && f.solid)
        f.ordered.push_back("S");
      switch (q.function) {
      case BattleTexturePlaneCall::CurrentCamera:
        return C;
      case BattleTexturePlaneCall::TextureObject:
        assert(q.integers[0] == Palette &&
               (q.integers[1] == 17 || q.integers[1] == 18));
        f.ordered.push_back("T" + std::to_string(q.integers[1]));
        return q.integers[1];
      case BattleTexturePlaneCall::TextureFormat:
        return 14;
      case BattleTexturePlaneCall::TextureWidth:
        return 40;
      case BattleTexturePlaneCall::TextureHeight:
        return 16;
      default:
        return 0;
      }
    };
    return s;
  }
  void bind() {
    assert(frame.bind(9, services(), [this] { return current; }));
  }
  bool run(bool enqueue = true) {
    if (enqueue)
      assert(queue.enter(8, 0, 0x8011fde0, B, 902) ==
             DisplayAdmission::Registered);
    queue.sort();
    DisplayDispatchServices service;
    service.user = this;
    service.state = [](void *, const DisplayStateRequest &) { return true; };
    service.callback = [](void *u, uint32_t, uint32_t, uint32_t) {
      auto &f = *static_cast<Fixture *>(u);
      f.done = f.frame.resume(f.pass, f.dispatch, f.queue);
      if (f.done) {
        const auto accepted = f.accepted;
        assert(f.frame.resume(f.pass, f.dispatch, f.queue));
        assert(f.accepted == accepted);
      }
      return f.done;
    };
    return queue.dispatch(8, service, dispatch);
  }
};
} // namespace
int main() {
  Fixture baseline;
  baseline.bind();
  const auto ready = baseline.run();
  if (!ready)
    std::cerr << "pending " << std::hex << baseline.frame.pending()
              << " effects " << std::dec << baseline.accepted << "\n";
  assert(ready);
  assert(baseline.frame.complete(1));
  assert((baseline.numerals == std::vector<uint16_t>{500, 492}));
  assert((baseline.numeralColors ==
          std::vector<uint32_t>{0xffffffff, 0xffffffff}));
  const auto count = baseline.accepted;
  for (unsigned stop = 0; stop < count; ++stop) {
    Fixture held;
    held.stop = int(stop);
    held.bind();
    assert(!held.run());
    assert(held.frame.active() && !held.frame.complete(1));
    const auto effects = held.accepted;
    assert(!held.run(false) && held.accepted == effects);
    auto replacement = held.dispatch;
    assert(!held.frame.resume(1, replacement, held.queue));
    assert(!held.frame.resume(2, held.dispatch, held.queue));
    held.released = true;
    assert(held.run(false));
    assert(held.trace == baseline.trace && held.numerals == baseline.numerals &&
           held.numeralColors == baseline.numeralColors);
    assert(held.frame.complete(1));
    // Same-pass duplicate acknowledgement was checked inside actual dispatch.
  }
  for (unsigned stop : {0u, count / 2, count - 1}) {
    Fixture stale;
    stale.invalidateAt = int(stop);
    stale.bind();
    assert(!stale.run());
    const auto done = stale.accepted;
    stale.current = true;
    assert(!stale.run(false));
    assert(stale.accepted == done && !stale.frame.complete(1));
    stale.frame.retire();
  }
  Fixture retired;
  retired.retireAt = 5;
  retired.bind();
  assert(!retired.run());
  assert(!retired.frame.active());
  retired.retireAt = -1;
  retired.bind();
  retired.queue.reinitialize();
  retired.dispatch = {};
  assert(retired.run());
  Fixture reentrant;
  reentrant.reenterAt = 5;
  reentrant.bind();
  assert(reentrant.run());
  assert(reentrant.trace == baseline.trace);
  Fixture gpu;
  gpu.useGX = true;
  gpu.bind();
  assert(gpu.run());
  assert(
      (gpu.ordered == std::vector<std::string>{"N", "N", "T17", "S", "T18"}));
  for (unsigned stop = 0; stop < gpu.accepted; ++stop) {
    Fixture held;
    held.useGX = true;
    held.stop = int(stop);
    held.bind();
    assert(!held.run());
    const auto effects = held.accepted;
    assert(!held.run(false) && held.accepted == effects);
    held.released = true;
    assert(held.run(false));
    assert(held.trace == gpu.trace && held.ordered == gpu.ordered);
  }
  // Native frame lease rejects reset/repopulation even when source keeps the
  // captured callback/current record and new bytes are identical (ABA).
  Fixture reset;reset.stop=5;reset.bind();assert(!reset.run());
  const auto epoch=reset.queue.resetEpoch();const auto effects=reset.accepted;
  const auto* record=reset.queue.current();reset.queue.reinitialize();
  assert(reset.queue.count()==0 && reset.queue.current()==record && reset.queue.resetEpoch()!=epoch);
  assert(reset.queue.enter(8,0,0x8011fde0,Fixture::B,902)==DisplayAdmission::Registered);reset.queue.sort();
  assert(reset.queue.current()==record);reset.released=true;
  assert(!reset.run(false) && reset.accepted==effects && !reset.frame.complete(1));
  Fixture multi;
  std::vector<std::pair<uint32_t, uint32_t>> unitWords;
  for (const auto &row : multi.words)
    if (row.first >= Fixture::U && row.first < Fixture::U + 0x300)
      unitWords.push_back(row);
  for (const auto &row : unitWords)
    multi.words[row.first + 0x1000] = row.second;
  std::vector<std::pair<uint32_t, uint16_t>> unitHalves;
  for (const auto &row : multi.halves)
    if (row.first >= Fixture::U && row.first < Fixture::U + 0x300)
      unitHalves.push_back(row);
  for (const auto &row : unitHalves)
    multi.halves[row.first + 0x1000] = row.second;
  multi.words[Fixture::B + 0x24] = Fixture::U + 0x1000;
  multi.halves[Fixture::U + 0x1000 + 0x10c] = 7;
  multi.bind();
  assert(multi.run());
  assert((multi.ordered == std::vector<std::string>{"N", "N", "T17", "S", "T18",
                                                    "N", "T17", "S", "T18"}));
  assert((multi.numerals == std::vector<uint16_t>{500, 492, 496}));
  const auto firstEffects = multi.accepted;
  multi.pass = 2;
  multi.queue.reinitialize();
  multi.dispatch = {};
  assert(multi.run() && multi.frame.complete(2) &&
         multi.accepted == 2 * firstEffects);
  multi.current = false;
  assert(!multi.frame.complete(2));
  multi.current = true;
  assert(!multi.frame.complete(2));
  Fixture unknown;
  unknown.allowStatus = false;
  unknown.bind();
  assert(!unknown.run() && unknown.numerals.empty());
  auto missing = unknown.services();
  missing.raster.request = nullptr;
  unknown.frame.retire();
  unknown.allowStatus = true;
  assert(unknown.frame.bind(10, missing, [&] { return unknown.current; }));
  unknown.queue.reinitialize();
  unknown.dispatch = {};
  assert(!unknown.run());
  assert(!unknown.frame.complete(1));
  Fixture invalidBind;
  auto services = invalidBind.services();
  services.parent.number = [](void *, const CameraViewMatrix &, int32_t,
                              uint32_t, uint32_t) { return true; };
  assert(!invalidBind.frame.bind(1, services, [] { return true; }));
  std::cout << "Queued whole HP source composition, stable numberRGBA, linked "
               "raster, waits and owner guards passed\n";
}

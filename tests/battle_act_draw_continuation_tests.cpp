#include "battle_act_draw_continuation.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <string>
using namespace ttyd;
namespace {
struct Call {
  std::string name;
  std::vector<float> operands;
  bool operator==(const Call &c) const {
    return name == c.name && operands == c.operands;
  }
};
struct Fixture {
  static constexpr uint32_t B = 0x10000, W = 0x20000, C = 0x30000, G = 0x40000;
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<Call> acknowledged, attempted;
  std::vector<std::array<uint32_t, 3>> memoryTrace;
  std::vector<BattleActDrawSlot> slots;
  size_t reads = 0, writes = 0;
  int hold = -1;
  bool released = false;
  void real(uint32_t p, float f) {
    uint32_t n;
    std::memcpy(&n, &f, 4);
    words[p] = n;
  }
  Fixture(uint32_t state, uint32_t mode = 0, uint32_t count = 3) {
    words[0x8041eb90] = B;
    words[B + 0x1c78] = W;
    words[W] = C;
    words[W + 4] = 0x50000;
    words[C + 8] = count;
    words[0x80415fd0] = G;
    words[G + 4] = 60;
    words[W + 0xc] = mode;
    const float constants[] = {0,
                               12,
                               3.1415927410125732f,
                               180,
                               270,
                               90,
                               360,
                               .01745329238474369f,
                               20,
                               -60,
                               55,
                               .1f,
                               1};
    for (unsigned i = 0; i < 13; ++i)
      real(0x8042242c + 4 * i, constants[i]);
    const float angles[] = {240, 120, 0,   0,   0,   270, 180, 90,
                            0,   0,   270, 203, 157, 90,  0};
    for (unsigned i = 0; i < 15; ++i)
      real(0x803635d8 + 4 * i, angles[i]);
    for (uint32_t i = 0; i < count; ++i) {
      words[W + 0x34 + 4 * i] = state;
      words[W + 0x84 + 4 * i] = i;
      words[W + 0x70 + 4 * i] = (i + 1) % count;
      words[W + 0x48 + 4 * i] = 3;
    }
  }
  bool call(std::string name, std::vector<float> operands = {}) {
    const Call c{std::move(name), std::move(operands)};
    attempted.push_back(c);
    if (int(acknowledged.size()) == hold && !released)
      return false;
    acknowledged.push_back(c);
    return true;
  }
  static std::vector<float> matrixOperands(const BattleActMatrix &a,
                                           const BattleActMatrix &b) {
    std::vector<float> r(a.begin(), a.end());
    r.insert(r.end(), b.begin(), b.end());
    return r;
  }
  BattleActDrawServices services() {
    BattleActDrawServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.reads;
      f.memoryTrace.push_back({0, a, f.words[a]});
      return f.words[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t n) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.writes;
      f.memoryTrace.push_back({1, a, n});
      f.words[a] = n;
    };
    s.memory.memory.storeByte = [](void *p, uint32_t a, uint8_t n) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.writes;
      f.memoryTrace.push_back({2, a, n});
      f.bytes[a] = n;
    };
    s.camera = [](void *p, uint32_t camera) {
      return static_cast<Fixture *>(p)->call("camera", {float(camera)});
    };
    s.center = [](void *p) -> std::optional<std::array<float, 3>> {
      if (!static_cast<Fixture *>(p)->call("center"))
        return {};
      return std::array<float, 3>{10, 20, 30};
    };
    s.offset = [](void *p, float angle) -> std::optional<std::array<float, 2>> {
      if (!static_cast<Fixture *>(p)->call("offset", {angle}))
        return {};
      return std::array<float, 2>{angle, angle * .5f};
    };
    s.translation =
        [](void *p,
           const std::array<float, 3> &v) -> std::optional<BattleActMatrix> {
      if (!static_cast<Fixture *>(p)->call("translation", {v[0], v[1], v[2]}))
        return {};
      return BattleActMatrix{1, 0, 0, v[0], 0, 1, 0, v[1], 0, 0, 1, v[2]};
    };
    s.rotation = [](void *p, float angle,
                    uint32_t axis) -> std::optional<BattleActMatrix> {
      if (!static_cast<Fixture *>(p)->call("rotation", {angle, float(axis)}))
        return {};
      return BattleActMatrix{1,
                             0,
                             0,
                             0,
                             0,
                             std::cos(angle),
                             -std::sin(angle),
                             0,
                             0,
                             std::sin(angle),
                             std::cos(angle),
                             0};
    };
    s.scale = [](void *p, float x, float y,
                 float z) -> std::optional<BattleActMatrix> {
      if (!static_cast<Fixture *>(p)->call("scale", {x, y, z}))
        return {};
      return BattleActMatrix{x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0};
    };
    s.concat = [](void *p, const BattleActMatrix &a,
                  const BattleActMatrix &b) -> std::optional<BattleActMatrix> {
      if (!static_cast<Fixture *>(p)->call("concat", matrixOperands(a, b)))
        return {};
      BattleActMatrix m{};
      for (unsigned i = 0; i < 3; ++i)
        for (unsigned j = 0; j < 4; ++j) {
          for (unsigned k = 0; k < 3; ++k)
            m[4 * i + j] += a[4 * i + k] * b[4 * k + j];
          if (j == 3)
            m[4 * i + j] += a[4 * i + 3];
        }
      return m;
    };
    s.interpolate = [](void *p, int32_t mode, int32_t step, int32_t duration,
                       float start, float end) -> std::optional<float> {
      if (!static_cast<Fixture *>(p)->call(
              "interpolate",
              {float(mode), float(step), float(duration), start, end}))
        return {};
      return duration ? start + (end - start) * float(step) / float(duration)
                      : end;
    };
    s.icons = [](void *p, uint32_t w, uint32_t rows,
                 const std::vector<BattleActDrawSlot> &slots) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.call("icons", {float(w), float(rows), float(slots.size())}))
        return false;
      f.slots = slots;
      return true;
    };
    s.partyButton = [](void *p) {
      return static_cast<Fixture *>(p)->call("party");
    };
    s.pinchMark = [](void *p) {
      return static_cast<Fixture *>(p)->call("pinch");
    };
    return s;
  }
};
bool sameSlots(const std::vector<BattleActDrawSlot> &a,
               const std::vector<BattleActDrawSlot> &b) {
  if (a.size() != b.size())
    return false;
  for (size_t i = 0; i < a.size(); ++i)
    if (a[i].position != b[i].position || a[i].matrix != b[i].matrix ||
        a[i].parameter != b[i].parameter)
      return false;
  return true;
}
void equivalent(uint32_t state, uint32_t mode, uint32_t count = 3,
                uint32_t counter = 3) {
  Fixture old(state, mode, count), fresh(state, mode, count);
  if (mode == 1) {
    old.words[Fixture::C] = fresh.words[Fixture::C] = 1;
  }
  for (uint32_t i = 0; i < count; ++i)
    old.words[Fixture::W + 0x48 + 4 * i] =
        fresh.words[Fixture::W + 0x48 + 4 * i] = counter;
  auto original = old.services();
  assert(drawPackedBattleActMenu(original));
  auto services = fresh.services();
  BattleActDrawCursor cursor;
  assert(resumePackedBattleActMenuDraw(services, cursor));
  assert(!cursor.active());
  assert(old.words == fresh.words && old.bytes == fresh.bytes &&
         sameSlots(old.slots, fresh.slots));
  assert(old.acknowledged == fresh.acknowledged);
  assert(old.reads == fresh.reads && old.writes == fresh.writes);
  assert(old.memoryTrace == fresh.memoryTrace);
  // Exercise every provider wait, including every row and all footer calls.
  for (size_t stop = 0; stop < fresh.acknowledged.size(); ++stop) {
    Fixture held(state, mode, count);
    if (mode == 1)
      held.words[Fixture::C] = 1;
    for (uint32_t i = 0; i < count; ++i)
      held.words[Fixture::W + 0x48 + 4 * i] = counter;
    held.hold = int(stop);
    auto hs = held.services();
    BattleActDrawCursor pending;
    assert(!resumePackedBattleActMenuDraw(hs, pending));
    assert(pending.active());
    const auto reads = held.reads, writes = held.writes;
    const auto attempt = held.attempted.back();
    assert(!resumePackedBattleActMenuDraw(hs, pending));
    assert(held.reads == reads && held.writes == writes &&
           held.attempted.back() == attempt);
    // A different service table or memory owner cannot consume this stack.
    auto changed = hs;
    changed.user = &fresh;
    assert(!resumePackedBattleActMenuDraw(changed, pending));
    changed = hs;
    changed.memory.memory.user = &fresh;
    assert(!resumePackedBattleActMenuDraw(changed, pending));
    assert(held.reads == reads && held.writes == writes);
    held.released = true;
    assert(resumePackedBattleActMenuDraw(hs, pending));
    assert(!pending.active());
    assert(held.words == fresh.words && held.bytes == fresh.bytes &&
           sameSlots(held.slots, fresh.slots));
    assert(held.acknowledged == fresh.acknowledged);
    assert(held.memoryTrace == fresh.memoryTrace);
  }
}
// Every unavailable callback preserves the exact synchronous prefix, including
// callbacks unused by a particular state. Bindings remain fixed during a hold.
void nullPrefixes() {
  for (uint32_t state : {0u, 1u, 3u, 4u, 5u, 10u, 11u}) {
    for (unsigned missing = 0; missing < 11; ++missing) {
      Fixture legacy(state), retained(state);
      auto a = legacy.services(), b = retained.services();
      auto remove = [&](BattleActDrawServices &s) {
        switch (missing) {
        case 0:
          s.camera = nullptr;
          break;
        case 1:
          s.center = nullptr;
          break;
        case 2:
          s.offset = nullptr;
          break;
        case 3:
          s.interpolate = nullptr;
          break;
        case 4:
          s.translation = nullptr;
          break;
        case 5:
          s.rotation = nullptr;
          break;
        case 6:
          s.scale = nullptr;
          break;
        case 7:
          s.concat = nullptr;
          break;
        case 8:
          s.icons = nullptr;
          break;
        case 9:
          s.partyButton = nullptr;
          break;
        case 10:
          s.pinchMark = nullptr;
          break;
        }
      };
      remove(a);
      remove(b);
      const bool expected = drawPackedBattleActMenu(a);
      BattleActDrawCursor cursor;
      assert(resumePackedBattleActMenuDraw(b, cursor) == expected);
      assert(cursor.active() != expected);
      assert(legacy.memoryTrace == retained.memoryTrace);
      assert(legacy.acknowledged == retained.acknowledged);
      assert(legacy.words == retained.words && legacy.bytes == retained.bytes);
      assert(sameSlots(legacy.slots, retained.slots));
      if (!expected) {
        const auto trace = retained.memoryTrace;
        const auto calls = retained.acknowledged;
        assert(!resumePackedBattleActMenuDraw(b, cursor));
        assert(trace == retained.memoryTrace && calls == retained.acknowledged);
      }
      if (missing == 8) {
        for (const auto &operation : retained.memoryTrace)
          assert(operation[0] != 0 || operation[1] != Fixture::W + 4);
      }
    }
  }
}
} // namespace
int main() {
  nullPrefixes();
  for (uint32_t state : {0u, 1u, 2u, 3u, 4u, 5u, 6u, 10u, 11u, 99u})
    equivalent(state, 0);
  for (uint32_t count : {3u, 4u, 5u})
    for (uint32_t mode : {1u, 2u, 10u})
      equivalent(3, mode, count);
  equivalent(3, 0, 0);
  for (uint32_t state : {0u, 1u, 4u, 5u, 10u, 11u}) {
    equivalent(state, 0, 3, 80);
    equivalent(state, 0, 3, uint32_t(-1));
  }
  // Changing current globals/menu owners during a camera wait does not
  // recapture B/W/C. Fresh reads after that call still observe changed fields
  // of saved W.
  Fixture retained(3);
  retained.hold = 0;
  auto s = retained.services();
  BattleActDrawCursor cursor;
  assert(!resumePackedBattleActMenuDraw(s, cursor));
  retained.words[0x8041eb90] = 0x90000;
  retained.words[Fixture::B + 0x1c78] = 0x91000;
  retained.words[Fixture::W] = 0x92000;
  retained.words[Fixture::W + 0xc] = 10;
  retained.released = true;
  assert(resumePackedBattleActMenuDraw(s, cursor));
  assert(retained.bytes[Fixture::W + 0x14] == 1 &&
         retained.words[Fixture::W + 0xc] == 1);
  assert(retained.slots.size() == 3 &&
         retained.acknowledged.back().name == "pinch");
  // Captured rotation/translation operands survive source counter and center
  // mutations across the pending call. A later source counter load stays fresh.
  Fixture mutation(1);
  auto ms = mutation.services();
  BattleActDrawCursor mc;
  mutation.hold = 3; // camera,center,offset acknowledged; rotation is pending.
  assert(!resumePackedBattleActMenuDraw(ms, mc));
  assert(mutation.attempted.back().name == "rotation");
  const auto saved = mutation.attempted.back();
  const auto reads = mutation.reads, writes = mutation.writes;
  mutation.words[Fixture::W + 0x48] = 25;
  mutation.real(0x8042242c, 123); // Original zero remains retained.
  assert(!resumePackedBattleActMenuDraw(ms, mc));
  assert(mutation.attempted.back() == saved);
  assert(mutation.reads == reads && mutation.writes == writes);
  mutation.released = true;
  assert(resumePackedBattleActMenuDraw(ms, mc));
  assert(mutation.slots[0].parameter && *mutation.slots[0].parameter == 0);
  // A delayed row retains the original unwritten stack representation.
  Fixture unwritten(0);
  auto us = unwritten.services();
  BattleActDrawCursor uc;
  assert(resumePackedBattleActMenuDraw(us, uc));
  for (const auto &slot : unwritten.slots)
    assert(!slot.position && !slot.matrix && !slot.parameter);
}

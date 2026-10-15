#include "battle_act_icon_continuation.hpp"
#include <cassert>
#include <cstring>
#include <limits>
#include <map>
using namespace ttyd;
namespace {
struct Child {
  bool bubble = false;
  uint32_t work = 0, row = 0;
  BattleActMatrix matrix{};
  std::array<float, 3> position{};
  float parameter = 0;
  bool operator==(const Child &c) const {
    return bubble == c.bubble && work == c.work && row == c.row &&
           std::memcmp(matrix.data(), c.matrix.data(), sizeof(matrix)) == 0 &&
           std::memcmp(position.data(), c.position.data(), sizeof(position)) ==
               0 &&
           std::memcmp(&parameter, &c.parameter, sizeof(parameter)) == 0;
  }
};
struct Fixture {
  static constexpr uint32_t B = 0x10000, U = 0x20000, W = 0x30000,
                            Rows = 0x40000;
  std::map<uint32_t, uint32_t> words;
  std::vector<std::array<uint32_t, 2>> reads;
  std::vector<Child> acknowledged, attempted;
  std::vector<BattleActDrawSlot> slots;
  int stop = -1;
  bool released = false;
  unsigned mutation = 0;
  std::vector<uint32_t> countReads;
  size_t countIndex = 0;
  Fixture(std::vector<float> y, uint32_t kind = 222) {
    words[0x8041eb90] = B;
    words[B + 0x420] = 1;
    words[B + 0x24] = U;
    words[U + 8] = kind;
    words[B + (kind == 222 ? 0x1bbcu : 0x1bc8u)] = uint32_t(y.size());
    slots.resize(y.size());
    for (size_t i = 0; i < y.size(); ++i) {
      slots[i].position = std::array<float, 3>{float(i), y[i], float(i + 10)};
      BattleActMatrix matrix{1, 0, 0, float(i + 1), 0, 1, 0, float(i + 2),
                             0, 0, 1, float(i + 3)};
      slots[i].matrix = matrix;
      slots[i].parameter = float(90 * i);
      words[W + 0x20 + 4 * i] = i % 2 ? 3 : 1;
    }
  }
  uint32_t cursorCount() const {
    return B + (words.at(U + 8) == 222 ? 0x1bbcu : 0x1bc8u);
  }
  bool child(Child c) {
    attempted.push_back(c);
    if (int(acknowledged.size()) == stop && !released)
      return false;
    acknowledged.push_back(c);
    if (acknowledged.size() == 1) {
      switch (mutation) {
      case 1:
        words[cursorCount()] = 1;
        break; // Fresh draw count stops remaining siblings.
      case 2:
        words[0x8041eb90] = 0x90000;
        words[B + 0x420] = 7;
        words[U + 8] = 123;
        break;
      case 3:
        words[W + 0x20] = 0;
        break;
      case 4:
        if (slots.size() > 2) {
          slots[2].matrix->at(3) = 444;
          slots[2].parameter = 555;
          slots[2].position->at(0) = 666;
        }
        break;
      case 5:
        words[cursorCount()] = uint32_t(slots.size() + 1);
        break;
      }
    }
    return true;
  }
  BattleActIconDrawServices services() {
    BattleActIconDrawServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      uint32_t value = f.words[a];
      if ((a == B + 0x1bbc || a == B + 0x1bc8) &&
          f.countIndex < f.countReads.size())
        value = f.countReads[f.countIndex++];
      f.reads.push_back({a, value});
      return value;
    };
    s.bubble = [](void *p, uint32_t w, uint32_t row, const BattleActMatrix &m,
                  float parameter) {
      return static_cast<Fixture *>(p)->child({true, w, row, m, {}, parameter});
    };
    s.circle = [](void *p, uint32_t w, uint32_t row, const BattleActMatrix &m,
                  const std::array<float, 3> &position, float parameter) {
      return static_cast<Fixture *>(p)->child(
          {false, w, row, m, position, parameter});
    };
    return s;
  }
};
bool original(Fixture &f, const BattleActIconDrawServices &s) {
  return drawPackedBattleActIcons(Fixture::W, Fixture::Rows, f.slots, s);
}
bool resume(Fixture &f, const BattleActIconDrawServices &s,
            BattleActIconDrawCursor &c) {
  return resumePackedBattleActIcons(Fixture::W, Fixture::Rows, f.slots, s, c);
}
void equivalent(const std::vector<float> &depths, uint32_t kind = 222,
                unsigned mutation = 0) {
  Fixture old(depths, kind), fresh(depths, kind);
  old.mutation = fresh.mutation = mutation;
  auto a = old.services(), b = fresh.services();
  const bool expected = original(old, a);
  BattleActIconDrawCursor c;
  assert(resume(fresh, b, c) == expected);
  assert(c.active() != expected);
  assert(old.reads == fresh.reads && old.acknowledged == fresh.acknowledged &&
         old.words == fresh.words);
  for (size_t stop = 0; stop < fresh.acknowledged.size(); ++stop) {
    Fixture held(depths, kind);
    held.mutation = mutation;
    held.stop = int(stop);
    auto s = held.services();
    BattleActIconDrawCursor cursor;
    assert(!resume(held, s, cursor));
    const auto reads = held.reads;
    const auto saved = held.attempted.back();
    const auto completed = held.acknowledged;
    assert(!resume(held, s, cursor));
    assert(held.reads == reads && held.attempted.back() == saved &&
           held.acknowledged == completed);
    auto wrong = s;
    wrong.user = &fresh;
    assert(!resume(held, wrong, cursor));
    wrong = s;
    wrong.memory.memory.user = &fresh;
    assert(!resume(held, wrong, cursor));
    assert(!resumePackedBattleActIcons(Fixture::W + 1, Fixture::Rows,
                                       held.slots, s, cursor));
    assert(!resumePackedBattleActIcons(Fixture::W, Fixture::Rows + 1,
                                       held.slots, s, cursor));
    auto other = held.slots;
    assert(!resumePackedBattleActIcons(Fixture::W, Fixture::Rows, other, s,
                                       cursor));
    assert(held.reads == reads && held.acknowledged == completed);
    held.released = true;
    assert(resume(held, s, cursor) == expected);
    assert(old.reads == held.reads && old.acknowledged == held.acknowledged &&
           old.words == held.words);
    if (!expected) {
      const auto terminalReads = held.reads;
      const auto calls = held.acknowledged;
      assert(!resume(held, s, cursor));
      assert(held.reads == terminalReads && held.acknowledged == calls);
    }
  }
}
void nullCallbacks() {
  for (bool bubble : {false, true}) {
    Fixture old({2, 9, 1}), fresh({2, 9, 1});
    auto a = old.services(), b = fresh.services();
    if (bubble)
      a.bubble = b.bubble = nullptr;
    else
      a.circle = b.circle = nullptr;
    assert(!original(old, a));
    BattleActIconDrawCursor c;
    assert(!resume(fresh, b, c));
    assert(old.reads == fresh.reads && old.acknowledged == fresh.acknowledged);
    const auto reads = fresh.reads;
    const auto acknowledged = fresh.acknowledged;
    assert(!resume(fresh, b, c));
    assert(fresh.reads == reads && fresh.acknowledged == acknowledged);
  }
}
void unwritten() {
  for (unsigned missing = 0; missing < 3; ++missing) {
    Fixture old({2, 9, 1}), fresh({2, 9, 1});
    for (auto *f : {&old, &fresh}) {
      if (missing == 0)
        f->slots[0].position.reset();
      else if (missing == 1)
        f->slots[2].matrix.reset();
      else
        f->slots[2].parameter.reset();
    }
    auto a = old.services(), b = fresh.services();
    assert(!original(old, a));
    BattleActIconDrawCursor c;
    assert(!resume(fresh, b, c));
    assert(old.reads == fresh.reads && old.acknowledged == fresh.acknowledged);
    const auto reads = fresh.reads;
    const auto calls = fresh.acknowledged;
    assert(!resume(fresh, b, c));
    assert(reads == fresh.reads && calls == fresh.acknowledged);
  }
  // Sorting itself observes changing source counts, but never fabricates stack.
  Fixture old({2, 9, 1}), fresh({2, 9, 1});
  old.countReads = fresh.countReads = {3, 4};
  auto a = old.services(), b = fresh.services();
  assert(!original(old, a));
  BattleActIconDrawCursor c;
  assert(!resume(fresh, b, c));
  assert(old.reads == fresh.reads && old.acknowledged == fresh.acknowledged);
}
void capturedChild() {
  Fixture f({2, 9, 1});
  f.stop = 0;
  auto s = f.services();
  BattleActIconDrawCursor c;
  assert(!resume(f, s, c));
  const auto pending = f.attempted.back();
  assert(pending.bubble);
  f.words[Fixture::W + 0x24] =
      1; // Selected bubble becomes circle in live flags.
  f.slots[1].matrix->at(3) = 777;
  f.slots[1].parameter = 888;
  f.slots[1].position->at(1) = -999;
  const auto reads = f.reads;
  assert(!resume(f, s, c));
  assert(f.reads == reads && f.attempted.back() == pending);
  f.released = true;
  assert(resume(f, s, c));
  assert(f.acknowledged[0] == pending);
  assert(f.acknowledged[1].row ==
         Fixture::Rows); // Retained original depth order.
  assert(f.acknowledged[2].row == Fixture::Rows + 40);
}
} // namespace
int main() {
  for (const auto &y :
       {std::vector<float>{}, std::vector<float>{4},
        std::vector<float>{2, 9, 1}, std::vector<float>{9, 2, 1},
        std::vector<float>{1, 2, 9}, std::vector<float>{2, 2, 9},
        std::vector<float>{2, 2, 2}, std::vector<float>{0, -2, 5, 5, -1}})
    equivalent(y);
  for (unsigned mutation = 1; mutation <= 5; ++mutation)
    equivalent({2, 9, 1}, 222, mutation);
  equivalent({2, 9, 1}, 225);
  equivalent({2, std::numeric_limits<float>::quiet_NaN(), 9});
  equivalent({-0.f, 0.f, std::numeric_limits<float>::infinity()});
  nullCallbacks();
  unwritten();
  capturedChild();
}

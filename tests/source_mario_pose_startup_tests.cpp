#include "source_mario_pose_startup.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Global = 0x8041e900, Table = 0x80415eb0, A = 0x10000,
                   B = 0x20000, C = 0x30000;
struct Fixture {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<uint32_t> accepted, names;
  unsigned attempts = 0, stores = 0;
  bool holdStore = true, holdSecond = true;
  SourceMarioPoseStartupCursor cursor;
  Fixture() {
    words[Global] = A;
    bytes[A + 0x3d] = 0;
    bytes[B + 0x3d] = 255;
    bytes[C + 0x3d] = 1;
    words[Table] = 0x111;
    words[Table - 12 + 4] = 0x222;
    words[Table + 12 + 8] = 0x333;
  }
  BattleSequenceEntryServices services() {
    BattleSequenceEntryServices s;
    s.user = this;
    s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      auto i = f.words.find(a);
      if (i == f.words.end())
        return {};
      return i->second;
    };
    s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto &f = *static_cast<Fixture *>(p);
      auto i = f.bytes.find(a);
      if (i == f.bytes.end())
        return {};
      return i->second;
    };
    s.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.stores;
      if (f.holdStore && a == A + 0x22c)
        return false;
      f.words[a] = v;
      return true;
    };
    s.call = [](void *p, BattleSequenceCall &q) {
      auto &f = *static_cast<Fixture *>(p);
      ++f.attempts;
      assert(q.function == 0x8004176c && q.args[1] == 2 && !q.args[2]);
      assert(
          !initializeSourceMarioPoses(f.services(), f.cursor)); // No reentry.
      f.names.push_back(q.args[0]);
      if (q.args[0] == 0x222 && f.holdSecond)
        return false;
      f.accepted.push_back(q.args[0]);
      if (q.args[0] == 0x111) {
        q.result = 0xfffffffe;
        f.words[Global] = B;
      } else if (q.args[0] == 0x222) {
        q.result = 7;
        f.words[Global] = C;
      } else {
        assert(q.args[0] == 0x333);
        q.result = 9;
      }
      return true;
    };
    return s;
  }
};
void freshSelectorsAndHeldEffects() {
  Fixture f;
  assert(!initializeSourceMarioPoses(f.services(), f.cursor));
  assert(f.cursor.pending == 0x8005ba80 &&
         f.accepted == std::vector<uint32_t>{0x111} && f.attempts == 1);
  assert(!f.words.count(A + 0x22c));
  // The first source Entry has completed; a held store must not repeat it.
  f.words[Table] = 0x999;
  f.bytes[A + 0x3d] = 3;
  f.holdStore = false;
  assert(!initializeSourceMarioPoses(f.services(), f.cursor));
  assert(f.words[A + 0x22c] == 0xfffffffe && f.cursor.pending == 0x8004176c &&
         f.attempts == 2);
  // Signed ff selected Table-12+4. Once selected, a held callee must keep that
  // request despite a later table/global mutation; third selection is fresh.
  f.words[Table - 12 + 4] = 0x999;
  f.words[Global] = A;
  f.holdSecond = false;
  assert(initializeSourceMarioPoses(f.services(), f.cursor));
  assert(f.accepted == std::vector<uint32_t>({0x111, 0x222, 0x333}));
  assert(f.names == std::vector<uint32_t>({0x111, 0x222, 0x222, 0x333}));
  assert(f.words[A + 0x230] == 7 && f.words[A + 0x234] == 9 &&
         !f.words.count(B + 0x230) && !f.words.count(C + 0x234));
  const auto attempts = f.attempts, stores = f.stores;
  assert(initializeSourceMarioPoses(f.services(), f.cursor) &&
         f.attempts == attempts && f.stores == stores && !f.cursor.active &&
         f.cursor.complete);
}
void unavailableReadsAreNotZeros() {
  Fixture f;
  f.words.erase(Global);
  assert(!initializeSourceMarioPoses(f.services(), f.cursor) &&
         f.cursor.pending == 0x8005ba68 && f.attempts == 0);
  f.words[Global] = A;
  f.bytes.erase(A + 0x3d);
  assert(!initializeSourceMarioPoses(f.services(), f.cursor) &&
         f.cursor.pending == 0x8005ba6c && f.attempts == 0);
  // Captured first owner survives a held byte read.
  f.words[Global] = B;
  f.bytes[A + 0x3d] = 0;
  f.words.erase(Table);
  assert(!initializeSourceMarioPoses(f.services(), f.cursor) &&
         f.cursor.pending == 0x8005ba78 && f.cursor.mario == A &&
         f.attempts == 0);
}
} // namespace
int main() {
  freshSelectorsAndHeldEffects();
  unavailableReadsAreNotZeros();
}

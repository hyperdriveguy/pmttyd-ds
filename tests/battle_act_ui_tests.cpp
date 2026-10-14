#include "battle_act_ui.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, C = 0x30000, W = 0x50000, G = 0x70000;
  std::map<uint32_t, uint32_t> words{
      {0x80415fd0, G}, {G + 4, 60}, {C, 1}, {C + 8, 3}};
  std::map<uint32_t, uint8_t> bytes;
  std::vector<uint32_t> released;
  bool allocationReady = true, releaseReady = true;
  BattleActUiServices services() {
    BattleActUiServices s;
    s.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->words[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<Fixture *>(p)->words[a] = v;
    };
    s.memory.memory.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(p)->bytes[a] = v;
    };
    s.allocate = [](void *p, uint32_t n) -> std::optional<uint32_t> {
      assert(n == 0x124);
      return static_cast<Fixture *>(p)->allocationReady
                 ? std::optional<uint32_t>(W)
                 : std::nullopt;
    };
    s.release = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.released.push_back(a);
      return f.releaseReady;
    };
    return s;
  }
};
int main() {
  constexpr auto B = Fixture::B, C = Fixture::C, W = Fixture::W, G = Fixture::G;
  Fixture entry;
  entry.words[W + 8] = 0xfeedface;
  entry.words[B + 0x1c78] = 0x90000;
  assert(initializePackedBattleActUi(B, C, 0x80000, 2, entry.services()));
  assert(entry.released == std::vector<uint32_t>{0x90000} &&
         entry.words[B + 0x1c78] == W && entry.words[W] == C &&
         entry.words[W + 4] == 0x80000 && entry.words[W + 8] == 0xfeedface &&
         entry.words[W + 0xc] == 0 && entry.bytes[W + 0x14] == 0 &&
         entry.words[W + 0x18] == 1);
  assert(entry.words[W + 0x84 + 4 * 2] == 0 && entry.words[W + 0x84] == 1 &&
         entry.words[W + 0x84 + 4] == 2);
  assert(entry.words[W + 0x48 + 4 * 2] == 0 && entry.words[W + 0x48] == 5 &&
         entry.words[W + 0x48 + 4] == 10);
  Fixture resume;
  assert(initializePackedBattleActUi(B, C, 0x80000, 3, resume.services()));
  assert(resume.words[W + 0xc] == 10 && resume.words[W + 0x34 + 4] == 10 &&
         resume.words[W + 0x34 + 8] == 11 && resume.words[W + 0x34] == 11);
  assert(resume.words[W + 0x84 + 4] == 2 && resume.words[W + 0x84 + 8] == 0 &&
         resume.words[W + 0x84] == 1);
  Fixture signedRate;
  signedRate.words[G + 4] = uint32_t(-60);
  assert(initializePackedBattleActUi(B, C, 0, 0, signedRate.services()) &&
         signedRate.words[W + 0x48] == uint32_t(-5) &&
         signedRate.words[W + 0x48 + 4] == uint32_t(-10));
  Fixture empty;
  empty.words[C + 8] = 0;
  empty.words[W + 0x34] = 0x123;
  assert(initializePackedBattleActUi(B, C, 0, 1, empty.services()) &&
         empty.words[W + 0x34] == 0x123);
  assert(endPackedBattleActUi(B, entry.services()) &&
         entry.words[B + 0x1c78] == 0 && entry.released.back() == W);
  const auto size = entry.released.size();
  assert(endPackedBattleActUi(B, entry.services()) &&
         entry.released.size() == size);
  Fixture failed;
  failed.words[B + 0x1c78] = 0x90000;
  failed.releaseReady = false;
  assert(!initializePackedBattleActUi(B, C, 0, 0, failed.services()) &&
         failed.words[B + 0x1c78] == 0x90000);
  failed.releaseReady = true;
  failed.allocationReady = false;
  assert(!initializePackedBattleActUi(B, C, 0, 0, failed.services()) &&
         failed.words[B + 0x1c78] == 0x90000);
}

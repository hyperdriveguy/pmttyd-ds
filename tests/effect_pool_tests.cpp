#include "effect_pool.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t MG = 0x80415ee0, GP = 0x80415fd0, M = 0x1000,
                   G = 0x2000, E = 0x3000;
struct Fixture {
  std::map<uint32_t, uint32_t> words;
  std::map<uint32_t, uint8_t> bytes;
  std::vector<uint32_t> reads, writes, updates, frees, names, clearArgs;
  EffectPoolServices services;
  bool ready = true, mutate = false;
  int async = 0;
  uint32_t allocation = 0, updateCalls = 0, asyncCalls = 0, allocCalls = 0,
           freeCalls = 0, heapCalls = 0, clearCalls = 0;
  Fixture() {
    words[MG] = M;
    words[GP] = G;
    words[M] = 3;
    words[M + 4] = E;
    words[M + 0x14] = 1;
    words[G + 0x16c] = 1;
    for (unsigned i = 0; i < 7; ++i)
      words[0x80415ee4 + 4 * i] = 0x80420180 + 4 * i;
    services.user = this;
    services.memory.user = this;
    services.memory.word = [](void *u, uint32_t a) {
      auto &f = *static_cast<Fixture *>(u);
      f.reads.push_back(a);
      return f.words[a];
    };
    services.memory.storeWord = [](void *u, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(u);
      f.writes.push_back(a);
      f.words[a] = v;
    };
    services.memory.storeByte = [](void *u, uint32_t a, uint8_t v) {
      static_cast<Fixture *>(u)->bytes[a] = v;
    };
    services.update = [](void *u, uint32_t callback, uint32_t effect) {
      auto &f = *static_cast<Fixture *>(u);
      ++f.updateCalls;
      f.updates.push_back(callback);
      f.updates.push_back(effect);
      if (!f.ready)
        return false;
      if (f.mutate) {
        // A newly allocated later slot is visited in this same captured bank.
        f.words[E + 40] = 3;
        f.words[E + 44] = 1;
        f.words[E + 56] = 0xbb;
        f.words[G + 0x14] = 0xffffffffu;
        f.words[M] = 0;
        f.words[M + 4] = 0x9999;
        f.mutate = false;
      }
      return true;
    };
    services.fileFree = [](void *u, uint32_t h) {
      auto &f = *static_cast<Fixture *>(u);
      ++f.freeCalls;
      f.frees.push_back(h);
      if (!f.ready)
        return false;
      if (f.mutate) {
        f.words[MG] = M + 0x100;
        f.words[G + 0x16c] = 2;
      }
      return true;
    };
    services.fileAsync = [](void *u, int32_t type, uint32_t zero,
                            uint32_t format, uint32_t root, uint32_t n,
                            uint32_t n2) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(u);
      ++f.asyncCalls;
      assert(type == 4 && zero == 0 && format == 0x802c1f20);
      assert(root == 0x8041f494 && n == n2);
      f.names.push_back(n);
      if (!f.ready)
        return {};
      if (f.mutate)
        f.words[G + 0x16c] = 3;
      return f.async;
    };
    services.fileAllocate = [](void *u, int32_t type, uint32_t format,
                               uint32_t root, uint32_t n,
                               uint32_t n2) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      ++f.allocCalls;
      assert(type == 4 && format == 0x802c1f20);
      assert(root == 0x8041f494 && n == n2);
      f.names.push_back(n);
      if (!f.ready)
        return {};
      if (f.mutate) {
        f.words[G + 0x16c] = 4;
        f.words[MG] = M + 0x200;
      }
      return f.allocation;
    };
    services.heapAllocate = [](void *u, int32_t heap,
                               uint32_t size) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(u);
      ++f.heapCalls;
      assert(heap == 0 && size == 512 * 40);
      if (!f.ready)
        return {};
      if (f.mutate) {
        f.words[MG] = M + 0x100;
        f.words[M + 0x100] = 2;
      }
      return f.allocation;
    };
    services.clear = [](void *u, uint32_t a, uint32_t size) {
      auto &f = *static_cast<Fixture *>(u);
      ++f.clearCalls;
      f.clearArgs.push_back(a);
      f.clearArgs.push_back(size);
      if (!f.ready)
        return false;
      if (f.mutate) {
        if (size == 8)
          f.words[0x80419a40] = 0x8888;
        else
          f.words[MG] = M + 0x200;
      }
      return true;
    };
    services.heapFree = [](void *u, int32_t heap, uint32_t payload) {
      auto &f = *static_cast<Fixture *>(u);
      ++f.freeCalls;
      assert(heap == 3);
      f.frees.push_back(payload);
      if (!f.ready)
        return false;
      if (f.mutate) {
        f.words[E + 40] = 1;
        f.words[E + 44] = 2;
        f.words[E + 52] = 0;
        f.words[M] = 0;
        f.words[M + 4] = 0x9999;
      }
      return true;
    };
  }
};
void entry() {
  for (auto count : {0u, 0xffffffffu, 3u}) {
    Fixture f;
    f.words[M] = count;
    f.words[E] = 2;
    f.words[E + 40] = 0x80000000;
    f.words[E + 80] = 0;
    f.words[G + 0x14] = 0xffffffffu;
    const uint32_t chosen = E + (count == 3 ? 80 : 0);
    for (uint32_t i = 0; i < 16; ++i)
      f.bytes[chosen + 0x18 + i] = 0xa5;
    assert(effectPoolEntry(f.services) == chosen);
    assert(f.words[chosen] == 1 && f.words[chosen + 4] == 1);
    assert((f.writes == std::vector<uint32_t>{chosen, chosen + 4,
                     chosen + 0x14, chosen + 8, chosen + 0xc, chosen + 0x10}));
    assert(f.bytes[chosen + 0x18] == 0);
    for (uint32_t i = 1; i < 16; ++i)
      assert(f.bytes[chosen + 0x18 + i] == 0xa5);
  }
  Fixture full;
  full.words[E] = full.words[E + 40] = full.words[E + 80] = 4;
  assert(effectPoolEntry(full.services) == E + 120);
}
void scanAndWait() {
  Fixture f;
  EffectMainCursor c;
  f.words[E] = 1;
  f.words[E + 0x10] = 0xaa;
  f.words[E + 80] = 1;
  f.words[E + 84] = 2; // Nonzero context other than 1 is not admitted.
  f.words[E + 96] = 0xcc;
  f.ready = false;
  assert(!resumeEffectMain(f.services, c) && c.phase == 2);
  auto reads = f.reads.size();
  f.words[E + 0x10] = 0xdd;
  assert(!resumeEffectMain(f.services, c));
  assert(f.reads.size() == reads && f.updates[2] == 0xaa);
  auto other = f.services;
  other.user = nullptr;
  assert(!resumeEffectMain(other, c));
  f.ready = true;
  f.mutate = true;
  assert(resumeEffectMain(f.services, c));
  assert(f.updateCalls == 4);
  assert(f.updates[4] == 0xaa && f.updates[6] == 0xbb);
  assert(!c.active && f.asyncCalls == 0);
}
void cacheTail() {
  Fixture f;
  f.words[M] = 0xffffffffu;
  f.words[M + 0x14] = 0;
  f.words[M + 0x10] = 0x55;
  f.mutate = true;
  f.async = -1; // Source DVD-open failure is nonzero and still allocates.
  EffectMainCursor c;
  assert(resumeEffectMain(f.services, c));
  assert(f.frees == std::vector<uint32_t>{0x55});
  assert(f.words[M + 0x110] == 0);
  assert((f.names == std::vector<uint32_t>{0x80420188, 0x8042018c}));
  assert(f.words[M + 0x210] == 0 && f.words[M + 0x214] == 4);
  // Allocation zero still publishes the current selector, suppressing retry.
  assert(resumeEffectMain(f.services, c) && f.allocCalls == 1);

  Fixture wait;
  wait.words[M] = 0;
  wait.words[M + 0x14] = 0;
  wait.ready = false;
  assert(!resumeEffectMain(wait.services, c) && c.phase == 6);
  auto reads = wait.reads.size();
  wait.words[G + 0x16c] = 6;
  assert(!resumeEffectMain(wait.services, c));
  assert(wait.reads.size() == reads);
  wait.ready = true;
  assert(resumeEffectMain(wait.services, c));
  assert(wait.names.back() == 0x80420184 && wait.words[M + 0x14] == 0);
  assert(wait.allocCalls == 0); // async zero returns normally, not a wait.
  assert(resumeEffectMain(wait.services, c));
  assert(wait.names.back() == 0x80420198);
}
void initAndRelease() {
  Fixture f;
  f.words[0x80419a40] = 0x7777;
  f.ready = false;
  EffectInitCursor init;
  assert(!resumeEffectInit(f.services, init));
  f.words[M] = 7; // Held allocation retains original 20KiB request.
  f.ready = true;
  f.mutate = true;
  assert(resumeEffectInit(f.services, init));
  assert(f.heapCalls == 2 && f.words[M + 0x104] == 0);
  assert((f.clearArgs == std::vector<uint32_t>{0, 80, 0x7777, 8}));
  assert(f.words[M + 0x214] == 0xffffffffu && f.words[0x888c] == 0);

  Fixture r;
  r.words[E] = 7;
  r.words[E + 4] = 2;
  r.words[E + 0xc] = 0x99;
  r.words[E + 80] = 2; // Occupied, but inactive.
  r.words[E + 84] = 2;
  r.ready = false;
  EffectReleaseCursor release;
  assert(!resumeEffectAutoRelease(2, r.services, release));
  assert(r.words[E] == 7);
  const auto reads = r.reads.size();
  r.words[E + 0xc] = 0xaa;
  assert(!resumeEffectAutoRelease(1, r.services, release));
  assert(!resumeEffectAutoRelease(2, r.services, release));
  assert(r.reads.size() == reads && r.frees.back() == 0x99);
  r.ready = true;
  r.mutate = true;
  assert(resumeEffectAutoRelease(2, r.services, release));
  assert(r.words[E] == 0 && r.words[E + 40] == 0 && r.words[E + 80] == 2);
  assert(r.words[E + 0xc] == 0xaa && r.frees.back() == 0);
}
void retainedServiceOperands() {
  Fixture f;
  f.words[M] = 0;
  f.words[M + 0x14] = 0;
  f.words[M + 0x10] = 0x55;
  f.ready = false;
  EffectMainCursor c;
  assert(!resumeEffectMain(f.services, c) && c.phase == 4);
  f.words[M + 0x10] = 0x66;
  auto reads = f.reads.size();
  assert(!resumeEffectMain(f.services, c));
  assert(f.reads.size() == reads && f.frees.back() == 0x55);
  // Acknowledge free and async, then hold only the synchronous allocation.
  f.ready = true;
  f.async = 1;
  auto allocate = f.services.fileAllocate;
  f.services.fileAllocate = nullptr;
  assert(!resumeEffectMain(f.services, c) && c.phase == 8);
  assert(f.freeCalls == 3 && f.asyncCalls == 1);
  f.words[G + 0x16c] = 6;
  reads = f.reads.size();
  f.services.fileAllocate = allocate;
  f.ready = false;
  assert(!resumeEffectMain(f.services, c));
  assert(f.reads.size() == reads && f.names.back() == 0x80420184);
  f.ready = true;
  assert(resumeEffectMain(f.services, c));
  assert(f.freeCalls == 3 && f.asyncCalls == 1 && f.allocCalls == 2);
  assert(f.words[M + 0x14] == 6);

  Fixture initFixture;
  EffectInitCursor init;
  initFixture.words[0x80419a40] = 0x7000;
  auto clear = initFixture.services.clear;
  initFixture.services.clear = nullptr;
  initFixture.allocation = E;
  assert(!resumeEffectInit(initFixture.services, init) && init.phase == 2);
  initFixture.words[M] = 1;
  initFixture.words[M + 4] = 0x6666;
  initFixture.services.clear = clear;
  initFixture.ready = false;
  reads = initFixture.reads.size();
  assert(!resumeEffectInit(initFixture.services, init));
  assert(initFixture.reads.size() == reads);
  assert((initFixture.clearArgs == std::vector<uint32_t>{E, 20480}));
  initFixture.ready = true;
  assert(resumeEffectInit(initFixture.services, init));
  assert(initFixture.heapCalls == 1);
  assert((initFixture.clearArgs == std::vector<uint32_t>{E, 20480, E, 20480,
                                                       0x7000, 8}));
}
} // namespace
int main() {
  entry();
  scanAndWait();
  cacheTail();
  initAndRelease();
  retainedServiceOperands();
}

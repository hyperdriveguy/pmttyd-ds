#include "event_native_runtime.hpp"
#include "source_mario_constructor.hpp"
#include <cassert>
#include <cstring>
#include <string>
using namespace ttyd;
namespace {
using namespace SourceMarioConstructor;
constexpr uint32_t M = 0x90000000, P = 0x90001000, Q = 0x90002000,
                   Stack = 0x90010000, Name = 0x90020000;
struct Fixture {
  EventNativeRuntime vm{0x81000000, {}};
  FieldMapSourceCursor cursor;
  bool holdMessage = false, holdZ = false, holdGroup = false,
       injectPosition = false;
  unsigned allocations = 0, clears = 0, groups = 0, firstXStores = 0,
           firstYStores = 0;
  uint32_t copyDestination = 0;
  Fixture() {
    EventScriptAssets image;
    assert(image.load("romfs/mario-constructor/memory.pmevt") &&
           vm.memory.importScripts(image));
    assert(vm.memory.registerRam(M, std::vector<uint8_t>(MarioBytes, 0x77)) &&
           vm.memory.registerRam(P, std::vector<uint8_t>(PouchBytes, 0x77)) &&
           vm.memory.registerRam(Q, std::vector<uint8_t>(PouchBytes, 0x88)) &&
           vm.memory.registerRam(Stack, std::vector<uint8_t>(0x8000)));
    const char text[] = "test name";
    std::vector<uint8_t> name(32);
    std::memcpy(name.data(), text, sizeof(text));
    assert(vm.memory.registerRam(Name, std::move(name)));
  }
  uint32_t word(uint32_t a) {
    uint32_t v;
    assert(vm.memory.readWord(a, v));
    return v;
  }
  uint16_t half(uint32_t a) {
    uint16_t v;
    assert(vm.memory.readHalf(a, v));
    return v;
  }
  void put(uint32_t a, uint32_t v) { assert(vm.memory.writeWord(a, v)); }
  FieldMapSourceServices services() {
    FieldMapSourceServices s;
    s.user = this;
    s.word = [](void *p, uint32_t a, uint32_t &v) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.holdZ && a == M + 0x94) {
        f.holdZ = false;
        return false;
      }
      return f.vm.memory.readWord(a, v);
    };
    s.half = [](void *p, uint32_t a, uint16_t &v) {
      return static_cast<Fixture *>(p)->vm.memory.readHalf(a, v);
    };
    s.byte = [](void *p, uint32_t a, uint8_t &v) {
      return static_cast<Fixture *>(p)->vm.memory.readByte(a, v);
    };
    s.store = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.vm.memory.writeWord(a, v))
        return false;
      if (a == BlurPositions)
        ++f.firstXStores;
      if (a == BlurPositions + 4) {
        ++f.firstYStores;
        if (f.injectPosition)
          f.holdZ = true;
      }
      return true;
    };
    s.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      return static_cast<Fixture *>(p)->vm.memory.writeHalf(a, v);
    };
    s.storeByte = [](void *p, uint32_t a, uint8_t v) {
      return static_cast<Fixture *>(p)->vm.memory.writeByte(a, v);
    };
    s.call = [](void *p, FieldMapSourceCall &q) {
      auto &f = *static_cast<Fixture *>(p);
      const auto &a = q.arguments;
      switch (q.function) {
      case 0x800300f0: // TEST-only backed addresses; actual heap is a separate
                       // native test.
        assert(!a[0]);
        ++f.allocations;
        q.result = a[1] == MarioBytes ? M : P;
        return true;
      case 0x800050b4:
        assert(!a[1]);
        ++f.clears;
        for (unsigned i = 0; i < a[2]; ++i)
          assert(f.vm.memory.writeByte(a[0] + i, 0));
        if (f.injectPosition && a[0] == 0x803dc228) {
          f.put(M + 0x8c, 10);
          f.put(M + 0x90, 20);
          f.put(M + 0x94, 30);
        }
        q.result = a[0];
        return true;
      case 0x80081b8c:
        if (f.holdMessage)
          return false;
        q.result = Name;
        return true;
      case 0x8026d5fc: {
        f.copyDestination = a[0];
        uint8_t b;
        unsigned i = 0;
        do {
          assert(f.vm.memory.readByte(a[1] + i, b) &&
                 f.vm.memory.writeByte(a[0] + i, b));
          ++i;
        } while (b);
        q.result = a[0];
        return true;
      }
      case 0x80039d0c:
        assert(a[1] == 2 && !a[2]);
        if (f.holdGroup)
          return false;
        ++f.groups;
        q.result = 0;
        return true; // Completed source polling0, not readiness.
      default:
        assert(false);
        return false;
      }
    };
    return s;
  }
  std::optional<uint32_t> run(uint32_t fn, std::array<uint32_t, 5> a = {}) {
    return resumeFieldMapSource(fn, a, Stack + 0x7fe0, services(), cursor,
                                65536);
  }
};
void constructorHeldGroupAndLateBlurZ() {
  Fixture f;
  f.holdGroup = true;
  f.injectPosition = true;
  assert(!f.run(Init) && f.cursor.pc == 0x8005b658 && f.allocations == 2 &&
         f.clears == 2);
  f.holdGroup = false;
  assert(!f.run(Init) && f.cursor.pc == 0x8005b9b0 && f.groups == 3 &&
         f.allocations == 2);
  assert(f.firstXStores == 1 && f.firstYStores == 1 &&
         f.word(BlurPositions) == 10 && f.word(BlurPositions + 4) == 20);
  f.put(M + 0x8c, 40);
  f.put(M + 0x90, 50);
  f.put(M + 0x94, 60);
  assert(f.run(Init));
  for (unsigned row = 0; row < 8; ++row)
    assert(f.word(BlurPositions + 12 * row) == 10 &&
           f.word(BlurPositions + 12 * row + 4) == 20 &&
           f.word(BlurPositions + 12 * row + 8) == 60);
  assert(f.firstXStores == 1 && f.firstYStores == 1 && f.half(P + 0xa0) == 9 &&
         f.word(M + 0xc) == 0x1001);
}
void pouchLateNameRefresh() {
  Fixture f;
  f.holdMessage = true;
  assert(!f.run(PouchInit) && f.cursor.pc == 0x800d5e8c && f.allocations == 1 &&
         f.clears == 1 && f.half(P + 0x70) == 10);
  assert(!f.run(PouchInit) && f.allocations == 1 && f.clears == 1);
  f.put(PouchGlobal, Q);
  f.holdMessage = false;
  assert(f.run(PouchInit));
  assert(f.copyDestination == Q + 0x5c0 && f.half(Q + 0x70) == 0x8888 &&
         f.half(P + 0x70) == 10);
}
void itemNineUpgradeBeforeFullFailure() {
  Fixture f;
  f.put(PouchGlobal, P);
  for (unsigned i = 0; i < 121; ++i)
    assert(f.vm.memory.writeHalf(P + 0xa0 + 2 * i, 1));
  assert(f.vm.memory.writeByte(P + 0x99, 0));
  const auto result = f.run(PouchGetItem, {9});
  assert(result && *result == 0);
  uint8_t upgrade;
  assert(f.vm.memory.readByte(P + 0x99, upgrade) && upgrade == 1);
  for (unsigned i = 0; i < 121; ++i)
    assert(f.half(P + 0xa0 + 2 * i) == 1);
}
} // namespace
int main() {
  constructorHeldGroupAndLateBlurZ();
  pouchLateNameRefresh();
  itemNineUpgradeBeforeFullFailure();
}

#include "battle_audience_actor_control.hpp"
#include "battle_audience_incarnation.hpp"
#include "battle_audience_init.hpp"
#include "battle_audience_item_control.hpp"
#include "battle_audience_item_main.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_audience_population.hpp"
#include "battle_audience_selected.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_timing_a.hpp"
#include "battle_weapon_menu_main.hpp"
#include "event_native_runtime.hpp"
#include "event_script_assets.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <map>
#include <vector>
using namespace ttyd;
struct MemoryFixture {
  static constexpr uint32_t B = 0x10000, A = B + 0x2858, P = 0x40000,
                            S = 0x50000;
  EventScriptAssets assets;
  std::map<uint32_t, uint8_t> bytes;
  BattleRandom rng{1};
  std::vector<int32_t> bounds;
  std::vector<uint32_t> writes;
  unsigned rawCalls = 0;
  std::optional<int32_t> forcedRaw;
  bool mutateRaw = false, rawAvailable = true;
  MemoryFixture() {
    assert(assets.load("romfs/trio-attack-events.pmevt"));
    word(0x8041eb90, B);
    word(0x8041eb00, P);
    word(B + 0x2738, S - 0x100);
    word(S - 0x100 + 0xc, S);
    half(P + 0x8a, 1);
    real(B + 0x163fc + 0xb34, 17);
    for (unsigned i = 0; i < 16; ++i)
      bytes[S + 0xb0 + i] = i == 10 ? 5 : i >= 13 ? 1 : 0;
  }
  std::vector<uint32_t> reads;
  uint32_t word(uint32_t a) {
    reads.push_back(a);
    if (bytes.count(a) || bytes.count(a + 1) || bytes.count(a + 2) ||
        bytes.count(a + 3)) {
      uint32_t v = 0;
      for (unsigned i = 0; i < 4; ++i)
        v = v << 8 | bytes[a + i];
      return v;
    }
    uint32_t v = 0;
    if (a >= 0x80000000) {
      if (!assets.readWord(a, v)) {
        std::fprintf(stderr, "Missing original word %08x\n", unsigned(a));
        assert(false);
      }
    }
    return v;
  }
  uint8_t byte(uint32_t a) {
    if (bytes.count(a) || a < 0x80000000)
      return bytes[a];
    return word(a & ~3u) >> (24 - 8 * (a & 3u));
  }
  void word(uint32_t a, uint32_t v) {
    writes.push_back(a);
    for (unsigned i = 0; i < 4; ++i)
      bytes[a + i] = v >> (24 - 8 * i);
  }
  uint16_t half(uint32_t a) { return uint16_t(byte(a)) << 8 | byte(a + 1); }
  void half(uint32_t a, uint16_t v) {
    bytes[a] = v >> 8;
    bytes[a + 1] = v;
  }
  void real(uint32_t a, float v) {
    uint32_t b;
    std::memcpy(&b, &v, 4);
    word(a, b);
  }
  float real(uint32_t a) {
    uint32_t b = word(a);
    float v;
    std::memcpy(&v, &b, 4);
    return v;
  }
  uint32_t member(unsigned i) { return A + 0x1bc + i * 0x134; }
  BattleAudiencePopulationServices services() {
    BattleAudiencePopulationServices s;
    s.user = this;
    s.actors.user = this;
    s.actors.memory.user = this;
    s.actors.byte = [](void *p, uint32_t a) {
      return static_cast<MemoryFixture *>(p)->byte(a);
    };
    auto &m = s.actors.memory;
    m.word = [](void *p, uint32_t a) {
      return static_cast<MemoryFixture *>(p)->word(a);
    };
    m.half = [](void *p, uint32_t a) {
      return static_cast<MemoryFixture *>(p)->half(a);
    };
    m.storeWord = [](void *p, uint32_t a, uint32_t v) {
      static_cast<MemoryFixture *>(p)->word(a, v);
    };
    m.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      static_cast<MemoryFixture *>(p)->half(a, v);
    };
    m.storeByte = [](void *p, uint32_t a, uint8_t v) {
      static_cast<MemoryFixture *>(p)->bytes[a] = v;
    };
    s.random = [](void *p, int32_t bound) {
      auto &f = *static_cast<MemoryFixture *>(p);
      f.bounds.push_back(bound);
      return f.rng.bounded(bound);
    };
    s.randomRaw = [](void *p) -> std::optional<int32_t> {
      auto &f = *static_cast<MemoryFixture *>(p);
      ++f.rawCalls;
      if (f.mutateRaw)
        f.word(0x8041eb90, 0x20000);
      if (!f.rawAvailable)
        return {};
      return f.forcedRaw ? *f.forcedRaw : f.rng.next();
    };
    s.truncate = [](void *, float v) -> std::optional<int32_t> {
      if (std::isfinite(v) && v >= -2147483648.f && v < 2147483648.f)
        return int32_t(v);
      return {};
    };
    s.fma = [](void *, float a, float b, float c) { return std::fma(a, b, c); };
    return s;
  }
  void positive(int32_t count, unsigned mode = 0) {
    half(P + 0x8c, 1);
    word(B + 0x19078, 0x10000);
    word(B + 0x19084, count);
    word(S + 0xac, mode);
  }
};

namespace {
struct Fixture : MemoryFixture {
  BattleAudienceIncarnationOwner incarnations;
  bool hooked = true;
  BattleActorServices actors() {
    auto a = services().actors;
    if (hooked)
      a.audienceLifecycle = incarnations.hook();
    return a;
  }
  void reset() {
    BattleAudienceInitServices s;
    s.actors = actors();
    s.random = [](void *, int32_t) -> int32_t { return 0; };
    s.setting = [](void *) { return false; };
    // The actual source fill occurs before the intentionally unavailable
    // setting.
    assert(!initializePackedBattleAudience(s));
  }
  bool enter(unsigned slot = 4, unsigned type = 10) {
    auto s = services();
    s.actors = actors();
    return packedBattleAudienceEntry(slot, type, 0, s) == 1;
  }
  bool create(unsigned slot = 4, int32_t kind = 236, int32_t limit = 100) {
    BattleAudienceItemEntryServices s;
    s.actors = actors();
    s.switchValue = [](void *, int32_t n) -> std::optional<int32_t> {
      assert(n == 235);
      return 1;
    };
    return createPackedBattleAudienceItem(slot, kind, limit, s) == 1;
  }
  uint32_t item(unsigned i = 0) { return A + 0xf25c + 0x48 * i; }
  BattleAudienceItemMainServices main(bool sound = true) {
    BattleAudienceItemMainServices s;
    s.actors = actors();
    s.user = this;
    s.interpolate = [](void *, int32_t, int32_t, int32_t, float,
                       float) -> std::optional<float> { return 0; };
    if (sound)
      s.stopSound = [](void *, int32_t) -> bool { return true; };
    return s;
  }
};
void actualResetEntryAndAllocation() {
  Fixture f;
  auto a = f.actors();
  assert(!f.incarnations.token());
  // Raw active-looking memory is not observed creation.
  f.word(f.member(4), 13);
  f.half(f.member(4) + 0x1c, 0);
  f.word(f.item(), 1);
  assert(!f.incarnations.actorGeneration(4) && !f.incarnations.identity(4, 0));
  f.reset();
  assert(f.incarnations.token() && f.incarnations.allocation() == Fixture::A);
  const auto token = f.incarnations.token();
  assert(f.word(f.member(4)) == 0 && f.word(f.item()) == 0);
  assert(!f.incarnations.identity(4, 0));
  assert(f.enter());
  const auto actor = f.incarnations.actorGeneration(4);
  assert(actor);
  const auto draws = f.rawCalls;
  assert(!f.enter() && f.rawCalls == draws);
  assert(f.incarnations.actorGeneration(4) == actor);
  assert(f.create());
  const auto original = f.incarnations.identity(4, 0);
  assert(original);
  assert(original->owner == token && original->audience == Fixture::A &&
         original->slot == 4 && original->item == 0);
  assert(!f.incarnations.identity(5, 0) && !f.incarnations.identity(4, 1));
  const auto reads = f.reads, writes = f.writes;
  assert(f.incarnations.identity(4, 0));
  assert(reads == f.reads && writes == f.writes);
  assert(!f.create());
  assert(f.incarnations.identity(4, 0)->itemGeneration ==
         original->itemGeneration);
  packedBattleAudienceDelete(4, a);
  assert(!f.incarnations.actorGeneration(4) && !f.incarnations.identity(4, 0));
  assert(f.enter());
  assert(f.incarnations.actorGeneration(4) != actor);
  assert(!f.incarnations.identity(
      4, 0)); // Old item's link cannot attach to a reused actor slot.
  f.reset();
  assert(f.incarnations.token() != token &&
         !f.incarnations.actorGeneration(4) &&
         !f.incarnations.itemGeneration(0));
}
void incompleteAndPaired() {
  Fixture f;
  f.reset();
  auto s = f.services();
  s.actors = f.actors();
  f.rawAvailable = false;
  assert(!packedBattleAudienceEntry(4, 10, 0, s));
  assert(f.word(f.member(4)) & 1);
  assert(!f.incarnations.actorGeneration(
      4)); // Actual birth happened, constructor did not finish.
  f.rawAvailable = true;
  assert(packedBattleAudienceEntry(4, 10, 0, s) == 0);
  assert(!f.incarnations.actorGeneration(
      4)); // No polling/source-success inference.
  packedBattleAudienceDelete(4, f.actors());
  assert(f.enter() && f.incarnations.actorGeneration(4));
  assert(f.enter(6, 9));
  assert(f.word(f.member(7)) == 0x101);
  assert(f.incarnations.actorGeneration(6) &&
         !f.incarnations.actorGeneration(7));
  packedBattleAudienceDelete(6, f.actors());
  assert(!f.incarnations.actorGeneration(6) &&
         !f.incarnations.actorGeneration(7));
}
void actualClearPathsAndPreservedFlags() {
  Fixture f;
  f.reset();
  assert(f.enter(4) && f.enter(6));
  assert(f.create(4) && f.create(6));
  const auto selected = f.incarnations.identity(4, 0),
             other = f.incarnations.identity(6, 1);
  assert(selected && other);
  packedBattleAudienceSetTarget(4, f.actors());
  assert(f.incarnations.identity(4, 0) && !f.incarnations.itemGeneration(1));
  assert(f.incarnations.actorGeneration(6) == other->actorGeneration);
  f.word(f.member(4), f.word(f.member(4)) | 0x40);
  BattleAudienceItemControlServices control;
  control.actors = f.actors();
  assert(controlPackedBattleAudienceItemState(0x80191a48, 4, 0, control));
  assert(!f.incarnations.itemGeneration(0) &&
         f.incarnations.actorGeneration(4) == selected->actorGeneration);
  packedBattleAudienceChangeStatus(4, 0, f.actors());
  assert(f.create(4));
  const auto recreated = f.incarnations.identity(4, 0);
  assert(recreated && recreated->itemGeneration != selected->itemGeneration);
  // Source telegraph's |2 and expiration blink's &~2 preserve alive bit1.
  assert(processPackedBattleAudienceItems(f.main()));
  assert(f.word(f.item()) & 2);
  assert(f.incarnations.identity(4, 0)->itemGeneration ==
         recreated->itemGeneration);
  f.word(f.item() + 4, 6);
  f.word(f.item() + 8, 27);
  assert(processPackedBattleAudienceItems(f.main()));
  assert((f.word(f.item()) & 3) == 1);
  assert(f.incarnations.identity(4, 0)->itemGeneration ==
         recreated->itemGeneration);
  f.word(f.item() + 8, 0);
  assert(!processPackedBattleAudienceItems(f.main(false)));
  assert(f.incarnations.itemGeneration(0) ==
         recreated->itemGeneration); // Stop holds before actual clear.
  assert(processPackedBattleAudienceItems(f.main()));
  assert(!f.incarnations.itemGeneration(0));
  packedBattleAudienceChangeStatus(4, 0, f.actors());
  assert(f.create(4));
  packedBattleAudienceChangeStatus(4, 13, f.actors());
  assert(processPackedBattleAudienceItems(f.main(false)));
  assert(!f.incarnations.itemGeneration(0));
}
void giftHideIsNotDestruction() {
  Fixture f;
  f.reset();
  assert(f.enter() && f.create(4, 121));
  assert(processPackedBattleAudienceItems(f.main()));
  const auto before = f.incarnations.identity(4, 0);
  assert(before);
  constexpr uint32_t U = 0x90000;
  f.word(U, 1);
  f.word(U + 8, 222);
  f.word(U + 0x104, 0);
  f.word(Fixture::B + 0xef4, 0x80);
  f.word(f.item() + 4, 3);
  f.word(f.item() + 0x40, U);
  f.word(f.item() + 0x44, 1);
  assert(processPackedBattleAudienceItems(f.main()));
  assert((f.word(f.item()) & 3) == 1 && f.word(f.item() + 4) == 7);
  assert(f.incarnations.identity(4, 0)->itemGeneration ==
         before->itemGeneration);
}
void unchangedGuestTrace() {
  Fixture observed, plain;
  plain.hooked = false;
  for (auto *f : {&observed, &plain}) {
    f->reset();
    assert(f->enter(4) && f->enter(6));
    assert(f->create(4) && f->create(6));
    packedBattleAudienceSetTarget(4, f->actors());
    packedBattleAudienceDelete(6, f->actors());
  }
  assert(observed.bytes == plain.bytes && observed.writes == plain.writes &&
         observed.reads == plain.reads);
  assert(observed.incarnations.token() && !plain.incarnations.token());
}
void nativeVmOwner() {
  auto first =
      std::make_unique<EventNativeRuntime>(0x81000000, EventNativePlatform{});
  auto second =
      std::make_unique<EventNativeRuntime>(0x82000000, EventNativePlatform{});
  assert(first->audienceIncarnations && second->audienceIncarnations &&
         first->audienceIncarnations != second->audienceIncarnations);
  assert(!first->audienceIncarnations->token());
  assert(first->memory.registerRam(0x8041eb00, std::vector<uint8_t>(0x100)));
  assert(first->memory.registerRam(Fixture::B, std::vector<uint8_t>(0x20000)));
  assert(first->memory.writeWord(0x8041eb90, Fixture::B));
  auto a = nativeBattleActorServices(*first);
  const auto again = nativeBattleActorServices(*first);
  assert(a.audienceLifecycle.user == first->audienceIncarnations.get() &&
         a.audienceLifecycle.user == again.audienceLifecycle.user &&
         a.audienceLifecycle.event == again.audienceLifecycle.event);
  BattleAudienceInitServices s;
  s.actors = a;
  s.random = [](void *, int32_t) -> int32_t { return 0; };
  s.setting = [](void *) { return false; };
  assert(!initializePackedBattleAudience(s));
  assert(first->audienceIncarnations->token());
  assert(!second->audienceIncarnations->token());
  const auto old = first->audienceIncarnations->token();
  assert(!initializePackedBattleAudience(s));
  assert(first->audienceIncarnations->token() != old);
}
void observerIdentityDuringHolds() {
  Fixture f;
  f.reset();
  assert(f.enter() && f.create());
  auto a = f.actors();
  BattleMenuAudienceServices s;
  s.memory = a;
  s.user = &f;
  s.privateItem = 0x70000;
  s.trigger = [](void *, uint32_t) -> uint32_t { return 0x400u; };
  s.mario = [](void *, uint32_t) -> uint32_t { return 0x90000u; };
  s.itemOn = [](void *p, uint32_t output, int32_t, int32_t, int32_t, int32_t) {
    auto &f = *static_cast<Fixture *>(p);
    packedBattleAudienceItemOn({output, 0, 0, 0, 0}, f.actors());
    return true;
  };
  s.setTarget = [](void *p, int32_t slot) {
    auto &f = *static_cast<Fixture *>(p);
    packedBattleAudienceSetTarget(slot, f.actors());
  };
  s.endActMenuReady = [](void *, uint32_t) { return false; };
  s.lightOff = [](void *, int32_t, int32_t) {};
  BattleMenuAudienceCursor c;
  assert(!resumePackedBattleMenuAudienceCheck(s, c));
  const auto reads = f.reads, writes = f.writes;
  auto wrong = s;
  wrong.memory.audienceLifecycle.user = nullptr;
  assert(!resumePackedBattleMenuAudienceCheck(wrong, c));
  wrong = s;
  wrong.memory.audienceLifecycle.event = nullptr;
  assert(!resumePackedBattleMenuAudienceCheck(wrong, c));
  assert(f.reads == reads && f.writes == writes);
  const auto identity = f.incarnations.identity(4, 0);
  assert(identity);
  BattleAudienceSelectedInput input{
      identity->owner,
      identity->audience,
      {4, 0, identity->actorGeneration, identity->itemGeneration}};
  BattleAudienceSelectedLive live{
      identity->owner, identity->audience, {input.target, true, true, true}};
  BattleAudienceSelectedServices selected;
  selected.memory = a;
  selected.mario = [](void *, uint32_t) -> std::optional<uint32_t> {
    return {};
  };
  BattleAudienceSelectedCursor pending;
  assert(!resumeSelectedBattleAudienceInterruption(input, live, selected,
                                                   pending));
  const auto readCount = f.reads.size();
  auto replaced = selected;
  replaced.memory.audienceLifecycle.user = nullptr;
  assert(!resumeSelectedBattleAudienceInterruption(input, live, replaced,
                                                   pending));
  replaced = selected;
  replaced.memory.audienceLifecycle.event = nullptr;
  assert(!resumeSelectedBattleAudienceInterruption(input, live, replaced,
                                                   pending));
  assert(f.reads.size() == readCount);
}
} // namespace
int main() {
  actualResetEntryAndAllocation();
  incompleteAndPaired();
  actualClearPathsAndPreservedFlags();
  giftHideIsNotDestruction();
  unchangedGuestTrace();
  nativeVmOwner();
  observerIdentityDuringHolds();
  return 0;
}

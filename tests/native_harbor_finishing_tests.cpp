#include "native_harbor_finishing.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Battle = 0x84000000, Info = 0x84500020, Setup = 0x84600000,
                   Mario = 0x84101000, OtherMario = 0x84103000,
                   Party = 0x84102000, NpcWork = 0x85000000, Npc = 0x85001000;
struct Op {
  char kind;
  uint32_t key;
  std::vector<uint32_t> values;
  bool operator==(const Op &o) const {
    return kind == o.kind && key == o.key && values == o.values;
  }
};
struct Fixture {
  NativeHarborFinishingServices services;
  std::unique_ptr<NativeHarborFinishing> owner;
  std::map<uint32_t, uint32_t> memory;
  std::vector<Op> trace;
  std::vector<NativeHarborFinishing::Row> rows;
  bool available = true, ordinary = true, reenter = false, party = false,
       partyDead = false, npc = true, replaceMario = false,
       mutateNpcHold = false;
  uint32_t marioFlags = 0, setupFlags = 0, partyKind = 224;
  uint8_t partyByte = 0;
  unsigned marioGets = 0, voices = 0, types = 0, poseNames = 0, audience = 0,
           fadeEntries = 0, musicStops = 0, acks = 0, opaqueQueries = 0,
           held = 0;
  int failAt = -1;
  int rate = 60;
  bool upperPresented = false, lowerPresented = false;
  const NativeHarborFinishingRequest *heldRequest = nullptr;
  SourceBattleVoiceCursor *voiceCell = nullptr;
  const uint32_t *colorCell = nullptr;
  Fixture() {
    memory = {{Battle + 0xef4, 0}, {Battle + 0x2738, Info}, {Info + 0xc, Setup},
              {Mario + 0x138, 0},  {Setup + 4, 0},          {NpcWork + 8, Npc},
              {Npc + 0x140, 0x40}, {0x80427144, 0xff},      {Party + 8, 224}};
    services.user = this;
    services.lifetime = std::make_shared<int>(1);
    services.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter) {
        assert(!f.owner->advance(99999));
        assert(!f.owner->ready());
      }
      return f.available;
    };
    services.ordinaryRewardDone = [](void *p) {
      return static_cast<Fixture *>(p)->ordinary;
    };
    services.word = [](void *p, uint32_t at) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold())
        return {};
      assert(f.memory.count(at));
      auto value = f.memory.at(at);
      f.trace.push_back({'R', at, {value}});
      return value;
    };
    services.byte = [](void *p, uint32_t at) -> std::optional<uint8_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold())
        return {};
      assert(at == Party + 0x12b);
      f.trace.push_back({'r', at, {f.partyByte}});
      return f.partyByte;
    };
    services.writeWord = [](void *p, uint32_t at, uint32_t value) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold()) {
        if (f.mutateNpcHold)
          f.memory[at] = 0x1000;
        return false;
      }
      assert(at == Npc + 0x140);
      f.trace.push_back({'W', at, {value}});
      f.memory[at] = value;
      return true;
    };
    services.call =
        [](void *p,
           const NativeHarborFinishingRequest &q) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold()) {
        if (f.heldRequest)
          assert(f.heldRequest == &q);
        f.heldRequest = &q;
        return {};
      }
      f.heldRequest = nullptr;
      std::vector<uint32_t> args(q.arguments.begin(),
                                 q.arguments.begin() + q.count);
      f.trace.push_back({'C', q.callsite, {q.function}});
      f.trace.back().values.insert(f.trace.back().values.end(), args.begin(),
                                   args.end());
      switch (q.function) {
      case 0x800f87a8:
        assert(args == std::vector<uint32_t>{Battle});
        ++f.marioGets;
        return f.replaceMario && f.marioGets > 1 ? OtherMario : Mario;
      case 0x800f8728:
        assert(args == std::vector<uint32_t>{Battle});
        return f.party ? Party : 0;
      case 0x80127814:
        assert((args == std::vector<uint32_t>{Party, 27}));
        return f.partyDead ? 1 : 0;
      case 0x8012619c:
        assert(args.size() == 4 && args[0] == Mario &&
               args[1] ==
                   ((f.setupFlags & 0x20000000) ? 0x802feafc : 0x802feb14) &&
               args[2] == 0xf1194d80 && args[3] == 0 && q.voice);
        if (f.voiceCell)
          assert(f.voiceCell == q.voice);
        f.voiceCell = q.voice;
        ++f.voices;
        return 123;
      case 0x801275f4:
        assert((args == std::vector<uint32_t>{
                            f.replaceMario ? OtherMario : Mario, 0x80427298}));
        ++f.poseNames;
        return 0;
      case 0x80127640:
        assert(args.size() == 2 && args[1] == 42);
        ++f.types;
        return 0;
      case 0x8019fddc:
        assert((args == std::vector<uint32_t>{0, 30}));
        assert(f.owner->sourceSequence() == 0x0700001d);
        ++f.audience;
        return 0;
      case 0x8004829c:
        assert(args.empty());
        return NpcWork;
      case 0x80013368:
        assert((args == std::vector<uint32_t>{10, 200}) && q.fadeColor &&
               *q.fadeColor == 0xff);
        f.colorCell = q.fadeColor;
        ++f.fadeEntries;
        return 0;
      case 0x800daefc:
        assert((args == std::vector<uint32_t>{0x201}));
        assert(f.owner->sourceSequence() == 0x0700001e);
        ++f.musicStops;
        return 0;
      default:
        assert(false);
        return {};
      }
    };
    services.refreshRate = [](void *p) -> std::optional<int32_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold())
        return {};
      f.trace.push_back({'H', 0, {uint32_t(f.rate)}});
      return f.rate;
    };
    services.opaquePresented = [](void *p,
                                  uint64_t visit) -> std::optional<bool> {
      auto &f = *static_cast<Fixture *>(p);
      if (f.hold())
        return {};
      assert(visit == f.owner->fadeVisit() && f.owner->fade().finished &&
             f.owner->fade().alpha() == 255);
      f.trace.push_back({'O', 0, {uint32_t(visit)}});
      ++f.opaqueQueries;
      return f.upperPresented && f.lowerPresented;
    };
  }
  bool hold() {
    if (failAt >= 0 && acks == unsigned(failAt) && held < 2) {
      ++held;
      return true;
    }
    ++acks;
    return false;
  }
  void start() {
    memory[Mario + 0x138] = marioFlags;
    memory[Setup + 4] = setupFlags;
    memory[Party + 8] = partyKind;
    memory[NpcWork + 8] = npc ? Npc : 0;
    owner = std::make_unique<NativeHarborFinishing>(Battle, services);
  }
  void run() {
    start();
    for (uint64_t visit = 1; visit < 200 && !owner->ready(); ++visit) {
      unsigned tries = 0;
      while (!owner->advance(visit)) {
        assert(++tries < 8);
        const auto count = trace.size();
        assert(!owner->advance(visit + 1) && trace.size() == count);
      }
      rows.push_back(owner->row());
      // TESTONLY explicit external frame receipts, not a query-count fallback
      // in the controller. One screen alone must never publish readiness.
      if (owner->phase() == NativeHarborFinishing::Phase::OpaqueWait) {
        if (opaqueQueries == 1) {
          upperPresented = true;
          assert(!owner->ready());
        }
        if (opaqueQueries == 2) {
          lowerPresented = true;
          assert(!owner->ready());
        }
      }
      const auto count = trace.size();
      assert(owner->advance(visit) && trace.size() == count);
    }
    assert(owner->ready() && voices <= 1 && poseNames == 1 &&
           fadeEntries == 1 && musicStops == 1 && audience == 1 &&
           opaqueQueries == 3);
  }
};
} // namespace
int main() {
  static_assert(!std::is_copy_constructible<NativeHarborFinishing>::value &&
                    !std::is_move_constructible<NativeHarborFinishing>::value,
                "stable borrowed source cells");
  Fixture base;
  base.run();
  assert(base.voices == 0 && base.types == 1 &&
         base.memory[Npc + 0x140] == 0x140);
  assert(base.rows[0].entrySequence == 0x0700000e &&
         base.rows[0].sequence == 0x07000010);
  assert(base.rows[1].sequence == 0x07000019);
  assert(base.rows[2].timer == 60 && base.rows[2].sequence == 0x0700001a);
  for (unsigned i = 0; i < 59; ++i)
    assert(base.rows[3 + i].timer == 59 - int(i) &&
           base.rows[3 + i].sequence == 0x0700001a);
  assert(base.rows[62].timer == 0 && base.rows[62].sequence == 0x0700001b);
  assert(base.rows[63].sequence == 0x0700001c &&
         base.rows[64].sequence == 0x0700001d &&
         base.rows[65].sequence == 0x0700001e);
  for (unsigned boundary = 0; boundary < base.acks; ++boundary) {
    Fixture f;
    f.failAt = int(boundary);
    f.reenter = true;
    f.run();
    assert(f.held == 2 && f.trace == base.trace &&
           f.rows.size() == base.rows.size());
  }
  {
    unsigned writeAt = 0;
    while (writeAt < base.trace.size() && base.trace[writeAt].kind != 'W')
      ++writeAt;
    assert(writeAt < base.trace.size());
    Fixture f;
    f.failAt = int(writeAt);
    f.mutateNpcHold = true;
    f.run();
    assert(f.trace == base.trace && f.memory[Npc + 0x140] == 0x140);
  }
  for(unsigned scenario=0;scenario<2;++scenario){
    Fixture reference;
    reference.party=true;reference.partyKind=225;
    reference.marioFlags=scenario?0:0x10000000;
    reference.partyDead=scenario!=0;reference.setupFlags=scenario?0:0x20000000;
    reference.partyByte=scenario?1:0;
    reference.run();assert(reference.voices==1);
    for(unsigned boundary=0;boundary<reference.acks;++boundary){
      Fixture f;f.party=true;f.partyKind=225;f.marioFlags=reference.marioFlags;
      f.partyDead=reference.partyDead;f.setupFlags=reference.setupFlags;
      f.partyByte=reference.partyByte;f.failAt=int(boundary);f.run();
      assert(f.held==2&&f.trace==reference.trace&&f.voices==1);
    }
  }
  for (uint32_t flags : {0u, 0x10000000u, 0x20000000u, 0x30000000u})
    for (bool party : {false, true})
      for (bool dead : {false, true})
        for (uint32_t setup : {0u, 0x20000000u}) {
          Fixture f;
          f.marioFlags = flags;
          f.party = party;
          f.partyDead = dead;
          f.setupFlags = setup;
          f.run();
          assert(f.voices == unsigned(flags != 0 || (party && dead)));
        }
  for (uint8_t byte :
       {uint8_t(0), uint8_t(1), uint8_t(127), uint8_t(128), uint8_t(255)}) {
    Fixture f;
    f.party = true;
    f.partyKind = 225;
    f.partyByte = byte;
    f.run();
    assert(f.types == (byte >= 1 && byte < 128 ? 1u : 2u));
  }
  {
    Fixture f;
    f.npc = false;
    f.services.writeWord = nullptr;
    f.services.byte = nullptr;
    f.run();
    assert(f.memory[Npc + 0x140] == 0x40);
  }
  {
    Fixture f;
    f.marioFlags = 0x10000000;
    f.replaceMario = true;
    f.run();
    assert(f.voices == 1 && f.types == 1);
  }
  {
    Fixture f;
    f.start();
    f.memory[Battle + 0xef4] = 0x00400000;
    assert(!f.owner->advance(1) && f.owner->pending() == 0x800374ac &&
           f.trace.size() == 1);
    f.memory[Battle + 0xef4] = 0;
    assert(!f.owner->advance(1) && !f.owner->advance(2) && f.trace.size() == 1);
  }
  {
    Fixture f;
    f.start();
    f.ordinary = false;
    assert(!f.owner->advance(1) && f.trace.empty());
    f.ordinary = true;
    assert(f.owner->advance(2));
    f.available = false;
    assert(!f.owner->advance(3));
  }
  {
    Fixture f;
    f.rate = 5;
    f.run();
    unsigned rates = 0;
    for (const auto &o : f.trace)
      rates += o.kind == 'H';
    assert(rates == 2);
  }
  {
    Fixture f;
    f.run();
    f.available = false;
    assert(!f.owner->ready() && !f.owner->advance(500));
  }
  {
    Fixture f;
    f.start();
    uint64_t visit = 1;
    while (f.owner->phase() != NativeHarborFinishing::Phase::FadeWait) {
      assert(f.owner->advance(visit++));
    }
    f.rate = 0;
    assert(!f.owner->advance(visit));
    const auto count = f.trace.size();
    f.rate = 60;
    assert(!f.owner->advance(visit) && f.trace.size() == count);
  }
  std::cout
      << "Native source ending rows/voice gates/60 ticks/type42/NPC/color; "
         "every boundary twice; real-fade plus both-screen receipt pass\n";
}

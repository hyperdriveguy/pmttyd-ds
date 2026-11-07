#include "source_status_values.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
  std::shared_ptr<const void> token = std::make_shared<const int>(1);
  std::map<uint32_t, uint8_t> memory, out;
  std::vector<std::string> trace;
  std::optional<size_t> blocked;
  int holds = 2;
  bool alive = true, party = true;
  uint32_t background = 0, kind = 1;
  unsigned pouchCalls = 0;
  SourceStatusValuesCursor cursor;
  std::function<void(const SourceStatusValueCall &)> onCall;
  std::function<void(uint32_t)> onStore;
  SourceStatusValueServices services;
  SourceStatusValueOutput output;
  static constexpr uint32_t gp = 0x81000000, battle = 0x82000000,
                            mario = 0x83000000, partner = 0x84000000,
                            pouch = 0x85000000, pouch2 = 0x86000000,
                            fieldMario = 0x87000000;
  Fixture(bool inBattle = true, bool hasParty = true) : party(hasParty) {
    word(0x80415fd0, gp);
    word(gp + 0x14, inBattle ? 2 : 0);
    word(0x8041eb90, battle);
    half(mario + 0x10c, 7);
    half(mario + 0x108, 10);
    half(mario + 0x112, 4);
    half(mario + 0x10e, 5);
    word(partner + 8, 224);
    half(partner + 0x10c, 9);
    half(partner + 0x108, 15);
    half(pouch + 14 + 6, 8);
    half(pouch2 + 14 + 2, 12);
    memory[fieldMario + 0x247] = 1;
    services.user = this;
    services.lifetime = token;
    services.current = [](void *p) { return static_cast<Fixture *>(p)->alive; };
    services.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
      return static_cast<Fixture *>(p)->read(a, 4);
    };
    services.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto v = static_cast<Fixture *>(p)->read(a, 2);
      return v ? std::optional<uint16_t>(uint16_t(*v)) : std::nullopt;
    };
    services.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto v = static_cast<Fixture *>(p)->read(a, 1);
      return v ? std::optional<uint8_t>(uint8_t(*v)) : std::nullopt;
    };
    services.call = [](void *p, const SourceStatusValueCall &q) {
      return static_cast<Fixture *>(p)->call(q);
    };
    output.user = this;
    output.lifetime = token;
    output.half = [](void *p, uint32_t a) -> std::optional<uint16_t> {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.allowed())
        return {};
      auto v = f.outputHalf(a);
      if (v)
        f.trace.push_back("O" + std::to_string(a) + ":" + std::to_string(*v));
      return v;
    };
    output.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      auto &f = *static_cast<Fixture *>(p);
      if (!f.allowed())
        return false;
      assert(a <= 0x14 && !(a & 1));
      f.out[a] = uint8_t(v >> 8);
      f.out[a + 1] = uint8_t(v);
      f.trace.push_back("S" + std::to_string(a) + ":" + std::to_string(v));
      if (f.onStore)
        f.onStore(a);
      return true;
    };
  }
  bool allowed() {
    if (blocked && trace.size() == *blocked && holds > 0) {
      --holds;
      return false;
    }
    return true;
  }
  void word(uint32_t a, uint32_t v) {
    for (unsigned i = 0; i < 4; ++i)
      memory[a + i] = uint8_t(v >> (24 - 8 * i));
  }
  void half(uint32_t a, uint16_t v) {
    memory[a] = uint8_t(v >> 8);
    memory[a + 1] = uint8_t(v);
  }
  std::optional<uint16_t> outputHalf(uint32_t a) const {
    const auto hi = out.find(a), lo = out.find(a + 1);
    if (hi == out.end() || lo == out.end())
      return {};
    return uint16_t(uint16_t(hi->second) << 8 | lo->second);
  }
  std::optional<uint32_t> read(uint32_t a, unsigned size) {
    if (!allowed())
      return {};
    uint32_t v = 0;
    for (unsigned i = 0; i < size; ++i) {
      auto it = memory.find(a + i);
      if (it == memory.end())
        return {};
      v = v << 8 | it->second;
    }
    trace.push_back("R" + std::to_string(a) + ":" + std::to_string(size) + ":" +
                    std::to_string(v));
    return v;
  }
  std::optional<uint32_t> call(const SourceStatusValueCall &q) {
    if (!allowed())
      return {};
    uint32_t v = 0;
    switch (q.function) {
    case 0x800d3d30:
      v = 7;
      break;
    case 0x800d3d24:
      v = 10;
      break;
    case 0x800d3c98:
      v = 4;
      break;
    case 0x800d3c8c:
      v = 5;
      break;
    case 0x800d3c44:
      v = 1234;
      break;
    case 0x800d3bb8:
      v = 2000;
      break;
    case 0x800d3424:
      v = 99;
      break;
    case 0x800d3e64:
      v = 120;
      break;
    case 0x800f87a8:
      assert(q.count == 1 && q.arguments[0] == battle);
      v = mario;
      break;
    case 0x800f8728:
      assert(q.count == 1 && q.arguments[0] == battle);
      v = party ? partner : 0;
      break;
    case 0x800f8444:
      assert(q.count == 1 && q.arguments[0] == 224);
      v = 1;
      break;
    case 0x8005c164:
      v = background;
      break;
    case 0x801410e8:
      v = kind;
      break;
    case 0x8005e0c0:
      v = fieldMario;
      break;
    case 0x800d5eb0:
      v = pouchCalls++ ? pouch2 : pouch;
      break;
    default:
      assert(false);
    }
    if (q.function != 0x800f87a8 && q.function != 0x800f8728 &&
        q.function != 0x800f8444)
      assert(q.count == 0);
    trace.push_back("C" + std::to_string(q.callsite) + ":" +
                    std::to_string(q.function) + ":" + std::to_string(v));
    if (onCall)
      onCall(q);
    return v;
  }
  bool resume() { return resumeSourceStatusValues(output, services, cursor); }
  void check(bool inBattle) const {
    assert(outputHalf(0) == 7 && outputHalf(2) == 10 && outputHalf(4) == 4 &&
           outputHalf(6) == 5);
    assert(outputHalf(0x12) == 1234 && outputHalf(0x14) == 2000 &&
           outputHalf(0x0e) == 99 && outputHalf(0x10) == 120);
    assert(outputHalf(8) == (party ? 1 : 0));
    assert(outputHalf(0x0a) == (party ? (inBattle ? 9 : 8) : 0));
    assert(outputHalf(0x0c) == (party ? (inBattle ? 15 : 12) : 0));
  }
};
void allHolds(bool battle, bool party, uint32_t background = 0) {
  Fixture reference(battle, party);
  reference.background = background;
  assert(reference.resume());
  reference.check(battle);
  for (size_t i = 0; i < reference.trace.size(); ++i) {
    Fixture f(battle, party);
    f.background = background;
    f.blocked = i;
    assert(!f.resume() && f.cursor.active() && f.cursor.pending());
    const auto prefix = f.trace;
    assert(!f.resume() && f.trace == prefix);
    assert(f.resume() && !f.cursor.active() && f.trace == reference.trace &&
           f.out == reference.out);
  }
}
void freshReads() {
  Fixture f(false);
  f.onCall = [&](const SourceStatusValueCall &q) {
    if (q.callsite == 0x8013ca58) {
      f.out[8] = 0;
      f.out[9] = 2;
      f.half(Fixture::pouch2 + 28 + 2, 22);
    }
    if (q.function == 0x800d3d30)
      f.word(0x80415fd0, 0x90000000);
  };
  assert(f.resume());
  assert(f.outputHalf(8) == 2 && f.outputHalf(0x0a) == 8 &&
         f.outputHalf(0x0c) == 22);
  Fixture battle;
  const auto originalBattle = Fixture::battle;
  battle.onCall = [&](const SourceStatusValueCall &q) {
    if (q.function == 0x800f87a8)
      battle.word(0x8041eb90, 0x90000000);
    if (q.function == 0x800f8728)
      battle.half(Fixture::mario + 0x10c, 0xffff);
  };
  assert(battle.resume() && battle.outputHalf(0) == 0xffff);
  assert(originalBattle == Fixture::battle);
  Fixture negative(false);
  negative.kind = 0xffff;
  negative.half(Fixture::pouch - 14 + 6, 0xfffe);
  negative.half(Fixture::pouch2 - 14 + 2, 0xfffd);
  assert(negative.resume() && negative.outputHalf(8) == 0xffff &&
         negative.outputHalf(0x0a) == 0xfffe &&
         negative.outputHalf(0x0c) == 0xfffd);
  Fixture bg(false);
  bg.background = 1;
  bg.memory[Fixture::fieldMario + 0x247] = 0xff;
  bg.half(Fixture::pouch - 14 + 6, 4);
  bg.half(Fixture::pouch2 - 14 + 2, 6);
  assert(bg.resume() && bg.outputHalf(8) == 0xffff &&
         bg.outputHalf(0x0a) == 4 && bg.outputHalf(0x0c) == 6);
  for (uint32_t id : {0u, 1u, 0x7fffu, 0x8000u, 0xffffu}) {
    Fixture indexed(false);
    indexed.kind = id;
    const int32_t signedId = id < 0x8000 ? int32_t(id) : int32_t(id) - 65536;
    const uint32_t offset = uint32_t(signedId) * 14;
    indexed.half(Fixture::pouch + offset + 6, 17);
    indexed.half(Fixture::pouch2 + offset + 2, 23);
    assert(indexed.resume() && indexed.outputHalf(8) == id &&
           indexed.outputHalf(0x0a) == 17 && indexed.outputHalf(0x0c) == 23);
  }
}
void lifecycle() {
  Fixture f;
  f.blocked = 3;
  assert(!f.resume());
  auto original = f.services;
  f.services.lifetime =
      std::shared_ptr<const void>(f.token.get(), [](const void *) {});
  const auto prefix = f.trace;
  assert(!f.resume() && f.trace == prefix);
  f.services = original;
  assert(!f.resume() && f.trace == prefix);
  assert(f.resume());
  Fixture stale;
  stale.onStore = [&](uint32_t) { stale.alive = false; };
  assert(!stale.resume() && stale.cursor.stale());
  auto saved = stale.trace;
  stale.alive = true;
  assert(!stale.resume() && stale.trace == saved);
  assert(stale.cursor.reset());
  stale.onStore = {};
  assert(stale.resume());
  Fixture reentrant;
  bool seen = false;
  reentrant.onCall = [&](const SourceStatusValueCall &) {
    if (!seen) {
      seen = true;
      assert(!reentrant.resume() && !reentrant.cursor.reset());
    }
  };
  assert(reentrant.resume() && seen);
  Fixture missing;
  missing.memory.erase(Fixture::mario + 0x10c);
  assert(!missing.resume());
  saved = missing.trace;
  assert(!missing.resume() && missing.trace == saved);
  missing.half(Fixture::mario + 0x10c, 7);
  assert(missing.resume());
  Fixture optional(true, false);
  optional.output.half = nullptr;
  optional.services.byte = nullptr;
  assert(optional.resume());
  optional.check(true);
  Fixture child;
  child.services.call = nullptr;
  assert(!child.resume() && child.cursor.pending() == 0x800f87a8);
  auto held = child.trace;
  assert(!child.resume() && child.trace == held);
}
} // namespace
int main() {
  allHolds(true, true);
  allHolds(true, false);
  allHolds(false, true);
  allHolds(false, true, 1);
  freshReads();
  lifecycle();
  std::cout
      << "whole original status values/field/battle/holds/lifetime PASS\n";
}

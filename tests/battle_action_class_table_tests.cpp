#include "battle_action_class_table.hpp"
#include "message_icons.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
  static constexpr uint32_t B = 0x10000, M = B + 0x171c, U = 0x40000,
                            P = 0x50000, S = 0x60000, D = 0x70000,
                            C = M + 0x498;
  std::map<uint32_t, uint32_t> w{
      {B + 0x420, 1}, {B + 0x2738, S}, {S + 0xc, D}, {U + 8, 222}};
  std::map<uint32_t, uint8_t> bytes{{P + 0x98, 1}, {P + 0x99, 1}};
  std::map<uint32_t, uint16_t> halves;
  std::vector<uint32_t> keys, reads;
  std::vector<int> entry;
  std::function<void(uint32_t)> messageHook;
  std::function<void(uint32_t, uint32_t)> storeHook;
  bool messageReady = true;
  BattleActionClassServices services() {
    BattleActionClassServices s;
    s.user = this;
    s.memory.user = this;
    s.memory.memory.user = this;
    s.memory.memory.word = [](void *p, uint32_t a) {
      auto &f = *static_cast<Fixture *>(p);
      f.reads.push_back(a);
      return f.w[a];
    };
    s.memory.memory.half = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->halves[a];
    };
    s.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
      auto &f = *static_cast<Fixture *>(p);
      f.w[a] = v;
      if (f.storeHook)
        f.storeHook(a, v);
    };
    s.memory.memory.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      static_cast<Fixture *>(p)->halves[a] = v;
    };
    s.memory.byte = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->bytes[a];
    };
    s.unit = [](void *p, uint32_t, int32_t) -> std::optional<uint32_t> {
      static_cast<Fixture *>(p)->entry.push_back(1);
      return U;
    };
    s.pouch = [](void *p) -> std::optional<uint32_t> {
      static_cast<Fixture *>(p)->entry.push_back(2);
      return P;
    };
    s.message = [](void *p, uint32_t key) -> std::optional<uint32_t> {
      auto &f = *static_cast<Fixture *>(p);
      f.keys.push_back(key);
      if (f.messageHook)
        f.messageHook(key);
      return f.messageReady ? std::optional<uint32_t>(key + 4) : std::nullopt;
    };
    return s;
  }
  std::optional<uint32_t> run() {
    return makePackedBattleActionClassTable(B, services());
  }
  std::vector<uint32_t> types(uint32_t n) {
    std::vector<uint32_t> out;
    for (uint32_t i = 0; i < n; ++i)
      out.push_back(w[M + 20 * i + 8]);
    return out;
  }
};
int main() {
  assert(loadMessageIcons("romfs/battle-class-icons.bin"));
  for(uint32_t id=334;id<344;++id){const auto* art=messageIcon(id);assert(art && art->loop==0 && art->frames.size()==1 && art->frames[0].width==48 && art->frames[0].height==48);}
  constexpr auto B = Fixture::B, M = Fixture::M, U = Fixture::U, P = Fixture::P,
                 C = Fixture::C, D = Fixture::D;
  Fixture f;
  f.w[M] = 99;
  f.w[M + 4] = 123;
  f.w[C + 4] = 55;
  f.halves[P + 0x8c] = 0xffff;
  f.halves[M + 0x16] = 0xabcd;
  f.w[M + 0x540] = 77;
  f.w[M + 200] = 0x9876;
  assert(f.run() == 5 && f.types(5) == std::vector<uint32_t>({0, 1, 2, 4, 5}));
  assert(f.w[M] == 99 && f.w[M + 4] == 123 && f.w[C + 4] == 55 &&
         f.halves[M + 0x16] == 0xabcd && f.w[M + 0x540] == 77 &&
         f.w[M + 200] == 0x9876);
  assert(f.keys == std::vector<uint32_t>({0x802f0018, 0x802f0044, 0x802eff0c,
                                          0x802f0058, 0x802f0074}));
  assert(f.halves[M + 0x14] == 334 && f.halves[M + 20 + 0x14] == 337);
  Fixture absent;
  absent.bytes[P + 0x98] = 0;
  absent.bytes[P + 0x99] = 255;
  assert(absent.run() == 3 &&
         absent.types(3) == std::vector<uint32_t>({0, 2, 5}) &&
         absent.w[M + 0xc] == 0 && absent.w[M + 0x18] == 0 &&
         absent.halves[M + 0x14] == 336);
  Fixture deny;
  deny.w[U + 0x104] = 0x10;
  deny.bytes[P + 0x98] = 0;
  deny.w[B + 0x19050] = 0xffffffff;
  assert(deny.run() == 4 && deny.w[M + 0x18] == 3 && deny.w[M + 0xc] == 0);
  Fixture signedCounter;
  signedCounter.w[B + 0x19050] = 0xffffffff;
  assert(signedCounter.run() == 4 && signedCounter.w[M + 0xc] == 1);
  Fixture definition;
  definition.w[D + 4] = 0x03000000;
  assert(definition.run() == 4 && definition.w[M + 0xc] == 0 &&
         definition.w[M + 20 + 0xc] == 0 && definition.w[M + 0x18] == 3);
  for (uint32_t kind : {224u, 230u, 231u, 223u}) {
    Fixture party;
    party.w[U + 8] = kind;
    const auto n = kind < 231 && kind >= 224 ? 3u : 1u;
    assert(party.run() == n && party.w[M + (n - 1) * 20 + 8] == 5 &&
           party.w[M + 0x4a4 + 8] == n && party.w[C + 8] == 0);
  }
  Fixture partyDeny;
  partyDeny.w[U + 8] = 224;
  partyDeny.bytes[U + 0x12b] = 255;
  assert(partyDeny.run() == 3 && partyDeny.w[M + 0x18] == 3 &&
         partyDeny.w[M + 20 + 0x18] == 3);
  Fixture mutation;
  mutation.messageHook = [&](uint32_t key) {
    if (key == 0x802f0018) {
      mutation.w[U + 8] = 224;
      mutation.bytes[P + 0x98] = 2;
    }
    if (key == 0x802f002c)
      mutation.w[U + 8] = 222;
    if (key == 0x802f0044)
      mutation.bytes[P + 0x99] = 3;
  };
  assert(mutation.run() == 5 &&
         mutation.types(5) == std::vector<uint32_t>({0, 6, 1, 2, 5}) &&
         mutation.w[C + 8] == 5 && mutation.w[M + 0x4a4 + 8] == 0 &&
         mutation.halves[M + 0x14] == 335 &&
         mutation.halves[M + 40 + 0x14] == 339);
  Fixture both;
  both.messageHook = [&](uint32_t key) {
    if (key == 0x802f0074)
      both.w[B + 0xef4] = 0x08000000;
    if (key == 0x802f008c)
      both.w[B + 0xef4] |= 0x10000000;
  };
  assert(both.run() == 5 &&
         both.types(5) == std::vector<uint32_t>({0, 1, 2, 5, 13}) &&
         both.w[M + 80 + 0x18] == 3);
  // Read hooks can alter flags between the independent final admissions.
  Fixture neither;
  neither.w[U + 8] = 223;
  auto ns = neither.services();
  // Use a provider with a separate counter rather than relying on other reads.
  struct Hook {
    Fixture *f;
    unsigned flags = 0;
  } hook{&neither};
  ns.memory.memory.user = &hook;
  ns.memory.memory.word = [](void *p, uint32_t a) {
    auto &h = *static_cast<Hook *>(p);
    if (a == B + 0xef4)
      return ++h.flags == 1 ? 0x08000000u : 0u;
    return h.f->w[a];
  };
  ns.memory.memory.storeWord = [](void *p, uint32_t a, uint32_t v) {
    static_cast<Hook *>(p)->f->w[a] = v;
  };
  ns.memory.memory.half = [](void *p, uint32_t a) {
    return static_cast<Hook *>(p)->f->halves[a];
  };
  ns.memory.memory.storeHalf = [](void *p, uint32_t a, uint16_t v) {
    static_cast<Hook *>(p)->f->halves[a] = v;
  };
  assert(makePackedBattleActionClassTable(B, ns) == 0 &&
         neither.w[M + 0x4a4] == 0xffffffff);
  for (auto index : {0xffffffffu, 4u, 7u}) {
    Fixture cursor;
    cursor.w[C] = index;
    assert(cursor.run() == 4 &&
           cursor.w[C] == (index == 0xffffffff ? index : 3u));
  }
  Fixture count;
  count.w[C] = 7;
  count.storeHook = [&](uint32_t a, uint32_t) {
    if (a == C + 8)
      count.w[a] = 2;
  };
  assert(count.run() == 2 && count.w[C] == 1);
  Fixture missing;
  missing.messageReady = false;
  assert(!missing.run() && missing.w[M + 8] == 0 && missing.w[M + 0xc] == 1 &&
         missing.w[C + 8] == 0);
  Fixture zero;
  const auto b = 0u - 0x171c;
  zero.w[b + 0x2738] = Fixture::S;
  assert(makePackedBattleActionClassTable(b, zero.services()) == P &&
         zero.entry == std::vector<int>({1, 2}) && zero.keys.empty());
}

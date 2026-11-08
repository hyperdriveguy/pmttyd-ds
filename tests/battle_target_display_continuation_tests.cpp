#include "battle_target_display_continuation.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Entry = 0x80120a44, Bytes = 0x5a0, Battle = 0x90000000,
                   Cursor = 0x91000000, Weapon = 0x92000000;
uint32_t bits(float x) {
  uint32_t r;
  std::memcpy(&r, &x, 4);
  return r;
}
float single(uint32_t x) {
  float r;
  std::memcpy(&r, &x, 4);
  return r;
}
uint64_t bits(double x) {
  uint64_t r;
  std::memcpy(&r, &x, 8);
  return r;
}
double wide(uint64_t x) {
  double r;
  std::memcpy(&r, &x, 8);
  return r;
}
int32_t signedWord(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
struct Original {
  struct Span {
    uint32_t address;
    std::vector<uint8_t> data;
  };
  std::vector<Span> spans;
  Original() {
    std::ifstream f("/home/carson/games/roms/G8ME01.iso", std::ios::binary);
    assert(f);
    auto get = [&](uint64_t at, unsigned n) {
      std::vector<uint8_t> b(n);
      f.seekg(at);
      f.read(reinterpret_cast<char *>(b.data()), n);
      assert(f);
      return b;
    };
    auto u = [](const std::vector<uint8_t> &b, unsigned i) {
      return uint32_t(b[i]) << 24 | uint32_t(b[i + 1]) << 16 |
             uint32_t(b[i + 2]) << 8 | b[i + 3];
    };
    const auto id = get(0, 8);
    assert((id == std::vector<uint8_t>{'G', '8', 'M', 'E', '0', '1', 0, 0}));
    const auto dol = u(get(0x420, 4), 0);
    const auto h = get(dol, 256);
    for (unsigned i = 0; i < 18; ++i) {
      const auto n = u(h, 0x90 + 4 * i);
      if (n)
        spans.push_back(
            {u(h, 0x48 + 4 * i), get(uint64_t(dol) + u(h, 4 * i), n)});
    }
  }
  uint8_t byte(uint32_t a) const {
    for (const auto &s : spans)
      if (a >= s.address && uint64_t(a) - s.address < s.data.size())
        return s.data[a - s.address];
    throw std::runtime_error("Unmapped original DOL byte");
  }
  uint32_t word(uint32_t a) const {
    return uint32_t(byte(a)) << 24 | uint32_t(byte(a + 1)) << 16 |
           uint32_t(byte(a + 2)) << 8 | byte(a + 3);
  }
};
using Event = std::vector<uint64_t>;
size_t matrixCases = 0, heldCases = 0;
struct Case {
  bool single = true, attribute = false, yoshi = false, mutate = false;
  int count = 3, selected = 1;
  uint32_t width = 0xabcd8001;
};
struct Fixture {
  const Original &original;
  Case mode;
  std::map<uint32_t, uint8_t> memory;
  std::vector<Event> events;
  BattleTargetDisplayServices services;
  BattleTargetDisplayCursor cursor;
  size_t operation = 0, holdAt = SIZE_MAX;
  unsigned attempts = 0;
  bool effectApplied = false, alive = true, reenter = false;
  const BattleTargetDisplayRequest *heldRequest = nullptr;
  Event heldMath;
  uint32_t lastUnit = 0;
  static uint32_t row(int i) { return Battle + 0x428 + 4 + uint32_t(i) * 0x24; }
  static uint32_t unit(uint32_t id) {
    return 0x93000000 + (id & 0xffff) * 0x200;
  }
  static uint32_t part(uint32_t u, uint32_t id) {
    return 0x96000000 + ((u >> 9) & 0xffff) * 0x400 + (id & 255) * 0x10;
  }
  void put(uint32_t a, uint8_t v) { memory[a] = v; }
  void putWord(uint32_t a, uint32_t v) {
    for (unsigned i = 0; i < 4; ++i)
      put(a + i, uint8_t(v >> (24 - 8 * i)));
  }
  void putHalf(uint32_t a, uint16_t v) {
    put(a, uint8_t(v >> 8));
    put(a + 1, uint8_t(v));
  }
  uint8_t raw(uint32_t a) const {
    const auto i = memory.find(a);
    return i == memory.end() ? original.byte(a) : i->second;
  }
  uint32_t read(uint32_t a, unsigned n) {
    uint32_t v = 0;
    for (unsigned i = 0; i < n; ++i)
      v = v << 8 | raw(a + i);
    events.push_back({n == 4 ? 0u : n == 2 ? 1u : 2u, a, v});
    return v;
  }
  Fixture(const Original &o, Case c) : original(o), mode(c) {
    putWord(Battle + 0x1c5c, 2);
    putWord(Cursor, 3);
    putWord(Battle + 0x171c + 0x80 + 3 * 0x1c, Weapon);
    putWord(Weapon + 0x64, c.single ? 0x01000000 : 0);
    put(Battle + 0xedf, uint8_t(c.selected));
    put(Battle + 0x428 + 0xa6c, uint8_t(c.count));
    for (int i = -3; i <= 8; ++i) {
      const auto r = row(i), u = unit(uint32_t(i + 6));
      putHalf(r, uint16_t(i + 6));
      putHalf(r + 2, uint16_t(i - 7));
      putHalf(r + 4, uint16_t(-30000 + 113 * i));
      putHalf(r + 6, uint16_t(30000 - 73 * i));
      putHalf(r + 8, uint16_t(-91 * i));
      putWord(u + 0x104, c.attribute ? 2 : 0);
      putWord(u + 8, c.yoshi ? 226 : 112);
      for (int p = -12; p < 8; ++p)
        putWord(part(u, uint32_t(p)) + 8, 0x80200000 + uint32_t(i + 3) * 32);
    }
    const int order[]{2, -1, 0, 2, 1, 0, 1};
    for (unsigned i = 0; i < 7; ++i)
      put(Battle + 0x428 + 0xa6d + i, uint8_t(order[i]));
    put(Battle + 0x428 + 0xa6d - 1, uint8_t(-2));
    services.user = this;
    services.current = [](void *p) {
      auto &f = *static_cast<Fixture *>(p);
      if (f.reenter) {
        assert(!f.run());
        assert(!f.cursor.reset());
      }
      return f.alive;
    };
    services.word = [](void *p, uint32_t a) {
      return static_cast<Fixture *>(p)->read(a, 4);
    };
    services.half = [](void *p, uint32_t a) {
      return uint16_t(static_cast<Fixture *>(p)->read(a, 2));
    };
    services.byte = [](void *p, uint32_t a) {
      return uint8_t(static_cast<Fixture *>(p)->read(a, 1));
    };
    services.request = [](void *p, const BattleTargetDisplayRequest &q) {
      return static_cast<Fixture *>(p)->request(q);
    };
    services.subtractToSingle = [](void *p, double a, double b) {
      auto &f = *static_cast<Fixture *>(p);
      return f.math(false, f.cursor.pendingCallsite(), a, b);
    };
    services.add = [](void *p, float a, float b) {
      auto &f = *static_cast<Fixture *>(p);
      return f.math(true, f.cursor.pendingCallsite(), a, b);
    };
  }
  bool run() {
    return resumePackedBattleTargetDisplay(Battle, services, cursor);
  }
  static Event encode(const BattleTargetDisplayRequest &q) {
    Event e{10, q.function, q.callsite};
    for (auto n : q.integers)
      e.push_back(n);
    e.push_back(uint32_t(q.transport));
    for (auto f : q.point)
      e.push_back(bits(f));
    for (auto f : q.scalars)
      e.push_back(bits(f));
    e.push_back(q.color);
    return e;
  }
  void effect(uint32_t function) {
    if (!mode.mutate)
      return;
    if (function == 0x800f88cc)
      for (int i = -3; i <= 8; ++i)
        putHalf(row(i) + 2, uint16_t(-2));
    if (function == 0x80128cd8)
      for (int i = -3; i <= 8; ++i) {
        putHalf(row(i) + 4, uint16_t(-1234));
        putHalf(row(i) + 6, uint16_t(2345));
        putHalf(row(i) + 8, uint16_t(-3456));
      }
    if (function == 0x801009b4) {
      putWord(lastUnit + 0x104, readUnlogged(lastUnit + 0x104) ^ 2);
      putWord(0x80427440 - 20072, bits(9.25f));
    }
    if (function == 0x8001a918)
      putWord(lastUnit + 8, 226);
    if (function == 0x80075528)
      for (int off : {-20112, -20108, -20104, -20100})
        putWord(0x80427440 + uint32_t(off), 0x11223344 + uint32_t(operation));
    if (function == 0x80077b4c) {
      putWord(0x80427440 - 20004, bits(12.75f));
      putWord(0x80427440 - 20000, bits(19.5f));
      putWord(0x80427440 - 19992, bits(37.25f));
    }
    if (function == 0x800773d0 && !mode.single && operation > 15)
      put(Battle + 0x428 + 0xa6c, 2);
  }
  uint32_t readUnlogged(uint32_t a) const {
    return uint32_t(raw(a)) << 24 | uint32_t(raw(a + 1)) << 16 |
           uint32_t(raw(a + 2)) << 8 | raw(a + 3);
  }
  bool wait() {
    if (operation == holdAt && attempts++ < 2)
      return true;
    return false;
  }
  std::optional<BattleTargetDisplayReply>
  request(const BattleTargetDisplayRequest &q) {
    if (reenter) {
      assert(!run());
      assert(!cursor.reset());
    }
    if (heldRequest)
      assert(heldRequest == &q);
    heldRequest = &q;
    if (!effectApplied) {
      effect(q.function);
      effectApplied = true;
    }
    if (wait())
      return {};
    events.push_back(encode(q));
    BattleTargetDisplayReply out;
    switch (q.function) {
    case 0x80124cb8:
      out.value = Cursor;
      break;
    case 0x800f88cc:
      out.value = unit(q.integers[1]);
      lastUnit = out.value;
      break;
    case 0x80128cd8:
      out.value = part(q.integers[0], q.integers[1]);
      break;
    case 0x801009b4:
      out.point = {q.point[0] + 3.25f, q.point[1] - 1.5f, q.point[2] * .5f};
      break;
    case 0x800d2db4:
      out.value = 0x80222222;
      break;
    case 0x80081b8c:
      out.value = q.integers[0] + 0x100;
      break;
    case 0x80075528:
      out.value = mode.width;
      break;
    default:
      break;
    }
    ++operation;
    attempts = 0;
    effectApplied = false;
    heldRequest = nullptr;
    return out;
  }
  std::optional<float> math(bool add, uint32_t pc, double a, double b) {
    if (reenter) {
      assert(!run());
      assert(!cursor.reset());
    }
    const Event key{add ? 12u : 11u, pc, bits(a), bits(b)};
    if (!heldMath.empty())
      assert(key == heldMath);
    heldMath = key;
    if (!effectApplied) {
      if (mode.mutate) {
        for (int off : {-20016, -20012, -20008, -20044, -20084, -19996})
          putWord(0x80427440 + uint32_t(off),
                  bits(float(off % 19) + float(operation) / 8));
      }
      effectApplied = true;
    }
    if (wait())
      return {};
    const auto value = float(add ? float(a) + float(b) : a - b);
    auto e = key;
    e.push_back(bits(value));
    events.push_back(e);
    ++operation;
    attempts = 0;
    effectApplied = false;
    heldMath.clear();
    return value;
  }
};
// TEST ONLY abstract instruction carrier: private addresses are tagged native
// offsets, never numeric guest stack pointers. Unknown linkage/saved values
// remain opaque. No decoder or private-address adapter ships in the product.
struct Value {
  std::optional<uint32_t> guest;
  std::optional<int32_t> privateOffset;
  static Value g(uint32_t n) { return {n, {}}; }
  static Value p(int32_t n) { return {{}, n}; }
};
struct Oracle {
  Fixture &f;
  std::array<Value, 32> r{};
  std::array<std::optional<double>, 32> fp{};
  std::array<bool, 32> cr{};
  std::map<int32_t, uint8_t> privateBytes;
  std::map<int32_t, Value> privateWords;
  std::map<int32_t, std::optional<double>> opaqueDouble;
  uint32_t pc = Entry;
  explicit Oracle(Fixture &x) : f(x) {
    r[1] = Value::p(160);
    r[2] = Value::g(0x80427440);
    r[4] = Value::g(Battle);
  }
  uint32_t g(unsigned i) {
    assert(r[i].guest);
    return *r[i].guest;
  }
  Value offset(Value a, int32_t n) {
    if (a.privateOffset)
      return Value::p(*a.privateOffset + n);
    assert(a.guest);
    return Value::g(*a.guest + uint32_t(n));
  }
  Value address(unsigned ra, int32_t d) {
    return offset(ra ? r[ra] : Value::g(0), d);
  }
  uint32_t pword(int32_t at) {
    uint32_t v = 0;
    for (unsigned i = 0; i < 4; ++i) {
      const auto it = privateBytes.find(at + int32_t(i));
      assert(it != privateBytes.end());
      v = v << 8 | it->second;
    }
    return v;
  }
  Value load(Value a, unsigned n) {
    if (a.guest)
      return Value::g(f.read(*a.guest, n));
    assert(a.privateOffset && n == 4);
    const auto i = privateWords.find(*a.privateOffset);
    if (i != privateWords.end())
      return i->second;
    return Value::g(pword(*a.privateOffset));
  }
  void store(Value a, Value v) {
    assert(a.privateOffset);
    privateWords[*a.privateOffset] = v;
    if (v.guest)
      for (unsigned i = 0; i < 4; ++i)
        privateBytes[*a.privateOffset + int32_t(i)] =
            uint8_t(*v.guest >> (24 - 8 * i));
  }
  std::optional<double> loadDouble(Value a) {
    if (a.guest)
      return wide(uint64_t(f.read(*a.guest, 4)) << 32 |
                  f.read(*a.guest + 4, 4));
    assert(a.privateOffset);
    const auto o = opaqueDouble.find(*a.privateOffset);
    if (o != opaqueDouble.end())
      return o->second;
    return wide(uint64_t(pword(*a.privateOffset)) << 32 |
                pword(*a.privateOffset + 4));
  }
  void storeDouble(Value a, std::optional<double> v) {
    assert(a.privateOffset);
    opaqueDouble[*a.privateOffset] = v;
    if (v) {
      const auto b = bits(*v);
      store(a, Value::g(uint32_t(b >> 32)));
      store(offset(a, 4), Value::g(uint32_t(b)));
    }
  }
  float pfloat(Value p) {
    const auto v = load(p, 4);
    assert(v.guest);
    return single(*v.guest);
  }
  std::array<float, 3> point(Value p) {
    return {pfloat(p), pfloat(offset(p, 4)), pfloat(offset(p, 8))};
  }
  void condition(int32_t a, int32_t b, unsigned field = 0) {
    cr[field * 4] = a < b;
    cr[field * 4 + 1] = a > b;
    cr[field * 4 + 2] = a == b;
  }
  void child(uint32_t target) {
    BattleTargetDisplayRequest q;
    q.function = target;
    q.callsite = pc;
    switch (target) {
    case 0x80124cb8:
      q.integers = {g(3), g(4), 0};
      q.transport = BattleTargetPrivate::CursorOutput;
      break;
    case 0x800f88cc:
    case 0x80128cd8:
      q.integers = {g(3), g(4), 0};
      break;
    case 0x801009b4:
      q.transport = BattleTargetPrivate::ProjectPoint;
      q.point = point(r[3]);
      break;
    case 0x8001a918:
      q.transport = BattleTargetPrivate::IconPoint;
      q.integers = {g(4), g(5), 0};
      q.point = point(r[3]);
      assert(fp[1]);
      q.scalars[0] = float(*fp[1]);
      break;
    case 0x80081b8c:
    case 0x80075528:
      q.integers[0] = g(3);
      break;
    case 0x8007b44c:
      q.transport = BattleTargetPrivate::WindowColor;
      assert(g(3) == 0);
      assert(load(r[4], 4).guest);
      q.color = *load(r[4], 4).guest;
      for (unsigned i = 0; i < 5; ++i) {
        assert(fp[i + 1]);
        q.scalars[i] = float(*fp[i + 1]);
      }
      break;
    case 0x800773d0:
      q.integers[0] = g(3);
      assert(fp[1] && fp[2]);
      q.scalars[0] = float(*fp[1]);
      q.scalars[1] = float(*fp[2]);
      break;
    case 0x8012c028:
      assert(g(3) == 0 && g(4) == 0);
      break;
    case 0x800d2db4:
    case 0x80077b4c:
      break;
    default:
      throw std::runtime_error("Unexpected original child");
    }
    const auto out = f.request(q);
    assert(out);
    if (target == 0x80124cb8) {
      store(r[5], Value::g(out->value));
      r[3] = Value::g(0xdeadbeef);
    } else if (target == 0x801009b4) {
      for (unsigned i = 0; i < 3; ++i)
        store(offset(r[4], 4 * int32_t(i)), Value::g(bits(out->point[i])));
    } else
      r[3] = Value::g(out->value);
  }
  void run() {
    for (unsigned steps = 0; steps < 100000; ++steps) {
      assert(pc >= Entry && pc < Entry + Bytes);
      const auto w = f.original.word(pc);
      const unsigned op = w >> 26, rt = w >> 21 & 31, ra = w >> 16 & 31,
                     rb = w >> 11 & 31, xo = w >> 1 & 1023;
      const int32_t d = int16_t(w);
      uint32_t next = pc + 4;
      switch (op) {
      case 7:
        r[rt] = Value::g(uint32_t(int64_t(signedWord(g(ra))) * d));
        break;
      case 14:
        r[rt] = address(ra, d);
        break;
      case 15:
        r[rt] = address(ra, int32_t(uint32_t(d) << 16));
        break;
      case 11:
        condition(signedWord(g(ra)), d, w >> 23 & 7);
        break;
      case 16: {
        const unsigned bo = w >> 21 & 31, bi = w >> 16 & 31;
        assert(bo == 12 || bo == 4);
        if (cr[bi] == (bo == 12))
          next = pc + uint32_t(int32_t(int16_t(w & 0xfffc)));
        break;
      }
      case 18: {
        uint32_t raw = w & 0x03fffffc;
        if (raw & 0x02000000)
          raw |= 0xfc000000;
        const auto target = pc + raw;
        if (w & 1)
          child(target);
        else
          next = target;
        break;
      }
      case 19:
        assert(xo == 16);
        return;
      case 21: {
        const unsigned sh = w >> 11 & 31, mb = w >> 6 & 31, me = w >> 1 & 31;
        const auto v = g(rt);
        const auto rotated = sh ? (v << sh | v >> (32 - sh)) : v;
        uint32_t mask = 0;
        for (unsigned i = 0; i < 32; ++i)
          if (mb <= me ? (i >= mb && i <= me) : (i >= mb || i <= me))
            mask |= 1u << (31 - i);
        r[ra] = Value::g(rotated & mask);
        if (w & 1)
          condition(signedWord(g(ra)), 0);
        break;
      }
      case 27:
        r[ra] = Value::g(g(rt) ^ (uint32_t(uint16_t(w)) << 16));
        break;
      case 31:
        switch (xo) {
        case 339:
          r[rt] = Value{};
          break;
        case 467:
          break;
        case 444:
          r[ra] = r[rt];
          assert(rt == rb);
          break;
        case 266:
          r[rt] = Value::g(g(ra) + g(rb));
          break;
        case 87:
          r[rt] = load(offset(ra ? r[ra] : Value::g(0), signedWord(g(rb))), 1);
          break;
        case 954:
          r[ra] = Value::g(uint32_t(int32_t(int8_t(g(rt) & 255))));
          break;
        case 0:
          condition(signedWord(g(ra)), signedWord(g(rb)), w >> 23 & 7);
          break;
        default:
          throw std::runtime_error("Unexpected original X opcode");
        }
        break;
      case 32:
        r[rt] = load(address(ra, d), 4);
        break;
      case 34:
        r[rt] = load(address(ra, d), 1);
        break;
      case 42: {
        const auto n = load(address(ra, d), 2);
        assert(n.guest);
        r[rt] = Value::g(uint32_t(int32_t(int16_t(*n.guest))));
        break;
      }
      case 36:
        store(address(ra, d), r[rt]);
        break;
      case 37: {
        const auto a = address(ra, d);
        store(a, r[rt]);
        r[ra] = a;
        break;
      }
      case 46:
        for (unsigned i = rt; i < 32; ++i)
          r[i] = load(address(ra, d + 4 * int32_t(i - rt)), 4);
        break;
      case 47:
        for (unsigned i = rt; i < 32; ++i)
          store(address(ra, d + 4 * int32_t(i - rt)), r[i]);
        break;
      case 48: {
        const auto n = load(address(ra, d), 4);
        assert(n.guest);
        fp[rt] = double(single(*n.guest));
        break;
      }
      case 50:
        fp[rt] = loadDouble(address(ra, d));
        break;
      case 52:
        assert(fp[rt]);
        store(address(ra, d), Value::g(bits(float(*fp[rt]))));
        break;
      case 54:
        storeDouble(address(ra, d), fp[rt]);
        break;
      case 56:
      case 60:
        assert(pc == 0x80120a54 || pc == 0x80120fc8);
        break; // Opaque f31 linkage carrier, restored before return.
      case 59: {
        const unsigned fn = w >> 1 & 31;
        assert(fn == 20 || fn == 21);
        assert(fp[ra] && fp[rb]);
        const auto v = f.math(fn == 21, pc, *fp[ra], *fp[rb]);
        assert(v);
        fp[rt] = double(*v);
        break;
      }
      default:
        throw std::runtime_error("Unexpected original opcode");
      }
      pc = next;
    }
    throw std::runtime_error("Original oracle instruction limit");
  }
};
void compare(const Fixture &a, const Fixture &b) {
  if (a.events != b.events) {
    size_t i = 0;
    while (i < std::min(a.events.size(), b.events.size()) &&
           a.events[i] == b.events[i])
      ++i;
    std::cerr << "Trace mismatch event=" << i << " portable=" << a.events.size()
              << " oracle=" << b.events.size() << '\n';
    if (i < a.events.size()) {
      for (auto n : a.events[i])
        std::cerr << std::hex << n << ' ';
      std::cerr << '\n';
    }
    if (i < b.events.size()) {
      for (auto n : b.events[i])
        std::cerr << std::hex << n << ' ';
      std::cerr << '\n';
    }
    std::abort();
  }
}
void matrix(const Original &o) {
  for (bool singleMode : {false, true})
    for (bool attribute : {false, true})
      for (bool yoshi : {false, true})
        for (int count : {-128, -1, 0, 1, 4})
          for (int selected : {-1, 1})
            for (uint32_t width : {0u, 0x1234ffffu}) {
              Case c;
              c.single = singleMode;
              c.attribute = attribute;
              c.yoshi = yoshi;
              c.count = count;
              c.selected = selected;
              c.width = width;
              Fixture actual(o, c), expected(o, c);
              assert(actual.run());
              Oracle(expected).run();
              compare(actual, expected);
              assert(!actual.cursor.active());
              ++matrixCases;
            }
}
void waits(const Original &o) {
  for (bool singleMode : {false, true})
    for (bool attribute : {false, true})
      for (bool mutate : {false, true}) {
        Case c;
        c.single = singleMode;
        c.attribute = attribute;
        c.mutate = mutate;
        c.count = 4;
        Fixture expected(o, c);
        Oracle(expected).run();
        for (size_t held = 0; held < expected.operation; ++held) {
          Fixture f(o, c);
          f.holdAt = held;
          f.reenter = true;
          unsigned visits = 0;
          while (!f.run())
            assert(++visits < 4);
          assert(visits == 2);
          compare(f, expected);
          ++heldCases;
        }
      }
}
void lifetime(const Original &o) {
  Case c;
  Fixture f(o, c);
  f.holdAt = 0;
  assert(!f.run());
  auto old = f.services;
  f.services.add = nullptr;
  assert(!f.run());
  f.services = old;
  assert(!f.run());
  assert(f.run());
  Fixture stale(o, c);
  stale.holdAt = 0;
  assert(!stale.run());
  stale.alive = false;
  assert(!stale.run() && stale.cursor.stale());
  stale.alive = true;
  assert(!stale.run());
}
} // namespace
int main() {
  Original original;
  matrix(original);
  waits(original);
  lifetime(original);
  std::cout
      << matrixCases << " static cases, " << heldCases
      << " held cases; TESTONLY ISO instruction oracle: target display ordered "
         "reads/children/math, signed paths, mutations, all waits and "
         "ownership PASS\n";
}

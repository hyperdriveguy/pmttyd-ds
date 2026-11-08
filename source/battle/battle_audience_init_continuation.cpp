#include "battle_audience_init_continuation.hpp"
#include "event_memory_opcodes.hpp"
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, PouchGlobal = 0x8041eb00;
constexpr uint32_t AudienceOffset = 0x2858, ActorOffset = 0x1bc,
                   ActorStride = 0x134, WindowOffset = 0x1375c;
constexpr uint32_t InitEntry = 0x801a83f8, SettingEntry = 0x801a60c4;
// Fixed recovered source registers and source-private frame. This is a finite
// translation of the pinned two bodies, not a bytecode/PPC execution service.
// Private offsets have no guest numeric address and cannot escape as one.
struct Value {
  enum class Kind { Unknown, Word, Private } kind = Kind::Unknown;
  uint32_t bits = 0;
  static Value word(uint32_t v) { return {Kind::Word, v}; }
  static Value frame(uint32_t v) { return {Kind::Private, v}; }
};
struct Floating {
  std::optional<uint32_t> high, low, single;
};
float fromBits(uint32_t b) {
  float v;
  std::memcpy(&v, &b, 4);
  return v;
}
int32_t signedWord(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
Floating single(uint32_t raw) {
  const double d = fromBits(raw);
  uint64_t b;
  std::memcpy(&b, &d, 8);
  return {uint32_t(b >> 32), uint32_t(b), raw};
}
bool same(const BattleAudienceSetupServices &a,
          const BattleAudienceSetupServices &b) {
  const auto &x = a.actors.memory;
  const auto &y = b.actors.memory;
  return a.user == b.user && a.current == b.current && a.call == b.call &&
         a.number == b.number && a.lifetime == b.lifetime &&
         !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) &&
         a.actors.user == b.actors.user && a.actors.byte == b.actors.byte &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall &&
         a.actors.audienceLifecycle.user == b.actors.audienceLifecycle.user &&
         a.actors.audienceLifecycle.event == b.actors.audienceLifecycle.event;
}
bool admitted(const BattleAudienceSetupServices &s) {
  const auto &m = s.actors.memory;
  return s.lifetime && s.current && m.word && m.half && m.storeWord &&
         m.storeHalf && m.storeByte && s.actors.byte;
}
struct Program {
  BattleAudienceSetupServices services;
  uint32_t pc, pending = 0, ctr = 0;
  bool stale = false, blocked = false, carry = false;
  std::array<bool, 4> condition{};
  std::array<Value, 32> r{};
  std::array<Floating, 32> f{};
  // Includes the caller linkage word reached by original LR spills. All
  // incoming saved operands are unknown, not assumed zero or guest addresses.
  std::array<Value, 0x560 / 4> frame{};
  std::optional<BattleAudienceSetupRequest> held;
  std::optional<BattleAudienceSetupNumberRequest> heldNumber;
  std::unique_ptr<Program> setting;
  BattleAudienceSetupPrivateCopy copy;
  Program(const BattleAudienceSetupServices &s, uint32_t entry)
      : services(s), pc(entry) {
    r[1] = Value::frame(entry == InitEntry ? 0x30 : 0x550);
    r[2] = Value::word(0x80427440);
    r[13] = Value::word(0x8041cf20);
  }
  bool current() {
    if (stale || !services.current(services.user)) {
      stale = true;
      return false;
    }
    return true;
  }
  uint32_t word(uint32_t a) const {
    return services.actors.memory.word(services.actors.memory.user, a);
  }
  uint8_t byte(uint32_t a) const {
    return services.actors.byte(services.actors.user, a);
  }
  uint32_t scalar(Value v) {
    if (v.kind != Value::Kind::Word) {
      blocked = true;
      return 0;
    }
    return v.bits;
  }
  Value add(Value a, Value b) {
    if (a.kind == Value::Kind::Unknown || b.kind == Value::Kind::Unknown)
      return {};
    if (a.kind == Value::Kind::Private && b.kind == Value::Kind::Private) {
      blocked = true;
      return {};
    }
    return {a.kind == Value::Kind::Private || b.kind == Value::Kind::Private
                ? Value::Kind::Private
                : Value::Kind::Word,
            a.bits + b.bits};
  }
  Value offset(Value a, uint32_t d) { return add(a, Value::word(d)); }
  Value load(Value a) {
    if (a.kind == Value::Kind::Private) {
      if ((a.bits & 3) || a.bits / 4 >= frame.size()) {
        blocked = true;
        return {};
      }
      return frame[a.bits / 4];
    }
    const auto address = scalar(a);
    return blocked ? Value{} : Value::word(word(address));
  }
  void store(Value a, Value v) {
    if (a.kind == Value::Kind::Private) {
      if ((a.bits & 3) || a.bits / 4 >= frame.size()) {
        blocked = true;
        return;
      }
      frame[a.bits / 4] = v;
      return;
    }
    const auto address = scalar(a), value = scalar(v);
    if (blocked)
      return;
    services.actors.memory.storeWord(services.actors.memory.user, address,
                                     value);
  }
  void loadSingle(unsigned d, Value a) {
    auto v = load(a);
    const auto b = scalar(v);
    if (!blocked)
      f[d] = single(b);
  }
  void loadDouble(unsigned d, Value a) {
    auto high = load(a), low = load(offset(a, 4));
    // Incoming saved FPRs may be opaque. Restoring them is native linkage,
    // while any subsequent arithmetic still requires both real operand words.
    f[d] = {high.kind == Value::Kind::Word ? std::optional<uint32_t>(high.bits)
                                           : std::nullopt,
            low.kind == Value::Kind::Word ? std::optional<uint32_t>(low.bits)
                                          : std::nullopt,
            {}};
  }
  void storeDouble(unsigned d, Value a) {
    store(a, f[d].high ? Value::word(*f[d].high) : Value{});
    store(offset(a, 4), f[d].low ? Value::word(*f[d].low) : Value{});
  }
  void storeSingle(unsigned d, Value a) {
    store(a, f[d].single ? Value::word(*f[d].single) : Value{});
  }
  uint64_t doubleRaw(unsigned d) {
    if (!f[d].high || !f[d].low) {
      blocked = true;
      return 0;
    }
    return (uint64_t(*f[d].high) << 32) | *f[d].low;
  }
  uint32_t singleRaw(unsigned d) {
    if (!f[d].single) {
      blocked = true;
      return 0;
    }
    return *f[d].single;
  }
  void compare(Value a, Value b, bool sign) {
    auto x = scalar(a), y = scalar(b);
    if (blocked)
      return;
    condition = {sign ? signedWord(x) < signedWord(y) : x < y,
                 sign ? signedWord(x) > signedWord(y) : x > y, x == y, false};
  }
  void compareFloat(unsigned a, unsigned b) {
    uint64_t x = doubleRaw(a), y = doubleRaw(b);
    if (blocked)
      return;
    double u, v;
    std::memcpy(&u, &x, 8);
    std::memcpy(&v, &y, 8);
    condition = {u<v, u> v, u == v, !(u < v || u > v || u == v)};
  }
  void record(Value v) { compare(v, Value::word(0), true); }
  uint32_t base() const { return word(BattleGlobal) + AudienceOffset; }
  uint32_t actor(uint32_t index) const {
    return base() + ActorOffset + index * ActorStride;
  }
  bool link(uint32_t function);
  bool numeric(BattleAudienceSetupNumeric operation, unsigned dest,
               std::array<uint64_t, 3> operands, bool integral = false) {
    if (blocked)
      return false;
    pending = pc;
    if (!current())
      return false;
    if (!heldNumber)
      heldNumber = BattleAudienceSetupNumberRequest{operation, pc, operands};
    if (!services.number)
      return false;
    const auto value = services.number(services.user, *heldNumber);
    if (!current() || !value)
      return false;
    heldNumber.reset();
    pending = 0;
    if (integral)
      r[dest] = Value::word(*value);
    else
      f[dest] = single(*value);
    return true;
  }
  bool truncate(unsigned d, unsigned a) {
    const auto input = singleRaw(a);
    if (blocked)
      return false;
    pending = pc;
    if (!current())
      return false;
    if (!heldNumber)
      heldNumber = BattleAudienceSetupNumberRequest{
          BattleAudienceSetupNumeric::TruncateSingle, pc, {input, 0, 0}};
    if (!services.number)
      return false;
    const auto value = services.number(services.user, *heldNumber);
    if (!current() || !value)
      return false;
    heldNumber.reset();
    pending = 0;
    f[d] = {{}, *value, {}};
    return true;
  }
  bool run();
};
bool Program::link(uint32_t function) {
  pending = function;
  if (!current())
    return false;
  // Whole pure source helpers, including discarded BaseGetPtr accesses.
  if (function == 0x801a86a0) {
    r[3] = Value::word(base());
    pending = 0;
    return true;
  }
  if (function == 0x801a85fc) {
    r[3] = Value::word(base() + WindowOffset);
    pending = 0;
    return true;
  }
  if (function == 0x800d5eb0) {
    r[3] = Value::word(word(PouchGlobal));
    pending = 0;
    return true;
  }
  if (function == 0x801a8664) {
    const auto i = scalar(r[3]);
    if (blocked)
      return false;
    r[3] = Value::word(actor(i));
    pending = 0;
    return true;
  }
  if (function == 0x801a29c0) {
    const auto i = scalar(r[3]);
    if (blocked)
      return false;
    (void)base();
    const auto flags = word(actor(i));
    r[3] = Value::word((flags & 1) && !(flags & 0x102) ? 1 : 0);
    pending = 0;
    return true;
  }
  if (function == SettingEntry) {
    if (!setting)
      setting = std::make_unique<Program>(services, SettingEntry);
    if (!setting->run()) {
      pending = setting->pending;
      return false;
    }
    setting.reset();
    pending = 0;
    return true;
  }
  if (function == 0x800050b4) {
    const auto count = scalar(r[5]), fill = scalar(r[4]);
    if (blocked)
      return false;
    if (r[3].kind == Value::Kind::Private) {
      // This parent's sole private memset is the original thirteen-word
      // allocation clear. No source SP value is exported to a guest service.
      if (count != 52 || fill != 0 || r[3].bits != 0x34) {
        blocked = true;
        return false;
      }
      for (uint32_t i = 0; i < count; i += 4)
        store(offset(r[3], i), Value::word(0));
    } else {
      const auto address = scalar(r[3]);
      if (blocked)
        return false;
      eventFillMemory(address, int32_t(fill), count, services.actors.memory);
      // Metadata observes the actual Reset strictly after its source memset.
      if (pc == 0x801a842c)
        observeBattleAudienceLifecycle(services.actors.audienceLifecycle,
                                       BattleAudienceLifecycleEvent::Reset,
                                       address);
    }
    pending = 0;
    return !blocked;
  }
  if (!held) {
    BattleAudienceSetupRequest q;
    q.function = function;
    q.callsite = pc;
    if (function == 0x8000519c) {
      copy.destination = scalar(r[3]);
      const auto length = scalar(r[5]);
      if (blocked || length != copy.bytes.size() ||
          r[4].kind != Value::Kind::Private) {
        blocked = true;
        return false;
      }
      for (unsigned i = 0; i < copy.bytes.size(); ++i) {
        const auto v = load(offset(r[4], i & ~3u));
        copy.bytes[i] =
            v.kind == Value::Kind::Word
                ? std::optional<uint8_t>(uint8_t(v.bits >> (24 - 8 * (i & 3))))
                : std::nullopt;
      }
      q.arguments = {copy.destination, 0, length};
      q.privateCopy = &copy;
    } else {
      // Values in unused ABI registers are not source arguments.
      unsigned count = 0;
      switch (function) {
      case 0x80005da0:
      case 0x801a86ac:
        count = 1;
        break;
      case 0x801a3400:
      case 0x801a6050:
      case 0x8019fe28:
        count = 3;
        break;
      case 0x800d3bac:
      case 0x801a25e0:
      case 0x801a21a0:
      case 0x801a5a0c:
      case 0x801a0b88:
      case 0x801a1c60:
        break;
      default:
        blocked = true;
        return false;
      }
      for (unsigned i = 0; i < count; ++i)
        q.arguments[i] = scalar(r[3 + i]);
    }
    if (blocked)
      return false;
    held = q;
  }
  if (!services.call)
    return false;
  const auto result = services.call(services.user, *held);
  if (!current() || !result)
    return false;
  r[3] = Value::word(result->value);
  if (function == 0x800d3bac)
    f[1] = single(result->singleBits);
  held.reset();
  pending = 0;
  return true;
}
bool Program::run() {
  if (!current())
    return false;
  blocked = false;
  for (;;) {
    pending = pc;
    switch (pc) {
    case 0x801a60c4: {
      auto target = offset(r[1], uint32_t(-1360));
      store(target, r[1]);
      r[1] = target;
      if (blocked)
        return false;
      pc = 0x801a60c8;
      break;
    }
    case 0x801a60c8: {
      r[0] = {}; // opaque native linkage, not a guest return address
      if (blocked)
        return false;
      pc = 0x801a60cc;
      break;
    }
    case 0x801a60cc: {
      auto target = offset(r[1], uint32_t(1364));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a60d0;
      break;
    }
    case 0x801a60d0: {
      storeDouble(31, offset(r[1], uint32_t(1344)));
      if (blocked)
        return false;
      pc = 0x801a60d4;
      break;
    }
    case 0x801a60d4: {
      store(offset(r[1], 1352), {});
      store(offset(r[1], 1356), {}); // incoming paired-save cells stay unknown
      if (blocked)
        return false;
      pc = 0x801a60d8;
      break;
    }
    case 0x801a60d8: {
      storeDouble(30, offset(r[1], uint32_t(1328)));
      if (blocked)
        return false;
      pc = 0x801a60dc;
      break;
    }
    case 0x801a60dc: {
      store(offset(r[1], 1336), {});
      store(offset(r[1], 1340), {}); // incoming paired-save cells stay unknown
      if (blocked)
        return false;
      pc = 0x801a60e0;
      break;
    }
    case 0x801a60e0: {
      for (unsigned i = 20; i < 32; ++i)
        store(offset(offset(r[1], uint32_t(1280)), 4 * (i - 20)), r[i]);
      if (blocked)
        return false;
      pc = 0x801a60e4;
      break;
    }
    case 0x801a60e4: {
      auto target = offset(r[13], uint32_t(7280));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a60e8;
      break;
    }
    case 0x801a60e8: {
      r[25] = offset(r[3], uint32_t(131072));
      if (blocked)
        return false;
      pc = 0x801a60ec;
      break;
    }
    case 0x801a60ec: {
      if (!link(0x801a86a0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a60f0;
      break;
    }
    case 0x801a60f0: {
      auto target = offset(r[13], uint32_t(7280));
      r[4] = load(target);
      if (blocked)
        return false;
      pc = 0x801a60f4;
      break;
    }
    case 0x801a60f4: {
      r[30] = r[3];
      if (blocked)
        return false;
      pc = 0x801a60f8;
      break;
    }
    case 0x801a60f8: {
      auto target = offset(r[4], uint32_t(10040));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a60fc;
      break;
    }
    case 0x801a60fc: {
      auto target = offset(r[3], uint32_t(12));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6100;
      break;
    }
    case 0x801a6100: {
      r[24] = offset(r[3], uint32_t(172));
      if (blocked)
        return false;
      pc = 0x801a6104;
      break;
    }
    case 0x801a6104: {
      if (!link(0x800d5eb0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6108;
      break;
    }
    case 0x801a6108: {
      r[4] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a610c;
      break;
    }
    case 0x801a610c: {
      r[0] = Value::word(uint32_t(32));
      if (blocked)
        return false;
      pc = 0x801a6110;
      break;
    }
    case 0x801a6110: {
      r[4] = offset(r[4], uint32_t(-24916));
      if (blocked)
        return false;
      pc = 0x801a6114;
      break;
    }
    case 0x801a6114: {
      r[31] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6118;
      break;
    }
    case 0x801a6118: {
      r[5] = offset(r[1], uint32_t(196));
      if (blocked)
        return false;
      pc = 0x801a611c;
      break;
    }
    case 0x801a611c: {
      r[4] = offset(r[4], uint32_t(-4));
      if (blocked)
        return false;
      pc = 0x801a6120;
      break;
    }
    case 0x801a6120: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6124;
      break;
    }
    case 0x801a6124: {
      auto target = offset(r[4], uint32_t(4));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6128;
      break;
    }
    case 0x801a6128: {
      auto target = offset(r[4], uint32_t(8));
      r[0] = load(target);
      r[4] = target;
      if (blocked)
        return false;
      pc = 0x801a612c;
      break;
    }
    case 0x801a612c: {
      auto target = offset(r[5], uint32_t(4));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a6130;
      break;
    }
    case 0x801a6130: {
      auto target = offset(r[5], uint32_t(8));
      store(target, r[0]);
      r[5] = target;
      if (blocked)
        return false;
      pc = 0x801a6134;
      break;
    }
    case 0x801a6134: {
      --ctr;
      if (ctr) {
        pc = 0x801a6124;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6138;
      break;
    }
    case 0x801a6138: {
      auto target = offset(r[4], uint32_t(4));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a613c;
      break;
    }
    case 0x801a613c: {
      auto target = offset(r[5], uint32_t(4));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6140;
      break;
    }
    case 0x801a6140: {
      const auto addr = scalar(offset(r[31], uint32_t(136)));
      if (blocked)
        return false;
      const auto raw =
          services.actors.memory.half(services.actors.memory.user, addr);
      r[0] = Value::word(raw < 0x8000 ? uint32_t(raw)
                                      : uint32_t(int32_t(raw) - 65536));
      if (blocked)
        return false;
      pc = 0x801a6144;
      break;
    }
    case 0x801a6144: {
      compare(r[0], Value::word(uint32_t(2)), true);
      if (blocked)
        return false;
      pc = 0x801a6148;
      break;
    }
    case 0x801a6148: {
      if (condition[2]) {
        pc = 0x801a618c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a614c;
      break;
    }
    case 0x801a614c: {
      if (!condition[0]) {
        pc = 0x801a6160;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6150;
      break;
    }
    case 0x801a6150: {
      compare(r[0], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a6154;
      break;
    }
    case 0x801a6154: {
      if (condition[2]) {
        pc = 0x801a616c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6158;
      break;
    }
    case 0x801a6158: {
      if (!condition[0]) {
        pc = 0x801a617c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a615c;
      break;
    }
    case 0x801a615c: {
      pc = 0x801a61a8;
      break;
    }
    case 0x801a6160: {
      compare(r[0], Value::word(uint32_t(4)), true);
      if (blocked)
        return false;
      pc = 0x801a6164;
      break;
    }
    case 0x801a6164: {
      if (!condition[0]) {
        pc = 0x801a61a8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6168;
      break;
    }
    case 0x801a6168: {
      pc = 0x801a619c;
      break;
    }
    case 0x801a616c: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a6170;
      break;
    }
    case 0x801a6170: {
      r[0] = Value::word(uint32_t(50));
      if (blocked)
        return false;
      pc = 0x801a6174;
      break;
    }
    case 0x801a6174: {
      auto target = offset(r[3], uint32_t(14224));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6178;
      break;
    }
    case 0x801a6178: {
      pc = 0x801a61a8;
      break;
    }
    case 0x801a617c: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a6180;
      break;
    }
    case 0x801a6180: {
      r[0] = Value::word(uint32_t(100));
      if (blocked)
        return false;
      pc = 0x801a6184;
      break;
    }
    case 0x801a6184: {
      auto target = offset(r[3], uint32_t(14224));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6188;
      break;
    }
    case 0x801a6188: {
      pc = 0x801a61a8;
      break;
    }
    case 0x801a618c: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a6190;
      break;
    }
    case 0x801a6190: {
      r[0] = Value::word(uint32_t(150));
      if (blocked)
        return false;
      pc = 0x801a6194;
      break;
    }
    case 0x801a6194: {
      auto target = offset(r[3], uint32_t(14224));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6198;
      break;
    }
    case 0x801a6198: {
      pc = 0x801a61a8;
      break;
    }
    case 0x801a619c: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a61a0;
      break;
    }
    case 0x801a61a0: {
      r[0] = Value::word(uint32_t(200));
      if (blocked)
        return false;
      pc = 0x801a61a4;
      break;
    }
    case 0x801a61a4: {
      auto target = offset(r[3], uint32_t(14224));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a61a8;
      break;
    }
    case 0x801a61a8: {
      if (!link(0x800d5eb0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a61ac;
      break;
    }
    case 0x801a61ac: {
      const auto addr = scalar(offset(r[3], uint32_t(140)));
      if (blocked)
        return false;
      const auto raw =
          services.actors.memory.half(services.actors.memory.user, addr);
      r[0] = Value::word(raw);
      if (blocked)
        return false;
      pc = 0x801a61b0;
      break;
    }
    case 0x801a61b0: {
      compare(r[0], Value::word(uint32_t(0)), false);
      if (blocked)
        return false;
      pc = 0x801a61b4;
      break;
    }
    case 0x801a61b4: {
      if (!condition[2]) {
        pc = 0x801a61c4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a61b8;
      break;
    }
    case 0x801a61b8: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a61bc;
      break;
    }
    case 0x801a61bc: {
      r[0] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a61c0;
      break;
    }
    case 0x801a61c0: {
      auto target = offset(r[3], uint32_t(14224));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a61c4;
      break;
    }
    case 0x801a61c4: {
      const auto addr = scalar(offset(r[31], uint32_t(138)));
      if (blocked)
        return false;
      const auto raw =
          services.actors.memory.half(services.actors.memory.user, addr);
      r[3] = Value::word(raw < 0x8000 ? uint32_t(raw)
                                      : uint32_t(int32_t(raw) - 65536));
      if (blocked)
        return false;
      pc = 0x801a61c8;
      break;
    }
    case 0x801a61c8: {
      r[0] = Value::word(uint32_t(64));
      if (blocked)
        return false;
      pc = 0x801a61cc;
      break;
    }
    case 0x801a61cc: {
      r[3] = offset(r[3], uint32_t(-1));
      if (blocked)
        return false;
      pc = 0x801a61d0;
      break;
    }
    case 0x801a61d0: {
      compare(r[3], Value::word(uint32_t(65)), true);
      if (blocked)
        return false;
      pc = 0x801a61d4;
      break;
    }
    case 0x801a61d4: {
      if (!condition[0]) {
        pc = 0x801a61dc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a61d8;
      break;
    }
    case 0x801a61d8: {
      r[0] = r[3];
      if (blocked)
        return false;
      pc = 0x801a61dc;
      break;
    }
    case 0x801a61dc: {
      r[0] = Value::word(scalar(r[0]) << 2);
      if (blocked)
        return false;
      pc = 0x801a61e0;
      break;
    }
    case 0x801a61e0: {
      r[3] = offset(r[1], uint32_t(200));
      if (blocked)
        return false;
      pc = 0x801a61e4;
      break;
    }
    case 0x801a61e4: {
      loadSingle(0, add(r[3], r[0]));
      if (blocked)
        return false;
      pc = 0x801a61e8;
      break;
    }
    case 0x801a61e8: {
      r[5] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a61ec;
      break;
    }
    case 0x801a61ec: {
      storeSingle(0, offset(r[5], uint32_t(14200)));
      if (blocked)
        return false;
      pc = 0x801a61f0;
      break;
    }
    case 0x801a61f0: {
      auto target = offset(r[25], uint32_t(-28552));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a61f4;
      break;
    }
    case 0x801a61f4: {
      const auto raw = scalar(r[0]);
      r[0] = Value::word(raw & 0x00010000u);
      record(r[0]);
      if (blocked)
        return false;
      pc = 0x801a61f8;
      break;
    }
    case 0x801a61f8: {
      if (condition[2]) {
        pc = 0x801a6260;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a61fc;
      break;
    }
    case 0x801a61fc: {
      auto target = offset(r[25], uint32_t(-28540));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6200;
      break;
    }
    case 0x801a6200: {
      r[3] = Value::word(uint32_t(17200) << 16);
      if (blocked)
        return false;
      pc = 0x801a6204;
      break;
    }
    case 0x801a6204: {
      r[4] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a6208;
      break;
    }
    case 0x801a6208: {
      auto target = offset(r[1], uint32_t(1264));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a620c;
      break;
    }
    case 0x801a620c: {
      r[0] = Value::word(scalar(r[0]) ^ (uint32_t(32768) << 16));
      if (blocked)
        return false;
      pc = 0x801a6210;
      break;
    }
    case 0x801a6210: {
      loadDouble(2, offset(r[4], uint32_t(-23768)));
      if (blocked)
        return false;
      pc = 0x801a6214;
      break;
    }
    case 0x801a6214: {
      auto target = offset(r[1], uint32_t(1268));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6218;
      break;
    }
    case 0x801a6218: {
      loadDouble(0, offset(r[1], uint32_t(1264)));
      if (blocked)
        return false;
      pc = 0x801a621c;
      break;
    }
    case 0x801a621c: {
      auto target = offset(r[1], uint32_t(1272));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a6220;
      break;
    }
    case 0x801a6220: {
      const auto a = doubleRaw(0), b = doubleRaw(2);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 0,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6224;
      break;
    }
    case 0x801a6224: {
      storeSingle(0, offset(r[5], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a6228;
      break;
    }
    case 0x801a6228: {
      auto target = offset(r[5], uint32_t(14224));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a622c;
      break;
    }
    case 0x801a622c: {
      loadSingle(1, offset(r[5], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a6230;
      break;
    }
    case 0x801a6230: {
      r[0] = Value::word(scalar(r[0]) ^ (uint32_t(32768) << 16));
      if (blocked)
        return false;
      pc = 0x801a6234;
      break;
    }
    case 0x801a6234: {
      auto target = offset(r[1], uint32_t(1276));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6238;
      break;
    }
    case 0x801a6238: {
      loadDouble(0, offset(r[1], uint32_t(1272)));
      if (blocked)
        return false;
      pc = 0x801a623c;
      break;
    }
    case 0x801a623c: {
      const auto a = doubleRaw(0), b = doubleRaw(2);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 0,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6240;
      break;
    }
    case 0x801a6240: {
      compareFloat(1, 0);
      if (blocked)
        return false;
      pc = 0x801a6244;
      break;
    }
    case 0x801a6244: {
      if (!condition[1]) {
        pc = 0x801a62c4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6248;
      break;
    }
    case 0x801a6248: {
      auto target = offset(r[1], uint32_t(1276));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a624c;
      break;
    }
    case 0x801a624c: {
      auto target = offset(r[1], uint32_t(1272));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a6250;
      break;
    }
    case 0x801a6250: {
      loadDouble(0, offset(r[1], uint32_t(1272)));
      if (blocked)
        return false;
      pc = 0x801a6254;
      break;
    }
    case 0x801a6254: {
      const auto a = doubleRaw(0), b = doubleRaw(2);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 0,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6258;
      break;
    }
    case 0x801a6258: {
      storeSingle(0, offset(r[5], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a625c;
      break;
    }
    case 0x801a625c: {
      pc = 0x801a62c4;
      break;
    }
    case 0x801a6260: {
      if (!link(0x800d3bac))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6264;
      break;
    }
    case 0x801a6264: {
      f[30] = f[1];
      if (blocked)
        return false;
      pc = 0x801a6268;
      break;
    }
    case 0x801a6268: {
      r[3] = Value::word(uint32_t(10));
      if (blocked)
        return false;
      pc = 0x801a626c;
      break;
    }
    case 0x801a626c: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6270;
      break;
    }
    case 0x801a6270: {
      r[23] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6274;
      break;
    }
    case 0x801a6274: {
      r[3] = Value::word(uint32_t(10));
      if (blocked)
        return false;
      pc = 0x801a6278;
      break;
    }
    case 0x801a6278: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a627c;
      break;
    }
    case 0x801a627c: {
      r[3] = add(r[3], r[23]);
      if (blocked)
        return false;
      pc = 0x801a6280;
      break;
    }
    case 0x801a6280: {
      r[0] = Value::word(uint32_t(17200) << 16);
      if (blocked)
        return false;
      pc = 0x801a6284;
      break;
    }
    case 0x801a6284: {
      r[3] = offset(r[3], uint32_t(-13));
      if (blocked)
        return false;
      pc = 0x801a6288;
      break;
    }
    case 0x801a6288: {
      r[4] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a628c;
      break;
    }
    case 0x801a628c: {
      r[3] = Value::word(scalar(r[3]) ^ (uint32_t(32768) << 16));
      if (blocked)
        return false;
      pc = 0x801a6290;
      break;
    }
    case 0x801a6290: {
      auto target = offset(r[1], uint32_t(1272));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6294;
      break;
    }
    case 0x801a6294: {
      loadDouble(1, offset(r[4], uint32_t(-23768)));
      if (blocked)
        return false;
      pc = 0x801a6298;
      break;
    }
    case 0x801a6298: {
      auto target = offset(r[1], uint32_t(1276));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a629c;
      break;
    }
    case 0x801a629c: {
      loadDouble(0, offset(r[1], uint32_t(1272)));
      if (blocked)
        return false;
      pc = 0x801a62a0;
      break;
    }
    case 0x801a62a0: {
      const auto a = doubleRaw(0), b = doubleRaw(1);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 31,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a62a4;
      break;
    }
    case 0x801a62a4: {
      if (!link(0x800d3bac))
        return false;
      if (blocked)
        return false;
      pc = 0x801a62a8;
      break;
    }
    case 0x801a62a8: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a62ac;
      break;
    }
    case 0x801a62ac: {
      loadSingle(0, offset(r[2], uint32_t(-10920)));
      if (blocked)
        return false;
      pc = 0x801a62b0;
      break;
    }
    case 0x801a62b0: {
      loadSingle(2, offset(r[3], uint32_t(14200)));
      if (blocked)
        return false;
      pc = 0x801a62b4;
      break;
    }
    case 0x801a62b4: {
      const auto a = doubleRaw(1), b = doubleRaw(2);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 1,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a62b8;
      break;
    }
    case 0x801a62b8: {
      const auto a = singleRaw(1), b = singleRaw(31);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::MultiplySingle, 1, {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a62bc;
      break;
    }
    case 0x801a62bc: {
      const auto a = singleRaw(0), b = singleRaw(1), c = singleRaw(30);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::FmaSingle, 0, {a, b, c}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a62c0;
      break;
    }
    case 0x801a62c0: {
      storeSingle(0, offset(r[3], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a62c4;
      break;
    }
    case 0x801a62c4: {
      auto target = offset(r[24], uint32_t(0));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a62c8;
      break;
    }
    case 0x801a62c8: {
      compare(r[0], Value::word(uint32_t(4)), true);
      if (blocked)
        return false;
      pc = 0x801a62cc;
      break;
    }
    case 0x801a62cc: {
      if (condition[2]) {
        pc = 0x801a6304;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a62d0;
      break;
    }
    case 0x801a62d0: {
      if (!condition[0]) {
        pc = 0x801a62e8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a62d4;
      break;
    }
    case 0x801a62d4: {
      compare(r[0], Value::word(uint32_t(2)), true);
      if (blocked)
        return false;
      pc = 0x801a62d8;
      break;
    }
    case 0x801a62d8: {
      if (!condition[0]) {
        pc = 0x801a631c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a62dc;
      break;
    }
    case 0x801a62dc: {
      compare(r[0], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a62e0;
      break;
    }
    case 0x801a62e0: {
      if (!condition[0]) {
        pc = 0x801a62f4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a62e4;
      break;
    }
    case 0x801a62e4: {
      pc = 0x801a6350;
      break;
    }
    case 0x801a62e8: {
      compare(r[0], Value::word(uint32_t(7)), true);
      if (blocked)
        return false;
      pc = 0x801a62ec;
      break;
    }
    case 0x801a62ec: {
      if (!condition[0]) {
        pc = 0x801a6350;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a62f0;
      break;
    }
    case 0x801a62f0: {
      pc = 0x801a631c;
      break;
    }
    case 0x801a62f4: {
      loadSingle(0, offset(r[2], uint32_t(-10936)));
      if (blocked)
        return false;
      pc = 0x801a62f8;
      break;
    }
    case 0x801a62f8: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a62fc;
      break;
    }
    case 0x801a62fc: {
      storeSingle(0, offset(r[3], uint32_t(14208)));
      if (blocked)
        return false;
      pc = 0x801a6300;
      break;
    }
    case 0x801a6300: {
      pc = 0x801a6350;
      break;
    }
    case 0x801a6304: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a6308;
      break;
    }
    case 0x801a6308: {
      loadSingle(1, offset(r[2], uint32_t(-10696)));
      if (blocked)
        return false;
      pc = 0x801a630c;
      break;
    }
    case 0x801a630c: {
      loadSingle(0, offset(r[3], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a6310;
      break;
    }
    case 0x801a6310: {
      const auto a = singleRaw(1), b = singleRaw(0);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::MultiplySingle, 0, {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6314;
      break;
    }
    case 0x801a6314: {
      storeSingle(0, offset(r[3], uint32_t(14208)));
      if (blocked)
        return false;
      pc = 0x801a6318;
      break;
    }
    case 0x801a6318: {
      pc = 0x801a6350;
      break;
    }
    case 0x801a631c: {
      r[5] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a6320;
      break;
    }
    case 0x801a6320: {
      r[0] = Value::word(uint32_t(17200) << 16);
      if (blocked)
        return false;
      pc = 0x801a6324;
      break;
    }
    case 0x801a6324: {
      auto target = offset(r[5], uint32_t(14224));
      r[4] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6328;
      break;
    }
    case 0x801a6328: {
      r[3] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a632c;
      break;
    }
    case 0x801a632c: {
      auto target = offset(r[1], uint32_t(1272));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6330;
      break;
    }
    case 0x801a6330: {
      r[0] = Value::word(scalar(r[4]) ^ (uint32_t(32768) << 16));
      if (blocked)
        return false;
      pc = 0x801a6334;
      break;
    }
    case 0x801a6334: {
      loadDouble(2, offset(r[3], uint32_t(-23768)));
      if (blocked)
        return false;
      pc = 0x801a6338;
      break;
    }
    case 0x801a6338: {
      auto target = offset(r[1], uint32_t(1276));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a633c;
      break;
    }
    case 0x801a633c: {
      loadSingle(0, offset(r[5], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a6340;
      break;
    }
    case 0x801a6340: {
      loadDouble(1, offset(r[1], uint32_t(1272)));
      if (blocked)
        return false;
      pc = 0x801a6344;
      break;
    }
    case 0x801a6344: {
      const auto a = doubleRaw(1), b = doubleRaw(2);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 1,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6348;
      break;
    }
    case 0x801a6348: {
      const auto a = doubleRaw(1), b = doubleRaw(0);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 0,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a634c;
      break;
    }
    case 0x801a634c: {
      storeSingle(0, offset(r[5], uint32_t(14208)));
      if (blocked)
        return false;
      pc = 0x801a6350;
      break;
    }
    case 0x801a6350: {
      auto target = offset(r[24], uint32_t(0));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6354;
      break;
    }
    case 0x801a6354: {
      compare(r[0], Value::word(uint32_t(5)), true);
      if (blocked)
        return false;
      pc = 0x801a6358;
      break;
    }
    case 0x801a6358: {
      if (condition[2]) {
        pc = 0x801a63c4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a635c;
      break;
    }
    case 0x801a635c: {
      if (!condition[0]) {
        pc = 0x801a636c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6360;
      break;
    }
    case 0x801a6360: {
      compare(r[0], Value::word(uint32_t(2)), true);
      if (blocked)
        return false;
      pc = 0x801a6364;
      break;
    }
    case 0x801a6364: {
      if (condition[2]) {
        pc = 0x801a6378;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6368;
      break;
    }
    case 0x801a6368: {
      pc = 0x801a6458;
      break;
    }
    case 0x801a636c: {
      compare(r[0], Value::word(uint32_t(7)), true);
      if (blocked)
        return false;
      pc = 0x801a6370;
      break;
    }
    case 0x801a6370: {
      if (!condition[0]) {
        pc = 0x801a6458;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6374;
      break;
    }
    case 0x801a6374: {
      pc = 0x801a6408;
      break;
    }
    case 0x801a6378: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a637c;
      break;
    }
    case 0x801a637c: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6380;
      break;
    }
    case 0x801a6380: {
      r[4] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6384;
      break;
    }
    case 0x801a6384: {
      r[5] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6388;
      break;
    }
    case 0x801a6388: {
      r[7] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a638c;
      break;
    }
    case 0x801a638c: {
      r[6] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6390;
      break;
    }
    case 0x801a6390: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6394;
      break;
    }
    case 0x801a6394: {
      compare(r[7], Value::word(uint32_t(1)), true);
      if (blocked)
        return false;
      pc = 0x801a6398;
      break;
    }
    case 0x801a6398: {
      if (condition[2]) {
        pc = 0x801a63a4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a639c;
      break;
    }
    case 0x801a639c: {
      compare(r[7], Value::word(uint32_t(7)), true);
      if (blocked)
        return false;
      pc = 0x801a63a0;
      break;
    }
    case 0x801a63a0: {
      if (!condition[2]) {
        pc = 0x801a63ac;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a63a4;
      break;
    }
    case 0x801a63a4: {
      store(add(r[5], r[3]), r[6]);
      if (blocked)
        return false;
      pc = 0x801a63a8;
      break;
    }
    case 0x801a63a8: {
      pc = 0x801a63b0;
      break;
    }
    case 0x801a63ac: {
      store(add(r[5], r[3]), r[4]);
      if (blocked)
        return false;
      pc = 0x801a63b0;
      break;
    }
    case 0x801a63b0: {
      r[7] = offset(r[7], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a63b4;
      break;
    }
    case 0x801a63b4: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a63b8;
      break;
    }
    case 0x801a63b8: {
      --ctr;
      if (ctr) {
        pc = 0x801a6394;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a63bc;
      break;
    }
    case 0x801a63bc: {
      r[29] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a63c0;
      break;
    }
    case 0x801a63c0: {
      pc = 0x801a66b4;
      break;
    }
    case 0x801a63c4: {
      r[7] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a63c8;
      break;
    }
    case 0x801a63c8: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a63cc;
      break;
    }
    case 0x801a63cc: {
      r[4] = r[7];
      if (blocked)
        return false;
      pc = 0x801a63d0;
      break;
    }
    case 0x801a63d0: {
      r[5] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a63d4;
      break;
    }
    case 0x801a63d4: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a63d8;
      break;
    }
    case 0x801a63d8: {
      r[6] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a63dc;
      break;
    }
    case 0x801a63dc: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a63e0;
      break;
    }
    case 0x801a63e0: {
      compare(r[7], Value::word(uint32_t(1)), true);
      if (blocked)
        return false;
      pc = 0x801a63e4;
      break;
    }
    case 0x801a63e4: {
      if (!condition[2]) {
        pc = 0x801a63f0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a63e8;
      break;
    }
    case 0x801a63e8: {
      store(add(r[5], r[3]), r[6]);
      if (blocked)
        return false;
      pc = 0x801a63ec;
      break;
    }
    case 0x801a63ec: {
      pc = 0x801a63f4;
      break;
    }
    case 0x801a63f0: {
      store(add(r[5], r[3]), r[4]);
      if (blocked)
        return false;
      pc = 0x801a63f4;
      break;
    }
    case 0x801a63f4: {
      r[7] = offset(r[7], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a63f8;
      break;
    }
    case 0x801a63f8: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a63fc;
      break;
    }
    case 0x801a63fc: {
      --ctr;
      if (ctr) {
        pc = 0x801a63e0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6400;
      break;
    }
    case 0x801a6400: {
      r[29] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6404;
      break;
    }
    case 0x801a6404: {
      pc = 0x801a66b4;
      break;
    }
    case 0x801a6408: {
      r[3] = Value::word(uint32_t(-32709) << 16);
      if (blocked)
        return false;
      pc = 0x801a640c;
      break;
    }
    case 0x801a640c: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6410;
      break;
    }
    case 0x801a6410: {
      r[8] = offset(r[3], uint32_t(19940));
      if (blocked)
        return false;
      pc = 0x801a6414;
      break;
    }
    case 0x801a6414: {
      r[6] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6418;
      break;
    }
    case 0x801a6418: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a641c;
      break;
    }
    case 0x801a641c: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6420;
      break;
    }
    case 0x801a6420: {
      r[7] = Value::word(uint32_t(2));
      if (blocked)
        return false;
      pc = 0x801a6424;
      break;
    }
    case 0x801a6424: {
      r[5] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6428;
      break;
    }
    case 0x801a6428: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a642c;
      break;
    }
    case 0x801a642c: {
      const auto address = scalar(add(r[8], r[4]));
      if (blocked)
        return false;
      r[0] = Value::word(byte(address));
      if (blocked)
        return false;
      pc = 0x801a6430;
      break;
    }
    case 0x801a6430: {
      r[0] = Value::word(scalar(r[0]) & (0xffffffffu >> 31));
      record(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6434;
      break;
    }
    case 0x801a6434: {
      if (condition[2]) {
        pc = 0x801a6440;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6438;
      break;
    }
    case 0x801a6438: {
      store(add(r[6], r[3]), r[7]);
      if (blocked)
        return false;
      pc = 0x801a643c;
      break;
    }
    case 0x801a643c: {
      pc = 0x801a6444;
      break;
    }
    case 0x801a6440: {
      store(add(r[6], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a6444;
      break;
    }
    case 0x801a6444: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6448;
      break;
    }
    case 0x801a6448: {
      r[4] = offset(r[4], uint32_t(32));
      if (blocked)
        return false;
      pc = 0x801a644c;
      break;
    }
    case 0x801a644c: {
      --ctr;
      if (ctr) {
        pc = 0x801a642c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6450;
      break;
    }
    case 0x801a6450: {
      r[29] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6454;
      break;
    }
    case 0x801a6454: {
      pc = 0x801a66b4;
      break;
    }
    case 0x801a6458: {
      r[3] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a645c;
      break;
    }
    case 0x801a645c: {
      r[0] = Value::word(uint32_t(5));
      if (blocked)
        return false;
      pc = 0x801a6460;
      break;
    }
    case 0x801a6460: {
      r[3] = offset(r[3], uint32_t(-24656));
      if (blocked)
        return false;
      pc = 0x801a6464;
      break;
    }
    case 0x801a6464: {
      r[5] = offset(r[1], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6468;
      break;
    }
    case 0x801a6468: {
      r[4] = offset(r[3], uint32_t(-4));
      if (blocked)
        return false;
      pc = 0x801a646c;
      break;
    }
    case 0x801a646c: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6470;
      break;
    }
    case 0x801a6470: {
      auto target = offset(r[4], uint32_t(4));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6474;
      break;
    }
    case 0x801a6474: {
      auto target = offset(r[4], uint32_t(8));
      r[0] = load(target);
      r[4] = target;
      if (blocked)
        return false;
      pc = 0x801a6478;
      break;
    }
    case 0x801a6478: {
      auto target = offset(r[5], uint32_t(4));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a647c;
      break;
    }
    case 0x801a647c: {
      auto target = offset(r[5], uint32_t(8));
      store(target, r[0]);
      r[5] = target;
      if (blocked)
        return false;
      pc = 0x801a6480;
      break;
    }
    case 0x801a6480: {
      --ctr;
      if (ctr) {
        pc = 0x801a6470;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6484;
      break;
    }
    case 0x801a6484: {
      auto target = offset(r[4], uint32_t(4));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6488;
      break;
    }
    case 0x801a6488: {
      r[20] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a648c;
      break;
    }
    case 0x801a648c: {
      r[3] = Value::word(uint32_t(100));
      if (blocked)
        return false;
      pc = 0x801a6490;
      break;
    }
    case 0x801a6490: {
      auto target = offset(r[5], uint32_t(4));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a6494;
      break;
    }
    case 0x801a6494: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6498;
      break;
    }
    case 0x801a6498: {
      r[0] = Value::word(uint32_t(11));
      if (blocked)
        return false;
      pc = 0x801a649c;
      break;
    }
    case 0x801a649c: {
      r[5] = offset(r[1], uint32_t(8));
      if (blocked)
        return false;
      pc = 0x801a64a0;
      break;
    }
    case 0x801a64a0: {
      r[26] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a64a4;
      break;
    }
    case 0x801a64a4: {
      r[6] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a64a8;
      break;
    }
    case 0x801a64a8: {
      r[4] = Value::word(uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a64ac;
      break;
    }
    case 0x801a64ac: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a64b0;
      break;
    }
    case 0x801a64b0: {
      r[0] = offset(r[4], uint32_t(-4));
      if (blocked)
        return false;
      pc = 0x801a64b4;
      break;
    }
    case 0x801a64b4: {
      r[0] = load(add(r[5], r[0]));
      if (blocked)
        return false;
      pc = 0x801a64b8;
      break;
    }
    case 0x801a64b8: {
      r[20] = add(r[20], r[0]);
      if (blocked)
        return false;
      pc = 0x801a64bc;
      break;
    }
    case 0x801a64bc: {
      compare(r[3], r[20], true);
      if (blocked)
        return false;
      pc = 0x801a64c0;
      break;
    }
    case 0x801a64c0: {
      if (!condition[0]) {
        pc = 0x801a64cc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a64c4;
      break;
    }
    case 0x801a64c4: {
      r[26] = r[6];
      if (blocked)
        return false;
      pc = 0x801a64c8;
      break;
    }
    case 0x801a64c8: {
      pc = 0x801a64d8;
      break;
    }
    case 0x801a64cc: {
      r[6] = offset(r[6], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a64d0;
      break;
    }
    case 0x801a64d0: {
      r[4] = offset(r[4], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a64d4;
      break;
    }
    case 0x801a64d4: {
      --ctr;
      if (ctr) {
        pc = 0x801a64b0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a64d8;
      break;
    }
    case 0x801a64d8: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a64dc;
      break;
    }
    case 0x801a64dc: {
      r[4] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a64e0;
      break;
    }
    case 0x801a64e0: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a64e4;
      break;
    }
    case 0x801a64e4: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a64e8;
      break;
    }
    case 0x801a64e8: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a64ec;
      break;
    }
    case 0x801a64ec: {
      store(add(r[4], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a64f0;
      break;
    }
    case 0x801a64f0: {
      r[5] = offset(r[5], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a64f4;
      break;
    }
    case 0x801a64f4: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a64f8;
      break;
    }
    case 0x801a64f8: {
      --ctr;
      if (ctr) {
        pc = 0x801a64ec;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a64fc;
      break;
    }
    case 0x801a64fc: {
      r[20] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6500;
      break;
    }
    case 0x801a6500: {
      r[27] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a6504;
      break;
    }
    case 0x801a6504: {
      r[3] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6508;
      break;
    }
    case 0x801a6508: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a650c;
      break;
    }
    case 0x801a650c: {
      r[23] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6510;
      break;
    }
    case 0x801a6510: {
      r[3] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6514;
      break;
    }
    case 0x801a6514: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6518;
      break;
    }
    case 0x801a6518: {
      r[4] = Value::word(scalar(r[23]) << 2);
      if (blocked)
        return false;
      pc = 0x801a651c;
      break;
    }
    case 0x801a651c: {
      r[3] = Value::word(scalar(r[3]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6520;
      break;
    }
    case 0x801a6520: {
      r[5] = load(add(r[27], r[4]));
      if (blocked)
        return false;
      pc = 0x801a6524;
      break;
    }
    case 0x801a6524: {
      r[20] = offset(r[20], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6528;
      break;
    }
    case 0x801a6528: {
      r[0] = load(add(r[27], r[3]));
      if (blocked)
        return false;
      pc = 0x801a652c;
      break;
    }
    case 0x801a652c: {
      compare(r[20], Value::word(uint32_t(24)), true);
      if (blocked)
        return false;
      pc = 0x801a6530;
      break;
    }
    case 0x801a6530: {
      store(add(r[27], r[4]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a6534;
      break;
    }
    case 0x801a6534: {
      store(add(r[27], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a6538;
      break;
    }
    case 0x801a6538: {
      if (condition[0]) {
        pc = 0x801a6504;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a653c;
      break;
    }
    case 0x801a653c: {
      r[31] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a6540;
      break;
    }
    case 0x801a6540: {
      r[29] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6544;
      break;
    }
    case 0x801a6544: {
      r[23] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6548;
      break;
    }
    case 0x801a6548: {
      r[27] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a654c;
      break;
    }
    case 0x801a654c: {
      compare(r[27], r[26], true);
      if (blocked)
        return false;
      pc = 0x801a6550;
      break;
    }
    case 0x801a6550: {
      r[28] = load(add(r[31], r[23]));
      if (blocked)
        return false;
      pc = 0x801a6554;
      break;
    }
    case 0x801a6554: {
      if (condition[0]) {
        pc = 0x801a656c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6558;
      break;
    }
    case 0x801a6558: {
      r[0] = Value::word(scalar(r[28]) << 2);
      if (blocked)
        return false;
      pc = 0x801a655c;
      break;
    }
    case 0x801a655c: {
      r[3] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6560;
      break;
    }
    case 0x801a6560: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6564;
      break;
    }
    case 0x801a6564: {
      store(add(r[3], r[0]), r[4]);
      if (blocked)
        return false;
      pc = 0x801a6568;
      break;
    }
    case 0x801a6568: {
      pc = 0x801a65dc;
      break;
    }
    case 0x801a656c: {
      r[22] = offset(r[28], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6570;
      break;
    }
    case 0x801a6570: {
      const auto address = scalar(add(r[24], r[22]));
      if (blocked)
        return false;
      r[0] = Value::word(byte(address));
      if (blocked)
        return false;
      pc = 0x801a6574;
      break;
    }
    case 0x801a6574: {
      const auto raw = scalar(r[0]) & 255u;
      r[0] = Value::word(raw < 128 ? raw : uint32_t(int32_t(raw) - 256));
      record(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6578;
      break;
    }
    case 0x801a6578: {
      if (condition[2]) {
        pc = 0x801a65cc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a657c;
      break;
    }
    case 0x801a657c: {
      r[3] = Value::word(uint32_t(10));
      if (blocked)
        return false;
      pc = 0x801a6580;
      break;
    }
    case 0x801a6580: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6584;
      break;
    }
    case 0x801a6584: {
      const auto address = scalar(add(r[24], r[22]));
      if (blocked)
        return false;
      r[5] = Value::word(byte(address));
      if (blocked)
        return false;
      pc = 0x801a6588;
      break;
    }
    case 0x801a6588: {
      r[4] = Value::word(uint32_t(-32709) << 16);
      if (blocked)
        return false;
      pc = 0x801a658c;
      break;
    }
    case 0x801a658c: {
      r[6] = offset(r[3], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6590;
      break;
    }
    case 0x801a6590: {
      r[0] = Value::word(scalar(r[28]) << 5);
      if (blocked)
        return false;
      pc = 0x801a6594;
      break;
    }
    case 0x801a6594: {
      const auto raw = scalar(r[5]) & 255u;
      r[5] = Value::word(raw < 128 ? raw : uint32_t(int32_t(raw) - 256));
      if (blocked)
        return false;
      pc = 0x801a6598;
      break;
    }
    case 0x801a6598: {
      r[3] = offset(r[4], uint32_t(19940));
      if (blocked)
        return false;
      pc = 0x801a659c;
      break;
    }
    case 0x801a659c: {
      r[4] = Value::word(scalar(r[5]) * scalar(r[6]));
      if (blocked)
        return false;
      pc = 0x801a65a0;
      break;
    }
    case 0x801a65a0: {
      const auto address = scalar(add(r[3], r[0]));
      if (blocked)
        return false;
      r[0] = Value::word(byte(address));
      if (blocked)
        return false;
      pc = 0x801a65a4;
      break;
    }
    case 0x801a65a4: {
      r[3] = Value::word(scalar(r[28]) << 2);
      if (blocked)
        return false;
      pc = 0x801a65a8;
      break;
    }
    case 0x801a65a8: {
      r[5] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a65ac;
      break;
    }
    case 0x801a65ac: {
      r[0] = Value::word(scalar(r[0]) & (0xffffffffu >> 31));
      record(r[0]);
      if (blocked)
        return false;
      pc = 0x801a65b0;
      break;
    }
    case 0x801a65b0: {
      store(add(r[5], r[3]), r[4]);
      if (blocked)
        return false;
      pc = 0x801a65b4;
      break;
    }
    case 0x801a65b4: {
      if (condition[2]) {
        pc = 0x801a65c4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a65b8;
      break;
    }
    case 0x801a65b8: {
      r[0] = load(add(r[5], r[3]));
      if (blocked)
        return false;
      pc = 0x801a65bc;
      break;
    }
    case 0x801a65bc: {
      r[0] = Value::word(scalar(r[0]) << 1);
      if (blocked)
        return false;
      pc = 0x801a65c0;
      break;
    }
    case 0x801a65c0: {
      store(add(r[5], r[3]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a65c4;
      break;
    }
    case 0x801a65c4: {
      r[27] = offset(r[27], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a65c8;
      break;
    }
    case 0x801a65c8: {
      pc = 0x801a65dc;
      break;
    }
    case 0x801a65cc: {
      r[0] = Value::word(scalar(r[28]) << 2);
      if (blocked)
        return false;
      pc = 0x801a65d0;
      break;
    }
    case 0x801a65d0: {
      r[3] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a65d4;
      break;
    }
    case 0x801a65d4: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a65d8;
      break;
    }
    case 0x801a65d8: {
      store(add(r[3], r[0]), r[4]);
      if (blocked)
        return false;
      pc = 0x801a65dc;
      break;
    }
    case 0x801a65dc: {
      r[29] = offset(r[29], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a65e0;
      break;
    }
    case 0x801a65e0: {
      r[23] = offset(r[23], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a65e4;
      break;
    }
    case 0x801a65e4: {
      compare(r[29], Value::word(uint32_t(11)), true);
      if (blocked)
        return false;
      pc = 0x801a65e8;
      break;
    }
    case 0x801a65e8: {
      if (!condition[1]) {
        pc = 0x801a654c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a65ec;
      break;
    }
    case 0x801a65ec: {
      auto target = offset(r[25], uint32_t(-28536));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a65f0;
      break;
    }
    case 0x801a65f0: {
      compare(r[0], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a65f4;
      break;
    }
    case 0x801a65f4: {
      if (condition[2]) {
        pc = 0x801a66b0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a65f8;
      break;
    }
    case 0x801a65f8: {
      compare(r[0], Value::word(uint32_t(12)), false);
      if (blocked)
        return false;
      pc = 0x801a65fc;
      break;
    }
    case 0x801a65fc: {
      if (condition[1]) {
        pc = 0x801a6674;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6600;
      break;
    }
    case 0x801a6600: {
      r[3] = Value::word(uint32_t(-32710) << 16);
      if (blocked)
        return false;
      pc = 0x801a6604;
      break;
    }
    case 0x801a6604: {
      r[0] = Value::word(scalar(r[0]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6608;
      break;
    }
    case 0x801a6608: {
      r[3] = offset(r[3], uint32_t(-12920));
      if (blocked)
        return false;
      pc = 0x801a660c;
      break;
    }
    case 0x801a660c: {
      r[0] = load(add(r[3], r[0]));
      if (blocked)
        return false;
      pc = 0x801a6610;
      break;
    }
    case 0x801a6610: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6614;
      break;
    }
    case 0x801a6614: {
      if (blocked)
        return false;
      pc = ctr;
      break;
    }
    case 0x801a6618: {
      r[28] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a661c;
      break;
    }
    case 0x801a661c: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6620: {
      r[28] = Value::word(uint32_t(10));
      if (blocked)
        return false;
      pc = 0x801a6624;
      break;
    }
    case 0x801a6624: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6628: {
      r[28] = Value::word(uint32_t(8));
      if (blocked)
        return false;
      pc = 0x801a662c;
      break;
    }
    case 0x801a662c: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6630: {
      r[28] = Value::word(uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6634;
      break;
    }
    case 0x801a6634: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6638: {
      r[28] = Value::word(uint32_t(7));
      if (blocked)
        return false;
      pc = 0x801a663c;
      break;
    }
    case 0x801a663c: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6640: {
      r[28] = Value::word(uint32_t(2));
      if (blocked)
        return false;
      pc = 0x801a6644;
      break;
    }
    case 0x801a6644: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6648: {
      r[28] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a664c;
      break;
    }
    case 0x801a664c: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6650: {
      r[28] = Value::word(uint32_t(3));
      if (blocked)
        return false;
      pc = 0x801a6654;
      break;
    }
    case 0x801a6654: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6658: {
      r[28] = Value::word(uint32_t(5));
      if (blocked)
        return false;
      pc = 0x801a665c;
      break;
    }
    case 0x801a665c: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6660: {
      r[28] = Value::word(uint32_t(6));
      if (blocked)
        return false;
      pc = 0x801a6664;
      break;
    }
    case 0x801a6664: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6668: {
      r[28] = Value::word(uint32_t(9));
      if (blocked)
        return false;
      pc = 0x801a666c;
      break;
    }
    case 0x801a666c: {
      pc = 0x801a6674;
      break;
    }
    case 0x801a6670: {
      r[28] = Value::word(uint32_t(11));
      if (blocked)
        return false;
      pc = 0x801a6674;
      break;
    }
    case 0x801a6674: {
      r[7] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6678;
      break;
    }
    case 0x801a6678: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a667c;
      break;
    }
    case 0x801a667c: {
      r[4] = r[7];
      if (blocked)
        return false;
      pc = 0x801a6680;
      break;
    }
    case 0x801a6680: {
      r[5] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6684;
      break;
    }
    case 0x801a6684: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6688;
      break;
    }
    case 0x801a6688: {
      r[6] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a668c;
      break;
    }
    case 0x801a668c: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6690;
      break;
    }
    case 0x801a6690: {
      compare(r[7], r[28], true);
      if (blocked)
        return false;
      pc = 0x801a6694;
      break;
    }
    case 0x801a6694: {
      if (!condition[2]) {
        pc = 0x801a66a0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6698;
      break;
    }
    case 0x801a6698: {
      store(add(r[5], r[3]), r[6]);
      if (blocked)
        return false;
      pc = 0x801a669c;
      break;
    }
    case 0x801a669c: {
      pc = 0x801a66a4;
      break;
    }
    case 0x801a66a0: {
      store(add(r[5], r[3]), r[4]);
      if (blocked)
        return false;
      pc = 0x801a66a4;
      break;
    }
    case 0x801a66a4: {
      r[7] = offset(r[7], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a66a8;
      break;
    }
    case 0x801a66a8: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a66ac;
      break;
    }
    case 0x801a66ac: {
      --ctr;
      if (ctr) {
        pc = 0x801a6690;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a66b0;
      break;
    }
    case 0x801a66b0: {
      r[29] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a66b4;
      break;
    }
    case 0x801a66b4: {
      r[3] = offset(r[1], uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a66b8;
      break;
    }
    case 0x801a66b8: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a66bc;
      break;
    }
    case 0x801a66bc: {
      r[5] = Value::word(uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a66c0;
      break;
    }
    case 0x801a66c0: {
      if (!link(0x800050b4))
        return false;
      if (blocked)
        return false;
      pc = 0x801a66c4;
      break;
    }
    case 0x801a66c4: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a66c8;
      break;
    }
    case 0x801a66c8: {
      loadSingle(1, offset(r[3], uint32_t(14204)));
      if (blocked)
        return false;
      pc = 0x801a66cc;
      break;
    }
    case 0x801a66cc: {
      loadSingle(0, offset(r[3], uint32_t(14208)));
      if (blocked)
        return false;
      pc = 0x801a66d0;
      break;
    }
    case 0x801a66d0: {
      const auto a = singleRaw(1), b = singleRaw(0);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::AddSingle, 0, {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a66d4;
      break;
    }
    case 0x801a66d4: {
      if (!truncate(0, 0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a66d8;
      break;
    }
    case 0x801a66d8: {
      storeDouble(0, offset(r[1], uint32_t(1272)));
      if (blocked)
        return false;
      pc = 0x801a66dc;
      break;
    }
    case 0x801a66dc: {
      auto target = offset(r[1], uint32_t(1276));
      r[27] = load(target);
      if (blocked)
        return false;
      pc = 0x801a66e0;
      break;
    }
    case 0x801a66e0: {
      compare(r[27], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a66e4;
      break;
    }
    case 0x801a66e4: {
      if (!condition[0]) {
        pc = 0x801a66ec;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a66e8;
      break;
    }
    case 0x801a66e8: {
      r[27] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a66ec;
      break;
    }
    case 0x801a66ec: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a66f0;
      break;
    }
    case 0x801a66f0: {
      auto target = offset(r[3], uint32_t(14224));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a66f4;
      break;
    }
    case 0x801a66f4: {
      compare(r[27], r[0], true);
      if (blocked)
        return false;
      pc = 0x801a66f8;
      break;
    }
    case 0x801a66f8: {
      if (!condition[1]) {
        pc = 0x801a6700;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a66fc;
      break;
    }
    case 0x801a66fc: {
      r[27] = r[0];
      if (blocked)
        return false;
      pc = 0x801a6700;
      break;
    }
    case 0x801a6700: {
      r[3] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6704;
      break;
    }
    case 0x801a6704: {
      r[5] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6708;
      break;
    }
    case 0x801a6708: {
      r[26] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a670c;
      break;
    }
    case 0x801a670c: {
      r[0] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6710;
      break;
    }
    case 0x801a6710: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6714;
      break;
    }
    case 0x801a6714: {
      ctr = scalar(r[3]);
      if (blocked)
        return false;
      pc = 0x801a6718;
      break;
    }
    case 0x801a6718: {
      r[3] = load(add(r[5], r[4]));
      if (blocked)
        return false;
      pc = 0x801a671c;
      break;
    }
    case 0x801a671c: {
      r[4] = offset(r[4], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6720;
      break;
    }
    case 0x801a6720: {
      r[0] = add(r[0], r[3]);
      if (blocked)
        return false;
      pc = 0x801a6724;
      break;
    }
    case 0x801a6724: {
      --ctr;
      if (ctr) {
        pc = 0x801a6718;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6728;
      break;
    }
    case 0x801a6728: {
      r[4] = Value::word(uint32_t(-32709) << 16);
      if (blocked)
        return false;
      pc = 0x801a672c;
      break;
    }
    case 0x801a672c: {
      r[3] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6730;
      break;
    }
    case 0x801a6730: {
      r[6] = offset(r[4], uint32_t(19940));
      if (blocked)
        return false;
      pc = 0x801a6734;
      break;
    }
    case 0x801a6734: {
      r[8] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6738;
      break;
    }
    case 0x801a6738: {
      r[9] = offset(r[1], uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a673c;
      break;
    }
    case 0x801a673c: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6740;
      break;
    }
    case 0x801a6740: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6744;
      break;
    }
    case 0x801a6744: {
      ctr = scalar(r[3]);
      if (blocked)
        return false;
      pc = 0x801a6748;
      break;
    }
    case 0x801a6748: {
      r[7] = load(add(r[8], r[5]));
      if (blocked)
        return false;
      pc = 0x801a674c;
      break;
    }
    case 0x801a674c: {
      const auto address = scalar(add(r[6], r[4]));
      if (blocked)
        return false;
      r[3] = Value::word(byte(address));
      if (blocked)
        return false;
      pc = 0x801a6750;
      break;
    }
    case 0x801a6750: {
      r[7] = Value::word(scalar(r[27]) * scalar(r[7]));
      if (blocked)
        return false;
      pc = 0x801a6754;
      break;
    }
    case 0x801a6754: {
      r[3] = Value::word(scalar(r[3]) & (0xffffffffu >> 31));
      record(r[3]);
      if (blocked)
        return false;
      pc = 0x801a6758;
      break;
    }
    case 0x801a6758: {
      const auto a = scalar(r[7]), b = scalar(r[0]);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::DivideSigned, 3, {a, b, 0},
                   true))
        return false;
      if (blocked)
        return false;
      pc = 0x801a675c;
      break;
    }
    case 0x801a675c: {
      store(add(r[9], r[5]), r[3]);
      if (blocked)
        return false;
      pc = 0x801a6760;
      break;
    }
    case 0x801a6760: {
      if (condition[2]) {
        pc = 0x801a6784;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6764;
      break;
    }
    case 0x801a6764: {
      r[3] = load(add(r[9], r[5]));
      if (blocked)
        return false;
      pc = 0x801a6768;
      break;
    }
    case 0x801a6768: {
      const auto raw = scalar(r[3]);
      carry = (raw & 0x80000000u) && (raw & ((1u << 1) - 1));
      r[3] = Value::word((raw >> 1) |
                         ((raw & 0x80000000u) ? (0xffffffffu << (32 - 1)) : 0));
      if (blocked)
        return false;
      pc = 0x801a676c;
      break;
    }
    case 0x801a676c: {
      r[3] = Value::word(scalar(r[3]) + uint32_t(carry));
      if (blocked)
        return false;
      pc = 0x801a6770;
      break;
    }
    case 0x801a6770: {
      store(add(r[9], r[5]), r[3]);
      if (blocked)
        return false;
      pc = 0x801a6774;
      break;
    }
    case 0x801a6774: {
      r[3] = load(add(r[9], r[5]));
      if (blocked)
        return false;
      pc = 0x801a6778;
      break;
    }
    case 0x801a6778: {
      r[3] = Value::word(scalar(r[3]) << 1);
      if (blocked)
        return false;
      pc = 0x801a677c;
      break;
    }
    case 0x801a677c: {
      r[26] = add(r[26], r[3]);
      if (blocked)
        return false;
      pc = 0x801a6780;
      break;
    }
    case 0x801a6780: {
      pc = 0x801a678c;
      break;
    }
    case 0x801a6784: {
      r[3] = load(add(r[9], r[5]));
      if (blocked)
        return false;
      pc = 0x801a6788;
      break;
    }
    case 0x801a6788: {
      r[26] = add(r[26], r[3]);
      if (blocked)
        return false;
      pc = 0x801a678c;
      break;
    }
    case 0x801a678c: {
      r[4] = offset(r[4], uint32_t(32));
      if (blocked)
        return false;
      pc = 0x801a6790;
      break;
    }
    case 0x801a6790: {
      r[5] = offset(r[5], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6794;
      break;
    }
    case 0x801a6794: {
      --ctr;
      if (ctr) {
        pc = 0x801a6748;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6798;
      break;
    }
    case 0x801a6798: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a679c;
      break;
    }
    case 0x801a679c: {
      r[4] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a67a0;
      break;
    }
    case 0x801a67a0: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a67a4;
      break;
    }
    case 0x801a67a4: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a67a8;
      break;
    }
    case 0x801a67a8: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a67ac;
      break;
    }
    case 0x801a67ac: {
      store(add(r[4], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a67b0;
      break;
    }
    case 0x801a67b0: {
      r[5] = offset(r[5], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a67b4;
      break;
    }
    case 0x801a67b4: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a67b8;
      break;
    }
    case 0x801a67b8: {
      --ctr;
      if (ctr) {
        pc = 0x801a67ac;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a67bc;
      break;
    }
    case 0x801a67bc: {
      r[22] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a67c0;
      break;
    }
    case 0x801a67c0: {
      r[21] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a67c4;
      break;
    }
    case 0x801a67c4: {
      r[20] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a67c8;
      break;
    }
    case 0x801a67c8: {
      r[31] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a67cc;
      break;
    }
    case 0x801a67cc: {
      r[28] = offset(r[20], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a67d0;
      break;
    }
    case 0x801a67d0: {
      r[23] = Value::word(scalar(r[28]) << 2);
      if (blocked)
        return false;
      pc = 0x801a67d4;
      break;
    }
    case 0x801a67d4: {
      pc = 0x801a6820;
      break;
    }
    case 0x801a67d8: {
      r[3] = load(add(r[22], r[31]));
      if (blocked)
        return false;
      pc = 0x801a67dc;
      break;
    }
    case 0x801a67dc: {
      r[0] = load(add(r[22], r[23]));
      if (blocked)
        return false;
      pc = 0x801a67e0;
      break;
    }
    case 0x801a67e0: {
      compare(r[3], r[0], true);
      if (blocked)
        return false;
      pc = 0x801a67e4;
      break;
    }
    case 0x801a67e4: {
      if (!condition[2]) {
        pc = 0x801a67f8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a67e8;
      break;
    }
    case 0x801a67e8: {
      r[3] = Value::word(uint32_t(2));
      if (blocked)
        return false;
      pc = 0x801a67ec;
      break;
    }
    case 0x801a67ec: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a67f0;
      break;
    }
    case 0x801a67f0: {
      compare(r[3], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a67f4;
      break;
    }
    case 0x801a67f4: {
      if (condition[2]) {
        pc = 0x801a6808;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a67f8;
      break;
    }
    case 0x801a67f8: {
      r[3] = load(add(r[22], r[31]));
      if (blocked)
        return false;
      pc = 0x801a67fc;
      break;
    }
    case 0x801a67fc: {
      r[0] = load(add(r[22], r[23]));
      if (blocked)
        return false;
      pc = 0x801a6800;
      break;
    }
    case 0x801a6800: {
      compare(r[3], r[0], true);
      if (blocked)
        return false;
      pc = 0x801a6804;
      break;
    }
    case 0x801a6804: {
      if (!condition[0]) {
        pc = 0x801a6818;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6808;
      break;
    }
    case 0x801a6808: {
      r[3] = load(add(r[21], r[31]));
      if (blocked)
        return false;
      pc = 0x801a680c;
      break;
    }
    case 0x801a680c: {
      r[0] = load(add(r[21], r[23]));
      if (blocked)
        return false;
      pc = 0x801a6810;
      break;
    }
    case 0x801a6810: {
      store(add(r[21], r[31]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a6814;
      break;
    }
    case 0x801a6814: {
      store(add(r[21], r[23]), r[3]);
      if (blocked)
        return false;
      pc = 0x801a6818;
      break;
    }
    case 0x801a6818: {
      r[28] = offset(r[28], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a681c;
      break;
    }
    case 0x801a681c: {
      r[23] = offset(r[23], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6820;
      break;
    }
    case 0x801a6820: {
      compare(r[28], Value::word(uint32_t(11)), true);
      if (blocked)
        return false;
      pc = 0x801a6824;
      break;
    }
    case 0x801a6824: {
      if (!condition[1]) {
        pc = 0x801a67d8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6828;
      break;
    }
    case 0x801a6828: {
      r[20] = offset(r[20], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a682c;
      break;
    }
    case 0x801a682c: {
      r[31] = offset(r[31], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6830;
      break;
    }
    case 0x801a6830: {
      compare(r[20], Value::word(uint32_t(10)), true);
      if (blocked)
        return false;
      pc = 0x801a6834;
      break;
    }
    case 0x801a6834: {
      if (!condition[1]) {
        pc = 0x801a67cc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6838;
      break;
    }
    case 0x801a6838: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a683c;
      break;
    }
    case 0x801a683c: {
      r[8] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a6840;
      break;
    }
    case 0x801a6840: {
      r[4] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6844;
      break;
    }
    case 0x801a6844: {
      r[7] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6848;
      break;
    }
    case 0x801a6848: {
      pc = 0x801a68dc;
      break;
    }
    case 0x801a684c: {
      r[0] = Value::word(scalar(r[3]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6850;
      break;
    }
    case 0x801a6850: {
      r[6] = load(add(r[8], r[0]));
      if (blocked)
        return false;
      pc = 0x801a6854;
      break;
    }
    case 0x801a6854: {
      r[9] = Value::word(scalar(r[6]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6858;
      break;
    }
    case 0x801a6858: {
      r[0] = load(add(r[7], r[9]));
      if (blocked)
        return false;
      pc = 0x801a685c;
      break;
    }
    case 0x801a685c: {
      compare(r[0], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a6860;
      break;
    }
    case 0x801a6860: {
      if (!condition[1]) {
        pc = 0x801a68c8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6864;
      break;
    }
    case 0x801a6864: {
      r[5] = Value::word(uint32_t(-32709) << 16);
      if (blocked)
        return false;
      pc = 0x801a6868;
      break;
    }
    case 0x801a6868: {
      r[0] = Value::word(scalar(r[6]) << 5);
      if (blocked)
        return false;
      pc = 0x801a686c;
      break;
    }
    case 0x801a686c: {
      r[5] = offset(r[5], uint32_t(19940));
      if (blocked)
        return false;
      pc = 0x801a6870;
      break;
    }
    case 0x801a6870: {
      const auto address = scalar(add(r[5], r[0]));
      if (blocked)
        return false;
      r[0] = Value::word(byte(address));
      if (blocked)
        return false;
      pc = 0x801a6874;
      break;
    }
    case 0x801a6874: {
      r[0] = Value::word(scalar(r[0]) & (0xffffffffu >> 31));
      record(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6878;
      break;
    }
    case 0x801a6878: {
      if (condition[2]) {
        pc = 0x801a68b4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a687c;
      break;
    }
    case 0x801a687c: {
      r[0] = offset(r[27], uint32_t(-2));
      if (blocked)
        return false;
      pc = 0x801a6880;
      break;
    }
    case 0x801a6880: {
      compare(r[26], r[0], true);
      if (blocked)
        return false;
      pc = 0x801a6884;
      break;
    }
    case 0x801a6884: {
      if (condition[1]) {
        pc = 0x801a68a4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6888;
      break;
    }
    case 0x801a6888: {
      r[6] = offset(r[1], uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a688c;
      break;
    }
    case 0x801a688c: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6890;
      break;
    }
    case 0x801a6890: {
      r[5] = load(add(r[6], r[9]));
      if (blocked)
        return false;
      pc = 0x801a6894;
      break;
    }
    case 0x801a6894: {
      r[26] = offset(r[26], uint32_t(2));
      if (blocked)
        return false;
      pc = 0x801a6898;
      break;
    }
    case 0x801a6898: {
      r[0] = offset(r[5], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a689c;
      break;
    }
    case 0x801a689c: {
      store(add(r[6], r[9]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a68a0;
      break;
    }
    case 0x801a68a0: {
      pc = 0x801a68c8;
      break;
    }
    case 0x801a68a4: {
      r[4] = offset(r[4], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a68a8;
      break;
    }
    case 0x801a68a8: {
      compare(r[4], Value::word(uint32_t(12)), true);
      if (blocked)
        return false;
      pc = 0x801a68ac;
      break;
    }
    case 0x801a68ac: {
      if (!condition[0]) {
        pc = 0x801a68e4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a68b0;
      break;
    }
    case 0x801a68b0: {
      pc = 0x801a68c8;
      break;
    }
    case 0x801a68b4: {
      r[6] = offset(r[1], uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a68b8;
      break;
    }
    case 0x801a68b8: {
      r[26] = offset(r[26], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a68bc;
      break;
    }
    case 0x801a68bc: {
      r[5] = load(add(r[6], r[9]));
      if (blocked)
        return false;
      pc = 0x801a68c0;
      break;
    }
    case 0x801a68c0: {
      r[0] = offset(r[5], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a68c4;
      break;
    }
    case 0x801a68c4: {
      store(add(r[6], r[9]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a68c8;
      break;
    }
    case 0x801a68c8: {
      r[5] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a68cc;
      break;
    }
    case 0x801a68cc: {
      r[3] = offset(r[3], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a68d0;
      break;
    }
    case 0x801a68d0: {
      const auto a = scalar(r[3]), b = scalar(r[5]);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::DivideSigned, 0, {a, b, 0},
                   true))
        return false;
      if (blocked)
        return false;
      pc = 0x801a68d4;
      break;
    }
    case 0x801a68d4: {
      r[0] = Value::word(scalar(r[0]) * scalar(r[5]));
      if (blocked)
        return false;
      pc = 0x801a68d8;
      break;
    }
    case 0x801a68d8: {
      r[3] = Value::word(scalar(r[3]) - scalar(r[0]));
      if (blocked)
        return false;
      pc = 0x801a68dc;
      break;
    }
    case 0x801a68dc: {
      compare(r[26], r[27], true);
      if (blocked)
        return false;
      pc = 0x801a68e0;
      break;
    }
    case 0x801a68e0: {
      if (condition[0]) {
        pc = 0x801a684c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a68e4;
      break;
    }
    case 0x801a68e4: {
      compare(r[29], Value::word(uint32_t(1)), true);
      if (blocked)
        return false;
      pc = 0x801a68e8;
      break;
    }
    case 0x801a68e8: {
      if (!condition[2]) {
        pc = 0x801a69bc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a68ec;
      break;
    }
    case 0x801a68ec: {
      compare(r[26], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a68f0;
      break;
    }
    case 0x801a68f0: {
      if (condition[2]) {
        pc = 0x801a69bc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a68f4;
      break;
    }
    case 0x801a68f4: {
      r[3] = Value::word(uint32_t(20));
      if (blocked)
        return false;
      pc = 0x801a68f8;
      break;
    }
    case 0x801a68f8: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a68fc;
      break;
    }
    case 0x801a68fc: {
      compare(r[3], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a6900;
      break;
    }
    case 0x801a6900: {
      if (condition[2]) {
        pc = 0x801a6910;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6904;
      break;
    }
    case 0x801a6904: {
      auto target = offset(r[25], uint32_t(-28552));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6908;
      break;
    }
    case 0x801a6908: {
      const auto raw = scalar(r[0]);
      r[0] = Value::word(raw & 0x00040000u);
      record(r[0]);
      if (blocked)
        return false;
      pc = 0x801a690c;
      break;
    }
    case 0x801a690c: {
      if (condition[2]) {
        pc = 0x801a69bc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6910;
      break;
    }
    case 0x801a6910: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6914;
      break;
    }
    case 0x801a6914: {
      r[4] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a6918;
      break;
    }
    case 0x801a6918: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a691c;
      break;
    }
    case 0x801a691c: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6920;
      break;
    }
    case 0x801a6920: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6924;
      break;
    }
    case 0x801a6924: {
      store(add(r[4], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a6928;
      break;
    }
    case 0x801a6928: {
      r[5] = offset(r[5], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a692c;
      break;
    }
    case 0x801a692c: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6930;
      break;
    }
    case 0x801a6930: {
      --ctr;
      if (ctr) {
        pc = 0x801a6924;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6934;
      break;
    }
    case 0x801a6934: {
      r[22] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6938;
      break;
    }
    case 0x801a6938: {
      r[21] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a693c;
      break;
    }
    case 0x801a693c: {
      r[3] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6940;
      break;
    }
    case 0x801a6940: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6944;
      break;
    }
    case 0x801a6944: {
      r[23] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6948;
      break;
    }
    case 0x801a6948: {
      r[3] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a694c;
      break;
    }
    case 0x801a694c: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6950;
      break;
    }
    case 0x801a6950: {
      r[4] = Value::word(scalar(r[23]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6954;
      break;
    }
    case 0x801a6954: {
      r[3] = Value::word(scalar(r[3]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6958;
      break;
    }
    case 0x801a6958: {
      r[5] = load(add(r[21], r[4]));
      if (blocked)
        return false;
      pc = 0x801a695c;
      break;
    }
    case 0x801a695c: {
      r[22] = offset(r[22], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6960;
      break;
    }
    case 0x801a6960: {
      r[0] = load(add(r[21], r[3]));
      if (blocked)
        return false;
      pc = 0x801a6964;
      break;
    }
    case 0x801a6964: {
      compare(r[22], Value::word(uint32_t(24)), true);
      if (blocked)
        return false;
      pc = 0x801a6968;
      break;
    }
    case 0x801a6968: {
      store(add(r[21], r[4]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a696c;
      break;
    }
    case 0x801a696c: {
      store(add(r[21], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a6970;
      break;
    }
    case 0x801a6970: {
      if (condition[0]) {
        pc = 0x801a693c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6974;
      break;
    }
    case 0x801a6974: {
      r[0] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a6978;
      break;
    }
    case 0x801a6978: {
      r[6] = offset(r[1], uint32_t(152));
      if (blocked)
        return false;
      pc = 0x801a697c;
      break;
    }
    case 0x801a697c: {
      r[7] = offset(r[1], uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a6980;
      break;
    }
    case 0x801a6980: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6984;
      break;
    }
    case 0x801a6984: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6988;
      break;
    }
    case 0x801a6988: {
      r[0] = load(add(r[6], r[3]));
      if (blocked)
        return false;
      pc = 0x801a698c;
      break;
    }
    case 0x801a698c: {
      r[5] = Value::word(scalar(r[0]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6990;
      break;
    }
    case 0x801a6990: {
      r[4] = load(add(r[7], r[5]));
      if (blocked)
        return false;
      pc = 0x801a6994;
      break;
    }
    case 0x801a6994: {
      compare(r[4], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a6998;
      break;
    }
    case 0x801a6998: {
      if (!condition[1]) {
        pc = 0x801a69b4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a699c;
      break;
    }
    case 0x801a699c: {
      r[0] = offset(r[4], uint32_t(-1));
      if (blocked)
        return false;
      pc = 0x801a69a0;
      break;
    }
    case 0x801a69a0: {
      store(add(r[7], r[5]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a69a4;
      break;
    }
    case 0x801a69a4: {
      auto target = offset(r[1], uint32_t(100));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a69a8;
      break;
    }
    case 0x801a69a8: {
      r[0] = offset(r[3], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a69ac;
      break;
    }
    case 0x801a69ac: {
      auto target = offset(r[1], uint32_t(100));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a69b0;
      break;
    }
    case 0x801a69b0: {
      pc = 0x801a69bc;
      break;
    }
    case 0x801a69b4: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a69b8;
      break;
    }
    case 0x801a69b8: {
      --ctr;
      if (ctr) {
        pc = 0x801a6988;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a69bc;
      break;
    }
    case 0x801a69bc: {
      auto target = offset(r[24], uint32_t(0));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a69c0;
      break;
    }
    case 0x801a69c0: {
      compare(r[0], Value::word(uint32_t(2)), true);
      if (blocked)
        return false;
      pc = 0x801a69c4;
      break;
    }
    case 0x801a69c4: {
      if (condition[2]) {
        pc = 0x801a69cc;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a69c8;
      break;
    }
    case 0x801a69c8: {
      pc = 0x801a6a14;
      break;
    }
    case 0x801a69cc: {
      r[23] = Value::word(uint32_t(20));
      if (blocked)
        return false;
      pc = 0x801a69d0;
      break;
    }
    case 0x801a69d0: {
      pc = 0x801a6a04;
      break;
    }
    case 0x801a69d4: {
      const auto a = scalar(r[3]), b = scalar(r[23]);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::DivideSigned, 0, {a, b, 0},
                   true))
        return false;
      if (blocked)
        return false;
      pc = 0x801a69d8;
      break;
    }
    case 0x801a69d8: {
      r[0] = Value::word(scalar(r[0]) * scalar(r[23]));
      if (blocked)
        return false;
      pc = 0x801a69dc;
      break;
    }
    case 0x801a69dc: {
      r[0] = Value::word(scalar(r[3]) - scalar(r[0]));
      if (blocked)
        return false;
      pc = 0x801a69e0;
      break;
    }
    case 0x801a69e0: {
      compare(r[0], Value::word(uint32_t(9)), true);
      if (blocked)
        return false;
      pc = 0x801a69e4;
      break;
    }
    case 0x801a69e4: {
      if (condition[1]) {
        pc = 0x801a69f8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a69e8;
      break;
    }
    case 0x801a69e8: {
      r[4] = Value::word(uint32_t(7));
      if (blocked)
        return false;
      pc = 0x801a69ec;
      break;
    }
    case 0x801a69ec: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a69f0;
      break;
    }
    case 0x801a69f0: {
      if (!link(0x801a3400))
        return false;
      if (blocked)
        return false;
      pc = 0x801a69f4;
      break;
    }
    case 0x801a69f4: {
      pc = 0x801a6a04;
      break;
    }
    case 0x801a69f8: {
      r[4] = Value::word(uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a69fc;
      break;
    }
    case 0x801a69fc: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6a00;
      break;
    }
    case 0x801a6a00: {
      if (!link(0x801a3400))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6a04;
      break;
    }
    case 0x801a6a04: {
      if (!link(0x801a25e0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6a08;
      break;
    }
    case 0x801a6a08: {
      compare(r[3], Value::word(uint32_t(-1)), true);
      if (blocked)
        return false;
      pc = 0x801a6a0c;
      break;
    }
    case 0x801a6a0c: {
      if (!condition[2]) {
        pc = 0x801a69d4;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6a10;
      break;
    }
    case 0x801a6a10: {
      pc = 0x801a6ad8;
      break;
    }
    case 0x801a6a14: {
      r[5] = offset(r[1], uint32_t(460));
      if (blocked)
        return false;
      pc = 0x801a6a18;
      break;
    }
    case 0x801a6a18: {
      r[6] = offset(r[1], uint32_t(52));
      if (blocked)
        return false;
      pc = 0x801a6a1c;
      break;
    }
    case 0x801a6a1c: {
      r[7] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6a20;
      break;
    }
    case 0x801a6a20: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6a24;
      break;
    }
    case 0x801a6a24: {
      r[24] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6a28;
      break;
    }
    case 0x801a6a28: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6a2c;
      break;
    }
    case 0x801a6a2c: {
      r[0] = load(add(r[6], r[3]));
      if (blocked)
        return false;
      pc = 0x801a6a30;
      break;
    }
    case 0x801a6a30: {
      ctr = scalar(r[0]);
      if (blocked)
        return false;
      pc = 0x801a6a34;
      break;
    }
    case 0x801a6a34: {
      compare(r[0], Value::word(uint32_t(0)), true);
      if (blocked)
        return false;
      pc = 0x801a6a38;
      break;
    }
    case 0x801a6a38: {
      if (!condition[1]) {
        pc = 0x801a6a4c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6a3c;
      break;
    }
    case 0x801a6a3c: {
      store(add(r[5], r[4]), r[7]);
      if (blocked)
        return false;
      pc = 0x801a6a40;
      break;
    }
    case 0x801a6a40: {
      r[24] = offset(r[24], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6a44;
      break;
    }
    case 0x801a6a44: {
      r[4] = offset(r[4], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6a48;
      break;
    }
    case 0x801a6a48: {
      --ctr;
      if (ctr) {
        pc = 0x801a6a3c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6a4c;
      break;
    }
    case 0x801a6a4c: {
      r[7] = offset(r[7], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6a50;
      break;
    }
    case 0x801a6a50: {
      r[3] = offset(r[3], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6a54;
      break;
    }
    case 0x801a6a54: {
      compare(r[7], Value::word(uint32_t(12)), true);
      if (blocked)
        return false;
      pc = 0x801a6a58;
      break;
    }
    case 0x801a6a58: {
      if (!condition[1]) {
        pc = 0x801a6a2c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6a5c;
      break;
    }
    case 0x801a6a5c: {
      r[22] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6a60;
      break;
    }
    case 0x801a6a60: {
      r[21] = offset(r[1], uint32_t(460));
      if (blocked)
        return false;
      pc = 0x801a6a64;
      break;
    }
    case 0x801a6a64: {
      r[3] = r[24];
      if (blocked)
        return false;
      pc = 0x801a6a68;
      break;
    }
    case 0x801a6a68: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6a6c;
      break;
    }
    case 0x801a6a6c: {
      r[23] = r[3];
      if (blocked)
        return false;
      pc = 0x801a6a70;
      break;
    }
    case 0x801a6a70: {
      r[3] = r[24];
      if (blocked)
        return false;
      pc = 0x801a6a74;
      break;
    }
    case 0x801a6a74: {
      if (!link(0x80005da0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6a78;
      break;
    }
    case 0x801a6a78: {
      r[4] = Value::word(scalar(r[23]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6a7c;
      break;
    }
    case 0x801a6a7c: {
      r[3] = Value::word(scalar(r[3]) << 2);
      if (blocked)
        return false;
      pc = 0x801a6a80;
      break;
    }
    case 0x801a6a80: {
      r[5] = load(add(r[21], r[4]));
      if (blocked)
        return false;
      pc = 0x801a6a84;
      break;
    }
    case 0x801a6a84: {
      r[22] = offset(r[22], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6a88;
      break;
    }
    case 0x801a6a88: {
      r[0] = load(add(r[21], r[3]));
      if (blocked)
        return false;
      pc = 0x801a6a8c;
      break;
    }
    case 0x801a6a8c: {
      compare(r[22], Value::word(uint32_t(150)), true);
      if (blocked)
        return false;
      pc = 0x801a6a90;
      break;
    }
    case 0x801a6a90: {
      store(add(r[21], r[4]), r[0]);
      if (blocked)
        return false;
      pc = 0x801a6a94;
      break;
    }
    case 0x801a6a94: {
      store(add(r[21], r[3]), r[5]);
      if (blocked)
        return false;
      pc = 0x801a6a98;
      break;
    }
    case 0x801a6a98: {
      if (condition[0]) {
        pc = 0x801a6a64;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6a9c;
      break;
    }
    case 0x801a6a9c: {
      r[23] = offset(r[1], uint32_t(460));
      if (blocked)
        return false;
      pc = 0x801a6aa0;
      break;
    }
    case 0x801a6aa0: {
      r[21] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6aa4;
      break;
    }
    case 0x801a6aa4: {
      r[25] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6aa8;
      break;
    }
    case 0x801a6aa8: {
      pc = 0x801a6ad0;
      break;
    }
    case 0x801a6aac: {
      if (!link(0x801a21a0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6ab0;
      break;
    }
    case 0x801a6ab0: {
      compare(r[3], Value::word(uint32_t(-1)), true);
      if (blocked)
        return false;
      pc = 0x801a6ab4;
      break;
    }
    case 0x801a6ab4: {
      if (condition[2]) {
        pc = 0x801a6ad8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6ab8;
      break;
    }
    case 0x801a6ab8: {
      r[0] = load(add(r[23], r[25]));
      if (blocked)
        return false;
      pc = 0x801a6abc;
      break;
    }
    case 0x801a6abc: {
      r[5] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a6ac0;
      break;
    }
    case 0x801a6ac0: {
      r[4] = Value::word(scalar(r[0]) & (0xffffffffu >> 24));
      if (blocked)
        return false;
      pc = 0x801a6ac4;
      break;
    }
    case 0x801a6ac4: {
      if (!link(0x801a3400))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6ac8;
      break;
    }
    case 0x801a6ac8: {
      r[21] = offset(r[21], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a6acc;
      break;
    }
    case 0x801a6acc: {
      r[25] = offset(r[25], uint32_t(4));
      if (blocked)
        return false;
      pc = 0x801a6ad0;
      break;
    }
    case 0x801a6ad0: {
      compare(r[21], r[24], true);
      if (blocked)
        return false;
      pc = 0x801a6ad4;
      break;
    }
    case 0x801a6ad4: {
      if (condition[0]) {
        pc = 0x801a6aac;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a6ad8;
      break;
    }
    case 0x801a6ad8: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a6adc;
      break;
    }
    case 0x801a6adc: {
      r[4] = offset(r[1], uint32_t(104));
      if (blocked)
        return false;
      pc = 0x801a6ae0;
      break;
    }
    case 0x801a6ae0: {
      r[5] = Value::word(uint32_t(48));
      if (blocked)
        return false;
      pc = 0x801a6ae4;
      break;
    }
    case 0x801a6ae4: {
      r[3] = offset(r[3], uint32_t(14228));
      if (blocked)
        return false;
      pc = 0x801a6ae8;
      break;
    }
    case 0x801a6ae8: {
      if (!link(0x8000519c))
        return false;
      if (blocked)
        return false;
      pc = 0x801a6aec;
      break;
    }
    case 0x801a6aec: { // Native linkage restores incoming paired-save metadata;
                       // no guest effect.
      if (blocked)
        return false;
      pc = 0x801a6af0;
      break;
    }
    case 0x801a6af0: {
      loadDouble(31, offset(r[1], uint32_t(1344)));
      if (blocked)
        return false;
      pc = 0x801a6af4;
      break;
    }
    case 0x801a6af4: { // Native linkage restores incoming paired-save metadata;
                       // no guest effect.
      if (blocked)
        return false;
      pc = 0x801a6af8;
      break;
    }
    case 0x801a6af8: {
      loadDouble(30, offset(r[1], uint32_t(1328)));
      if (blocked)
        return false;
      pc = 0x801a6afc;
      break;
    }
    case 0x801a6afc: {
      for (unsigned i = 20; i < 32; ++i)
        r[i] = load(offset(offset(r[1], uint32_t(1280)), 4 * (i - 20)));
      if (blocked)
        return false;
      pc = 0x801a6b00;
      break;
    }
    case 0x801a6b00: {
      auto target = offset(r[1], uint32_t(1364));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a6b04;
      break;
    }
    case 0x801a6b04: { // Original linkage restoration: native caller owns
                       // actual return.
      if (blocked)
        return false;
      pc = 0x801a6b08;
      break;
    }
    case 0x801a6b08: {
      r[1] = offset(r[1], uint32_t(1360));
      if (blocked)
        return false;
      pc = 0x801a6b0c;
      break;
    }
    case 0x801a6b0c:
      pending = 0;
      return true;
    case 0x801a83f8: {
      auto target = offset(r[1], uint32_t(-48));
      store(target, r[1]);
      r[1] = target;
      if (blocked)
        return false;
      pc = 0x801a83fc;
      break;
    }
    case 0x801a83fc: {
      r[0] = {}; // opaque native linkage, not a guest return address
      if (blocked)
        return false;
      pc = 0x801a8400;
      break;
    }
    case 0x801a8400: {
      auto target = offset(r[1], uint32_t(52));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a8404;
      break;
    }
    case 0x801a8404: {
      for (unsigned i = 25; i < 32; ++i)
        store(offset(offset(r[1], uint32_t(20)), 4 * (i - 25)), r[i]);
      if (blocked)
        return false;
      pc = 0x801a8408;
      break;
    }
    case 0x801a8408: {
      if (!link(0x801a86a0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a840c;
      break;
    }
    case 0x801a840c: {
      r[30] = r[3];
      if (blocked)
        return false;
      pc = 0x801a8410;
      break;
    }
    case 0x801a8410: {
      if (!link(0x801a85fc))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8414;
      break;
    }
    case 0x801a8414: {
      r[31] = r[3];
      if (blocked)
        return false;
      pc = 0x801a8418;
      break;
    }
    case 0x801a8418: {
      if (!link(0x800d5eb0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a841c;
      break;
    }
    case 0x801a841c: {
      r[4] = Value::word(uint32_t(1) << 16);
      if (blocked)
        return false;
      pc = 0x801a8420;
      break;
    }
    case 0x801a8420: {
      r[3] = r[30];
      if (blocked)
        return false;
      pc = 0x801a8424;
      break;
    }
    case 0x801a8424: {
      r[5] = offset(r[4], uint32_t(14612));
      if (blocked)
        return false;
      pc = 0x801a8428;
      break;
    }
    case 0x801a8428: {
      r[4] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a842c;
      break;
    }
    case 0x801a842c: {
      if (!link(0x800050b4))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8430;
      break;
    }
    case 0x801a8430: {
      if (!link(0x801a60c4))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8434;
      break;
    }
    case 0x801a8434: {
      r[3] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a8438;
      break;
    }
    case 0x801a8438: {
      r[3] = offset(r[3], uint32_t(-22768));
      if (blocked)
        return false;
      pc = 0x801a843c;
      break;
    }
    case 0x801a843c: {
      if (!link(0x801a86ac))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8440;
      break;
    }
    case 0x801a8440: {
      r[4] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a8444;
      break;
    }
    case 0x801a8444: {
      auto target = offset(r[30], uint32_t(12));
      store(target, r[3]);
      if (blocked)
        return false;
      pc = 0x801a8448;
      break;
    }
    case 0x801a8448: {
      r[5] = offset(r[4], uint32_t(-22732));
      if (blocked)
        return false;
      pc = 0x801a844c;
      break;
    }
    case 0x801a844c: {
      r[3] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a8450;
      break;
    }
    case 0x801a8450: {
      r[4] = Value::word(uint32_t(12));
      if (blocked)
        return false;
      pc = 0x801a8454;
      break;
    }
    case 0x801a8454: {
      if (!link(0x801a6050))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8458;
      break;
    }
    case 0x801a8458: {
      if (!link(0x801a5a0c))
        return false;
      if (blocked)
        return false;
      pc = 0x801a845c;
      break;
    }
    case 0x801a845c: {
      r[29] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a8460;
      break;
    }
    case 0x801a8460: {
      r[28] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a8464;
      break;
    }
    case 0x801a8464: {
      auto target = offset(r[28], uint32_t(14600));
      store(target, r[29]);
      if (blocked)
        return false;
      pc = 0x801a8468;
      break;
    }
    case 0x801a8468: {
      r[25] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a846c;
      break;
    }
    case 0x801a846c: {
      auto target = offset(r[28], uint32_t(14220));
      store(target, r[29]);
      if (blocked)
        return false;
      pc = 0x801a8470;
      break;
    }
    case 0x801a8470: {
      auto target = offset(r[28], uint32_t(14216));
      store(target, r[29]);
      if (blocked)
        return false;
      pc = 0x801a8474;
      break;
    }
    case 0x801a8474: {
      auto target = offset(r[28], uint32_t(14212));
      store(target, r[29]);
      if (blocked)
        return false;
      pc = 0x801a8478;
      break;
    }
    case 0x801a8478: {
      r[26] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a847c;
      break;
    }
    case 0x801a847c: {
      r[27] = offset(r[29], uint32_t(10));
      if (blocked)
        return false;
      pc = 0x801a8480;
      break;
    }
    case 0x801a8480: {
      r[27] = add(r[26], r[27]);
      if (blocked)
        return false;
      pc = 0x801a8484;
      break;
    }
    case 0x801a8484: {
      r[3] = r[27];
      if (blocked)
        return false;
      pc = 0x801a8488;
      break;
    }
    case 0x801a8488: {
      if (!link(0x801a29c0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a848c;
      break;
    }
    case 0x801a848c: {
      r[0] = Value::word(scalar(r[3]) & (0xffffffffu >> 24));
      if (blocked)
        return false;
      pc = 0x801a8490;
      break;
    }
    case 0x801a8490: {
      compare(r[0], Value::word(uint32_t(1)), false);
      if (blocked)
        return false;
      pc = 0x801a8494;
      break;
    }
    case 0x801a8494: {
      if (!condition[2]) {
        pc = 0x801a84c0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a8498;
      break;
    }
    case 0x801a8498: {
      r[3] = r[27];
      if (blocked)
        return false;
      pc = 0x801a849c;
      break;
    }
    case 0x801a849c: {
      if (!link(0x801a8664))
        return false;
      if (blocked)
        return false;
      pc = 0x801a84a0;
      break;
    }
    case 0x801a84a0: {
      const auto addr = scalar(offset(r[3], uint32_t(25)));
      if (blocked)
        return false;
      r[0] = Value::word(byte(addr));
      if (blocked)
        return false;
      pc = 0x801a84a4;
      break;
    }
    case 0x801a84a4: {
      compare(r[0], Value::word(uint32_t(12)), false);
      if (blocked)
        return false;
      pc = 0x801a84a8;
      break;
    }
    case 0x801a84a8: {
      if (condition[2]) {
        pc = 0x801a84c0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a84ac;
      break;
    }
    case 0x801a84ac: {
      compare(r[0], Value::word(uint32_t(11)), false);
      if (blocked)
        return false;
      pc = 0x801a84b0;
      break;
    }
    case 0x801a84b0: {
      if (condition[2]) {
        pc = 0x801a84c0;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a84b4;
      break;
    }
    case 0x801a84b4: {
      auto target = offset(r[28], uint32_t(14216));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a84b8;
      break;
    }
    case 0x801a84b8: {
      r[0] = offset(r[3], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a84bc;
      break;
    }
    case 0x801a84bc: {
      auto target = offset(r[28], uint32_t(14216));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a84c0;
      break;
    }
    case 0x801a84c0: {
      r[26] = offset(r[26], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a84c4;
      break;
    }
    case 0x801a84c4: {
      compare(r[26], Value::word(uint32_t(10)), true);
      if (blocked)
        return false;
      pc = 0x801a84c8;
      break;
    }
    case 0x801a84c8: {
      if (condition[0]) {
        pc = 0x801a847c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a84cc;
      break;
    }
    case 0x801a84cc: {
      r[25] = offset(r[25], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a84d0;
      break;
    }
    case 0x801a84d0: {
      r[29] = offset(r[29], uint32_t(20));
      if (blocked)
        return false;
      pc = 0x801a84d4;
      break;
    }
    case 0x801a84d4: {
      compare(r[25], Value::word(uint32_t(10)), true);
      if (blocked)
        return false;
      pc = 0x801a84d8;
      break;
    }
    case 0x801a84d8: {
      if (condition[0]) {
        pc = 0x801a8478;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a84dc;
      break;
    }
    case 0x801a84dc: {
      r[28] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a84e0;
      break;
    }
    case 0x801a84e0: {
      r[27] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a84e4;
      break;
    }
    case 0x801a84e4: {
      r[29] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a84e8;
      break;
    }
    case 0x801a84e8: {
      r[26] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a84ec;
      break;
    }
    case 0x801a84ec: {
      r[25] = add(r[26], r[29]);
      if (blocked)
        return false;
      pc = 0x801a84f0;
      break;
    }
    case 0x801a84f0: {
      r[3] = r[25];
      if (blocked)
        return false;
      pc = 0x801a84f4;
      break;
    }
    case 0x801a84f4: {
      if (!link(0x801a29c0))
        return false;
      if (blocked)
        return false;
      pc = 0x801a84f8;
      break;
    }
    case 0x801a84f8: {
      r[0] = Value::word(scalar(r[3]) & (0xffffffffu >> 24));
      if (blocked)
        return false;
      pc = 0x801a84fc;
      break;
    }
    case 0x801a84fc: {
      compare(r[0], Value::word(uint32_t(1)), false);
      if (blocked)
        return false;
      pc = 0x801a8500;
      break;
    }
    case 0x801a8500: {
      if (!condition[2]) {
        pc = 0x801a852c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a8504;
      break;
    }
    case 0x801a8504: {
      r[3] = r[25];
      if (blocked)
        return false;
      pc = 0x801a8508;
      break;
    }
    case 0x801a8508: {
      if (!link(0x801a8664))
        return false;
      if (blocked)
        return false;
      pc = 0x801a850c;
      break;
    }
    case 0x801a850c: {
      const auto addr = scalar(offset(r[3], uint32_t(25)));
      if (blocked)
        return false;
      r[0] = Value::word(byte(addr));
      if (blocked)
        return false;
      pc = 0x801a8510;
      break;
    }
    case 0x801a8510: {
      compare(r[0], Value::word(uint32_t(12)), false);
      if (blocked)
        return false;
      pc = 0x801a8514;
      break;
    }
    case 0x801a8514: {
      if (condition[2]) {
        pc = 0x801a852c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a8518;
      break;
    }
    case 0x801a8518: {
      compare(r[0], Value::word(uint32_t(11)), false);
      if (blocked)
        return false;
      pc = 0x801a851c;
      break;
    }
    case 0x801a851c: {
      if (condition[2]) {
        pc = 0x801a852c;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a8520;
      break;
    }
    case 0x801a8520: {
      auto target = offset(r[28], uint32_t(14220));
      r[3] = load(target);
      if (blocked)
        return false;
      pc = 0x801a8524;
      break;
    }
    case 0x801a8524: {
      r[0] = offset(r[3], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a8528;
      break;
    }
    case 0x801a8528: {
      auto target = offset(r[28], uint32_t(14220));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a852c;
      break;
    }
    case 0x801a852c: {
      r[26] = offset(r[26], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a8530;
      break;
    }
    case 0x801a8530: {
      compare(r[26], Value::word(uint32_t(10)), true);
      if (blocked)
        return false;
      pc = 0x801a8534;
      break;
    }
    case 0x801a8534: {
      if (condition[0]) {
        pc = 0x801a84ec;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a8538;
      break;
    }
    case 0x801a8538: {
      r[27] = offset(r[27], uint32_t(1));
      if (blocked)
        return false;
      pc = 0x801a853c;
      break;
    }
    case 0x801a853c: {
      r[29] = offset(r[29], uint32_t(20));
      if (blocked)
        return false;
      pc = 0x801a8540;
      break;
    }
    case 0x801a8540: {
      compare(r[27], Value::word(uint32_t(10)), true);
      if (blocked)
        return false;
      pc = 0x801a8544;
      break;
    }
    case 0x801a8544: {
      if (condition[0]) {
        pc = 0x801a84e8;
        break;
      }
      if (blocked)
        return false;
      pc = 0x801a8548;
      break;
    }
    case 0x801a8548: {
      r[6] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a854c;
      break;
    }
    case 0x801a854c: {
      r[0] = Value::word(uint32_t(17200) << 16);
      if (blocked)
        return false;
      pc = 0x801a8550;
      break;
    }
    case 0x801a8550: {
      auto target = offset(r[6], uint32_t(14216));
      r[5] = load(target);
      if (blocked)
        return false;
      pc = 0x801a8554;
      break;
    }
    case 0x801a8554: {
      r[3] = Value::word(uint32_t(-32720) << 16);
      if (blocked)
        return false;
      pc = 0x801a8558;
      break;
    }
    case 0x801a8558: {
      auto target = offset(r[6], uint32_t(14220));
      r[4] = load(target);
      if (blocked)
        return false;
      pc = 0x801a855c;
      break;
    }
    case 0x801a855c: {
      auto target = offset(r[1], uint32_t(8));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a8560;
      break;
    }
    case 0x801a8560: {
      r[0] = add(r[5], r[4]);
      if (blocked)
        return false;
      pc = 0x801a8564;
      break;
    }
    case 0x801a8564: {
      loadDouble(1, offset(r[3], uint32_t(-23768)));
      if (blocked)
        return false;
      pc = 0x801a8568;
      break;
    }
    case 0x801a8568: {
      auto target = offset(r[6], uint32_t(14212));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a856c;
      break;
    }
    case 0x801a856c: {
      auto target = offset(r[6], uint32_t(14212));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a8570;
      break;
    }
    case 0x801a8570: {
      r[0] = Value::word(scalar(r[0]) ^ (uint32_t(32768) << 16));
      if (blocked)
        return false;
      pc = 0x801a8574;
      break;
    }
    case 0x801a8574: {
      auto target = offset(r[1], uint32_t(12));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a8578;
      break;
    }
    case 0x801a8578: {
      loadDouble(0, offset(r[1], uint32_t(8)));
      if (blocked)
        return false;
      pc = 0x801a857c;
      break;
    }
    case 0x801a857c: {
      const auto a = doubleRaw(0), b = doubleRaw(1);
      if (blocked)
        return false;
      if (!numeric(BattleAudienceSetupNumeric::SubtractDoubleToSingle, 0,
                   {a, b, 0}))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8580;
      break;
    }
    case 0x801a8580: {
      storeSingle(0, offset(r[31], uint32_t(24)));
      if (blocked)
        return false;
      pc = 0x801a8584;
      break;
    }
    case 0x801a8584: {
      if (!link(0x801a0b88))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8588;
      break;
    }
    case 0x801a8588: {
      r[3] = Value::word(uint32_t(2));
      if (blocked)
        return false;
      pc = 0x801a858c;
      break;
    }
    case 0x801a858c: {
      r[4] = Value::word(uint32_t(60));
      if (blocked)
        return false;
      pc = 0x801a8590;
      break;
    }
    case 0x801a8590: {
      r[5] = Value::word(uint32_t(180));
      if (blocked)
        return false;
      pc = 0x801a8594;
      break;
    }
    case 0x801a8594: {
      if (!link(0x8019fe28))
        return false;
      if (blocked)
        return false;
      pc = 0x801a8598;
      break;
    }
    case 0x801a8598: {
      r[3] = offset(r[30], uint32_t(65536));
      if (blocked)
        return false;
      pc = 0x801a859c;
      break;
    }
    case 0x801a859c: {
      r[0] = Value::word(uint32_t(0));
      if (blocked)
        return false;
      pc = 0x801a85a0;
      break;
    }
    case 0x801a85a0: {
      auto target = offset(r[3], uint32_t(14608));
      store(target, r[0]);
      if (blocked)
        return false;
      pc = 0x801a85a4;
      break;
    }
    case 0x801a85a4: {
      if (!link(0x801a1c60))
        return false;
      if (blocked)
        return false;
      pc = 0x801a85a8;
      break;
    }
    case 0x801a85a8: {
      for (unsigned i = 25; i < 32; ++i)
        r[i] = load(offset(offset(r[1], uint32_t(20)), 4 * (i - 25)));
      if (blocked)
        return false;
      pc = 0x801a85ac;
      break;
    }
    case 0x801a85ac: {
      auto target = offset(r[1], uint32_t(52));
      r[0] = load(target);
      if (blocked)
        return false;
      pc = 0x801a85b0;
      break;
    }
    case 0x801a85b0: { // Original linkage restoration: native caller owns
                       // actual return.
      if (blocked)
        return false;
      pc = 0x801a85b4;
      break;
    }
    case 0x801a85b4: {
      r[1] = offset(r[1], uint32_t(48));
      if (blocked)
        return false;
      pc = 0x801a85b8;
      break;
    }
    case 0x801a85b8:
      pending = 0;
      return true;
    default:
      blocked = true;
      return false;
    }
  }
}
} // namespace
struct BattleAudienceSettingCursor::State {
  Program program;
  explicit State(const BattleAudienceSetupServices &s)
      : program(s, SettingEntry) {}
};
struct BattleAudienceInitCursor::State {
  Program program;
  explicit State(const BattleAudienceSetupServices &s)
      : program(s, InitEntry) {}
};
namespace {
struct Invocation {
  bool &active;
  explicit Invocation(bool &b) : active(b) { active = true; }
  ~Invocation() { active = false; }
};
} // namespace
#define TTYD_DEFINE_AUDIENCE_CURSOR(Name, Fn)                                  \
  Name::Name() = default;                                                      \
  Name::~Name() = default;                                                     \
  bool Name::active() const { return bool(state_); }                           \
  uint32_t Name::pending() const {                                             \
    return state_ ? state_->program.pending : 0;                               \
  }                                                                            \
  bool Name::reset() {                                                         \
    if (invoking_)                                                             \
      return false;                                                            \
    state_.reset();                                                            \
    return true;                                                               \
  }                                                                            \
  bool Fn(const BattleAudienceSetupServices &s, Name &cursor) {                \
    if (cursor.invoking_ || !admitted(s) ||                                    \
        (cursor.state_ && !same(cursor.state_->program.services, s)))          \
      return false;                                                            \
    Invocation invocation(cursor.invoking_);                                   \
    if (!cursor.state_)                                                        \
      cursor.state_ = std::make_unique<Name::State>(s);                        \
    if (!cursor.state_->program.run())                                         \
      return false;                                                            \
    cursor.state_.reset();                                                     \
    return true;                                                               \
  }
TTYD_DEFINE_AUDIENCE_CURSOR(BattleAudienceSettingCursor,
                            resumePackedBattleAudienceSetting)
TTYD_DEFINE_AUDIENCE_CURSOR(BattleAudienceInitCursor,
                            resumePackedBattleAudienceInit)
#undef TTYD_DEFINE_AUDIENCE_CURSOR
} // namespace ttyd

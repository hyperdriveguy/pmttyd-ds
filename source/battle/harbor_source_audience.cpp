#include "harbor_source_audience.hpp"
#include "battle_audience_sound.hpp"
#include "battle_event_callbacks.hpp"
#include "source_mario_dvd_root.hpp"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, PouchGlobal = 0x8041eb00;
constexpr uint32_t AudienceOffset = 0x2858, ActorOffset = 0x1bc,
                   ActorStride = 0x134;
constexpr uint32_t FileTypeTpl = 4, PathCapacity = 256;
constexpr uint32_t BattleWorkBytes = 0x19098, SourceAudienceSlots = 200;
constexpr uint32_t DesiredCountOffset = 0x1377c, ExtraCountOffset = 0x13780,
                   CapacityOffset = 0x13790, PrivateLayoutOffset = 0x13794;
constexpr uint32_t PrivateLayoutBytes = 48;
uint32_t bits(float f) {
  uint32_t v;
  std::memcpy(&v, &f, 4);
  return v;
}
float single(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
double wide(uint64_t v) {
  double d;
  std::memcpy(&d, &v, 8);
  return d;
}
int32_t integer(uint32_t v) {
  return v < 0x80000000u ? int32_t(v) : int32_t(int64_t(v) - 0x100000000ll);
}
struct Guard {
  bool &v;
  explicit Guard(bool &x) : v(x) { v = true; }
  ~Guard() { v = false; }
};
bool sameNumeric(const EventNativePlatform &a, const EventNativePlatform &b) {
  return a.user == b.user && a.irand == b.irand && a.truncate == b.truncate &&
         a.divideWord == b.divideWord && a.addFloat == b.addFloat &&
         a.multiplyFloat == b.multiplyFloat && a.fmaFloat == b.fmaFloat &&
         a.subtractDouble == b.subtractDouble &&
         a.roundDoubleFloat == b.roundDoubleFloat;
}
bool numericBound(const EventNativePlatform &p) {
  return p.irand && p.truncate && p.divideWord && p.addFloat &&
         p.multiplyFloat && p.fmaFloat && p.subtractDouble &&
         p.roundDoubleFloat;
}
struct Bank {
  const char *file;
  const char *path;
  uint32_t size, count;
  uint64_t hash;
};
constexpr Bank Banks[] = {
    {"audience-normal.tpl", "./battle/audience/audience_normal.tpl", 428896,
     205, 0xbf4b14f59935fc7aULL},
    {"audience-luigi.tpl", "./battle/audience/audience_luigi.tpl", 39776, 19,
     0x45a4cb0fa39c3317ULL}};
} // namespace
bool stageHarborAudienceFiles(NativeFileCache &files, const char *root) {
  if (!root)
    return false;
  std::vector<NativeFileCache::Source> source;
  for (const auto &bank : Banks) {
    if (files.contains(bank.path, FileTypeTpl))
      return false; // no adoption by name
    const auto name = std::string(root) + "/" + bank.file;
    FILE *f = std::fopen(name.c_str(), "rb");
    if (!f)
      return false;
    std::vector<uint8_t> raw(bank.size);
    bool ok = std::fread(raw.data(), 1, raw.size(), f) == raw.size() &&
              std::fgetc(f) == EOF && !std::ferror(f);
    std::fclose(f);
    uint64_t hash = 14695981039346656037ULL;
    for (auto b : raw) {
      hash ^= b;
      hash *= 1099511628211ULL;
    }
    const auto word = [&](unsigned at) {
      return uint32_t(raw[at]) << 24 | uint32_t(raw[at + 1]) << 16 |
             uint32_t(raw[at + 2]) << 8 | raw[at + 3];
    };
    if (!ok || hash != bank.hash || word(0) != 0x0020af30 ||
        word(4) != bank.count || word(8) != 12)
      return false;
    source.push_back({bank.path, FileTypeTpl, std::move(raw)});
  }
  // FNV is a trusted-pack corruption guard. Existing audience exporter pins
  // exact original SHA256; this is not a cryptographic authenticity claim.
  return files.stage(std::move(source));
}
struct HarborSourceAudience::State {
  HarborSourceAudienceBinding binding;
  EventNativePlatform platform;
  BattleAudienceSetupServices services;
  BattleAudienceInitCursor cursor;
  std::shared_ptr<const uint8_t> allocation;
  bool done = false, invalid = false;
  mutable bool checking = false;
  uint32_t helperPending = 0;
  struct Texture {
    BattleAudienceSetupRequest request;
    uint32_t audience = 0, root = 0, phase = 0;
    std::optional<uint32_t> entry;
    std::string guestFormat, guestName, privateName, format, rootName, name,
        path;
  };
  std::optional<Texture> texture;
  struct Target {
    unsigned phase = 0;
    uint32_t owner = 0, count = 0;
    float desired = 0, extra = 0, amount = 0;
    int32_t target = 0;
    BattleAudienceAddNumInvocation add;
  };
  std::optional<Target> target;
  std::optional<uint32_t> throwingOwner;
  bool current() {
    if (invalid || checking)
      return false;
    Guard guard(checking);
    uint32_t b = 0;
    if (!binding.current() || !binding.pool->current() ||
        binding.pool->allocationLifetime(binding.battle) != allocation ||
        !binding.vm->memory.readWord(BattleGlobal, b) || b != binding.battle ||
        !sameNumeric(platform, binding.vm->platform) ||
        (binding.files && !binding.files->aliases(binding.vm->memory))) {
      invalid = true;
      return false;
    }
    return true;
  }
  uint32_t word(uint32_t a) const {
    uint32_t v = 0;
    const bool ok = binding.vm->memory.readWord(a, v);
    if (!ok)
      std::abort();
    return v;
  }
  uint16_t half(uint32_t a) const {
    uint16_t v = 0;
    const bool ok = binding.vm->memory.readHalf(a, v);
    if (!ok)
      std::abort();
    return v;
  }
  uint8_t byte(uint32_t a) const {
    uint8_t v = 0;
    const bool ok = binding.vm->memory.readByte(a, v);
    if (!ok)
      std::abort();
    return v;
  }
  void store(uint32_t a, uint32_t v) const {
    if (!binding.vm->memory.writeWord(a, v))
      std::abort();
  }
  uint32_t base() const { return word(BattleGlobal) + AudienceOffset; }
  uint32_t actor(uint32_t i) const {
    return base() + ActorOffset + i * ActorStride;
  }
  bool readable(uint32_t a, uint32_t n) const {
    if (!n || uint64_t(a) + n > 0x100000000ULL)
      return false;
    for (uint32_t i = 0; i < n; ++i) {
      uint8_t b;
      if (!binding.vm->memory.readByte(a + i, b))
        return false;
    }
    return true;
  }
  bool preflight() {
    if (!current() ||
        !binding.vm->memory.contiguousRam(binding.battle, BattleWorkBytes))
      return false;
    uint32_t pouch = 0, info = 0, setup = 0;
    if (!binding.vm->memory.readWord(PouchGlobal, pouch) ||
        !readable(pouch + 0x84, 12) ||
        !binding.vm->memory.readWord(binding.battle + 0x2738, info) ||
        !binding.vm->memory.readWord(info + 12, setup) ||
        !readable(setup + 0xac, 20))
      return false;
    constexpr std::array<std::pair<uint32_t, uint32_t>, 6> spans{
        {{0x802f9eac, 260},
         {0x802f9fb0, 44},
         {0x802fa328, 8},
         {0x803b4de4, 384},
         {0x8039cd88, 52},
         {0x80424988, 0xf4}}};
    for (auto span : spans)
      for (uint32_t i = 0; i < span.second; ++i) {
        uint8_t original = 0, actual = 0;
        if (!binding.sourceData->readByte(span.first + i, original) ||
            !binding.vm->memory.readByte(span.first + i, actual))
          return false;
        if (original != actual) {
          invalid = true;
          return false;
        }
      }
    return current();
  }
  bool text(uint32_t at, std::string &captured) {
    for (size_t i = 0; i <= captured.size(); ++i) {
      if (i >= PathCapacity || uint64_t(at) + i > 0xffffffffULL)
        return false;
      uint8_t expected = 0, actual = 0;
      helperPending = at + uint32_t(i);
      if (!binding.sourceData->readByte(at + uint32_t(i), expected) ||
          !binding.vm->memory.readByte(at + uint32_t(i), actual))
        return false;
      if (expected != actual) {
        invalid = true;
        return false;
      }
      if (i < captured.size()) {
        if (actual != uint8_t(captured[i])) {
          invalid = true;
          return false;
        }
        continue;
      }
      if (!actual)
        return true;
      captured.push_back(char(actual));
    }
    return false;
  }
  std::optional<BattleAudienceSetupOutput>
  tpl(const BattleAudienceSetupRequest &q) {
    if (!texture) {
      Texture t;
      t.request = q;
      if (q.function == 0x801a6050) {
        t.audience = base();
        if (integer(q.arguments[0]) >= 2)
          return BattleAudienceSetupOutput{};
      } else
        t.phase = 1;
      texture = std::move(t);
    }
    auto &t = *texture;
    if (t.request.function != q.function || t.request.callsite != q.callsite ||
        t.request.arguments != q.arguments)
      return {};
    if (t.phase == 0) {
      if (!text(0x802fa6fc, t.guestFormat) ||
          !text(q.arguments[2], t.guestName))
        return {};
      if (t.guestFormat != "battle/audience/%s")
        return {};
      t.privateName = "battle/audience/" + t.guestName;
      if (t.privateName.size() >= PathCapacity)
        return {};
      t.phase = 1;
    }
    if (t.phase == 1) {
      t.root = sourceMarioDvdRoot(OriginalMarioDvdSourceR2);
      t.phase = 2;
    }
    if (t.phase == 2) {
      if (!text(0x80424a88, t.format) || !text(t.root, t.rootName))
        return {};
      if (q.function == 0x801a6050)
        t.name = t.privateName;
      else if (!text(q.arguments[0], t.name))
        return {};
      if (t.format != "%s/%s")
        return {};
      t.path = t.rootName + "/" + t.name;
      t.phase = 3;
    }
    // Immutable inputs are revalidated at the held native boundary. Cached
    // path text cannot revive a changed or retired original source mapping.
    if ((q.function == 0x801a6050 && (!text(0x802fa6fc, t.guestFormat) ||
                                      !text(q.arguments[2], t.guestName))) ||
        !text(0x80424a88, t.format) || !text(t.root, t.rootName) ||
        (q.function == 0x801a86ac && !text(q.arguments[0], t.name)))
      return {};
    if (t.phase == 3) {
      helperPending = 0x8018a61c;
      if (!binding.files || !binding.files->contains(t.path, FileTypeTpl) ||
          !current())
        return {};
      const auto acquired = binding.files->acquire(t.path, FileTypeTpl);
      if (!current() || !acquired)
        return {};
      t.entry = *acquired;
      t.phase = 4;
    }
    const auto entry = t.entry;
    if (q.function == 0x801a6050) {
      const auto index = q.arguments[0];
      const uint32_t paletteField = t.audience + 0x10 + index * 4,
                     countField = t.audience + 0x18 + index;
      if (!readable(paletteField, 4) || !readable(countField, 1))
        return {};
      store(paletteField, *entry);
      if (!binding.vm->memory.writeByte(countField, uint8_t(q.arguments[1])))
        std::abort();
    }
    BattleAudienceSetupOutput result;
    result.value = *entry;
    texture.reset();
    helperPending = 0;
    return result;
  }
  std::optional<BattleAudienceSetupOutput> numToTarget() {
    if (!target) {
      Target t;
      t.owner = base();
      if (word(t.owner) & 0x40000)
        return BattleAudienceSetupOutput{};
      t.desired = single(word(t.owner + DesiredCountOffset));
      t.extra = single(word(t.owner + ExtraCountOffset));
      target = t;
    }
    auto &t = *target;
    if (t.phase == 0) {
      helperPending = 0x801a1c8c;
      t.amount = platform.addFloat(platform.user, t.desired, t.extra);
      if (!current())
        return {};
      t.phase = 1;
    }
    if (t.phase == 1) {
      helperPending = 0x801a1c90;
      if (!std::isfinite(t.amount) || t.amount < -2147483648.f ||
          t.amount >= 2147483648.f)
        return {};
      t.target = platform.truncate(platform.user, t.amount);
      if (!current())
        return {};
      if (t.target < 0)
        t.target = 0;
      const auto capacity = integer(word(t.owner + CapacityOffset));
      if (t.target > capacity)
        t.target = capacity;
      t.phase = 2;
    }
    if (t.phase == 2) {
      (void)
          base(); // Whole GetAudienceNumSub801a1bf4 begins with discarded Base.
      for (uint32_t i = 0; i < SourceAudienceSlots; ++i) {
        (void)base();
        const auto flags = word(actor(i)); // Whole GetExist: two getters.
        if ((flags & 1) && !(flags & 0x102) && byte(actor(i) + 0x19) != 12)
          ++t.count;
      }
      t.add.amount = integer(uint32_t(t.target) - t.count);
      t.add.battle = word(BattleGlobal);
      t.add.audience = base();
      t.add.pouch = word(PouchGlobal);
      t.add.capacity = half(t.add.pouch + 0x8c);
      if (!t.add.capacity || !t.add.amount ||
          (word(t.add.battle + 0xef4) & 0x100)) {
        target.reset();
        helperPending = 0;
        return BattleAudienceSetupOutput{};
      }
      t.phase = 3;
    }
    helperPending = 0x801a176c;
    if (!binding.addAudience)
      return {};
    const auto completed = binding.addAudience(t.add);
    if (!current() || !completed)
      return {};
    target.reset();
    helperPending = 0;
    return BattleAudienceSetupOutput{};
  }
  std::optional<uint32_t> number(const BattleAudienceSetupNumberRequest &q) {
    if (!current())
      return {};
    const auto a = q.operands[0], b = q.operands[1], c = q.operands[2];
    uint32_t result = 0;
    switch (q.operation) {
    case BattleAudienceSetupNumeric::SubtractDoubleToSingle:
      result = bits(platform.roundDoubleFloat(
          platform.user,
          platform.subtractDouble(platform.user, wide(a), wide(b))));
      break;
    case BattleAudienceSetupNumeric::AddSingle:
      result = bits(platform.addFloat(platform.user, single(uint32_t(a)),
                                      single(uint32_t(b))));
      break;
    case BattleAudienceSetupNumeric::MultiplySingle:
      result = bits(platform.multiplyFloat(platform.user, single(uint32_t(a)),
                                           single(uint32_t(b))));
      break;
    case BattleAudienceSetupNumeric::FmaSingle:
      result =
          bits(platform.fmaFloat(platform.user, single(uint32_t(a)),
                                 single(uint32_t(b)), single(uint32_t(c))));
      break;
    case BattleAudienceSetupNumeric::TruncateSingle: {
      const auto f = single(uint32_t(a));
      if (!std::isfinite(f) || f < -2147483648.f || f >= 2147483648.f)
        return {};
      result = uint32_t(platform.truncate(platform.user, f));
      break;
    }
    case BattleAudienceSetupNumeric::DivideSigned: {
      const auto n = integer(uint32_t(a)), d = integer(uint32_t(b));
      if (!d || (n == INT32_MIN && d == -1))
        return {};
      result = uint32_t(platform.divideWord(platform.user, n, d));
      break;
    }
    }
    return current() ? std::optional<uint32_t>(result) : std::nullopt;
  }
  std::optional<BattleAudienceSetupOutput>
  call(const BattleAudienceSetupRequest &q) {
    if (!current())
      return {};
    BattleAudienceSetupOutput out;
    switch (q.function) {
    case 0x80005da0:
      out.value =
          uint32_t(platform.irand(platform.user, integer(q.arguments[0])));
      break;
    case 0x800d3bac:
      out.singleBits = word(word(PouchGlobal) + 0x84);
      break;
    case 0x8000519c: {
      if (!q.privateCopy ||
          q.arguments[0] != binding.battle + AudienceOffset + PrivateLayoutOffset ||
          q.arguments[2] != PrivateLayoutBytes || q.privateCopy->destination != q.arguments[0])
        return {};
      for (auto b : q.privateCopy->bytes)
        if (!b)
          return {};
      bool copied = binding.vm->memory.withRawRamAccess([&](auto &ram) {
        if (!ram.bind(q.arguments[0], PrivateLayoutBytes) ||
            !ram.disjointStorage(&q.privateCopy->bytes,
                                 sizeof(q.privateCopy->bytes)))
          return false;
        // Native atomic nonalias raw-RAM policy: no callbacks/observers can see
        // direction-dependent byte prefixes, and VM mapping changes are locked.
        for (unsigned i = 0; i < PrivateLayoutBytes; ++i)
          if (!ram.writeByte(q.arguments[0] + i, *q.privateCopy->bytes[i]))
            std::abort();
        return true;
      });
      if (!copied)
        return {};
      out.value = q.arguments[0];
      break;
    }
    case 0x801a86ac:
    case 0x801a6050:
      return tpl(q);
    case 0x801a5a0c: {
      if (!throwingOwner)
        throwingOwner = base();
      const auto draw = platform.irand(platform.user, 100);
      if (!current())
        return {};
      const auto a = *throwingOwner;
      if (draw < 80)
        store(a + 0x137f4, 0);
      else if (draw < 90)
        store(a + 0x137f4, 1);
      else if (draw < 95)
        store(a + 0x137f4, 2);
      else if (draw < 100)
        store(a + 0x137f4, 3);
      store(a + 0x137f0, 0);
      store(a + 0x137f8, 0);
      throwingOwner.reset();
      break;
    }
    case 0x8019fe28:
      setBattleAudienceVolume(integer(q.arguments[0]), integer(q.arguments[1]),
                              integer(q.arguments[2]), services.actors);
      break;
    case 0x801a1c60:
      return numToTarget();
    default:
      if (!binding.child)
        return {};
      return binding.child(q);
    }
    return current() ? std::optional<BattleAudienceSetupOutput>(out)
                     : std::nullopt;
  }
};
HarborSourceAudience::HarborSourceAudience() = default;
HarborSourceAudience::~HarborSourceAudience() = default;
std::unique_ptr<HarborSourceAudience>
HarborSourceAudience::create(HarborSourceAudienceBinding b) {
  if (!b.vm || !b.pool || !b.battle || !b.lifetime || !b.current ||
      !b.sourceData || !numericBound(b.vm->platform))
    return {};
  auto owner = std::unique_ptr<HarborSourceAudience>(new HarborSourceAudience);
  owner->state_ = std::make_unique<State>();
  auto &s = *owner->state_;
  s.binding = std::move(b);
  s.platform = s.binding.vm->platform;
  s.allocation = s.binding.pool->allocationLifetime(s.binding.battle);
  if (!s.allocation || !s.current())
    return {};
  s.services.actors = nativeBattleActorServices(*s.binding.vm);
  s.services.lifetime = s.binding.lifetime;
  s.services.user = &s;
  s.services.current = [](void *p) {
    return static_cast<State *>(p)->current();
  };
  s.services.call = [](void *p, const BattleAudienceSetupRequest &q) {
    return static_cast<State *>(p)->call(q);
  };
  s.services.number = [](void *p, const BattleAudienceSetupNumberRequest &q) {
    return static_cast<State *>(p)->number(q);
  };
  return owner;
}
bool HarborSourceAudience::resume() {
  if (invoking_ || !state_)
    return false;
  Guard guard(invoking_);
  auto &s = *state_;
  if (!s.current())
    return false;
  if (s.done)
    return true;
  if (!s.preflight())
    return false;
  if (!resumePackedBattleAudienceInit(s.services, s.cursor))
    return false;
  s.done = true;
  return s.current();
}
bool HarborSourceAudience::complete() const {
  return state_ && state_->current() && state_->done;
}
bool HarborSourceAudience::current() const {
  return state_ && state_->current();
}
uint32_t HarborSourceAudience::pending() const {
  return state_ ? (state_->helperPending ? state_->helperPending
                                         : state_->cursor.pending())
                : 0;
}
bool HarborSourceAudience::retire() {
  if (invoking_ || !state_ || state_->checking)
    return false;
  state_->invalid = true;
  return true;
}
} // namespace ttyd

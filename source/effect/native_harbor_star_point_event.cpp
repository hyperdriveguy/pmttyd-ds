#include "event_manager.hpp"
#include "native_harbor_star_point.hpp"
#include <cstring>
namespace ttyd {
namespace {
struct Guard {
  bool &v;
  explicit Guard(bool &b) : v(b) { v = true; }
  ~Guard() { v = false; }
};
float real(uint32_t v) {
  float f;
  std::memcpy(&f, &v, 4);
  return f;
}
uint32_t bits(float f) {
  uint32_t v;
  std::memcpy(&v, &f, 4);
  return v;
}
bool same(const NativeHarborStarPointServices &a,
          const NativeHarborStarPointServices &b) {
  return a.user == b.user && a.lifetime == b.lifetime &&
         !a.lifetime.owner_before(b.lifetime) &&
         !b.lifetime.owner_before(a.lifetime) && a.current == b.current &&
         a.word == b.word && a.value == b.value && a.floating == b.floating &&
         a.byte == b.byte && a.spawn == b.spawn;
}
} // namespace
bool resumeNativeHarborStarPoint(EventEntry &e,
                                 const NativeHarborStarPointServices &s,
                                 NativeHarborStarPointCursor &c) {
  if (c.invoking_ || !s.lifetime || !s.current || !(e.flags & 1) ||
      e.argumentCount < 14)
    return false;
  Guard guard(c.invoking_);
  if (c.event_ &&
      (c.event_ != &e || c.id_ != e.id || c.command_ != e.currentCommand ||
       c.args_ != e.arguments || !same(s, c.services_)))
    return false;
  if (!s.current(s.user))
    return false;
  if (!c.event_) {
    c.event_ = &e;
    c.id_ = e.id;
    c.command_ = e.currentCommand;
    c.args_ = e.arguments;
    c.services_ = s;
  }
  if (c.event_ != &e || c.id_ != e.id || c.command_ != e.currentCommand ||
      c.args_ != e.arguments || !same(s, c.services_))
    return false;
  if (c.complete_)
    return true;
  if (!c.prepared_) {
    size_t read = 0;
    auto load = [&](uint32_t kind, uint32_t address, uint32_t &out) {
      if (read < c.reads_.size()) {
        const auto &r = c.reads_[read++];
        if (r.kind != kind || r.address != address)
          return false;
        out = r.value;
        return true;
      }
      c.pending_ = kind == 0   ? address
                   : kind == 1 ? 0x80031260
                   : kind == 2 ? 0x80030840
                               : address;
      if (!s.current(s.user))
        return false;
      std::optional<uint32_t> v;
      if (kind == 0 && s.word)
        v = s.word(s.user, address);
      else if (kind == 1 && s.value) {
        auto q = s.value(s.user, e, int32_t(address));
        if (q)
          v = uint32_t(*q);
      } else if (kind == 2 && s.floating) {
        auto q = s.floating(s.user, e, int32_t(address));
        if (q)
          v = bits(*q);
      } else if (kind == 3 && s.byte) {
        auto q = s.byte(s.user, address);
        if (q)
          v = *q;
      }
      if (!v || !s.current(s.user))
        return false;
      c.reads_.push_back({kind, address, *v});
      ++read;
      out = *v;
      return true;
    };
    auto argument = [&](uint32_t index, uint32_t kind, uint32_t &out) {
      uint32_t raw;
      return load(0, c.args_ + 4 * index, raw) && load(kind, raw, out);
    };
    std::array<uint32_t, 15> integers{}, floating{};
    // Original getter calls are repeated, not deduplicated. Index14 reads the
    // next mapped script word even when this command declared14 operands.
    for (unsigned n = 0; n < 2; ++n)
      for (unsigned k = 0; k < 3; ++k) {
        uint32_t captured;
        if (!argument(n, 1, captured))
          return false;
        if (!k)
          integers[n] = captured;
      }
    for (unsigned n = 2; n < 15; ++n)
      if (!argument(n, 1, integers[n]))
        return false;
    for (unsigned n = 2; n < 15; ++n)
      if (!argument(n, 2, floating[n]))
        return false;
    c.pending_ = 0x8005e8a0;
    constexpr char SupportedName[] = "star_point";
    for (unsigned n = 0; n < sizeof(SupportedName); ++n) {
      uint32_t ch;
      if (!load(3, integers[1] + n, ch) || ch != uint8_t(SupportedName[n]))
        return false;
    }
    // Untyped naming writes require an actual guest entry, never native IDs.
    if (integers[2] != 0)
      return false;
    for (unsigned n = 0; n < 3; ++n)
      c.request_.position[n] = real(floating[n + 3]);
    c.request_.subtype = int32_t(integers[2]);
    c.request_.amount = int32_t(integers[6]);
    c.name_ = integers[0];
    c.prepared_ = true;
  }
  c.pending_ = 0x8020b0f4;
  if (!s.current(s.user) || !s.spawn)
    return false;
  // Native admission preflights the original tail empty-name predicate BEFORE
  // publishing an instance. Original source evaluates this after its guest
  // constructor. This finite adaptation never supplies a guest naming pointer.
  if (c.name_) {
    c.pending_ = 0x8026d464;
    constexpr uint32_t OriginalEmptyLiteral = 0x8042033c;
    size_t tailRead = 0;
    auto readByte = [&](uint32_t address) -> std::optional<uint8_t> {
      size_t seen = 0;
      for (const auto &q : c.reads_)
        if (q.kind == 4 && seen++ == tailRead) {
          ++tailRead;
          if (q.address != address)
            return {};
          return uint8_t(q.value);
        }
      if (!s.byte || !s.current(s.user))
        return {};
      auto v = s.byte(s.user, address);
      if (!v)
        return {};
      c.reads_.push_back({4, address, *v});
      ++tailRead;
      return v;
    };
    auto first = readByte(c.name_);
    if (!first)
      return false;
    auto second = readByte(OriginalEmptyLiteral);
    if (!second)
      return false;
    if (*first || *second)
      return false;
  }
  if (!c.spawned_) {
    auto result = s.spawn(s.user, c.request_);
    if (!result || !*result)
      return false;
    c.spawned_ = true;
  }
  if (!s.current(s.user))
    return false;
  c.complete_ = true;
  c.pending_ = 0;
  return true;
}

} // namespace ttyd

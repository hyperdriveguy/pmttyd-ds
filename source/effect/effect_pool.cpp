#include "effect_pool.hpp"

namespace ttyd {
namespace {
// Original global tokens:manager15ee0,GP15fd0,prefix table15ee4,DVD root
//1f494,format2c1f20,N64 state-pointer419a40. Manager count0/array4,cache10/
// selector14;record stride40 decimal(28hex),flags0/context4/payloadc/callback10.
constexpr uint32_t Manager = 0x80415ee0, GP = 0x80415fd0;
constexpr uint32_t Prefixes = 0x80415ee4, Root = 0x8041f494,
                   Format = 0x802c1f20, State64 = 0x80419a40;
bool positive(uint32_t n) { return n != 0 && !(n & 0x80000000u); }
bool same(const EventMemoryServices &a, const EventMemoryServices &b) {
  return a.user == b.user && a.word == b.word &&
         a.storeWord == b.storeWord && a.storeByte == b.storeByte;
}
// Validate memory primitive/user identity on resume;external callback stability
// is caller-owned. No raw read/store failure channel or rollback is invented.
bool begin(const EffectPoolServices &s, EffectPoolCursorOwner &c) {
  if (!s.memory.word || !s.memory.storeWord)
    return false;
  if (c.active)
    return same(c.memory, s.memory) && c.serviceOwner == s.user;
  c.active = true;
  c.memory = s.memory;
  c.serviceOwner = s.user;
  return true;
}
} // namespace
// Ascending EXACT-zero search. Signed nonpositive count writes base;full pool
// writes one-past source record. Valid mapped storage/capacity is provider policy.
// Stamp normalized GP context AFTER flags1;clear only14/8/c/10/byte18,not row.
std::optional<uint32_t> effectPoolEntry(const EffectPoolServices &s) {
  const auto &m = s.memory;
  if (!m.word || !m.storeWord || !m.storeByte)
    return {};
  auto get = [&](uint32_t a) { return m.word(m.user, a); };
  auto put = [&](uint32_t a, uint32_t v) { m.storeWord(m.user, a, v); };
  const auto manager = get(Manager);
  const auto count = get(manager);
  auto record = get(manager + 4);
  if (positive(count))
    for (uint32_t index = 0; index < count; ++index, record += 40)
      if (get(record) == 0)
        break;
  put(record, 1);
  const auto gp = get(GP);
  put(record + 4, get(gp + 0x14) != 0 ? 1 : 0);
  put(record + 0x14, 0);
  put(record + 8, 0);
  put(record + 0xc, 0);
  put(record + 0x10, 0);
  m.storeByte(m.user, record + 0x18, 0);
  return record;
}
// Capture signed count/array once. Each reached slot freshly reads GP context,
// exact record0/1 context,active1,callback. A later-slot allocation may run in
// this same scan;earlier-slot allocation waits. Callback return is ignored.
bool resumeEffectMain(const EffectPoolServices &s, EffectMainCursor &c) {
  if (!begin(s, c))
    return false;
  const auto &m = s.memory;
  auto get = [&](uint32_t a) { return m.word(m.user, a); };
  auto put = [&](uint32_t a, uint32_t v) { m.storeWord(m.user, a, v); };
  auto next = [&] { ++c.index; c.record += 40; c.phase = 1; };
  auto finish = [&] { c = {}; return true; };
  for (;;)
    switch (c.phase) {
    case 0: {
      const auto manager = get(Manager);
      c.count = get(manager);
      c.record = get(manager + 4);
      c.phase = 1;
      break;
    }
    case 1:
      if (!positive(c.count) || c.index >= c.count) {
        c.phase = 3;
        break;
      }
      {
        const auto gp = get(GP);
        const auto context = get(gp + 0x14) != 0 ? 1u : 0u;
        if (get(c.record + 4) != context || !(get(c.record) & 1u)) {
          next();
          break;
        }
        c.callback = get(c.record + 0x10);
        if (!c.callback) {
          next();
          break;
        }
        c.phase = 2;
      }
      break;
    case 2:
      if (!s.update || !s.update(s.user, c.callback, c.record))
        return false;
      next();
      break;
    // Cache tail uses fresh manager/GP after traversal. Free old handle first,
    // then clear CURRENT manager cache. Async and sync independently reload
    // selector/prefix;wrapping selector<<2 is not a checked language enum.
    case 3: {
      const auto manager = get(Manager);
      const auto gp = get(GP);
      const auto cached = get(manager + 0x14);
      if (cached == get(gp + 0x16c))
        return finish();
      c.handle = get(manager + 0x10);
      c.phase = c.handle ? 4 : 5;
      break;
    }
    case 4:
      if (!s.fileFree || !s.fileFree(s.user, c.handle))
        return false;
      put(get(Manager) + 0x10, 0);
      c.phase = 5;
      break;
    case 5: {
      const auto gp = get(GP);
      const auto selector = get(gp + 0x16c);
      c.name = get(Prefixes + (selector << 2));
      c.phase = 6;
      break;
    }
    // fileAsync type4:NULLopt is unavailable;executed0 finishes without tag.
    // ANY nonzero(including-1) proceeds to sync allocate. Sync handle0 is also
    // an executed result and publishes before fresh manager/GP selector tag.
    case 6: {
      if (!s.fileAsync)
        return false;
      auto result = s.fileAsync(s.user, 4, 0, Format, Root, c.name, c.name);
      if (!result)
        return false;
      if (*result == 0)
        return finish();
      c.phase = 7;
      break;
    }
    case 7: {
      const auto gp = get(GP);
      const auto selector = get(gp + 0x16c);
      c.name = get(Prefixes + (selector << 2));
      c.phase = 8;
      break;
    }
    case 8: {
      if (!s.fileAllocate)
        return false;
      auto result = s.fileAllocate(s.user, 4, Format, Root, c.name, c.name);
      if (!result)
        return false;
      put(get(Manager) + 0x10, *result);
      const auto gp = get(GP);
      const auto manager = get(Manager);
      put(manager + 0x14, get(gp + 0x16c));
      return finish();
    }
    default:
      return false;
    }
}
// Publish count512,heap0 allocate count*40,then recapture manager/count/base
// for clear. Reset cache10=0/selector14=ffffffff and unresolved words8/c=0.
// N64 state clear8 captures pointer before child;final +4 zero reloads pointer.
// No allocation-null repair or implicit per-effect initialization added.
bool resumeEffectInit(const EffectPoolServices &s, EffectInitCursor &c) {
  if (!begin(s, c))
    return false;
  const auto &m = s.memory;
  auto get = [&](uint32_t a) { return m.word(m.user, a); };
  auto put = [&](uint32_t a, uint32_t v) { m.storeWord(m.user, a, v); };
  for (;;)
    switch (c.phase) {
    case 0:
      put(get(Manager), 512);
      c.bytes = get(get(Manager)) * 40u;
      c.phase = 1;
      break;
    case 1: {
      if (!s.heapAllocate)
        return false;
      auto result = s.heapAllocate(s.user, 0, c.bytes);
      if (!result)
        return false;
      put(get(Manager) + 4, *result);
      const auto manager = get(Manager);
      c.bytes = get(manager) * 40u;
      c.address = get(manager + 4);
      c.phase = 2;
      break;
    }
    case 2:
      if (!s.clear || !s.clear(s.user, c.address, c.bytes))
        return false;
      put(get(Manager) + 0x10, 0);
      put(get(Manager) + 0x14, 0xffffffffu);
      put(get(Manager) + 8, 0);
      put(get(Manager) + 0xc, 0);
      c.address = get(State64);
      c.phase = 3;
      break;
    case 3:
      if (!s.clear || !s.clear(s.user, c.address, 8))
        return false;
      put(get(State64) + 4, 0);
      c = {};
      return true;
    default:
      return false;
    }
}
// Raw context word is exact,not normalized. Capture signed count/array once;
// active1/context-match frees payload on heap3 then clears ONLY record flags.
// No effect callback,pose/map/audio cleanup,queue cancellation or cache free:
// those remain separate owners,not an inferred per-effect destructor.
bool resumeEffectAutoRelease(uint32_t context, const EffectPoolServices &s,
                             EffectReleaseCursor &c) {
  if (c.active && c.context != context)
    return false;
  if (!begin(s, c))
    return false;
  const auto &m = s.memory;
  auto get = [&](uint32_t a) { return m.word(m.user, a); };
  auto next = [&] { ++c.index; c.record += 40; c.phase = 1; };
  for (;;)
    switch (c.phase) {
    case 0: {
      c.context = context;
      const auto manager = get(Manager);
      c.count = get(manager);
      c.record = get(manager + 4);
      c.phase = 1;
      break;
    }
    case 1:
      if (!positive(c.count) || c.index >= c.count) {
        c = {};
        return true;
      }
      {
        const auto flags = get(c.record);
        if (!(flags & 1) || get(c.record + 4) != c.context || !c.record ||
            !flags) {
          next();
          break;
        }
        c.payload = get(c.record + 0xc);
        c.phase = 2;
      }
      break;
    case 2:
      if (!s.heapFree || !s.heapFree(s.user, 3, c.payload))
        return false;
      m.storeWord(m.user, c.record, 0);
      next();
      break;
    default:
      return false;
    }
}
} // namespace ttyd

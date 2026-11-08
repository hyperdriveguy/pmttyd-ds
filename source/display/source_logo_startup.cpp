#include "source_logo_startup.hpp"
namespace ttyd {
namespace {
constexpr uint32_t WorkGlobal = 0x80416108, GpGlobal = 0x80415fd0,
                   Memset = 0x800050b4, DvdRoot = 0x80007388,
                   FileAllocate = 0x8018a61c, Format = 0x802c3b68,
                   ResetCode = 0x8029b544, Dtv = 0x802a5088,
                   ProgressiveGet = 0x8029c250, ProgressiveSet = 0x8029c2c0,
                   Buttons = 0x80005fd0, Configure = 0x802a4420,
                   Flush = 0x802a4c28, Retrace = 0x802a3f58,
                   InterlacedMode = 0x8030c230, ProgressiveMode = 0x8030c26c;
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
bool read(const BattleSequenceEntryServices &s, SourceLogoStartupCursor &c,
          uint32_t address, uint32_t &out, uint32_t site) {
  c.pending = site;
  const auto v = s.word ? s.word(s.user, address) : std::nullopt;
  if (!v)
    return false;
  out = *v;
  return true;
}
bool store(const BattleSequenceEntryServices &s, SourceLogoStartupCursor &c,
           uint32_t address, uint32_t value, uint32_t site) {
  c.pending = site;
  return s.store && s.store(s.user, address, value);
}
bool call(const BattleSequenceEntryServices &s, SourceLogoStartupCursor &c,
          uint32_t function, std::array<uint32_t, 5> args = {}) {
  c.pending = function;
  if (!c.queued) {
    c.call = {};
    c.call.function = function;
    c.call.args = args;
    c.queued = true;
  }
  if (!s.call || !s.call(s.user, c.call))
    return false;
  c.value = c.call.result;
  c.queued = false;
  return true;
}
bool finish(SourceLogoStartupCursor &c) {
  c.active = false;
  c.complete = true;
  c.pending = 0;
  return true;
}
} // namespace
bool initializeSourceLogo(const BattleSequenceEntryServices &s,
                          SourceLogoStartupCursor &c) {
  if (c.invoking || (c.entry && c.entry != 0x800874b0))
    return false;
  Guard guard(c.invoking);
  c.entry = 0x800874b0;
  if (c.complete)
    return true;
  c.active = true;
  for (;;)
    switch (c.phase) {
    case 0:
      if (!read(s, c, WorkGlobal, c.work, 0x800874c4))
        return false;
      ++c.phase;
      break;
    case 1:
      if (!call(s, c, Memset, {c.work, 0, 0x20}))
        return false;
      ++c.phase;
      break;
    case 2:
      if (!read(s, c, WorkGlobal, c.work, 0x800874cc))
        return false;
      ++c.phase;
      break;
    case 3:
      if (!store(s, c, c.work + 4, 0xffffffff, 0x800874d4))
        return false;
      ++c.phase;
      break;
    case 4:
      if (!call(s, c, DvdRoot))
        return false;
      c.root = c.value;
      ++c.phase;
      break;
    case 5:
      if (!call(s, c, FileAllocate, {4, Format, c.root}))
        return false;
      c.handle = c.value;
      ++c.phase;
      break;
    case 6:
      if (!read(s, c, WorkGlobal, c.work, 0x800874f4))
        return false;
      ++c.phase;
      break;
    case 7:
      if (!store(s, c, c.work, c.handle, 0x800874f8))
        return false;
      ++c.phase;
      break;
    case 8:
      if (!call(s, c, ResetCode))
        return false;
      c.phase = (c.value & 0x80000000) ? 9 : 10;
      break;
    case 9:
      if (!call(s, c, ResetCode))
        return false;
      c.phase = (c.value & 2) ? 10 : 20;
      break;
    case 10:
      if (!call(s, c, Dtv))
        return false;
      c.phase = c.value ? 11 : 17;
      break;
    case 11:
      if (!call(s, c, ProgressiveGet))
        return false;
      if (c.value == 1) {
        c.workValue = 1;
        c.workReadSite = 0x8008752c;
        c.workStoreSite = 0x80087534;
        c.phase = 13;
      } else
        c.phase = 12;
      break;
    case 12:
      if (!call(s, c, Buttons, {0}))
        return false;
      c.workValue = (c.value & 0x200) ? 1 : 0;
      c.workReadSite = c.workValue ? 0x8008754c : 0x8008755c;
      c.workStoreSite = c.workValue ? 0x80087554 : 0x80087564;
      c.phase = 13;
      break;
    case 13:
      if (!read(s, c, WorkGlobal, c.work, c.workReadSite))
        return false;
      ++c.phase;
      break;
    case 14:
      if (!store(s, c, c.work + 0x18, c.workValue, c.workStoreSite))
        return false;
      c.phase = 20;
      break;
    case 17:
      if (!call(s, c, ProgressiveGet))
        return false;
      c.phase = c.value == 1 ? 18 : 19;
      break;
    case 18:
      if (!call(s, c, ProgressiveSet, {0}))
        return false;
      ++c.phase;
      break;
    case 19:
      if (!read(s, c, WorkGlobal, c.work, 0x80087580))
        return false;
      c.workValue = 0;
      c.workStoreSite = 0x80087588;
      c.phase = 14;
      break;
    case 20:
      if (!call(s, c, ResetCode))
        return false;
      if (c.value & 0x80000000)
        c.phase = 21;
      else {
        c.mode = InterlacedMode;
        c.terminalVideo = true;
        c.phase = 30;
      }
      break;
    case 21:
      if (!call(s, c, ResetCode))
        return false;
      if (c.value & 2) {
        c.mode = InterlacedMode;
        c.terminalVideo = true;
        c.phase = 30;
      } else
        c.phase = 22;
      break;
    case 22:
      if (!call(s, c, ResetCode))
        return false;
      c.extraRetrace = (c.value & 1) != 0;
      c.mode = c.extraRetrace ? ProgressiveMode : InterlacedMode;
      c.terminalVideo = false;
      c.phase = 30;
      break;
    case 30:
      if (!call(s, c, Configure, {c.mode}))
        return false;
      ++c.phase;
      break;
    case 31:
      if (!call(s, c, Flush))
        return false;
      ++c.phase;
      break;
    case 32:
      if (!call(s, c, Retrace))
        return false;
      if (c.terminalVideo)
        return finish(c);
      c.phase = c.extraRetrace ? 33 : 40;
      break;
    case 33:
      if (!call(s, c, Retrace))
        return false;
      ++c.phase;
      break;
    case 34:
      if (!read(s, c, GpGlobal, c.gp, 0x8008760c))
        return false;
      ++c.phase;
      break;
    case 35:
      if (!read(s, c, c.gp, c.value, 0x80087610))
        return false;
      ++c.phase;
      break;
    case 36:
      if (!store(s, c, c.gp, c.value | 8, 0x80087618))
        return false;
      c.phase = 40;
      break;
    case 40:
      if (!read(s, c, GpGlobal, c.gp, 0x8008761c))
        return false;
      ++c.phase;
      break;
    case 41:
      if (!read(s, c, c.gp, c.value, 0x80087620))
        return false;
      ++c.phase;
      break;
    case 42:
      if (!store(s, c, c.gp, c.value | 4, 0x80087628))
        return false;
      return finish(c);
    default:
      return false;
    }
}
bool exitSourceLogo(const BattleSequenceEntryServices &s,
                    SourceLogoStartupCursor &c) {
  if (c.invoking || (c.entry && c.entry != 0x8008746c))
    return false;
  Guard guard(c.invoking);
  c.entry = 0x8008746c;
  if (c.complete)
    return true;
  c.active = true;
  constexpr uint32_t Children[] = {0x80077bdc, 0x8007fbd8, 0x80013aa4,
                                   0x8001b694, 0x8019df84, 0x8005ee0c,
                                   0x8005ba50};
  for (;;) {
    if (c.phase < 7) {
      if (!call(s, c, Children[c.phase]))
        return false;
      ++c.phase;
      continue;
    }
    switch (c.phase) {
    case 7:
      if (!read(s, c, WorkGlobal, c.work, 0x80087494))
        return false;
      ++c.phase;
      break;
    case 8:
      if (!read(s, c, c.work, c.handle, 0x80087498))
        return false;
      ++c.phase;
      break;
    case 9:
      if (!call(s, c, 0x8018a260, {c.handle}))
        return false;
      return finish(c);
    default:
      return false;
    }
  }
}
} // namespace ttyd

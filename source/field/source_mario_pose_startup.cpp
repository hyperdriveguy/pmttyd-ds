#include "source_mario_pose_startup.hpp"
namespace ttyd {
namespace {
constexpr uint32_t MarioGlobal = 0x8041e900, ResourceTable = 0x80415eb0,
                   PoseEntry = 0x8004176c, KindOffset = 0x3d,
                   FirstHandle = 0x22c, ResourceStride = 12, PoseCount = 3;
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
} // namespace
bool initializeSourceMarioPoses(const BattleSequenceEntryServices &s,
                                SourceMarioPoseStartupCursor &c) {
  if (c.invoking)
    return false;
  Guard guard(c.invoking);
  if (c.complete)
    return true;
  c.active = true;
  for (;;)
    switch (c.phase) {
    case 0: {
      c.pending = 0x8005ba68;
      const auto v = s.word ? s.word(s.user, MarioGlobal) : std::nullopt;
      if (!v)
        return false;
      c.mario = *v;
      ++c.phase;
      break;
    }
    case 1:
      // The first selector uses captured r31. Later selectors reread mp, while
      // all three return stores still use that original r31.
      if (!c.index)
        c.selection = c.mario;
      else {
        c.pending = c.index == 1 ? 0x8005ba8c : 0x8005bab4;
        const auto v = s.word ? s.word(s.user, MarioGlobal) : std::nullopt;
        if (!v)
          return false;
        c.selection = *v;
      }
      ++c.phase;
      break;
    case 2: {
      c.pending = c.index == 0   ? 0x8005ba6c
                  : c.index == 1 ? 0x8005ba90
                                 : 0x8005bab8;
      const auto v =
          s.byte ? s.byte(s.user, c.selection + KindOffset) : std::nullopt;
      if (!v)
        return false;
      const int32_t kind = *v < 128 ? int32_t(*v) : int32_t(*v) - 256;
      c.table = ResourceTable + uint32_t(kind * int32_t(ResourceStride)) +
                4 * c.index;
      ++c.phase;
      break;
    }
    case 3: {
      c.pending = c.index == 0   ? 0x8005ba78
                  : c.index == 1 ? 0x8005baa0
                                 : 0x8005bac8;
      const auto v = s.word ? s.word(s.user, c.table) : std::nullopt;
      if (!v)
        return false;
      c.name = *v;
      ++c.phase;
      break;
    }
    case 4:
      c.pending = PoseEntry;
      if (!c.queued) {
        c.call = {};
        c.call.function = PoseEntry;
        c.call.args[0] = c.name;
        c.call.args[1] = 2;
        c.queued = true;
      }
      if (!s.call || !s.call(s.user, c.call))
        return false;
      c.result = c.call.result;
      c.queued = false;
      ++c.phase;
      break;
    case 5:
      c.pending = c.index == 0   ? 0x8005ba80
                  : c.index == 1 ? 0x8005baa8
                                 : 0x8005bad0;
      // Original stores every signed result, including failure; no rollback,
      // prior release or negative-handle guard is present in this wrapper.
      if (!s.store ||
          !s.store(s.user, c.mario + FirstHandle + 4 * c.index, c.result))
        return false;
      if (++c.index < PoseCount)
        c.phase = 1;
      else
        ++c.phase;
      break;
    case 6:
      c.pending = 0;
      c.active = false;
      c.complete = true;
      return true;
    default:
      return false;
    }
}
} // namespace ttyd

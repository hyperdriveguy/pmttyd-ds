#include "field_npc_main_history.hpp"
namespace ttyd {
// Unconditional current8c->previous98, then optional oldest-first five-vector
// shift c8->d4,bc->c8,b0->bc,a4->b0,current8c->a4. Stride12 is XYZ WORDS;
// +4/+8 are Y/Z. Preserve payloads and fresh scalar Z; no bulk vector snapshot.
bool advanceFieldNpcMainHistory(uint32_t actor, const FieldNpcClockServices &s,
                                 FieldNpcHistoryCursor &c) {
  if (!s.word || !s.store) return false;
  if (!c.active) { c = {}; c.active = true; c.owner = s; c.actor = actor; }
  if (c.actor != actor || c.owner.user != s.user || c.owner.word != s.word ||
      c.owner.store != s.store) return false;
  auto read = [&](uint32_t a, uint32_t &v) {
    const auto w = s.word(s.user, a); if (!w) return false; v = *w; return true;
  };
  // First copy always runs; subsequent copies run only for flag100/even count.
  static constexpr uint32_t from[]{0x8c,0xc8,0xbc,0xb0,0xa4,0x8c};
  static constexpr uint32_t to[]{0x98,0xd4,0xc8,0xbc,0xb0,0xa4};
  for (;;) switch (c.phase) {
  case 0:
    // Source reads X/Y, writes X/Y, then reads and writes Z. Never bulk copy:
    // successful earlier captures/stores survive a hold at any later primitive.
    switch (c.step) {
    case 0: if (!read(actor + from[c.copy], c.x)) return false; break;
    case 1: if (!read(actor + from[c.copy] + 4, c.y)) return false; break;
    case 2: if (!s.store(s.user, actor + to[c.copy], c.x)) return false; break;
    case 3: if (!s.store(s.user, actor + to[c.copy] + 4, c.y)) return false; break;
    case 4: if (!read(actor + from[c.copy] + 8, c.z)) return false; break;
    case 5: if (!s.store(s.user, actor + to[c.copy] + 8, c.z)) return false; break;
    default: return false;
    }
    if (++c.step == 6) {
      c.step = 0;
      if (c.copy == 0) c.phase = 1;
      else if (++c.copy == 6) c.phase = 3;
    }
    break;
  // Fresh main100 gates motion history; only then read live MainCount8041e8ac.
  // Odd skips the five-vector shift; even performs it. Final current X/Y are
  // reread, not reused from initial previous-position copy. No active/query test.
  case 1:
    if (!read(actor, c.flags)) return false;
    c.phase = c.flags & 0x100 ? 2 : 3;
    break;
  case 2:
    if (!read(0x8041e8ac, c.mainCount)) return false;
    if (c.mainCount & 1) c.phase = 3;
    else { c.copy = 1; c.phase = 0; }
    break;
  case 3: return true;
  default: return false;
  }
}
} // namespace ttyd

# Original enemy HP draw and native Tattle checker

US G8ME01 disc0 revision0; callback BattleDrawEnemyHP8011fde0/244,
checker battleCheckUnitMonosiriFlag8012ee3c/e4. Read-only full-body recovery:
artifacts/command-main-display-evidence/report.md and constants-and-bodies.json.
Active projective/texture recovery: artifacts/enemy-hp-active-evidence/report.md.
Static recovery and synthetic tests are distinct from original execution.

The callback captures its passed BattleWork, initializes local barRGBAffffffff
once, then unconditionally requests camera8 even when every slot is empty.
Walk64 registry slots at B20+4*i in order. Capture first U; null skips. Query
original status27 on U; nonzero skips. Reread that slot and call the
[Tattle checker](MONOSIRI_FLAG_GROUPS_AND_SWITCH_STORAGE.md) on the reloaded U.
Zero skips. Resume with first U: U104 mask02000000 skips; otherwise B1720
mask01000000 bypasses the U1c mask1 requirement. There is no global ending,
sequence, enemy-kind, belong or HP-sign gate. Camera registration proves none
of these per-slot conditions.

The checker captures GLOBAL B independently of passed callback B, then U.kind8.
Policy B163f4 mask2 returns1 without switch/flags/bank queries. Otherwise call
actual swGet(wrap32(4474+kind)), retain its result, then read fresh U1c mask4 and
retained B163d4 bank bit for captured kind. Mask4 additionally reads U.original4
and that bank bit, without another persistent switch request. Return normalized
0/1. Signed division/remainder and PPC slw are retained: negative remainders
use low6 shift bits, values32..63 yield0; native C++ never shifts by an invalid
count. The original has no authored kind clamp. Cursor holds retain captured B,
U, kind and successful switch result; no cached Tattle boolean substitutes them.
Native uses EventNativeRuntime's actual saved-switch operands and memory bank.

For an admitted slot, copy first U3c/40/44, then fresh Bef4 mask80 optionally
replaces XYZ with U30/34/38. Add signed16 Ud4/d6 to X/Y with separate float
stores. btlGetScreenPoint requests camera4, uses its original published3x4 view
then4x4 projective matrix, multiplies projected X/Y by304/240 and sets Z=+0.
Existing cameraDepthView/Project preserve the recovered paired-product/FMA/sum
boundaries and rounded reciprocal divide; no camera8/viewport/PICA transform
substitutes them. The numeric adapter does not establish Gekko FPSCR/NaN payload
or original framebuffer parity, and introduces no zero-W clamp.

After projection, fresh first U1c mask1, U16.1fe mask1 and signed16 U204<20
jointly select interpolation(mode0,elapsed,20,0,255), source truncate and low8
bar-alpha update. No lower elapsed clamp. This color carries to later slots
that skip fade. Reread registry U and signed16 HP10c to select numeric X offset
12 for HP>=100,16 for HP>=10,otherwise20. Build original translation matrix at
screenX+offset,screenY-32,+0. Reread registry AGAIN and signed16 HP for
iconNumberDispGx(matrix,HP,1,ffffffff). A held number retains its already-read
HP and matrix. Bar receives a fresh local copy of screenXYZ, first U and carried
barRGBA; original bar helper may mutate that local copy. Numeric white is
independent of carried bar alpha. Successful prefixes are not replayed on holds.

Native queue bridge validates the actual invoking callback/camera/userdata and
uses passed userdata as B. The portable callback and checker execute all gates
and admitted parent logic. Camera4 projection is bound to published native
matrices. Number and bar GPU providers remain unbound; an admitted slot holds at
80019120 or8011fc3c and cannot silently complete. Original bar uses live U210/214
gauge fields, not a currentHP-derived ratio. Textures17/18 are original40x16 CMPR
cells in battle/common/battle_common.tpl, accessed through GLOBAL B163f8 file
owner; existing class-menu atlas cells0/1 are not substitutes. These providers,
original numeric mode1 resources and complete field restoration remain required.

Host tests exercise global/passed B separation, switch waits and capture order,
policy bypass, negative kind shifts, all64 empty/status skips, registry mutation
between status/checker/width/number, held number without replayed projection,
carried alpha and actual missing-child holds. Synthetic services are explicit
fixtures; native validation and subsequent live frontier are recorded in
[progress](../../PROGRESS.md) and [EVT-01](../../requests/EVT-01.md).

# Portable original NPC wander core

2026-10-13; implements the recovered direct graph in
[original wander/Goomba4 evidence](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md).
This core is compiled for host and3DS. The separate
[live native binding](NATIVE_LIVE_NPC_WANDER.md) now activates original Goomba4
under a bounded zero-vertical profile; quest/event/ground providers remain
separate from this portable arithmetic.

`NpcWanderWork` retains the44-byte source phase/flags/countdown/flip/budget/
anchor/radius fields. Initialization uses caller-provided live frame conversion,
retains current NPCY and adds original mainflag00400000 without moving roots.
`tickNpcWander` preserves the four phases, distinct ordered readiness requests,
source multiple20 truncation, strict comparisons, component retention and
minimum20ms displacement cap. Phase3 changes yaw without changing movement
heading. Each invocation returns0, matching the ongoing source USER_FUNC;
allocation, event stopped/category state and ownership remain external.

Typed providers supply original horizontal distance/angle, readiness, sine/
cosine and sound resolution. The sound provider resolves the source-name token
and submits `(resolvedID|mask,root,255,255,0,8)`. Sound threshold/selection,
null-sound reset, signed byte toggle and conditional elapsed FMA remain in the
portable core. NPC-main must consume/clear displacement separately; this core
never advances rootXYZ or synthesizes pose changes.

`npcWanderFrames` retains wrapped32-bit multiplication followed by signed
truncation; its frame rate is supplied, not globally fixed60.
`npcWanderRandom` retains the original LCG and zero-bound no-consumption rule;
its seed is caller-owned shared state. No private seed is chosen and no original
random trajectory is claimed. `npcWanderTurnReady` reads poseflag8 and the
absolute retained/target difference, without angular wrapping or clip timing.
Angle normalization uses the existing source reviseAngle adapter. Distance/
libm/FPSCR/service interleaving and supported-platform integer conversion remain
explicit numeric/runtime dependencies.

Tests verify conditional60/50Hz budgets and wrapped multiplication, original
LCG words/results, RNG bound/order, multiple20 timing, phase3's retained heading,
count expiry while unready, distinct three/four readiness queries, mixed
readiness observations, strict radius/contact90degree tests, min-cap equality,
wall return, strict sound threshold/null sound/alternation/byte extension and
facing10degree/NaN behavior. These are static deductions/host checks, not original
Dolphin timing or New2DS performance measurements. Exact build results are in
[progress](../../PROGRESS.md).

The [live native binding](NATIVE_LIVE_NPC_WANDER.md) retains source event work
while stopped and consumes
previously published NPCdt/components/contact in the documented root order,
applying the bound NPC-main/zero-vertical/pose gates and sharing live roots/flags/pose with
rendering and talk. Goomba4's inventory/quest branches precede ordinary talk;
unknown quest state cannot be replaced by an early-story dialogue assumption.
Native model/sound conversion and exact original/new-hardware comparisons remain
required before claiming that moving actor complete.

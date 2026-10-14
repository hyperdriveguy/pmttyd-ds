# AP report construction and invitation ownership

US G8ME01 disc0 revision0. Complete InviteApInfoReport8010436c/4c and
ApRecoveryBuild801a1d0c/294 were inspected read-only. The original report is not
an immediate pouch SP grant or an event completion shortcut for empty audiences.

## Ordered source operations

The caller retains current B+18ffc, requests the build with that twelve-byte
record, then clears its retained twelve bytes. The builder independently
captures current B and inline audience A=B2858. If A137c8 is null, request
BattleAlloc(12), store the returned pointer there, then copy all12 input bytes
to that current pointer. It preserves the allocation across calls. Source
allocation/null/address semantics belong to its provider; no guessed fallback
buffer or skipped allocation is supplied.

Resolve Mario then party through retained B. Start single sum0 from80425548.
For all13 kinds, query GetPPAudienceNumKind, convert its signed count to single
and fused-add weight from803b4de4+kind*20hex+14. Take double sqrt then round to
single. Read input U8+8 and float+4; each positive contribution adds1 to a
separate factor sum. Mario138 mask20000000 selects8042558c, otherwise10000000
selects80425590, otherwise1. Party selects80425590/804255a0 for those masks,
otherwise1; null party uses1. Actual constants are read from source memory.

GetBreakTurn independently resolves current slot work. Turn>=1 fetches the
current slot again and includes its float14 bonus. Preserve single arithmetic
order: audience factor, AC-plus-byte sum, positive-contribution factor, Mario,
party and optional slot bonus. Capture the old A137c4 at its source branch site
before truncation/store; add the truncated word with wrapping arithmetic.
Always draw irand(100), even with zero population/gain/chance. A draw less than
fresh U8(input+a) requests BattleBreakSlot_PointInc; other draws omit it. Point
increment is a separate provider, not synthesized source bingo/reward state.

Kind count scans200 records. GetExist requires flag1 and excludes100/2. The
retained record must have state19 other than12/13/17; then a fresh actor getter
reads kind1b for the requested kind. It does not reuse Waiting's <=3 criterion.
Getters retain original current-global/captured-record separation and wrapping
index arithmetic. Report construction preserves the full source count/math
path even for the native empty audience.

## Implementation and native boundary

[Shared implementation](../../../source/battle/battle_ap_report.cpp) retains
memory/math/allocator/unit/kind/slot/RNG/point providers. A false return marks an
unbound service after preceding effects and does not authorize blind retry.
The caller clears input only after build readiness. Native Harbor binds the
actual getters, fields, source coefficients and arithmetic, uses its SDK heap0
as explicit allocation policy, and retains the resulting original-field pointer.
Its zero-chance preflight prevents reaching the unbound point-increment producer;
other chance domains remain pending before report effects. This preflight/heap
choice is native policy, not an extra original request or measured behavior.

Tests cover allocation/reuse/copy, source clear ordering and unbound allocation,
weighted sums/multipliers/slot bonus, unconditional RNG, conditional point
increment and missing provider, plus presence/state/kind gates. The original
failed Jump now allocates its report buffer and clears the report, keeps HP4/
dealt1/failure1 and advances to WeaponAftereffect801054e8. Deferred StopAC state
and callbacks remain separate scheduling. Success still waits at voice80104508.
These are controlled native fixtures, not complete original timing, animation,
audio, SP payout, attack completion, tutorial or playable Game evidence.

Next: complete point/bingo producer, original voice/backend, aftereffect and
post-action control; compose native TimingA/main/display ownership and original
class/target menus. HUD/menu and second-screen scene/audience must still use
original resources and physical controls. Read-only full listings and validation
logs stay under ignored artifacts; no original executable/assets are committed.

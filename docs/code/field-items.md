# Field items, coins, quest delivery and battle entry

This guide covers the finite item95 pickup, coin presentation, supporting registry/
effect/Mario/BGM helpers, Goomba quest acquisition/hand-in/reward graphs, and field
battle preparation. [NPC/object owners](npc-objects.md) covers actor identity and
talk admission; [map lifecycle](map-lifecycle.md) covers original area/registry
selection. [Behavior contracts](../behavior/README.md) retain source evidence.
These paths do not complete general pickups, discard UI, encounter scheduling
or all quest/story graphs.

## Item and icon state are separate

`FieldItemRegistry` owns separate fixed pools of 32 item and icon entries. Native
name/free-slot admission covers the explicit finite path; it does not implement
original eviction/overflow. Item lookup includes inactive matching names, while
icon lookup requires active bit 1. Duplicate construction still allocates a new
slot, and later named setters may update an older icon. Named deletion repeats
first-active icon deletion for every matching item name. Do not deduplicate these
operations into one lookup.

The item95 profile starts mode 16/sequence 7 and icon 307. Force-get publishes
sequence 2, resets source counters and sets/clears specified flags; it does not
receive an item or open a window. Subsequent poll returns source status 2 for
missing/inactive/completion flags, independently of pouch receive success.
Icon synchronization checks active state again after the item callback, so a
just-deleted item cannot resubmit its icon.

Coin icon 403 retains original textures 414..417, four frames and delay 6 per
admitted iconMain visit. Hidden active icons still animate. Deferred buffer
initialization and countdown decrement occur in the same visit. Rendering admits
only the supported ID/flag/40x40 material domain; unsupported map-blend paths
hold. The quad uses original .6000000238 size scaling and negative camera yaw.
RGBA alpha is the low byte and controls depth-write/blend admission. Camera flag
priority selects slots 8, 5, 7, then ordinary 4.

## Forced pickup and Mario/effect ownership

`FieldItem95Get` admits its service set before requests. It retains source flag
and status domains separately. Entry falls through to item-get motion, item at
MarioY+50, kind-6 effect at Y+62, original windows and real pouch receive. It
then yields before the next substate disables controls and polls the presentation.
A/B mask 0x300 only acknowledges when the main window action is zero. Fanfare
cleanup is remembered independently from window acknowledgment.

Receive success restores controls/audio/camera and deletes named entries.
Failure enters discard selection (kind 1/item95/mode1) and `DiscardPending`;
that frontier cannot be reported as a successful grant or completed teardown.
Original window definition addresses and effect payload selector 5 remain source
identities, not replacement story messages.

Mario motion 15 is item-get. Ordinary idle/fall rows need their real post hooks
before mutation, while special motion branches retain their distinct speed/no-op
behavior. Transition stores precede the post callback. Entry resets counters and
speed, derives the retained travel heading and requests original `M_I_U`.
Alternate heading aliases and unsupported motions require their real providers.

Kind-6/7 effect lifetime 1000 is a sentinel, not automatic expiry. Soft deletion
sets 16 then immediately decrements to 15; alpha follows the source tail, while
age and scale have separate update rules. The 12-entry percentage scale curve
runs only for early ages and retains its final scale thereafter. Kind 7 selects
the original coin size/pink TEV palette.

Drawing preserves separate texture projection, world camera-facing shift and
shadow-only local offset. The latter must not enter projected texture 44; texture
42 retains direct UVs. Shadow alpha truncates 128*alpha/255. Colors use RRGGBBAA,
which differs from other native ABGR owners. The camera-5 helper uses explicit
up with its own lens/depth range and rejects invalid bases. Input camera vectors
already use native units; effect roots/quad offsets remain source engine units.

## Coin callback and borrowed window parameters

Coin placement uses original polynomial/quadrant operations, not generic sine/
cosine replacement. It evaluates (heading*100+5)/100 before angle revision,
offsets 10 horizontally, and puts effect/icon at Y+62/Y+52. Every callback visit
resamples placement and accepts a newly resolved amount, including cleanup.
The window's amount is captured at creation; credit uses the amount supplied on
the later state-100 visit, preserving source operand resolution.

Timing deliberately subtracts low tick words with U32 wrap, divides by
(hardwareFrequency>>2)/1000, narrows signed32 and tests strict greater-than 200
or 500 milliseconds. High words do not replace that arithmetic. A zero divisor
means unavailable clock. State 10 waits for source A mask 0x100 before state 100
credits coins and closes presentation; it does not also require window-ready,
key/system ownership or the item-get fanfare checks.

The original window borrows two parameter words. Native stable owner storage
replaces the map allocation, so status 2 at state 100 does not end that storage
lifetime. A deleted slot can retain its action value. Explicit window-owner
retirement proves deletion separately from the source cleanup poll. Self-owner
identity prevents a copied presentation object advancing pointers into another
owner.

## Retained BGM and stream envelopes

BGM state numbers are source domains: primary 2 detaches, secondary 8 mutes,
secondary 16 resumes. `silent()` tests assigned plus state 8, not zero measured
output; the retained target is 40 percent. `check()` returns zero assigned and
-1 vacant. Duration conversion truncates signed32 and wraps low16 before native
positive-duration admission. It does not saturate to a guessed tick count.

Main publishes integer volume before updating counters. Secondary factor uses
127, optional global percentage is signed, and U8 narrowing is not clamping.
Ordinary/fast secondary counter steps are 1 or 5 per visit; reaching full volume clears
the state on a later call. Primary detach requests a separate 100ms stream tail.
Stream fade can publish volume zero after subtraction; stop occurs only on a
later gated call observing preexisting exact zero. A closed gate advances nothing.
These are simulation/service invocations, not rendering frames or original IRQ
scheduling equivalence.

## Quest callers and source completion

Goomba graphs bind exact actor identities, known quest switches and live pouch
observations. Shared art, unknown flags or cached mushroom counts cannot select
an original quest arm. Original message IDs, order and speaker attribution remain
unchanged.

Quest2 acquisition prints, enters/forces the named item, yields, then polls on a
later event visit before setting flag 5405. ItemMain is a separate owner and can
continue while mask-6 event stop pauses eventMain. Before each external request,
source item fields publish to the registry. Readiness alone never grants inventory.

Quest2 hand-in reads and releases selector result before branching. Cancellation
-1 does not remove the item. Revised removal is optional: unknown holds; known
zero is a real failed remove whose result the original event ignores before
setting flag 5406. Reward waits for blocking coin status 2 for 20 coins, then
resets quest1420, writes flag 5376 and opens the final original speaker-zero
message. An unavailable coin result preserves first-call state.

Quest13 retains one selection-table allocation; positive refresh writes item107
and -1 sentinel, while zero refresh leaves prior bytes intact. Native admission
rejects growth beyond the original capacity. The first branch uses two authored
500ms pose waits. Only exact selector result 107 removes; other completed results
finish. First-branch cancellation still writes flag 5407. The final message only
opens if live item107 count is zero. These graphs do not substitute general quest
completion or additional story content.

## Field battle service and packed information

Control flag 2/wrapping byte depth is separate from key flag 8/depth. Battle
start repeats work/NPC lookups on resumed calls: first visit prepares owners,
requests sequence 4 and yields; the next consumes after lookup only. Wait admits
sequence 2 and any nonzero result, including non-victory outcomes, then restores
controls/audio/Mario in original order. Callers consume status 2 rather than
continuing to poll restoration indefinitely.

Packed setup clears the 200-byte NPC payload and its additional source subclear
before testing configuration. Audience modes outside 1/2 consume 16 inclusive
signed-byte RNG draws, re-reading the first endpoint afterwards. Weighted group
and held-item tables retain signed widths, source terminators, wrapping totals
and traversal order. The selector bounds are exactly 0xf8405b80..0xf8d9f200
(-130000000..-119934464), not a rounded namespace interval. Shared original RNG
and saved-flag services remain required.

Index -1 selects null configuration; other nonpositive indices select row zero,
positive indices advance by 0x44. Live GP area prefix selects mutable module
registration. Four associated children receive ordered payload pair copies.
Field information lives at F+0x20, selects admitted party kind and maps exact
first-attack selector words; combined bits do not form a generic OR mask. Source
information stores preserve order and widths. Whole lower BattleInit, encounter
readiness and complete original entry/return orchestration remain separate owners.

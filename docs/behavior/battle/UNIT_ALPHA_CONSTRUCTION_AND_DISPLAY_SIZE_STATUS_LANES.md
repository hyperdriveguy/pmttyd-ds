# Unit alpha construction and display size-status lanes

2026-10-14 US G8ME01 disc0 revision0; original ISO/DOL and current native
implementation inspected read-only. Narrow constructor/input closure for
[surviving-unit display progression](SURVIVING_UNIT_STATUS_EFFECT_ICON_AND_PART_FRONTIER.md).
No code/shared docs/index edits or commit.

## Original U311/U312 construction

Inside BtlUnit_Entry80128fe0/6b4, the original ordered constructor prefix
at801295b4 obtains body part, requests GetPoseNameFromType65→U2d8, then
GetPoseNameFromType43→U2dc. Immediately afterwards:

```
801295e8 li r0,255
801295ec mr r3,retainedUnit
801295f0 stb r0,U311
801295f4 stb r0,U312
801295f8 bl btlDispEntAnime801011fc
```

The two bytes are initialized to the same literal255 before animation resource
entry. There is no intervening service between their stores, no kind/HP/story
condition, and no derived pose/transparency value. Later U314/U318 zeroing and
kind-specific event construction follow these writes.

U311 is target move-color/alpha and U312 its current display value in the used
Main branch. Main first clears U1c bit2, compares captured U312 to freshly read
U311, sets flag2 only if different, slews+12 toward a higher target or-2 toward
a lower target with crossing clamp, and stores LOW8 U312. Initial255/255
therefore does not begin a native0→255 display ramp. Later actual target
writers must remain operational; do not force both bytes255 on every frame.

Original btlsubResetMoveColorLvAll80125398/54 captures passed B, scans64 slots
via BattleGetUnitPtr(B,index), and writes U311=255 only for each nonnull unit.
It never writes U312. Its ending invocation does not replace the constructor
initial current value or normalize any existing animated current byte.

## Read-only current native gap

Shared source/battle_unit_entry.cpp already reproduces both255 stores in its
full portable source entry service, at the same relative pose/resource order.
TrioAttackRuntime's custom unit construction in source/trio_attack.cpp creates
zeroed mapped Units storage and initializes identity, parts, geometry, resources
and statuses, but it does not invoke the shared complete Entry constructor and
currently does not write U311/U312 during this custom loop. Its ending reset
exports U311 only. Consequently initial U312 remains0 and first admitted
original display Main would slew it to12, a construction gap rather than
evidence that original actors fade in from0.

Bind the two literal constructor writes once for each newly created native
trio unit, and retain any subsequent source U311/U312 history. This closes the
specific used scalar producer; it does not declare the custom native unit
constructor equivalent to whole BtlUnit_Entry or animation-resource initialization.

## GetStatus11/10: existing portable service and exact bytes

Original BtlUnit_GetStatus80127f34/1c4 writes optional output BYTE pointers,
defaulting both values0. Used size-status IDs10/11 share duration BYTE U123
and strength BYTE U124:

| ID | Admission | Returned duration / strength |
| --- | --- | --- |
| 11 | signed8(U124)<0 | raw U8(U123),raw U8(U124) |
| 10 | signed8(U124)>0 | raw U8(U123),raw U8(U124) |
| either | its strength-sign predicate false | 0,0 |

No absolute value, sign extension in the output store, duration-positive test,
status mutation or timer decrement occurs in this getter. The display caller
then interprets both returned bytes SIGNED and requires duration>0, applying
its status11 negative-strength or status10 positive-strength factor. A raw
duration128 is nonzero for CheckStatus but negative for this caller's >0 gate;
do not use a generic presence boolean in place of returned byte semantics.

source/battle_status.cpp already implements this used mapping in getStatus:
durationOffsets[10/11]=123hex, strengthOffset=124hex; even ID strength<=0
returns empty, odd ID strength>=0 returns empty, otherwise returns StatusValue
{rawduration,rawstrength}. BattleStatusUnit owns original118..135 byte storage
and StatusValue fields are uint8. This is adequate for fixed IDs11/10 if the
native binding imports the live original30 status bytes and explicitly casts
consumer values to int8. Fresh-zero status output0/0 follows current storage;
it is not permission to overwrite status bytes or infer other display gates.

## Evidence and validation

Complete original Entry80128fe0/6b4 SHA256
28c393e0dda25fa7af7e6c82f477f4067e7e5e7f6b50d09766996228814707f9.
Exact initialization span801295e8/14 SHA256
d6595c6bb6036f580c81ea2630c6b86cdebd9310e5bf50aff9b645185192c750.
GetStatus80127f34/1c4 SHA256
1844c5989153ff06fa1639a54f08af95913427b8b88d4dd7ac31f66867702322.
Ignored whole Entry listing display-alpha-BtlUnit_Entry.dis under
artifacts/recovered-temp-evidence/. Source reset prefix listing remains scoped
to48hex bytes, not falsely described as a whole54-byte body.

Read-only original/native inspection and scoped documentation diff check.
No native tests, build, rendering or emulator claim.

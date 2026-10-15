# Packed pinch checks and status recovery

Implementation: [pinch source](../../source/battle/battle_pinch_continuation.cpp),
[pinch API](../../include/battle/battle_pinch_continuation.hpp),
[recovery source](../../source/battle/battle_packed_status_recovery.cpp),
[recovery API](../../include/battle/battle_packed_status_recovery.hpp).
Original numeric IDs and offsets remain authoritative; unnamed fields keep their
numeric identity. These are implementation notes, not new Dolphin measurements.

Pinch has separate direct-unit and all-unit entry paths. The cursor pins entry,
input, mode and provider identity; callbacks must keep their owners alive. Reset
cannot roll back status flags, audience changes or countdown writes. The all-unit
path refreshes all 64 registry slots before scanning alert candidates. A held
per-unit audience adapter retains its unit, phase and operands.

| Packed field | Reached behavior |
| --- | --- |
| Unit `+0x10` | Definition pointer; thresholds at definition `+0xd` / `+0xc` |
| Unit `+0x10c` / `+0x108` | Signed current / maximum HP |
| Unit `+0x138`, `0x20000000` | First threshold flag and first alert group |
| Unit `+0x138`, `0x10000000` | Second threshold flag and second alert group |
| Unit `+8` / `+0xc` | Kind / belonging byte |
| Battle `+0x18ff8` | Shared wrapping alert countdown byte |
| Battle `+0x1720`, bit 0 | Sound gate |

A threshold requires HP at or below its limit **and** below max HP. Newly setting
a flag may call the retained audience target adapter for mode 0/Mario kind 222.
The first flag suppresses the second adapter. Flag stores precede holds; clearing
a flag does not undo a previous audience adjustment. See
[audience continuation](battle-retained-audience.md).

Alert scan first excludes status 27 and nonzero belonging. Its first otherwise
qualified unit ends the group even for wrong kind or dead HP. Accepted kinds are
Mario 222 and partners 224 through 230 with positive HP. Do not turn this into a
search for any later living partner. Decrement countdown wraps as a byte; values
1 through 127 delay the alert. Expiry writes 60 source invocations, not rendering
frames, before the sound gate/provider. Sound keys are original guest addresses
`0x802efc04` / `0x802efc14`. A held sound retains that completed countdown work.
No accepted candidate clears Mario's unnamed `+0x238` word and shared countdown.

Recovery is synchronous and separate from the retained pinch cursor. Null unit,
status 27 or nonpositive signed HP skips before GetStatus. GetStatus runs before
selector validation: raw IDs narrow to signed low byte, accept 0 through 24, and
exclude 16/17. The duration-offset table deliberately shares slots for 10/11,
12/13 and 14/15. Do not expand those into independent fields.

ID 1 clears duration at <=1, including negative durations. Other signed durations
step toward zero with byte wrapping. IDs 10 through 13 preserve strength until
expiry; other reached strengths step toward zero except 4/14/15/23/24. Zero
duration clears local strength. Opposite statuses 8/9 cancel their opposite
shared field first rather than also storing incoming duration. IDs 14/15 use
fresh signed strength to decide cancellation of their shared defense pair. IDs
19/20 cancel opposite positive speed durations and then run the reached move
reset leaf: unit `+0x27c` bit `0x1000` suppresses it; positive signed baseline
`+0x23` must equal current move count `+0x22`.

Recovery's return value reports whether its **local duration** reached zero.
A zero result can still mutate state; a one result does not prove every shared
field cleared. Keep fresh reads after GetStatus and source-ordered writes rather
than snapshotting a unit and writing it back wholesale.

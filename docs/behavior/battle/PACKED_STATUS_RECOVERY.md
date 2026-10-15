# Packed source recovery step

`checkPackedBattleRecoveryStatus(unit, rawId, actors)` implements the complete
local BtlUnit_CheckRecoveryStatus (`80127890/2c8`) directly against actual guest
bytes. This supplies the synchronous child requested by the retained original
Turn controller as function `80127890`, arguments unit/status. It returns the
original explicit integer0/1; zero can follow mutations. The runtime caller must
bind this leaf using its actual VM actor services, rather than interpreting zero
as unavailable or replaying a native whole-unit snapshot.

Source evidence: [status storage](BATTLE_STATUS_STORAGE.md),
[Turn recovery](BATTLE_TURN_RECOVERY.md). The new read-only verifier
[verify_battle_packed_status_recovery.py](../../../tools/verify_battle_packed_status_recovery.py)
pins complete Recovery, SetStatus, CheckStatus, GetStatus, _CheckMoveCount and
ResetMoveStatus bodies (629 instructions), plus all25 recovery jump targets at
`80364204`. Original ISO bytes are only read; no PPC is executed.

## Ordered guards and dispatch

Null unit returns0 before any provider access. Otherwise CheckStatus27 runs
first, then signed halfword HP at10c; down or HP<=0 returns0 before the requested
status is read. GetStatus receives the original raw ID and owns signed-low8
narrowing, shared duration/strength sign gates, and exact field read order.
Duration0 returns0. The signed low8 ID then selects0..24; charge16/status17
are explicit no-op targets. IDs25/26 have no stored status; other IDs outside
this recovery dispatch cannot mutate. Badge-only Electric does not gain an
invented recoverable duration or extra badge read.

Sleep1 writes only byte118: signed duration>1 decrements, otherwise clears.
Auxiliary byte119 is neither read nor written. The other mutating groups move
the signed duration one toward zero. Shared10/11 and12/13 directly write their
original duration then strength bytes; they retain strength until duration0.
Poison4/defense14/15/regeneration23/24 preserve strength while active and use
the source setter. Other reached statuses independently move strength toward
zero before that setter. New local duration0 forces local strength0. Return1
uses that local duration, even when setter cancellation changes other bytes.

## Reached setter and move children

The private setter contains only branches reachable from this recovery body.
It preserves their fresh reads instead of reconstructing current guest state
from GetStatus's earlier outputs. Size/attack10..13 bypass it exactly as source.
Opposite fire/freeze8/9 cancellation reads the current opposite byte. Defense
14/15 rechecks current signed strength before its original cancellation or
zero-strength clear. Flipped shared strength between getter and setter can
therefore cancel the source pair.

Fast19/Slow20 read the current opposite status, cancel it when both signed
inputs are positive, otherwise write the current duration, then always run
_CheckMoveCount (`801280f8/4c`). That child reads flags27c first; mask1000 skips
counter reads. Otherwise signed baseline23 must be positive and equal freshly
read current22 before the existing exact packed ResetMoveStatus child runs.
The reset's Fast/Slow/status/move-byte reads and writes stay source ordered.
No unrelated unit bytes are read or written and no RNG/numeric provider occurs.

## Validation and integration frontier

Focused tests compare all28 IDs across duration/strength sign boundaries and
opposite-status combinations against the existing native status model, checking
whole guest storage for unrelated writes. Explicit traces check null/down/HP
prefixes, lower8 aliasing, preserved Sleep119, shared-sign duration-read omission,
getter/setter mutation, source Fast cancellation before move checks, reset
children, and flag/baseline short circuits. Strict host execution and ARM syntax
pass. The source verifier independently matches the read-only original ISO.

The module does not schedule a turn or claim original tick cadence. Root runtime
integration must bind the actual retained Turn request to this synchronous leaf;
normal battle activation and the surrounding recovery event/effect/audio owners
remain their separate actual providers.

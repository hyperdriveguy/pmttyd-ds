# Bottomless event chain

US G8ME01 disc0 revision0; static contracts, no execution or gameplay claim.
Closes `bottomlessBero` and `bottomlessEvt` in the [unaddressed-functions
register](../binary/UNADDRESSED_BINARY_FUNCTIONS.md), continuing the
bottomless flag/position stores in [speed/fall/landing
leaves](OVERWORLD_SPEED_FALL_LANDING_LEAVES.md) ([Mario+736] = 1,
[Mario+740/744/748] = arg triple).

## bottomlessEvt (8007ac7c, 51 lines): event entry, triple conversion

Event callback, returns 2. Resolves three event words via `evtGetValue`,
converts each signed integer to float with the 0x43300000-bias double trick
against the table double at 0x802C3058 (the same constant bero_length's leaf
uses), and tail-calls `bottomlessBero` (0x80096de4, same-TU `<.text+0x1c168>`
edge) with the three floats. No destination write, no result test.

## bottomlessBero (80096de4): recovery-point writes

Float args f1/f2/f3 are retained across marioGetPtr. The complete register
traffic establishes Mario+740=X,+744=single(Y+37),+748=Z. FPRf0 loads37
from80420aa4 (42140000) at80096e1c,then fadds at80096e28 consumes that FPR.
The intervening lwz writes GPRr0; it does not overwrite FPRf0. The earlier
zero-table-word interpretation mixed those register namespaces and is corrected.

[Complete fresh producer/actor contract](BERO_INITIAL_POSITION_LOCAL_PUBLICATION_AND_ACTOR_WRITES.md)
records exact source world side effects and input provenance. bottomlessPos
stores the plain argument triple; this Bero helper adds37 to its Y lane.
This is static source evidence, not a measured gameplay recovery trajectory.

## Not established

Table contents, offset meanings, SDA values, call cadence, and any gameplay
effect.

# Retained complete original Turn parent

The native `resumePackedBattleTurn` translates the entire original US
`btlseqTurn8011c510`, size1ed8(hex), rather than the earlier empty-phase harness.
`tools/verify_battle_turn_controller.py` opens the ISO read-only and pins all1974
instructions, the78-entry table at80363cc8, original conversion bias802efb78 and
startup80005418 establishing r2=80427440. Original extracted bytes stay ignored.
This is instruction evidence, not a Dolphin timing or hardware parity claim.

The source dispatch, calls, resource mutation and barriers are one retained
invocation. Tail-recursive source calls become immediate native dispatch with
fresh aggregate flags; they introduce no render delay. The simulation owner must
finish a held invocation before admitting another tick. Service identity, battle
identity and nonreentry guards prevent replacing a held child's owner. Reset is
refused while a provider executes. Calls retain their exact arguments across
unavailability, including private position/status outputs; no guest stack
addresses or successful resource readiness are fabricated.

The complete dispatch includes:

- 00 increment/yield;01 TurnFirst;02/05 immediate progress;03 first-turn setup
  byte and120-count initialization;04 decrement, physical button mask300 and
  original camera8/class1 callback8012c350 with source float900.
- 06 Poison;08 first Burn eligibility;09 retained registration/poll/wait substates.
  Burn polls every nonnull unit after entry, including units no longer eligible.
- 0a/0c status HP/FP regeneration;0e/10 Happy trials. Original signed division
  precedes multiplication; each copy consumes one RNG draw. Status HP display
  bookkeeping retains the original pre-call attribute branch. Resource narrowing
  and FP ownership stay with their actual children.
- 12..3e the23 source-ordered recovery passes, with early null calls versus later
  explicit null skips, Slow move reset, event-allocation failure semantics,
  once-per-pass REST sound and Freeze direct damage/effect/3D sound exception.
- 40/42/44 signed-word countdown decrements;46/48 admitted live-unit halfword
  decrements;4a immediate increment;4b increments then initializes channel4;
  4c original phase parent;4d original audience TurnEnd then sequence reset and
  battle flags clearing. All original odd wait entries preserve event/conclusion
  order. Conclusion writes channel0=3 then channel7=07000000.

The details remain in [periodic damage](BATTLE_PERIODIC_DAMAGE.md),
[regeneration](BATTLE_REGENERATION.md), [recovery](BATTLE_TURN_RECOVERY.md),
[first process](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md),
[phase rules](BATTLE_ACTION_RECORD_TURN_RULES.md) and
[waits](BATTLE_WAITS_AND_CONCLUSION.md). Their current source contracts govern
callee behavior. `BattleTurnRequest` exposes **original child** calls only, not
whole case handlers; the parent owns all scans and transitions. Private outputs
replace the source stack's duration/strength/position locals. An engaged zero
means a real source return, including failed event/effect allocation; missing
services hold at the actual function.

Binary32 expressions retain signed-word lfd/fsubs conversion from the actual
bias and fused multiply-add. Status regeneration reads bias before unit scale;
Happy reads scale first. Happy zero maximum remains an explicit unsupported
source divw domain; no denominator, threshold or RNG result is invented. Original
Freeze scale1 comes from8042256c. A stale earlier inferred r2 address is not used.

Focused host cases cover every dispatch index and out-of-range returns,
empty recursive passes, held children without mutation replay, changed Sleep's
next-invocation wait, Slow reset, failed/successful event allocation, exact Poison
arguments, Burn entry/poll separation, regeneration bookkeeping and fused height,
Happy draws/yield, Freeze-only presentation, countdown widths, conclusion order,
and changed-service/battle refusal. Actual children, native source Main cadence,
active camera4→HP8 and field return require their runtime integration; this
parent alone is not proof of a playable original battle or tutorial.

## Native integration status

Commit e78d2325 routes actual retained SequenceDispatch to the VM-backed Turn
owner, including real packed recovery, event barriers and audience TurnEnd.
Original Phase remains unavailable; the legacy harness is preserved and ordinary
source Main is not activated. Full validation and remaining providers are in
[progress](../../PROGRESS.md). ARM component fixtures pass without claiming
active battle rendering or original execution equivalence.

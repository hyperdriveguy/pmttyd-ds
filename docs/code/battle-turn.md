# Retained Turn controller

Implementation: [source](../../source/battle/battle_turn_continuation.cpp) and
[API](../../include/battle/battle_turn_continuation.hpp). Original evidence and
callee contracts: [complete parent](../behavior/battle/RETAINED_COMPLETE_TURN_CONTROLLER.md),
[periodic damage](../behavior/battle/BATTLE_PERIODIC_DAMAGE.md),
[regeneration](../behavior/battle/BATTLE_REGENERATION.md),
[recovery](../behavior/battle/BATTLE_TURN_RECOVERY.md).

The native `Step` is a retained program counter inside **one** source invocation.
It is separate from Turn sequence channel 3 (`0x03000000` base). `inc()` advances
that channel, `skip()` advances an action/wait pair, and `recurse()` dispatches
again immediately within the invocation. These operations do not introduce a
render delay. The caller supplies simulation cadence and must finish a held
invocation before starting another. Cursor reset cannot undo earlier writes.

`BattleTurnRequest.function` is an original guest callee token. Its eight integer
and four float operands belong to that reached child's signature. Output `value`
is interpreted by the child (integer, handle or guest entry pointer); position,
duration and strength replace original stack outputs with typed private storage.
The request builder runs once, including its guest reads. Missing service or
`nullopt` retains that request; any engaged output, including zero, acknowledges
it. Preserve callback/users and battle identity. The equality check does not
include `audienceLifecycle`; caller still owns every service/allocation lifetime.

| Turn index (hex) | Reached work / completion boundary |
| --- | --- |
| `00` / `01` | Increment and yield / original TurnFirst child |
| `02`, `05`, `4a` | Increment and dispatch immediately |
| `03` / `04` | First-turn intro gate / decrement countdown, physical mask `0x300`, queued camera-8 callback |
| `06` | Poison status strength, shadow check and direct damage |
| `08` / `09` | Burn eligibility / registration, poll and eventual wait |
| `0a`, `0c` | Status HP / FP regeneration |
| `0e`, `10` | Happy HP / FP trials; skip both on signed turn <=1 |
| `12` through `3e`, even | 23 status recovery passes in original selector order |
| Odd action waits | Barrier, then conclusion; source not-ready completes invocation without advancing |
| `40`, `42`, `44` | Nonzero word countdowns at battle `+0x19050/+0x19054/+0x19058` |
| `46`, `48` | Admitted unit halfword countdowns at `+0x13c/+0x13e` |
| `4b` / `4c` | Increment then initialize channel 4 / original phase child |
| `4d` | Audience TurnEnd, reset Turn to `0x03000001`, clear battle `+0xef4` bit `0x2000` |

Unsigned bank subtraction outside indexes `00..4d` completes without work.
Dispatch resets per-pass aggregate/changed flags only when starting another pass.
A held child retains its `Step` and earlier mutations. Intro uses original
120-count initialization at battle `+0xefc`; timer decrements before its signed
expiry check. Camera 8/class 1 callback `0x8012c350` reads original priority from
`0x80422594`. A queued display yields with the same Turn bank.

Each unit pass scans 64 registry slots. Ordinary eligibility calls CheckStatus27
before reading signed HP. Recovery is dispatched **before** these ordinary
filters: early passes call recovery even for null units, while indexes >=`32`
explicitly skip nulls. Preserve that difference. An engaged barrier zero is
completed source “not ready”; `nullopt` is a provider hold. Ready checks conclusion
and either increments or writes channel 0 = 3, then end channel 7 = `0x07000000`.
See [barrier implementation](battle-event-barriers.md).

Burn uses battle halfword `+2`: zero stores one before registration; one polls;
later values wait. Registration then polls immediately. Poll every nonnull unit,
including those no longer eligible. Any active burn yields without advancing;
when none is active, increment the substate and yield. Burn detection at index
`08` resets that substate and stops its scan on the first positive signed strength.

Recovery follows selectors `{1,2,3,5,6,7,8,9,10,11,4,12,13,14,15,0,23,24,18,19,20,21,22}`.
A zero return may have mutated status. Aggregate nonzero results yield one REST
sound at key `0x802efb80`, except Freeze pass `20`. Slow pass `3a` resets move
status before event creation. Freeze uses its special shadow/direct-damage,
position/height, effect and positional-sound path. Poison shadow success skips
damage; Freeze still reaches presentation. Direct damage keeps actor -5, part 0,
FP 0, source codes 279/282, pattern 0 and recovery flag 1. See
[recovery leaf](battle-pinch-recovery.md).

HP regeneration's pre-child attribute branch controls display bookkeeping:
unit `+0x208/+0x20c` contain before/after HP, halfwords `+0x200/+0x204` copy
`+0x202/+0x206`, and `+0x1fe` ORs bit 0. Do not recompute the attribute decision
after the child. Resource ownership and narrowing remain with the actual HP/FP
children. Body ID, part pointer, part XYZ and unit height are separate children;
regeneration adds five to height, Happy does not. Bias-before-scale versus
scale-before-bias load order and fused multiply-add preserve source arithmetic.

Happy reads badge copies at unit `+0x2ee/+0x2ef` and uses one `irand(100)` per
copy. Threshold is `33 - 3 * (signed current / signed maximum)`, integer division
first, with fresh current/max between trials. Zero maximum holds explicitly;
it does not gain a made-up denominator or consume RNG. Happy HP narrows the sum
before upper-clamping to max and bypasses the HP helper; Happy FP uses its child.
Neither is interchangeable with a float probability or generic heal routine.

`evtEntry(script,10,0)` returns a guest entry **pointer**, not an event ID.
Engaged zero allocation failure still sets changed. On nonzero, fresh entry
`+0x15c` supplies unit `+0x2a8`, then fresh unit ID supplies entry `+0x160`.
Earlier damage/resource/effect calls never replay during allocation holds.
Unchanged passes immediately skip their wait. Changed Sleep `12` and Happy HP
`0e` yield after increment; other changed passes may reach their wait immediately.

This guide covers the reusable parent. It does not certify current Trio provider
integration, active source Main, playable battle, original execution parity or
field return. See [Trio scheduling](trio-scheduling.md) and
[audience TurnEnd](battle-retained-audience.md) for adjacent owners.

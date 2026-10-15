# Trio events, entry and damage

This covers `call`, `initialize`, damage/motion helpers and barrier queries in
[TrioAttackRuntime](../../source/battle/trio_attack.cpp). See
[provider ownership](trio-providers.md), [commands](trio-commands.md),
[packed motion](packed-battle-motion.md), [damage order](battle-damage-order.md)
and [ending](trio-ending.md). The runtime still has a partial audit: turn/handoff,
Main/display scheduling and remaining public draw/material adapters need review.

## Callback domains and completion

First visits record original function/command and raw operands. Typed registry
bindings take precedence over the switch's guest-address cases. Raw operands,
resolved values and output destinations are different domains. Return 0 waits,
2 completes, 255 is a special scheduler exit; individual movement wrappers also
use original return 1. A signed negative damage result is separate from native
provider unavailability.

The switch includes three kinds of work: actual packed source mutations, narrow
native admissions around linked helpers, and explicitly request-only presentation
adapters. Stylish/voice/effect/camera requests in the last group can return 2
without completed audio/GPU ownership. Unknown callbacks retain their pending
function and wait. Do not generalize a specific request-only adapter into a
success fallback for other functions.

| Packed domain | Offsets/constants and constraints |
| --- | --- |
| Installed scripts | Unit `0x28c/0x2a0/0x2b0/0x2a4`: wait/attack/damage/confusion; attack clears `0x2ac` |
| Action callbacks | `0x1ca0/0x1ca4/0x1ca8/0x1cac`: Main/Result/Display/Delete |
| Action setup | Table `0x80364348`, 20-byte ID + four-pointer rows, ID 0 terminator |
| Difficulty | Raw/adjusted bytes `0x1cbc/0x1cbd`, clamp 0–6 |
| Pose table | Part `0x1bc`, 8-byte rows, fallback types 28/39/69 |
| Position output | Raw sentinel `-250000000` omits destination; numeric setters separately compare float sentinel |
| General candidates | `B+0x428`, weapon admission/attributes at `0x64/0x68`, choice at `0x7c` |
| Stylish window | `B+0x271c` count, `0x2720/24/28` lo/hi/end, `0x272c` actor, `0x2730` result, `0x2734` history scan |

Table-pose lookup accepts the first exact **or fallback** row; an earlier fallback
can win over a later exact match. Types 29–39 force animation restart. Ordinary
identical pose publication preserves the scene clock. Wing-loss redefinition
admits the specific source kind/part-definition combination and copies original
attributes/defenses/offsets; the following pose request owns animation restart.

Prechecks admit their declared fresh status/accuracy/counter/fog domains. Even
accuracy 100 can consume `irand(100)` when bypass flags are absent. Protector
queries scan all admitted actors before writing both unit/part outputs. A
zero-cost weapon still performs the supported FP/accounting writes; nonzero
item/FP/AP cost does not become free through missing providers. Source kind is
reloaded after each random drop draw, preserving callback-visible reads.

The stylish-window count increments before testing. A history hit can fail early;
`lo<t<=hi` succeeds, later input fails, absent/flagged actor or timeout completes
without success. Input history, automatic actor mode, source counters and result
codes remain distinct from render frames. TimingA/Hammer Stop requests their
cleanup state; reading result/count does not advance evaluation.

## Scoped entry and memory

Native entry retains a supplied canonical pouch; a legacy checkpoint supplies
only current Mario HP over the explicit fresh snapshot. It binds actual file,
camera/stock-EXP and spotlight owners and allocates separate SDK/map arenas.
This composition does not claim complete original battle startup/UnitEntry.

Native heap is `0x82000000..0x82100000`; map reservation is at `0x85600000`.
Battle/Units/Parts/Kinds/Work use their declared disjoint native spans. Pouch
projection at `0x85000000` includes the original `0x5d4` bytes. Guest pointers
remain 32-bit tokens; float transport copies bits and required access failure
aborts. Source sequence/phase/actor/attack selectors are `0x0300004c`,
`0x04000002`, `0x05000009` and `0x06000007`. A rate word of 60 is native entry
context, not measured hardware cadence.

Registry actors occupy IDs 1–6. Mario has three authored parts: his primary part
keeps its address, while the extra two occupy the unused unit-0 part partition.
Three `0x500`-byte records in his normal `0x1000` slot would overlap Goombella.
Linked order and original `0x4c` definitions remain intact. Pose ownership requires original resources and matching names; missing factories
are not replaced with generated poses. Resource import follows fresh status clearing before first-turn/phase
work. Movement sound requests can explicitly return failed handle -1; a retained
name is not successful public audio allocation.

## Damage, motion and barriers

Damage read/write is selective. It neither copies a whole typed struct nor
exports all FP/status/badge storage. Primary enemy elemental defenses come from
original spec rows. Hit dispatch publishes the current typed damage snapshot
before retiring old hit/wait/phase/attack events and creating a new damage root.
A null installed script returns before cancellation. Inventory count is a scoped
native zero provider; general inventory/revival behavior remains unbound.

The admitted Jump, Hammer, Goombella and enemy-default power domains stay
separate. Superguard can apply zero defender damage and an attacker reaction,
then return -1 to the original wrapper. Ordinary damage calculates HP then FP
with shared hit code, updates defender sequence, applies direct damage and then
publishes attacker flags/sequence. Results 18/19 distinguish nonzero/zero HP-or-FP
amounts. Goombella's level/power index has an upper clamp only; valid lower level
is caller-owned.

Dive and generic jump use source visit counts, not rendered frames. Dive samples
before incrementing elapsed and ends after `elapsed>count`, with signed sine-arc
Y and ordered movement-sound control. Generic jump integrates Y, gravity, then
horizontal motion before count decrement and final snap. Count/speed divisions
retain source order, including zero-count exits after preparation; documentation
adds no fallback arithmetic.

A barrier checks actual live event IDs/active flags, clears retired handles,
requires empty reaction queues and checks stage events. Conclusion uses three
source group policies and exclusion masks, not simply enemy HP. Audience and
break-slot event pointers clear only after retirement. Unsupported queue,
reserve-item, slot mode or message work remains pending.

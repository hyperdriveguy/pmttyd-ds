# Audience work, population and actor control

See [items/selection](battle-audience-items.md) for held items and lifecycle-aware
native selection, [sound](battle-audience-sound.md) for voice envelopes and
[class display](battle-class-raster.md) for audience animation bytecodes/queueing.
Guest work and native provenance metadata are separate owners. This guide explains
the implementation; recovered evidence remains in the
[behavior contracts](../behavior/README.md).

## Work and parent ordering

Audience `A = B+0x2858`; current B comes from `0x8041eb90`. Offsets are hexadecimal
unless a decimal count/stride is stated:

| Audience-relative region | Use |
| --- | --- |
| 1bc, stride 134 | 200 actors |
| f25c, stride 48 | 100 item records |
| 133fc, stride 24 | 24 sound records |
| 1375c | Window work |
| 13778 / 1377c / 13780 | Baseline / desired / offset floats |
| 13784 / 13788 / 1378c | Total / right / left control counts |
| 13790 / 13794 | Signed capacity / twelve species weights |
| 137f0 / 137f4 / 137f8 | Throw count / limit / impact counter |
| 138bc / 138c0..c8 | Selected slot / XYZ cache |

Init clears `0x13914` bytes and emits the actual Reset observation before setup.
Normal and Luigi texture paths require real resource handles. It independently
captures owners for guest texture and throw setup. A draw below 80/90/95 of 100
chooses throw limit 0/1/2, otherwise 3. Counts, window float, noise, slot-2 volume
request `(60,180)` and target initialization follow in source order. Missing late
providers can follow reset, resource stores and RNG.

Main independently captures B/A. A rising battle `0x80` flag requests window
activation, then runs Control, Item, AP-source, Window and Sound children in that
order. Only afterward does it mirror battle state into audience `0x10000`. This
immediate parent has no retained suffix. Completed child effects survive a later
failure; the caller needs retained/admitted composition rather than replaying it.
Inactive-item/AP helpers implement only rejection prefixes, not active children.

Control counts ten rows of twenty seats, right half first, excluding states 11/12.
Sound counting excludes 12/13/17 instead. Motion flag `0x10` takes direct vertical
integration and bypasses the actor child, even without Exists. Other existing
actors with species at most 12 require their real child controller. Throw attempts
use bound 60 under forced cap 10000, otherwise bounds 1800/6000/30000 for counters
0/1/2; higher counters reach an unassigned source-bound frontier. A zero draw
shuffles 200 seat indices with 1000 swaps, then stops only on an actual ItemOn
result 1. Optional result zero is acknowledged rejection, not missing work.

## Population and seat choice

Source actor flags 1/0x100/0x80 mean active/reserved/paired-owner. Seat selection
reads rank-specific 40-byte width rows; rows/columns have source front/back weights.
Weights multiply and add one with word wrap. Weighted selection still calls raw
RNG and truncation when empty; its exclusive cumulative endpoint can return -1
at raw RNG 32767. Uniform selection returns -1 without RNG when empty. Below 20
counted actors, front-then-back falls back only on an actual -1, never on missing
weighted numeric providers.

Entry rejects occupied/reserved slots and positive odd paired slots (signed
remainder equals 1). It publishes flags 5/0x85 and ActorBirth before raw-RNG
jitter. Source X spacing is 25 with original center offsets; stage floor and
`FMA(columnCoefficient,column,row*30+115)` produce Y/Z. Species 11 and appearance
11 independently alter depth/Y. Flag 8 marks columns 4..15 of the first three
rows. Paired completion reserves the following record with 0x101, then emits
ActorReady. Earlier Birth alone is not a completed actor.

SetAnimation requires an active nonreserved actor. Equal nonnull animation root
skips restart only with force zero; otherwise current/root/timer/loop fields reset.
ChangeStatus redirects reserved slots to the preceding owner and resets original
transforms/counters/alpha. Delete clears the paired neighbor before owner and
observes each clear. These leaves do not invent item unlinking.

Full population uses rank capacity `50*(rank+1)`, the copied 65-row level table,
twelve weighted species plus guest 12. Mode 2 chooses two special species, 5 one,
6 all paired-adjusted species; other modes preserve threshold-cardinality draws,
24 shuffle swaps, signed setup weights and source override mapping. Quotients use
wrapped signed arithmetic; zero divisor and overflow division remain unavailable
after earlier stores. Paired species consume two seats. Residual allocation and
guest substitution retain their original ordering. The sort compares weight[i/j],
not weight[order[i/j]]; changing that would alter the source algorithm.

Mode 2 fills enumerated uniform seats; other modes build and shuffle a seat list.
Even length zero makes 300 bound-zero RNG requests. The narrow setup-4/zero-capacity
adapters preserve this work and still make sixteen bound-one setup requests.
A zero audience does not imply a side-effect-free initialization. Requests with
bound zero need not advance the particular RNG implementation, but cannot be
silently removed from the service trace.

## Actor reactions and target/window numerics

Actor fields `+4/+8/+0x30` are phase/count/timer, `+0x19/+0x1b` state/species.
The 21-entry state table is followed by XYZ position/velocity/acceleration updates
at `+0x48/+0x78/+0x84`, even for an out-of-table state. Separate jump fields
`+0xd0/+0xdc/+0xe4` govern vertical motion; species 11 zeros them and species 2
adds a wrapped sine hover. Render position publishes at `+0xc0/c4/c8`. A late sine
hold retains earlier integration/phase stores; there is no local retry cursor.

Turn/head-tilt/jump timer thresholds, arm curve 4/duration 15, repetition counts
and raw-RNG jump coefficients are source phase parameters, not measured render
cadence. Preserve separate binary32 operations and explicit linked math services.
Idle Wait has a further register contract: low/unordered ratio paths require the
caller's FPR29, while a distinct FPR0 feeds breathing FMA. Neither zero nor the
previous actor's local register is a valid substitute. Species 4 has its own
180/188 threshold ladder, 6/7/9 distinct breathing coefficients, and 2 skips scale
breathing. Hit handling inspects item slot **zero** for the harmful-item penalty
bypass. One draw of 360000 gates idle reactions; BreakTurn and row-neighbor checks
remain late reads. Positive row offset subtracts twenty seats rather than adding.

Target adjustment adds bias to offset first, consuming opposite-signed delta
there before changing desired count. Desired changes in chunks at most ±1,
recomputing ratio and signed denominators each time. The child recaptures A,
while the parent later clamps its earlier owner; this is deliberately not one
snapshot. Numeric failure does not undo earlier chunks or offset stores.

Window work can capture a different owner from count work. It smooths toward a
fresh count with source asymmetric coefficients/minimum step and overshoot reads.
Its timer wraps as a word before signed clamp 0..15; quadratic position retains
separate binary32 boundaries. This animation state does not establish real-world
timing, native renderer cadence or original Gekko arithmetic parity.

## Original files and native atlas

Resource handles 1/2 own normal/Luigi TPL payloads, not GameCube fileAllocf layouts.
Exact files are 428896 bytes/205 frames and 39776 bytes/19 frames. BE TPL magic is
0x0020af30; each image is 64x64 CMPR format 14 with no palette and 2048-byte data.
Private PMAUD001 is little-endian: 20-byte header and 224 32-byte records for a
1024-square atlas. Fifteen-column packing uses 68-pixel cells, two-pixel insets
and flipped V. Exact UV validation rejects nonfinite/cross-frame values. Complete
local validation precedes publication, so failed reload preserves prior payloads/
readiness. Atlas readiness does not allocate actors or select animation.

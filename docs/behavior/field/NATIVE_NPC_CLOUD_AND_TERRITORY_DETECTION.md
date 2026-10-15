# Retained original NPC cloud and territory detection

The [Main entry binding](NATIVE_NPC_MAIN_ENTRY_AND_CLOCK_COMPOSITION.md) now
composes the [movement](NATIVE_NPC_MAIN_MOVEMENT_SECTION.md) and
[automatic-pose](NATIVE_NPC_MAIN_AUTOMATIC_POSE_SECTION.md) sections with source
8004a160..8004a4e4 and its reached common interrupt8004acc4..8004accc.
The [parent map](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md) and
[child ABI audit](ORIGINAL_NPC_MAIN_CHILD_ABI_AND_NUMERIC_FRONTIER.md) remain
authoritative. Mario and Display are unfinished frontiers, never Main success,
actor traversal, final state10 clear or history publication.

## Actual event identity and class byte

Read actor11c regular event ID. Zero goes directly to Display8004accc without
lookup or interrupt. Nonzero requests actual evtGetPtrID80035a28; null result
goes to Interrupt. Otherwise read returned event8 flags and, if stopped bit2
is set, go to Interrupt. Only then read actor13c script word, followed by event
byte10. This captured class is EventEntry.rawBytes[0], NOT scheduler type+c.
Even a null script still loads and retains the class byte.

The native lookup follows original read order: fresh GP pointer/14 context,
selected field803d8d30 or battle803d8dd0, signed count0 then array90. Scan
ascending1b0 records; allocated byte8 bit1 gates word15c ID comparison. Return
the FIRST matching actual mapped entry address, including stopped records.
Stopped state is a separate parent check. Nonpositive counts return null after
the array read. Native admission requires a resolved mapped array owned by that
VM and positive count<=256; unsupported domains hold, never clamp or report a
missing event. The selected event context can differ from the NPC bank captured
by earlier entry if live GP context changes. No boolean event-exists result,
scheduler type substitution or fabricated event pointer is used.

## Cloud priority and sample capture

A nonnull actor13c script and captured class neither5 nor8 enter the cloud
owner. Capture mainflags once;20000000 or20 bypasses all samples. Otherwise:

- Top: capture root X,Y,Z words in that order; then current width14c.
- Middle: freshly capture Y,X,Z,height150, then source.5 at8041fc80. Compute
  Y=NEGATE(fused32(height*.5-Y)), retaining the source fnmsubs negation.
  Freshly read width14c before the child.
- Bottom: freshly capture Y,X,Z,height150; single Y-height. Fresh width14c.

Each requests cloudGetBreathPower80187f10 with the semantic local XYZ sample
and width. No synthetic guest stack pointer is exposed. The original child
only reads its sample; it does not mutate it or pass its address onward. After
each completed call freshly load source zero8041fc7c and compare power>zero.
First positive power posts trigger8 and enters Interrupt, skipping territory
and later Mario detection. Zero, negative and unordered powers continue in
priority order. Earlier script/class/mainflag captures are not reloaded after
a callback mutates actor or event state; subsequent coordinates/width are fresh.

## Territory bounds and caller-specific norm

Fresh actor1f8 kind nonzero and captured class1/2 enter territory. Kind1 captures
centerZ204,currentZ94,centerX1fc, then currentX8c and sourcezero. Compute rounded
dz and dx, then q=fused32(dx*dx+rounded32(dz*dz)). This differs from automatic
pose selection's separate squares/add. A shared retained inlined norm helper
consumes the completed q; it preserves explicit seed admission, three double
Newton rounds, source canonical-NaN/zero classification and final float round.
Only AFTER that helper completes, load fresh radius214 for BOTH class1 and2.
Strict distance>radius posts trigger4. Unordered distance/radius does not.

Kind2 uses center1fc/204. Class2 halfextents are214/21c; class1 uses208/210.
For each axis capture center, extent, current once; reject strictly below
rounded(center-extent), then strictly above rounded(center+extent). An X
rejection skips all Z reads. Equality is inside. NaN on one axis does not
reject that axis and does not skip the other axis. Do not normalize negative
extents or replace these comparisons with a generalized containment test.
Unknown kinds, no territory, classes other than1/2 and inside results reach
Mario before source8004a4e4. They do not bypass the later avoidance/proximity
logic or invoke Interrupt early.

## Trigger, interrupt and continuation ownership

Posting8 or4 freshly reads actor140, ORs the bit and publishes it once.
Then request actual npcCheckInterrupt800440f8(actor). Missing native cloud or
interrupt providers hold; no fixture no-op is enabled in ordinary Game. The
interrupt result is discarded. After it genuinely completes, reach Display
before the fresh mainflags load8004accc. A callback's subsequent actor mutation
must survive; earlier trigger, cloud, movement, clock and animation effects
never replay across holds. Repeated frontier calls are effect-free.

advanceDetection retains the same actual NPC owner, VM and shared pose aliases
as the preceding sections. It neither creates shadow storage nor converts a
catalog key to a packed pointer. The core/helper allocate no memory while
advancing; native scalar operations use the existing ARMv6K hard-float build.
Original seed/FPSCR/GQR/subnormal parity and physical performance remain open;
the opt-in reciprocal estimate adapter retains its explicit qualification.
Normal Game still uses its existing field path until the rest of Main and
real Mario/collision/gravity/clock/audio/cold resources are bound.

## Evidence and validation

Read-only original helper bodies, ignored under artifacts/npc-main-parent-evidence:
evtGetPtrID96 bytes SHA2562a2d6ccb73423297ece3533d7c6cdd56e4783376654e6218305aa68bbef04614;
cloudGetBreathPower368 bytes SHA2567bf3a8e49328f8b1a54c0ff1b47041b01011e52a8890a6bcaa4f4be70dc60a6b.
Exact load/branch notes: detection-prefix-review-notes.txt. No original body,
ISO, source asset or compiled pack is committed. Existing rights are unchanged.

Tests specify an independent cloud-trigger trace, every-primitive suspension,
all cloud priorities/fresh samples/captured gates, late radius, fused-norm
counterexample, boundary/NaN/short-circuit rectangle checks, unknown kinds,
missing/stopped/zero IDs, child mutation and owner/frontier identity. Actual
VM integration covers first stopped match, inactive skip, duplicate IDs,
raw byte10 versus type+c, current field/battle context, missing ID, held cloud/
interrupt and unchanged parent/MainCount. Fixture children remain explicit.
Existing automatic-pose regression tests retain their separate norm oracle.
See PROGRESS for frozen-tree/full/native validation and rendering limitations.

Next mandatory source work: Mario avoidance/proximity at8004a4e4..acc4, followed
by display/material/shadow, late history, final actor-loop work and field tail.

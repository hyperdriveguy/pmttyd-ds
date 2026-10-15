# NPC Main detection, display, history and field tail

The [detection](../../source/field/field_npc_main_detection.cpp),
[Mario response](../../source/field/field_npc_mario_detection.cpp),
[display prefix](../../source/field/field_npc_main_display.cpp),
[shadow suffix](../../source/field/field_npc_main_shadow.cpp),
[history](../../source/field/field_npc_main_history.cpp) and
[field tail](../../source/field/field_npc_main_tail.cpp) follow
[entry/movement/native composition](npc-main-entry-movement.md).
Their terminal states retain successful operations across holds; they do not
reset automatically. The binding retires actor children after history and the
final clear/next-entry boundary, and resets whole Main only for a new invocation.
Normal Game activation and actual deferred rendering remain separate work.

## Detection priority and source reads

Actor0x11c is a regular event ID. Zero skips lookup and interrupt; a nonzero ID
uses the actual event pointer, with null/stopped results going to interrupt.
Script0x13c loads before the event's raw class byte0x10, even when script is null.
This byte is not scheduler type. A cloud-eligible script/class checks captured
main flags, then samples top, middle and bottom in source order with fresh
coordinates/width. Middle Y uses negated fused height*0.5-minus-Y, while bottom
uses rounded Y-height. Fresh zero after every cloud response makes the first
strictly positive result post trigger8 and skip territory/Mario checks.

Territory kind1 uses a fused horizontal norm and reads radius only afterward.
Kind2 checks X before Z with class-specific extents and strict outside tests.
Equality stays inside; NaN does not reject an axis or skip the other axis.
An X rejection skips all Z reads. Trigger4 or8 freshly ORs actor0x140, then
interrupts. Other eligible paths preserve the captured event class for Mario.
The [detection API](../../include/field/field_npc_main_detection.hpp) carries
semantic copied point words, not fabricated guest pointers.

Mario response has two intentionally different graphs:

| Captured class | Ordering and result |
| --- | --- |
| 2 avoidance | Capture player/actor position, motion-derived facing, then independent eligibility lookup; failure posts2 |
| 1 proximity | Capture position and norm before eligibility; success posts1 |
| 4 inner proximity | Same proximity graph, then inner territory check; success posts1 |
| Other | Common interrupt without proximity/avoidance |

The [Mario API](../../include/field/field_npc_mario_detection.hpp) retains separate
position-source and eligibility Mario pointers. Eligibility reads actual flags,
fresh GP state, sequence, retained Mario halfword and Vivian status. A nonzero
first Vivian result requires a separate second query returning1; calls cannot be
merged. Movement-facing calculation uses previous-minus-current paired XY and
fresh Z, with separate squares/add; player-distance calculation uses a fused norm.
Neither substitutes a new previous-position snapshot.

Avoidance's quarter-range bypass skips all extra checks, including height.
Proximity's quarter bypass skips range/angle only: all admitted proximity cases
still check vertical bounds. Full-range values stay captured while later bounds
are fresh. Angle callbacks and strict comparisons preserve unordered outcomes.
For Kpa-adjusted proximity, all four radius words copy before the level query;
native index outside0..3 holds rather than clamping. Class4 kind1 uses double0.75
and a double threshold product; other kinds use float0.75 and scaled rectangles.
Replacing these with one generic distance/containment helper changes branches.

Missing cloud, numeric, Mario or interrupt providers hold at their reached
boundary; successful calls/stores do not replay. The native reciprocal estimate
remains an explicit adaptation, without Gekko FPSCR/GQR/exception parity claims.
See [cloud/territory evidence](../behavior/field/NATIVE_NPC_CLOUD_AND_TERRITORY_DETECTION.md)
and [Mario evidence](../behavior/field/NATIVE_NPC_MARIO_DETECTION.md).

## Display registration and shadow suffix

The [display API](../../include/field/field_npc_main_display.hpp) distinguishes
Shadow, SeparatePose and History routes. Main mask0x800 goes straight to history.
Otherwise two separate camera4 queries supply view and projection, retaining the
local point between them. Ordered culling and short-circuit literal reads remain
visible. A failed/NaN cull freshly checks force0x40; a successful cull skips that
read. Missing cameras/matrices hold rather than providing identity transforms.

Color bytes inspect sequentially until the first non-ff. Nonwhite sets material
mask0x40, then copies the packed color, reloads handle and writes the copied color;
white clears the mask. These leaves mutate actual shared pose rows, including
mapped inactive slots. Depth uses the source FMA and ordered clamps; NaN survives.
Fresh flags and blink/state tests choose separate pose or ordinary registration.
Alpha chooses opaque classes0/1/2 or translucent class2. Every ordinary queue
record reloads camera0x1f4 while retaining actor/callback/depth.

The [shadow API](../../include/field/field_npc_main_shadow.hpp) binds actor, input
route, exact depth bits and providers. Ordinary shadow copies root, resolves sine
from revised yaw, then reloads the descriptor for fused scaled offsets. Radius
retains the source divide/multiply order, including ordinary1.33. Vivian's downward
ray preserves seven optional output identities and consumes only actually written
hit Y. Missing Y after an acknowledged hit stays held without replaying the child.

Vivian pose receives the copied hit point; its later shadow entry instead reloads
live actor XYZ and omits ordinary1.33. Shadow index0 is valid. Collision disable
changes the shadow flag, not its collision field. A fresh blur flag can enqueue a
separate callback. Separate-pose registration uses literal camera1 and an alternate
handle sentinel; its alpha/callback/class choice stays retained across holds.

Queue registration does not dispatch a callback. Source NaN rejection can be
acknowledged without a queue record; unavailable capacity/provider must hold.
Actual queue generation, camera/shadow pools, producer pass and borrowed lifetimes
survive holds and deferred dispatch. See [display evidence](../behavior/field/NATIVE_NPC_MAIN_DISPLAY_PREFIX.md)
and [shadow evidence](../behavior/field/NATIVE_NPC_MAIN_SHADOW_SUFFIX.md).

## Late history and field-tail ownership

The [history API](../../include/field/field_npc_main_history.hpp) copies raw words:
current X/Y, stores previous X/Y, then fresh current Z and previous Z store.
It then freshly tests motion-history flag0x100 and live MainCount parity. Even
count shifts oldest-first vectors; odd count skips. Every vector preserves the
same scalar read/store order, with fresh current coordinates for the final copy.
NaN payloads and signed zeros survive. History precedes final state clear; hidden
or culled body paths still reach it. See [traversal evidence](../behavior/field/NATIVE_NPC_MAIN_HISTORY_AND_TRAVERSAL.md).

The [tail API](../../include/field/field_npc_main_tail.hpp) runs after the actor
scan. Original GP captures F and effect E=F+0x550; E word0 alone is the effect gate.
Counters increment with wrapping, store, then reload before signed comparisons.
The two animation states preserve their original easing/plateau/cutoff branches
and distinct zero loads. Queue payload retains actual E, camera9 and depth;
clearing E+4 does not clear the gate E+0.

Only encounter mode continues to Kpa bookkeeping. It captures a new F2 before
parent Mario lookup. Two separate type gates surround jump and fire queries;
each child itself gets Mario again. Eligible score retains its signed score and
copied parent XYZ, including original Y+60 before fresh Z. Jump true clears two
bookkeeping words even when score is ineligible; fire false clears its own word.
The final byte0x54d always clears through ORIGINAL F, regardless of later GP/F2
changes. Neighbor bytes stay intact. That clear precedes whole-Main completion;
repeated terminal calls have no effects. See [field-tail evidence](../behavior/field/NATIVE_NPC_MAIN_FIELD_TAIL_AND_COMPLETION.md).

# Retained class drawing and audience touch admission

## Original class draw composition

The queued class callback `8012210c/40` retains its passed battle owner and
completes `BattleActMenu_GetCursor` with menu offset171c/type14. Its lookup
result/output are unused by the source. The wrapper `80116cb8/20` ignores its
passed battle argument: retained DrawMainMenu captures the current global battle
only after the lookup completes. Ignored camera arguments may change during a
wait; held source owners and service identities may not. This is not a native
queue registration or proof that the ordinary game dispatches this callback.

`BattleActIconDrawCursor` retains source row ordering, captured child operands
and acknowledged siblings. Circle/bubble cursors retain every acknowledged
SDK/GX/math call, copied template/XY/UV bytes and actual camera reads. Live
memory reloads occur only at their original source boundaries. Unwritten stack
positions and absent providers remain unavailable. See
[class source drawing](../battle/CLASS_CIRCLE_AND_SPEECH_BUBBLE_DRAW.md) and
[draw continuation ownership](../battle/NATIVE_CLASS_DRAW_CONTINUATIONS_AND_RESOURCE_OWNERS.md).

`resumeBattleClassDrawIcons` composes the outer cursor with these retained
children using a stable heap context and borrowed slot-vector identity. The
caller retains the vector and real raster/memory/math resources throughout a
wait. FontString requires an explicit provider that retains partial glyph/RNG
work. It is never forwarded to the older synchronous backend. Missing ownership
holds at that boundary. Reset abandons native continuation metadata; it does
not undo original side effects or release the caller's resources. The caller
must validate and publish a completed actual batch separately.

Focused tests compare acknowledged effects and guest reads across170 external
wait boundaries, including an emitted glyph prefix followed by a wait. These
are provider fixtures, not proof of ordinary menu entry, original camera
publication, uninitialized-row producers, full HUD or complete GPU behavior.

## Lower-screen audience requests

`BattleAudienceTouchPublication` accepts copied actual completed scene camera,
world quads and source eligibility for one lifetime token/allocation/sequence.
It uses the existing source projection and quad geometry helper. The producer
must include all item-holder geometry that could overlap the target; actual
visibility ownership must resolve other occlusion. Unknown states are not
estimated from seat numbers, item IDs or a fabricated audience.

Touch publication requires a bound independent physical-X alternative. The
renderer admits only the exact frame it actually drew on the lower screen.
Touch-down edges queue one immutable target request, never mutate simulation
or synthesize X. Simulation consumes once after rechecking frame/allocation,
actor/item incarnations, geometry, visibility, harmful and interruptible state.
Replaced owners/allocations/frames discard admission and pending requests;
physical binding loss also removes admission. A rejected edge cannot activate
later after eligibility changes.

The layer is not yet bound to normal audience population/drawing. Required
producers remain actual original camera4, complete source draw publication,
actor/item incarnation lifetimes, proven harmful/interruptible predicates and
visibility/occlusion. Consumption must feed the real selected-slot interruption
owner. The existing independent physical X path remains available; the new
layer does not replace it or claim the user's requested tap flow is complete.

# Same-process retained NPC animation pass

The native animation owner exposes the original `animPoseSetAnim80041170` and
`animPoseMain8003ed2c` children to the retained NPC bindings. These calls use the
same cold VM, native files, pose aliases, source scratch and executor as Mario.
This is an available service binding; normal Game NPC scheduling, original GPU
Draw/Inner, field camera/queue ownership and offscreen GPU work remain separate.

## Admission and lifetime

`beginNpcPass(owner, pass, cameras, current)` admits a quiescent ready owner.
The token is identity only. The caller retains the token, borrowed service,
cameras, VM aliases and complete parent/callback/queue state through all holds.
The predicate must be pure, non-reentrant, independent of animation source
stores and private publication writes, and prove the same immutable producer
and camera pass. There is no timeout, cancellation or fabricated completion.

Entry, Release, Mario presentation, source Main, selection and another NPC lease
block admission. Once admitted, the lease blocks native tick, Entry, Release,
Mario, root changes and presentation reconfiguration, including between child
calls. Identical cameras reuse the configured Main executor; a new NPC pass does
not allocate another executor or pose namespace. Its current predicate admits
either the exclusive Mario request or the exclusive NPC pass.

`endNpcPass` requires the same owner/pass, current predicate and no pending source
child or publication. This is a child-quiescence check: **the caller must first
finish the entire owning callback and drain its queue**. Main completion alone
does not authorize ending a draw pass. A false current predicate leaves the pass
retained until its owner can prove it current again. It does not roll back stores.
The ordinary Game drain currently resumes Mario; a caller of this new NPC API
must resume its own retained NPC parent. Normal NPC activation is still open.

## Routing and original effects

`FieldNpcAnimationServices` has an independent borrowed context, preserving the
other numeric, movement, camera, queue and GPU callbacks' existing context. When
installed, it exclusively owns SetAnim/Main in deferred draw callbacks, and
SetAnim in automatic-pose selection. A false response never falls through to a
legacy handler. With no animation provider installed, existing legacy behavior
is preserved.

Every request checks exact VM identity, Main configuration generation, active
NPC pass/current predicate and absence of Mario/Entry/Release/publication work.
Arguments remain raw original words. Native selector ownership pins all three
SetAnim arguments; Main pins handle/pass. A changed request cannot steal a held
child. The provider calls the retained process directly; the public raw Main
wrapper rejects calls during an NPC lease.

The [source selector](NATIVE_RETAINED_ANIMATION_SELECTION.md) retains directory
loads, signed-selector string/index behavior, force0 equality before the clock,
selection/link stores, original clock reads and ordered reset stores. Automatic
pose publishes actor phase108 only after the child acknowledges. Native Main
runs the original CPU graph and writes real source outputs/quantization state.
Neither call acknowledges original Draw, substitutes an atomic mesh for GPU
Draw, or invents a camera, display registration or bounding box.
Raw NPC Main does not update the typed published mesh. Future field Draw must
consume its actual source outputs under the retained original draw context; an
older Mario/typed shape is not evidence that this NPC pass rendered.

## Validation boundary

`tests/native_npc_animation_tests.cpp` uses actual cold startup and original
Mario resources from the existing ISO-derived packs. It tests clock-unavailable
force0 equality, forced-selection partial stores/repair, changed request rejection,
Main holds/current invalidation, exclusive lease/barriers, executor reuse and
return to Mario. The real retained draw binding holds at missing GPU work after
Main and resumes without replaying Main or material stores. An explicit test GPU
receipt permits checking terminal callback traversal; it is not a renderer.

The same VM/registry fixture traverses actual Main entry and movement prefixes
into automatic idle selection. An explicit gravity fixture bounds that test;
it stops at the named Detection frontier, not full NPC Main. Missing original
literal words come from `door-data.pmevt`; existing cold-process bytes are not
replaced. Raw actor bytes are an owned test setup, not source actor creation.
No new story, message, gameplay constant or Dolphin timing measurement is added.

# Original NPC display prefix and retained native requests

Source `npcMain`8004accc..8004aeac is now an allocation-free retained controller,
composed after [Mario detection](NATIVE_NPC_MARIO_DETECTION.md). It reaches
three explicit unfinished frontiers: Shadow8004aeac, SeparatePose8004b080,
or History8004b10c. None completes Main, advances the actor, clears final state,
or updates history. The [parent control flow](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md)
remains authoritative. Ordinary Area activation and original callbacks remain open.

Fresh mainflags800 skips directly to History. Otherwise request camera4,
transform actual actor+8c by that camera+11c view, request camera4 AGAIN,
then transform the retained local point in place by the new camera+15c
projection. Local point/color values are semantic source stack captures, never
fabricated guest addresses. Providers own actual camera/matrix data and preserve
SDK grouping and homogeneous reciprocal; there is no identity-camera fallback.
The SDK row graph is single add of two single fused results:
`fma(m2,z,single(m0*x)) + fma(m3,1,single(m1*y))`.
Projection retains single1/W and multiplies XYZ; it does not clamp W/nonfinite.

Cull in source order: ordered Z>-1, Z<0, X>=-2, X<=2, Y>=retained-2,
Y<=retained2. Short-circuit literal reads remain visible. Any failure including
NaN rereads mainflags; force40 continues, otherwise History. Successful cull
skips that force read. Constants come from unchanged read-only original spans.

Color checks bytes114..117 sequentially, stopping at first non-ff. Nonwhite
loads pose104, sets material flag40, THEN loads/copies word114, reloads pose104,
and writes that COPIED color through the color leaf. White loads pose104 and
clears flag40. Native leaves use the same actual AnimationPoolMemory pose row:
e8 OR/AND and f0 copied color. They deliberately add no active/resource gate;
the source getter is arithmetic. Unsupported handle/alias domains hold without
effects. The row is not a detached renderer snapshot.

Depth loads5000, retained projectedZ and fresh zero, then single fused
`fma(5000,z,5000)`. Negative values clamp to zero without loading10000;
otherwise fresh10000 clamps only ordered greater. NaN survives. Fresh flags,
fresh-1, single depth multiplication precede flags80 -> SeparatePose. Otherwise
half320 bit2 reads state1d4 only when set; absent10000 -> SeparatePose.
Fresh alpha117 selects xlu8004c7e0 class2, or opaque8004c50c classes0,1,2.
Each dispEntry reloads camera1f4, retaining actor, callback choice and depth.
All ordinary registrations lead to Shadow. Shadow/history/separate pose are
not skipped even when ordinary registrations finish.

Every empty/false primitive has no effects. Cursors capture successful reads,
calls and stores across holds, bound to actor and service owner. Native camera,
SDK and queue callbacks must preserve the SAME producer pass, mappings and
queue generation through holds and deferred dispatch; changing/resetting them
violates the provider contract. Missing providers remain Pending. Source NaN
priority rejection is acknowledged without a record; queue capacity/domain
unavailability holds. Existing DisplayQueue is used by fixtures; native Game
still needs a real field camera/pass/callback owner. The battle callback camera
bridge has different epoch admission and is not used as a field fallback.

Tests independently enumerate the white-path trace, hold every primitive in
white/nonwhite branches, change handles/colors after material flags, retain
opaque branch/depth while each queued camera changes, exercise ordered bounds,
NaN/force, clamps/negative zero, skip flags/half/state, owner identity and all
unfinished frontiers. Native shared-VM tests use an inactive pose255 and prove
material writes are not replayed after a held actual DisplayQueue registration.
Camera/matrix responses in these tests are explicit fixtures, not measured game
camera output or a complete rendering acceptance result.

Evidence: ignored artifacts/npc-main-parent-evidence/display-prefix-review-notes.txt,
helper listings and native validation artifacts. SDK PSMTXMultVec8029e6a4
size54hex SHA2560e86b49668bd9172bfd1256aa2842a4326032efab48cb6b8df58523a63992e78;
PSMTX44MultVec8029eb08 size78hex
SHA256a9ff54d2a86e2d747391bd563743295604a932ccc380d95c320b983d85df395b.
These are static original-source observations, not Dolphin execution traces.

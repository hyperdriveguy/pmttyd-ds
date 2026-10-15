# Native prior-scan NPC radial hint

Original US G8ME01 static evidence: [encounter scan](FIELD_ORDINARY_NPC_TALK_CANDIDATE.md),
[packed wander consumer](ORIGINAL_WANDER_EVENT_PACKED_BINDING_FRONTIER.md) and
[complete Main parent](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md). This change
corrects the active native catalog-based Game path. It does not activate the
retained original Area VM or turn catalog keys into source actor addresses.

## Publication and consumption

The scan retains two separate values: the final talk candidate and the radial
hint. The latter may be updated before facing or obstruction rejects an actor;
it is not an independent nearest-distance reduction. NpcTalkScan already
implements that source distinction. At the end of an admitted native encounter
scan, Game now retains scan.nearActor separately from its talk candidate.
An admitted scan with no radial actor publishes zero. A skipped scan preserves
the prior hint, including field-key/control gates. Catalog generation or room
retirement clears it with the retired native actor instances; checkpoint
restoration starts with no transient hint. These are native lifetime boundaries,
not a claim that the source resets Mario+1d8 at those exact call sites.

Game visits regular wander before its current movement/collision/encounter scan.
NpcWanderMario.contact now receives the prior radial publication. That field
represents marioGetTouchNpcPtr80096948 reading Mario+1d8; Mario+1dc is a distinct
side-collision pointer. fieldNpcContact remains the native side-collision result.
The pure wander driver and packed event implementation remain independently
usable by future areas through their existing services and actual registry.
No rendering callback computes or mutates this hint. The correction adds one
retained32-bit value and one assignment per admitted scan, without a new scan,
allocation or GPU submission.

## Angle boundary used by facing and wandering

Source reviseAngle80007328/60 calls double fmod, rounds to binary32, replaces
NaN with positive zero, and adds360 for negative results. Its fadds80007364 may
round a tiny negative remainder up to360; comparison80007368 and guard80007374
then replace that result with positive zero. npcReviseAngle now retains that
last guard. Negative zero remains negative zero. This is a source control-flow
correction, not a general Gekko/libm numerical-parity claim.

Read-only inspection: tools/inspect_dol_function.py reviseAngle using
artifacts/reference/us_symbols.csv and the original ISO. Body SHA256
e44664bf511be1e4b10e4f0b3e40004fbd1d412c40ce6b34fee14987db320ed5.
Ignored listing and inventory: artifacts/npc-main-parent-evidence/.

## Regression scope and remaining work

field_npc_radial_hint_tests loads the original Goomba4 catalog and gor_02
collision data. It checks radial-only proximity with no side collision, prior
hint use by a deliberately configured single wander input visit, skipped-scan
retention, completed empty publication, catalog retirement and checkpoint
restoration. The event-input fixture explicitly changes phase/count/heading and
pause state; it is not an observation of authored story scheduling.
npc_presentation_tests covers the rounded360 boundary, negative zero and NaN.

The complete original npcMain clocks, event detection, actual packed Mario
publication, source iteration/snapshot and display services remain separate
integration work. Existing native Game traversal, clocks and catalog ownership
are adaptations. Neither these tests nor an emulator boot establish original
story, pixel, frame-timing or physical New2DSXL performance parity.

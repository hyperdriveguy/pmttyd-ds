# OW-WALL-01 — Swept directional movement and sliding

**Status: OPEN (wall contract delivered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Filed by the native overworld implementation session, 2026-10-12, and moved here from `OVERWORLD_DOCUMENTATION_REQUESTS.md` during the inbox merge; request text is preserved verbatim. The documentation slice was explicitly requested; no independent research-agent ownership or full-game completion claim is made. Updated 2026-10-12.

## Open todos

- [ ] Full `hitCheckVecFilter`/triangle intersection and query traversal
  ordering, floating-point exceptional boundaries and scene/native registry
  identity.
- [ ] +14 view flag writers (direction8/2), dynamic NPC hidden-block owner
  resolution, balloon contact selection and remaining wall-contact counter/field
  consumers.
- [ ] Vivian/roll/Paper/motion25/26/28, map-specific bypass/stationary stage and
  dou10_yoko_yari hazard-helper execution/reachability.
- [ ] Original isolated collision traces: oblique walls, approach corners,8/2
  view, side guard final-height ownership, falling correction cap, cull/filter
  edges, and frame-correlated root/contact/position updates.

## Request text (verbatim)

Remaining documentation requests:

- Full hitCheckVecFilter/triangle intersection and query traversal ordering,
  floating-point exceptional boundaries and scene/native registry identity.
- +14 view flag writers (direction8/2), dynamic NPC hidden-block owner resolution,
  balloon contact selection and remaining wall-contact counter/field consumers.
- Vivian/roll/Paper/motion25/26/28, map-specific bypass/stationary stage and
  dou10_yoko_yari hazard-helper execution/reachability.
- Original isolated collision traces: oblique walls, approach corners,8/2 view,
  side guard final-height ownership, falling correction cap, cull/filter edges,
  and frame-correlated root/contact/position updates.

## Answers and delivery notes (append-only history)

Delivered [ordinary wall movement contract](../behavior/overworld/OVERWORLD_WALL_MOVEMENT.md)
and `tools/verify_overworld_wall_movement.py`. Static source contract closes
ordinary forward ray heights/length, closest Qdistance/normal selection, whole-
vector half tangent, ordered +/-45 side guards with second-pass last-token tail,
wrapper pass-through/contact/counter ownership, stationary disposable queries,
and two around-correction passes with view8/2 direction count, ordinary/fall
correction cap and second-pass dot/half policy. Complete ordinary attribute
filter, hidden-owner string dependencies and cull1 dot>=0 rejection are pinned.

Important native correction: player movement must no longer use X-then-Z endpoint
circle rejection. Full source triangle/cull/attribute ray provider is needed;
legacy `.walls` near-vertical filter is not the original complete query domain.
Retain original world-unit quantization before adapting geometry scale0.01.
An adapted host ray/trigonometric/norm service is not proven numerically identical.

Verifier reads the US ISO read-only, pins eight containing bodies,45 instruction
anchors and17 data spans (14 scalar constants/three names). Inspection listings/
ELFs stay ignored or in/tmp. No native code, assets, emulator profiles or saves
were changed by this documentation delivery.

Cross-session validation breadcrumb (2026-10-12,event constructor session):
current make test fails the core overlap-floor landing assertion at
`tests/core_tests.cpp:543` during field-wall/ray integration;expected grounded
height0/sourceNode2. New event_creation and all other CTest suites pass.
Please reconcile this case with the recovered wall/side-ray/contact ordering
while completing the active OW-WALL-01 native changes. No field code was changed
by the constructor session. Exact failure is also recorded in docs/PROGRESS.md.

Cross-session validation follow-up (2026-10-12,event descendant session):
current full make test passes29 CTest suites,including core and field_wall,
plus58 Python tests. The previously reported core overlap-floor landing failure
is cleared in the current worktree. Concurrent field source changes remain owned
by the overworld session;no field code was changed by the event session.

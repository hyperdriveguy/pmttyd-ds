# INPUT-01 — Remaining native adaptation and WorkInit byte clarification

**Status: OPEN (partially answered).** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md); validated deliveries and locks are registered in the [delivery log](DELIVERY_LOG.md).
Request text below is preserved verbatim from the inbox; answers and implementation breadcrumbs are append-only history. Updated 2026-10-12.

## Open todos

- [x] WorkInit byte map clarification — answered (exact
  snapshot-clear/preservation map delivered; see the
  `WorkInit exact snapshot clear and preserved-byte map` row in the
  [delivery log](DELIVERY_LOG.md)).
- [ ] `PADRead`/`Clamp`/`Reset` service behavior and supported errors.
- [ ] GP frame-rate parameter/writers, motor/control/chord ownership and
  pause/physical poll boundaries.
- [ ] Native C-stick/trigger mapping and sequence/display/event registration.
- [ ] Start/impact schedules and the full event-credit loop; provide original
  identities/traces separately from native fixtures (one input projection per
  simulation tick, rate 60 and zero unused channels are adaptation inputs).

Do not invent extra samples for script invocations or suppress inactive
private sampling.

## Request text (verbatim)

Implemented the physical producer and12-byte battle snapshots in
`source/runtime/physical_input.cpp`/`source/battle/battle_defense.cpp`. Native battle now follows
general pads→sequence adapter→private pad/AC main→later event adapter. Four
general buffers and the independent private buffer retain ownership;physical
state persists across scene/battle initialization. Abstract host callers may
still inject already-produced logical GP masks rather than raw levels.

Please clarify the older WorkInit summary:original listing0x8011a3d0..3f4 clears
snapshot bytes0..a but has no byteb store. Implementation preserves each old
byteb on WorkInit and both trailing bytes on update,while clearing bytea on init.
Pin that exact map in the contract/verifier before describing full-snapshot
clearing. The native snapshot bytes start zero via construction/battle allocation.

Remaining:PADRead/Clamp/Reset service behavior and supported errors;GP frame-rate
parameter/writers,motor/control/chord ownership,pause/physical poll boundaries,
native C-stick/trigger mapping and sequence/display/event registration. Native
uses one input projection per simulation tick,rate60,zero unused channels and no
motor service;these are adaptation inputs,not observed GC polling or connection
counts. Start/impact schedules and the full event-credit loop remain unbound.
Do not invent extra samples for script invocations or suppress inactive private
sampling. Provide original identities/traces separately from native fixtures.

## Answers and delivery notes

No answer paragraphs were recorded in the inbox section for this request yet; validated deliveries (including the WorkInit clarification) are registered in the [delivery log](DELIVERY_LOG.md).

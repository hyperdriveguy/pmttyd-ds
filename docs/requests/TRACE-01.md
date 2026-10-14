# TRACE-01 — Original opening-encounter execution trace (demo validation oracle)

**Status: OPEN.** Part of the [request inbox](../DOCUMENTATION_REQUESTS.md);
validated deliveries and locks are registered in the
[delivery log](DELIVERY_LOG.md). Filed by the native implementation session,
2026-10-13, for the first demo milestone ([milestones](../MILESTONES.md)); full
game playability remains the end goal. The slice is explicitly requested; no
independent research-agent ownership or completion claim is made. This request
is for **original execution observation**, which the inbox keeps distinct from
static deduction.

## Open todos

- [ ] A matched original execution trace of the opening sequence (new file →
  harbor arrival → Goombella/Crump encounter → tutorial battle → field return),
  kept distinct from static contracts.
- [ ] Input boundaries and action-command timing at the tutorial impact frames —
  the input sample actually included at impact — in simulation ticks.
- [ ] The branch taken and the story/flag/save writes observed at scene
  completion.
- [ ] Capture provenance (emulator build/version, ISO revision, save state, input
  script) so the trace is reproducible.

## Request text (verbatim)

Native implementation source audit / documentation request (2026-10-13), first
demo milestone: the completion assessment
([US completion](../behavior/meta/US_COMPLETION_AND_PRIORITIES.md)) states that
"no matched original-game gameplay trace establishes attack timing or end-to-end
equivalence", and its P0a row asks for an "original branch/re-entry trace" for
the opening graph. The native demo cannot be validated against static contracts
alone: `source/runtime/game.cpp` still uses a development impact tick48 and samples input
before impact evaluation, and [AC-01](AC-01.md) records that "an isolated original
input/state trace is still needed", as do [INPUT-01](INPUT-01.md) and the OW-*
requests for input and collision traces. Please capture a matched original
execution trace of the opening encounter — input boundaries and action-command
timing at the tutorial impact frames, the branch taken, and the story/flag/save
writes at scene completion — with capture provenance, kept distinct from static
deduction. This is the oracle D1 acceptance depends on and the first instance of
the whole-game trace corpus G1 needs. No lock reserved; the integration contract
is [DEMO-01](DEMO-01.md), the result side is [RESULT-01](RESULT-01.md).

## Answers and delivery notes (append-only history)

No delivery yet.

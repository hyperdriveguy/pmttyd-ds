# Documentation agent requests from the native implementation

Updated 2026-10-12 (America/Boise). The user requested using files in `docs/` to communicate with the documentation agent and leave breadcrumbs. This file is the **index**; request bodies, answers and history live in [`requests/`](requests/). Static deductions and original execution observations must remain distinct. Do not change native harness choices into original-game evidence.

## How to use this inbox

- Pick a request in the status table below and open its file in `docs/requests/`.
- Append answers, deliveries and breadcrumbs **to that request file** (new paragraph at the end of its history section); do not append them to this index.
- Keep the request text untouched; flag remaining work as `- [ ]` checkboxes under the request's "Open todos".
- Register validated deliveries and locks in the [delivery log](requests/DELIVERY_LOG.md), then mark the matching todo answered here.
- A portable helper alone does not answer its full runtime binding request; a partial answer leaves its parent request open.

## Request status

| Request | State | Open work (see file for full todos) | File |
| --- | --- | --- | --- |
| AC-01 — Opening Crump encounter and Jump callers | Open (partially answered) | REL entry/selection, Jump state writers, pad cadence vs private AC pad, seed/RNG writers, original impact trace | [requests/AC-01.md](requests/AC-01.md) |
| AC-02 — Ordinary Hammer binding and presentation | Open (period/base difficulty answered) | Suppression/automatic/forced writers, result/damage consumers, display/gauge and sound-key macros | [requests/AC-02.md](requests/AC-02.md) |
| AC-03 — Defense callers, pad cadence and eligibility | Open (attack graph/target ABI answered) | Global edge/repeat producer, snapshot bindings, pad update order/pause cadence, +0x13 gate, original input traces | [requests/AC-03.md](requests/AC-03.md) |
| DMG-01 — Weapon power, unit stats and damage dispatch | Open (power callbacks/part tables/calculators answered) | Allocation-copy/init consumers, full consumer graph, multi-hit timing and input/RNG traces | [requests/DMG-01.md](requests/DMG-01.md) |
| DMG-02 — Direct application service internals | Open (position queries/movement sound answered) | Parts/hit/height/belong schemas, sound pointers, action-record/audience state, rounding/order traces | [requests/DMG-02.md](requests/DMG-02.md) |
| STATUS-01 — Status lifetime, turn ownership | Open (clear/import/export answered) | Entry initialization, event/turn cadence, +20/+23/+24 writers, multi-turn traces | [requests/STATUS-01.md](requests/STATUS-01.md) |
| INPUT-01 — Remaining native adaptation, WorkInit bytes | Open (WorkInit byte map answered) | PADRead/Clamp/Reset, GP writers/chords, poll boundaries, impact schedule, credit loop | [requests/INPUT-01.md](requests/INPUT-01.md) |
| EVT-01 — Native event scheduler and dispatch closure | Open (manager/constructors/opcodes partial; 4 sub-asks answered) | Complete evtmgrMain/evtmgrCmd contract, audio chain, Monosiri, weapon-address writers, NormalJump writers | [requests/EVT-01.md](requests/EVT-01.md) |
| JUMP-01 — Ordinary Jump follow-up and per-hit event flow | Open (SFX provenance answered) | Complete event 80355cf0 control flow and original timing/state traces | [requests/JUMP-01.md](requests/JUMP-01.md) |
| FIELD-RECOVERY-01 — Water/void stages, pose domains, pipe paper (field/AC-03) | Open | Water/void numeric stages, pose table slice, pipe paper lifecycle | [requests/FIELD-RECOVERY-01.md](requests/FIELD-RECOVERY-01.md) |
| MAINLOOP-01 — marioMain per-retrace body (P0c) | Open | Motion dispatch/subsystems/input gate/battle transition, marioStMain pause cadence | [requests/MAINLOOP-01.md](requests/MAINLOOP-01.md) |
| SAVE-01 — cardMain/swSet persistence (P1b) | Open | Save format/state machine, story-switch bank, round-trip namespaces | [requests/SAVE-01.md](requests/SAVE-01.md) |

Overworld-specific requests are tracked separately in [OVERWORLD_DOCUMENTATION_REQUESTS.md](OVERWORLD_DOCUMENTATION_REQUESTS.md).

## Implementation priorities

Use the scoped lock table in the [delivery log](requests/DELIVERY_LOG.md)
when claiming work. A portable helper alone
does not answer its full runtime binding request.

| Priority | Request / implementation blocker | State / owner / reserved slice |
| --- | --- | --- |
| 0 | EVT-01: complete native event scheduling and continuation | Open; needed to replace provisional Jump timers with the recovered graph; request file specifies remaining complete local contracts |
| 1 | DMG-01: real weapon power, part defenses and dispatch/application | Power bodies/operands and scoped part construction/table/lifetime delivered by ID2;level/bonus/AC producers,mutation and application binding remain open |
| 2 | AC-03: enemy hit branch and actual defense sampling boundary | Attack graph/target ABI delivered and lock released; pad cadence,global eligibility and execution remain open |
| 3 | AC-01: tutorial/ordinary Jump state ownership and impact cadence | Shared enemy-attack graph delivered; next recover pad/update and movement callback composition |
| 4 | AC-02: Hammer lifecycle, result consumers and original audio binding | Open; ordinary weapon period/base difficulty already answered |
| 5 | STATUS-01: initialization, infliction and turn lifetime | Open; portable storage alone does not establish turn cadence |

The ordering serves the early original encounter's implementation. It does not
remove the full game's field, partner, story, inventory, save, audio, rendering
and optional-content obligations. Do not insert provisional native timing into
an original contract. Complete-game requirements remain deferred until system
coverage is adequate.


## Implementation coverage and next work

Three requests were previously grouped under this heading: [INPUT-01](requests/INPUT-01.md), [EVT-01](requests/EVT-01.md) and [JUMP-01](requests/JUMP-01.md). Their append-only delivery history is kept in [EVT-01_HISTORY.md](requests/EVT-01_HISTORY.md) and [JUMP-01_HISTORY.md](requests/JUMP-01_HISTORY.md).

## Related files

- [Delivery log and lock protocol](requests/DELIVERY_LOG.md) — validated deliveries, owners and lock rules.
- [US completion assessment and priorities](behavior/meta/US_COMPLETION_AND_PRIORITIES.md).
- [Behavior documentation register](behavior/README.md).

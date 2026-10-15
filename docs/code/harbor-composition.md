# Harbor tutorial composition

[HarborTutorial](../../include/battle/harbor_tutorial.hpp) is a scoped native
composition of original setup2 events and admitted battle services. Its
[implementation](../../source/battle/harbor_tutorial.cpp) does not infer story
outcomes from HP. Read [packed storage](harbor-packed-state.md),
[service adapters](harbor-service-adapters.md) and
[underlying battle controllers](battle-control.md) alongside it.

## Catalog and identity

The [converter](../../tools/harbor_tutorial_assets.py) writes `PMHTUT01` metadata
as little-endian words. The event pack itself retains big-endian guest data.
The loader validates all 12 catalog pointers, unique function tokens, callback
indices below 37, exact EOF, Mario kind/part access and six original tutorial
message IDs. It publishes the new catalog only after original menu resources
also load. Global and `gor_00` US message bytes/control tokens remain original.

| Catalog index | Meaning |
| --- | --- |
| 0 / 1 / 2 | Crump init / phase / wait event roots |
| 3 / 4 | Spawned helper init / wait roots |
| 5 | Original spawned-helper setup record |
| 6 / 7 | Crump / spawned-helper kind records |
| 8 | Setup2 opponent unit record |
| 9 / 10 | Original battle group / stage records |
| 11 | Mario pose table |

Runtime registry IDs are system 0, Mario 1, Crump 2 and helper 3. Visual array
indices are Crump 0, Mario 1 and helper 2; the invisible system has no visual
slot. `actorById` handles system separately. Fallback actor selectors `-2` and
`-3` mean source event owner and Mario before translating to a visual slot.
Never index the visual array directly with a registry ID.

The runtime owns a VM at guest GP base `0x81000000`, a native 1MiB heap in
`0x82000000..0x82100000`, rate word 60 and battle context 1. Native time is
monotonic milliseconds converted through the source 243MHz clock convention.
These are scoped storage/scheduling choices, not measured source scheduler or
Gekko math parity. The platform arithmetic callbacks keep distinct operation
boundaries; flush/system-call hooks here are no-ops with limited scope.

## Entry, events and projection

`begin` is a legacy bounded entry. `beginFresh` and `beginFromPouch` additionally
install original pouch context and known story state, prepare source field
battle information and run Mario's kind init event. A supplied pouch is copied;
HP/FP resources import through existing source resource rules. The source entry
order is system, Mario, then setup2 opponent; helper creation occurs later from
its original spawn callback. Complete original UnitEntry/equipment/display and
whole-battle lifecycle ownership remain separate work.

On a callback's first visit, the runtime records function/command/owner and raw
operands. Packed service dispatch takes precedence over converter-index
fallback. The scene publishes selected fields before admitted work and captures
selected results afterward; neither operation is a whole-memory snapshot.
Unknown callbacks return source wait 0 and retain the first pending function
plus event ID. Missing work never becomes an invented source success.

Fallback indices come from the converter's service list:

| Indices | Admitted fallback meaning |
| --- | --- |
| 0–4 | Install wait/attack/damage/confusion/phase scripts |
| 5 / 6 / 7 | Battle flags / guarded helper spawn / unit-work write |
| 8–9 | Position/home XYZ |
| 10–11, 17–18, 28 | Retain original sound/camera requests for presentation |
| 12–16 | Start wait, turn query, phase query, fresh wait stop, direct pose |
| 19–22 | Per-event interpolation init/sample, rotation, live-event wait |
| 23–26 | Status window, work query, talk/stay poses |
| 27 / 29 | Original message playback / actual animation-completion wait |
| 30–36 | Table pose, max/current HP, success/failure counts, table pose, max moves |

The request-only presentation leaves are limited adapters. A recorded audio or
camera request is not proof of original playback or rendering. The fresh wait
stop fallback admits the particular self-deleting wait-script context; it is
not general cancellation of arbitrary source wait loops.

Message opening captures its original ID and speaker once, then waits on real
`MessagePlayback`. Animation wait first delays one visit, then requires an
actual completion acknowledgement whose generation equals current `poseAt`.
A new pose invalidates prior completion. Native scan bounds (80 pose rows,
256 guest string bytes) are safety/admission policy, not observed original
maximums. Interpolation state is per event ID and advances on source visits,
not GPU frames; rotation sentinel `-1000000000` leaves that axis unchanged.

Phase, installed attack, confirmed player attack and hit roots have explicit
entry APIs. Initialization waits for actual source init-event retirement.
Opening completion requires phase-root retirement without a hit interruption.
Hit dispatch observes roots canceled by source hit work and retires diagnostics
only for owners that actually vanished. State `0x06000006` and battle flag 4
select the confirmed attack route; source status can suppress root creation
after successful registration. No replacement root is fabricated.

## Action and display boundaries

[The action adapter](../../source/battle/harbor_tutorial_action.cpp) separates
general history at `B+0xf2c` from private command pad `B+0x1d1c` on channel 0.
A physical simulation sample gates native action work; a repeated screen draw
is not another command tick. Only the original TimingA Main/Result/Delete and
Display callback tokens are bound here. Difficulty is 0–6. Missing input stays
unavailable when the actual result evaluation needs physical history; automatic
source branches can bypass that requirement. Delete requests state 1000;
TimingA Main owns later cleanup, including its four callback clears.

TimingA display preflights camera 8, callback `0x8012a7cc`, native Battle userdata
and original icons 108/109 before executing admitted display work. It collects
icon requests rather than submitting GPU work. Complete battle Main cadence
and generalized suspension ownership are not established by this scoped bridge;
concurrent retained-manager work needs validation in its own committed snapshot.

A class-menu draw binding keeps its VM/runtime alive but still proves current
allocation ownership before each dispatch/resume: both the fresh lifetime token
and live `B+0x1c78` must agree. A completed cursor lookup can have no output;
that is distinct from an unavailable lookup. A reused guest address alone does
not admit an old draw. `classMenuPresented` is explicit UI acknowledgement at
`W+0x14`; creating resources or registering a queue does not acknowledge it.

Menu resource Init/End have explicit reference-count boundaries. VM destruction
and rendering do not synthesize End. Command display palette memory belongs to
a process owner reused across battles; reattaching the same owner is allowed,
replacing it is rejected, and entry/rendering must not reset source color history.

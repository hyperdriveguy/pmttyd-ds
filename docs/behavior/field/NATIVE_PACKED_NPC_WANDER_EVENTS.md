# Native packed NPC wander events

The retained [NPC setup bridge](NATIVE_REGISTERED_NPC_SETUP_WORLD.md) now
implements original urouro_init_func80237878 and urouro_main_func802372a8
through actual event/actor/map memory. The [packed source contract](ORIGINAL_WANDER_EVENT_PACKED_BINDING_FRONTIER.md)
and [four-phase graph](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md) remain authoritative.
This does not replace NPC Main, move roots, run an approximate second actor
simulation, or complete normal Game activation/rendering.

## Original memory and ordered services

Init resolves name/actor before remaining float/flag operands. It captures each
argument only at its source point, reads actual8041e808 and requests44 map
bytes. It publishes the returned token through actual EventEntry+c4/LW10,
reloads it, writes flags/phase/count/flips, then makes the6000/9000/1500 live
frame-budget requests with interleaved stores. Anchor Y is a fresh actor read
only afterward. Radius, speed and main flag00400000 follow source order.
It returns2; it adds no pose, component or root writes.

Main freshly resolves actor, actual Mario pointer, LW10 and phase on each
source invocation. It implements all four phases and every original continuing
exit returns0. Budget reads occur separately before/after shared RNG requests;
yaw is read after RNG and read back after publication. Phase1 retains its
initial distance while making separate reached paper-readiness, touch, angle
and comparison requests. Count decrement and exact-zero phase2 publication
precede component/sound work. Sine and cosine each receive a separately
computed radians input from fresh movement heading. Sound/material queries
precede fresh speed/period/elapsed reads; sound-name lookup and full original
3D sound request precede elapsed reset, fresh toggle and final dt reads.
Idle flips affect visual yaw only. Source unsigned count wrapping and signed
half-budget/multiple20/remainder shapes are retained.

`FieldNpcWanderRequest` carries original callee identity, captured ABI words
and float arguments; `FieldNpcWanderResponse` separates integer and numeric
results. Trig results are double until source frsp. Source binary32 local
operations, including final fused1000*dt+elapsed, remain explicit. Recovered
immutable numeric constants are compiled into the portable body; this native
domain does not model mutation of those original constant locations. Numeric
fixtures do not prove Gekko/FPSCR or original SDK/libm parity.

## Suspension and source return0 are distinct

Cursors belong to the actual event pointer/ID, original function, facade entry
address and provider owner. A failed request has no effects; its captured
inputs and prior stores survive. Completed allocations, frame requests, RNG,
count decrements, component stores and audio submissions never repeat on retry.
Successful services may mutate admitted memory; later source reads observe it.
Stable actor/work mappings and caller lifetime remain mandatory through a
held invocation. Removing mappings, recycling actors or retiring a caller
inside a service violates that admitted lifetime domain.

Genuine main0 clears its cursor. The next scheduled invocation reloads the
source inputs and progresses another phase visit. An unavailable service0
retains the cursor within the same visit. Init2 clears it as well. Area dispatch
uses a stack cursor for a completed visit; only unavailable work retains a map
node. Actual event/room retirement discards cursors before their bindings.
No per-entry map free or automatic init replay is added. Ordinary scheduling,
StopID/StartID credit and interrupt replacement remain the source VM's owners.
Normal world activation still needs all production providers and correct outer
producer/consumer ordering; pending native resource services are not proof of
source timing or movement parity.

## Real retained native owners

The bridge reuses the actual actor resolver and checked operand router from
[actor leaves](NATIVE_NPC_ACTOR_EVENT_LEAVES.md). It accesses actual pose handles:
paper readiness reads flags0/retained74/target7c; material flags read e8, never
the separate light word ec. Mere loaded clips do not invent readiness.
Live GP+4 supplies frame conversion; numeric setup and wandering share the same
externally retained RNG callback. No per-actor seed, render-frame timer or
additional RNG history is created. Shared SourceSoundFlags supplies its actual
value when installed; missing sound, math or audio providers hold.

`BattleSequenceBinding::allocateMap` admits only its exact VM facade, attached
retained storage, cache/syscall providers and the existing complete ordered
header/bounds/free-block check. It then calls real eventMapAllocate, whose44
request produces a64-byte zeroed/flushed payload and split header. This is the
shared primary map arena, distinct from the SDK event-entry heap.
`ownsMapAllocation` verifies a live used payload and bounded ordered headers in
that exact retained owner/facade before admitting work. Foreign facades, free
or unowned work do not become successful placeholder buffers. Existing native
arena address/size policy is explicit; it is not original allocation identity.

Mario requires the actual packed8041e900 owner plus explicit admission callback.
The first pointer is retained for root/travel reads. Touch independently loads
fresh8041e900 and **+1d8 radial hint**, matching marioGetTouchNpcPtr. Native
Game's collision contact+1dc is a different owner and is not substituted.
Current semantic Mario fragments do not yet provide this packed ownership;
normal integration holds until the real shared producer/facade exists.

## Validation and remaining integration

Direct fixtures use original15-row setup, registered actors/poses, actual root
constructors/scheduler and the real bound map arena. Remaining unbound init
roots are explicitly retired by the setup fixture, not claimed executed.
The actual Goomba4 regular wrapper performs source get-position/init and holds
at missing Mario. Explicit admitted packed Mario test storage then permits
phase0 and real component publication with roots unchanged. StopID/StartID
retains the same work and pauses/resumes count. Deleting the entry leaves map
allocation live, as source does. The actual Borodo wrapper retires without
allocation at story215, and outside the inclusive guard (262) performs source
allocation/init/main with a distinct work block and shared RNG.

Portable checks exercise all phases, outside/inside radius, contact/wall turns,
count transitions, readiness gates, service-visible budget/yaw/heading/audio
mutation, and held allocation/frame/RNG/math/audio requests. Test math/Mario
providers are explicit fixtures, not a completed original Mario Main or SDK
numeric implementation. Host/native/ARM results are recorded in PROGRESS.

Still required: genuine packed Mario/radial hint, complete math/audio routing,
NPC Main dt/history/component consumption, collision/gravity, automatic pose/
facing, candidate/interrupt lifecycle, shared world activation and actual NPC
GPU/pixel/physical3DS performance validation. Source story guards, original
radius70/speed30 and original event order remain in the original scripts.

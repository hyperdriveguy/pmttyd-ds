# Native atomic mesh publication and field simulation barrier

`AnimationStartupProcess` now owns `AnimationMeshPublication` beside the
[retained original Main executor](NATIVE_RETAINED_MAIN.md). Normal native field
Mario uses this owner. Main's completed source buffers feed a private reusable
mesh candidate; the published shape changes only after capture, decoding,
assembly and final ownership checks succeed. This adapter is a fresh post-Main
consumer. It does not implement original Draw/Inner load order, and it is not a
receipt of Main's earlier captured output registers.

## Retained source pass and fresh capture

Reserve the exact native handle, pass, generation, resource and scratch sidecar
before Mario selection/setup. Source Main executes once. Its completion advances
the owner to a permanent source-complete boundary and returns pending; failures
in subsequent phases never restart Main or repeat its GQR writes/cache stores.
Each resume retains the phase, scalar cursor and completed assembly state.
Wrong handle/pass, stale lifetime/current predicate or missing memory holds at
that boundary with the previous mesh intact.

After Main returns, the adapter reads, in order: global W, model table, R10 model
index, model entry and holder, MODEL, R14 selection, We8 output selector, five
MODEL counts (f0/f8/13c/140/12c), five output pointers, then R3c/R2c/R40/R24
sample diagnostics. Positions/normals select Wf8/Wfc when We8 is nonzero;
otherwise they use R4c/R54. Visibility, nodes and textures use R5c/R64/R6c.
This deliberately observes post-call redirection rather than preserving Main's
old output pointer captures. Captured metadata is not silently recaptured on a
retry. Diagnostic float bits are preserved; decoded mesh numeric lanes must be
finite in this native admission domain.

Static assembly is admitted only when selected original resource dimensions and
live MODEL geometry agree. `NativeFileCache::modelGeometryCurrent` proves that
the VM mapping uses the actual file payload, checks immutable original resource
identity, relocated roots, header and all relevant static geometry/material/UV/
shape/index/group/directory name tables. Timeline pointers/deltas and base
position/normal/visibility/node arrays are excluded: null timelines and dynamic
output buffers remain valid. This is a trusted converted-resource relationship,
not a second source geometry interpreter. Short RAM leases never escape the
check. Static closure is rechecked immediately before publication.

Decode order is positions XYZ, normals XYZ, visibility bytes, node floats, then
texture selector and five transform floats per 24-byte record. Mapped global
buffers and linked poses are supported; no private-output/nonalias restriction
is imposed. A failed read retains the precise scalar frontier. Assembly writes
only the candidate. Failure leaves the old published shape intact. Success has
a separate terminal boundary; the final pass/lifetime/static checks precede one
shape swap. All thirteen sample/workspace vector capacities and Main frames
reuse storage after warmup for an unchanged pose/domain.

Native bounds (128 groups, 8192 bindings, 0x180000 bytes per channel, 12000 vertices)
and the 8192-visit resume budget are port admission/scheduling policies, not
measured original constants. Visit budgets never claim original frame timing.

## Scoped native backing acceleration

The original source Main slice and each uninterrupted capture/decode slice use
[callback-scoped raw RAM backing proofs](../event/CALLBACK_SCOPED_RAW_RAM_ACCESS.md).
The fixed stack cache stores whole-region byte addresses only when every semantic
provider excludes the full region. Otherwise a separately excluded exact request
range can be cached; semantic addresses never enter the cache. Every load still observes fresh bytes; pose,
pass, generation, GQR and header value checks remain in place. The W guard reads
the actual root and compares the actual 24-byte BE header against its admitted
original layout using native memcmp, with a semantic/cross-region fallback. It
never caches the comparison result. Fixed tangent/scratch owner identities are
proved once per closed borrow; RAII clears that proof on every return. Source
alias writes into W invalidate the very next primitive, as the regression test
checks. A fully
nonsemantic unmapped reservation can reuse its proof within the same scope.
Mapping changes and nested borrows reject; independent semantic registries and
backing owners must stay fixed, and current/reservation callbacks must be pure.

The closed native Main binding additionally opts into a whole-copy service for
original helpers 80005634 and 8000519c. It requires a live raw borrow, complete
nonwrapping ranges, a wholly owned destination, no virtual or physical source/
destination overlap and no physical destination overlap with any external RAM
owner. It excludes the owned W-root word explicitly. Semantic storage and W/R/
file/owner aliases therefore retain scalar behavior. The word helper copies only
`(bytes >> 2) * 4` bytes and rejects shifted-zero counts, preserving its original
do/while wrap on the scalar path. The byte helper admits positive disjoint ranges.
Admission failure performs no stores. Acceleration is attempted only before the
copy cursor becomes active; retained parent identities include the service slot.
The native pass predicate must be invariant under an admitted scratch data copy;
general replay bindings default to scalar services. Copies use native libc;
visit budgets are scheduling policies rather than source instruction counts.

Main invokes its closed internal numeric/camera/linked services without an
external child or caller-visible yield while borrowed. Capture/decode release
their borrow before static admission, assembly or final geometry checks. No
address cache survives a resume. Standard ARMv6K hard-float compilation and
native libc operations retain the existing 3DS service/GPU path. This is native
backing and copy acceleration; original numeric operation order remains intact. The read slice avoids a duplicate loop-top
check because each word/byte primitive still checks fresh ownership and pure cursor
transitions have no external effects.

The publication owner accepts a native handle/resource domain rather than a
Mario-specific mesh format. New original areas and actors can reuse the file,
resource and startup-process owners; room code supplies the producer pass and
lifetime policy. The current runtime activation is Mario only. Adding other NPCs
still requires their original lifecycle, selection and draw-service bindings.

## Mario and Game ownership

A stable Mario request captures selection/upload identity, bank/handle, source
start time, root, yaw and reflection. Native setup runs once. The existing
selectName/epoch bridge remains an admitted Mario subset; complete original
SetAnim/SetLocalTime semantics are not claimed. The pass predicate borrows its uniquely owning NativeFieldAnimation. Explicit
process retirement in the owner destructor keeps request state alive throughout
teardown and avoids per-primitive weak-pointer synchronization. The semantic
camera remains shared, and a monotonic Main configuration generation detects
foreign reconfiguration even when an allocator reuses an address.

Clock/Entry/Release/reconfiguration refuse overlapping Main or presentation.
Mario also refuses retained Entry/Release before setup. A separate raw Main hold
cannot be stolen by a new publication request. Game/runtime drain pending native
work before new sequence/audio/tutorial/input/camera/clock/room/restore/save
owners. A drain-only iteration consumes neither a simulation tick nor input
edges; catch-up dropping is disabled during the hold. A real tick that starts a
hold accounts its input exactly once. Direct external VM/file/pool mutations
still require caller-owned same-pass serialization; these guards do not provide
a transaction around arbitrary writes.

Published mesh, sampled handle/bank, root, yaw and reflection become visible
only on completed publication. Rendering while pending emits no Mario vertices
and suppresses the fallback; it does not evaluate source animation. This avoids
mixing a new root with an old mesh. Existing citro3d mesh submission and renderer
capacity bounds remain in use. Other actors still use their prepared rendering
paths; complete source NPC Main/DrawMtx/Inner/queue/GPU/material/light/shadow
ownership and physical New2DS XL performance remain open.

## Validation and remaining work

`animation_mesh_publication_tests` uses original cold Mario resources. It compares
all five decoded arrays, assembled vertices/colors/batches/surfaces and sample
diagnostics against the existing resource sampler; exercises every one-visit
capture/decode hold; checks stale identity, static geometry mismatch/repair,
missing global mappings, fresh pointer redirection, linked/null timelines,
assembly failure, lifecycle conflicts and stable Mario/Game barriers. It checks
capacity/frame reuse and no source replay after completion. The same assertions
run as an isolated ARM probe. Exact suite/build/capture evidence and limitations
are recorded in [progress](../../PROGRESS.md).

The next source dependency is complete `animPoseSetAnim` 80041170, including its
force-0 already-selected return before clock admission, then the same-process NPC
Main/DrawMtx/Inner/queue integration. Keep offscreen/blur and source child
frontiers explicit. EVT-01 remains open; this delivery does not add story scenes,
claim all NPCs rendered correctly or complete the full remake.

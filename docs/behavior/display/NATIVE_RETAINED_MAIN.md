# Native retained source Main executor

`AnimationStartupProcess` owns `AnimationNativeMain`, composing the complete
[original Main prelude and tail](RETAINED_ANIMATION_MAIN_ENTRY.md),
[scalar evaluation](RETAINED_ANIMATION_MAIN_TIMELINE.md),
[cache replay](RETAINED_ANIMATION_MAIN_CACHE_REPLAY.md) and
[interpolation/publications](RETAINED_ANIMATION_MAIN_INTERPOLATION.md).
It uses the actual VM, shared pool/array aliases, files and source scratch.
There is no `BattlePosePool::main/evaluate` fallback. The raw owner is now composed with
[atomic native mesh publication and a Mario/Game barrier](NATIVE_ATOMIC_MESH_PUBLICATION.md).
Complete source NPC Draw/Inner/queue/GPU ownership remains open; EVT-01 stays open.

## Process, platform and pass ownership

Configure once while quiescent with the semantic camera bank, an unmapped
camera-token reservation and a pure `current(pass)` predicate. The predicate
must prove the exact source/file/camera producer pass throughout pending work.
The executor checks actual process readiness, alias identity, reservation and
native pose generation/resource/scratch sidecar identity at each primitive.
Native handle admission is the existing256-slot, live-sidecar domain; the
portable Main controller retains its original raw handle arithmetic. Unsupported
native lifetime admission holds, with no fabricated source success.

The process owns a persistent semantic GQR0..7 bank. Original SPR914..917 writes
set GQR2..5 in order to40004/50005/60006/70007. Writes survive holds and linked
calls, and run again for each real Main invocation. The native signed-byte
quantized provider consults actual shared GQR4 and requires60006 at each load.
A wrong/missing state holds there; no callback supplies an assumed constant.
This adapts source SPR semantics to3DS; it does not execute PPC instructions or
claim full FPSCR/Gekko numeric parity.

`main(handle,pass,visits)` retains request identity, source cursor and the same
producer pass across false returns. Visit budget limits kernel/scheduler visits
per native resume, never total original iterations. Copies/entries retain their
acknowledged progress. There is no render-frame clock, per-tick deduplication,
finite/count/depth clamp or restarted source evaluation. Changing handle/pass,
stale native lifetime, stale current predicate or overlapping token reservation
holds before further source effects. A completed invocation retires all frames;
a later invocation performs the original GQR and Main work again.

## Linked children and captured registers

Each invocation has a stable heap frame containing Main and geometry cursors,
handle/generation/resource/scratch ownership and one retained linked request.
The parent captures R/D/T/output pointers before linked execution. The first
linked request admits and pushes one child; repeated parent visits acknowledge
that same request only after its child completes. Child success pops its frame
once and marks the parent request complete. No parent prefix or completed child
is replayed. Retired blank frames are reused; unchanged nesting depth causes no further frame
allocation after warm-up. Tests count allocations across repeated invocations.
Frames use stable `unique_ptr` storage, so vector growth cannot
invalidate callback users. An explicit stack avoids recursive native C++ calls.

Linked flag2 skips the child as in the source parent. Cycles continue pushing
explicit frames within successive visit budgets, with pending status; a cycle
is never reported complete by a visited-set shortcut. Native heap exhaustion
and general resource admission remain platform limits; no total source depth
bound is asserted. The actual shared VM permits original aliases and parent
captures to survive linked mutation. Global W output buffers use real mapped
RAM, without forcing typed private R4c/R54 output admission.

## Reached facing and producer guards

Ordinary facing VM reads/stores use the same native memory owner even when
camera4 is unpublished. Null timeline exits after original GQR/linked work and
skips evaluation/facing entirely; flag8-clear completes after its fresh flag
read. Camera admission is lazy at reached camera operations/token reads. All
token addresses remain reserved with no VM fallthrough. Once admitted, the
existing actual-camera [facing binding](RETAINED_ANIMATION_PAPER_FACING.md) supplies
camera lanes and native math. A camera hold after geometry cannot replay the
successful evaluation/final cache stores.

While Main is pending, process reconfiguration/startup/Entry and native field
clock tick, Entry, Release and legacy Mario selection/evaluation return before
mutating the producer. The native Main facade also rejects active Entry/Release.
Direct VM/file/pool mutators remain caller-owned: `current(pass)` and serialized
producer ownership must prevent external release, remap, selection and camera
publication inconsistent with the pending source pass. These guards are not a
transaction over arbitrary external writes.

## Raw completion and remaining integration

Source completion writes actual scratch/global buffers and source scalar state.
It intentionally does not invoke the old evaluator or replace `BattlePose::shape`.
The separate [native publication owner](NATIVE_ATOMIC_MESH_PUBLICATION.md) now
captures fresh post-Main outputs, assembles a private candidate and swaps the
presentation snapshot atomically. Normal Mario and the Game/runtime pass barrier
are activated through that adapter. Source Draw/Inner/queue/GPU activation for
other NPCs remains separate; the raw Main API still does not assemble a mesh.

Trace/portable child contracts and actual cold-process tests cover real shared
GQR holds/repair, all five caches and three outputs, no shape fallback, wrong
handle/pass, stale generation/current/reservation, pending lifecycle guards,
null/flag8-clear without a camera, reached camera hold without evaluation replay,
linked parent captures/push/pop/flag2 skips and cyclic budgets. Native ARM runs
the same assertions. Exact suite/build/probe/capture hashes are in
[progress](../../PROGRESS.md). Original field visual parity and physical New2DS XL
performance remain unmeasured. Future raw-RAM bulk acceleration must preserve
these lifetime boundaries and exact source operation order.

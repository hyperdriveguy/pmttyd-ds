# Source animation RAM access — 2026-10-15

The native ordinary-Mario source sampler repeatedly searched the same VM region
and semantic-provider tables for each BE scalar. It now binds validated raw
spans once per synchronous sampling callback. A fixed 32-entry stack cache
serves BE reads/writes and physically disjoint `memcpy`; unbound/semantic access
and capacity overflow use the original facade. No simulation timing, sparse-key
arithmetic, source storage, held-reader capture point or GPU ownership changes.
See the [access contract](../behavior/event/CALLBACK_SCOPED_RAW_RAM_ACCESS.md).

All storage mutators reject during the callback. Data remains live through
other aliases. Distinct guest mappings can alias native storage: overlap keeps
the original forward copy loop, and final owner/MODEL validation remains after
scratch writes. A regression remaps node output onto MODEL metadata, verifies
late rejection without publishing invalid presentation state, restores it and
verifies recovery. Facade/provider identity must remain stable; callbacks cannot
yield, invoke external children or copy/move the facade.

The accepted frozen source is `artifacts/animation-ram-borrow-source`, based on
`652dded02f1472ea2b56666e6caa8392a0ee55e6` plus the scoped journal. The before
sampler is byte-identical to that base. Private benchmark harnesses use actual
source Entry/Main, GP clock, relocated MODEL/animations and authoritative BE
cache/output buffers, hashing source state, every cache/output byte and resulting
vertex/color bytes after every sample. Hashing and tick work are outside the
timed Main interval. Host: four clips × 240 samples, 960 matching frames. ARM:
four clips × 40 samples, 160 matching frames. Comparisons are within each target;
host and ARM numeric domains are not asserted identical.

| Clip | ARM before mean µs | ARM after mean µs | Before/after | Matching 40-frame hash |
| --- | ---: | ---: | ---: | --- |
| M_R_1 | 25064.960 | 3927.859 | 6.38× | 791da2a59a446244 |
| M_R_1R | 15868.760 | 2633.214 | 6.03× | 3c87612d9a39bd6e |
| M_I_5 | 20700.985 | 3059.360 | 6.77× | f40b74afd33aee56 |
| PKR_W_1 | 18042.264 | 2810.884 | 6.42× | 39eb8aadb8517b10 |

ARM timings use `svcGetSystemTick`/`SYSCLOCK_ARM11` under isolated Azahar,
normal ARMv6K hard-float `-O2` compilation. These are sampler microbenchmarks,
not whole-loop or physical New 2DS XL measurements. Host core libraries use the
same default CMake flags (no optimization); their timings varied with concurrent
build load and are retained only as local context. No target FPS is inferred.
The private probes add SD hashing/logging and strict fixtures; they are excluded
from the normal binary. The ordinary field capture shows front/rear walk/run
and jump, with native-source markers and original art. Existing camera/occlusion,
partner lifecycle, full source renderer and story-flow gaps remain open.

A separate matching-flag ARM `-fstack-usage` compile reports 776 bytes for
`AnimationScratchPose::evaluate` and 576 bytes for the snapshot access wrapper;
these are individual function frames, not a whole-call-chain stack bound. The
32-span cache itself is 384 bytes on ARM. No per-access heap allocation is added.

The full validation, source/product hashes and probe artifacts are recorded in
`artifacts/animation-ram-borrow-validation-manifest.json` and
`artifacts/animation-ram-borrow-comparison-final.json`. The initial wider test
run exposed stale inherited battle packs and a missing CTest working directory;
matching accepted `trio-attack-events.pmevt`/`recovery-effect.bin` and an explicit
source-root working directory fixed those failures without weakening assertions.
The door pack was regenerated with the frozen converter/read-only ISO for the
newer baseline. No asset conversion choices are changed by this optimization.

Required full gates and exact terminal results are in
[progress](../PROGRESS.md). Further work needs fixed-route whole-loop profiling
on New 2DS XL while preserving source timing, draw ordering and GPU lifetimes.

## Retained Main and atomic publication follow-up — 2026-10-15

Normal Mario now uses the [retained Main/publication owner](../behavior/display/NATIVE_ATOMIC_MESH_PUBLICATION.md).
The raw backing cache also accelerates facade access within a closed borrow;
whole-region admission excludes all semantic extents, with exact-range fallback
when needed. It caches addresses, never source values or header comparisons.
Fresh W-root/24-byte header checks and lifetime/pass checks remain; fixed process
structure is proved once per borrow and RAII clears that proof at return.
Native Main can complete disjoint original helper copies using libc after the
strict owned-destination/external-alias/root exclusions in the contract. Original
numeric/GQR operations and scalar fallback frontiers remain retained. The request
predicate borrows the uniquely owning NativeFieldAnimation; explicit process
retirement precedes request-state destruction, avoiding repeated weak locking.

Earlier ordinary-sampler microbenchmarks above do not measure this new retained
owner. Initial atomic integration was too slow in Azahar. Whole-loop phase
measurements without per-call SD writes identified roughly 26 ms Main plus 11 ms
publication per tick before whole-copy admission. Those are emulator observations,
not physical hardware measurements or original constants. Exact final evidence,
source hashes and current performance limits are recorded in
[progress](../PROGRESS.md) and the ignored animation-publish validation manifest.
Per-call SD profiling from an intermediate diagnostic distorted the timings and
is not used for final performance conclusions. No profiler remains in the normal
binary, and no new asset conversion or shader change was made.

## Private publication decode acceleration — 2026-10-15

Split native phase counters identified capture/decode as the dominant remaining
publication cost. The [private capture proof](../behavior/display/NATIVE_PRIVATE_CAPTURE_PROOFS.md)
now removes redundant per-scalar ownership checks only within an admitted raw,
physically private, predicate-independent decode borrow. General callers default
to scalar guards. Native VM reads, BE/float operations and finite-failure/cursor
frontiers retain their order; geometry validation and assembly remain intact.

Stable idle intervals under isolated Azahar, milliseconds per simulation tick:

| Phase | Before | After |
| --- | ---: | ---: |
| Capture/decode | 5.180 | 2.153 |
| Assembly | 1.085 | 1.092 |
| Both geometry checks combined | 0.719 | 0.719 |

Source Main remains about 2.2–2.4 ms in these intervals. Prepare includes the first
geometry check, so it must not be added to the geometry row again. Timed counters
stay in memory; sparse SD logging occurs outside simulation intervals. Both use
the same original field fixture, ARMv6K hard-float/O2 flags and original packs.
Main resumes equal actual Game ticks in the compared idle intervals. These are
emulator observations rather than physical hardware/original timing constants.
Exact source/asset/binary hashes, clean captures and remaining limits are in
[progress](../PROGRESS.md) and the ignored publication-fast validation manifest.
No profiler remains in the normal binary.

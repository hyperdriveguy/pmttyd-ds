# Animation startup, async requests and mesh publication

[Startup process](../../source/display/animation_startup_process.cpp) owns cold
source initialization and Entry adoption. [Group requests](../../source/display/animation_group_request_binding.cpp)
use the same VM/file holders; [file async](../../source/platform/file_async.cpp)
retains the underlying preload/completion graph. [Mesh publication](../../source/display/animation_mesh_publication.cpp)
is a distinct native consumer after source Main. See
[pose lifecycle](animation-pose-lifecycle.md),
[group polling](animation-group-traversal.md) and
[native Main ownership](animation-sdk-draw-ownership.md).

Guest addresses and field offsets below are hexadecimal; sizes and counts are
decimal. Native limits are admission policy, not measured original constants.

## Cold source initialization precedes alias attachment

The runtime admits exactly the ordered ImageSpans layout. It contains original
data/BSS for W/tangents, initialized Vivian storage, heap/arena globals, paths,
formats, normal actor tokens, node dispatch, render data and numeric literals.
The read-only build extractor pins every span hash; runtime checks layout and
critical words/strings/zero storage. Initialized DOL sections take precedence
over broad overlapping BSS. No executable segments enter this image.

Cold() rejects an established or nonempty typed pool. The caller supplies arena,
GP and cache/barrier services. Arena alignment is32 bytes; the minimum uses
11124KiB of enabled configured pools plus128 bytes for descriptor/final-header
allowance. Heap rows are7300/3472/224/128KiB enabled and100KiB disabled. This
native policy replaces pre-OS arena sentinels before actual event SDK bootstrap.
Allocation then uses the same real SDK heap owner, including headers, zero fill
and cache services.

initializeAnimation runs once. Subsequent pool/tangent/scratch alias attachment
may hold independently; a retry must not replay allocations, file references or
Vivian copies. Only the two cold literal-only paths are admitted by that startup
file adapter. Formatting completes before a retained acquire/Yield. Copy admits
the original positive Vivian0xc00 bytes and16-byte tail only. Work-pointer and
owner mismatch makes readiness unavailable; readiness alone does not establish
quiescence or complete rendering.

Entry uses its own retained source cursor. Native strings/formatting have bounded
admission; the raw source Entry has no such length clamp. Semantic stack filename
arguments never become fabricated guest addresses. MODEL5/TPL4 filenames remain
captured through acquire holds. Completed source-2 is a model failure; an empty
optional is a hold.

After source Entry, adoption checks actual names, files, positive signed refs,
MODEL dimensions, descriptor0 CPU geometry and scratch ownership. It does not
call typed enter/restart or rewrite BE source rows, clocks, refs or stale fields.
Native generation/resource/presentation/cache sidecars can be written before a
later scratch-admission hold; adoption is not a transaction over those sidecars.
The completed source parent still never replays. Reverse lifetime order retires
children/scratch/aliases before VM and shared pool/file owners. See
[startup](../behavior/display/NATIVE_ANIMATION_STARTUP_PROCESS.md) and
[Entry](../behavior/display/NATIVE_SOURCE_ANIMATION_POSE_ENTRY.md).

## Group and file async results are not acquired references

Group binding retains the file cache and borrows VM. Binding requires actual
shared manager aliases and can install only the verified zero empty literal at
0x8041fb30 when its four bytes are wholly unmapped. It never overwrites another
mapping. Byte strings require NUL within128 bytes, with supported MODEL/TPL
formats, prefix a and at most123 name bytes. These are native limits. The raw
parent still scans live ag2tg bytes and returns engaged0 for a completed poll.
The caller keeps the borrowed mapping/string/file owners valid through holds;
this binding does not add an independent producer-pass guard.

FileEntry state/type/refcount/name/holder/next/callback/pending fields are
0/1/2/20/a0/a4/a8/ac. Manager root0x80418498 supplies used/free lists. Async
search uses the first matching nonzero-state name, independently of new type or
completion. Pending returns0; state2 becomes3 when ready. A hit adds no ref.
Synchronous memory primitives assume admitted mappings; child unavailability
has a separate retained continuation.

On a miss, garbage modes1 then0 can supply a free node. Archive mode0 probes
once, modes1/2 up to three times; other modes skip archive. The archive path
sets a holder at E+4 and preserves the prior callback word. After E+a4 clears,
it pops CURRENT free-head's live next, rather than the previously captured NEXT.
DVD instead publishes captured NEXT. This difference must not be factored away.

DVD first opens a length probe, rounds to32 with wrapping arithmetic, closes,
then allocates. First-open failure returns-1; rounded0 returns0 after close.
The entry is published before a second open, whose raw result becomes pending.
Submission then returns0 without proving I/O completion. The request leaf only
writes DVD fields/flags; it runs no worker. No latency or tick cadence follows.

Completion searches raw pending+40 against callback info, without a state/type
gate or a graceful no-match return. It clears pending first. Only exact result-3
marks state2/type0b and skips relocation/user callback. Every other value follows
relocation with signed type byte, close, then a fresh callback-word read. No
reference or data-success guarantee is inferred from those raw statuses. See
[file request](../behavior/file/FILE_ASYNC_REQUEST_AND_CACHE_PUBLICATION.md) and
[completion](../behavior/file/FILE_ASYNC_COMPLETION_AND_DVD_REQUEST_STATE.md).

## Publication swaps a complete native candidate

Reservation pins handle/pass/generation/resource/buffers before selection/setup.
Source Main completes once, then a separate resume captures fresh post-Main W,
MODEL/selection, output selector, counts/pointers and diagnostics. These are not
Main's earlier captured registers. A failed read resumes its scalar frontier,
without silently refreshing earlier metadata.

Decode order is position/normal XYZ, visibility bytes, node floats, texture
selector and five floats per24-byte row. Even absent native surfaces retain all
source reads. Decoded mesh lanes must be finite; time/fraction diagnostics retain
raw bits. Static geometry is checked against the admitted converted original
resource before assembly and immediately before publication.

The candidate alone receives assembly writes. Native caps are128 groups,
8192 bindings,0x180000 bytes/channel and12000 vertices; group nodes require24
floats. These are port policies. Successful assembly has its own retained
boundary; later holds do not assemble again. Final checks precede one shape swap,
so earlier failures preserve the old published mesh. GPU residency and Draw
completion remain separate.

Private capture may elide repeated guards only within one raw borrow after an
explicit independent-predicate promise, root/header/raw-span checks and physical
disjointness from cursor/candidate writes. Proofs reset on every return. The
borrow ends before static admission, assembly or publication. Other capture
uses per-primitive current checks. See [atomic publication](../behavior/display/NATIVE_ATOMIC_MESH_PUBLICATION.md)
for the complete lifetime/acceleration contract and integration limits.

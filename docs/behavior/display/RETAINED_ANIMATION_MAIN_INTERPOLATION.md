# Retained original Main output interpolation

Complete original nonnull Main `80040084..80040540`, following
[cache replay](RETAINED_ANIMATION_MAIN_CACHE_REPLAY.md). The
`advanceAnimationMainGeometry` owner composes time, timeline, replay and
interpolation through both final cache publications. It supplies a retained
Main evaluation child; it does not acknowledge optional facing, linked/native
platform ownership or activation of the normal Game NPC display path. EVT-01
and original rendering/performance acceptance remain open.

## Original evidence and inputs

Read-only US revision0 ISO region `80040084+4bc` SHA256
`515ca3b12ebde6ba8d8ae707cb93344fe518440fc7fd286d9164e52e789b0155`;
final stores `80040538+8` SHA256
`3fe5efdd62747012117ed3d7c042e34ce034861767f0ba3bc7c0c296c684ef18`.
Private source listing and detailed instruction audit are ignored
`artifacts/npc-main-parent-evidence/animation-main-interpolation-review.txt`
and `animation-main-interpolation-notes.txt`.

Inputs are captured Main R/D/T/output position/normal pointers and replay's
captured current, next, fraction and five MODEL counts. No recapture of live
R28/R2c/R30/counts/R4c/R54 is substituted. Identity compares fraction bits,
including NaN payload and signed zero. Three branches use signed index order:

- Equal: copy position and auxiliary caches to captured outputs for each raw
  nonzero count. For nodes read fresh output R64 before source cache R60.
- Current greater than next: skip all copies, headers, constants and updates;
  existing output remains, including a loop seam. Continue to final stores.
- Current less than next: full position/auxiliary vectors and cubic nodes below.

Copies share the original retained forward-word helper with replay, including
raw modulo32 lengths, overlapping behavior and shifted-count-zero do/while
wrap. Byte and texture outputs have no interpolation copy in this region.
Counts gate base copies only; zero MODEL counts never suppress a nonempty
stream or the original header reads.

## Increasing-key vector graph

Position reads T28, scale 8041fbac, T2c, nextKey+4 offset; multiplies
scale by captured fraction; reads nextKey+8 count and R48 cache. Range and
next-key pointer are captured before optional output copy. Unsigned
start<end gates updates; derived count is `low32(end+3-start)>>2`. No index
or finite clamp, count cap or bounds sanitizer is added. Every entry reads
index byte, advances cumulative output by 12*index, captures all three GQR4
signed deltas, then reads/computes/stores X, Y and Z separately. FMA operands
are delta first, scaled fraction second, old value third. Sparse indices
remain cumulative and repeated entries are applied repeatedly.

Auxiliary reads offset/count from the SAME retained next-key pointer, then
fresh T30 and R50. It captures range before copying to captured normals and
uses the same retained scale/fraction and triple-before-destination graph.
Changed T28 or R4c/R54 during earlier stores does not redirect these reads.
Bulk4 and remainder entry arithmetic share an identical observable memory
graph; the native controller yields per completed entry without a total cap.

## Node graph

Fresh T28 supplies duration times only. Offset/count come from the earlier
retained next-key pointer; read next time before current time, then T3c;
subtract duration; read R60 cache BEFORE R64 output (opposite equal branch).
Capture range and output before optional node copy. Compute t2=t*t after
copy even for empty range. The unsigned range gate and derived count follow
the original modulo32 operations.

Each node reads byte3 (signed b), then byte0 (unsigned sparse increment), and
advances output by 4*index even when rejected. Reject b>=89 or b<=-89; do
not read delta/tangents/constants or write that entry. Otherwise:

1. Quantized byte1 delta; t3=t*t2; byte2 signed a; TWO at 8041fb64.
2. Source FNMSUBS(TWO,t2,t3), using the shared `animationFnmsubs` convention;
   THREE at 8041fbb0; tangentA at `803d96e4+4*a`.
3. Round THREE*t2; load NEGATIVE_TWO at 8041fb98; round h10=t+fnmsubs
   and m0=duration*tangentA; load SCALE 8041fbac; read tangentB at
   `803d96e4+4*b`.
4. FMA(NEGATIVE_TWO,t3,THREE*t2); round SCALE*delta and h10*m0;
   read old destination BEFORE remaining arithmetic.
5. Round h11=t3-t2 and m1=duration*tangentB; two ordered FMAs add the delta
   and h11*m1 terms; round old+term and store.

All ordinary operations retain binary32 rounding boundaries. Constants and
tangents are fresh per admitted entry; do not hoist them or predecode both
tangents. Signed a has no range guard: a=-128 reads actual Work+74 at
803d94e4; unavailable adjacent memory holds rather than clamping or using a
host tangent table. Current GQR4 must be 00060006 at each quantized load.

## Publications, continuation and validation

All successful branches store captured current to R3c first, then captured
fraction bits to R40. A hold between them never replays the first store.
Only after both stores may Main proceed to its optional fresh-flag/facing tail.
Scalar/replay success remains captured across later interpolation holds.
Input/service identities, same producer pass and cursor must remain stable.
Primitive unavailable means no effects; controller false can follow stores or
cooperative copy/entry yields. Cursors are fixed-size and allocate nothing per
invocation. Actual replay VM/pool/alias/GQR binding is reused; no detached
memory or assumed quantization owner is introduced.

Independent fixtures cover all signed index routes, exact empty and cubic
read traces, every primitive hold, copy wrap, adjacent tangent addresses,
gate boundaries, destination advance on rejection, captured outputs/next-key
versus fresh duration table, dynamic constants/tangents, NaN/signed-zero
identity and zero MODEL counts with live streams. Actual cold MarioM_W at
17 ticks compares all five cache and all three output buffers with the admitted
source-backed sampler; final R40 holds while epoch/index mutate and resumes
one store only. Temporary ARM harness runs the same assertions. Exact test,
build and native capture evidence is recorded in [progress](../../PROGRESS.md).

This establishes the local source operation graph and admitted sampler
comparison. It does not establish general Gekko NaN-payload/exception/alignment
parity, original field visual parity, full Main/runtime integration, physical
New 2DS XL performance or full-game completion. Future synchronous raw-RAM
bulk admission must preserve these observable accesses and release raw borrows
before callbacks/yields. Optimize against measured device bottlenecks after
complete native ownership is connected.

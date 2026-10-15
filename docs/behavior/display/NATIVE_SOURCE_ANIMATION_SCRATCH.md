# Native original animation scratch ownership

This implements the bounded ordinary unlinked/private-output sampler described
by [Main entry/time](ANIMATION_MAIN_ENTRY_AND_TIME.md),
[compressed cache](ANIMATION_COMPRESSED_DELTA_CACHE.md), and
[output interpolation](ANIMATION_OUTPUT_INTERPOLATION.md). It extends
[real source Entry](NATIVE_SOURCE_ANIMATION_POSE_ENTRY.md) and
[live tangent ownership](NATIVE_LIVE_ANIMATION_TANGENTS.md).

Actual `AnimationStartupProcess` Entry attaches `AnimationScratchPose` to its
adopted pose. Sampling follows the actual loaded FileEntry/holder/MODEL/directory/
descriptor/T chain. All runtime pointers are already relocated. Controls,
continuous44-byte keys, discrete8-byte rows and sparse streams are read from
that loaded file, rather than the converted PMANIM timeline. The caller still
supplies source-domain millisecond quotients; this does not bind the whole
original GP clock/update loop or implement GQR/FP exception behavior.

The actual AllocBuffer reservations remain authoritative BE RAM. Positions and
normals use separate cache/output12-byte vectors; nodes use separate arrays of
scalar4-byte floats. Visibility P58/5c and texture P68/6c are aliases. Source
MODEL140 counts scalar floats; MODEL12c counts24-byte texture rows. No decoded
native vector is copied back to source scratch.

Reset copies bases in position/normal/visibility/node/texture order, then replays
keys0..current. Forward sampling replays only crossed keys. Same-key reverse
fraction publishes positive0 before output. Position/normal sparse records use
unsigned relative indices and signed8 deltas with1/16 fused multiply-add.
Visibility and selector additions wrap bytes. Texture updates alter byte0 and
floats4/8 only; padding and scale/rotation survive. Forward continuous output
copies caches then applies the next key; current==next copies only, while
current>next retains previous output across the loop seam. Cubic node output
uses the same live BE tangent owner and explicit binary32/FMA boundaries.

The phase is synchronous and has no yielding children. Native admission checks
nonoverlapping private cache/output spans inside the actual source scratch
allocation, true visibility/texture aliases, matching converted static geometry
counts, and backed accessed source tables/relative target indices. Checks are
native safety policy, not recovered source guards. Zero-count unused pointers
are ignored, including genuine wrapped sentinels. Counts are bounded by admitted
geometry, continuous keys1024 and discrete keys65536. Invalid predecessors,
arbitrary incoming tangent addresses, malformed/overlapping scratch, linked
recursion, ring outputs and optional facing remain unsupported. Failure can
retain timeline publications before scratch preflight; it does not certify a
completed full source Main or retry arbitrary partial source child graphs.

Traversal reads visibility and24 scalar node lanes at its existing capture
points. Material selection reads the current selector, and its second walk
captures the five transform floats independently. Fan FIFO reads current BE
position/normal outputs when each vertex is staged; held completed fans retain
their already copied geometry. Native vectors remain presentation snapshots.
The native flush adapter decodes current source vectors before its CPU cache
flush; final GPU staging remains the existing Citro3D path. Color/UV/static
hierarchy/material/atlas metadata remain converted original resources. This
work does not activate the source process in normal Mario/partner owners.
Typed legacy release holds while a live source buffer provider is attached;
actual source file/heap release cannot be replaced by decrementing sidecars.

Copies freeze current BE cache/output into independent native vectors and clone
the live tangent table. Context retirement freezes active valid poses before
VM/array destruction; control-block identity prevents an unrelated later owner
from adopting failed dead views when heap addresses are reused. Failed retirement
leaves unavailable weak views, never
stale VM pointers or a silent converted fallback. Standalone context destruction
uses a direct validated snapshot because weak_ptr::lock already fails while the
last shared owner is destructing. A retired/copied pool is a detached native
adapter; arbitrary runtime edits of raw source timelines are not transplanted
into its immutable converted metadata.

Original source evidence: animPoseMain8003ed2c/1940; private ignored disassembly
and pointer-chain notes in artifacts/animation-scratch-evidence/. See the shared
contracts above for original store/stream order and arithmetic. Native libm,
FMA and matrix composition remain adaptations rather than Gekko bit parity.

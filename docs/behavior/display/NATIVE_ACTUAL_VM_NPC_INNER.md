# Actual-VM NPC Inner parent and CPU binding

US G8ME01 disc0 revision0, static source review; no original execution or timing
measurement. Inner8003bc68/7e8 SHA256
`b224075659cd7de6e8abb7f76d9adac1adadbcf91024482283a13d85c438e59b`;
mapTestXLU8001ca2c/a8 SHA256
`5c65a0530038f9e33f75b326aa97bcb531167a3aa931feae8eae5a275a4a0718`.
Ignored read-only evidence: `artifacts/native-npc-inner-evidence.txt` and
`artifacts/npc-main-parent-evidence/animPoseInner-review.txt`.

The [unlinked wrapper](NATIVE_UNLINKED_NPC_DRAW_WRAPPER.md) can now install an
owned `AnimationNativeInner` CPU binding with `configureNpcInnerBackend`.
`resumeAnimationInner` implements the complete local parent and map classifier;
GX, material callbacks, cache flush/invalidation and dispProc remain explicit
backend children with no successful defaults. This is not production NPC GPU
activation or a claim of rendered equivalence. The existing typed battle Inner
remains a separate adapter; it is not substituted for actual VM outputs.

## Capture order and retained effects

Initial reads interleave resource resolution and sample captures: global W,
P10 index, W0 table, P14 animation, entry, P28 current, entrya0 holder,
P2c next, holder0 MODEL D, P30 fraction, D1ac directory, descriptor T.
A null T returns after this prefix. INITIAL W10c is captured once before the
sole Pc mode read. Admission preserves pass2's repeated Dc4 read and PPC low6-bit
slw. Secondary color Pf4 precedes primary Pf0 and Pe8 flags. Each map query
loads index before base, halfword flags before alpha byte; the second query
reloads both globals. Callback(E) skips builtin GX and We0 publication.
Unknown raw E skips GX calls but still publishes fresh We0.

[Facing](ANIMATION_INNER_DRAW_FACING.md) retains original camera4/math/matrix
boundaries. Facing P70 stores A−90, not A+90. Camera binding follows the current
lease camera owner and configuration between completed passes. Changed owners
cannot reuse a previous camera projection. No camera is fabricated for flag8.

A fresh P3c cache miss reads global W, P4 flags, then W10 pool. Handle conversion
is high32(0xb21642c9 * low32(P−pool)) >> 8, without a source bounds clamp. The
inline ring captures capacity before completing fresh MODEL/count resolution;
sums wrap32 and comparisons are signed. Exact capacity does not wrap the cursor.
Oversize skips allocation but still calls Main and clears fresh We8 if the ring
flag was set. Each allocation stage reloads W as the source does. Main uses the
same actual VM, native pass and handle; completed partial Main work is retained.
Captured descriptor/MODEL/sample/fraction survive late Main unchanged.

Modeling globals are reset, effective matrix copied, then separately reloaded W
publishes P/T/D. Flush selector is captured once across both calls. Each flush
captures its output pointer before finishing fresh resource resolution; the
second call reloads W/resource/count, even for zero length. Invalidation precedes
a fresh D144 read. The exact traversal request is parent −1, groups−1, captured
current/next/fraction; no pass parameter is prepared at the original call site.
All uint32 address/count operations wrap as in the source; no local null/count/
finite/range guard is attributed to the original parent.

## Native ownership and numeric scope

The native owner retains input float bits, providers, VM, lease/pass/current
predicate, pose generation/resource/buffer lifetime and configuration. Holds do
not replay completed reads, writes or child acknowledgements. An unavailable
backend never falls through to legacy draw. Only quiescent state admits backend
replacement or destruction; pending Draw/Main prevents lease retirement.

Cold process setup provisions original modeling BSS at803d8e70/600 and
8041e89c/8, once before callers. This supplies writable VM storage; it does not
simulate source Inner stores. Matrix Copy adapts finite matrices as six paired
8-byte BE writes into complete raw RAM. Each admitted pair stores both lanes
without a provider callback between them; earlier pairs survive a later missing
pair. Missing/partial RAM and nonfinite inputs hold. This finite-domain native
admission is not an original guard. GQR0/FPSCR, subnormal/NaN behavior and full SDK
numeric parity remain unproven. Existing explicit math adapters supply matrix
and trig requests; the original paired-single instructions are not executed.

The retained core uses fixed cursor/matrix storage. There is no per-draw VM or
mesh construction. Performance on physical New2DS XL and actual Citro3D model/
material/cache integration still require measurement and implementation.

Tests enumerate holds for the parent/map/ring, fresh selector/resource behavior,
signed overflow, magic handle conversion and exact traversal captures. Actual
VM tests execute real late Main, model-stack repair, current-owner rejection,
backend exclusion and camera-owner replacement. Test GPU/traversal receipts are
explicit fixtures and do not constitute a renderer. Continue with real actual-VM
group/shape/material/texture/GPU owners, then normal Game queue scheduling and
linked wrapper work. Entire outer callbacks must drain before ending the lease.

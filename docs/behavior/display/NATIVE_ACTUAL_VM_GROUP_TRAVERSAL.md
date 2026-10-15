# Actual-VM group traversal and modeling publication

US G8ME01 disc0 revision0; static source review, no original execution or timing
measurement. Complete dispProc8003c4bc/3c8 SHA256
`3f5b22a0fe508307adf32b84e907e387b09d221b78626071f05240fb99222476`.
Ignored read-only evidence: `artifacts/npc-dispProc-ordered-evidence.txt`,
`artifacts/npc-actual-traversal-next-evidence.txt` and
`artifacts/npc-main-parent-evidence/dispProc-inner-abi.txt`.

`resumeAnimationGroup` implements the complete local parent described by
[group traversal](ANIMATION_GROUP_TRAVERSAL_AND_REQUESTS.md). The request is
parent/group/current/next/fraction, with exact float bits and no draw-pass field.
Only group ffffffff is empty. Other indices, pointers, products and stack updates
wrap32 without alleged source bounds/depth/cycle/finite guards. Native provider
admission and unavailable backing remain separate from source control flow.

## Reached reads and retained calls

Capture W, W20 MODEL D, W18 pose P, D1a8 group base, P5c visibility pointer,
group4c index, signed visibility byte. Any nonzero byte, including ff, is visible.
Invisible nodes skip push/outputs/child/pop but still visit their sibling. Visible
nodes capture group54 kind, group50 node index and P64 node buffer. Joint parent
scales use the incoming parent in the captured group array and captured node
buffer. Parent −1 or nonjoint parent reads ONE once and reuses all three lanes.
Transform/Joint requests carry actual guest node addresses and parent floats.
Their SDK bodies remain explicit children, not identity or C++-stack defaults.

Each flag40/80/20 query reloads global W and current P independently. Captured
G persists across callbacks, changed global MODEL/pose and recursion. Flag40
reads the current modeling pointer, P15c holder, seed words802c1048/4c,
translation X, seed802c1050, holder target, then translations Y/Z. Seed words
are reached even though native local XYZ overwrites them. Store raw XYZ to
holderTarget+12*group in order. Flag80 reads P164 holder before the modeling
pointer and holder target, then requests VM-to-VM Copy to target+48*group.
The holder indirections must not be replaced by inline pose output addresses.

Matched flag20 requires word[word[P120]]==group; unmatched flags proceed through
ordinary callback/render/child. The matched route captures P11c kind, ZERO,
inline saved-matrix address P+124, animation P14, table80415db0+4*kind and current
modeling pointer. The table is read before classifying kind, including unknown
or negative raw values. Kind4's table entry aliases global W80415dc0: no generic
four-entry bounds clamp or independent table storage is inferred. Material shape/
current is called before pair floats are loaded and vivihimo is called. Kind1/2/3
use source −2/−3/−1 and .5; other values retain captured ZERO/ZERO.

After the effect acknowledges, reload W, load factor8041fba0, then reload P and
clear only flag20. Blend nine basis lanes in source order: fresh M lane, fresh
saved lane, single fsubs, fmadds, store saved lane. Earlier output may alias later
input. Translation is untouched; matrices are never snapshotted for this loop.
Effect writes precede these reads. The factor and M/saved addresses remain pinned.

Ordinary callback captures identity P154, data P158, group and current modeling
pointer. Then reread captured G48 for render(shape,current). The render ABI
contains neither next nor original pass. Read G44 after render, recurse using
parent=CURRENT group, then pop actual level and pointer globals. Pop stores
level−1 before pointer=803d8e70+48*newLevel, without underflow clamp. Finally
read captured G40 and recurse with parent=CURRENT group for the sibling too.
Only the recursive child's reached reads observe newly selected MODEL/pose data.

Stable heap frames retain exact requests across holds and reuse observed depth
between calls. No total traversal cap, visited-set skip, recursive host C++ call,
per-draw frame teardown or rebuilt mesh/VM substitutes for the source graph.
Budget counts native scheduling visits and has no action-command timing meaning.

## Native integration and numeric scope

Setting `NativeNpcInnerBackend::groups` selects an owned `AnimationGroupBinding`
inside [actual-VM Inner](NATIVE_ACTUAL_VM_NPC_INNER.md). Its absence preserves
the explicit external traversal route. Once selected, a held group never falls
through to that external route. The binding reads/writes the same actual VM and
shares Draw's source lifetime/current/lease guards. Pending nodes/render/effects
block Inner, wrapper and lease retirement; backend replacement stays quiescent.

Native Copy adapts finite GQR0 float pairs, sequential offsets0,8,10,18,20,28
(hex). Each complete raw 8-byte source pair is captured before its matching
complete raw destination pair is stored. Source/destination overlap follows the
original forward pair sequence, including repeated values on forward overlap.
Partial destinations cannot retire one lane. A held pair keeps its source bits;
earlier pairs stay committed while later pairs read fresh after repair. This is
not memmove or the owned-matrix snapshot API used by outer Inner.

The numeric blend adapter admits finite operands/results, explicitly narrows the
subtraction then uses std::fma. Nonfinite Copy pairs and nonfinite/overflow blend
holds retain captured values; later edits of already captured source/factor do
not undo that admission frontier. GQR0/FPSCR/NaN/subnormal/exception and original
paired-single numeric parity remain unproven. These are native adaptation limits,
not original guards. Unsupported semantic pair mappings likewise hold.

Node builders, material, Vivian, dynamic callback and GPU render have no
successful defaults. Missing seed/literal/table backing is a source-read hold,
not zero/opaque/empty geometry. Original-node capture audit records Joint's12
and Transform's21 fields before the first SDK child, in interleaved order with
ZERO/TWO loads; a whole node24 memcpy is not equivalent. Continue those complete
SDK children and live-literal/stack publications, then actual render/material/
texture/FIFO/Citro3D work, linked Draw and ordinary Game camera/queue scheduling.

Validation traces cover every parent operation hold, invisible/signed visibility,
child/sibling parent semantics, fresh globals, output holders/seed order, raw pop
underflow, all special kinds including table/root alias, effect-before-blend and
basis aliasing. Native tests walk the actual original Mario group graph with
explicit node/render receipts, reject competing leases, repair partial raw pairs,
cover both overlap directions and run finite/nonfinite blend frontiers. These
fixtures prove control/ownership; they do not prove original node matrices, GPU
pixels, ordinary NPC activation or physical New2DS XL performance.


## Actual NodePush delivery (2026-10-15)

The default node child now owns [actual Joint/Transform controllers and CPU SDK
binding](NATIVE_ACTUAL_VM_NODE_BUILDERS.md). It replaces the missing-node hold
with source captures, live constants, exact component/dispatch requests and real
modeling-stack output. An explicitly installed backend node provider remains
exclusive, including on false. Material/Vivian/callback/render GPU work remains
unfinished; the original typed node24/C++ stack is not substituted.

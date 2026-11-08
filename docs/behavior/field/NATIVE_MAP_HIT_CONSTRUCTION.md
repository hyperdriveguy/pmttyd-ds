# Native original map hit construction

The retained map graph includes complete US revision0 hitEntry80018658/9c,
hitEntrySub800186f4/278, _hitEnt8001896c/48c and hitCalcVtxPosition800165a8/4b4.
The source verifier checks all 4,840 original words across 37 whole bodies against
the read-only ISO. This is static instruction evidence, not an original runtime
collision trace. The constructor operates on the same relocated mutable D and
actual map heap used by the source loader.

Entry writes E158, allocates align32((jointCount+128)*e4), clears it and stores
E15c. Sub preserves the actual parent/child/sibling hierarchy. Each hit object
borrows the original joint and attributes, constructs T*Rz*Ry*Rx*S, composes its
parent world matrix, and writes local/world centers and radius. The six original
bbox +/-0.1 stores happen once per object; retained child holds do not replay
that expansion or allocation. Actual source triangle allocation counts candidates
before degenerate filtering and does not shrink the allocation afterward.

Triangle records retain three vertices, V2-V0, V0-V1, V1-V2 and their unnormalized
cross product, at offsets0/c/18/24/30/3c/48 with stride54. Even and odd strips
retain source winding. Unpacked signed indices read float positions and always
advance the output. Packed unsigned indices read signed16 components through
fresh source-computed GQR5; load type7 and signed six-bit scale are decoded live.
W1 supplies paired lane1=1. Zero-normal packed candidates are rejected only after
all source writes, leaving their slot available for overwrite. No epsilon,
normalization or input prefilter replaces these effects.

The private source cursor admits zero-seeded binary32 scalar/paired registers,
unquantized float GQR0, and reached type7 GQR5. HitCalc's real eight-byte scalar
save and eight-byte pair save/restore execute through retained word transfers.
The scalar save is the binary64 promotion of the admitted binary32 value, not
a raw32 word. lfs replicates the paired value; lfd preserves paired lane1.
This does not preserve arbitrary external binary64 FPR/FPSCR state. GQR5 remains
live in the cursor after its source write; the body does not restore it.

The shared SDK adapter adds finite subtract/cross and actual Distance arithmetic.
It admits exact in-place or disjoint vectors and rejects shifted aliases. Cross
uses the original single-rounded products/fused grouping, including negation.
Distance captures squared distance and source half/three, retains an unavailable
estimate, then executes the original single refinement. Its estimate provider
returns binary64; the default has no estimate and therefore holds. Providers,
VM epoch and request identity must stay stable. A test/native demo provider may
explicitly choose1/sqrt(double); that is a numerical adaptation, not Gekko seed
parity. No GPL estimate implementation/table is imported. Existing finite matrix
sin/cos services are also native adaptations. FPSCR, nonfinite/subnormal exception
behavior and original transformed contact/radius comparisons remain open.

Original harbor inputs contain98 collision joints,85 packed parts,732 strips and
1,604 allocation candidates/134,736 payload bytes. These are static input counts;
35 local integer degenerates do not establish the transformed retained count.
Tests execute real relocated harbor construction with an explicitly approximate
seed, then hold at the actual next missing light owner; they do not publish the
map's loaded count, install Game collision, or prove original wall/support traces.
Native Game/EVT epochs, active hit query/update/unload ownership, light/camera/GX,
original captures and hardware timing remain separate unfinished work.

See [map owner](NATIVE_MAP_OBJECT_SOURCE_OWNER.md), [wall request](../../requests/OW-WALL-01.md),
[support request](../../requests/OW-SUPPORT-01.md), and [asset provenance](../../ASSETS.md).

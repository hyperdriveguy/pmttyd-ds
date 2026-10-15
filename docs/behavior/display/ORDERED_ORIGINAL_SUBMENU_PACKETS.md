# Ordered original submenu2/3 packets

This module closes the finite textured submenu geometry/material child path.
It does not activate the weapon menu, register a callback, acknowledge font/icon
children, infer an allocation, or manufacture a Camera8 projection. It uses the
separate original submenu surface lease from
[the resource contract](RETAINED_SOURCE_WINDOW_AND_SUBMENU_SURFACES.md).
Indices2/3 never enter the footer4..16 packet/resource path.

## Actual original source path

DrawSubIconSub8011847c first calls InitSub80103a14, concatenates the actual
Camera8 view with its caller matrix and loads it. It queries original common
index2 at80118600 and draws two Quad801037c4 children at80118630/654, then
queries index3 at80118670 and draws one Quad at801186a0. Source supplies all
rectangle coordinates and colors. This module adds no selector rectangle or
camera setup. Existing retained DrawSubIconRaster remains the parent owner.

`resumeBattleSubmenuInit` owns whole InitSub80103a14/134 and its whole
InitRasta80103c3c/94 child. All22 original GX calls retain individual operands
and successful boundaries. The source Rasta blend logical op is5; it is not
silently replaced with the window primitive's logical op7. Native blend mode1
uses source-alpha/inverse-source-alpha; the logical-op selector has no effect
in that blend mode. The remaining source values are preserved in the finite GX
translator before native packet generation.

`resumeBattleSubmenuQuad` owns whole Quad801037c4/124 and whole inline
Vertex801038e8/30. Begin is128/0/4. Four vertices use caller X0/Y0/X1/Y1;
source UV reads from80422240/4c occur in original order, including the fmr reuse
for the first and third vertices. Each vertex stores X/Y, then freshly reads
its Z constant, then stores Z, four low8 RGBA bytes, U/V. Nine FIFO lanes per
vertex survive holds independently. Caller colors are original word arguments;
low8 truncation models the actual stb instructions. No guest stack address is
invented for their private copies. Missing source words and GX operations hold.

The existing complete retained TexturePlane2/Init/PlainGX can feed the same
provider. Its actual matrix, original metadata query, per-vertex colors and
FIFO geometry are retained by that existing child; it is not reduced to a
Quad success acknowledgement. InitSub and TexturePlane Init differ, and each
keeps its own complete source order. The packet provider does not claim that
all opaque source children are available.

## GX provider and immutable packet

`BattleSubmenuPacketProvider` accepts only exact finite source state requests:
original cull/depth-location/alpha/blend/depth, direct POS/COLOR/UV descriptors
and formats, original channel controls, one generator and TEV stage, original
TEV color/alpha/order/modulate paths, valid matrix and actual texture queries.
Begin requires all applicable state to have genuinely reached this owner. A
caller may not prefill readiness to bypass missing Init/Rasta. Unknown material,
indices, formats, descriptor combinations, nonfinite geometry and source UV
outside0..1 remain unavailable. This supported CMPR path does not invent support
for another GX material or repeated/mirrored UV domain.

Original metadata is resolved through actual live global B8041eb90,
B+163f8/FileEntry+a0/pointer-cell/palette, relocated TPL magic/count97/table,
actual index2/3 image headers/data/span/sampler/relocation flags. Missing bytes
remain unavailable. The metadata reader is separate from the footer reader's
explicit4..16 domain. The query must complete the actual TPLGetGXTexObj child into its actual typed
private output owner and supply its handle, all eight known source GX words,
metadata and lifetime/current proof. No index-to-object handle is manufactured
and no guest SP address is invented. Missing SDK construction remains a hold.
Width/height/format use the exact source GetTexObj leaves on those object words,
and must match the original index2/3 descriptor. LoadTexture copies the known
object state; later reuse of the original private output cell does not alter
already loaded geometry/material state. Handle reuse supersedes the old cached
output object. Source metadata sampler must be actual wrap0/min-mag1/LOD0;
unsupported mutations are held, not silently replaced with native defaults.

The provider copies raw source positions, source UV, per-vertex RGBA, the
original loaded affine matrix, actual metadata/loaded GX words, distinct native surface lease
and actual Camera8 proof into `shared_ptr<const BattleSubmenuPacket>`. The
successful append happens only at the final ST store. If the sink holds, the
same immutable packet and final request survive; preceding FIFO/state/query
operations do not repeat. The sink owns any partial work and must not report
false after accepting a packet without retaining its own deduplication cursor.
Recursive provider/source resume and reset while invoked are rejected. Source
service identities, actual context lifetime and copied argument bits are pinned
until completion. Observed retirement poisons the held owner; apparent revival
cannot replay an accepted child.

## Genuine short and long ownership proofs

The weapon Frame owner supplies the captured actual Camera8 projection,
viewport6/scissor4, nonzero allocation generation and successful source pass,
strong stable context, and an explicit long current callback. It must capture
these from its admitted actual camera view while the original80121ce8 callback
is executing. A default CameraManager record or viewPublished bit is not proof.
`query.current` separately proves the held executing callback/short camera
ownership. An actual source common-bank resource owner and independent long
`sourceResourceCurrent` predicate are also required and copied into the packet.
A pointer/address/hash or a token invented during lookup cannot supply these
lifetimes. Contexts must remain alive, stationary and serially accessed; current
predicates are pure, non-reentrant ownership checks, not game-state callbacks.

After source completion the immutable geometry may survive CameraDraw's normal
queue reset/epoch finish. Its long camera-publication/allocation and source
resource predicates must therefore remain independent of the ended short
camera context. Retirement/replacement still rejects it. No code in this module
relaxes Frame admission or derives these predicates by polling source bytes.

`validateBattleSubmenuBatch` checks every packet, current GPU generation,
source/common-bank lifetime, long Camera8 owner, matrix/positions/UV finite
domain and transformed-vertex overflow before native mutation. A batch must use
one actual camera pass/projection/viewport/scissor and one source resource
ownership identity. Empty geometry is no new source drawing. Packet creation is
not presentation acknowledgement.

## Native ordered submission

The exact Renderer journal adds canDrawSubmenuPackets/drawSubmenuPackets;
the prerequisite journal owns explicit submenu atlas preparation/shutdown.
The native method precomputes all vertices and preflights the whole supplied
batch again before its first C2D/GPU operation. It uses the captured original
viewport/scissor/depth and existing exact nativeCameraRect adapter; unsupported
endpoint domains remain unavailable. Source affine matrix already includes
the actual view, so nativeCameraTransform receives captured projection and
identity view, avoiding a second view multiplication. No hardcoded orthographic
projection, screen center, UI half-scale or selector geometry is used.

Actual C3D submission modulates the real submenu atlas and per-vertex source
RGBA, preserves quad input order through triangle fans, binds all samplers,
sets explicit material/depth/blend/attributes, and returns success only after
all submitted primitives. Resource retirement must be serialized with native
submission. There are no externally callable ownership callbacks after the
final preflight and before GPU completion; no false return can acknowledge a
partial native prefix. Actual viewport conversion and GPU/float interpolation
remain the declared native adapters, not measured Gekko/pixel parity.

The whole mixed weapon batch owner must retain each immutable packet at its
exact original append ordinal, interleaved with original glyph/icon/window
packets. It preflights every packet type before drawing any prefix. It may call
this native method for contiguous submenu spans or one ordered ordinal, but
must not collect submenu packets into a later overlay. The actual Frame source
completion/seal/submit/camera.finish/publication lifecycle is still owned by
BattleWeaponNativeFrame. This module neither binds that source callback nor
certifies missing font/icon/window helpers.

## Validation and limitations

Host tests exercise all22 Init GX calls and every Quad source word/FIFO boundary
with two holds, compare successful traces and captured raw/transformed geometry,
retain two final sink holds without prior-lane replay, test fresh post-wait Z
reads and earlier captured UV, raw byte truncation, reentry/reset, invalid
state/material, stale GPU/source/pass owners, mixed camera snapshots, and
completed packets surviving normal short-camera end. The actual existing whole
TexturePlane2 feeds this provider and retains two final sink holds. Packed
common-bank tests check the real source pointer chain and missing/replaced
metadata. Missing known GX object words remain unavailable; actual output-handle
reuse copies each loaded object independently. Host GPU descriptors/camera fixtures are explicitly test inputs; they
are not normal runtime ownership or fake source readiness.

Read-only verifier pins complete original InitSub/Rasta/Quad/Vertex bodies,
GetTexObjWidth/Height/Format leaves and all22 GX child operands. Strict host/ARM and sanitizer results, native Renderer
journal syntax proof and exact frozen hashes are in the handoff manifest.
Native GPU execution/visual comparison, normal weapon activation and the final
mixed ordered batch composition remain required integration checks. Original
physical controls, original menu assets/style and lower audience scene policy
are unchanged. No touch-only controls are introduced.

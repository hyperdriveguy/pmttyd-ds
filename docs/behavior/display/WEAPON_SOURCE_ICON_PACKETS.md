# Source-backed weapon icon packets

`BattleWeaponSourceIcon` composes the existing whole source Matrix/Position and
GX continuations with a genuine Icon process and private TEX output. It supplies
a finite native packet backend for ordinary camera-8 UI icons. It does not
schedule a battle display pass, complete a weapon frame, import textures or
submit GPU work.

## Original bodies and ownership

The read-only US revision-0 DOL is the authority. The verifier pins complete
`iconGX` (`800197f8`, length `9a4`), Matrix/color entry (`8001a4bc`, `224`),
Matrix entry (`8001a6e0`, `238`), Position wrapper (`8001a918`, `24`), and
Position/alpha parent (`8001a93c`, `2b4`). They remain the existing source
continuations; this module does not reproduce their control flow. It also pins
both complete SDK getters (`802b76c4` and `802b76d4`, each `10`): load word at
object+8, extract the relevant ten-bit dimension and add one. Linked whole
IconInit/callback, Unpack, TEX and GX-object verifiers run too.

The [real Icon process](NATIVE_SOURCE_ICON_PROCESS.md) must have completed its
actual callbacks. `BattleWeaponIconTextures` reads that process's relocated
palette, count, descriptor and pixels. It calls the complete source TEX and
GXInit/LOD continuations over a caller-admitted 32-byte private output object.
The private address must be genuinely unmapped in the VM and every other native
view domain; it is a documented native address policy, never a guest stack
address, image index or fabricated source handle. Every initially unknown byte
becomes known through the real source clear and stores. Source memory has first
refusal for owned unavailable words.

The actual bank has 613 images, no TLUT, and formats I4, IA4, RGB5A3 and CMPR.
Only those admitted source formats and bounded original descriptor/pixel spans
are accepted. That CPU admission does not imply native atlas eligibility.
The unsigned-byte double-bias conversion is explicitly limited to the original
256 finite values. The actual source constant must be present. It is not a
general PowerPC floating-point environment implementation.

The source IconGX LOD override completes before LoadTexObj. A loaded snapshot
copies the real eight object words and descriptor and pins an independent long
source-resource proof. Later private-output reuse cannot change that snapshot.
SDK dimension reads still use the private object after Load; its held invocation
is released only after the actual whole quad is accepted.

## Source effects and packet boundary

The owner retains stable source service tables, source inputs and child cursors
through waits. Reentry is unavailable. Actual math and camera memory remain
caller providers, including the source-selected GP language and clock. No old
synthetic camera token, source-global substitution or index-as-handle policy is
used. The existing position parent keeps its original late borrowed-position
capture.

At the reached texture Load, the owner captures inherited ordinary GX state
before IconGX installs its own grayscale swaps. Ordinary icons require one
inherited stage; grayscale requires three. The immutable snapshot is retained
by the actual ordered publication resource proof. Later legitimate setters do
not invalidate earlier loaded geometry. Accepted icon setters update their
finite material/pipeline owner and are then observed by the shared source GX
history. Unknown linked world/map/light parents (`800208c0`, `800217c8`,
`8002b438`, `8001d968`) remain unavailable. Ordinary UI flags bypass these source
branches; they are never acknowledged as generic GX calls.

The original quad is copied with its source material, reached depth/blend/alpha
state, actual camera-8 projection/viewport/scissor/depth and loaded source
matrix. Projection tilt/view are applied once. Nonfinite geometry and a
conservative overflowing native clip domain are unavailable. `append` is an
atomic whole-quad owner: false consumes no packet or external prefix; true is
remembered before any retry. Packet completion is not a whole-frame seal.

Load also marks the genuine pending image pin and queries actual ready-only
native residency. A missing or unsupported catalogue image holds without
import, GPU work or source readiness. Imports must run in the explicit
pre-frame phase. Successful ordered append acknowledges only intent already
retained by that actual publication; a request/import alone is not an
acknowledgment.

## Binding and lifetime contract

Callers supply the actual admitted UI allocation, resource owner, camera epoch,
math providers, ready residency and ordered sink. Source callbacks execute
serially against a frozen camera/VM epoch. All callback contexts are pinned by
`textures.lifetime`; weak callbacks avoid context ownership cycles. The original
Matrix helper assumes the complete admitted raw resource and camera words are
available. It is not claimed to retain an arbitrary interrupted pure metadata
scan; CPU TEX/GX construction itself retains all optional child progress.

Short `sourceCurrent` is distinct from long `longCurrent`. Published resources
must independently pin and validate the actual Icon process plus native
import generations after camera completion and temporary child destruction.
An inherited GX snapshot alone does not prove Icon resource lifetime: retiring
Icon resources leaves unrelated global GX history intact. Do not retain loaded
texture snapshots inside their own long proof; that creates a cycle. Preserve
immutable ordinary evidence in the publication proof, independently of the
short camera context. UI/process retirement invalidates the corresponding
long proofs. None of these native guards writes guest End/readiness state.

## Validation and limits

Focused tests load actual original raw BIN/TPL and execute real NativeUi
Font/Window/Icon startup, whole TEX/GX constructors and source Matrix/Position
parents. They cover ordinary/grayscale restoration, all reached math and
conversion waits twice, missing real SDK constants, unavailable residency,
append waits, held input replacement, private-output reuse, reentry, owner
retirement and nonfinite input rejection. The packet fixture explicitly uses a
test SDK camera and host residency metadata. It proves source/packet behavior,
not actual CameraMain, queued UI ownership, C3D import or GPU pixels. The earlier
native importer probe has separate provenance and is not evidence of this new
CPU-object path running in a normal battle.

No original raw pack, ISO payload or compiled cache is added to source history.
Existing editable art and importer provenance are unchanged.

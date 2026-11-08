# Original Crump exclamation and question effects

This contract covers the US G8ME01 disc0 revision0 `evt_eff_fukidashi` calls
reached by the harbor beforebattle and afterbattle events. Original source
inspection establishes the callback, effect construction/update, attachment,
animation, draw requests and texture identities. It does not establish original
execution timing, Gekko numeric parity, native GPU equivalence or a complete
playable encounter. See [opening integration requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).

All 33 calls pass zero as operand4. **That operand is not a background-disable
flag.** The complete draw body unconditionally draws bubble textures5 and4
before dispatching effect kind0/1 to symbol texture6/7. Consequently all four
textures are required for the Crump scene. Operand4 is stored as variant data;
its observed display consumer is kind3's numeric payload, not the kinds0/1
used here. Retain it without inventing background suppression.

## Source identity and evidence

All twelve complete bodies below were independently re-read and SHA256 checked
against the read-only original ISO: 2,608 instruction words. Complete local-body
coverage is distinct from closure of every called SDK/GX/audio/file service.
`calc_pos` below is specifically the `eff_fukidashi.o` function.

| Function | US VA | Bytes | Original SHA256 |
| --- | --- | --- | --- |
| evt_eff_fukidashi | `8006c3c4` | `0x314` | `1784acdf35150f3076e423445bba0b013733d2d49e15fd389f49fb35872ee287` |
| effFukidashiDisp | `8013174c` | `0x175c` | `00e0b303b8defd986ec588b7c60fdaf4086ed82e701e7614ff3fa305f0dae2aa` |
| effFukidashiMain | `80132ea8` | `0x2bc` | `69f955364fa018fede56589a919bcf7e62107941ce8024263cfd213bb941f298` |
| effFukidashiEntry | `80133164` | `0x31c` | `c077f3128cafafa315ded5b48f21f933d7dc93a66e804da7717e565c485ceb8f` |
| calc_pos | `80133480` | `0x384` | `ade48368b6d0839d919c2457a1d0367809fbd776ddb5993527b089fe7a479dd4` |
| effGetTexObj | `8005ed58` | `0xb4` | `3ff2959894d37772b67585407cf956145ee6228689d3ad57103d0657d9a29c53` |
| effEntry | `8005ec68` | `0x68` | `482732598b7e4aa96b36a33981a3e3d05e9e06a61b1ed3c16b2c1ecccde47776` |
| effDelete | `8005ea30` | `0x4c` | `77774472b17e3c7dff6160e4dc4eb5321c1eb225c7e89405cc43a8ec37afeeec` |
| effSetName | `8005ebec` | `0x7c` | `06b484444f48b9c60a38bf96d697b1eabe75b60942a7dd46636ddb294bcb9e78` |
| effTexSetup | `8005ee0c` | `0x114` | `e8a424af988969ead9221bfdba948bdf01400eba90828dd88db6afdb36f452ff` |
| TEXGetGXTexObjFromPalette | `80037a80` | `0xdc` | `7c513078ebe80f85256108702b3b74d65b9036b6a1115b88c8a3750d19819fe6` |
| UnpackTexPalette | `80037b6c` | `0x120` | `21062a25695107ab6ea889d92fbfe7beb69ba3f5d8826a9415cf6ec7dda83d55` |

Private evidence consists of `artifacts/crump-fukidashi-bodies-evidence.json`,
`crump-fukidashi-verification.json`, the complete per-function `.disasm`
listings, `crump-fukidashi-data-evidence.json` and
`crump-fukidashi-fifo-quads.json`. These retain original span identities,
animation bytes/pointers, texture headers, constants and exact FIFO vertices.
They are static evidence, not a Dolphin observation or successful native draw.

## Original event requests

Offsets in this table are gor REL file offsets, not loaded addresses. Actor
keys and all independent inline delays remain those of the
[original harbor events](../story/STORY_HARBOR_OPENING_BINDINGS.md).

| Stream/site | Attachment mode | Kind | Actor/position | Duration |
| --- | --- | --- | --- | --- |
| Beforebattle26698 | 1, NPC | 0, exclamation | Original Goombella key14b28 | 30 effect polls |
| Afterbattle27134 | 0, Mario | 0, exclamation | Actual Mario owner | 30 effect polls |
| Afterbattle271c0 | 1, NPC | 0, exclamation | Original Goombella key14b28 | 30 effect polls |
| Remaining30 afterbattle calls | 2, fixed | 1, question | Individual original XYZ; radius30, angle0 | 60 effect polls |

All calls use an empty optional effect name and variant data0. The three
attached marks pass zero for the otherwise evaluated XYZ/radius/angle operands.
The 30 fixed marks run in their authored independent inlines; do not replace
those events, positions or delays with one aggregate crowd timer. Mode2 is an
attachment mode and does not select effect kind2.

The eleven event operands are:

| Index | Interpretation | Original decoder |
| --- | --- | --- |
| 0 | Attachment mode | evtGetValue |
| 1 | Optional instance-name pointer | evtGetValue |
| 2 | Actor name or raw actor identity | evtGetValue |
| 3 | Effect kind | evtGetValue |
| 4 | Variant data, stored at payload+3c | evtGetValue |
| 5,6,7 | Input XYZ | evtGetFloat, in that order |
| 8 | Radius/size parameter | evtGetFloat |
| 9 | Angle | evtGetFloat |
| 10 | Duration | evtGetValue |

The callback evaluates operands0..4, then5..9, then10, before mode dispatch.
Unused inputs are still evaluated. It ignores the first-call argument and
returns2 after construction/name processing; it does not wait for expiration.
The [operand routing](../event/EVENT_FLOAT_OPERAND_ROUTING.md) and original
integer namespaces remain authoritative.

For mode0, query Mario and read height-like field+1bc. Constructor floats are
`(0, (2*H)/3, 0, H/3, angle)` and attachment is sentinel-1. Preserve multiply
then division and the independent `H/3`; input XYZ/radius do not replace them.
Mode1 compares the actor name to original `me`: equality uses EventEntry+170,
otherwise `npcNameToPtr`. Its field+150 supplies H and the same float arguments;
attachment is the actual NPC pointer. No null-NPC fallback is supplied.
Mode2 passes the five input floats unchanged and attachment0.

Modes3/4 query ordinary/extra party and use attachments-2/-3. A missing party
skips construction. Mode3 party kind5 substitutes vertical offset30. These
recovered branches are outside the Crump call domain. Invalid modes construct
nothing. A nonnull, nonempty optional name and nonnull returned effect invoke
`effSetName`; there is no effect-pointer output operand.

## Construction, storage and sounds

The constructor acquires one actual 40-byte effect record, sets its class
string to `Fukidashi`, count1 and Main callback, and allocates an 88-byte
payload from heap3. No allocation-failure recovery exists in this local body.
Use the real [effect-pool admission](EFFECT_POOL_ACQUISITION_AND_MAP_PUBLICATION.md)
and [context lifetime](EFFECT_POOL_INITIALIZATION_AND_CONTEXT_RELEASE.md),
including actual field/battle context stamping; do not allocate a detached
visual object and acknowledge the original service before it is admitted.

Payload offsets are hexadecimal:

| Offset | Source field |
| --- | --- |
| 00 | Effect kind |
| 04/08/0c | Computed world XYZ |
| 10/14/18 | Stored input X, input Y+3, input Z |
| 1c/20 | Input angle and radius |
| 24 | Y spin, initially0 |
| 28 | Derived Z rotation |
| 2c | Derived auxiliary float; the complete display body does not read it |
| 30 | Remaining duration |
| 34/38 | Animation counter, selected signed frame |
| 3c | Variant data |
| 40 | Initial duration after negative-value handling |
| 44 | Attachment sentinel/pointer |
| 48/4c | Alpha255, camera ID4 |
| 54 | Spin phase0 |

Payload+38 and+50 are not explicitly initialized. Main writes the first
selected frame at38. Do not zero the whole payload as an original behavior
claim. A negative duration becomes10000 before writing30/40. The constructor
calls `calc_pos(payload,1)` after storing its inputs. Kind6 then writes Y spin24
to the original -450 constant; it is outside this scene's domain.

Construction also has sound effects. Actual NPC attachments with kind0 call
`psndSFXOn3D` using `SFX_EVT_NPC_BIKKURI_MARK1`; kind1 uses
`SFX_EVT_NPC_QUESTION_MARK1`. Their original string addresses are802f36bc and
802f36d8. Ordinary/extra party attachments use original numeric IDs93/94.
Attachment0 and Mario sentinel-1 skip this constructor sound. Thus Goombella's
marks require the implicit constructor cue; Mario and every fixed question
mark must not automatically play the same cue. Sound position comes from the
source's separate attachment resolution, not from a guessed screen anchor.
Original macro/sample/group admission remains with [AUDIO-01](../../requests/AUDIO-01.md).

## Attachment and camera-relative position

`calc_pos(payload,1)` queries camera4. It builds an axis from
`(sin(k*yaw), 0, -cos(k*yaw))`, with k equal to original binary32
`0x3c8efa35`. The camera yaw at+114 is read independently across the sin and cos
calls. Each result narrows to float. It calls `PSMTXRotAxisRad` with this axis
and angle `k * -((inputAngle-20)+20*secondArgument)`, preserving each original
rounded operation. The scene uses secondArgument1.

For each component, the resulting point is the stored input coordinate plus
rotation column1 times `(16+radius)`, with the attached actor coordinate added
in the original order when applicable. Mario uses8c/90/94; ordinary/extra party
uses58/5c/60; a real NPC uses8c/90/94. Preserve the FMA grouping and per-field
live reads. A missing party retains prior computed XYZ; there is no analogous
null-NPC fallback. Fixed attachment0 omits the actor contribution.

The helper also writes rotation28 as `(inputAngle-20)+20*secondArgument` and
auxiliary2c as `FMA(0.5,radius/12,0.5)`. The auxiliary value is not a draw-scale
request in this body. Do not multiply geometry by it merely because the value
resembles a size formula.

Main refreshes attached positions each poll. Fixed attachment0 retains the
point computed at construction, even if the camera subsequently changes;
drawing independently faces the current camera. The event's original fixed
XYZ therefore must not be treated as an unadjusted screen-space mark.

## Update chronology and duration

Follow the [effect driver](EFFECT_DRIVER_UPDATE_AND_TEXTURE_CACHE.md): one
callback per admitted record visit, actual context selection, source ascending
record traversal and possible same-pass creation eligibility. Rendering does
not tick the effect. Thirty and sixty here count effect polls, not milliseconds
or rendered frames. Original outer scheduling and measured cadence are separate.

Main first captures the old computed XYZ into a local depth vector. It then
reads initial duration40 and effect kind00. Duration0 chooses pointer table
80417220; any nonzero duration chooses80417278. Both contain seven pointers.
The signed frame is byte `animation[truncSigned(counter/2)]`. Main increments
the counter and stores the frame before interpreting it:

- Frame-1 deletes the effect and returns without display registration.
- Another negative frame resets counter to `(-10-frame)*2`, rereads the selected
  table at that counter, increments counter again and publishes the new frame.
- A nonnegative frame proceeds. For scene kinds0/1 with positive initial duration,
  remaining values below10000 decrement once; deletion occurs only when the
  resulting signed value is negative.

Initial30 therefore queues30 updates and deletes on the31st; initial60 queues60
and deletes on the61st. The clamped10000 sentinel does not decrement. These are
local call-count deductions, not observed half-second/one-second timing.

| Kind | One-shot sequence | Nonzero-duration sequence |
| --- | --- | --- |
| 0 | `0,0,1,2,3,5,4`, then frame5 through byte37; byte38=-1 | `0,0,1,2,3,5,4,5,-17,-1` |
| 1 | `0,0,2,1,3,2`, then frame3 through byte36; byte37=-1 | `0,0,2,1,3,2,3,-16,-1` |

Each byte lasts two driver polls. The negative branches in the nonzero-duration
sequences retain final frame5 or3 until lifetime expiration; negative bytes are
not texture-cell indices.

If attachment44 is nonzero, refresh position with `calc_pos(payload,1)` after
the animation/lifetime work. Register the callback with camera4 and mode8,
using `dispCalcZ` on the **previously captured** point. Display later rereads
payload XYZ. Moving attachment depth and final draw position can therefore
come from different source captures. Kind4's mode2 registration is outside the
current scene. Deletion frees the payload through heap3 then clears the effect
record flags, as defined by [effect deletion](EFFECT_SOFT_AND_HARD_DELETION.md).

## Texture resources and editable native assets

These calls use the primary `effect.tpl` bank, not the language-specific
`effect_us.tpl` bank and not `effect_n64.tpl`. For IDs below118,
`effGetTexObj` uses the primary manager payload and readiness field, then
`TEXGetGXTexObjFromPalette`. If not ready, the original constructs a 1x1 I4
fallback. That genuine source branch is not permission for a permanently
missing native resource to count as completed presentation.

Primary setup first queries archive resource `effect.tpl`. If absent, it forms
`%s/e/%s/effect.tpl`: source language0 selects `jp`, nonzero selects `us`.
The original external US file is `e/us/effect.tpl`, 266,784 bytes, SHA256
`3b62850fe75afb95eebb9fc240e20204d5ed320fba0fc956729f28405c2c789b`.
Archive/DVD readiness, palette relocation and texture ownership remain real
providers. The language-cache tail described in the effect-driver contract is
not this primary texture source.

| TPL index | Content | Source size/format | Header/data offsets |
| --- | --- | --- | --- |
| 4 | Bubble body | 32x64 IA4, no palette | 44c /4660 |
| 5 | Bubble shadow | 32x64 I4, no palette | 470 /4e60 |
| 6 | Exclamation strip, six cells | 96x32 I4, no palette | 494 /5260 |
| 7 | Question strip, four cells | 128x48 I4, no palette | 4b8 /5860 |

All four are checked-in editable original-derived PNGs under
[assets/original/effects/fukidashi](../../../assets/original/effects/fukidashi/atlas.json),
with per-image source index, descriptor, decoded-pixel/file hashes and licensing
in its [provenance](../../../assets/original/effects/fukidashi/provenance.json).
They are unscaled. The normal manifest overlay compiles the 256x128 RGBA8 atlas
and metadata as `fukidashi.t3x` and `fukidashi.atlas.json`; two-pixel padding and
original strip frames remain in the recipe. Source wrap/filter/LOD fields must
still inform the future consumer. Repacking texels does not reproduce material
state or justify sampling across neighboring atlas cells.

See [ASSETS](../../ASSETS.md). Original rights remain with Nintendo/Intelligent
Systems; asset tracking grants no redistribution license. No native fukidashi
consumer is installed by this import.

## Matrix, material and FIFO request order

Display receives camera ID and effect record, reads the payload and alpha, and
calls `mapSetMaterialFog`. Its pass count is1 when Y spin24 equals zero and2
otherwise; kind2 also forces2 outside this scene. Current Crump kinds0/1 begin
with zero spin and take one pass. Pass0 requests cull2; pass1 requests cull1.

For each pass, original matrix calls first build:

`M = cameraView * (T(point) * Ry(-currentCameraYaw) * S(.8,.8,.8)
                  * S(1,.8,1) * Rz(rotation28))`.

This notation describes the ordered products, not permission to regroup their
paired arithmetic or collapse captures. The initial query captures the camera
pointer. Its live view matrix at+11c is consumed by concat after the pass
matrix children; yaw is reacquired inside each pass. The original SDK
[affine/alias contract](../display/ANIMATION_MATRIX_STACK_AND_AFFINE_NUMERICS.md)
and [axis rotation contract](../display/ANIMATION_SDK_AXIS_ROTATION.md) apply.
The local body later appends fixed `S(1.3,1.3,1.3)` for the symbol only.

Before quads it installs current matrix0, identity texture matrix30/type1,
ZCompLoc1, ZMode(0,0,0), alpha blending(1,4,5,0), color update1 and
AlphaCompare(7,0,0,7,0), along with its source TEV/channel/texture-generator
requests. Vertex descriptors are direct XYZ and direct UV, vat0 float formats.
No color vertex stream or fabricated indexed character model is submitted.
There is no state-restore tail.

The first two quads are unconditional, before the kind dispatch:

1. Get/load texture5. Use source material-color word804295a0 with the payload
   alpha byte, then `M*T(1.3,-1.3,0)*Ry(spin24)` and upload position matrix0.
   Emit bubble-shadow quad.
2. Get/load texture4. Pass0 uses source word80422898 (`ffffff00`), pass1 uses
   8042289c (`a0a0a000`), replacing its alpha byte with payload alpha. Apply
   the source zero translation and Y spin, upload matrix0 and emit body quad.

The bubble quad has these four vertices in FIFO order. Every row emits XYZ
then UV as five float writes to `cc008000`; `GXBegin(0x80,0,4)` precedes them.
The U range0..2 is intentional and accompanies original source S-wrap2.
Applying mirror to the whole native atlas would sample neighboring images;
the consumer must reproduce wrapping within the selected original texture.

| Vertex | X,Y,Z | U,V |
| --- | --- | --- |
| 0 | -16,13,0 | 0,0 |
| 1 | 16,13,0 | 2,0 |
| 2 | 16,-19,0 | 2,1 |
| 3 | -16,-19,0 | 0,1 |

After these quads, append fixed scale1.3 and dispatch on effect kind. Kind0
gets/loads texture6 and uploads texture matrix `S(1/6,1,1)*T(frame,0,0)` to30,
type1. Kind1 uses texture7 and `S(1/4,1,1)*T(frame,0,0)`. The original generator
is `(0,1,4,30,0,125)`. Preserve generator/matrix/load order and the entire strip
identity before mapping a source cell into the native atlas.

For either kind, shadow alpha is the original signed integer reduction
`alpha*176/255`; foreground alpha is `alpha*255/255`. The shadow TEV color uses
raw word804295a4 with the computed alpha. Its position matrix is
`PSMTXTransApply(symbolMatrix,1,-1,0)`: this adds a translation to the already
composed matrix. It is not `PSMTXScaleApply` or a substituted object-space
`symbolMatrix*T(1,-1,0)`. Emit shadow quad first. Then choose foreground
TEV color word804228a0 (`ff000000`) on pass0 or804228a4 (`a0000000`) on pass1,
replace alpha, upload the unshifted symbol matrix, and emit foreground quad.
These foreground colors depend on **pass**, not whether the mark is a question
or exclamation.

| Symbol | Ordered XY vertices, all Z0 | Ordered UVs |
| --- | --- | --- |
| Exclamation | (-4,8),(4,8),(4,-8),(-4,-8) | (0,0),(1,0),(1,1),(0,1) |
| Question | (-8,16),(8,16),(8,-8),(-8,-8) | (0,0),(1,0),(1,1),(0,1) |

Both shadow and foreground emit the same corresponding quad. Thus the ordinary
Crump pass emits four quads per effect, including the two bubble quads. It must
not be reduced to a font `!`/`?` or a single texture rectangle without preserving
the source material, placement, shadow and sampling behavior.

Words804295a0 and804295a4 are mutable SBSS2 storage, not initialized DOL color
spans. Native bootstrap/write history must establish their actual contents;
do not manufacture file bytes or assume arbitrary RGB because the alpha is
known. Live constants, texture object state, fog and GX inherited history retain
their source capture boundaries. Full TEV request tuples for other kinds2..6
remain in the complete body; those variants must not be aliased to kinds0/1.

## Native continuation and lifetime requirements

A portable/native implementation must retain each admitted invocation's actual
Event/NPC/effect/camera/storage identities and reached reads across unavailable
providers. Repeating a held request must not replay pool allocation, sound,
frame increments, source stores or acknowledged FIFO prefixes. A genuine source
callback return2, update deletion or original not-ready texture branch is
distinct from an unavailable native child.

Effect updates must run at the source driver point, display registration must
own the actual queue entry and depth capture, and GPU drawing must not advance
life or animation. Keep actor mappings, payload, atlas, material and display
bindings alive until their real parent retires them. A map change or effect
flag clear does not itself prove that a queued native GPU borrow is finished.
Use actual renderer synchronization and callback retirement before releasing
physical textures or storage. The source has no general native cancellation
protocol; a retained implementation must state its supported retirement domain.

The small original atlas and four source quads permit reusable vertex storage
and Citro3D submission without per-frame allocations. Batch only when source
queue, transparency, matrices and material ordering permit it. Source architecture
or a successful build does not establish New2DS XL performance or pixel parity.

## Remaining integration and verification

The original source bodies and required art are available. Production still
needs the actual event-to-effect dispatcher, shared effect-pool/driver owners,
actor/height/camera bindings, primary TPL readiness, constructor sounds, original
material/quad consumer, display retirement and source outer scheduling.
Beforebattle currently retains and acknowledges an effect request without this
complete presentation backend; afterbattle is not yet a running native owner.
Asset import alone does not close either boundary.

Required checks include every operand decoder/hold, actor sentinel and `me`
branch; exact 30/60-poll lifetimes and held last frames; old depth versus live
position; immutable-versus-live camera/constant captures; all four texture
identities; bubble and symbol FIFO/UV order; pass-based colors and mutable SBSS
history; source sound admission without duplicate replay; pending deletion/map
retirement; and real native scene captures. Compare original execution in
Dolphin and record observations in [MECHANICS](../../MECHANICS.md). Measure actual
hardware CPU/GPU/memory behavior separately from these static deductions.

[DEMO-01](../../requests/DEMO-01.md) owns complete scene/effect requirements and
remaining original observations; [EVT-01](../../requests/EVT-01.md) owns retained
same-owner/provider integration; [AUDIO-01](../../requests/AUDIO-01.md) owns the
implicit constructor cue bindings; [TRACE-01](../../requests/TRACE-01.md) owns
matched original timing and presentation evidence. These parent requests remain
open. This contract does not establish a completed Crump battle or field return.


## Portable Main implementation (2026-11-07)

`FukidashiMainCursor` and `resumeFukidashiMain` now reproduce Main80132ea8
for all seven source kinds, using original-address byte tables and constants.
The new `fukidashi-effect.pmevt` manifest overlay contains the 15 initialized
DOL spans under `assets/original/effects/fukidashi/main/`. Its importer verifies
source hashes and requires an empty destination; alignment retains original
adjacent bytes. It neither writes the ISO nor supplies fabricated memory.

One cursor invocation is one effect-driver simulation poll. Inline storage
avoids per-update allocation. The animation/lifetime/spin prefix executes once
across held children. Position, genuine deletion, depth and display remain
explicit provider calls: depth uses old computed XYZ, mode uses kind after the
position child, and camera is captured only after depth succeeds. Display
retry preserves that complete tuple. Providers must retain actual child work
and queued consumers; this adapter cannot turn a missing provider into success.

Tests load the real pack and cover every table kind, duration loops, one-shot
terminal deletion, 30/60-poll expiration, sentinel lifetime, signed-counter
truncation, remaining-word wrap, kind6 spin, old/new position and retained child
arguments. These are host invocations of the recovered source contract. They
do not establish original execution timing, native rendering or a running
Crump scene. Entry/EVT decoding, attachment calculation, draw/material/sound,
effect-pool production routing and retirement remain open as listed above.


## Portable constructor and corrected scalar evidence (2026-11-07)

`resumeFukidashiEntry` now implements the complete constructor using actual
`effectPoolEntry`, heap3 allocation and held position/actor/audio providers.
Its inline cursor copies input float bits before any child call and retains
the real admitted record/payload without replaying allocation or cue selection.
Flags and the Main callback become visible before the position child finishes.
Native suspension must not invent an extra source driver visit to a partial
record; the composing owner preserves actual source ordering and storage
lifetime. Reset drops the continuation without deleting the admitted effect.
The shared initialized-data importer adds an `entry` component and normal
`fukidashi-entry.pmevt` overlay: four sound vectors, class/key strings, zero,
input-Y adjustment and kind6 initial spin, all pinned to read-only ISO bytes.
Existing Main imports retain their default behavior and exact output.

After position completes, fresh payload44 selects coordinate resolution.
Mario/party add their coordinates to original input XYZ; an NPC replaces XYZ
with its live world position. Party ID is retained across the pointer child.
A genuine null party pointer preserves original XYZ. Sound selection then uses
ORIGINAL kind/attachment arguments, not payload44: numeric93/94 for ordinary/
extra party, original string keys for NPC, none for fixed/Mario or other kinds.
Thus even a now-missing party can reach its original constructor cue. Any input-coordinate contribution uses original input Y. The NPC coordinate
branch instead replaces XYZ with live actor coordinates; neither stored Y+3
nor the computed effect point supplies the constructor sound anchor.

The source re-audit corrects prior prose rather than changing original data:
with r2=80427440, Entry3321c's b4f0 resolves80422930=3, not0.25;
calc_pos3351c/337b4's b510 resolves80422950=20, not25; and337cc's b4dc
resolves8042291c=0.5, not13. Kind6 uses8042294c=-450 after position.
These effective-address calculations and scalar bytes were independently
verified directly in the ISO. Constructor also leaves both frame38 and50
uninitialized. The display constants were rechecked; its initial camera
query retains a pointer, with live matrix values consumed later. Private
`artifacts/fukidashi-entry/source-order.txt` and
`display-constant-addresses.txt` retain full read/store order and source review.

Host tests exercise real Entry/Main/pool/deletion together with explicit
fixture position/audio children, plus allocation/position/actor/party/audio
holds, missing-party source results, original-versus-live attachment routing,
input-bit/provider identity and recursive-entry/reset rejection. Fixture
children do not prove original calc_pos or sound playback. Actual EVT
decoding/name admission, original attachment math, native material/audio
owners, GPU queue retirement and normal scene composition remain required.


## Portable attachment calculation (2026-11-07)

`resumeFukidashiPosition` implements the complete calc_pos80133480 body.
Its inline cursor retains camera, trigonometric inputs/rounded returns,
wrapped secondArgument*20, SDK axis/angle/matrix and actor/party identity
across held children. It never constructs a guest stack address or allocates
per update. All seven original initialized-data spans are available through
`fukidashi-position.pmevt`: axis vector, actual binary64 conversion bias,
radian factor, base radius16, angle offset20, divisor12 and auxiliary0.5.
The importer adds a `position` component; Main/Entry outputs remain unchanged.

The two yaw reads remain independent; a held sine call does not cause its
input to be sampled again. The matrix request captures axis/angle once, then
attachment is freshly selected only after SDK completion. Actual
[arbitrary-axis SDK rotation](../display/STAGE_LIGHT_AXIS_ROTATION_AND_NORMALIZE.md)
includes normalization even if a camera-derived axis appears to be unit length.
This differs from the char-axis wrapper used by display rotations. Do not
simplify the separately rounded `(angle-20)+20` for secondArgument1.

Each computed component freshly reads its radius, actor coordinate and stored
input in source order. Fused multiply-add remains one original operation;
hoisting all coordinates or replacing FMA with multiply/add changes behavior.
The missing-party branch leaves XYZ intact, then still writes the fresh
rotation/auxiliary tail. Raw bias is reread at both conversion sites, retaining
the signed wrapped product rather than recomputing it as an unbounded integer.

Host tests cover held camera/trig/matrix/party identity and captures, actor
variants, live radius/coordinates after the first store, missing-party tail,
signed product wrap, an FMA cancellation residue and negative-zero rounding.
Actual Entry calls this helper with the existing explicitly finite native SDK
adapter in a composition test. Fixture and finite native arithmetic do not
establish Gekko frsqrte/libm/FPSCR equivalence or GPU presentation. Independent
whole-body review found no concrete caller mismatch; SDK bodies/constants were
independently re-extracted and matched their existing contract hashes. Evidence
is private `artifacts/fukidashi-position/axis-sdk-source.txt` and its companion
body manifests. The existing local source contracts suffice; no new unknown
function was invented or marked closed by a placeholder.

Actual runtime registration, EVT operands/name routing, shared pool scheduling,
native audio/material/texture/queue retirement and scene playback remain open.
Main and Entry now have an actual portable position child to bind; successful
fixture composition does not install it into the normal Game path.

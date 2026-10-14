# Native animation texture state and atlas identity

2026-10-14 read-only cross-check of original US G8ME01 MODEL/TPL metadata,
existing converted animation packages and the native texture continuation.
The original [object requests](ANIMATION_TEXTURE_OBJECT_SETUP.md) and
[coordinate requests](ANIMATION_TEXTURE_COORDINATE_MATRICES.md) already specify
the complete local animSetMaterial_Texture8003dcfc graph. This document records
the bounded storage/backend bridge, not original GPU image equivalence.

## Original T24 state versus PMANIM3 and PMANIM4

The source texture state has24-byte records: an index byte at+0 and floats
tx,ty,sx,sy,angle at+4/+8/+c/+10/+14. Original Main accumulates index-byte
deltas modulo256 and adds floating shifts to tx/ty at crossed keys. These
texture streams have no fractional interpolation; see
[compressed delta cache](ANIMATION_COMPRESSED_DELTA_CACHE.md). Source padding
bytes are not new material fields.

Native shape.textures holds the evaluated index bytes. In PMANIM4,
shape.surfaces[slot].transform retains the five live float lanes. Its wrapS/T
members are separately derived sampler state, not original T24 fields. The
cache copies base transforms and applies crossed-key tx/ty shifts, preserving
scale/angle. It must not use a precomputed legacy UV transform instead of these
live source lanes.

PMANIM3 omits surfaces and UV-shift payloads. The source converter admits that
format only after checking every original base transform equals(0,0,1,1,0) and
every authored key UV shift equals(0,0), for each converted clip. Empty surfaces
are therefore justified by that package's conversion/provenance, not by the
absence of a native field alone. A new or edited source outside those predicates
needs PMANIM4 or an explicit provider boundary. The native helper reads the raw
MODEL base floats when surfaces are absent; current admitted packages satisfy
the required identity/no-shift domain.

The current source-backed coordinate driver retains the exact negative-angle
degree coefficient3c8efa35, R*M*S grouping, pivot requests and literal sy!=0
translation gate. Even an identity base state requests an identity translation,
then generator before matrix load. The older ActorTextureTransform convenience
helper uses a different coefficient for nonzero angles; it is not the numeric
provider for this new source continuation. Current packages' zero angles do not
exercise that legacy difference.

## Palette mapping is global within one converted package

compile_mesh uses one TPL-image-to-cell table across all its converted poses;
compile_surface_mesh keys cells by(TPL source,image index,wrapS,wrapT).
Per-pose palettes reference that shared table. The table is not global across
different packages: field and battle Mario can both name a_mario while packing
its images into different cells. Texture-name equality cannot select an atlas.

A read-only check traversed every nonempty palette entry(slot,state), resolved
raw MODEL texture-base first+state, then its descriptor's TPL image index, and
compared the package metadata's cell. Every mapping agreed across all poses:

| Package | Format/clips | T24 slots | Nonempty mappings checked | Distinct image/cell keys |
| --- | --- | --- | --- | --- |
| trio-attack-mario | PMANIM3/15 | 17 | 310 | 54 |
| trio-attack-goombella | PMANIM3/10 | 10 | 129 | 28 |
| professor | PMANIM4/17 | 12 | 201 | 15 |

All currently converted base transforms were identity, all key UV shifts zero,
and all base wrap overrides0. Every used original image was CMPR format14,
without a palette, with clamp0/0, min/mag filter1/1, edgeLOD0 and min/maxLOD0.
These are finite resource witnesses, not permission to normalize other samplers.
For example Goombella slot9 state0 resolves descriptor44/TPL44/cell0; its
PKR_D_1 state1 resolves TPL45/cell10. A missing palette entryffff must hold.
Original out-of-range descriptor diagnostic/fallback remains a separate branch;
do not treat a missing converted cell as the original fallback image.

## GPU dimensions and UV composition

Each logical atlas is1024 wide, using128-pixel cells with a124-pixel decoded
image and2-pixel gutters. Mario/Goombella runtime images are subsequently
halved; Frankly is not. Current package dimensions are:

| Package | Logical crop height H | Actual GPU width/height | uvScale=1024/H |
| --- | --- | --- | --- |
| trio-attack-mario | 1024 | 512/512 | 1 |
| trio-attack-goombella | 512 | 512/256 | 2 |
| professor | 256 | 1024/256 | 4 |

Apply the actual source T24 matrix to original UV first. For resulting(u,v)
and selected cell c, the final native normalized atlas coordinates are:

```
U = (128*(c%8) + 2 + 124*u) / 1024
V = 1 - (128*(c/8) + 2 + 124*v) / H
```

c/8 is integer division. Equivalently compute logical V with denominator1024,
then apply1-(1-V)*uvScale once. Halving both pixel dimensions preserves these
normalized coordinates. Apply neither a second height correction nor a second
V inversion. Resampling/gutters are native texture adaptations, not lossless
original pixel/LOD equivalence.

## Real source and GPU owners

Unchanged original TPL companions use suffix.draw-textures.bin next to the
pose-directory/raw-model packages. AnimationTextureResource validates and retains
image/palette header projections and original file offsets. Its temporary raw
TPL byte vector is discarded after parsing. Image-data and palette-data offsets
are provenance offsets, not live host pixel pointers or an owned relocated TPL.

The actual images for the current native path live in resident converted C3D
textures. Mario/Goombella use Renderer::trioMarioTexture/trioPartnerTexture,
admitted by prepareSceneActorTextures before an active trio frame. Frankly uses
the professor entry in Room::actorModels; loadFieldActors explicitly requires it
for room5. This is distinct from professor-map(the icon) and professor-mario.
The current texture backend admits only the actual retained Mario/Goombella
resource package pointers and expected resident atlas dimensions; Frankly's
generic helper graph can advance only as far as its real unbound backend.

Bind original TPL identity, raw descriptor index/TPL image, selected package
palette/cell and resident GPU package together. Keep the GPU resource alive
until submitted commands complete; CPU model/texture reference counts alone
do not establish GPU residency. Room/scene retirement and reacquisition must
respect the existing [GPU texture lifetime policy](NATIVE_SCENE_ACTOR_GPU_TEXTURE_LIFETIME.md).

## Request order and retained admission

The reviewed driver completes **all** reverse-slot first-pass image work before
starting its second reverse-slot coordinate pass. Kind0 requests two distinct
TEXGet calls; the first determines the ordinary/CI branch. CI order is TLUT
initialization, CI texture initialization, LOD, TLUT load, texture load. Ordinary
order is texture initialization, LOD, texture load. Nonzero kinds skip object
loading but still enter the coordinate pass. Queued GX and SDK matrix arguments
remain captured across holds; coordinate float lanes are read at their actual
second-pass capture point.

Native object initialization/LOD prepare typed state; LoadTexObj requires the
matching converted palette and real resident image. Only coordinate0, kind0,
CMPR, clamp, linear filtering, no CI and no LOD are currently admitted. Generic
CI/multislot requests do not imply native submission. The source invalid-index
diagnostic, changed resource/context domains and absent texture ownership hold
explicitly. No fan geometry, ordinary callback/history or queue completion is
acknowledged by finishing this texture helper alone.

## Evidence and remaining requests

Private `artifacts/battle-display-queue-evidence/convertedPoseTextureAtlasMappingCheck.json`
retains package hashes/dimensions and original TPL identities for this check;
`goombellaSlot9Texture44Metadata.json` retains the concrete image-header witness.
They are temporary inspection evidence, not tracked source assets.

| Original TPL | Bytes | SHA256 |
| --- | --- | --- |
| a/a_mario- | 731296 | 2ccf4f212340bb9cce33cd8457ac92a24ca3fc0db4fc95237224775e9534845e |
| a/c_pkr- | 90432 | 162b5151601b129fc667f51b90ac6124de863ab2dff79b997cd143ed4d5c4b88 |
| a/c_kuri_h- | 24896 | c4114e4c5991d0cc19fd9c5b97849369cc5eb361b7e723d3f2ee1b69135e12ef |

Existing local texture-object/TEXGet/coordinate contracts need no duplicate
request. Remaining [EVT-01](../../requests/EVT-01.md) work includes explicit
native binding for further resource packages and sampler domains, full original
GX texture-object/LOD/load internals where needed, raw image/TLUT ownership for
future upload providers, and original frame comparisons. Native GPU-cache
commands are separately covered by the [Citro3D adapter](CITRO3D_TEXTURE_CACHE_AND_VIEWPORT_COMMAND_ADAPTER.md).

# Original surviving Trio enemy pose resources

This producer supplies model-specific resource data; it does not acknowledge a
source Shape call, source workspace allocation, native GPU residency or a camera
queue. The existing underground presentation bank contains flattened field
geometry and is not a substitute for these original MODEL draw tables.

## Source dependency and provenance

`tools/trio_enemy_pose_assets.py` opens the US original ISO read-only through
`iso_assets.extract`. Original `rel/tik.rel` SHA256 must be
`ca30b06f0b7176dbe364349f239ef3ddfe66650e50ea1246921bd2a368a9dac7`.
The producer resolves local ADDR32 relocations from actual setup4 records at
file offsets263a0/263d0/26400(hex). Kind+b4 is a **byte** part count; kind+b8
selects the part, part+8 the model name and part+48 the pose table. The actual
`packedBattlePoseName(part,28)` walk selects the first tag28 row, stopping before
any later rows. Its tag>=70 termination and first exact/fallback ordering are
preserved; the producer does not infer the entire table extent.

| Runtime unit | Kind/part file offsets (hex) | Pose table | Model | Initial type28 |
| --- | --- | --- | --- | --- |
| 3 | 2a7d8 / 2a974 | 2a89c | c_kuribo | KUR_S_1 |
| 4 | 30c30 / 30d1c | 30e28 | c_kuribo | TGK_S_1 |
| 5 | 2f578 / 2f724 | 2f770 | c_kuribo | PTK_S_1 |

All three share **one** original model/texture bank, including45 original
ordered descriptors, starting with `KUR_Z_2`; the original KUR/TGK/PTK names
remain distinct. No replacement idle name, pose alias or invented model is used.

| ISO asset | Disc byte offset | Bytes | SHA256 |
| --- | --- | --- | --- |
| a/c_kuribo | 886507548 | 125524 | 9b934dee83eb489f5151d8a497ac59611e402b7db786c9e82b4809d7d7a475e4 |
| a/c_kuribo- | 886633072 | 23680 | bccf3f47fd1f713cf8c86a162f9b8a2f86af0b4398a9806e57b114baaf9fc692 |

The manifest retains per-file source provenance, all descriptor names, role
pointer offsets, original TPL image headers and product hashes. Original rights
remain with Nintendo/Intelligent Systems; local extraction grants no new license
or redistribution permission. ISO and generated packs stay outside history.

## Products and source binding

Run `python3 tools/trio_enemy_pose_assets.py --output
local-assets/trio-enemy-native/pack` for an isolated private pack; the ordinary
converter default is ignored `romfs/`. Prefix `trio-enemy-kuribo` produces:

- `.pmanim`: all45 CPU sparse clips, PMANIM4 surface/material metadata.
- `.pose-directory.bin`: PMPOSE02 with original descriptor order, MODEL+c4 and
  exact original discrete time/value words.
- `.draw-model.bin` and `.draw-textures.bin`: unchanged original MODEL/TPL bytes.
- `.t3x`: dedicated RGBA8 converted atlas; `.bin`: PMSUV1 plus LE float4 UV scale.
- `.json`: source/provenance/conversion/product and atlas evidence.

`TrioEnemyPoseResources::load(directory,pack)` reuses the existing immutable
`loadBattlePoseResource` cache. Additional admission requires both raw companions,
matching model/texture/flags, complete source directory/discrete metadata, every
converted clip with original position/normal/node/visibility/group domains and
valid original texture descriptor indices. Captured textures remain unavailable.
`find(actualModel,actualInitialName)` returns the same shared resource only for
an exact source model/pose name. It does not select a pose or register a unit.

A Runtime binding must read the existing original kind/part names and pose28
selection and create separate real mode1 pose handles for ids3/4/5, storing each
handle/generation at the existing part owner. Existing source pose initialization
and source process/Shape traversal must still run and allocate their actual
buffers. CPU sampling or evaluated triangles do not acknowledge those calls.

## Native atlas ownership plan

Original TPL has14 images, all CMPR(format14), no palettes, original clamp wraps,
linear min/mag filters, zero LOD/bias/edge. This matches the existing narrow
`finishingAnimationTexture` material admission. Conversion uses the existing
surface packer: images resize to124x124 with Lanczos, each gets2px clamp gutters
inside128px cells. The logical1024-square atlas crops to1024x256 without further
resizing. Fourteen cells occupy two rows; original per-image dimensions and data
remain in raw TPL, never replaced by atlas dimensions for source queries.

The native renderer must retain the exact validated resource pointer alongside a
**dedicated** `trioEnemyTexture`, UV scale4 and successful RGBA8 import. Bind by
resource pointer and actual selected descriptor/slot/state via
`animationTextureCell`; texture-name equality or `undergroundTexture` is
insufficient. Import before FrameBegin, reject missing/import-failed/wrong-format
or mismatched dimensions, keep source material/sampler requests in their original
order, retire retained fan/atlas references before synchronized texture release
and release while C3D remains alive. Share this one atlas among three real poses.
Do not regroup their original camera4 callbacks or defer them behind camera8.

`artifacts/trio-enemy-pose-integration-journal.json` proposes exact unique-anchor
changes for root review: original asset load, mode1 enemy resource selection,
Renderer dedicated residency/release/identity routing, and Make conversion. It is
not applied or native-validated by this producer. New module/test CMake entries,
actual source buffers and the active battle scheduler remain integration work.

## Focused validation

Private original conversion completes all45 descriptors. Four Python checks
verify pinned REL/model dependency evidence, all directory/order/product hashes,
unchanged MODEL/TPL companions, original14 header records, atlas metadata and
unavailable/truncated input rejection. A focused C++ test verifies exact shared
resource lookup, original45 descriptors, missing/wrong metadata rejection and
three independent CPU pose owners. CPU evaluation is explicitly detached proof.
ARMv6k syntax passes for the new resource module. ASan/UBSan also passes the focused native-resource CPU test (leak scanning disabled). Root owns full make test,
make all, actual source callback traversal and native pixel/lifetime validation.

## Applied integration and validation

Root applied the proposed journal in e78d2325: actual source model/pose lookup,
three separate mode1 handles, shared validated resource, dedicated native atlas
residency/identity/retirement and Make conversion. Host and ARM checks verify
all45 original descriptors and separate CPU poses; existing allocation/lifetime
tests now require all enemy owners and their retirement. This does not establish
source Shape completion, active camera4→HP8 or visible battle pixels. Exact full
gates, provenance and isolated captures are in [progress](../../PROGRESS.md).

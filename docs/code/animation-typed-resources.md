# Typed animation resource and texture adapters

[MODEL import](../../source/display/animation_draw_resource.cpp),
[TPL/texture continuation](../../source/display/animation_texture.cpp),
[typed traversal](../../source/display/animation_draw_traversal.cpp) and the
[resident texture bridge](../../source/display/render_animation_texture.cpp)
provide bounded native resource paths. They are distinct from the
[actual-VM renderer](animation-native-render-owners.md) and its source reads.
Field offsets below are hexadecimal; byte sizes and counts are decimal.

## Serialized resources retain file identities

MODEL words are BE file-relative offsets. Successful import admission precedes
asserted word reads. Table span checks divide remaining bytes rather than
multiplying untrusted count×stride. Names, shapes, draws, groups, directories
and reachable index streams receive native bounds. Group ceiling65536 and
reachable depth31 are importer policy, not original runtime guards. The last
group is root. Cycles/shared revisits reject this typed import; the raw source
traversal does not turn a reached cycle into successful empty work.

Index words narrow to LOW16 before shape-local bases are added, with widened
bounds to prevent malformed arithmetic wrapping into valid tables. Up to eight
UV slots and cull indices0..3 bound this adapter. Empty original UV streams can
retain non-address sentinels because no selected index is read for zero count.
read() clears the mutable object and can leave partial state on failure; load()
publishes no immutable shared owner until all admission succeeds.

TPL import retains image/palette header projections and file offsets. The raw
byte buffer used to validate base image and palette extents is temporary.
data/paletteData are not owned pixel buffers, relocated VM pointers or GPU
allocations. Recognized GX tile layouts,16MiB/4096-image caps and sampler checks
are native loader bounds. See [texture-state identity](../behavior/display/NATIVE_ANIMATION_TEXTURE_STATE_AND_ATLAS_IDENTITY.md).

## Texture requests preserve two walks and captured children

The typed helper first completes reverse-slot image/object work for every slot,
then walks reverse slots again for coordinate matrices. Two TEXGet calls stay
distinct: the first captures palette presence, the second supplies image data.
CI order is InitTlut, InitTexObjCI, LOD, LoadTlut, LoadTexObj. Ordinary order is
InitTexObj, LOD, LoadTexObj. Nonzero texture kinds skip object work but still
reach coordinate setup.

Negative wrap override preserves the original TPL sampler. Otherwise bit0/1
select S/T repeat, with bit2/3 forcing S/T mirror over repeat; zero means clamp.
T24 transforms are tx,ty,sx,sy,angle. Rotation uses negative degrees around
(.5,.5), then translation and scale in retained grouping. The literal sy!=0
translation gate can request an identity matrix. Keep (1-ty)-sy and its rounding.
Generator setup precedes matrix upload; GX IDs30+3*j, identity60 and post125
retain their source coordinate meaning.

A live source transform failure holds. MODEL base fallback is limited to legacy
packages without live surface storage; PMANIM3 omission requires identity/no-shift
conversion proof. Pose/resource identity is checked on resume, but changed draw
or service arguments are not independently compared. Callers keep the request,
backend and producer lifetime stable through holds. Queued GX and SDK requests
latch separately. Texture-helper completion is not a fan draw or GPU fence.

## Converted package identity selects the resident atlas

A cell is admitted only when selected pose, source slot/state, descriptor kind,
TPL image index and converted palette agree. State has256 entries per slot; the
logical grid has8×8 cells. Missing ffff cells hold rather than becoming an
original descriptor0 fallback. Equal texture names across packages do not prove
atlas identity.

Native accepted atlas widths are512/1024 with exact crop ratios1/2/4/8. The
renderer explicitly matches retained resource package pointers to their resident
atlas and UV crop. It supports only the documented coordinate0 CMPR/clamp/
linear/no-mip subset. CPU decode or a generic CI request does not establish a
GPU provider. On a rejected bind it restores the previous logical atlas; the
resident object's native filter/wrap settings were already applied. GPU lifetime
barriers remain necessary through command consumption. See
[native texture binding](../behavior/display/NATIVE_ANIMATION_TEXTURE_BINDING.md).

## Typed traversal keeps copied context and a narrower branch domain

The typed traversal captures context and compares only handle on resume. Caller
ownership keeps resources, pass, providers and pool generations consistent.
It snapshots24 node floats before SDK planning; the raw node controller instead
retains interleaved12/21 source reads. These are different storage adapters.

Any pose flag20 currently holds the typed route. It cannot assume an unmatched
raw special-group test or acknowledge an absent material/Vivian provider. The
actual-VM group owns that complete branch. Child and sibling parent tokens both
use the current group; invisible groups still follow siblings. Stack restoration
precedes sibling traversal. Captured callbacks may mutate the typed matrix while
held, so their original request must resume rather than being reconstructed.
Existing [node planning](../../source/display/animation_node_draw.cpp) and
[buffer recovery](../../source/display/animation_buffer_refresh.cpp) comments
explain their fixed workspaces, asymmetric gates, scratch offsets and retained
recovery paths. Local resource admission does not prove original pixels,
scheduling, numeric parity or complete runtime ownership.

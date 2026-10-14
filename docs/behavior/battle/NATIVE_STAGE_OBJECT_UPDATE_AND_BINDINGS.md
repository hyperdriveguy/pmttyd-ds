# Portable stage object update and original named geometry bindings

The [source contract](SETUP4_STAGE_OBJECT_CACHE_AND_NAMED_MAP_REQUESTS.md)
now has a portable StageObjectMain implementation in battle_stage_objects.
It captures the current battle owner, scans all32 records and skips only signed
nonpositive ordinals. It preserves position/offset/additional-translation/angle
cache comparison order, raw copies before providers, forcebit2 consumption and
shared layer2 orientation-cache writes. Float equality preserves signed-zero
cache bits and treats NaN as dirty.

Dirty layers0/1/4/5/6/7 request total translation then source angles. Layer2
requests translation, negative offset-Y pivot, global orientation and positive
pivot; layer3 uses position-Y pivot and record angles. Total translation keeps
two single additions peraxis. Current descriptor/name is reloaded for each
map call. Layers>=8 skip transforms but still synchronize visibility. All
active records independently query group flags and request only the needed
mask1 change. Final BattleObjectConfig is the source complete blr.

The boolean provider result means native resource unavailability. It is not
an original return gate, missing-name reinterpretation or success fallback.
Cache/force writes already performed remain when a provider is unavailable;
there is no invented rollback or retry-dirty bit. Real map-group callbacks,
additional-matrix latch/recalculation, authored/animation local hierarchy and
complete used initializer remain separate services.

## Original converted object ownership

The original stage converter now exports PMHOBJ02 metadata for selected owners
isi_1,isi_2,A1,B_mati,C, preserving authored ancestor matrices and the existing
source placement bake. All3393 stage vertices are covered exactly once by
456/360/1488/1083/6 bindings. This is actual original geometry membership,
not a synthetic division of the room. Metadata retains local and original
positions and vertexindices; parent B Z=-.8/C Z=-1 nativeunits remain.

Background converter now also hides source builtin spotlight, and retains
escape_wall/spotlight metadata as two zero-draw-vertex named owners. Original
Init marks both hidden; a real map lookup can therefore own these flags even
though their geometry is excluded from this pack. See source contract for
escape-wall Y200 override and spotlightY240. No missing map name is treated
as an empty successful initialized registry.

Metadata is copied to romfs/bti_01.houseobj and romfs/stg_00_1.houseobj.
All original/private packs remain ignored. Existing rendered stage geometry
still uses the baked selected placement. The native engine still holds at
StageObjectMain until the actual map/renderer provider consumes these bindings;
this portable implementation alone does not acknowledge native completion.
Undo/account for the bake exactly once when composing new additional matrices.

## Verification and remaining work

Tests cover all32 slots, signed inactive records, every layer0..8, all vector
inputs, distinct pivots, shared layer2 orientation consumption, clean-cache
passes, raw signed-zero/NaN behavior, forcebit2 and visibility independence,
callback changes to current name/angles/flags, and unavailable providers with
retained early cache writes. Original private metadata tests reconstruct
geometry coordinates, verify each vertexindex exactly once and retain empty
hidden owners and authored B/C translations. Full command results are recorded
in ../../PROGRESS.md. No original-runtime/GPU/hardware equivalence claim.

Next native named-map additional matrices and live vertex uploads, source
frame-latch reset/recalculation, actual rank0 seven-record initializer and
upstream animation/force writers. Then icon/stage spotlight/help/audience/
slot/display services, battle teardown and field sequence return.

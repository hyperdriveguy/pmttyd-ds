# Native field pose resource bridge

This describes the reviewed native implementation in FieldPosePool and the
BattlePosePool leaf additions. Original source contracts are
[field NPC pose leaves](ORIGINAL_FIELD_NPC_POSE_LEAF_SERVICES.md) and
[map registration](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md).
No new validation result or complete native area/NPC integration is asserted.

## Owned resources and admission

FieldPosePool owns a catalog keyed by exact original model name and a separate
pose pool. Installation requires original MODEL and TPL companions, model and
texture identities, original model flags, full ordered descriptor names and
discrete timelines, a converted CPU bank, and a usable descriptor0 CPU sample.
It checks draw-resource identities/count against the directory and ordinary
texture references against the retained TPL resource. These are actual retained
resource objects, not fabricated source file pointers or SDK allocation ABI.

The existing Professor exporter provides an immediate real resource bridge:
original a/c_kuri_h and a/c_kuri_h-, PMPOSE02 descriptor metadata, unchanged
.draw-model.bin/.draw-textures.bin companions and converted CPU animation.
Source descriptor0 is Z_1. The source tribe initial/stay/talk fields+8/+10/+14
are S_1/S_1/T_1. Lowercase d_1 belongs to distinct tribe fields+20/+24 and
falls back to descriptor0 when resolved case-sensitively against this bank;
it is not the initial setup pose request.

canEnter checks supported mode0..2, installed resource, available pose slot,
and model/texture cache capacity. enter passes the exact requested model name
to the underlying pool. Unavailable resources/capacity remain unavailable;
absence is not mapped into a successful pose handle. animationReady uses the
original descriptor-name lookup rule: exact name match, otherwise descriptor0,
and requires that selected converted clip to exist. A known descriptor whose
clip was omitted remains unavailable; it is not replaced with an arbitrary
idle pose.

Catalog/resource owners must survive all live poses. Duplicate installation
accepts the same retained resource object and refuses replacement by another
object under the same key. poses() provides const inspection only; all entry passes through the catalog
so a different same-name resource cannot bypass its readiness invariant.
Model/texture sharing and release stay with the underlying retained pool.

## Constructor leaf state

paperOn ORs8 into the live pose's main flags. materialLightFlagOn ORs its
argument into pose+ec. vivianType returns the signed word at pose+11c.
Invalid/unallocated handles return unavailable instead of asserting that a
source unchecked pointer was valid.

Entry captures the original requested model name before choosing/reusing a
cached model resource. Every admitted Entry overwrites pose+11c/+120 using
the source request identity: tst_vivi→1/80415da0, c_vivian→1/80415da4,
c_maririn→2/80415da8, c_majyorin→3/80415dac, otherwise0/80415dc4.
The source addresses remain guest identities, not host pointers. This mapping
also resets reused slots; default fresh zero storage would not suffice.
Native code retains the source tail ordering of fc before f8 and type/pointer
stores before clearing154/158. It does not clear an entire reused scalar
record or invent an initialized paper-facing sign+78.

height/radius require the retained original draw resource, read signed32
MODEL+cc/+c8, convert to rounded single and multiply by2 with another single
round. They return original engine units without native .01 scale conversion.
Tribe dimensions, rendered geometry bounds and model flags are separate
quantities. The later map producer's tribe-dimension overwrite does not erase
the constructor's earlier source query requirement.

## Constructor fixture and remaining frontier

The scoped constructor composition uses the original packed NPC constructor
with explicit fixture-owned memory/allocation and these real Professor pose
resources. That can exercise genuine resource-backed leaf requests without
claiming that Game's finite NPC presentation catalog has become the original
registered actor pool. This document records the implementation boundary;
validation results must be supplied by the implementing session separately.

Paper-on intentionally leaves the pose with flag8. Existing pool Main does
not acknowledge its missing facing tail merely because CPU sampling succeeds.
The field-facing provider, complete Main scheduling and retained clocks,
actual GPU publication/readiness, and original global field/battle allocation
history remain separate. The new field pool is its own native handle namespace.

Actual Game registration still needs original tagged-map construction, stable
actor records/names/descriptors, source setup/init/regular event ownership,
sound/battle services and room/deletion lifetime. The six additional gor02
banks c_deale, c_joho_b, c_luigi, c_binbo, c_roten_g and c_bomt_n and the
hoshi constructor fallback are not made available by the Professor bridge.
Existing selected field packs also need complete admitted source sidecars.

Future exports can reuse battle_pose_directory.write_directory on unchanged
original model/TPL inputs, preserve full descriptor order and descriptor0,
and compile the actual requested finite clip set. Include directory/raw
companions in required products and conversion fingerprints. Keep GPU uploads,
model/texture reference counts, per-pose CPU state, allocation failure/fallback,
and release/room lifetime explicit; resource export alone cannot acknowledge
those services or the original map initialization parent.

Subsequent [map resource exporter](ORIGINAL_MAP_NPC_RESOURCE_EXPORT.md) now
supplies complete source companions/CPU banks for all12 gor02 models and hoshi,
including the six banks absent from the finite presentation exporter above.
This removes that asset prerequisite; actual Game-owned actors and facing/Main/
GPU/lifetime providers remain pending. Professor-only bridge fixtures are not
being reclassified as complete map registration.

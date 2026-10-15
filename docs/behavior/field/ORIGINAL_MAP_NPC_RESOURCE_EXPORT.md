# Original map NPC resource export

The native exporter tools/field_npc_resource_assets.py derives resource
dependencies from original tagged-map data and the first matching DOL tribe
row. It supplies real CPU animation/raw resource prerequisites for the
[field pose bridge](NATIVE_FIELD_POSE_RESOURCE_BRIDGE.md); it does not
acknowledge [map registration](ORIGINAL_MAP_NPC_REGISTRATION_AND_POSE_OWNERS.md),
NPC setup, facing Main or GPU readiness. Original source identity is US
G8ME01 disc0 revision0, read-only ISO input.

## Source census and shared banks

For each --area, read m/AREA/s, retain its exact actor/tribe names, and scan
npcTribe803316d0 in0x54-byte steps until the null-name terminator. Exact
Shift-JIS first-match semantics match npcGetTribe; duplicate tribe names do
not overwrite the first row. The manifest records source row address/hash,
model identity and all eight pose-role fields. Resource banks are shared by
exact original model name across the selected maps. The real hoshi fallback
resource is included independently of tagged tribes.

The original gor02 fifteen actors require twelve distinct model banks; hoshi
adds the thirteenth. Their full original descriptor census is:

| Model | Descriptors | Descriptor0 |
|---|---|---|
| c_deale | 13 | Z_1 |
| c_kuri_h | 17 | Z_1 |
| c_joho_b | 5 | Z_1 |
| c_binbo | 5 | Z_1 |
| c_monban | 16 | Z_1 |
| c_bomt_n | 41 | Z_1 |
| c_kuribo_n | 5 | Z_1 |
| c_tyusan | 6 | CHU_Z_1 |
| c_roten_g | 5 | Z_1 |
| c_borodo_n | 5 | Z_1 |
| c_f_mura | 13 | Z_1 |
| c_luigi | 36 | Z_1 |
| hoshi | 6 | HOS_Z_1 |

The total is173 original descriptors, with unchanged ordering. This census
is derived from the source resources rather than the finite native visibility
catalog. Initial source model files and their original texture companions
are retained with source-path/content provenance.

## Pose requests and case-sensitive fallback

Role fields are descriptor+8 initial, +c separate pose role, +10 stay,
+14 talk, +18 walk, +1c run, and separate roles+20/+24. The ordinary gor
tribes request S_1/S_1/T_1/W_1/R_1 at initial/stay/talk/walk/run, except
Chu's CHU_S_1/CHU_S_1/CHU_T_1/CHU_W_1/CHU_R_1. Field+c is Z_1,
or CHU_Z_1 for Chu.

Roles+20/+24 are D_1 for c_monban/c_bomt_n; lowercase d_1 for c_kuri_h;
Z_1 for c_deale/c_joho_b/c_f_mura; S_1 for c_binbo/c_kuribo_n/c_roten_g/
c_borodo_n/c_luigi; CHU_S_1 for Chu. Professor's d_1 is absent from the
original case-sensitive directory and therefore selects original descriptor0
Z_1. It must not be aliased to uppercase D_1. Professor's initial/stay/talk
remain S_1/S_1/T_1; the lowercase requests belong to other roles.

Hoshi's original directory is HOS_Z_1, HOS_S_1, HOS_S_2, HOS_S_3,
HOS_A_1, HOS_A_2. Its texture animation is retained with its source state
changes rather than replaced with static fallback art. Full descriptor export
also retains additional original init/story poses beyond the ordinary role
set; their event/world providers remain separate.

## Exported products, cache and ownership

Each bank publishes all descriptors as PMANIM4 CPU clips and PMPOSE02 original
directory/discrete/model-flag metadata. Unchanged original MODEL and TPL
companions preserve the source dimensions, hierarchy, draw/index/material
data and texture identities. The surface conversion retains source geometry,
animation states, vertex colors and admitted sampler behavior using the
existing native CPU/atlas adapter. Atlas PNGs are private conversion evidence;
they are not GPU resource publication or original cache/file ABI.

The cache fingerprint covers original model/TPL contents and conversion helper
sources. Cached production files and atlas evidence are content-hash checked;
missing or changed products require regeneration. A completed manifest
describes successful products and their hashes. The source asset and compiled
outputs remain outside source history and have no redistribution permission.
This contract adds no independent test or hardware-validation claim.

Generic --area support is a data dependency mechanism. New unsupported model
animation/material domains must fail visibly rather than silently substituting
another pose/model, inventing aliases, or reducing descriptor metadata. Actual
geometry/material conversion and native admission still determine which future
areas can run faithfully.

## Next real owner boundary

Game must compose PackedNpcRegistryServices with its retained field-owned
pose pool, original constants and stable addressable actor memory. The actual
tagged-map registration producer must write the original names/descriptors,
position/yaw, dimensions, AI/territory/ranges, sound/battle metadata and pose
requests in source order. Main readiness, source flags and init/regular event
ownership require their real providers.

Named lookup, collision and rendering must refer to these same registered
actors. A separate preplaced catalog model or initialized presentation bit
does not establish registration. Preserve pose/model/texture lifetime and
reference ownership across NPC deletion, room changes and retained independent
events; unsupported world services must remain pending. Exporting all banks
supplies resources for this work, not completion of the original map parent.

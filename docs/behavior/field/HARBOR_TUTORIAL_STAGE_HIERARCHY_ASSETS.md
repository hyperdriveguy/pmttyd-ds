# Original Harbor tutorial stage hierarchy assets

The original Crump stage descriptor at `95540870` in US `gor.rel` selects
`bti_01` and `stg_00_0`. Its four 24-byte rows at `955407e0` select groups A1,
A2, B and C, layers 0–3, category 2, with positions `(33,0,-48)`, `(0,0,71)`
and zero for B/C. These are resource selection facts, not a stage-ready receipt.

## Editable sources and identity

`assets/original/tutorial-stage/stg_00_0-hierarchy.json` preserves all 144
original nodes and bindings for the 7,995 selected foreground vertices.
`assets/original/tutorial-stage/base/bti_01-hierarchy.json` preserves all 108
nodes and bindings for the **11,829** selected Harbor base vertices. The existing
Trio `bti_01.mapobj` binds 11,721 vertices and must remain a separate resource.
A shared map name does not establish mesh or hierarchy identity.

Each directory contains per-asset source extent, SHA-256, selected REL hash,
original template hash, conversion description and licensing. Nintendo and
Intelligent Systems retain the original rights; importing a source grants no
redistribution license. The existing map decoder's MIT attribution is retained.
No model, texture or atlas is replaced by this import.

`tools/import_tutorial_stage_hierarchy.py` opens the ISO read-only and refuses a
nonempty destination. It runs the existing map conversion with the actual Harbor
selection and requires the resulting vertex bytes to match the supplied original
template exactly. It is a one-time import tool, not a normal-build step.

Normal manifest recipes use `stage-hierarchy` and compile the checked-in JSON
through `tools/stage_hierarchy_assets.py` into distinct generated products:

- `romfs/harbor-tutorial-stg_00_0.mapobj`
- `romfs/harbor-tutorial-bti_01.mapobj`

The codec retains original node order, parent indices, names, source-local
binary32 matrices and vertex bindings. It rejects invalid native-domain counts,
nonfinite components, repeated or missing vertex owners and malformed packs.
There is no geometry reduction or implicit matrix correction. Editable changes
remain authoritative; compilation never reimports from the ISO.

## Native owner and remaining binding

The existing `StageMapObjects` reader owns the hierarchy and actual named
visibility flags. Its root-scale and translation conventions remain unchanged.
The focused native test applies the four original source positions and compares
every resulting vertex with the matching original template. A1/A2/B/C each hide
a nonempty range; their ranges partition all foreground vertices. The base
contains actual `escape_wall` and `spotlight` handles.

Loading this data does **not** produce the source `MapGetWork` chain, establish
the actual current bank, complete `StageObjectInit`, import GPU textures or
publish a scene. The real process owner must bind source global `8041e660`,
current-bank word `8041e664`, the selected work/header/root chain and the exact
matching mesh/hierarchy incarnation. The primitive's source group operations
must execute against that owner. A native name lookup alone is insufficient.

## Validation scope

Focused Python checks cover exact round trips, editable matrix changes, malformed
packs and refusal to overwrite an import. The normal compiler was exercised for
both recipes. Strict host and ASan/UBSan tests use the real `StageMapObjects`
implementation and matching original template bytes. These are CPU resource and
visibility proofs, not native GPU or normal Crump playthrough observations.

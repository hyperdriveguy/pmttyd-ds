# Editable game assets

This tree is source history. Original game sources and custom sources live
alongside code, following the source-edit-build workflow used by
[pret/pokeemerald](https://github.com/pret/pokeemerald). The project is a native
port; it does not target a matching GameCube ROM or ISO.

`manifest.json` binds each source to a RomFS output and records its license.
`original/` includes PNG textures, lossless JSON models/animations, and PCM16 WAV
music/effects for the current slice. `custom/` is for new authored sources.
Original assets retain their original rights; their presence here grants no
redistribution license. Custom entries must state their own author/license.
The ISO, temporary extraction caches, compiler caches, and RomFS products stay
outside source history. See [provenance](../docs/ASSETS.md).

```sh
make assets-check                 # source paths, IDs, licenses, output collisions
make all                         # base pack, editable sources, native binary
make all SOUNDTRACK=gamecube      # select GameCube music at runtime
```

Normal builds never rewrite these sources. `make assets-import` is an explicit
migration operation and refuses a nonempty destination. It imports the selection
in `import.json`, exported model/animation banks and packaged audio into
`original/`; it is not needed again after checkout. To import a new batch, use
`python3 tools/import_editable_assets.py --destination assets/import-next`
after `make assets-base`, review its `recipes.json`/provenance, then merge selected
entries into `manifest.json`. This does not import the entire disc.

## Editing and adding

Edit a source and run `make all`. To replace an existing source, keep its output
name and ID, change `source` and `license` to the new authored source. To add an
asset, add a new entry with a unique ID and output name. Packaging a new name
does not add a new actor or sound trigger: bind that resource in its native
subsystem separately. Story message text/IDs/tokens remain ISO-backed and are
not editable story replacements.

The original import ledger records initial source hashes;
`original/conversion-provenance.json` retains upstream converter metadata,
including original ISO paths, conversion settings and sound/sample identities. Later edits are
expected to change them; build hashes record the current inputs and outputs in
ignored `local-assets/pipeline/build.json`. IDs are stable manifest identities,
not replacements for original game numeric IDs.

| Kind | Source | Native output |
| --- | --- | --- |
| `texture` | PNG, power-of-two dimensions 8..1024 | `.t3x`, `rgba8` or `rgb565` |
| `atlas` | JSON layout and PNG frames | `.t3x` plus `.t3x.atlas.json` UV rectangles |
| `model` | native triangle JSON or triangulated OBJ | `<output>.vertices` / `<output>.batches` |
| `actor` | JSON named frame-zero poses | `.pmmesh` (`PMCHAR1`) |
| `animation` | JSON hierarchy, base arrays and sparse keys | `.pmanim` (`PMANIM3`/`PMANIM4`) |
| `sfx`, `music` | PCM16 WAV, mono/stereo, 8..48kHz | canonical WAV plus optional `.wav.loop` |
| `pack` | subsystem source resource with its existing encoder/layout | byte copy; no automatic validation of its native format |

### Textures and atlases

The imported character PNGs are already atlases. Painting inside their existing
cells preserves native animation UV/palette references. Map PNGs preserve the
converter's dimensions and repeating coordinates. Existing pixels are compiled
without resizing, reflowing, or palette reduction.

New atlas entry:

```json
{"id":"custom/banner","kind":"atlas","source":"assets/custom/banner-atlas.json",
 "output":"custom/banner.t3x","license":"Your author and license"}
```

Layout (`source` paths are relative to the repository root):

```json
{"size":[256,256],"padding":2,"frames":[
  {"name":"idle","source":"assets/custom/banner.png","position":[2,2]}
]}
```

Positions mark the image content's upper-left pixel. Frames and their padding
must fit and must not overlap. The compiler extrudes edge pixels into padding;
UV metadata uses native bottom-left V. It does not invent frame timing or change
an existing animation's palette. A subsystem must consume new UV metadata.

### Models and animations

Imported model JSON uses native engine coordinates and flattened triangle rows
`[x,y,z,u,v,r,g,b,a]`. Color components are floats; native V points upward.
Batches are `[start,count,texture,flags,wrapS,wrapT]`, with texture count in the
header. Preserve vertex order and counts when using existing mesh/UV animation
or collision companions. Editing render geometry does not update collisions,
roads, object supports, visibility bindings, shadows or story placement.

For Blender, export a triangulated OBJ with Y up and explicit UVs, then map
material names in the recipe. OBJ coordinates are already engine units by
default; set a positive `scale` to convert authored units. The compiler retains
face winding and OBJ UVs (do not flip V on export).

```json
{"id":"custom/prop","kind":"model","source":"assets/custom/prop.obj",
 "output":"custom/prop","texture_count":1,"scale":0.1,
 "materials":{"paper":{"texture":0,"color":[1,1,1,1],"flags":0,"wrap_s":0,"wrap_t":0}},
 "license":"Your author and license"}
```

Texture IDs are pack-local indices; provide matching texture outputs and bind
the new model in the renderer. The current OBJ path is static: Blender rigs,
shape keys and skeletal animations are not automatically exported. Native
animation JSON is editable now, with original loop fields, hierarchy, sparse
signed deltas, timing keys, textures, colors and surface state. Its format is
an explicit representation of the existing pack, not a general animation editor.
Keep references/counts consistent and use the existing tick units. Values are
source data, not newly measured original behavior.

Export more existing native sources:

```sh
python3 tools/asset_pipeline.py export-model --source romfs/map --destination assets/custom/map.json
python3 tools/asset_pipeline.py export-actor --source romfs/characters.pmmesh --destination assets/custom/poses.json
python3 tools/asset_pipeline.py export-animation --source romfs/characters.pmanim --destination assets/custom/motion.json
```

### Sound and music

Edit WAVs in an audio editor; export PCM16 at the intended original rate and
channel count. The compiler normalizes RIFF chunk layout to the native 44-byte
header without resampling or changing samples. A recipe's optional
`"loop":[start,end]` uses **sample frames**, including both channels in a stereo
frame; start is inclusive, end exclusive. Omitting it produces a one-shot and
removes any old output loop sidecar. Music and SFX trigger/routing bindings stay
in their existing subsystem/config tables; editing a sample does not implement
MusyX macro playback or Switch adaptive music. Native readers can impose tighter
channel/size/known-profile constraints, including item-get fanfare lengths.

### Rebuilding and removing replacements

`assets-ready` restores the previous base files, runs original converters, then
compiles the manifest. Compilation finishes in staging before outputs install.
Removing an entry restores its original base asset (or removes a newly added
output). Cache keys include all source contents, atlas dependencies, recipe,
compiler code, Python/Pillow versions and tex3ds binary. Corrupt cached outputs
are recompiled. A root-level content stamp forces RomFS repackaging when a
nested asset changes. `make clean` retains these caches.

Do not edit `romfs/` directly. If a different tool changes an installed output,
`prepare` refuses to overwrite that change. Resolve the conflicting output
against `local-assets/pipeline/installed.json` and its baseline first. The build
should have one owner; concurrent builds into one RomFS are unsupported. Use a
separate checkout/output/cache for independent builds.

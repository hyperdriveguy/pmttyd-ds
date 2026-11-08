# Retained original map material and light source

`source_map_material` compiles eleven complete original US G8ME01 revision0
bodies to a fixed native continuation graph: mapSetMaterial80020e54/614,
mapSetLight8002493c/1cc, lightGetAmbient8001ba60/70,
lightNameToPtr8001bcf8/a0, mapGetActiveGroup8002b430/8,
lightGetNearObj8001bad0/1fc, lightSort8001bccc/2c,
qqsort800064fc/184, fsort80006680/60c, camGetCurPtr8000f2e4/18,
and TEXGet80037b5c/10 (addresses/sizes hex). The manifest pins SHA256 for
all1206 instructions. `tools/compile_map_material.py --check` rereads the
read-only ISO, verifies every complete body and reproduces the checked-in graph.
This is static binary evidence, not an original runtime or REA execution trace.

The local source contracts remain authoritative:
[entry/channels](MAP_MATERIAL_ENTRY_CHANNELS_AND_TEV_INPUTS.md),
[selection/names](MAP_MATERIAL_LIGHT_SELECTION_AND_NAMES.md),
[light requests](MAP_MATERIAL_LIGHT_OBJECT_REQUESTS.md),
[textures](MAP_MATERIAL_TEXTURE_LOOKUP_AND_OBJECT_LOADS.md), and
[nearest-light queries](MAP_CHARACTER_LIGHT_QUERIES.md).
The character material helper is not used as a replacement for the map path.

The caller supplies real source memory and private scratch through services.
The cursor pins function, five argument words, stack top and callback identities.
Each memory operation commits only on true; multiword saves/loads retain their
successful prefix. External requests retain r3..r10 and raw binary64 f1..f8 until
acknowledged; explicit integer/floating return values update only r3/f1.
The entire native graph is budgeted in instruction visits, never frame ticks.
Callers freeze the VM/providers/scratch and prevent reentry for the cursor lifetime;
these low-level services do not establish physical mapping identity themselves.

FPRs retain raw64-bit values. LFS promotes the original binary32; LFD preserves
both words across holds. Fsubs/fmuls round their results once to binary32 then
promote; ordinary fmr and stfd preserve binary64. In particular, unsigned texture
LOD conversion retains43300000:byte minus43300000:00000000 before its single
round, including bytes128..255. All six GXInitLightAttn inputs remain present.
Unquantized private paired saves/restores retain two lanes; caller registers are
zero seeded. Arbitrary caller GQR/FPSCR, PPC exception/trapping behavior and
nonfinite arithmetic payload fidelity are not established by this finite native ABI.

Both nearest-light branches execute the source graph. Census>8 obtains actual
distance results through a retained external PSVECDistance request, then runs the
original qqsort/fsort/comparator. Its original scratch803c5360/3100 and globals
8041e584/18 must belong to the same VM and be serialized with every other sort
user, including DVD sorting. Whole memcpy effects remain a retained external child.
The <=8 path does not calculate distance. Final flag filtering matches overlap,
skips ambient, and preserves the original write-before-maximum test and zero tail.

The imported editable material literals contain92 exact initialized DOL bytes;
no light records, GXData/FIFO or runtime Work is imported. See
[asset provenance](../../ASSETS.md). Normal builds compile field-map-material.pmevt.
Import refuses nonempty destinations and never runs implicitly during a build.

This step provides the portable source continuation and literals. It does not yet
bind this cursor into NativeMapProcess, acknowledge any GX service, run TEV's
separate body, initialize GXData, produce capture bytes or submit native geometry.
The existing parent remains held at real GXBeginDisplayList with group count0.
Native integration must retain a separate current stack/provider owner, compose
actual SDK and texture-object children, serialize sort scratch, and preserve the
Begin/End capture effects before the parent reaches mapSetMaterial. Host recording
callbacks are fixture acknowledgments, not GPU or source readiness receipts.

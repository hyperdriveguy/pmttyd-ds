# Original demo golden chest material check

Read-only US revision0 resource inspection and portable native-pack evaluation,
plus the parent's isolated Azahar final-title-guard capture. This is a specific
conversion/binding investigation, not original-runtime image parity.

The colorful assortment on the rug in front of the merchant belongs to original
map subtree S_roten. The separate golden chest is at Peach's hands to its right.
The supplied screenshot does not indicate a chest atlas/crop mismatch.

`tools/inspect_demo_chest_materials.py` independently extracts original
`a/MOBJ_GoldenTreasureBox` and `a/MOBJ_GoldenTreasureBox-` to a temporary
directory, recompiles source S_1/A_1 through the existing surface converter,
imports that PNG with tex3ds, and compares PMANIM4/T3X/uvscale bytes with the
requested native RomFS. No original resource is executed or saved into history.
A small temporary host inspector evaluates the actual pack and the native
`1-(1-v)*8` crop expression. The output report remains under ignored artifacts.

The resource explicitly owns texture bank MOBJ_GoldenTreasureBox. Its three
kind0 descriptors use original TPL indices0/1/2, with source name fields
bj/texture/mobj_gold_thako_06.tga, _05.tga and _02.tga. The fourth atlas cell is
neutral white for original untextured, vertex-colored lid/interior surfaces.
Atlas size is1024×128; stored UV scale8 is correct. Source wrapping and current
animation palette states retain their ordinary surface conversion ownership.

Recompilation agrees byte-for-byte with the parent's tested pack:

- PMANIM4 SHA256 `8e9e922a10a689fa446b529f60f0f3541a2d6ae7efbb728a4b66ec6a537d9b0d`.
- T3X SHA256 `b7eb45718f31f928eba1d5cbf57a2284f09eaa9426d3c673dfbc6b0a7e203454`.
- uvscale SHA256 `03e3c2420f5066a5fa6e36735ed8cc4f6a251046263e1a6024f009deeee3b952`.

Each of ten S_1/A_1 samples at times0/1/15/60/300 emits10,584 wrapped/subdivided
vertices. U spans0.001953125..0.376953125 and cropped V spans
0.015625..0.984375, within the correct atlas cells. The original atlas contains
gold/brown/lock details and neutral white, with zero blue-dominant opaque
pixels under the inspection predicate B>1.2*R+10. Source vertex colors are
grayscale/red; they do not introduce cyan or purple. This pixel predicate is
a diagnostic, not a perceptual proof or an original color-matching standard.

Independent S_roten source conversion yields2,364 vertices/788 triangles and
native bounds X[−1.298828125,−0.948828125],Y[0.000389364,0.230000028],
Z[4.15,4.85]. Its source textures include win_komono_01 goods and
nok_zyutan_01 rug. The portable final scene's distinct chest root is
(−85.9961,19.9609,450),A_1/yaw225. Applying the current native actor matrix
yields chest bounds X[−0.924130,−0.778141],Y[0.199609,0.347774],
Z[4.43583,4.58182]. The two X ranges are disjoint, supporting the visible
identification of map stock versus the golden chest actor.

No production texture/geometry correction was made: replacing the stock or
recoloring the chest would discard original data. GPU command/texture-state
capture and Dolphin image comparison were not performed. Original object
lighting, complete native pose/facing/material ownership, paper/crystal/
radiation consumers and hardware parity remain separate limitations from
[demo presentation](NATIVE_OPENING_DEMO_PRESENTATION.md).

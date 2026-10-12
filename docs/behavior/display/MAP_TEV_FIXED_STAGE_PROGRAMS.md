# Fixed TEV base stage programs for modes1..7 and12

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis,no original instructions/GX requests executed. Evidence tool
`tools/verify_map_tev_fixed.py` checks one containing TEV identity with partial
semantics,903 reviewed words,one dispatch span,two US lecture imports,six direct
DOL sites,eight fixed literal programs and150 decoded requests. A containing
body hash remains an identity check,not full-function semantic completion.

These blocks belong to mapSetMaterialTev80021b80/0x2dbc. The
[base dispatch](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md) selects them only when
signed texture-slot count N>0. No per-mode N>=2/N>=3 validation occurs. Counter
B=texture maps and C=coordinate generators retain incoming N;I=indirect stages
retains0. Each fixed branch configures its fixed number K of stages irrespective
of N,then exits to shared postlude80022f48. It does not load images or regenerate
UV matrices. Callers supply previously configured GX texture/coordinate state.

## Exact six-request sequence per stage

For each row below,s is stage index;perform in THIS order:

1. GXSetTevOrder(s,coord,map,channel)802b8e30.
2. GXSetTevColorOp(s,0,0,0,1,out)802b89e8.
3. GXSetTevAlphaOp(s,0,0,0,1,out)802b8a50.
4. GXSetTevColorIn(s,colorA,colorB,colorC,colorD)802b8960.
5. GXSetTevAlphaIn(s,alphaA,alphaB,alphaC,alphaD)802b89a4.
6. GXSetTevSwapMode(s,0,0)802b8c98.

Order triples,RGBA input enums and output IDs below are original literal integer
arguments. Out is shared between color/alpha requests for that row. Clamp
argument is1 in every row;op,bias,scale arguments are0. No K-color selector or
swap-table definition is written by these fixed base blocks. Their SDK packing,
GPU arithmetic/quantization/clamping and retained state remain backend work.
Do not replace source numeric enums with guessed portable material names.

| Mode | Stage | Order (coord,map,channel) | Out | Color inputs A,B,C,D | Alpha inputs A,B,C,D |
| --- | --- | --- | --- | --- | --- |
| 12 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,7 |
| 12 | 1 | 1,1,255 | 0 | 15,15,15,0 | 7,7,7,4 |
| 12 | 2 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |
| 1 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,4 |
| 1 | 1 | 1,1,255 | 0 | 0,8,9,15 | 7,7,7,0 |
| 1 | 2 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |
| 2 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,4 |
| 2 | 1 | 1,1,255 | 0 | 15,0,9,15 | 7,0,4,7 |
| 2 | 2 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |
| 3 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,4 |
| 3 | 1 | 1,1,255 | 0 | 0,15,9,15 | 0,7,4,7 |
| 3 | 2 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |
| 4 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,4 |
| 4 | 1 | 1,1,255 | 0 | 15,0,8,15 | 7,0,4,7 |
| 4 | 2 | 0,0,255 | 0 | 8,0,1,15 | 7,7,7,4 |
| 5 | 0 | 0,0,255 | 1 | 15,15,15,8 | 7,7,7,4 |
| 5 | 1 | 1,1,255 | 2 | 15,15,15,8 | 7,7,7,4 |
| 5 | 2 | 2,2,255 | 0 | 2,4,9,15 | 1,2,4,7 |
| 5 | 3 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |
| 6 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,4 |
| 6 | 1 | 1,1,255 | 0 | 0,8,1,15 | 7,4,0,7 |
| 6 | 2 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |
| 7 | 0 | 0,0,255 | 0 | 15,15,15,8 | 7,7,7,4 |
| 7 | 1 | 1,1,255 | 0 | 0,8,9,15 | 0,7,4,4 |
| 7 | 2 | 255,255,4 | 0 | 15,0,10,15 | 7,0,5,7 |

## Branch boundaries and publication

| Mode | Complete reviewed fixed block | K at shared postlude |
| --- | --- | --- |
| 1 | 80021c10..80021dbc exclusive | 3 |
| 2 | 80021dbc..80021f68 exclusive | 3 |
| 12 | 80021f68..80022114 exclusive | 3 |
| 3 | 80022114..800222c0 exclusive | 3 |
| 4 | 800222c0..8002246c exclusive | 3 |
| 5 | 8002246c..800226a4 exclusive | 4 |
| 6 | 800226a4..80022850 exclusive | 3 |
| 7 | 80022850..800229fc exclusive | 3 |

Modes1/2/3/6/7 share first and third rows but differ materially at stage1.
Mode7's alpha inputs (0,7,4,4) preserve a different final argument from mode1;
mode12 has its own texture alpha selection. Mode4's last row requests texture0
again instead of the common untextured channel4 row. Mode5 writes separate
output IDs1/2 in its first two stages and explicitly addresses coordinate/map2
in stage2;it uses FOUR stages. Do not deduplicate these programs by texture count
or by their mostly shared operation fields.

After the branch's K store,all continue into
[constant/character-shadow stages](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md),
remaining middle branches,and [final callback/count publication](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md).
K can increase or be replaced by a callback;3/4 are base-stage counts,not final
renderer counts. Mode8/9 time-driven blocks remain OPEN. This delivery expands
fixed base coverage;it does not close all TEV/GX/material/resource lifetimes.
See [mode12 caller/source and implementation boundaries](MAP_TEV_MODE12_CALLERS_AND_LITERAL_EVIDENCE.md).

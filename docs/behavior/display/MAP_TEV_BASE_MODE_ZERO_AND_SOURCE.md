# Ordinary TEV mode0, source witness and base selection

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original execution or image parity. Evidence tool
`tools/verify_map_tev_ordinary.py` checks one containing TEV body identity,four
complete shadow getter bodies,303 reviewed words,three original spans,two US
lecture imports,six direct DOL sites and original Mario draw-mode metadata.
The containing body hash does NOT claim complete semantics for every mode.

mapSetMaterialTev80021b80,size0x2dbc takes r3 signed texture-slot count N,
r4 mode M,r5 flags F,r6 model matrix pointer. Capture M,F,model,N;initialize
texture-map counter B=N,coordinate-generator counter C=N,indirect-stage count
I=0. It also forms local table base802bf958 by lis802c plus signed addi−1704.
This is not802cf958. TEV stage count K is supplied by the selected base branch.
No local N/GX stage-capacity/model-null guard is present.

If signed N<=0,skip mode dispatch and execute no-texture base80022eb8:
GXSetTevOrder(0,255,255,4);GXSetTevColorOp(0,0,0,0,1,0);
GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,10);
GXSetTevAlphaIn(0,7,7,7,5);GXSetTevSwapMode(0,0,0);set K=1.
B/C retain incoming N,including negative values,until later stages/count
publication. Do not substitute a successful zero-count return.

If N>0,compare M UNSIGNED against12. Greater enters generic base80022e24;
otherwise use pinned13-word dispatch table803049a0:

| M | Target |
| --- | --- |
| 0 | 80021bd8 |
| 1 | 80021c10 |
| 2 | 80021dbc |
| 3 | 80022114 |
| 4 | 800222c0 |
| 5 | 8002246c |
| 6 | 800226a4 |
| 7 | 80022850 |
| 8 | 800229fc |
| 9 | 80022b00 |
| 10,11 | 80022e24 |
| 12 | 80021f68 |

Mode0 is the following EXACT ordered base request list:

1. GXSetTevOrder(0,0,0,4)802b8e30.
2. GXSetTevOp(0,0)802b88d4.
3. GXSetTevSwapMode(0,0,0)802b8c98.
4. Set K=1;branch to shared postlude80022f48.

This does not loop over N. Larger N still configures only this base stage;
initial B/C follow the passed N. GXSetTevOp's internal color/alpha configuration
and GPU fixed-point equation are still called backend work;this note preserves
the original request instead of assuming a host multiply fully implements it.

Generic base80022e24 requests Order(0,0,0,4),ColorOp/AlphaOp(0,0,0,0,1,0),
ColorIn(0,15,10,8,15),AlphaIn(0,7,5,4,7),SwapMode(0,0,0);K=1. It covers modes10,
11 and unsigned values>12. Other table-target bodies remain OPEN in this recovery.

## Original used source

Read-only a/a_mario,215124 bytes,SHA256
61b585d6b2f1a36551d3b779551a12122f228bf1354ebc5bb44651c58f118293,
has24 draw records;ALL word[draw+8] modes are0. Previously checked slot counts
are all1. Existing [render defaults](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md)
pass draw mode+8 and slot count;this binds the shipped ordinary Mario mode0
path to a concrete source. Actual pose flags,scene admission/lifetime and frame
images are separate;the source witness alone does not prove which shared
branches ran. The extra-stage renderer explicitly requests mode12,which remains
open here. The six direct sites are mapSetMaterial80021448,renderProc8003cb34/8003cf34,
materialProc8003d80c/8003dba0,imgDisp_ProjPlane80072030. This census covers seven
DOL text sections and establishes static references,not execution.

Follow [constant/character-shadow stages](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md)
and [final publication/gaps](MAP_TEV_FINAL_CALLBACK_AND_OPEN_BRANCHES.md).
Full-game requirements need remaining modes,source predicates and GX behavior;
mode0 closure is useful implementation progress,not the complete shader system.

ID1 fixed-program dependency update (2026-10-12):the earlier open base-mode
list is superseded locally by [stage tables](MAP_TEV_FIXED_STAGE_PROGRAMS.md)
and [mode12 bindings](MAP_TEV_MODE12_CALLERS_AND_LITERAL_EVIDENCE.md),which close
modes1..7/12 as literal request blocks. Time-driven modes8/9 and all six middle
branch internals,GX/source/lifetime/images remain open. No full-function/system
closure or actually executed mode1..7 scene is claimed.

ID1 time-driven base update (2026-10-12):earlier open8/9 local-block status is
superseded by [pulse](MAP_TEV_MODE8_PULSE_AND_RAW_CLOCK.md),
[scroll](MAP_TEV_MODE9_SHARED_SCROLL_AND_MATRICES.md) and
[base closure](MAP_TEV_BASE_MODE_CLOSURE_AND_REMAINING_WORK.md). All local base
request paths now have contracts;actual clock/phase/color/source/cadence,
six middle branch internals,GX/libm/lifetime/images remain open. This does not
prove complete original shader/system behavior or actually executed US scenes.

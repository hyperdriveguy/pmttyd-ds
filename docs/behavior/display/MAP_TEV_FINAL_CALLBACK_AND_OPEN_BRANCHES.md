# TEV final callback, count publication and remaining shader branches

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary/source analysis,no original execution or image parity. Evidence tool
`tools/verify_map_tev_ordinary.py` checks one containing TEV body identity,four
complete shadow getter bodies,303 reviewed words,three original spans,two US
lecture imports,six direct DOL sites and original Mario draw-mode metadata.
The containing body hash does NOT claim complete semantics for every mode.

Final block800248a8..8002493c follows earlier base and optional stages.
K=TEV stage count,B=texture maps,C=coordinate generators,I=indirect stages;
K/C/I are republished after any selected callback. No local upper capacity guard.

If F&0x0f000000==0,skip callback. Otherwise resolve current map group
G=word[8041e660]+0x2f4*word[8041e664]. Build a seven-word local record:

| Offset | Initial value |
| --- | --- |
| +0 | Word[8041e66c],current render context |
| +4 | Captured model matrix pointer |
| +8 | Captured mode M |
| +0c | Running TEV stage count K |
| +10 | Running texture-map count B |
| +14 | Running coordinate-generator count C |
| +18 | ZERO indirect-stage publication field |

The callback pointer is word[G+110];if nonNULL,call with r3=&record,ignore its
return. All flagged nibble values select this same group callback slot locally.
An intermediate rotate/mask of the flag field is overwritten before callback
lookup;it does not establish a nibble-indexed callback table. Other integer
register values at bctrl are setup temporaries,not documented alternate APIs.

Whether callback pointer nonNULL or NULL,reload K=record+0c,C=record+14,
I=record+18. **Do not reload B,model,mode or context.** Consequently the flagged
branch resets published indirect-stage count to0 even if no callback exists,
unless the callback overwrites that field. No callback signature/source/body or
valid modified-count policy is closed by this request record alone.

Finally,ALWAYS request GXSetNumTexGens(low8(C))802b5084,
GXSetNumTevStages(low8(K))802b8fcc,GXSetNumIndStages(low8(I))802b87ac,
in that order;return. GX semantics for invalid/truncated values and resource
lifetimes remain backend work. No GX state snapshot/restore occurs here.

## Explicit unrecovered middle branches

These flags gate blocks after the documented character-shadow stage. Their
entry tests/skip destinations are pinned,not their internal semantics:

| Admission mask | Block entry | Skip destination |
| --- | --- | --- |
| 0x400 | 80023128 | 80023314 |
| 0x10000 | 80023314 | 80023d58 |
| 0x20000 | 80023d58 | 80023e9c |
| 0x40000 | 80023e9c | 800244ac |
| 0x80100000 (any) | 800244ac | 80024654 |
| 0x800000 | 80024654 | 800248a8 |

Thus F&0x80970400==0 bypasses ALL these unrecovered middle blocks. For N>0,M0
under that explicit condition,the local path is completely specified by the
[base](MAP_TEV_BASE_MODE_ZERO_AND_SOURCE.md),
[constant/character shadow](MAP_TEV_CONSTANT_AND_CHARACTER_SHADOW.md) and this
final block. F4 still optionally admits the documented character-shadow branch;
F0x10000000 can suppress the constant stage;F0x0f000000 can admit callback.
This path-domain condition is derived from decoded masks,not proof that every
Mario pose/scene has those flags clear. Actual flag writers and projected/depth/
indirect modes must be recovered before complete-game material claims.

Next shader work:all remaining base modes1..9/12 and all six middle branches,
original callback producers/bodies,source/GX capacity/cache/lifetime predicates,
SDK GXSetTevOp/GPU arithmetic and original frame comparisons. Original asset
reuse remains necessary but cannot replace these procedural material services.
The complete-game requirements deliverable remains deferred until broader
story/gameplay/rendering and all other system closure gates are satisfied.

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

ID1 projected-middle status update (2026-10-12):the earlier six-open-middle
list is superseded locally by [0x400 projection](MAP_TEV_FLAG400_TWO_STAGE_PROJECTION.md)
and [0x20000 projected shadow](MAP_TEV_FLAG20000_PROJECTED_SHADOW.md). These
blocks/four pswp query leaves now close as local request semantics. Four other
middle internals0x10000/0x40000/0x80100000/0x800000,source/GX/capture/lifetime/
original images remain open;no full TEV/system or actually executed scene claim.

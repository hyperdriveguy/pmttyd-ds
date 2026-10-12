# Renderer mode12 binding and static literal-program evidence

US G8ME01 disc0 revision0;documentation ID1,2026-10-12. Static read-only
binary analysis,no original instructions/GX requests executed. Evidence tool
`tools/verify_map_tev_fixed.py` checks one containing TEV identity with partial
semantics,903 reviewed words,one dispatch span,two US lecture imports,six direct
DOL sites,eight fixed literal programs and150 decoded requests. A containing
body hash remains an identity check,not full-function semantic completion.

The [fixed-stage table](MAP_TEV_FIXED_STAGE_PROGRAMS.md) makes mode12 a concrete
implementation dependency of the US renderer. Original a/a_mario's authored
24 draw modes are all0;that source witness does not invalidate a procedural
renderer request for12 after a texture override. Keep authored and generated
material modes separate.

## Exact actual caller inputs

Two direct call sites set N=2 and M=12 explicitly:

| Caller / site | Reviewed argument preparation |
| --- | --- |
| renderProc /8003cf34 | 8003cf20 li r3,2;8003cf28 li r4,12 |
| materialProc /8003dba0 | 8003db8c li r3,2;8003db94 li r4,12 |

Both load model pointer r6 from word[8041e8a0],reload current animation work,
then current pose pointer work+18 and flags pose+e8 into r5 before the call.
The evidence pins all six argument/call words at each site. The work-pointer
load precedes this six-word span and is already reviewed by the complete
[renderer/material contract](ANIMATION_SHAPE_RENDER_ENTRY_AND_DEFAULTS.md).
Neither substitutes original authored draw mode0 or pose default flags.

These calls occur in the documented
[extra-stage graph](ANIMATION_RENDER_LAST_STAGE_AND_REBIND.md):after a matched
kind1/2 override,draw word+8==0 and work+38 texture nonNULL admit the extra setup.
The caller patches the extra texture object with smartTexObj,configures base/
extra texture and matrix/coordinate state,then requests mode12. Pointer/matrix/
pass guards and their possible unguarded loads are specified in that contract;
do not infer admission merely from the fact this mode has a DOL call site.

Mode12 stages0/1 request the two previously configured texture coordinates/maps,
then stage2 requests channel4 with untextured order. Its base K=3,N=2 means B/C=2
at shared entry;the constant stage can increment K without incrementing B/C,
and character-shadow can increment all three. Later flags/callback publication
remain applicable. Fixed mode12 does not itself call smartTexObj or load a texture.
Its final pixels require retained prior GX state and full SDK/GPU composition.

## Verifier method and limits

The checker rereads the original ISO and statically inspects each fixed program.
Allowed grammar is add-immediate with base register0 into literal argument
registers3..8 or final stage-count register25,direct linked branches to the six
GX requests,then a final nonlinked branch to80022f48. It reconstructs only
explicit immediates and branch targets;there is no original instruction executor,
CPU emulator or invoked GX routine. After each call,argument literals are
cleared,so no caller-clobbered value is silently assumed retained. It rejects
unknown instructions/registers/call targets,nonliteral arguments,misplaced count
publication or wrong/missing final exit. Decoded request arguments/order/K must
match the reviewed manifest.

The manifest's891 fixed-program words plus12 actual call-preparation words total
903. Eight programs contain25 complete stage rows and150 request sites. Original
13-entry dispatch table is also checked. Six whole-DOL direct-reference sites
are carried from prior TEV census;they are not six executed mode12 scenes.
Changing an expected input/out value must fail independently of body hashing.
Byte identity checks remain evidence supporting manual contract review,not a
proof of original hardware/GPU arithmetic or complete source reachability.

## Native and whole-game remaining requirements

A faithful backend must support distinct per-stage color/alpha inputs,output
registers,texture/coordinate/channel selection,stage ordering and retained swaps/
constants. It must preserve source/program selection and K/B/C/I/callback logic.
A single multiplied texture with global alpha does not implement the documented
mode12 and all other fixed programs. Any platform texture/lighting adaptation
needs recorded source/lifetime/performance and original image comparison.
Original assets remain reused;this delivery changes no converter/native policy.

Still open:time-driven modes8/9,six middle projected/depth/indirect branches,
SDK GX request packing/GPU equations,model/pose/group/override/texture source
invariants and lifetimes,actual used mode1..7 scene bindings,and original frame
comparisons. Those modes are executable dispatch branches,not claimed witnessed
by the Mario mode0 resource. Full-system documentation and complete-game final
requirements remain incomplete pending these and broader gameplay/story closure.

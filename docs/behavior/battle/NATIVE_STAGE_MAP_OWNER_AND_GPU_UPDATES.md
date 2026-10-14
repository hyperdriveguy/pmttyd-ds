# Native stage-map owner, original hierarchy and GPU updates

The native trio now supplies the [stage-object Main](NATIVE_STAGE_OBJECT_UPDATE_AND_BINDINGS.md)
with actual original map names, hierarchy, per-node flags and matrices. A new
PMMAP001 private pack preserves every authored local node matrix and rendered
vertex's owning node/local coordinates. It includes124 stg nodes and108 bti
nodes, including hidden owners with no draw vertices. Source selected placement
bake is removed from local bindings during export; original additional
translations reapply it exactly once through the native map owner.

## Source record and map state

The sidecar trio-stage-objects.bin retains actual relocated source stage and
seven descriptor addresses: five tik definitions plus original DOL escape-wall
and spotlight records. Original readonly script/data pack supplies their names,
layers, positions and twelve cache999 words. Native rank0 initialization builds
seven actual records, with escape-wall Y from the retained StageInit W+b28=200;
allremaining records retain the fresh source bank-clear state. Signed rank uses
the pouch halfword88, separately from level halfword8a.

Typed map roots come from original hierarchy definitions. The native map owner
executes root clear2, optional A1/A2/B/C hide1 then clear2, escape-wall hide,
perselected reveal, two builtin hides, source null script gates and ironframe
if_body/if_wire hides. Group mutations touch the named node and descendants;
they exclude the target's siblings. A later selected-child reveal overrides
previously propagated parent hide without inheriting immutable suppression.

These typed maps are a native replacement for address-based MapWork/index and
map-object allocation. They do not claim the full original loader, field map
contexts, collision/draw record ABI or global8041e660/664 alias. Constructor
flags are a declared native projection before the actual used source group
requests, not a complete original map constructor implementation.

## Additional matrices and explicit map-frame owner

Translation resets Additional only when flag200 is clear, divides source XYZ
by10 and concatenates on the right. Rotation uses Rz,Ry,Rx in source order.
An explicit native simulation map-frame recalculation composes parent world,
authored local and retained Additional, clears only200 and retains200000.
Thus a new changed series resets once; its pivot/axis requests accumulate;
unchanged state retains its existing transform. Group flags and matrix latches
have separate ownership. Native model*.1 coordinate bridge reconstructs actual
selected3393 private vertices against the previous original converted pack.

This is a portable float/FMA/libm numerical adapter, not a claim of original
paired-single SDK numerics or outer-loop cadence. Animation-local flag2000,
full map animation/material/light lifecycle and dynamic upstream producers
remain open. Simulation owns recalculation; the renderer does not clear latches.

## 3DS consumption and late engine state

The renderer obtains the same live map owner from TrioAttack. When its revision
changes, it updates original bound positions in the Room linear vertexbuffer
and calls GSPGPU_FlushDataCache once. Unchanged revisions retain GPU data.
Named hidden owners suppress complete vertex ranges at C3D_DrawArrays through
the original material/pass batches; geometry is not teleported or alpha-faked.
Visible ranges are cached until actual mask1 changes; each batch intersects
those ranges instead of rescanning vertex owners. Original texture/UV/
material/color data remain the existing converted values.

Ordinary native attack ticks update the source stage-object bank; native end's
ordered Main pass now completes StageObjectMain and the actually inactive
16-record icon bank. It reaches StageMain's first mandatory batSpotMain8013800c
provider. Real spotlight allocation/work/display, remaining stage/audience/
slot/draw services and BattleEnd/field sequence restoration remain required.
Wrapper80000000 is still clear, field result0/control hold/story11 remain.
See [spotlight owner](STAGE_SPOTLIGHT_WORK_INIT_AND_MAIN_ADMISSION.md).

## Validation and limits

Full source-map tests exercise right-side composition, axis order, first/reset
and cumulative series, retained200000/cleared200, revision behavior, descendant
flags/child reveal/sibling exclusion and exact original3393-vertex reconstruction.
Native trio tests verify seven records, hidden/visible original names and the
ordered eight-request prefix ending at spotlight with no premature end flag.
Asset admission still short-circuits in its original order; real failures now
report the rejecting loader on the development console/native debug output,
rather than silently closing the application. This is not story dialogue.
Full make test/make all and isolated renderer capture evidence are recorded in
../../PROGRESS.md. This does not establish full prologue or hardware parity.

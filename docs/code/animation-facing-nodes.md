# Paper facing and source node matrices

[Facing](../../source/display/animation_facing.cpp) preserves separate Main and
Inner branches; [its binding](../../source/display/animation_facing_binding.cpp)
provides actual field-camera publications. [Node builders](../../source/display/animation_source_node.cpp)
retain the source Joint/Transform operation graph, while [their binding](../../source/display/animation_node_binding.cpp)
adapts reached numeric and SDK requests. These children do not independently
own a producer pass or complete the surrounding renderer. See
[parent ownership](animation-parent-ownership.md) and
[group traversal](animation-group-traversal.md).

Addresses and field offsets below are hexadecimal; sizes and counts are decimal.

## Facing reads live literals and two camera samples

Pose fields70/74/78/7c hold incoming-minus-90, retained angle, sign and target.
Camera4 comes from camGetPtr; targetX18, eyeXc, targetZ20 and eyeZ14 are read in
that order. atan2 consumes the negative X/Z differences and returns double,
which narrows before degree-factor reads. The source factor is exactly
57.29579162597656, rather than a replacement computed from pi.

Heading normalization adds360 while below-360, subtracts360 while at least360,
then adds360 once if negative. Difference/rear-target normalization omits that
last step. Backedges yield without a finite clamp or total iteration limit.
NaN follows ordered comparisons; nonprogressing infinity can remain pending.
Replacing these loops with fmod or a shortest-arc helper changes the graph.

Main flag8 classifies the sign using fresh retained74. Rear means difference
in (90,270] or [-270,-90); the boundaries±90 are front and±270 rear. It writes
-1 or+1 to78. Main's completed evaluation and earlier null-timeline exit remain
requirements of its caller.

Inner flag8 first stores incoming-90 to70 before camera work, then reloads it.
Both front and rear branches request a second fresh camera heading. Rear adds180
and wraps; front uses that new heading directly. Target7c publishes before the
retained angle unwrap loops. Those loops read fresh target/old on each condition
and reread old before each store. Their final captured target, old and difference
survive the fresh flag100 check. Set snaps; clear applies one FMA(.1,difference,
old) per admitted Inner invocation, without an elapsed-time multiplier.

Fresh74 supplies Y rotation, followed by exact sign78==-1 Z reflection, then
uniform input scale. Flag8 clear instead rotates the incoming angle and skips
camera/pose-angle/reflection work. Both paths read live ONE even for scale1.
Output matrix publishes only on completion, though pose/SDK writes can precede
a hold. The public helpers capture their operands/providers at entry and reject
mixing Main/Inner mode; they do not compare changed same-mode arguments on retry.
The caller must preserve the same invocation. See the
[retained facing contract](../behavior/display/RETAINED_ANIMATION_PAPER_FACING.md).

The binding reserves 13×0x280 semantic token bytes, proving the entire range
unmapped at every boundary. Only camera4 offsets c/14/18/20 have read meaning.
Unsupported tokens never fall through to VM storage, and camera tokens reject
stores. Camera4 must have a published perspective field view and not mode2.
The binding retains pool/camera ownership but borrows VM/aliases; a pure current
predicate protects the producer lifetime. It exposes native atan2 and explicit
SDK numeric adaptation, not measured original libm/FPSCR parity.

## Node captures are sparse and ordered

Both builders read translation and live ZERO/TWO, double primary angles through
numeric providers, then finish translation and scale captures before SDK work.
Joint captures 12 fields through2c, including secondary rotation. Transform
captures 21 fields through5c, skipping24/28/2c. A 24-word node memcpy or sorted
read loop would change these boundaries.

| Node offsets | Meaning |
| --- | --- |
| 00/04/08 | Translation XYZ |
| 0c/10/14 | Own scale XYZ |
| 18/1c/20 | Primary angles, multiplied by2 |
| 24/28/2c | Joint-only secondary angles |
| 30/34/38 | Transform rotation pivot |
| 3c/40/44 | Transform scale pivot |
| 48/4c/50 | Transform rotation offset |
| 54/58/5c | Transform scale offset |

Joint parent compensation and own-scale gates use x!=ONE OR y!=ONE OR z==ONE.
Allones therefore admits, while a Z-only change can skip. Admitted parent input
with any zero uses ONE for all three compensation axes; otherwise it divides
fresh ONE by each parent component. Transform uses an ordinary any!=ONE scale
gate. These branches must not be normalized to conventional identity tests.

A zero Transform pivot suppresses its whole offset block, including a nonzero
offset. Nonzero pivots retain explicit negative/positive/offset Trans calls and
Concat grouping, including first/second input-output aliases. Negation flips
source sign bits. Rotation Identity is unconditional. Joint rotates secondary
Z/Y/X then doubled primary Z/Y/X; Transform rotates only doubled primary Z/Y/X.
Each angle reads fresh ZERO and, only when admitted, the degree factor and SDK
axis request. ASCII axes122/121/120 mean z/y/x.

Ten durable 48-byte local matrices preserve SDK scratch identities. The parent
increments/stores modeling level and publishes stackBase+48×level BEFORE reading
the source dispatch table. Joint's16 and Transform's8 compiled targets are checked
at a native implementation frontier. An unknown captured target holds; changing
the old table cannot undo the captured branch. A missing uncaptured word can be
supplied before that read. Local dispatch grouping is explicit. The final Concat
reloads level after SDK calls, using previous guest world and selected local to
write current guest world, without republishing the pointer or popping.

The binding lends local matrices to one retained SDK owner. Pair reads require
an in-range slot and a four-byte-aligned start at most40 in the 48-byte matrix.
Multiply/Divide admit finite reached operands/results and narrow to binary32;
unused NaNs are not rejected preemptively. The VM/current predicate and stable
binding must survive both node and SDK holds. Complete CPU node work does not
establish GPU, queue, source numeric or hardware fidelity. See
[actual-VM nodes](../behavior/display/NATIVE_ACTUAL_VM_NODE_BUILDERS.md),
[Joint](../behavior/display/ANIMATION_JOINT_NODE_MATRICES.md) and
[Transform](../behavior/display/ANIMATION_TRANSFORM_NODE_MATRICES.md).

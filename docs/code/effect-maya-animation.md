# Maya effect animation and transforms

[Maya animation](../../source/effect/effect_maya_animation.cpp),
[its API](../../include/effect/effect_maya_animation.hpp),
[matrix evaluator](../../source/effect/effect_maya_matrix.cpp) and
[matrix API](../../include/effect/effect_maya_matrix.hpp) retain original operation
order around unavailable native owners. These waits are native adaptations of
synchronous source functions, not new original event scheduling.

Animation cursor operation0 is idle,1 allocate,2 play,3 calculate,4 delete.
A held operation rejects changed arguments/providers. Completion resets it for a
new invocation. Raw memory callbacks have no failure return; callers supply complete
mapped domains. Explicit allocation, clear, division, conversion, matrix and release
services can hold. Acknowledged children do not replay across later holds.

Allocate uses heap3 and wrapped channelCount*0x34+0x18 bytes, clears once, then
publishes descriptor. No original allocation-null guard is invented. Play captures
GP/context-selected source time and bus divisor, divides to milliseconds, publishes
start/frame, and ORs active1. It preserves ended2, so Play alone is not a restart
of an ended animation. Delete requests heap3 release; surrounding effect lifetime
belongs to its separate pool/controller.

Calculate admits active1 with ended2 clear. Context selects GP0x38/0x3c or
0x40/0x44 timestamp. Bus-quarter/high-product arithmetic forms the original ms
divisor. Elapsed subtraction is unsigned64 wrapping, and conversion rounds through
double then float. GP+4 rate uses the original signed-bias conversion and explicit
SP multiplication/division. Frame>=duration clamps and sets ended2, but the current
call still evaluates channels; future ended calls skip. This is source time,
independent of display-frame count.

Every channel iteration reloads descriptor, table and signed count after the prior
child. Channel records stride0x10; matrix/scalar outputs stride0x34. A held child
retains its argument addresses; a successful child can change bytes observed by
later iterations. Earlier channels never restart to satisfy a later child.

A matrix channel with signed key count<=1 writes neither output. Otherwise keys
stride0xb2: signed16 time plus22 eight-byte curve records. Scan finds the first next
time ordered>=fresh frame; otherwise evaluate the final pair at its next time.
Before-first frame is not clamped upward. NaN follows the final-pair path.

Signed16 values/tangents decode by1/64, reflecting original GQR5 type7/scale6.
All six source reads, including unused tangents, precede the hold byte. Nonzero
hold uses previous value. Otherwise Hermite evaluation retains signed time
subtraction, zero-duration epsilon, each SP arithmetic boundary, and the negated
fused subtraction. Algebraic polynomial equality does not establish equal rounding.

| Evaluated lanes | Meaning |
| --- | --- |
| 0..2 | Translation T |
| 3..5 | X/Y/Z rotations |
| 6..8 | Scale S |
| 9..11 / 12..14 | Pivot PA / translation TA |
| 15..17 / 18..20 | Pivot PB / translation TB |
| 21 | Scalar output |

SDK stages build all translations, rotations, scale and negative pivots before
ten source-order concatenations. In column-vector notation, the products are
B=((TB*PB)*S)*-PB, A=(((((TA*PA)*Rz)*Ry)*Rx)*-PA), output=(T*A)*B.
Negative-pivot symbols here mean translation by the negated pivot vector.
Scratch destinations preserve source alias order; regrouping changes rounding.
Each acknowledged SDK output remains captured, while false output is ignored.
The final twelve matrix words publish before ordered scalar clamping to0..1.
NaN and signed zero survive those comparisons. Then the cursor resets.

[The opt-in math adapter](../../source/effect/effect_maya_math_adapter.cpp) and
[API](../../include/effect/effect_maya_math_adapter.hpp) reuse established affine
services through copied payloads. They install numeric/SDK callbacks while leaving
memory/user ownership with the caller. Finite-domain refusal stays unavailable;
it is not identity output. Native libm/FMA behavior does not establish original
Gekko FPSCR, subnormal, exception or raster parity.

See [lifecycle evidence](../behavior/effect/RECOVERY_EFFECT_LIFECYCLE_AND_MAYA_OWNERSHIP.md)
and [quantized curves/transform evidence](../behavior/effect/MAYA_QUANTIZED_CURVES_AND_EFFECT_POOL_CONTINUATIONS.md).
Real effect-pool, resource, renderer and scheduling owners remain separate.

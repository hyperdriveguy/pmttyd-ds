# Retained original Mario forced movement consumer

`SourceMarioForceMoveMain` implements the complete original US
`marioForceMoveMain` (80095f64/94). `SourceMarioMovePos` implements the complete
coordinate leaf `movePos` (80005de4/a8). They are portable source continuations;
neither acknowledges the containing MarioMain or MarioMoveMain.

## Original state and ordering

ForceMoveMain captures the Mario pointer through 8041e900, reads main flags and
returns if bit 0x20 is clear. Otherwise it decrements the signed halfword at +4e,
stores the truncated halfword, and tests the signed truncated result. Thus
8000 decrements to 7fff and continues; zero decrements to ffff and expires.
Expiry reads source zero 80420a60, clears +4e, reads source one 80420a74,
reloads flags, clears only force bit 0x20, stores zero to speed +180 and reads
retained input +194. Input greater than one calls GetDashSpd80087ce4; input
greater than zero calls GetWalkSpd80087d84. The returned speed is stored on the
original captured Mario even if the child observes a different global pointer.
These speed children remain genuine typed providers; missing input is unknown,
not an inferred neutral stick.

movePos reads actual constants 8041f458 (binary32 two-pi) and 8041f45c (360),
performs single multiplication then single division, calls original sin80272410,
rounds its double result once, calls cos80271ea8, reads old X, then rounds cosine.
The single fused multiply-add writes X before old Z is read. The negative fused
multiply-subtract writes Z. Aliased X/Z destinations consequently observe the X
store. Numeric requests preserve those distinct source boundaries; no pi/180
replacement or split multiply/add is admitted. Portable tests use an explicitly
IEEE native numeric fixture, not a claim of bit parity for every Gekko FP mode.

## Native simulation policy and integration

The ordinary original forced-step branch 800595dc..800595f8 loads travel angle
+1a4, stores facing +1a0, loads speed +180, reloads travel angle and calls movePos
on X +8c and Z +94. A native caller may admit this branch only with real actual
field/form/motion/input/geometry ownership. Special motion/forms, unowned
support/collision and unknown speed children hold. Do not acknowledge the whole
5d0-byte MarioMoveMain using this branch.

In MarioMain the movement/support/ground/boots calls precede ForceMoveMain
(the reached call is 8005b0dc), and subsequent helpers follow it. The native
policy must execute actual ordinary movement, actual Game support/collision,
then this count consumer once per admitted simulation visit. Rendering retries
and repeated completed resumes do not decrement again. Reset a completed owner
only on the next actual visit. Never clear bit 0x20 from elapsed milliseconds.

A held expiry can have already cleared bit 0x20 before its reached speed child
returns. Freeze the containing simulation visit and do not poll the event again
until the consumer completes; no rollback or restarting a partial visit. Once
complete, the existing whole mov_pos2 callback performs its own fresh poll and
original final X/Z snap. This module does not synthesize that poll's result.

## Service and lifetime contract

Callbacks, user pointer, strong lifetime pointer and shared control-block
identity are fixed across holds. `current` is checked before each source step;
lifetime loss poisons the continuation. Recursive resume/reset rejects without
effects. Missing callbacks/data hold at the reached point, preserving operands
and successful reads/stores/children. A false store/child response must have no
completed effects; a partially working child must own its own continuation.
Services are stationary during each synchronous callback under the native
single-threaded admission policy. No raw Game selector is stored here.

## Validation and limits

Strict host tests compare complete traces with independent immediate reference
bodies and hold each external boundary twice. Cases cover signed wrap, NaN
comparisons, child pointer mutation, fresh flags, late coordinate changes,
aliasing, identity replacement, reentry and source-current loss. A two-visit
TEST native policy fixture proves actual source counter/bit clearing followed
by the whole original wrapper's final snap. It is not a field playthrough.
ASan/UBSan and strict ARM syntax use the same standalone source closure without
an older core archive. The read-only verifier pins both whole bodies, actual
constants, GetPtr and the original caller/ordinary-branch slices.

Root owns actual Game field admission, typed source state, real numeric/trig
and speed services and collision scheduling. This package changes no production
Game routing or normal activation. See [whole mov_pos2](RETAINED_MARIO_MOVE_POSITION.md)
for the source event wrapper and force producer.

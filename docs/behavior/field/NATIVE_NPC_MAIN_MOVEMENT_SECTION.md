# Retained original NPC Main movement section

The [entry and clock controller](NATIVE_NPC_MAIN_ENTRY_AND_CLOCK_COMPOSITION.md)
can now compose the complete source region `80049e8c..80049fbc` through
`FieldNpcMainEntryBinding::advanceMovement`. This is a subsection, not full
`npcMain` completion or normal Area activation. Source authority remains the
[parent control flow](ORIGINAL_NPC_MAIN_PARENT_CONTROL_FLOW.md) and
[child ABI audit](ORIGINAL_NPC_MAIN_CHILD_ABI_AND_NUMERIC_FRONTIER.md).

## Captures and publications

Capture state1d4 once. State10 clear returns Detection before source8004a160;
otherwise read dt1a4 and source zero. Only strictly positive dt reaches movement;
unordered dt bypasses movement but still reaches side-response gating and
position subtraction. Captured state100000 without10000000 halves both pending
components using source.5. Capture mainflags once for08000000 zeroZ and40000000
direct movement. Direct mode publishes currentX+dx before reading currentZ,
then publishes currentZ+dz. It does not update Y, previous position, or contact.

Other movement requests source npcHitCheckSide80048418 with actual actor,
actual heading1ac alias, pending dx/dz and temporary X/Z initialized from
current position. These stack outputs are semantic locals; zero request lanes
mark local outputs and are never guest pointers. Successful response supplies
both locals and contact. Store contact2fc, then reload that word. Nonzero contact
causes a second call with temporary-minus-fresh-current components and actual
actor8c/94 output aliases. That child's return is discarded; it must publish
its alias effects itself. Null contact copies localX then localZ into the actor.

Reload source zero, clear1b4 then1bc, and request npcMoveG80048d9c. Only after
that child finishes, freshly capture mainflags and bypass npcCheckHitMarioSide
800490cc for any of8/20/4000. Otherwise request that child with the same actor.
Missing original child providers hold without effects. Fixture children used
by tests are explicitly fixtures, never ordinary Game fallbacks.

## Vector subtraction and boundaries

PSVECSubtract8029eba4 loads currentXY together, previousXY together, subtracts
and stores localXY, then freshly reads currentZ and previousZ and stores localZ.
The core retains these source operations. Its paired-load service captures both
words or neither; the native facade reads the two mapped lanes synchronously.
It does not expose a hold between the words of an original paired load. The
result is a local delta, not a history/snapshot publication. Arithmetic is a
binary32 native adaptation; Gekko GQR/FPSCR/subnormal/payload parity remains
unverified. No blanket SDK instruction-parity claim follows from host tests.

After subtraction, AutomaticPose is an active unfinished frontier before the
fresh mainflags load80049fbc. It does not bypass the source400/dt gates, norm,
phase-dependent animPoseSetAnim, event detection/interrupts, display/material/
shadow work or late history. Detection is also unfinished. Neither frontier
advances the parent actor loop, clears final state10, or completes Main.
Repeated calls at either frontier are effect-free. The entry cursor remains at
phase60 with its captured factor for future automatic-pose composition.

## Native ownership and validation

The binding first verifies the retained entry against the same owner, shared
pose alias and VM, then supplies actual actor memory to the subsection. The
movement provider receives that same VM for actual output and heading aliases.
No catalog key is cast to a packed actor pointer, and no replacement registry
or shadow actor storage is created. There are no heap allocations in the core
advance loop. Native builds use the existing ARMv6K hard-float toolchain;
performance on physical New2DSXL remains unmeasured.

Tests check an independent direct-path source trace; suspension at every
primitive across direct, one-hit, two-hit, special, paused and bypass paths;
real output aliases versus discarded return; X-store/fresh-Z order; postgravity
flags; all side bypass bits; special-state bypass; zero/negative/NaN dt;
stable frontiers and owner/actor refusal. A shared NPC/VM/pose fixture composes
a held gravity child, resumes without replaying entry/clock/position, and
preserves MainCount and the parent phase. See PROGRESS for exact whole-tree and
ARM validation, artifacts and remaining rendering limitations.

The read-only original SDK helper is36 bytes, SHA256
`5997a5a32c7df8eb2a03901039345f35a84f00b017545800218ef21bdbfaf81a`.
Ignored evidence: `artifacts/npc-main-parent-evidence/PSVECSubtract-review.*`
and `movement-review-notes.txt`. No helper binary enters source history.

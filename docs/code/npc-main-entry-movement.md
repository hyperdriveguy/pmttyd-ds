# NPC Main entry, movement and native composition

The [entry helper](../../source/field/field_npc_main_entry.cpp),
[movement helper](../../source/field/field_npc_main_movement.cpp) and
[automatic pose helper](../../source/field/field_npc_main_auto_pose.cpp) retain
individual original source operations. The
[native binding](../../source/field/field_npc_main_entry_binding.cpp) composes
these with detection, display, shadow, history and field tail. A helper frontier
is the next caller-owned work; it is not whole-Main success. The composed
`advanceMain` acknowledges one invocation only after all reached stages finish.
This does not automatically activate the normal Game/Area path.

See the [entry API](../../include/field/field_npc_main_entry.hpp),
[movement API](../../include/field/field_npc_main_movement.hpp),
[pose API](../../include/field/field_npc_main_auto_pose.hpp) and
[binding API](../../include/field/field_npc_main_entry_binding.hpp).
Earlier behavior documents describe implementation milestones; their early
“unfinished” wording should be read alongside the later
[whole-Main composition](../behavior/field/NATIVE_NPC_MAIN_FIELD_TAIL_AND_COMPLETION.md).

## Captured source pass and entry

GP at0x80415fd0 supplies context+0x14. That selects original NPC bank0x8041e8b4
or0x8041e8c8; count+4 and array+0xc are whole-pass captures. `marioGetPtr` returns
the admitted actual Mario, and the parent clears candidate+0x1dc while retaining
radial hint+0x1d8. MainCount at0x8041e8ac increments with 32-bit wrapping, after
the captured count read and before the array read, then publishes only after
native storage admission. Later GP/bank mutations do not replace that pass.

Positive counts must fit the actual ordinary36 or alternate16 records and their
exact array alias. Zero or signed-negative counts traverse no actors. This is
availability checking, not count clamping. The source walk uses stride0x340,
not a visible-actor list or a separate clock prepass. Initial flags require both
active mask1 and query mask2. Rejected slots skip clocks, history and final
state clear. They still advance the source index.

Admitted actor handle0x104 resolves the actual shared pose row. Source getter
arithmetic has no active/resource-ready test; native admission bounds handles
0..255 and requires the same pose-pool/VM alias. Initial actor mask0x8000 decides
pose-row mask0x100 even if the getter mutates later actor flags.

Fresh main mask0x100000 clear ORs state0x10; fresh state0x10000 then clears0x10.
Unsigned halfword0x320 decrements only when nonzero. The
[retained clock](npc-clocks-matrices-numerics.md) runs before a fresh query2
check. Losing query here takes the final clear/index path, keeping prior
pose/timer/clock effects while skipping the actor body/history.

Special-room processing copies previous state0x100000 into0x200000, freshly
clears0x100000, and compares fresh GP+0x12c text with original `kpa_03`.
Only Y greater than original365 rejects the height branch; NaN enters by the
source comparison. Entry into the special state requests the original waterfall
SFX with the actual actor root alias. Missing sound holds after completed stores.
Special state without0x10000000 loads100 then0.5, stores the retained100 word to
0x1d0, and retains factor0.5; other paths use factor1 without changing0x1d0.
A fresh special-state check ORs halfword0x31e mask1; ordinary paths do not clear it.

## Movement and automatic pose

Movement captures state0x1d4. Mask0x10 clear skips directly to detection without
producing a movement delta. Otherwise fresh dt0x1a4 must be greater than source
zero for pending motion. Zero, negative or unordered dt still reaches side
response and position subtraction. Captured special-state flags can halve
pending X/Z; main0x08000000 suppresses Z. Direct mode0x40000000 publishes X
before freshly reading/publishing Z, leaving Y/contact/history with their owners.

The collision path calls original `npcHitCheckSide` with semantic temporary X/Z
and a real actor heading alias. Zero request lanes mark local outputs, not guest
pointers. It stores then reloads contact0x2fc. Nonzero contact issues a second
call with temporary-minus-fresh-current displacement and real root output aliases;
the child publishes them, and its return is discarded. Null contact copies the
local X then Z into the actor. Source zero clears pending0x1b4/0x1bc before gravity.
Only after gravity completes do fresh flags8/0x20/0x4000 decide Mario-side response.

Position subtraction retains paired currentXY, paired previousXY, then fresh
scalar currentZ/previousZ. A paired read admits both lanes or neither. Delta is
local: it must not prematurely replace previous-position/history fields.
Binary32 operation boundaries stay explicit; tests do not prove Gekko GQR/FPSCR
or subnormal/payload parity.

Automatic pose requires fresh main0x400 and dt>zero. It separately rounds dx²,
dz² and their sum, then invokes the retained scalar-length graph. It reloads dt
after that child. Strict speed<original0.1 selects stay1; otherwise descriptor
threshold times retained factor selects walk2/run3. Unordered comparisons select
run after the idle test fails. Stay checks phase before descriptor lookup;
walk/run retain the descriptor from their threshold read across the phase check.
The later pose-name word is still freshly read at its source point.

Equal phase skips both selection and phase store. Otherwise the original force1
selection must succeed before publishing phase0x108. A held phase store cannot
replay the child. Negative signed selectors resolve bounded guest strings;
nonnegative selectors, including0, are indices. The binding requires an explicit
source pose clock and performs no animation Main/evaluation as a substitute.

## Composition, atomic children and retirement

The binding requires an initialized owner, its exact registry/VM facade, and the
shared animation-pool alias. It retains the owner and borrows VM/mappings/pool.
Base clock and Mario admission are required at bind time; later providers may be
absent, holding when reached. No placeholder effects acknowledge a missing owner.
Scalar/half stores update actual VM bytes. Native atomic children can retry their
internal read-only admission on hold; this differs from the core's individually
retained successful primitives. False/empty must have no child effects.

The event-ID getter freshly selects the actual current event context independently
of the captured NPC bank. It validates the resolved entry-array alias and count,
then scans live entries in source order for the first ID match. Pose material and
shadow leaves update actual shared rows, including mapped inactive slots. Camera,
queue, movement, audio, interrupts and field services keep explicit providers.
Kpa helper queries fetch Mario independently; they cannot reuse the field-tail
parent's retained pointer or repeat its outer type gate. Interpolation admits
only the audited source modes and arithmetic grouping.

The actor cursor retains every child through detection, display, shadow and late
history. Only after history does it resume entry phase78 for the fresh final
state0x10 clear and index advance. If the next entry holds, old child captures
remain until it reaches Movement or FieldTail. Then the binding retires them.
A next-actor clock hold must not replay the prior actor's history.

The whole-Main cursor runs actor traversal once, field tail once, then stays in
terminal phase2. Repeated completed calls have no effects; reset explicitly for a
new invocation. Provider identity, mappings, actor lifetime and queue/camera/source
pass survive holds and deferred dispatch. Room retirement discards cursors before
actor reuse. Queued draw execution lives separately in
[field_npc_draw_binding.cpp](../../source/field/field_npc_draw_binding.cpp);
calling Main does not itself dispatch or replace the renderer.

See [entry evidence](../behavior/field/NATIVE_NPC_MAIN_ENTRY_AND_CLOCK_COMPOSITION.md),
[movement evidence](../behavior/field/NATIVE_NPC_MAIN_MOVEMENT_SECTION.md),
[automatic pose](../behavior/field/NATIVE_NPC_MAIN_AUTOMATIC_POSE_SECTION.md) and
[history/traversal](../behavior/field/NATIVE_NPC_MAIN_HISTORY_AND_TRAVERSAL.md).
The local source composition remains separate from production wiring, original
runtime cadence, complete field rendering and physical hardware performance.

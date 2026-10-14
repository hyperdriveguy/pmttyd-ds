# Native shadow request owner and trio live display traversal

The portable ShadowWork retains the original64-slot,5c-byte request layout,
source initialization of the primary request fields, and two typed auxiliary
flag/counter owners. Entry copies XYZ/radius and current type/height/collision
settings, returns the original slot index, and preserves padding and matrix
bytes. It accepts zero/nonfinite radius/position as source Entry does; only the
native64-slot capacity boundary returns unavailable. Source authority:
[pool and cadence](BATTLE_SHADOW_REQUEST_OWNER_AND_DEFERRED_PREPARATION.md).

Main registers shadowDisp8002d3d8 with original eight camera/class/+0 tuples,
including the live preparation gate. A native cursor retains registration
progress if display capacity is unavailable. Only after registration completes
are the active shadow count, flag1 and counters reset; other flags, settings,
records and queued callbacks survive. Both auxiliary flag/counter resets are
retained. GPU images, collision matrices and cylinder drawing are separate.

## Native lifetime and admitted update binding

Game owns one shared request pool for its session and preserves it across
restore/reset. Trio imports that owner rather than constructing a private
battle pool. Direct harness callers can supply it explicitly or obtain a fresh
native request owner. This implements source request-field initialization and
retention; it does not assert complete original shadowInit/image allocation or
all field shadow-setting writer history. Existing field/harbor GPU shadows are
not yet collected through this owner.

Before the trio's finishing BattleMain completion pass, native code invokes
ShadowMain, then collects fresh player pose and shadow requests. The callbacks
therefore see the new live pool after Main's reset. This is an admitted ordering
binding, not complete original outer-loop cadence across every field/battle
frame. Deferred sort/camera2 preparation/camera4 draw/reset remain providers;
records are never marked prepared to bypass the original independent ray.

## Actual per-unit/per-part continuation

The finishing display owner now retains registry index, current part and the
completed-unit-prefix state across missing depth. It resumes after depth/Entry,
executes source shadow Entry and the zero-table sound tail, then reloads the
current part's live next pointer. Unit alpha/gauge/status/scale processing does
not repeat for later linked parts. Unit-tail callbacks use the original U308/
U1c/Bef4 admission and camera4/class2/+0 borrowed unit records. See
[live-loop authority](BATTLE_DISPLAY_LIVE_PART_LOOP_UNIT_TAIL_AND_FOOTER.md).

Automatic parts now execute source status-type priority, separate fixed28-pair
lookup, player status/name overrides, fast/slow rate and pending-bit1 admission.
Only a real pending bit consumes the selected name. Mario's two extra parts
retain descriptor0 and run Main; original attributes13010000 then suppress both
sprite registration and shadows. They are hidden by source conditions, not by
skipping the part loop. Forced finishing Mario and Goombella register live
part callbacks and real shadow records. Nonempty sound tables, special extra
registration, midpoint shadows and exceptional capacity continuations remain
explicit missing domains.

The exercised finishing traversal now retains eight shadow callbacks, Mario
and Goombella pose records and two unprepared shadows. It next holds at the
actual remaining unit pose-constructor provider801011fc. Remaining unit banks,
footer services, current-camera/deferred matrices/material/GPU providers,
complete camera4 history and ordered field return remain unfinished. Renderer
continues using its existing handheld view; request ownership alone is not
rendered parity.

Tests cover source layout/defaults, copied records, preserved padding/matrices,
Main registration order/reset/gate, capacity continuation and nonfinite input.
The real finishing-pose regression verifies both player records, the original
hidden extra descriptors, two unprepared shadow records, no primary Main replay
and no repeated collection while the next provider is unavailable.

# Retained original target-selection display

`BattleTargetDisplayCursor` retains the whole US revision0
`BattleCommandDisplay_TargetSelectMenuDisp` at `80120a44`, size `0x5a0`.
`verify_battle_target_display.py` pins its original ISO bytes, every linked
child and all consumed source constants. This is static source evidence;
no Dolphin execution, native GPU submission or playable battle is inferred.

## Source ownership and private arguments

The battle operand is original r4, captured for the whole invocation. Original
r3 is unused. The parent reads battle+1c5c before calling GetCursorPtr with
battle+171c. The reply's `value` represents the private output cell written by
that helper, rather than the helper's otherwise unused r3 return. Cursor word0
selects the 28-byte command row, whose weapon+64 mask01000000 selects the
single-target branch. It is independent from native actor slot selection.

Targets use battle+428, signed count byte+a6c, signed order bytes+a6d, and
36-byte descriptors starting+4. Single selection first sign-extends battle+edf.
Unit/part IDs and XYZ descriptor coordinates are signed halfwords. Actual unit
and part lookup calls remain separate. All-target mode freshly reads the live
signed count after each row, including after drawing that row's name.

ProjectPoint, IconPoint, WindowColor and CursorOutput use typed private
transport. Their integer fields contain only live non-private arguments in
source register order, with unused fields zero. No private guest SP is created.
The project reply supplies the screen vector; icon drawing preserves screen X/Y
and loads original Z/scale, flags16/24, and icon503. The attribute gate is unit
+104 mask0x2. After icon completion, fresh unit+8 kind226 calls pouchGetYoshiName;
other kinds freshly read part+8 and call msgSearch. There are no invented names.

## Arithmetic and child order

Every conversion preserves the original magic-double construction, original
bias loads, source subtraction rounded to single and source single addition.
Signed bias is at802efe78; unsigned bias at802efe80. Constants use original
r2=80427440 with signed D-form offsets. Single mode captures the coordinate bias
after reading X; all mode captures it before its first live count test.

Four original branches keep their own callsite identities and color words.
FontGetMessageWidth is narrowed to its low16 bits. The original window, font
start and name draw are separate retained children. All-target vertical spacing
advances32 only after the whole row completes; the text path reloads its signed
bias after FontDrawStart. The final camera-mode(0,0) call is required even for
zero/negative all-target counts, and must finish before this parent succeeds.

## Admission, holds and retirement

The caller proves total readable source mappings during synchronous read
groups. Its `current` callback owns actual battle allocation, VM, resource and
process lifetime checks. The parent pins all callback identities and the battle
operand. A changed provider or operand cannot resume an existing invocation.
Failed currentness retires the cursor permanently until explicit reset.

A missing linked or numeric provider holds at its reached source callsite.
Each request and numeric operand pair is captured once and retained across
nullopt; completed preceding reads, calls and draws are not replayed. Engaged
child replies mean completion of the entire actual child, including any native
render work it owns. Reentry and reset while invoking are rejected. While a
Draw is held, its owner must freeze CPU, event, animation and action-command
clocks; display retries are not simulation ticks.

## Remaining integration

This portable parent does not bind a native camera projection, icon submission,
window/font renderer or battle camera controller. Those actual source children
must be admitted before the queued Harbor Target callback can be acknowledged.
The retained callback alone does not activate normal Crump startup or establish
a tutorial playthrough. See [the source display pass](../display/HARBOR_SOURCE_DISPLAY_PASS.md)
and [the opening requirements](../story/CRUMP_OPENING_INTEGRATION_REQUIREMENTS.md).

# Battle reports, presents, resources and small state leaves

See [audience items](battle-audience-items.md) for impact metadata,
[part/HP presentation](battle-part-presentation.md) for visual consumers and
[battle controllers](battle-control.md) for the sequence owners. This guide covers
portable/packed leaves and their actual provider boundaries.

## AP and drop reports

AP input/output is twelve source bytes. Builder allocates the output pointer at
A+0x137c8 when needed, then copies forward byte-by-byte; aliases and callbacks can
affect later reads. Thirteen species coefficients at table 0x803b4de4, stride 0x20,
offset 0x14 accumulate through FMA, followed by linked square root and rounding.
AC/addition positivity and distinct Mario/partner flags select source multipliers.

Break-turn branch reads its bonus and performs one multiply before capturing the
old AP counter. Ordinary branch captures the counter first. Preserve that order,
the remaining multiply sequence, wrapped truncation credit and unconditional
random draw of 100. A point-increment child can fail after credit; Invite clears
input only after the complete builder/child acknowledge. This immediate builder
has no saved suffix and is not safe to restart as an effect-free retry.

Drop selection writes original setup outputs and grants nothing directly to the
pouch. Heart/flower tables contain 16-byte threshold/chance/attempts/per-attempt
rows; threshold zero ends the table. Resource ratio `current*100/max` clamps only
above 100. The first matching threshold uses one gate draw then one draw per
source attempt. Valid resource maxima/table counts/RNG domains are caller-owned.

Resource drops precede the story item gate. Story at most 19 gives none weight
100, but badge 314 fallback can still select from the copied 25-word source list.
Badge count is queried twice: first to scale held/back weights, later independently
to admit fallback. Mode 1 uses signed-half weighted group rows; mode 2 chooses
among eight nonzero held-item words. Do not cache these queries or change the
cumulative subtraction/RNG sequence.

## Present-item continuation

Items at least 339 and target outside -1..63 return zero. Registry query still
occurs before item-zero/negative rejection; accepting -1 in the range check does
not establish a nonnull unit. Other non-123/124 items publish marker/type 12/menu
29 and AllEnd mask 0xffeeeeee. That is neither actual UI destruction nor an item
grant; original presentation events remain separate services.

HP item 123/FP item 124 recover one point once. GetPos writes the real private
XYZ address in order; aliases remain observable. Signed height conversion retains
0x43300000/xor bias encoding and linked double-subtract/single-round boundary.
The retained cursor then runs scale/height FMA, Y addition and actual effect 0/1
with amount 1. Holds cannot repeat recovery. Success clears item through freshly
resolved current B and returns zero despite effects. Keep providers stable;
identity checks cover selected memory callbacks/private address, not every service.

## Menu files and curtain

Menu resource Init captures B/root/formatted path and calls original fileAllocate
mode 4. Format token 0x802ef810 addresses source storage; it is not a native string
literal or made-up path. Guest FileEntry +0xa0 owns a pointer cell for actual TPL;
a host FILE or atlas index cannot substitute. Engaged descriptor zero is stored
at captured B+0x163f8, while nullopt means unavailable. End captures descriptor
once, performs real release and leaves the B field unchanged. Repeating completed
End is not automatically idempotent. Fixed bindings are caller obligations.

Curtain supports only original fade type 23 and OFF_d_maku_left_right/A_2.
Unsupported type leaves prior work untouched. Reentry retains the old pose until
new resource readiness, then releases and requires real new allocation. Raw
duration/color remain stored inputs but do not drive this branch. At loop at least
0.5 each RGB channel decays by 0.98 with integer truncation, alpha survives and
flag 0x40 is set. Pose main then runs; a separate loop query at least 1 finishes
and releases. A raw duration is not an alternative pose clock or timeout.

## Status window and Majinai bytes

Status visibility is `!B[273c] && !B[2740] && (B[274c] || (B[2748] && B[2744]))`,
with offsets hexadecimal. Actual isOpen is queried every check; open/close only
run on transition. Event captures B before resolving its operand: 0/1 write 2748,
2 sets latch 2744, other values do not store. Check then independently captures B.
Completion 2 differs from the desired predicate and does not imply the same owner.

Majinai pouch bytes +0x5b8/5b9/5ba hold count/countdown/mode. Count zero skips
Check. Countdown high bit clears mode, requests bound 5 plus 6 and decrements via
a fresh byte read. Existing nonzero mode returns before countdown. A new mode
uses source setup flag 0x20 cutoffs 45/90, otherwise 30/60/80, with one draw of 100.
Init only increases a requested count by its source comparison, then narrows to
U8 without saturation. Done decrements even zero (wrap to 255), preserves its
original reread and resets countdown/mode. EndCheck only ORs battle 0x00400000
when count is zero; it does not clear that flag later.

Power query uses a raw output destination, without resolution/sentinel skipping.
Only mode 1 writes result 1, then battle byte +0x18ff9, Done and EndCheck. Output
callbacks can change fields read afterward. All these events complete with 2
independently of selected mode/result; no measured cadence or original SDK/Gekko
numeric parity is implied by the native provider implementation.

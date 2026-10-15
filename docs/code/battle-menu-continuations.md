# Retained class table and weapon menu work

These owners retain complete local source operations across unavailable children.
A completed scalar zero differs from `nullopt`; callbacks must retain their own
partial effects. Fixed provider identities do not pin guest allocations by
themselves. The caller retains actual VM, actor, pouch, definition and text owners.
Reset abandons metadata without undoing source writes. See
[weapon menu behavior](battle-weapon-menus.md) for synchronous helper context.
Field offsets and masks below are hexadecimal; strides/counts/icon IDs are decimal.

## Class rows and live eligibility

[Class table construction](../../source/battle/battle_action_class_table_continuation.cpp)
captures acting U, pouch P, nested setup definition D and the first-kind-selected
class cursor. M is wrapped B+0x171c; setup/definition reads still precede the
M-zero branch, whose raw result is P. The ordinary result is the live published
count. Neither is an EVT completion code or a boolean.

The row anchor is M+20*n, eight bytes before the actual 20-byte record. Type,
enabled, label, icon halfword and reason are at anchor+8/+0xc/+0x10/+0x14/+0x18.
Padding and unused rows survive. Denial writes enabled 0 then opaque source
reason 3. A missing equipment level only clears enabled; it need not change reason.

| Type | Admission and notable reads | Original icon |
| --- | --- | --- |
| 0 Jump | Initial kind 222; missing boots disables rather than removes row | Fresh signed P+0x98 after message: 1/2/otherwise gives 334/335/336 |
| 6 Party | Fresh kind 224..230; signed U+0x12b and U16 U+0x13c gate enabled | 343 |
| 1 Hammer | Fresh kind 222 and signed P+0x99 >=1; later level rereads remain independent | 337/338/otherwise 339 |
| 2 Item | Fresh Mario/party admission; later kind chooses counter versus U16 restriction | 340 |
| 4 Special | Fresh Mario kind and nonzero unsigned pouch halfword +0x8c | 341 |
| 5 Operations | Independent battle flag 0x08000000 clear test | 342 |
| 13 Lecture | Independent fresh set test of that same bit | 342 |

Every source denial check still runs after an earlier denial. Rows wait only at
their actual original message key, keeping anchor/count and prior stores. Message
callbacks may change later gates, levels or kinds; Operations/Lecture can both
appear or both disappear. There is no invented stable-state row cap or English
label fallback. Label addresses retain original text/control bytes and must live
through subsequent visible UI use, beyond this producer's completion.

Publication stores computed count, then reads index and live count. Signed upper
correction alone writes count-1 when index>=count. Negative indices remain;
zero count can produce -1. See [the retained table contract](../behavior/battle/RETAINED_ACTION_CLASS_TABLE.md)
for exact masks, keys and source store order.

## Weapon UI allocation and parallel arrays

[UIInit](../../source/battle/battle_weapon_ui_continuation.cpp) retains passed B
only as invocation metadata. Its Common captures actual current global B, reads
B+0x1c78, frees existing work, independently captures heap for allocation and
requests 0x124 (292) bytes. A held Free keeps its heap/pointer; later Allocate
sees a changed heap global. Constructor Free does not locally clear B+0x1c78.
The actual allocator result, including zero, is published there and freshly
reloaded into a private output. Source accesses remain unchecked; zero is not
reclassified as unavailable or replaced with a no-work success.

Two native private outputs model original separate initialized stack words.
Work+0 is the input weapon cursor, +8 rows, +0xc/+0x10 zero, byte+0x14 zero,
and +0x18 fresh selection. Acting kind 222 selects class cursor B+0x1bb4,
otherwise B+0x1bc0. That class cursor differs from the input weapon cursor.
Work+0x98 receives the original literal; seven parallel arrays start at +0xa4,
+0xc0,+0xdc,+0xf8 with 0,1,negative ordinal,0. This parent supplies no blanket
clear; the actual MapAlloc child owns allocation initialization.

Subtype iteration advances selection before each row and reloads signed count
at the loop head and selected-last test. It writes timer, ordinal and flags in
source order. Low input flag 1 chooses selected/other states 0/10; otherwise
1/11. The source loop has no artificial seven-row clamp. The allocation-current
predicate is checked between synchronous groups and after map children; actual
memory/maps stay frozen within each group. Observed loss poisons the invocation.
End independently uses its passed B, waits for real Free, then clears B+0x1c78.
See [UI work ownership](../behavior/battle/RETAINED_WEAPON_UI_WORK.md).

## Main, Setup and End

[Weapon Main](../../source/battle/battle_weapon_menu_continuation.cpp) owns its
independent initially unknown cursor output and actual nested End child. KeyOK
zero still reaches CommonDisplay without consuming that output. Accepted keys
query it, run eligibility update and preserve fresh pending-byte behavior.
Confirm 0x100 precedes navigation/cancel. Enabled selection waits for sound,
End, decision, target setup and distinct fresh lighting checks before result 1.
Disabled selection clears pending and tests reason. A nonzero test waits for
End before writing state 8 and freshly rereading reason into M+0x564; that later
reason may now be zero.

Forward navigation uses Repeat 0x20000 within signed bounds and Trigger at wrap;
reverse uses 0x10000. A movement returns through display immediately. Arithmetic
reloads index/count after the provider. Cancel 0x200 preserves type-2 badge/mode
branches and their different sound/state results. Otherwise the real physical-X
audience helper may End and mark pending while Main returns zero. Each ordinary
path still requests CommonDisplay once.

Setup waits for Table, Cursor and UIInit before fresh flags OR 0x01000010.
End waits for UIEnd before fresh B+0x1720 bit-0x10 clear. These operations do
not fabricate readiness, tables, choices or whole battle activation. See
[retained controllers](../behavior/battle/RETAINED_WEAPON_MENU_CONTROLLERS.md).

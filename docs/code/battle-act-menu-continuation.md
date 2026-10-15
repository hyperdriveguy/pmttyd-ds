# Retained ActMenu, class UI and audience admission

[The implementation](../../source/battle/battle_act_menu_continuation.cpp) contains
separate menu, UI-allocation and audience-admission cursors. Provider/B/entry/flag
identities are fixed across a hold; actual VM, resource and allocation lifetimes
remain caller-owned. Native private SP08 starts unknown, and an actual zero write
is known data. Unconsumed unknown output may remain unknown. A consuming site
holds without repeating an acknowledged lookup. Reentry/reset from callbacks fails;
reset does not undo source effects. See [class/weapon records](battle-menu-continuations.md).

## Menu Setup, End and position gate

Setup waits for ClassTable(B), Cursor(M,14) and actual class UI Init(B,C,M+8,F).
M is wrapped B+0x171c. It then zeroes M+0x560 and freshly ORs M+4 with 0x01000001.
For F=0 and fresh byte M+0x558=0, it resets exactly nine cursor index/scroll pairs,
preserving counts and omitting offsets 0x498/0x4a4/0x510. It neither clears the
whole menu nor writes type/state. End waits for real UI End before freshly clearing
only mask 1 at B+0x1720. Internal End uses a retained return PC, so a hold cannot
skip free work or repeat earlier sounds.

Position change captures battle flag blockers 0x00800000/0x40000000 once. It
obtains Mario then Party independently. Each must have signed mode +0x20 zero,
signed max +0x23 >0 and signed moves +0x22 equal that captured max, followed by
short-circuit status queries 1/2/9/27. Only Mario gets attribute +0x104 mask 0x10;
Party ends with byte +0x12b equal zero. It does not substitute the retained acting
unit or add Mario's restriction to Party. See
[Setup/End/position source order](../behavior/battle/ACT_MENU_SETUP_END_AND_POSITION_GATE.md).

## Main source order

Main captures acting U, queries its independent private class cursor, then gives
Trigger 0x800/position priority over KeyOK and class processing. If admitted, live
signed class count controls a validation scan. Only type 0 with battle mask 0x200
or type 1 with mask 0x400 invokes WeaponTable and TargetCount. Row/type remain
captured through those children; nonpositive target count disables and fills
reason 4/5 only when the current reason is zero. It never reenables a row.

Now(-1) resets idle timer/camera mode; otherwise wrapped timer+1 is compared signed
against 900. Every qualifying invocation requests camera mode 6 then speed 5.
These are source invocation counts, not a measured duration. Pending-confirmation
suppression uses fresh selected type/flags after those camera children. Trigger
0x100 or a freshly republished pending byte enters confirmation.

Enabled selection sounds before freshly copying selected type into M+0x540.
Types 0/1/4/6/7/8 select state 5/result 1; type 5 selects 19/1, type 13 selects
29/1. Type 2 can publish state 5 yet return 0, or mode zero plus badge bytes can
select 14/1. Unsigned defaults still End without a local state write. Disabled
selection may omit sound, clears pending, then tests fresh reason. Nonzero reason
waits for End before state 3/result 1 and a new selected-row reason read into
M+0x564; that later reason may now be zero.

Forward Repeat 0x80000 has priority over reverse 0x40000 and audience. Within
bounds, index mutation precedes sound; wrapping sounds before fresh cursor/count
mutation. Zero/negative count is not clamped. If neither direction fires, actual
audience true goes directly to display with local result still zero. Audience
false reaches a second independent Trigger 0x800 and position query. Their
answers may differ from entry. Successful position paths publish type 9/state 29.
No class cancel branch is borrowed from weapon Main. Display is the actual
return-only original leaf; its acknowledgment does not prove visible rendering.
See [selection](../behavior/battle/ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md)
and [navigation/tail](../behavior/battle/ACT_MENU_MAIN_NAVIGATION_AND_SERVICE_ORDER.md).

## Distinct retained class UI

UI phases capture old B+0x1c78, complete real release, complete 0x124-byte
allocation and publish source fields. Constructor Free does not clear the owner
word; End clears it after release. Allocation zero remains an actual pointer.
Work W is reloaded after publication. Unlike weapon UI, class rows are at W+4;
W+0 cursor, +0xc animation state, byte+0x14 readiness zero and +0x18 selection.
This parent leaves unowned words, including W+8, untouched and supplies no
implicit memset; allocation initialization belongs to the actual child.

Flag 1 initializes selected-first states 10/11, zero delay, reverse order
`count+ordinal-1` with one signed wrap correction, and zero flags. Flag 0 advances
to the next row first. Its delay uses fresh GP word +4 per row, wrapped rate*10,
signed division by 60, wrapped ordinal*step and signed division by 2. Preserve
both truncations and live count/selection reads; do not replace them with float
durations or fixed native FPS. There is no seven-row snapshot. The caller keeps
mapped memory stable within synchronous groups; this cursor has no live allocation
predicate. See [class UI layout](../behavior/battle/ACT_SELECT_UI_ALLOCATION_AND_NATIVE_BINDING.md).

## Audience wrapper

Admission checks every required nonoptional source callback before entering the
existing retained audience checker, even if a particular early branch would not
consume all of them. It pins private-item, lifecycle, target and both End-provider
identities. Engaged false completes that source invocation; `nullopt` retains the
actual child. It supplies no fake item, target or successful missing service.
The physical-X owner preserves its actual source selection and is separate from
optional targeted tapping. See
[physical interruption](../behavior/battle/NATIVE_PHYSICAL_AUDIENCE_INTERRUPT.md).

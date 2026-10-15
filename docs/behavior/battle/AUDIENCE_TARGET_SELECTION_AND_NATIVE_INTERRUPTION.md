# Audience target selection and native menu interruption

Static read-only US G8ME01 revision0 inspection: complete
BattleAudience_SetTarget801a2bec/9c, GetExist801a29c0/64, GetPtr801a8664/3c,
ItemGetPtr801a8624/40 and the already implemented ChangeStatus801a2a24/104.
[Menu verifier](../../../tools/verify_menu_interruptions.py) pins SetTarget and
its caller; [population verifier](../../../tools/verify_audience_population.py)
pins ChangeStatus/GetPtr and their original data. GetExist SHA256
`21d4c42ea7469e96002e8a917d4e5ed2736c391d5dcbed4200a0e2ebc445e95d`.
No Dolphin timing/raster or live crowd admission is inferred from static bodies.

## Exact target and holder cleanup

GetPtr(selected) resolves fresh current B and returns B+2858+1bc+selected*134
(all offsets hex). Retain that pointer and OR20 into its flags before scanning.
Scan slots0..199 in ascending order, skipping the selected slot before queries.
For each other slot, GetExist first makes its unused BaseGetPtr call, then a fresh
GetPtr. Capture flags once: require1 and exclude100 and2. No population counter,
visual count, item kind or source holder-state substitute is consulted.

An existing member gets another fresh GetPtr. Only byte19==7 enters cleanup.
Load signed half1c, get its item through a fresh current B+2858+f25c+index*48,
then clear that item's flags word. Store half-1 to the previously captured member
and call existing ChangeStatus(slot,0), which independently resolves current B.
Other item fields are retained. Signed indices and wrapped source arithmetic
are not clamped or changed into host pointers. The selected member keeps its
item/state; ineligible members and states other than7 remain untouched.

Fresh global lookups are observable. A write-side mutation of current B between
item clear and ChangeStatus does not redirect the retained member's halfword
store, but does affect the subsequent status helper. No target index is written
to audience cache here: [GetItemOn](CLASS_MENU_FOOTERS_AND_AUDIENCE_ITEM_QUERY.md)
already supplies its original index/position cache before this service.

## Retained menu interruption

`resumePackedBattleMenuAudienceCheck` follows the original
[interruption writer](AUDIENCE_INTERRUPTION_MENU_TYPE_WRITER.md). It retains the
battle captured before Trigger400, then item-query and target acknowledgement
across an unavailable native UI End. Resumption does not poll another trigger,
rerun GetItemOn or repeat SetTarget after their acknowledgement. Cursor memory
owner/read/write functions and private output identity must still match.

Previous menu type is read only after acknowledged ActMenuEnd. The original
stores are previous type at B1c60, type11 at B1c5c and Input state29 at B171c;
the source light leaf follows, then fresh flags OR4. No pending byte, saved Input
state, selected weapon or attack-event result is fabricated. A synchronous
wrapper remains for existing callers; service waits requiring retained admission
must use the cursor API.

The native HarborTutorialPacked owner now supplies this shared service to class,
weapon and target callers. Its producer runs whole original GetItemOn for matches
as well as no-match. SetTarget runs the complete cleanup above. ActMenuEnd uses
the existing real UI release owner, clears its work pointer through original
End and clears only Act-menu flag1. Native class/weapon visibility follows that
release. The fixed weapon projection is its existing explicit adapter; tracked
class allocations use their actual SDK free path. Unknown UI owners hold and
are never declared freed. BattleStageOffLightInCommand is the verified blr leaf.

This helper does not supply a whole outer CommandInput/Main continuation. The
existing upper command-unbound latch still gates failed outer invocations;
normal known UI releases complete synchronously. A direct helper continuation
is verified for an unavailable/replaced UI owner, without claiming that the full
original scheduler resumes that frontier. Full Game class-menu activation,
AudienceAttack event/action/receipt effects, harmful-item telegraphs, optional
lower-audience tap selection and finishing field return remain open. Physical X
retains the source first-eligible query, including possible good-item holders;
no new bad-item filter changes the original button semantics.

## Validation scope

Host checks cover selected preservation,199/200 boundary, paired/flag2/inactive
holders, status filtering, signed item indices, live global mutation, retained
query/UI waits, changed output/target/global state during a wait, wrong owner,
no-match and battle blocker. The packed bridge uses the actual general battle
pad sample path for X, real GetItemOn/SetTarget/UI End and type11/state29. It
leaves HP unchanged and creates no attack event. Test constants/actors are
explicit fixture storage, not story audience construction or an item reward.

The existing private original harbor event pack was checked against the read-only
ISO for the three required status/query spans (vectors802fa104/30,
zero80424988/4, Z offset804249cc/4). No pack or image conversion changed.
Exact builds, ARM fixture and remaining work are recorded in
[PROGRESS](../../PROGRESS.md).

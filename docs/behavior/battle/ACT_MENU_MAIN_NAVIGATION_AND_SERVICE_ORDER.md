# ActMenuMain: disabled confirmation, navigation, and common tail

This completes [selection/type publication](ACT_MENU_MAIN_SELECTION_AND_TYPE_PUBLICATION.md)
for8012230c/0x488. Static US G8ME01 revision0; no original runtime trace.
Use retained B/M/U, private SP08 cursor output, and local result R initially0.
Original field accesses use wrapping word arithmetic and big-endian typed
loads. The [verifier](../../../tools/verify_act_menu_control.py) includes the
whole body; called audio, camera, UI, target, pad, and audience services remain
separate contracts unless expressly covered by linked documentation.

## Disabled confirmation and reason panel

When confirmation finds enabled word0 at80122524, read BYTE(B+0x1c74).
If zero, request psndSFXOn with original key address802efe88:
`SFX_NOT_CHOICE1`. If nonzero, omit that sound. Then always clear
BYTE(B+0x1c74), reload current cursor/index, and read the selected reason.
A zero reason falls through to navigation. A nonzero reason:

1. Request ActMenuEnd(B).
2. Store WORD(M+0)=3 and set local R=1.
3. Reload current cursor/index AFTER menu end, derive row, read its reason.
4. Store that fresh reason to WORD(M+0x564).
5. Request common display and return1.

There is no type publication in this path. A port must not retain the first
reason as the displayed value; menu-end services occur between the two reads.
Similarly, the cursor used to test enabled is not a permanent selected-row
snapshot for all later operations.

## Direction-repeat priority and cursor arithmetic

At8012265c, request BattlePadCheckRepeat(0x00080000). Nonzero handles the
forward direction and skips all reverse/audience/position gates. Reload cursor
from SP08, then read count before index. Compute wrap32(count-1); compare
index and that result signed.

- If index<count-1, store wrap32(index+1), then request cursor sound.
- Otherwise request cursor sound first, reload cursor from SP08, then store
  index0. Do not move the store before the sound request.

If the first repeat gate is zero, request BattlePadCheckRepeat(0x00040000).
Nonzero handles reverse direction, skipping audience/position gates. Reload
cursor and read signed index. If index>0, store wrap32(index-1) then request
cursor sound. Otherwise request cursor sound first, reload cursor, read its
current count, and store wrap32(count-1). Negative indices follow this wrap
branch; no zero-count or negative-count guard appears here.

Every cursor-sound request passes original address802efef4,
`SFX_BTL_CURSOR_MOVE1`, to psndSFXOn. The key is a source token for original
sound assets, not a generated tone or an invented menu message. Both direction
branches go to common display and return0; navigation does not set R1.

## Audience and late position path

If both direction gates are zero, request BattleCommandAttackAudienceCheck()
without a battle argument. A nonzero result goes straight to common display;
R stays0, even if this service changes menu fields. This slice pins the helper
identity without claiming its complete global/event ownership.

If audience returns zero, request BattlePadCheckTrigger(0x0800) AGAIN. A
nonzero result requests ChangePositionEnable(B) AGAIN. If this also returns
nonzero: sound2520, ActMenuEnd(B), WORD(M+0x540)=9, localR=1,
WORD(M+0)=29, then common display. Otherwise just common display. These
are real repeated service requests at80122720/80122730, not cached results
from entry. Services and earlier menu operations can change their answers.

## Common display and return

At80122774, request BattleMenuDisp_ActSelect_Main(B), then return localR.
The early successful position route makes its own identical display request
before directly returning1. All other paths make the common request exactly
once. The complete original display symbol at80116cd8/4 is a single `blr`;
this particular linked UI Main is a no-op and does not assign a return value.
Other ActSelect Init/End symbols are actual larger bodies and remain linked
service boundaries. Do not infer that all original menu rendering is absent
because this one stub has no draw instructions.

ActMenuMain returns its local0/1, not event completion2 or a called helper's
return. An enabled selection may publish a type and close the menu yet return0
(type2's ordinary branch or switch-default entries). Audience mutation may
also occur while R remains0. Therefore a port must not equate `return1` with
“any state changed” or regenerate type selection from the return value.

## Implementation checks derived from the body

Meaningful cases include live class-count changes, reason preservation after
failed weapon-target checks, timer overflow/signed threshold, fresh flag reads
before pending-confirmation clearing, sound changing the selected row, reason
changes during End, type2's short-circuit byte reads, unsigned switch defaults,
zero/negative count navigation, reverse/forward simultaneous priority, and
changed answers to repeated trigger/position gates. Preserve the original
request order with address-backed fixtures; do not replace unproved source
invariants with clamping or fabricated default weapons.

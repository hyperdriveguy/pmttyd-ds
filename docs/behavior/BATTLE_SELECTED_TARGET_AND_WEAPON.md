# Retained selected target and command weapon address

US G8ME01 revision0; recovered 2026-10-12 by documentation agent ID2.
Static evidence: complete GetSelectEnemy8010b540/a4,
CommandGetWeaponAddress801057e8/a8, BattleGetSelectWeapon8012021c/4c and
_btlcmd_GetCursorPtr80124cb8/110. Reproduce reads and service order below;
menu construction and original execution remain open. Verification:
`python3 tools/verify_weapon_selection_cost.py` against the read-only ISO.

## Selected target output

Let W be shared BattleWork and B=W+0x428. Capture both raw event output
operands before writing either. If S8(W+0xe94)<=0, write -1 to the first
(unit) output, then -1 to the second (part) output and return event result2.
Otherwise read m=S8(B+0xab7), j=S8(B+0xa6d+m), then row=B+4+0x24*j.
Write S16(row+0) to the first output through evtSetValue; only after that call
read S16(row+2) and write it to the second output. Return2. The count at
W+0xe94 is B+0xa6c. Signed indices and count are not clamped or validated.

This is a retained-selection query: it performs no target choice, random draw,
translation, HP/status eligibility check or resampling. Do not snapshot both
halfwords before the first output callback. Aliased outputs end with the second
write. Selection/permutation producers and field names beyond these local roles
remain unverified.

## Weapon-address event service

Capture W before resolving operands. Resolve actor operand0; retain raw output
operand1; translate actor through BattleTransID; fetch the unit through the
then-current shared BattleWork. Read signed word kind U+8.
For kind222 or224..230 call BattleGetSelectWeapon with the captured W.
For every other kind, read word held-item ID U+0x308, calculate the word address
0x803108a8 + wrap32(40*ID) +0x24 and read its weapon pointer. This ItemData
span is0x3520 bytes (340 records), but this service supplies no index check.
Write the pointer to raw output1 through evtSetValue and return2. There is no
null test or fallback weapon. Do not resolve the output operand as a value.

## Command selection and cursor ownership

BattleGetSelectWeapon uses M=W+0x171c and T=word(W+0x1c5c). It passes
(M,T,&cursor) to _btlcmd_GetCursorPtr. The stack cursor is not initialized;
the caller ignores the helper's scalar return. It reads index=word(cursor+0)
then returns word(M+wrap32(28*index)+0x80). No count/index validation occurs.

The cursor helper first fetches the current acting unit using word(current
shared W+0x420), even if M came from another captured W. Unsigned T>14 leaves
its output untouched. The15-entry dispatch table at0x80364108 supplies:

| T | Cursor output relative to M |
| --- | --- |
| 0 | +0x4b0 |
| 1 | +0x4bc |
| 2 | +0x51c |
| 3 | +0x528 |
| 4 | +0x4ec |
| 5 | +0x4f8 if acting kind222, otherwise +0x504 |
| 6 | +0x4c8 |
| 7 | +0x4d4 |
| 8 | +0x4e0 |
| 9 | No output write |
| 10 | +0x510 |
| 11..13 | No output write |
| 14 | +0x498 if acting kind222, otherwise +0x4a4 |

The kind tests dereference the acting unit without a null check. The alternative
includes every non222 kind. Negative T also takes the unsigned out-of-range
path. Unsupported modes therefore leave BattleGetSelectWeapon with an
uninitialized pointer; retain a valid-menu invariant rather than inventing a
fallback. Menu labels, entry writers, type transitions and target reachability
require separate evidence.

## Ordinary Jump binding and acceptance cases

The [ordinary Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) calls selected
target at0x80355db4 into L3/L4, weapon-address at0x80355ddc with (-2,L12),
and [payment](BATTLE_WEAPON_COST_AND_PAYMENT.md) at0x80355e08 with (-2,L12),
after AttackDeclare/WaitGuardMove. Its first-attack branch supplies the default
Jump weapon directly and bypasses these ordinary selection/payment calls.

Implementation checks should cover count0/negative outputs, signed permutation
lookup, ordered callbacks with aliases, Mario/partner command lookup versus
other-kind held-item lookup, both kind branches for types5/14, and untouched
cursor output for9/11..13/out-of-range. An adapter may report an invalid state,
but such a report is a host policy, not an original default weapon.

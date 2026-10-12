# Action-class table: eligibility checks and original menu keys

This is the per-row companion to [admission and publication](ACTION_CLASS_TABLE_ADMISSION_AND_PUBLICATION.md).
It documents the complete static `_btlcmd_MakeActClassTable` body, US G8ME01
revision0. All offsets and masks below are literal recovered values; reason3
is not assigned an invented status name. No enable rule removes an admitted
row. The [verifier](../../../tools/verify_action_class_table.py) pins the body
and original key bytes; actual localized text/icon rendering is a separate
asset consumer contract.

Use retained B/U/P/D/M and current row address R as defined in the companion.
`W`, `S8`, `U8`, and `U16` mean big-endian word, signed byte, unsigned byte,
and unsigned halfword reads. For any check described as **deny**, store word
0 at `R+0x0c`, then word3 at `R+0x18`. An **enable-only clear** stores word0
at `R+0x0c` and preserves the current reason. Perform every check in order,
including reads after a previous denial; do not short-circuit the list.

## Jump, type0

At `80124e30`, initialize type0, enabled1, reason0, in that store order.
Then perform these reads/checks:

1. `W(U+0x104) & 0x00000010 !=0`: deny.
2. `S8(P+0x98) <1`: enable-only clear.
3. `U8(U+0x300) !=0`: deny.
4. Signed `W(B+0x19050) >0`: deny.
5. `W(D+4) & 0x01000000 !=0`: deny.
6. `W(B+0x0ef4) & 0x06800000 !=0`: deny.

Request `msgSearch(0x802f0018)`, then store returned address to `R+0x10`.
Read `S8(P+0x98)` again *after* this lookup: exactly1 selects icon334,
exactly2 icon335, every other value icon336. Store the icon halfword and
then set local count to1. A missing Jump level still emits a disabled row;
it is not Hammer's admission policy. The setup definition D is captured at
entry, but its flags word is read here.

## Party technique, type6

Store type6. Read `S8(U+0x12b)`; nonzero initializes enabled0/reason3,
zero initializes enabled1/reason0. Next, `U16(U+0x13c) !=0` denies, then
`W(B+0x0ef4) & 0x04800000 !=0` denies. Lookup `0x802f002c`, store name,
increment local count, and store icon343 as a halfword. No pouch-level,
setup-definition, or Mario-counter checks occur in this stage.

## Hammer, type1

After the separate kind/level admission in the companion, initialize type1,
enabled1, reason0. In order:

1. `W(U+0x104) & 0x00000010 !=0`: deny.
2. Fresh `S8(P+0x99) <1`: enable-only clear, distinct from the earlier admission read.
3. `U8(U+0x2ff) !=0`: deny.
4. Signed `W(B+0x19054) >0`: deny.
5. `W(D+4) & 0x02000000 !=0`: deny.
6. `W(B+0x0ef4) & 0x05800000 !=0`: deny.

Lookup `0x802f0044`, store name; reread `S8(P+0x99)` afterward. Exactly1
selects icon337, exactly2 icon338, all other values icon339. Store halfword
icon and increment count. Do not turn the fresh level check into a second
row-admission test or preserve only the first level read for the icon.

## Item, type2

Store type2. `S8(U+0x12b) !=0` initializes enabled0/reason3; zero initializes
1/0. Read kind again. If exactly222, signed `W(B+0x19058)>0` denies.
Otherwise `U16(U+0x13e)!=0` denies; this second branch does not revalidate
that the actor still has a party kind. Then
`W(B+0x0ef4) & 0x1f800000 !=0` denies. Lookup `0x802eff0c`, store name,
increment count, store icon340 halfword. This stage does not count available
items or scan inventory; such behavior belongs to its other consumers.

## Special, type4

Initialize type4/enabled1/reason0. `W(U+0x104) & 0x00000010 !=0` denies,
then `W(B+0x0ef4) & 0x1b800000 !=0` denies. Lookup `0x802f0058`, store
name, store icon341 halfword, increment count. The pouch admission is an
unsigned nonzero halfword at +0x8c, not an inferred signed level or FP cost
comparison. No setup-definition or battle-counter restriction is checked here.

## Operations, type5, and Lecture end, type13

The independent admissions use two reads of battle flag bit0x08000000,
not one saved value. Operations initializes type5/enabled1/reason0, then
rereads `W(B+0x0ef4)` and denies if masked by `0x1f800000`. Lookup
`0x802f0074`, store name, increment count, store halfword icon342.

Lecture end initializes type13/enabled1/reason0, looks up `0x802f008c`,
stores name and halfword icon342, **then** rereads `W(B+0x0ef4)` and denies
if masked by `0x10800000`. Increment count after this check. A port that
collects all eligibility before message lookup changes the original ordering.
Type13 has no output-writing cursor case in `_btlcmd_GetCursorPtr`, while
[attack-event selection](BATTLE_ATTACK_EVENT_SELECTION.md) routes it to
LectureEnd. Its presence in a class table alone does not prove a later
selected-weapon query receives that type.

## Original message keys and presentation fields

The retained key-base address is `0x802efc28` (signed addi -984 after
lis 0x802f); do not calculate it as 0x802ffc28. Calls pass these actual
NUL-terminated keys to msgSearch, not English substitute labels:

| Type | Key address / base delta | Original key | Icon |
| --- | --- | --- | --- |
| 0 | 802f0018 / +3f0 | `btl_cmd_act_jump` | 334,335,336 |
| 6 | 802f002c / +404 | `btl_cmd_act_tech_party` | 343 |
| 1 | 802f0044 / +41c | `btl_cmd_act_hammer` | 337,338,339 |
| 2 | 802eff0c / +2e4 | `btl_cmd_act_item` | 340 |
| 4 | 802f0058 / +430 | `btl_cmd_act_super_action` | 341 |
| 5 | 802f0074 / +44c | `btl_cmd_act_operation` | 342 |
| 13 | 802f008c / +464 | `btl_cmd_act_lecture_end` | 342 |

Store msgSearch's return unchanged even if zero; this body neither substitutes
text nor tests lookup success. Icons are literal source indices, not generated
art assets. Reuse msg/US data and original icon resources with retained keys.
The linked `msgSearch` identity is 80081b8c/0xe0; `pouchGetPtr` is
800d5eb0/8; `BattleGetUnitPtr` is 800f88cc/0x3c. Their body hashes are
pinned to identify the services, without claiming a full message-service or
pouch-lifetime recovery in this slice.

Suggested meaningful native checks are retained versus changing kinds,
missing Jump versus missing Hammer admission, negative byte levels and icon
fallbacks, denial-reason preservation, signed counters, message-triggered
flag changes producing both/neither final class, untouched descriptor padding,
and negative/equal/above-count cursor indices. They are implementation
requirements derived from instructions; no native tests or runtime behavior
are supplied by this documentation-only delivery.

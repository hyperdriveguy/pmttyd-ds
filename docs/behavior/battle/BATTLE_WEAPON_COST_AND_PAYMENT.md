# Weapon cost, shared FP, affordability and payment

US G8ME01 revision0; recovered 2026-10-12 by documentation agent ID2.
Static evidence: GetWeaponCost80126968/158, CheckWeaponCost801268fc/6c,
PayWeaponCost80126840/bc, event PayWeaponCost801059a0/74,
GetFp80126c64/44, SetFp80126c10/54, BattleGetMarioPtr800f87a8/6c,
pouchEquipCheckBadge800d3300/c8, pouchGetAP800d3c44/c and
pouchAddAP800d3c00/44. No original execution/affordability UI claim.
Verification: `python3 tools/verify_weapon_selection_cost.py`.

## FP cost calculation

Input unit U and weapon V. Capture signed word item=V+8 and unsigned byte
baseFP=V+0x11. If item0, skip badge counting. Otherwise call
pouchEquipCheckBadge(item), retain its count K, then RELOAD V+8 to select:

| Reloaded item IDs | Cost before reduction |
| --- | --- |
| 240..243,247..249,252,256..257,332..334,337..338 | Convert wrap32(K-1) as signed32 to binary64; compute pow(2,exponent), multiply by binary64 baseFP, then fctiwz to signed32 |
| 244..246,251,253 | low32(baseFP*K) |
| All others | baseFP |

The badge query compares the input signed32 against every signed halfword in
200 slots at pouch+0x38a+2*i; no terminator or mutation. The counter is0..200.
Zero is countable by that leaf, but the initial item0 skips it here. Preserve
capture/reload ordering if services can mutate state.

For resulting word C: if C!=0, subtract U8(U+0x2f5) with word arithmetic;
if the signed result<=0, set1. Finally if signed C>=1000, set999. Return C.
An initially zero C stays zero. With K0, an exponential half-cost may truncate
to0 and linear scaling produces0; neither is forced to1. There is no clamp
before floating conversion. Extreme pow/fctiwz conversion behavior requires
original numerical/FPSCR evidence; a huge count does not establish cost999.
The [badge import](../unit/UNIT_BADGE_COUNT_IMPORT.md) supplies U+0x2f5; the local cost
routine does not depend on action-command success or damage.

The default Jump0x803538a8 and Hammer0x80353de8 both have item0, FP byte+0x11=0
and AP byte+0x12=0. Byte+0x10 is100 and is not an FP cost. AP is separate.

## FP owner and narrowing

GetFp loads kind, shared W and S16(U+0x112). For kind224..230 it replaces the
result with S16(Mario+0x112), where BattleGetMarioPtr(W) scans registry slots
0..63 ascending for the first nonnull unit with signed kind222 and S8(U+0xc)=0.
It ignores HP/status/flags and returns null if absent. Other kinds use their
own FP, including kind222 without that registry search.

SetFp routes224..230 to the same Mario lookup, sign-extending the input's low16
before lookup; other kinds store its low16 directly at their own+0x112. There
is no clamp, HP check or pouch synchronization. Partner callers dereference a
missing Mario pointer: valid ownership is a required invariant, not a fallback
to partner FP. Do not hard-code Mario's registry slot.

## Affordability query

CheckWeaponCost captures GetFp(U), then calculates GetWeaponCost(U,V).
If signed availableFP<cost, return0 without an AP query. Otherwise read
pouchGetAP (S16 pouch+0x7a), reread U8(V+0x12), and return1 when
availableAP>=100*thatByte, else0. It changes no resource. This query is a
separate service; payment never invokes it. Retain ordering and byte rereads.

## Ordered payment

Core PayWeaponCost captures W, then obtains GetFp(U) and GetWeaponCost(U,V).
Call SetFp(U,wrap32(retainedFP-C)). After the store, reread signed kind U+8:

| Kind | Action-record operations relative to captured W |
| --- | --- |
| 222 | AddPoint(+0x16f4b,U8(C)), then AddCount(+0x16f4c) |
| 224..230 | AddPoint(+0x16f4d,U8(C)), then AddCount(+0x16f4e) |
| Every other kind | None |

Then for EVERY actor call pouchAddAP(-100*U8(V+0x12)). Zero FP cost still
increments the appropriate action count; zero AP cost still calls AddAP(0).
There is no affordability test, refund, item removal, HP/status test or
success-result dependency. Resource shortage can therefore narrow into negative
FP. The event wrapper resolves actor then weapon, translates/fetches the actor,
calls this core and returns2 with no output operand and no core return use.
See [selection](BATTLE_SELECTED_TARGET_AND_WEAPON.md) for ordinary Jump's call.

pouchAddAP first adds the raw word delta to S16(current+0x7a), stores low16,
then reloads shared pouch and clamps a negative current to0. It subsequently
reloads current and signed maximum+0x7c and caps current when current>maximum.
The lower-then-upper order can write a negative maximum if that state is
invalid. Addition narrows BEFORE clamps; overflow is not a wide saturating sum.
The leaf does not establish a useful returned AP value or on-disk save write.

## Action-record byte arithmetic and acceptance cases

BtlActRec_AddPoint8018f96c/24 reads U8, adds its supplied word delta, stores
low8, reloads U8, then caps values>100 to100. Thus100+200 becomes44, while
0+(-1) becomes255 then100. Payment supplies U8(C), so cost999 supplies231.
BtlActRec_AddCount8018f990/18 reads U8; if>=100 it leaves it unchanged,
otherwise writes old+1. A corrupt count101 is preserved, not repaired.
These are independent helpers, not an ordinary saturating arithmetic library.

Useful implementation cases: base0 with a reduction byte; K0 in both scaling
classes; reduction equal to/nontrivially greater than nonzero cost; cost999;
partner routed to a nonzero Mario slot; failing FP query skips AP access;
negative AP and low16 overflow; payment with insufficient FP; zero-cost action
count; byte wrap before point cap and count101 preservation. Shared resource,
menu-check cadence, record initialization/consumers, numerical extremes and
original execution remain open. This contract does not establish complete
inventory, action-command, damage or save ownership.


## Complete action-record/first-process follow-up (2026-10-12)

[Byte accounting](BATTLE_ACTION_RECORD_BYTE_UPDATES.md),
[turn-rule sums/status writes](BATTLE_ACTION_RECORD_TURN_RULES.md) and
[first-process/fog requests](BATTLE_TURN_FIRST_PROCESS_COMPLETE.md) now close
local record helpers,judgment-before-counter order and64-slot preparation.
AddPoint narrows before its cap;count/halfword/unit-byte rules differ. General
rule/action/reset/persistence/source/turn/backend and original traces remain open.

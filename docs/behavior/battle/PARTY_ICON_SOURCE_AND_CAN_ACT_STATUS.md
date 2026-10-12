# Party icon source and complete CanActStatus request policy

Companion to [party producer](PARTY_SELECTION_TABLE_PRODUCTION_AND_CAPTURE.md)
and [operation rows](OPERATION_TABLE_CHARGE_DEFEND_ESCAPE_AND_KEYS.md).
Static US G8ME01 revision0;complete _battleGetPartyIcon80123524/0xc4 and
BtlUnit_CanActStatus8012724c/0x90. [Verifier](../../../tools/verify_operation_party_tables.py)
pins four complete bodies/472 instructions,12 linked service identities,and32
source spans. Reused data is original;no observed native/original UI equivalence.

## Source order and different scan bounds

Table base80363f20 has these eight producer records,stride16. Fields are kind
WORD+0,name-key WORD+4,icon U16+8,padding+0xa..b,and help-key WORD+0xc:

| Producer ordinal | Kind | Initial icon | Name key address | Help key address |
| --- | --- | --- | --- | --- |
| 0 | 0 | 0 | 0 | 0 |
| 1 | 224 | 349 | 802efc50 | 802efc68 |
| 2 | 225 | 350 | 802efc88 | 802efca0 |
| 3 | 227 | 359 | 802efcc0 | 802efcd4 |
| 4 | 226 | 352 | 802efcf4 | 802efd08 |
| 5 | 228 | 360 | 802efd28 | 802efd3c |
| 6 | 229 | 351 | 802efd5c | 802efd70 |
| 7 | 230 | 361 | 802efd90 | 802efda8 |

The verifier pins those128 bytes PLUS the following16 bytes because the icon
helper's actual loop probes source ordinals1..8,not0..7. Its eighth probe at
80363fa0 reads the first words of the adjacent confusion operation switch,
not an invented ninth party definition. Do not silently convert both routines
to the same container bounds or assign a new party to the trailing record.
All14 original nonzero name/help strings are pinned too. Yoshi-kind226 bypasses
its source name token in the producer and requests pouchGetYoshiName instead.
Other names use msgSearch;help fields retain KEY ADDRESSES. Reuse msg/US
text/control tokens,original custom name data,and icon resources.

## Complete icon search and Yoshi color branch

Argument K is kind word. Starting local counter0,each of8 iterations computes
base+16*(counter+1),reads its current kind,and compares to K. First match wins.
No match returns0. For any matched kind OTHER than226,return U16(record+8).
There is no actor/pouch query in that branch.

Matched226 requests pouchGetPartyColor(4);the argument is literal original
party slot4. Treat returned word unsigned;>6 (including negative signed words)
returns icon352. Otherwise dispatch through complete table803640ec:
color0→352,1→353,2→354,3→355,4→356,5→357,6→358.
No null guard,palette clamp,actor replacement,or rendering request occurs here.
The producer's name/HP owners and this helper's fresh color service can differ
under mutations;do not cache the color before caller presentation services.

## CanActStatus:short-circuit services and final flag predicate

Retain passed U. Request BtlUnit_CheckStatus(U,1),then2,then9,then27.
Any nonzero return immediately returns0,omitting later requests. If all four
are zero,request BtlUnit_CheckStatusFlag(U,1). Return1 iff this final word is
zero;otherwise0. The cntlzw/shift is an exact zero predicate,not a sign test.
No HP,unit104,unit12b,actor-kind,pouch,or setup-field read appears in this body.
Services can mutate U between requests;do not evaluate all statuses upfront.

Status request IDs/flag mask1 are literal caller arguments. Their concrete
packed status semantics remain linked service contracts rather than guessed
names. [Position-change admission](ACT_MENU_SETUP_END_AND_POSITION_GATE.md)
has additional ordered checks and is not identical to this helper.
Operation Change Party calls CanActStatus but ignores its return;Escape
stores it as enabled before later current-field/setup checks. Party producer
never calls it. Preserve those caller-specific uses.

## Implementation and remaining work

Implement actual source ordinals,typed icon reads,color unsigned default,
first-match search,current-global services,and short-circuit action admission.
Use [cost protocol](BATTLE_WEAPON_COST_AND_PAYMENT.md) for separate charge
requests. Leading charge heads/keys are evidence of bindings,not full script
closure. Actor/party ID translation,custom-name/color/pouch provisioning,
operation/menu consumers,party key1 weapon callbacks,query-event source
reachability,Game/platform/original runtime/full-game requirements remain open.

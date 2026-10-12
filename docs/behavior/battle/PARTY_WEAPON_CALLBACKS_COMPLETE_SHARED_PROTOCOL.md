# Complete remaining party weapon callbacks:shared protocol and Koops exception

Static US G8ME01 revision0. This closes the six callback bodies left open by
[Goombella](GOOMBELLA_WEAPON_TABLE_CALLBACK_AND_TECH_LEVEL.md) and the
[seven key1 source chains](PARTY_WEAPON_CALLBACK_KEY1_SOURCE_BINDINGS.md).
[All-callback verifier](../../../tools/verify_all_party_weapon_callbacks.py)
pins all seven bodies plus complete partyGetTechLv,1296 instructions,12 hashes
(8 complete/4 linked),and71 source spans. It additionally compares all183
instructions of five callbacks against Christine after normalizing ONLY source
address immediates and absolute direct-call destinations. Koops is checked
separately by its full181 instructions. No original execution claim.

## Full callback addresses and append policy

| Party/original namespace | Callback | Size | Complete protocol |
| --- | --- | --- | --- |
| Flurrie/unit_party_clauda.o | 8017fc68 | 2dc | Christine pattern with its own source pointers |
| Yoshi/unit_party_yoshi.o | 80180f34 | 2dc | same,own pointers |
| Ms. Mowz/unit_party_chuchurina.o | 80181db0 | 2dc | same,own pointers/order |
| Bobbery/unit_party_sanders.o | 80182600 | 2dc | same,own pointers |
| Vivian/unit_party_vivian.o | 8018303c | 2dc | same,own pointers |
| Koops/unit_party_nokotarou.o | 801890a0 | 2d4 | same admission/count/name policy;two literal icons below |

Each ABI is `(passedMenuM,mutableCountAddressC)`. All callbacks retain M/C,
query CURRENT global B→BattleGetPartyPtr(B)→returned unit kind→BattleTransPartyId
→partyGetTechLv,then capture signed level L once. No current-party identity
check,null guard,or passed-menu/global-owner equality invariant is added.
All emit row0 always,row1 iff L>=0,row2 iff L>=1,row3 iff L>=2,in that order.
Negative levels append1;zero2;one3;>=2 four,with unchanged count input.
Level helper's upper-bound default0 and unchecked negative ID behavior remain
exactly as the complete Goombella companion documents.

## Full183-instruction common row graph

For Flurrie/Yoshi/Ms. Mowz/Bobbery/Vivian,EVERY admitted row uses the complete
Goombella ordered protocol with its own fixed weapon from
[source mapping](PARTY_WEAPON_CALLBACKS_WEAPON_SOURCE_MAP.md):

1. Reload WORD(C),derive Q=M+28*count,store source ordinal-1 at Q90.
2. Reload count/derive row,store itemId0 at Q94.
3. Reload count/derive row,store fixed weapon at Q80.
4. Reload count/derive row,store enabled0 at Q84.
5. Reload count/derive row,reload CURRENT row weapon,read U16(weapon+4),
   store icon halfword Q8c.
6. Reload count/derive row,reload CURRENT row weapon,read its WORD+0 key,
   request msgSearch without a key-null/fallback/result test.
7. AFTER lookup,reload count/derive row,store returned name at Q88.
8. Reload count,add1 with word wrapping,store it to C.

Reason Q98 and padding Q8e..8f remain untouched by EVERY callback. No count
initialization,capacity check,cursor lookup/publication,type/Input-state write,
eligibility/cost/target service occurs locally. Count/row addresses are reloaded,
not one snapshot;native physical aliases and message-service mutation can
change later destinations. No invented return count:rawr3 is the count word
loaded BEFORE its final increment,while actual C carries the incremented value.
Caller ignores that scalar and uses current C.

Full normalized instruction equivalence is scoped to these local bodies only.
It does not assert equivalent weapon behavior,party source events,rank ownership,
or runtime state. The verifier still pins all unnormalized original bytes and
source heads/keys,so source normalization cannot erase a wrong binding.

## Koops first two icons differ

Koops row0 (always) writes literal icon344;row1(L>=0) writes literal345.
Their icon step reloads current count/derives Q and stores the halfword directly;
it DOES NOT reload row weapon or read weapon+4. Integer constants are loaded
before the row stores and remain live to the icon store. Changing a row weapon
through count/field aliasing therefore does not change these icon requests.

Koops rows2/3(L>=1/L>=2) use the ordinary CURRENT weapon+4 icon read exactly
like the shared graph. All four Koops rows still reload current row weapon for
name key,call msgSearch,store name using fresh count,and increment current C.
No ItemData fallback,reason reset,or extra eligibility guard is introduced.
Its smaller body results from these two literal-icon sequences and register
allocation differences,not two fewer rows or a different level threshold.

## Caller integration and native requirements

[Generic type6 builder](WEAPON_TABLE_ITEM_SPECIAL_AND_PARTY_ROUTES.md)
obtains actor key1,invokes the registered address with M/count-word,then owns
positive count/index/scroll publication and late eligibility update. All seven
local callback contracts are now supplied. Native registration must disambiguate
original namespaces/addresses and preserve these source mappings;reusing one
parameterized implementation is valid only when its address-backed order and
Koops literal-icon branch are retained.

Unit definition→runtime table provisioning/current party translation/pouch rank,
full weapon/action/event scripts,query-event reachability,Game/platform/original
execution/full-game requirements remain open. This closes local callback
behavior,not a complete playable party system or global safe-cursor invariant.

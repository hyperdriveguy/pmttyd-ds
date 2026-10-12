# Weapon attack classifier and selected data/audience leaves

US G8ME01 revision 0; recovered 2026-10-12 by documentation agent ID 2.
Complete `_check_weapon_type_attack` 0x8011f29c/0x1ec,
`BtlUnit_GetData` 0x8012733c/0x40,
`btlataudGetAttackEventPtr` 0x8014606c/0x40,
`btlataudGetMoveEventPtr` 0x801460ac/0x44,
`BattleAudience_GetPresentItemType` 0x8019eb14/0x28 and
`BattleAudienceBaseGetPtr` 0x801a86a0/0xc. Verification is
`python3 tools/verify_battle_attack_selection.py`. Linked registry, cursor,
count and message bodies are identities; their contracts are documented
separately. Static source evidence does not measure original execution.
Read the full [selector](BATTLE_ATTACK_EVENT_SELECTION.md) for consumers.

## Boolean classification, with short-circuit reads

Input is raw weapon pointer W. Null returns word 0 without dereferencing.
Non-null reads word W+0x1c: any nonzero bit pattern returns 1 immediately.
It does not invoke the pointer at that field or assess returned power/damage.
Otherwise evaluate the following rows IN ORDER, using SIGNED byte loads. A
successful row returns 1 immediately; later fields are not read. Within a row,
read subsequent fields only if all preceding tests in that row passed.

| Order | Success predicate | First instruction |
| --- | --- | --- |
| 1 | S8(W80)>0 and S8(W81)>0 | 0x8011f2c0 |
| 2 | S8(W82)>0 and S8(W83)>0 | 0x8011f2e0 |
| 3 | S8(W84)>0 and S8(W85)>0 | 0x8011f300 |
| 4 | S8(W86)>0 and S8(W87)>0 | 0x8011f320 |
| 5 | S8(W89)>0 and S8(W8a)>0 | 0x8011f340 |
| 6 | S8(W8f)>0 and S8(W90)>0 | 0x8011f360 |
| 7 | S8(W91)>0 and S8(W92)>0 | 0x8011f380 |
| 8 | S8(W93)>0 and S8(W94)>0 and S8(W95)<0 | 0x8011f3a0 |
| 9 | S8(W96)>0 and S8(W97)>0 and S8(W98)<0 | 0x8011f3cc |
| 10 | S8(W99)>0 and S8(W9a)>0 and S8(W9b)<0 | 0x8011f3f8 |
| 11 | S8(W9e)>0 | 0x8011f424 |
| 12 | S8(Wa4)>0 | 0x8011f438 |
| 13 | S8(Wa5)>0 | 0x8011f44c |
| 14 | S8(Wa2)>0 and S8(Wa3)>0 | 0x8011f460 |

If all rows fail, return 0. Comparisons are strictly positive/negative; zero
fails either sign test. Bytes128..255 are negative. Thus replacing signed
positive tests with nonzero tests changes behavior. The last pair a2/a3 is
read AFTER a4/a5; address order is not evaluation order. No field88,8b..8e,
9c/9d/9f..a1 test occurs, and the sign triples require their third value to be
negative. The routine writes no weapon/unit/record storage, makes no calls,
reads no global owner and consumes no RNG. Names and status-effect interpretation
of the fields should come from separate source tables, not be invented from
this boolean predicate. It is not a universal “has positive damage” service.

The selector uses this predicate for modes2/4/6 only, after publishing the event.
Mode2 exempts one exact original weapon pointer BEFORE this call. The null
return is part of the classifier's contract, but the selector dereferences W+b0
before calling it; it does not make a null selected weapon safe. Event fallback
also leaves classification attached to W rather than the fallback event.

## Borrowed unit data table

`BtlUnit_GetData(U,keyWord)` first dereferences word U+0x18 into table pointer
P. There is no null U guard. Null P returns 0. Otherwise read word P+0 as K;
K==0 terminates and returns 0 BEFORE comparing with keyWord. Nonzero K equal
to keyWord returns word P+4 immediately, including value zero. Otherwise add8
to P and repeat. All pointer arithmetic follows original word addressing.

The first duplicate key wins. Query key0 never matches the sentinel. There is
no count, schema, bounds, cycle, ownership, allocation, free, callback or table
copy in this routine. A null table differs from a null unit. A found zero value
is indistinguishable from absent key in the returned scalar. Both selector
key2 and audience key6 consumers use this same helper; their fallback policies
differ. Table producer/layout/lifetime evidence is separate from this lookup.

## Audience attack event lookup

`btlataudGetAttackEventPtr(idWord)` retains its input as registry index, freshly
reads global battle B2 and calls BattleGetUnitPtr(B2,idWord). Pass that result
DIRECTLY to GetData(unit,6); there is no null check before its unit dereference.
If returned value is nonzero, return it unchanged. Zero returns EXACT pointer
0x80373d00. The symbol file names `btlataudevtDummy` at 0x80373cfc, size0x40;
the fallback is symbol+4, not the symbol base. Do not repair this interior
pointer or replace it with generic Wait based on its annotation. Its stream and
subsequent category execution require separate evidence.

The [registry contract](BATTLE_UNIT_REGISTRY_AND_GEOMETRY_INITIALIZATION.md)
gives the original bounds behavior. It can return null; this helper supplies
no recovery for that case. It only falls back after a valid unit's data lookup
returns zero. Retained selector B used for counters may differ from freshly
selected B2 used for this lookup; no consistency validation is performed.

## Audience move event choice and owner recapture

`btlataudGetMoveEventPtr` does not read its incoming argument. Call
BattleAudience_GetPresentItemType. If returned WORD equals exactly1, return
0x80373d3c (`btlataudevtPresentItem_Damage`); every other word, including zero,
negative and2, returns0x80373d74 (`btlataudevtPresentItem_Get`). No RNG,
probability, actor-kind, data lookup, pointer fallback or record update occurs.

GetPresentItemType calls BattleAudienceBaseGetPtr, then reads word at
returned pointer+0x137e8. BaseGetPtr freshly reads global B3 and returns
wrap32(B3+0x2858). Under stable ownership the final word is B3+0x16040.
Preserve both local calls and the freshly captured owner: the U0 passed by the
selector is not the owner/index of this read. There is no null/global guard
or output mutation in these two getters. The field's writers, reset, item-kind
schema, audience script bodies and actual event playback remain open.

## Implementation checks and evidence limits

Fixtures should distinguish null W from a null selected W; word1c early return
from callback invocation; positive, zero and negative signed bytes; all seven
pairs, three sign triples, three singleton fields and final pair; exact read
order with early exits and untested fields; null table, key0 sentinel, duplicates,
found-zero value and missing key; valid/invalid registry index and helper null
precondition; data6 fallback at80373d00 including its+4 offset; present-type
word1 versus all other values; and different captured/current battle owners.

The verifier pins complete local bodies and scoped direct calls/tables. It does
not execute original code, prove record/callback reachability, validate borrowed
table lifetime, solve full command/menu/audience/event state, or establish
original turn frequency. Those remain documentation requests rather than native
policy defaults.

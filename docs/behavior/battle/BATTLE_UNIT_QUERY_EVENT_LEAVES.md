# Battle unit-query event leaves

US G8ME01 revision0; static contracts, no execution or gameplay claim.
Closes six rows of [UNADDRESSED_BINARY_FUNCTIONS.md](../binary/UNADDRESSED_BINARY_FUNCTIONS.md):
`btlevtcmd_GetHp`, `btlevtcmd_GetMaxHp`, `btlevtcmd_GetUnitWork`,
`btlevtcmd_SetUnitWork`, `btlevtcmd_CheckStatus`,
`btlevtcmd_CheckDataOfDataTable`.

## Shared resolve pipeline

All six open with the pipeline documented in [event
dispatch](BATTLE_EVENT_DISPATCH.md): `evtGetValue(event,arg0)` →
`BattleTransID` (0x801253ec, complete in
[actor translation](BATTLE_ACTOR_SELECTOR_TRANSLATION.md)) →
`BattleGetUnitPtr(globalBattle,id)` (0x800f88cc) with the word at [r13+7280]
as the battle word — cited positionally, as before. Callee identities come
from `evidence.json` annotated branch sites. Every body returns 2 and
discards its callees' returns. `BattleGetUnitPtr` itself has no prose
contract; like the dispatch doc's services, these bodies assume its result
is usable (no null check anywhere below).

## Per-leaf contracts

| Leaf (entry/size) | Fingerprint sha12 | After unit lookup | Destination / write |
| --- | --- | --- | --- |
| GetHp 801101c4/120 | ee4e1ca9732b | If TransID returned -1 (`cmpwi 4,-1`, taken-equal fall-through), return 2 with no write. Else `lha` unit+268 | `evtSetValue(entry, raw [block+4], halfword)` |
| GetMaxHp 801100cc/120 | ee15406de00c | Identical -1 gate | `evtSetValue(entry, raw [block+4], halfword at unit+264)` |
| GetUnitWork 8010f828/132 | a28f8e3bc83e | No -1 gate. Index = `evtGetValue([block+4])`; dest = raw [block+8]; value = word at [unit+536+4*index] (`slwi`/`add`, no bounds check) | `evtSetValue(entry, dest, value)` |
| SetUnitWork 8010f7a4/132 | 2ed0ef8d229c | No -1 gate. Index = `evtGetValue([block+4])`, value = `evtGetValue([block+8])`; **direct** `stw value, [unit+536+4*index]` — no evtSetValue call, and r3 is preset to 2 before the store | Raw store, return 2 |
| CheckStatus 80105f0c/148 | acac4146c9d6 | Status byte = `extsb` (sign-extended low byte) of **raw** [block+4] — no evtGetValue on it. `BtlUnit_CheckStatus(unit, byte)` (0x80127814); result==0 → write 0 else 1 | `evtSetValue(entry, raw [block+8], 0/1)` |
| CheckDataOfDataTable 80109f88/136 | 767e8135bb94 | Query = `evtGetValue([block+8])`; `BtlUnit_CheckData(unit, query)` (0x801272fc, [documented](BATTLE_EVENT_DISPATCH.md) as unit+0x18-reader, size 0x40) | `evtSetValue(entry, raw [block+4], result)` |

Details pinned by the listings: GetHp/GetMaxHp differ only in the halfword
offset (268 vs 264) and entry address — instruction-for-instruction otherwise.
`BtlUnit_CheckStatus`'s own contract remains open (its caller in [hit
precheck](BATTLE_HIT_PRECHECK.md) lists it as a dependency). Destinations are
raw event words in all six; only the queried/index/value operands pass
through evtGetValue routing (immediates and encoded floats legal there).

## Asymmetries (observed, not rationalized)

- The -1 early-out exists only in GetHp/GetMaxHp. GetUnitWork, SetUnitWork,
  CheckStatus, and CheckDataOfDataTable feed the TransID result straight into
  `BattleGetUnitPtr` with no -1 test in their bodies.
- SetUnitWork is the only direct store; the other five route through
  evtSetValue destination bands (a literal-band destination stores nothing).
- CheckStatus signs-extends only the low byte of a raw word; nothing else in
  this file narrows an operand.

`us-usage.json` (namespace `battle_event_cmd.o` throughout): GetHp 7
data-word candidates/79 REL imports; GetMaxHp 5/55; GetUnitWork 64/1106;
SetUnitWork 136/1534; CheckStatus 37/426; CheckDataOfDataTable 45/9. Only
CheckStatus is fingerprinted.

## Not established

Unit lifetime, operand provenance, cadence, and what HP/max/unit-work/status
mean in-game. REL-import counts are not caller censuses.

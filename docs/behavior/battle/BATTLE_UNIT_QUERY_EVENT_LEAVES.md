# Battle unit-query event leaves

US G8ME01 revision0; static contracts, no execution or gameplay claim.
Closes six rows of [UNADDRESSED_BINARY_FUNCTIONS.md](../binary/UNADDRESSED_BINARY_FUNCTIONS.md):
`btlevtcmd_GetHp`, `btlevtcmd_GetMaxHp`, `btlevtcmd_GetUnitWork`,
`btlevtcmd_SetUnitWork`, `btlevtcmd_CheckStatus`,
`btlevtcmd_CheckDataOfDataTable`.

## Resolution and capture order

All six resolve actor operand0 through `evtGetValue` then
`BattleTransID` (0x801253ec; [actor translation](BATTLE_ACTOR_SELECTOR_TRANSLATION.md)).
Their battle/unit captures occur at different points; there is no shared early
unit lookup. The battle pointer is word[r13+7280] (`0x8041eb90`). Translation's
own battle read is distinct from each caller's read. Every body returns event
result2; the queried values and unit pointers are consumed, rather than discarded.
[Unit lookup](BATTLE_GET_UNIT_PTR.md) supplies the callee's separate null/index
policy. These wrappers do not test a returned null pointer on a lookup path.

Read the [HP/work capture refinement](BATTLE_UNIT_HP_WORK_CAPTURE_ORDER.md).
Reinspection of all six complete bodies on2026-10-13 confirms this ordering:

| Leaf | Ordered captures and service calls |
| --- | --- |
| GetHp / GetMaxHp | Retain event/argument pointer; capture battle BEFORE actor resolution/translation. ID-1 returns2 BEFORE unit lookup or output load. Otherwise lookup with retained battle, read signed HP, then load raw output A4 and call SetValue. |
| GetUnitWork | Resolve/translate actor, resolve index A4, capture raw output A8, THEN capture current battle/lookup unit, load indexed work, SetValue. |
| SetUnitWork | Resolve/translate actor, resolve index A4, resolve value A8, THEN capture current battle/lookup unit and store indexed work directly. |
| CheckStatus | Resolve/translate actor, load raw status A4, capture raw output A8, read current battle, sign-extend low status byte, lookup unit, CheckStatus, SetValue with0/1. |
| CheckDataOfDataTable | Resolve/translate actor, read current battle/lookup unit, capture raw output A4, resolve query A8, CheckData with retained unit, SetValue. |

All retain the event and initial argument pointer. None uses firstCall. Raw
output words stay unresolved. Get-work captures its output before lookup;
HP captures it after the signed halfword load. CheckData retains the unit
across query resolution, which may differ from a fresh global/unit lookup.

## Per-leaf contracts

| Leaf (entry/decimal byte size) | Fingerprint sha12 | Query / gate | Destination / write |
| --- | --- | --- | --- |
| GetHp 801101c4/120 | ee4e1ca9732b | If TransID returned -1, return2 BEFORE lookup with no write. Else `lha` unit+268 | `evtSetValue(entry, raw [block+4], halfword)` |
| GetMaxHp 801100cc/120 | ee15406de00c | Identical -1 gate | `evtSetValue(entry, raw [block+4], halfword at unit+264)` |
| GetUnitWork 8010f828/132 | a28f8e3bc83e | No -1 gate. Index = `evtGetValue([block+4])`; dest = raw [block+8]; value = word at [unit+536+4*index] (`slwi`/`add`, no bounds check) | `evtSetValue(entry, dest, value)` |
| SetUnitWork 8010f7a4/132 | 2ed0ef8d229c | No -1 gate. Index = `evtGetValue([block+4])`, value = `evtGetValue([block+8])`; **direct** `stw value, [unit+536+4*index]` — no evtSetValue call, and r3 is preset to 2 before the store | Raw store, return 2 |
| CheckStatus 80105f0c/148 | acac4146c9d6 | Status byte = `extsb` (sign-extended low byte) of **raw** [block+4] — no evtGetValue on it. `BtlUnit_CheckStatus(unit, byte)` (0x80127814); result==0 → write 0 else 1 | `evtSetValue(entry, raw [block+8], 0/1)` |
| CheckDataOfDataTable 80109f88/136 | 767e8135bb94 | Query = `evtGetValue([block+8])`; `BtlUnit_CheckData(unit, query)` (0x801272fc, [documented](BATTLE_EVENT_DISPATCH.md) as unit+0x18-reader, size 0x40) | `evtSetValue(entry, raw [block+4], result)` |

Details pinned by the listings: GetHp/GetMaxHp differ only in the halfword
offset (268 vs 264) and entry address — instruction-for-instruction otherwise.
`BtlUnit_CheckStatus` is now [documented](BATTLE_UNIT_CHECK_STATUS.md).
Destinations in the five output-producing leaves are raw event words; SetUnitWork
has no output token. Actor/index/value/CheckData query operands use evtGetValue;
CheckStatus's status selector remains raw. Work addressing is
unit+0x218+wrap32(index*4), with wrapping word addition and no index bound.
Negative indices may address preceding fields.

Reinspection anchors: CheckStatus raw status80105f3c/output80105f44/battle80105f48/
lookup80105f50; CheckData battle80109fbc/lookup80109fc0/output80109fc4/query
resolution80109fd4. HP/work anchors are in the linked refinement. Reproduce
all six with `tools/inspect_dol_function.py` and
`artifacts/reference/us_symbols.csv`; inspection ELFs stay in `/tmp/inbox-*.elf`.

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

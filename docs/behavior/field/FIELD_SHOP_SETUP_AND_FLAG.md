# Shop setup and flag leaves

US G8ME01 disc0 revision0; static contracts, no execution or gameplay claim.
Answers SERVICE-01 todo 1's open-shape half (`docs/requests/SERVICE-01.md`):
how a shop record is built and which flag word gates it. Buy/sell flows,
prices, and stock semantics beyond admission stay open.

Both bodies newly extracted read-only for this contract (`shop_flag_onoff`
120 bytes, `evt_shop_setup` 780 bytes in `us-symbols`; pairs placed under
ignored `artifacts/reference/behavior/`). Callee identities from
`us-symbols` rows: `evtGetValue`, `_mapAlloc`, `memset` (0x800050b4),
`strncpy`, `sprintf`, `mapObjGetPos`, `mapObjFlagOn`, `iconEntry`,
`iconSetPos`, `iconFlagOn`, `npcNameToPtr`, `evtEntryType`, `winMgrEntry`,
`evtRunCaseEntry`, `pouchCheckItem`.

## evt_shop_setup (0x800ef604): record build and stock admission

Event callback resolving 4 args (r28, r27, r30, r25). Order:

1. `_mapAlloc` 216 bytes + `memset` 0; publish the record at [r13+7232]
   (the same word `shop_flag_onoff` below flips — shop work pointer).
2. `strncpy` currentMapName (gp+300, 15 chars) into the record.
3. Stock loop over the arg2 table while r27 < 0 counting up (r29 += 8,
   r28 += 2 per row): map-object position/flag, `sprintf`, message lookup
   (`iconEntry`), icon position/flag, and per-row key-item admission — if
   the row's item id equals 51 or 55–59 (0x33, 0x37–0x3B: contact lens and
   the carta books per the item enum) AND `pouchCheckItem(id)` is nonzero,
   set bit 0 of the row's +20 halfword. Other ids keep the caller's bit.
4. `npcNameToPtr`, `evtEntryType`, three `winMgrEntry` calls, then
   `evtRunCaseEntry(9, 1, 0, [r13−26996], 0)` whose return value is the
   body's return (no `li 3,2` — not a return-2 event callback).

## shop_flag_onoff (0x800ef4d8): shop-work bit switch

Same shape as the battleflag onoff leaf: resolve two event words; if the
first != 1, OR the mask into halfword [[r13+7232]+0], else ANDC-clear it;
return 2. The target is the shop work record's first halfword, not a
battle flag. Which bit(s) the callers use is not established here.

## Not established

Buy/sell/price/stock-meaning flows, window/menu ownership, story gating,
the 0x800d4ed4-check caller's other uses, and any gameplay effect.

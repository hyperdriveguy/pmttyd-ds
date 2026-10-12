# Operation table:charge rows,Defend,Escape,and original keys

Completes the static US G8ME01 [operation producer](OPERATION_TABLE_ENTRY_PARTIAL_ROWS_AND_PUBLICATION.md).
Retain B/U/M and local optional-row count N. Q=M+28*N,descriptor starts Q2cc.
[Verifier](../../../tools/verify_operation_party_tables.py) pins all instructions,
original eight operation keys,four charge weapon heads,and four shared charge
name/help strings. Cost services are already recovered in
[cost/payment](BATTLE_WEAPON_COST_AND_PAYMENT.md);the producer preserves
request/order/current field ownership rather than inventing a new cost policy.

## Independent charge admissions and retained weapon

Read U8(U+0x2fc). Nonzero emits code1;zero skips it. Later,after all code1
services,read U8(U+0x2fd). Nonzero emits code2;zero skips. Each stage reads
fresh WORD(U+8):exactly222 chooses Mario weapon;all other values choose
other weapon without a party-kind revalidation.

| Code | Kind222 | Other kind | Icon |
| --- | --- | --- | --- |
| 1 | 803550a8 | 80355168 | 366 |
| 2 | 80355228 | 803552e8 | 367 |

Retain selected W and row Q across services. Store code;request
CheckWeaponCost(U,W),store RAW return as enabled. Request GetWeaponCost(U,W)
separately,store RAW return as cost. Then read CURRENT row enabled:nonzero
stores reason0,zero reason1. The second cost service can change fields;do not
choose reason solely from the first retained scalar or reuse one cost result.

Read CURRENT WORD(W+0) as key,request msgSearch,store name;store halfword
icon;read CURRENT WORD(W+0x0c),store it as help-key pointer. Then fresh
WORD(U+0x104)&0x10 denies,writing enabled0 then reason3. Preserve the cost
already written;denial does not erase it. Increment N after restriction.
No null key/lookup-result test or additional setup-definition restriction.

## Defend,code6,always admitted

At optional ordinal N,store code6,enabled1;request msgSearch(802effb0),
store name;store icon365,reason0,cost0,help-key802effc4,in that order.
Fresh WORD(U+0x104)&0x10 denies(enabled0/reason3). N itself is not incremented;
the next row uses N+1 and final publication uses N+2.
No CanActStatus request or setup flag check appears in Defend's own stage.

## Escape,code3,always admitted with fresh setup

At N+1,store code3 BEFORE requesting CanActStatus(retainedU). Store its raw
return as enabled;request msgSearch(802effe4),store name;store icon363,cost0.
Read current enabled:nonzero stores reason0,zero reason1. Store help-key802efff8.
Fresh WORD(U+0x104)&0x10 denies(enabled0/reason3).

Then reload CURRENT WORD(B+0x2738),then nested WORD(S+0x0c),then its WORD(D+4).
Bit0x10 denies(enabled0/reason3). This is a new setup chain,unlike Change
Party's entry-captured D. Execute it even after an earlier denial. No fresh
setup null guards. The helper's status result,actor104 restriction,and setup
restriction are distinct checks with their own reason/store order.

## Original operation key bindings

Retained key-base802efc28 uses signed addi-984,not802ffc28.
Names go through msgSearch;help fields store original KEY ADDRESSES directly:

| Operation | Name key/address | Help key/address |
| --- | --- | --- |
| Change Party | `btl_cmd_op_chg_party` /802eff40 | `btl_hlp_cmd_operation_changeparty` /802eff58 |
| Appeal | `btl_cmd_op_appeal` /802eff7c | `btl_hlp_cmd_operation_appeal` /802eff90 |
| Defend | `btl_cmd_op_defend` /802effb0 | `btl_hlp_cmd_operation_defend` /802effc4 |
| Escape | `btl_cmd_op_escape` /802effe4 | `btl_hlp_cmd_operation_escape` /802efff8 |

Code1 uses original `btl_wn_mario_normal_charge` and
`btl_hlp_cmd_operation_charge`;code2 uses `btl_wn_mario_super_charge` and
`btl_hlp_cmd_operation_super_charge`,for both actor variants. Weapon heads
pin these pointer bindings,not full action/effect/event definitions. Reuse original
msg/US text and icons;screen reflow must not invent menu/story dialogue.

Native checks should distinguish independent byte admissions,kind changes
between stages,raw cost enable versus current row reread,late denial preserving
cost,ignored Change Party CanAct result,always-counted mandatory rows,and
captured versus fresh setup. Menu/source/query/Game/runtime domains remain open.

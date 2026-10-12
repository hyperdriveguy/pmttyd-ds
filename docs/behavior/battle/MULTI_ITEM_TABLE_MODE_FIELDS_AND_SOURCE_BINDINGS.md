# Multi-item table:mode fields,admission,and original sources

Complete static US G8ME01 `_btlcmd_MakeMultiItemTable`801235e8/0x2d0.
[Main](MULTI_ITEM_MENU_MAIN_SELECTION_AND_NAVIGATION.md),[lifecycle](MULTI_ITEM_MENU_SETUP_END_AND_INPUT_BINDINGS.md),
and [cancel check](MULTI_ITEM_CANCEL_CHECK_CONTROL_AND_DISPLAY_REQUEST.md)
complete this native slice.[Verifier](../../../tools/verify_multi_item_menu.py)
pins seven complete bodies/477 instructions,14 linked identities,and18 source
spans including keys/ItemData/weapon heads/startup/display-sort constant.
Static evidence only;full UI/event/source/query reachability remains separate.

## Entry and24-byte descriptors

Retain passed B. Load WORD(B+0x420),request BattleGetUnitPtr(B,id),retain U.
Compute M=B+0x171c;wrapped M0 exits without writes,after the unit query.
Otherwise request GetCursorPtr(M,3,&privateSP08),ignore return. Row base is
M+0x450+24*N;fields:enabled WORD+0,name WORD+4,help-key WORD+8,icon U16+12,
mode BYTE+14,untouched padding+15,cost WORD+16,reason WORD+20.
Do not bulk-clear rows,padding,or unused entries. No actor-null guard.

## Plain item row always emitted

Store BYTE(M+0x45e)=1 THEN WORD(M+0x450)=1. Request
msgSearch(802eff0c),original `btl_cmd_act_item`,store name M454. Store icon340,
help-key word0,cost word0,in that order. Set local count N1. Reason WORD(M464)
is NOT written by this plain-row producer;its previous bytes survive. There
is no unit/status/pouch/cost check for this initial enabled row.

## Independent optional mode2 and mode3 rows

Read U8(U+0x2f3);nonzero emits mode2 at N1. After all its services,read
U8(U+0x2f4);nonzero emits mode3 at CURRENT N. These are fresh independent
reads,not one initial capability snapshot. Disabled admitted rows still count.
Each optional row stores mode byte first,then reads CURRENT WORD(U+8) to
choose exactly-kind222 presentation versus all other kinds.

| Mode | Cost weapon for BOTH kind branches | Kind222 ItemData | Other-kind presentation |
| --- | --- | --- | --- |
| 2 | 803553a8 | record254 at80313058 | record255 at80313080 |
| 3 | 80355468 | record329 at80313c10 | explicit keys802eff20/802eff30,icon101 |

For all record-backed branches:request CheckWeaponCost(U,W),store RAW
return as enabled. Read CURRENT WORD(ItemData+4),request msgSearch,store
name. Read CURRENT U16(ItemData+0x20),store icon;read CURRENT WORD(ItemData+8),
store help-key. Request GetWeaponCost(U,W) separately,zero-extend LOW8 return
and store it as a WORD cost. Full raw cost is not retained in this descriptor.
Finally read CURRENT enabled word:zero writes reason1,nonzero reason0.

Mode3 other-kind branch uses the SAME80355468 cost weapon:check/store enable,
lookup `in_3kai_item_p` at802eff20/store name,store icon101,store help-key
802eff30 (`msg_3kai_item_p`),get/truncate/store cost,then current-enabled reason.
There is no status/actor104 denial or new party weapon substitution here.
All requests/read/store ordering remains observable through address-backed
services;reason uses current field after presentation and second cost service.
Mode2 emission sets N2;mode3 increments CURRENT N once.

The verifier pins full40-byte ItemData254/255/329 and their six exact name/help
keys,plus two leading16-byte cost-weapon heads. This establishes original
bindings,not complete weapon/action/event definitions or runtime capabilities.
Reused msg/US text must retain IDs/control tokens;do not invent labels.

## Cursor publication and native requirements

Publish current local N(1..3 on ordinary local paths) to WORD(C+8),reload
SP08/current signed index/current signed count;if index>=count,store count-1.
Preserve negative indices and C+4. No explicit scalar count/success assignment;
rawr3 remains cursor address on ordinary exit,unit-query return on wrapped-M0.
Callers ignore it. No M550 mode selection or menu type/state write occurs here.

Implement exact partial rows,plain reason preservation,independent capability/
kind reads,raw enable versus low8 WORD cost,current enable reread,source
presentation choices,and signed cursor policy. [Cost protocol](BATTLE_WEAPON_COST_AND_PAYMENT.md)
is already recovered separately.Source provisioning,event/query domains,native
Game/platform/original runtime/full-game requirements remain open.

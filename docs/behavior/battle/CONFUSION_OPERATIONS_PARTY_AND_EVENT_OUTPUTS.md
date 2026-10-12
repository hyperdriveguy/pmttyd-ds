# Confusion selector:operations,party choices,and event-address outputs

Completes [entry/class](CONFUSION_CLASS_SELECTION_AND_MENU_TYPE_WRITE.md)
and [weapon](CONFUSION_WEAPON_SELECTION_AND_TARGET_POLICY.md) for complete
US G8ME01 BattleSetConfuseAct8011f62c/0x610. Static only;no event execution.
[Verifier](../../../tools/verify_confusion_action.py) pins388 instructions,
nine-way switch,and four charge-event pointer words. Original script symbols
identify returned addresses;their full graphs are separate contracts.

## Type5 operation scan

Request `_btlcmd_MakeOperationTable(B)`,then GetCursorPtr(M,5,&SP08).
Ignore returns. Capture signed cursor count and use the shared low-byte candidate
list policy. For each Q=M+28*Low8(J),read WORD(Q+0x2d0) enabled;zero skips
operation read. Nonzero reads WORD(Q+0x2cc);admit EXACTLY0,6,or3.
Other words are excluded. No eligible low-byte count returns flustered803532b0.
Otherwise call irand(candidateCount),store selected source ordinal to cursor.

Reload cursor/index and derive Q. Current enabled0 returns flustered. Otherwise
read current operation WORD(Q+0x2cc),store WORD(M+0x548),then reload this
word and dispatch it unsigned using table80363fa0. Negative/>8 defaults to
flustered. No filtering assertion replaces this fresh operation read;service
mutation during irand can select a different operation than initially admitted.

| Operation | Original case target | Returned script/request |
| --- | --- | --- |
| 0 | 8011fb2c | party selection below |
| 1 | 8011fac4 | current actor kind222:WORD(803550a8+b0);other:WORD(80355168+b0) |
| 2 | 8011faf0 | kind222:WORD(80355228+b0);other:WORD(803552e8+b0) |
| 3 | 8011fab8 | 8034e7a4,btldefaultevt_Escape |
| 4 | 8011fc08 | 803532b0,subsetevt_confuse_flustered |
| 5 | 8011faa0 | 8034e41c,btldefaultevt_Appeal |
| 6 | 8011fa80 | current kind222:80416b6c Defend;other:80416b88 DefendParty |
| 7 | 8011faac | 80416b40,btldefaultevt_Wait |
| 8 | 8011fb1c | request BtlUnit_GetData(retainedU,2),return its word |

Kind checks reread WORD(retainedU+8) at the case;they are not retained entry
kind. Operation8 does NOT invoke the returned unit-data address. Cases1/2
read their original weapon event pointer word rather than constructing scripts.
Pinned initial bindings:80355158→80416cb0 NormalCharge,
80355218→80416ccc NormalChargeP,803552d8→80416ce8 SuperCharge,
80355398→80416d04 SuperChargeP. Full source weapon bodies/lifetimes remain
separate;mutable event pointer words are read at selection time.

## Operation0 uses existing party table

Request GetCursorPtr(M,10,&SAME SP08),capture current signed count. There is
NO party-table builder request here. For Q=M+24*Low8(J),enabled
WORD(Q+0x394)!=0 admits a source ordinal using the same list/sentinel/RNG
policy. No eligible low-byte count returns NULL0. Otherwise irand once,store
selected ordinal to current cursor.

Reload cursor/index,derive Q,current enabled0 returns flustered. Otherwise
read WORD(Q+0x390),store it to WORD(M+0x54c) selected party ID. Then store
WORD(M+0x4a4)=0,resetting the PARTY CLASS cursor index,not the type10
party-selection cursor. Return8034de14,btldefaultevt_ChangeParty. No new unit
lookup,source-table creation,pouch switch,category accounting,event constructor,
or Input-state29 write occurs in this local case.

## Return taxonomy and type-domain evidence

The fixed fallback803532b0 is `subsetevt_confuse_flustered`,not Wait80416b40.
Returning it can follow entry rejection,no class candidates,disabled selected
row,unsupported published type,no operation candidates,or failed weapon/target
rechecks. Empty weapon/party candidate lists return0. Admitted weapon/unit-data
pointer results can also be0 without conversion to flustered. These branches
leave preceding byte/type/index/table writes intact.

Exactly one direct menu TYPE store occurs in the class-publication path.
Operation code and party ID stores at M548/M54c are different fields. This
compiled selector may publish type13 then return flustered;under unchanged
class production it excludes type2/4 from its random class candidates.
It does not call BattleGetSelectWeapon or its query-event wrappers directly.
Downstream returned scripts/source callbacks must still be checked before
asserting safe selected-weapon query domains. Do not translate local script/null
returns into VM completion2 or fabricate a fallback weapon.

## Next native documentation boundaries

Complete operation-table producer,party-table source and key1 delegated weapon
producer,confusion invocation/returned-event constructor domains,remaining menu
writers,and query-event source graphs remain open. [Attack-event selection](BATTLE_ATTACK_EVENT_SELECTION.md)
is a separate route with category/accounting effects absent from this selector.
Reuse original event streams/assets;do not combine the routes solely because
some returned script addresses coincide. Game/platform/original runtime and
full-game requirements remain unproven.

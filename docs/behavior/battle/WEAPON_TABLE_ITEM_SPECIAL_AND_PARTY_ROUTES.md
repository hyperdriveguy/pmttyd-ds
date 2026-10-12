# Weapon-table inventory, Special bits, and party delegate

Completes the [builder](WEAPON_TABLE_BUILDER_ENTRY_LAYOUT_AND_PUBLICATION.md)
after [Jump/Hammer/badge stages](WEAPON_TABLE_JUMP_HAMMER_AND_BADGE_ROWS.md).
Static US G8ME01 revision0;all instructions are in
[the verifier](../../../tools/verify_weapon_table_builder.py).
Types other than0,1,2,4,6 emit nothing locally;they still perform the entry
queries before returning0 with the cursor untouched on empty result.

## Type2:20 inventory slots,partial rows,and explicit substitute

Retain inventory address A=P+0x192 and ItemData base0x803108a8. For original
slot ordinal J=0..19,read SIGNED halfword itemId at A+2*J. Derive
I=wrap32(ItemDataBase+40*itemId). Exactly itemId0 skips the record flag read
and emission. Otherwise read U16(I+0x10);bit0x2 must be set to admit. There
is no signed-ID bounds check;negative IDs are not reclassified as empty.

Admitted:store source slot ordinal J at Q90,itemId at Q94;read WORD(I+0x24),
store weapon Q80,then enabled0/reason0. Reload Q80 to test the CURRENT row
weapon pointer:

- Zero:store explicit substitute weapon0x80365488 at Q80;read WORD(I+4),
  request msgSearch,store name Q88;then read U16(I+0x20),store icon Q8c.
- Nonzero:read U16(I+0x20),store icon first;then reload row weapon and resolve
  common weapon-key/fallback-ItemData name,store name Q88.

The two presentation orders differ:substitute path is name THEN icon;ordinary
inventory path is icon THEN name. Keep retained I across msgSearch,with fresh
reads afterward. The explicit substitute applies ONLY to this inventory route,
not all null weapon pointers or unsupported cursor outputs. This routine does
not synthesize or mutate an ItemData definition in place.

Increment CURRENT N;store it,then if signed N>=21 leave the inventory scan
immediately. Otherwise increment source ordinal and byte offset by1/2 and
continue while signed J<20. The literal21 limit is retained even though normal
local N starts0 and20 slots can emit at most20 rows. No inventory sorting,
merging duplicate IDs,count-based duplication,or item removal occurs here.

## Empty inventory fallback descriptor

After the scan,read CURRENT N. Any nonzero value skips the fallback,including
negative values (which later fail final positive publication). Exactly0 emits:

1. Store source ordinal -1 at Q90.
2. Store itemId0 at Q94.
3. Store source ordinal0 at Q90 AGAIN,overwriting the earlier -1.
4. Store weapon0x80363e60 at Q80,enabled0,reason0,icon362 halfword.
5. Resolve common weapon-name route,store name,increment CURRENT N.

The original key at this weapon's+0 is `btl_wn_dummy_noitem`. Do not treat
this real empty-inventory row as a generic default weapon for other domains,
or optimize away the two source-ordinal stores when preserving observable
write order. Name lookup failure does not cancel the descriptor.

## Type4:captured Special mask and live pointer reads

Read U16(retainedP+0x8c) ONCE as mask F. For bit ordinal J=0..15,in ascending
order,check F & (1<<J). Clear skips all table-pointer reads for that bit.
Set reads WORD(0x80355ca8+4*J);zero omits the row. Nonzero admits:

- source ordinal -1,itemId0;
- reread the table word and store weapon Q80;
- enabled0;
- reread the table word AGAIN,read U16(thatWeapon+4),store icon Q8c;
- reason0;
- resolve name using the CURRENT row weapon,store Q88;
- increment CURRENT N,store it; signed N>=21 exits this loop immediately.

Otherwise advance J/byte offset by1/4 and continue while signed J<16.
Mask F does not change if services mutate the pouch;table pointer reads remain
fresh and can differ from the admission read. There is no second null check
before the icon read,and no guarantee icon/name use the same weapon under
mutations. The source companion pins all16 original pointers/eight nonzero
initial entries. Do not replace this order with one copied table snapshot.

## Type6:unchecked party callback delegation

Request BtlUnit_GetData(retainedEntryU,1). Treat its returned WORD as a function
address and call it indirectly with `(M,&privateSP08Count)` at801247ec.
No null/callability guard,argument reinterpretation,status check,or row cap.
Ignore its scalar return. It can populate descriptors and modify N through the
passed word address;the ordinary final positive-count/index/scroll/update logic
then uses that CURRENT N. No local party row enumeration is supplied here.
Unit-data key1 callback producer/body/source/lifetime remains a separate request.
It must not be replaced with invented generic party weapons to make a native
adapter appear complete.

## Requirements and scope

Implement typed inventory/address arithmetic,flag2 admission,original slot
ordinals,explicit route-specific substitute and dummy rows,unequal name/icon
ordering,captured Special mask versus fresh pointer reads,and callable-address/
count-word party delegation. Reuse original source data and text keys.
Full delegate and dynamic inventory/source provisioning,menu writer/query-event
invariants,Game/platform integration,and original execution remain open.

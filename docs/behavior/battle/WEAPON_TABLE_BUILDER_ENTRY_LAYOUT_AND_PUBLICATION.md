# Weapon-table builder: entry, row layout, and publication

Complete static US G8ME01 revision0 `_btlcmd_MakeSelectWeaponTable`
80123ec0/0xd50. Companions cover [Jump/Hammer and badges](WEAPON_TABLE_JUMP_HAMMER_AND_BADGE_ROWS.md),
[inventory/Special/party](WEAPON_TABLE_ITEM_SPECIAL_AND_PARTY_ROUTES.md),
and [source bindings/icon leaves](WEAPON_TABLE_SOURCE_BINDINGS_AND_ICON_LEAVES.md).
Native request:[EVT-01](../../requests/EVT-01.md).
[Verifier](../../../tools/verify_weapon_table_builder.py) pins the full builder,
two complete icon leaves,seven linked services,all894 local instructions,and
scoped original arrays/ItemData/weapon-head/key spans. No original execution
or global query reachability is claimed.

## Entry and private output ownership

Retain passed B and T. Load WORD(B+0x420),request BattleGetUnitPtr(B,actor),
retain U;compute M=wrap32(B+0x171c);request pouchGetPtr(),retain P.
Only afterward test M==0. Zero returns0 without table/cursor writes,even though
unit/pouch queries have already happened. There is no prior U/P guard.

For nonzero M,request `_btlcmd_GetCursorPtr(M,T,&privateSP0c)`,ignore return.
Then store local count N=0 at SP08. These are different private words.
Unsupported T leaves SP0c untouched;this function does not initialize it to
Input's class cursor or another function's output. The helper's current-global
actor lookup is separate from retained U/passed B.

Admission phases run in order:

1. T0 emits level-admitted ordinary Jump rows;T1 emits Hammer rows.
2. T0 scans eight Jump badge entries;T1 scans nine Hammer badge entries.
3. T2 scans inventory, T4 scans Special bits, T6 delegates to party callback.
   Other types emit nothing locally.
4. Publish only when current signed N>0,then update row eligibility.

N is a stack word repeatedly reloaded to derive row addresses. T6 deliberately
passes its address to a callback;do not replace it with an invisible host local
which delegated code cannot update. Direct local row paths increment N after
presentation/name requests. This body does not store menu TYPE M540.

## Seven-field 28-byte descriptors

For current N,derive Q=wrap32(M+28*N). Descriptor starts at Q+0x80:

| Menu-relative address | Descriptor offset | Width | Producer meaning |
| --- | --- | --- | --- |
| Q+80 | 0 | word | weapon address |
| Q+84 | 4 | word | enabled,initially0 |
| Q+88 | 8 | word | msgSearch result/name address |
| Q+8c | 12 | halfword | icon |
| Q+8e | 14 | two bytes | untouched padding |
| Q+90 | 16 | word | source ordinal or route-specific value |
| Q+94 | 20 | word | item/badge ID or0 |
| Q+98 | 24 | word | reason,initially0 |

Do not bulk-clear descriptors,padding,unused rows,or old cursors. Each path's
store/request order is in the companions. Name lookup reloads the row weapon:
if WORD(weapon+0)!=0,request msgSearch(thatKey);otherwise read
WORD(weapon+8) as itemId,derive wrap32(0x803108a8+40*itemId),read its+4
name-key word,request msgSearch. Store the result unchanged at Q88.
There is no weapon null guard on this common name route, nor lookup-success
check or invented fallback English label. Type2's null-pointer path has its
own explicit substitute weapon and ItemData lookup.

## Empty result preserves existing cursor count

At80124b84,read CURRENT local N and compare it signed against0. N<=0 returns0
immediately:do NOT write cursor count,index,scroll,or call eligibility updater.
Rows already emitted or delegated changes are not rolled back. Unsupported
T generally follows this path with initialized N0,despite its earlier cursor
query. Do not describe this as resetting the selected cursor to an empty menu.

For N>0:load SP0c cursor C and store WORD(C+8)=N. Reload SP0c,current
signed index WORD(C),then signed count WORD(C+8). If index>=count,store
index=count-1;otherwise preserve index,including negative values.

Then repeatedly reload cursor,scroll WORD(C+4),and count WORD(C+8).
If signed count<wrap32(scroll+6) AND signed scroll>0,decrement scroll by1,
store it,and test again. Otherwise stop. This only decreases positive scroll;
it neither centers selection nor repairs negative scroll. Signed wrapping of
scroll+6 is part of the original comparison. No render-frame measurement is
implied by the literal six-row calculation.

Request `_btlcmd_UpdateSelectWeaponTable(B,retainedT)` AFTER publication/
correction;ignore its return. Reload CURRENT local N from SP08 and return it.
[Complete eligibility updater](WEAPON_MENU_TARGET_COUNT_AND_ELIGIBILITY_UPDATE.md)
has its own actor query/private cursor slot/captured count and overwrites
visited enable/reason fields. Its selected cursor can differ from the builder's
under changing current-global ownership;do not assert shared owner equality
without source proof. Name/icon/source fields survive the updater's own stores.

## Port requirements and remaining proof

Implement retained B/T/U/P,private cursor/count words,all route order,
partial descriptors,original msg/US keys/icons,empty-result preservation,
signed index/scroll policy,late updater,and count return. Keep original
weapon/ItemData addresses or an explicit native address translation;reusing
asset data does not establish runtime pointer or callback invariants.
Full party callback/source provisioning,remaining menu writers,query-event
reachability,Game/platform integration,and original execution remain open.

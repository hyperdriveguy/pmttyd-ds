# Harbor battle index2: original configuration and unit bindings

US G8ME01 revision0,recovered2026-10-12. Evidence B:original gor.rel fields,
relocation records and [NPC setup consumers](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).
No executed tutorial behavior or complete unit stat interpretation is claimed.
All addresses below are REL file offsets unless marked DOL.

## Registration and selected record

The gor module prolog at0x5e4,size0x108 has an original call relocation at0x6d4
to relSetBtlAddr0x80038fd8. Immediately before it,lis/addi pairs build arguments
r3/r4/r5 with ADDR16_HA/LO relocations:area-name string section4+0x260,setup table
section5+0x40da8 and number table section5+0x40f84. Those tables are file offsets
0x60e30 and0x6100c. This installs the tables in the shared area record's+8/+0xc;
it is not a guessed runtime address from the symbol CSV.

In the harbor scene,evt_npc_set_battle_info supplies literal2 for actor-key
0x14b40. The current map's three-byte area prefix is gor. NPC numeric selection
uses stride0x44,so record2 is0x60e30+2*0x44=**0x60eb8**. The setup-number table
also binds original identifier BTLNO_GOR_00_01 to literal2,corroborating identity;
numeric NPC setup does not need that name-table lookup.

| Record2 field | Original data/binding |
| --- | --- |
| +0 | Local label string0x1dbfc (GOR00 annotation) |
| +4 | gor_00 string0x1dc10 |
| +8 | Null |
| +0xc | -1;outside encoded-switch range |
| +0x10 | Null on-list |
| +0x14 | Weighted off-list0x60dd0 |
| +0x18 | Raw word0x20000010;not installed by this setup helper |
| +0x1c | 0;audience initialization enabled |
| +0x40 | BGM_CHUBOSS_BATTLE1 string0x1dc18 |

The weighted list has one positive weight50,group pointer0x60c90,stage pointer
0x608f8 (gor_btlstage_data),then a zero-weight terminator. Therefore valid setup
always selects that group/stage,while still consuming an irand(50) call.

## One-unit group and kind/event pointers

Group0x60c90 has count1 and unit-array pointer0x60c60. Other original fields
include word+0x10=100,two DOL default heart/flower-drop bindings at+0x14/+0x18,
and word+0x1c=5. Their full battle/reward semantics require their consumers;
these are not substitutes for enemy HP,damage or XP formulas.

The unit setup is one0x30-byte record. Its kind relocation at+0 selects
**unit_boss_kanbu1**,file0x645c8,size0xc4. Its+0x2c item-table reference is DOL
0x80417920,battle_item_kanbu1,whose eight original bytes are all zero. The NPC
setup's item lottery therefore has total0 and makes no item draw for this unit.

The kind record points to model identifier btl_un_kanbu1 at0x1e1f8 and has:

| Kind field | Local target / annotation |
| --- | --- |
| +0xb0 | 0x6469c regist |
| +0xb8 | 0x646cc parts |
| +0xbc | 0x64808 init_event |
| +0xc0 | 0x646b4 data_table |

Raw first halfword at kind+8 is5. The [entry/stat-copy contract](../unit/UNIT_ENTRY_STATS_AND_SPAWN.md)
now confirms HP5 initialization into runtime current/max fields and the tutorial
getter/DIV threshold2. Later initialization/field overrides and original runtime
observations still need closure. Keep the prototype's tutorial behavior
provisional until those consumers are connected. Model/type labels alone do not
prove attack availability,invulnerability,damage,action timing or defeat policy.

## Known setup RNG consumption and closure gaps

Record type0 requests16 audience draws,one per original byte pair,including
fixed bounds. The single-group lottery adds one draw;the zero item table adds
none. This gives **17 irand calls in this local setup chain** for the verified
record/group/unit,assuming the recovered valid inputs. It is not full encounter
RNG consumption or seed/cadence observation. The separate byte copied to NPC
+0x316 is kind+0x12,which is0 here;meaning of that byte's later consumers is open.

Independent checks pin complete prolog,record,weighted-list,group,unit-setup,
kind and number-table spans,plus setup→group→kind relocations and zero DOL item
table. The containing REL hash remains the identified original US binary.
Original data/code/reports stay ignored;no unit script or dialogue is rewritten.

The [boss tutorial event contracts](HARBOR_BOSS_TUTORIAL_EVENTS.md) now recover
init/phase and data hooks. Next:part/unit lifecycle and stat-copy consumers;
resolve actual tutorial event/command gates,stats/attack/reward/forced-result
writers and original win/loss/reload timing. Close actor/field sequence handoff
and message conditions before implementing a faithful opening encounter. Full
US combat/story/game documentation and requirements remain unfinished.

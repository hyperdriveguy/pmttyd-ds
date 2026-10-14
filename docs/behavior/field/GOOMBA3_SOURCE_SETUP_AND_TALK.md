# Plaza Goomba3 source setup and talk

US G8ME01 disc0 revision0, read-only tagged map and REL recovery2026-10-13.
Complements [quest writers](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md)
and [ordinary NPC bindings](ROGUEPORT_ORDINARY_NPC_BINDINGS.md).

Original `m/gor_01/s` names **クリボー３** (full-width3), tribe NPCクリボー,
position(-400,0,-325),direction90,aitypeなし. Native catalog's existing FNV
room/name algorithm yields0x42ff5f1f; this hash is a native identity adapter,
not an original address. Retain tagged registry order for candidacy.

Original92-byte NpcSetupInfo at gor.rel file0x35174 has flags0x80600,
secondflags0, init0x32f64,regular0x32f6c,talk0x32f7c. Room init0x3c4c4 calls
setup on npcEnt0x34cc8. Init is RETURN/END, with **no**40000000 flag or
position override. Fresh npcEntry mainflags3 plus setup therefore gives80603.
Shared NPCクリボー tribe row8033335c binds c_kuribo_n, texture companion
c_kuribo_n-, idleS_1,talkT_1,walkW_1,runR_1,width/height30,drawscalar2.

Regular RUN_CHILDs gor_villager_urouro at0x200e8. That child reads currentXYZ
into LW0..2, calls urouro_init_func(me,LW0,LW2,0xf24c0a80,0xf24af280,0),
then urouro_main_func(me),RETURN/END. Encoded float operands decode radius100,
speed30; home is actual current XZ at initialization. Existing
[wander contract](FIELD_NPC_WANDER_DRIVER_AND_GOOMBA4.md) owns the shared driver.
Initialization OR00400000 gives00480603. Goomba3 lacks Goomba4's40000000
horizontal direct-add bypass: source NPC main invokes map-side collision and
npcMoveG. A native direct-horizontal-only wander adapter cannot declare this
NPC's complete original movement supported merely by reusing its art/driver.

Talk0x32f7c/240 first checks GSW1420==2. Acquisition/reward graph lives in
[quest writers](TROUBLE_CENTER_QUEST2_13_WRITERS_AND_ACQUISITION.md).
Its finite waiting arm is quest2,GSF5406==0,GSF5405!=0:
print(0,irai_02_01,0,me),RETURN. No inventory/coin/quest/switch write occurs.
Unknown quest/switch values do not prove this branch.

Only quest!=2 reaches GSW0 switch. First matching CASE_LESS thresholds
56,112,164,215,261,339,373,401 select gor_01_028_00..07; default selects08.
Every branch prints(0,ID,0,me). Story0..55 therefore selects00. Preserve
original US message bytes/control tokens and actual NPC attribution. Native
coverage0..55 is a bounded adapter envelope, not recovered lifetime visibility.

Evidence: gor.rel SHA256d4965f21254ecf1a78e5d66ea12a0b000eed3f123032af6ab3cb0f703d3f185a;
init span1e9fcd4ca7e6723c4a822c370faad68aa89031573b2376d87e4cc3c6626ede61;
regular4f73273d492946834fdcd9357c6139ed15d1f187730fb4a458599665c6b5a9c9;
talk2cf28e3589502e772dfb9a58b99ae310b0de556e702aecfa3a25ceaa1288b9e6.
Fresh tools/rel_event_inventory.py reports retained locally under ignored
artifacts/reference/goomba3-services. This documentation makes no emulator,
hardware, later-scene lifetime closure or native implementation claim.

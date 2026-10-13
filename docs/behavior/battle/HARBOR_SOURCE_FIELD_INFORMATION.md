# Harbor original field information constructor

Read-only US source: npcInit8004bb70(cc), npcSetupBattleInfo80043e78(280),
BattleInformationInit80114518(70), seq_battle battle_init800738ac(278),
BattleInit800f8f04(138), BattleInfomationSetBattleSetupInfo80114510(8).
The six bodies are pinned by verify_harbor_field_information.py. No original
code/data bytes are committed. These are binary/portable tests, not observations.

## Actual NPC and field information

Fresh npcInit allocates heap0 size580 for the Fbat work, publishes it at gp168,
then zeroes it. Original scripted evt_npc_battle_start stores NPC pointerF8,
clears result atF+20+10 and sets outer field sequence4. It does not author a first
attack. For this fresh scripted route F10/F14/F1c retain their cleared values.

The separate portable NPC setup service executes actual npcSetupBattleInfo. It
clears200 bytes atNPC230 and32 atinformation7c; null/missing configurations return
there without altering external NPC flags/type. Configuration1c is copied to
informationac. Source audience modes1/2 skip initialization; other modes make
sixteen signed-width/unsigned-add audience byte draws. It preserves the discarded
configuration18 read; information4/c/10/14 are cleared, configuration8 copied8,
and configuration40 copiedc0. Literal configuration-1 bypasses the encoded
saved-flag range. Weighted group/stage selection and item selection retain signed
weights, wrapping arithmetic, source terminators and exact random consumption.
Finally typeNPC316 is copied from the first chosen kind12 and NPC0 gets10000000.
It does not infer field-NPC model/control state from that bit.

Actual setup2 configuration is file60eb8, native95540e30. It is derived from the
already verified group address95540c08 plus the source file delta60eb8-60c90 in
the same relocated data section. The original pack test executes these records:
16 audience requests plus one irand50, group95540c08, stage95540870 and one unit.
The original80417920 item table has total0: no item draw or held item is selected.
Configuration20000010 is read/discarded; information4 stays0.

Original battle_init constructs I=F+20 with source init order: Mode0, Party4,
FirstAttack8, Result10, NPCInfoc, word14 and rule bytes18..1b are set0. It sets
Mode0 again, asks marioGetPartyId then partyGetPtr. Nonnull partner reads signed
party31, translates actual party kind, then checks pouch partner halfwordid*14
bit2 before setting I4. This generic provider branch is retained.

FirstAttack is zero if F1c&1. Otherwise F10 values20000/40000/80000/100000/200000/
400000/800000/1000000 map1..8;10000000 uses explicitF14; all other values map0.
Then Ic getsNPC230; NPC2f4→I18,2f5→I1a,2f6→I1b. I19 retains the init0. Actual
setup2 NPC clear includes2f4..6 and leaves all three0. BattleInit stores I at its
2738 field. Thus this fresh source entry has Mode0, Party0, FirstAttack0,
rule/parameters0, original NPCInfo/group/stage pointers and result0.

## Native owner and limits

Explicit beginFresh now owns Fbat84500000(size580), NPC information carrier
84501000(size340), I84500020 and existing pouch84400000. It publishes gp168,
F8, and Battle2738, runs the source setup/constructor, verifies selected original
group/stage, and requests the exact17 setup draws from the same native RNG owner
used by the VM. Seed0 remains an explicit native seed adapter; full field RNG
seed/consumption parity is unclosed. It does not claim these are original memory
addresses or a fully implemented field-NPC object.

The fresh opening provider has no active party; neutral spawned NPC Goombella
remains actual kind221/belong2/maxMoves0, independent of party information. The
source information fields are available to scheduler services; this does not
replace the current explicit intro phase adapter with a completed source
sequence controller. Startup resource/entry/fade barriers, original FirstAct→Turn
and player command/attack ownership remain next. No damage/result/field return is
fabricated from this constructor.

Tests cover null setup's external-field preservation, signed audience wrap,
negative item weights and both terminator conditions, party query order/pouch
eligibility, first-attack mapping/suppression, copied rule bytes, and the actual
setup2 records. See [field entry](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md),
[fresh pouch](BATTLE_FRESH_PACKED_POUCH.md) and
[Mario init](../story/NATIVE_HARBOR_MARIO_INIT.md).

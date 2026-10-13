# DMG-01 unit initialization and ordinary Jump count ownership

US G8ME01 disc0 revision0, static source evidence consolidated2026-10-13.
Answers the delivered allocation/stat/init portions of
[DMG-01](../../requests/DMG-01.md), priority1 in the documentation inbox.
No original starting-loadout, impact timing or complete damage trace is claimed.

## Allocation, publication and resource initialization

The [complete unit constructor](BATTLE_UNIT_ENTRY_COMPLETE.md) closes
BtlUnit_Entry80128fe0/6b4. Capture BattleWork once; scan64 registry slots;
return null for no slot or failed unit allocation. Clear unit0xb34 and publish
it BEFORE identity, kind, resources, parts, pouch and init-event setup. Part
allocation later is unchecked, unlike the unit allocation. A port must preserve
this distinction and early visibility of a partially initialized unit.

Kind HP/FP are signed halfwords. Each is copied into three independent unit
halfwords, with source reloads and ordered stores: HP10a→108, separately10c;
FP110→10e, separately112. Those stores have no nonnegative clamp. The
[stat/spawn slice](../unit/UNIT_ENTRY_STATS_AND_SPAWN.md) connects the original
Crump kind to HP5/FP0 at these stores; subsequent source overrides remain separate.
GetHp/GetMaxHp use the corrected
[retained battle capture order](BATTLE_UNIT_QUERY_EVENT_LEAVES.md).

The constructor also establishes raw belong, height, scalar114, vector scales,
kind-pointer copies, status clearing and initial work words. Body identity has
two distinct paths: U2d4 retains the LAST flagged part assigned during creation,
while GetBodyPartsId finds the FIRST flagged part for the initial pose requests.
Do not collapse these into one cache without a proven single-body data invariant.
See [part construction/lifetime](../unit/UNIT_PART_DEFENSE_AND_LIFETIME.md).

The [complete actor setup](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md) and
[opponent loop/equipment](BATTLE_UNIT_SETUP_OPPONENT_LOOP_AND_EQUIPMENT_MASK.md)
close Btl_UnitSetup800f8aac/2f8 and its concrete caller inputs.
The [badge import](../unit/UNIT_BADGE_COUNT_IMPORT.md) supplies the reset/unsigned
count maps and item289/290 bonus2ff/300 writers. Later
[pouch import](../unit/UNIT_STATUS_CLEAR_AND_POUCH_IMPORT.md) can replace resource
and partner snapshot values after kind initialization. Consequently kind HP/FP
copies alone do not prove final Mario/partner resources or bonuses at a hit.

Init-script creation requests root priority10/flags0, then writes unit ID to
event160 and stores event ID in U288. It does not synchronously execute init.
Source scheduling, equipment selection, later stat mutations and original
runtime readiness remain requirements.

## Count reset, success increment and read-only query

Here B denotes BattleWork. Its shared AC count WORD+B1cb4 and result
WORD+B1cb8 are distinct from unit badge/move counts and Jump's local L6 motion
duration. Independent complete-body reinspection confirms:

| Body / instruction | Operation and ordering |
| --- | --- |
| BattleActionCommandSetup800fa45c/ac; stores800fa47c/480 | Store count0, then result1 (pending), before callback-table search. Reset occurs even when the requested command ID is not found. |
| battleAcResult_TimingA8012a8c4/1a0; store8012a92c | Option-bit1 plus actor27c mask10 token route: negative irand(100) return writes result2 and count+1; nonnegative return writes result0 without count mutation. This branch is retained as encoded; no RNG-domain reachability is inferred. |
| Same; store8012a958 | Actor unsigned byte307 nonzero: write result2 then increment count. This follows the option/token route. |
| Same; store8012a990 | Battle flag01000000: classification3, result2, count+1, before ordinary input checking. |
| Same; store8012aa48 | Ordinary success: classification3, result2, count+1 after mash/window tests. Other ordinary failure paths do not increment. |
| btlevtcmd_GetResultCountAC80107548/34; load80107560 | Read current battle pointer before raw output token; load count, SetValue, return event2. No count write, actor translation or firstCall gate. |

Every increment is a wrapping32-bit load/add/store, not saturation and not an
increment tied to each rendered frame. The [TimingA result contract](BATTLE_TIMING_A_COMMAND.md)
defines branch precedence, history and window reads. Repeated successful
evaluations against unchanged history can increment again; the result evaluator
does not consume the press. Native code must not silently deduplicate success
or reset the count at each hit.

The [ordinary Jump graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md) has one setup/start
before its per-hit loop, and no new setup/start on repetition. GetResultCountAC
at80356790 feeds the signed <=1 repeat branch. A qualifying success at count1
uses CheckDamage flags20000; later success uses20100; failure uses100. Damage
L5 is not tested for repetition, and there is no explicit HP/alive query there;
the repeated precheck can still divert execution. These are source request
paths, not proof that both hits actually deal damage.

The [ResultAC wrapper](BATTLE_ACTION_COMMAND_CALLBACKS.md) discards the result
service's register return and checks stored pending bit0. Count readout and
callback return are therefore different protocol outputs. Stop and cleanup use
the [separate stop contract](BATTLE_JUMP_RESULT_AND_STOP_SERVICES.md); no fresh
per-hit count reset may be inferred from the harness's attack lifecycle.

## Validation and remaining priority1 work

`python3 tools/verify_battle_unit_entry_complete.py`:33 bodies,441 anchors,
11 data spans. `python3 tools/verify_battle_unit_setup.py`:8 bodies,190 anchors,
5 data spans. `python3 tools/verify_jump_event_flow.py`:268 commands,19 bodies,
57 anchors and5 concurrent blocks. `python3 tools/verify_behavior_contracts.py`
passes, including original power, TimingA, stat/spawn, part/status and badge
bindings. All were rerun against the read-only ISO.

Setup, TimingA result and count getter listings were independently regenerated
with `tools/inspect_dol_function.py` to `/tmp/inbox-acsetup.elf`,
`/tmp/inbox-timing-result.elf` and `/tmp/inbox-getcount.elf`. No extracted code,
asset, original dialogue or compiled pack is added to history.

Remaining: other attack/part counters and all mutation owners, complete
guard/status/counter/precheck/application/reward/defeat composition, global1000
tutorial policy, actual level/equipment/bonus producers, and original multihit
input/RNG/resource traces. The constructor and shared Jump count slice are
answered; DMG-01 and full-game fidelity remain OPEN.

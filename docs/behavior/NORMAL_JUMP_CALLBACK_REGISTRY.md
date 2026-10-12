# Direct original callback registry for the three packed Jump streams

US G8ME01 revision0; recovered2026-10-12 by documentation agent ID2.
[Registration](NORMAL_JUMP_ROOT_REGISTRATION.md) supplies conditional root/actor
binding. Machine-readable [census](../../tools/jump_callback_registry.json) retains
EVERY direct USER_FUNC site's original command address and containing stream,
callback code address/name/size/body fingerprint,and registration evidence.
It contains metadata only,not original executable/event bodies or dialogue.
Verify:`python3 tools/verify_jump_source_registry.py` against the read-only ISO
and original US symbol index. No native callback registration/execution is claimed.

## Closed census boundary and counts

The three streams already selected by tools/pack_event_scripts.py are:

| Stream | Address / size | Commands | USER_FUNC sites | Distinct addresses |
| --- | --- | --- | --- | --- |
| marioAttackEvent_NormalJump | 80355cf0 /f0c | 268 | 139 | 60 |
| marioAttackEvent_MajinaiPowerUpCheck | 80363388 /8c | 14 | 3 | 3 |
| btldefaultevt_SuitoruBadgeEffect | 8034f2e4 /1e8 | 32 | 14 | 9 |

Together these contain156 direct sites and72 unique callback addresses. This
counts static opcode91 records including inline/brother branches,not number
of runtime invocations or branch feasibility. Original callback operand words
are direct code addresses at these sites. The symbol names annotate identity;
full callee effects/return values require their own contracts and backend binding.
The verifier hashes each callback body as identity evidence without executing it.

The complete table lists sites per stream as Jump / power-check / badge-tail.
Exact individual caller addresses are in the linked machine-readable census.
Registry entries must key by ORIGINAL address,not just a convenient new enum,
name substring or host pointer. Polling and first-call arguments use the
[USER_FUNC protocol](EVENT_DISPATCH_AND_WAIT_CONTINUATIONS.md).

| Code address | Original symbol | Static sites J / P / B |
| --- | --- | --- |
| 8006d970 | evt_eff | 0 / 0 / 2 |
| 8010436c | btlevtcmd_InviteApInfoReport | 2 / 0 / 0 |
| 80104508 | btlevtcmd_snd_voice | 3 / 0 / 0 |
| 80104700 | btlevtcmd_snd_se | 3 / 0 / 0 |
| 801054e8 | btlevtcmd_WeaponAftereffect | 2 / 0 / 0 |
| 801057e8 | btlevtcmd_CommandGetWeaponAddress | 1 / 0 / 0 |
| 801058e0 | btlevtcmd_GetWeaponActionLv | 2 / 0 / 0 |
| 801059a0 | btlevtcmd_PayWeaponCost | 1 / 0 / 0 |
| 80106b20 | btlevtcmd_AnimeChangePoseType | 3 / 0 / 0 |
| 80106bb8 | btlevtcmd_AnimeChangePose | 9 / 0 / 0 |
| 80106d7c | btlevtcmd_ACRGetResult | 2 / 0 / 0 |
| 80106dfc | btlevtcmd_ACRStart | 2 / 0 / 0 |
| 80106eb4 | btlevtcmd_ACHelpSet | 1 / 0 / 0 |
| 80106ef0 | btlevtcmd_ac_timing_get_success_frame | 1 / 0 / 0 |
| 80106f40 | btlevtcmd_ac_timing_flag_onoff | 2 / 0 / 0 |
| 80106fa0 | btlevtcmd_ACSuccessEffect | 1 / 0 / 0 |
| 801072e4 | btlevtcmd_AudienceDeclareAcrobatResult | 2 / 0 / 0 |
| 8010749c | btlevtcmd_AudienceDeclareACResult | 2 / 0 / 0 |
| 80107548 | btlevtcmd_GetResultCountAC | 1 / 0 / 0 |
| 801075c4 | btlevtcmd_GetResultPrizeLv | 1 / 0 / 0 |
| 80107660 | btlevtcmd_GetResultAC | 1 / 0 / 0 |
| 80107708 | btlevtcmd_ResultAC | 1 / 0 / 0 |
| 80107758 | btlevtcmd_StopAC | 3 / 0 / 0 |
| 80107788 | btlevtcmd_StartAC | 1 / 0 / 0 |
| 801077ec | btlevtcmd_SetupAC | 2 / 0 / 0 |
| 80107b38 | btlevtcmd_AcSetDifficulty | 2 / 0 / 0 |
| 80109054 | btlevtcmd_GetTakeoffPosition | 2 / 0 / 0 |
| 801093a8 | btlevtcmd_MarioJumpPosition | 2 / 0 / 0 |
| 80109c68 | btlevtcmd_MarioJumpParam | 2 / 0 / 0 |
| 8010a010 | btlevtcmd_RunDataEventChild | 1 / 0 / 0 |
| 8010a1c8 | btlevtcmd_WaitGuardMove | 1 / 0 / 0 |
| 8010a26c | btlevtcmd_StartWaitEvent | 1 / 0 / 0 |
| 8010a78c | btlevtcmd_CheckDamage | 3 / 0 / 0 |
| 8010abac | btlevtcmd_PreCheckDamage | 2 / 0 / 0 |
| 8010ad00 | btlevtcmd_AttackDeclare | 1 / 0 / 0 |
| 8010b418 | btlevtcmd_GetFirstAttackTarget | 1 / 0 / 0 |
| 8010b540 | btlevtcmd_GetSelectEnemy | 1 / 0 / 0 |
| 8010b7c4 | btlevtcmd_FaceDirectionSub | 7 / 0 / 0 |
| 8010c8c0 | btlevtcmd_GetMoveFrame | 1 / 0 / 0 |
| 8010ca74 | btlevtcmd_MovePosition | 3 / 0 / 0 |
| 8010d0f0 | btlevtcmd_JumpContinue | 1 / 0 / 0 |
| 8010d208 | btlevtcmd_JumpPosition | 6 / 0 / 0 |
| 8010d6cc | btlevtcmd_JumpSetting | 1 / 0 / 0 |
| 8010d770 | btlevtcmd_SetFallAccel | 4 / 0 / 0 |
| 8010d860 | btlevtcmd_SetMoveSpeed | 3 / 0 / 0 |
| 8010e4bc | btlevtcmd_ResetFaceDirection | 1 / 0 / 0 |
| 8010e5a8 | btlevtcmd_ChangeFaceDirection | 3 / 0 / 0 |
| 8010e684 | btlevtcmd_CalculateFaceDirection | 3 / 0 / 0 |
| 8010ead8 | btlevtcmd_GetBodyId | 0 / 0 / 2 |
| 8010f200 | btlevtcmd_StartAvoid | 3 / 0 / 0 |
| 8010f988 | btlevtcmd_RecoverFp | 0 / 0 / 1 |
| 8010fa1c | btlevtcmd_RecoverHp | 0 / 0 / 1 |
| 80111cf4 | btlevtcmd_GetStatusMg | 0 / 0 / 2 |
| 80111dd0 | btlevtcmd_GetHeight | 0 / 0 / 2 |
| 801120a8 | btlevtcmd_GetHitPos | 2 / 0 / 0 |
| 801124f8 | btlevtcmd_GetHomePos | 4 / 0 / 0 |
| 80112d98 | btlevtcmd_GetPartsPos | 0 / 0 / 2 |
| 80112fcc | btlevtcmd_SetPos | 1 / 0 / 0 |
| 80113620 | btlevtcmd_GetPos | 3 / 0 / 0 |
| 8011384c | btlevtcmd_check_battleflag | 3 / 0 / 0 |
| 801138d8 | _get_flower_suitoru_point | 0 / 0 / 1 |
| 80113950 | _get_heart_suitoru_point | 0 / 0 / 1 |
| 80114614 | _battle_majinai_effect | 0 / 1 / 0 |
| 801146c4 | _majinai_powerup_check | 0 / 1 / 0 |
| 801153d8 | _jump_star_effect | 1 / 0 / 0 |
| 8012b8cc | evt_btl_camera_set_moveSpeedLv | 5 / 0 / 0 |
| 8012ba14 | evt_btl_camera_set_zoom | 4 / 0 / 0 |
| 8012bc70 | evt_btl_camera_set_homing_unitparts | 2 / 0 / 0 |
| 8012bfd0 | evt_btl_camera_set_mode | 6 / 0 / 0 |
| 80139898 | evt_audience_acrobat_notry | 2 / 0 / 0 |
| 80139af4 | evt_audience_ap_recovery | 2 / 0 / 0 |
| 801abdd0 | btlevtcmd_AnnounceMessage | 0 / 1 / 0 |

## Registry behavior and dependencies beyond the census

Required native services must resolve each original address to its faithfully
implemented callback,including unresolved camera/pose/sound/audience/effect/
status/data child paths. Missing implementations remain explicit missing bindings;
a no-op/completion2 substitute does not satisfy this list. There are72 identities,
not72 newly recovered callback contracts. Consult the
[Jump request graph](BATTLE_NORMAL_JUMP_EVENT_FLOW.md),
[attack badge tail](BATTLE_ATTACK_BADGE_RECOVERY_TAIL.md),
[action command services](BATTLE_ACTION_COMMAND_CALLBACKS.md),
[stylish requests](BATTLE_STYLISH_TIMING_AND_RESULTS.md) and
[packed alias requirements](EVENT_ENTRY_ADDRESS_AND_ALIAS_REQUIREMENTS.md).

NormalJump invokes unit-data child key7 through RunDataEventChild. Its actual
selected script depends on runtime unit data;the three packed streams do not
close that transitive domain. Other called callbacks can create wait/effect/
announcement/audio events,or use code/resource/data pointers outside these spans.
Those bodies,indirect pointers,REL callers and source writers require additional
registry/resource coverage. Literal ChildEvent/RUN/inline/brother opcodes also
have VM/lifecycle services beyond opcode91 counting. Script packaging alone
cannot supply those native services or their continuation results.

Callback E98/E18/count9/markerd caching,raw operand resolution,shared address
aliases,handler codes and manager credits must remain exact. FirstCall is not a
render frame;several invocations can share one sampled pad/time context. The
census records each static site once,regardless of loops/repeated polling.
Body hashes support address identity rather than a green fidelity verdict.

Acceptance:all156 sites map to72 addresses,all stream/callback fingerprints,
no duplicate/conflicting registry key,explicit unsupported bindings rather than
invented callback success,first-call/poll cache and each actual handler result,
original actor/context/owner binding and address aliases. Full source/callee/
transitive/runtime coverage and original Jump traces remain open under EVT-01.

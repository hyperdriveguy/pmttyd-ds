# Battle sequence get/set fields and init source bindings

Complete static US G8ME01 BattleGetSeq800f89a0/0x68 and
BattleSetSeq800f8a08/0x60.[Verifier](../../../tools/verify_battle_initialization_dispatch.py)
pins all instructions and complete adjacent8-entry dispatch tables at8034c6b0
(Get)/8034c6d0(Set).They serve the
[init sequence](BATTLE_INIT_SEQUENCE_DISPATCH_AND_RESOURCE_READINESS.md);
sequence words are separate from menu TYPE/current Input state.

## Exact8-selector map and unsigned rejection

Both compare selector I UNSIGNED against7.Negative signed words therefore
reject along with all values>7.No B-null guard on admitted selectors.

| Selector | Battle-relative WORD field | Get table target | Set table target |
| --- | --- | --- | --- |
| 0 | +04 | 800f89c0 | 800f8a28 |
| 1 | +0f0c | 800f89c8 | 800f8a30 |
| 2 | +0f10 | 800f89d0 | 800f8a38 |
| 3 | +0f14 | 800f89d8 | 800f8a40 |
| 4 | +0f18 | 800f89e0 | 800f8a48 |
| 5 | +0f1c | 800f89e8 | 800f8a50 |
| 6 | +0f20 | 800f89f0 | 800f8a58 |
| 7 | +0f24 | 800f89f8 | 800f8a60 |

Getter returns the selected word unchanged;rejected selector returns0.Setter
writes raw third argument V as a WORD;rejected selector performs no write.
Setter leaves passedB inr3 on every route,without explicit status result.It does
not increment sequence,reset other fields,run a handler,or schedule an event.
Do not implement SetSeq(B,I) with an inferred increment or translate sequence
values into menu-type enums.

## Concrete initialization chain now documented

[BattleInit stage-gate contract](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md)
initializes selector0 to0,selector1 to01000000,and requests registry/snapshot/
stage/audience services in order.The current verifier pins its body identity
and direct registry-reset site,not whole remaining service graphs.
[Registry reset/readiness](BATTLE_UNIT_REGISTRY_RESET_AND_INIT_EVENT_SCAN.md)
closes reset and later init-ID polling.[Complete Btl_UnitSetup](BATTLE_UNIT_SETUP_ACTORS_AND_REPEATED_IMPORTS.md)
closes the setup request at8011eee4 and explicit actor/import branches.
[Party snapshots](PARTY_BATTLE_SNAPSHOT_INITIALIZATION_AND_RAW_MAP.md),
[party switching](PARTY_SWITCH_SPAWN_HOME_AND_SNAPSHOT_EXPORT.md),and
[all party callbacks](PARTY_WEAPON_CALLBACKS_COMPLETE_SHARED_PROTOCOL.md)
supply the linked local provisioning/use contracts.

This is a static local chain,not a guarantee of all source selections,global
owner equality,resource readiness,actual event execution,or native complete
battle lifecycle.The exact three original effect keys `p_b_st`,`p_jyabara`,
`p_kaiten_h` are pinned as NUL-terminated bytes.Reuse original assets;their
request flag1 is literal,not a measured load duration or rewritten story action.

## Port requirements and remaining requests

Provide raw8 sequence fields,correct unsigned selector guards,raw-value stores,
explicit setter return ownership,state0 fall-through/state1..5 next-invocation
policies,and ordered resource/event requests through recovered caller bodies.
Meaningful checks include rejected negative selectors,carried third arguments
in init states0/4,full readiness scan after a pending event,completed ID clears,
and resource failure omitting later polls.No tests here assert original runtime.

Parent scheduler invocations,end reimport paths,source/field/global/resource
lifetime,selected-weapon query-event domains,Game/platform/original execution
and full-game requirements remain open.Do not mark full initialization complete
merely because these six local bodies and two scoped call sites are documented.

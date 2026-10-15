# Native same-VM field battle ownership

`FieldNpcBattleBinding` connects the [retained original callbacks](NATIVE_HARBOR_BATTLE_CALLBACKS.md)
to actual field storage and the retained OriginalSequence owner. It borrows the
same EventNativeRuntime and FieldNpcSetupBinding, retains their NPC owner, and
admits only the initialized field-context GP/F identity. A foreign VM, changed
GP/F, inactive caller, actor sentinel or missing registry actor cannot stand in.
The common original NPC-name resolver is shared with existing actor leaves.

Source fbatGetPointer reads the actual GP pointer and GP168. Start's F8 store
and information result at F30 operate on that same owner. Set/GetResult never
use the development FieldBattleWork or infer an outcome from HP. seqSetSeq calls
the supplied OriginalSequence.request with original parameters; seqGetSeq reads
its actual current value. A request does not set current or imply field return.
All Mario/party/audio children delegate to the stationary backend and retain
its own child continuation. Missing providers hold without successful defaults.

Regular ID queries read actual NPC11c. FieldNpcRegularEventHistory admits a
live same-context regular entry or an actually observed same-VM retirement with
matching actor/script. The field owner creates this history at map preparation,
before a late battle binding; real evtDelete snapshots record rawBytes0=1,
field170 and restartScript matching NPC124. Source ReInit/unload drops history.
A live reused ID takes precedence over old history; a nonregular reused-ID
retirement invalidates old proof. Unknown/foreign IDs hold; zero stays zero.

This history is necessary for the actual harbor: garawaru_regl21d28/f0 and
suifubomb_b_regl230c8/f0 can RETURN when GSF1179 is set, which the preceding
christine_osoware25ebc writes before battle entry. NPC11c retains the retired ID.
The source query must return it unchanged so DELETE_EVENT is a correct no-op.
Fresh original static evidence is in artifacts/harbor-native-npc-evidence;
this establishes the possible source path, not measured original timing.

Each held invocation pins caller address/ID/function, original first flag,
reached source captures and queued child arguments. Replacing live GP/F cannot
retarget the cursor. Completed source return0 ends an invocation; subsequent
source polls reacquire owners. Actual retirement quarantines a started native
parent because transitive child cancellation remains unbound. A pre-admission
cursor with no parent execution may dispose normally. Quarantine forbids further
invocations and preserves borrowed owners; destruction is not cancellation.

FieldAreaEvent exposes bindNpcBattle only after real map NPC/setup ownership.
Its dispatcher remembers first-call state even before late binding, observes
actual event retirement, stops manager completion around quarantine and refuses
map-NPC teardown while battle calls are pending. Battle binding retires before
setup/registry/VM mappings. Direct users must likewise retain setup, VM, sequence
and backend lifetimes until completion or explicit owner cancellation.

Tests use actual map registrations and npcSetup-created regular events. An
explicit synthetic control-flow fixture checks query return1 and next-visit
DELETE_EVENT against a real regular root in the same scheduler. Other fixtures
explicitly supply control/audio completion and current-sequence/result values;
these are not an original battle completion trace or production providers.
Actual deletion also exercises the retirement hook and held-child quarantine.
Additional fixtures retire a real regular root before late battle binding, keep
the exact stale ID, and reject/invalidate an explicit reused-ID alias.

Remaining: production source field bootstrap and all Mario/party/audio/battle
children, original battle scheduling/result/teardown, complete harbor NPC/sailor
setup and postbattle execution. This binding does not attach the independent
HarborEncounter adapter VM to the field scheduler, replace story scripts or close
RESULT-01/DEMO-01. It changes no assets/dialogue/audio or performance policy.

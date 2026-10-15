# Original battle sequence entry and field binding

Complete source parents seq_battleInit80073f20 and seq_battleMain80073b24 are
implemented in battle_sequence_entry. Their read-only complete listings, table,
constants and hashes live privately in artifacts/battle-sequence-init-ownership-evidence/.
Read the [lifecycle](../platform/ORIGINAL_SEQUENCE_TABLE_AND_FIELD_BATTLE_LIFECYCLE.md)
and [field start/return contract](../field/FIELD_BATTLE_ENTRY_AND_RETURN.md).
A parent completion does not acknowledge unbound resource, audio or engine children.

Init captures GP+c into original process8041e968, then captures F through fresh
GP168 and N through F8. It preserves separate GSW0 reads for109..110/402 routing,
BGMOff1024 before late retained-N2f0 BGMOn, ENVOff, rumble, actual event stop4,
sound flag80, a new F getter/type read and conditional Mario/party control-off.
Held GP168 reads retain their successful GP capture. Held services keep arguments
and never repeat acknowledged prefix effects. Init does not create BattleWork.

Main retains all twelve original table routes. State0/10 fades use original
color words and update freshly read work state only after acknowledgment.
State1 distinguishes fileAsyncf result0 polling from an unbound child; normal
and Luigi audience requests use separate DVD-root queries. Counter increment
occurs after both succeed and before fade-finished polling. State2 retains the
pre-child encounter type, controls/Mario flag clear and fresh F570 reset, then
falls3. State3 retains F/N/descriptor across pouch lookup, exact signed wait
conditions and freshly reread counter, rank-selected source bti string and late
descriptor+4. bmapLoad return is ignored. State4 calls actual battle_init800738ac;
its complete transitive graph remains required, with no success default.

State4 completion falls5 in the same invocation. State5 preserves end-check,
work mutation/debug override and Mario→camera4→listener/globalB flags→camera4→
fresh yaw→listener service order. Listener constants and optional f32 add use
original data. State6 increments first and requires signed>30, with no same-call
state6 increment after state5. Routes7..9/outside table return genuinely;10
fades22/200;11 polls fade, clears fresh GP+c and requests sequence5. A retained
callback survives waits without redispatching a mutated work state.

BattleSequenceBinding composes the parents with Game's actual OriginalSequence,
FieldNpcRegistryOwner and caller-owned EventNativeRuntime. Before initial effects,
it requires an active actual actor address in that owner's field allocation;
visual index1/3 and missing/unaligned actors do not become NPC pointers. Source
GSW0 comes from live GP saved switches, stopAllEvents4 uses that VM's actual
context, and SourceSoundFlags retains its fade cursor. Mario/party controls and
sequence requests use existing shared Game owners. Other services remain real
backend waits. Color/vector requests carry semantic local-stack contents, not
invented guest stack addresses; backends write the result field only.

Binding and Game/VM must retain stable addresses for the complete callback
lifetime; retire binding before VM or Game reset/relocation. Process8041e968 has
shared mutable4-byte backing and survives facade retirement/admitted checkpoint
handoff. Child cursors belong to that callback binding; unsaved mid-call history
is not reconstructed by a new facade or checkpoint. OriginalSequence publishes
current4 before Init and supplies the same work array to the binding. There is
no forced work5/6, field2 import or duplicate BattleWork. Binding/callback reject
an already-existing legacy Trio runtime, including one created during a wait.

This is an owner-composition API, not ordinary-game bootstrap completion. Native
Game still lacks a persistent shared field VM, full original tagged registration
for tik_01/c_kuribo, raw original NPC-name lookup/setup4 on that same actor, and
an authored battle_start bridge that supplies actual F8/F30. Its old visual-token
action snapshot cannot be certified by this binding. Connecting entry must
precede menu/action admission and consume one real BattleWork; never replay the
pending old Init after victory. The work owner
requires original npcInit's fourth580-byte allocation and actual GP168 publication;
aliasing allocations alone never publishes a new GP root or F pointer.

A private180-byte PMEVT data pack adds only original112-byte strings and32-byte
color/format/listener constants. No PPC, BSS, saved-mode, counter or readiness
fixture is exported. See [asset provenance](../../ASSETS.md). Validation and
remaining native source entry/resource ownership are recorded in
[PROGRESS](../../PROGRESS.md) and [EVT-01](../../requests/EVT-01.md).

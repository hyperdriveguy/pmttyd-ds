# Retained attack selection and native audience admission

US G8ME01 revision 0. This contract describes native suspension around the
statically recovered selector, not an original yielding API. Read the complete
[attack-event selector](BATTLE_ATTACK_EVENT_SELECTION.md),
[cursor/weapon selection](BATTLE_SELECTED_TARGET_AND_WEAPON.md),
[audience target and X interruption](AUDIENCE_TARGET_SELECTION_AND_NATIVE_INTERRUPTION.md)
and [present-item consumer](NATIVE_PRESENT_ITEM_CONSUMER.md).

## Original boundary and ordering

The original `_btlcmd_SetAttackEvent` is at `0x80120268`, size `0x5b4`.
`tools/verify_battle_attack_selection.py` pins eleven bodies, 301 anchors,
eight spans and seven scoped direct calls. Fresh read-only ISO disassembly in
`artifacts/selector-agent-audit/selector.txt`, `audience.txt` and `message.txt`
was reviewed for this adaptation; these ignored artifacts are local evidence,
not required distributable resources. This is static evidence, not a Dolphin
execution or timing measurement.

The original captures the override byte at `M+0x551`, then global battle B at
`0x8041eb90`, then type at `M+0x540`. The override applies only after all three
reads. Captured B owns the selector's counters and message flag/handle reads;
the audience helper independently rereads the current global battle pointer.

Types 11 and 12 require no weapon-decision flag or selected-weapon cursor.
Both first normalize signed byte `U+0x20`: 6 becomes 5, 7 becomes 6, 9 becomes
8, every other value becomes 4. Type 11 obtains the audience attack event from
the registry unit's data key 6, falling back to `0x80373d00`. It writes event
`U+0x2a0`, then category 3 at `U+0x2ac`. It next increments captured B's Mario
counter `+0x16f47` for kind 222, or other-kind counter `+0x16f48`, using the
original saturation-at-100 leaf. Only afterward does it test bit 0 at
`B+0x19090`; a set bit reads `B+0x19094` and calls message Continue. A clear
bit does not read the handle. The original tail anchors are event store at
`0x80120718`, category store at `0x80120720`, counter calls at
`0x80120738`/`0x80120748`, flag read at `0x80120750` and Continue call at
`0x80120760`.

Type 12 still reads `U+0`, although its helper ignores that argument. Through
a fresh global battle read it chooses damage event `0x80373d3c` when present
type `B+0x16040` is 1, otherwise get event `0x80373d74`, then publishes category
3. It neither increments these counters nor tests the message flag or handle.
These output publications select scripts; they do not create event roots.

## Retained native execution

`resumePackedBattleAttackSelection` owns a caller-supplied
`BattleAttackSelectionCursor`. A fresh call captures U, M, override, B and type
in the original read order. It also captures the memory service owner and
word/store-word/store-byte function identities, actor owner and byte-reader
identity. An active cursor rejects a changed U, M or any of those owners with
`false`, preserving the pending operation.

Phase 1 retains the dispatch inputs while an ordinary selected-weapon cursor
service is unavailable. A retry asks that service again using the original
captured menu and type. It does not recapture the override, global B or type.
After cursor acknowledgement, subsequent source memory reads and stores occur
at their original positions; this is not a snapshot of the entire menu or unit.

Phase 2 begins only after type 11 has normalized the byte, published the
script/category, updated its counter, tested its flag and captured the message
handle. Every retry calls only continuation for that captured handle. It does
not reread guest memory, normalize again, overwrite event/category again,
increment again, or follow a changed global, type, flag or handle. External
mutations during the hold remain intact. There is no rollback of acknowledged
source writes.

The optional `continueMessageReady` service returns native execution
acknowledgement. `false` retains phase 2; `true` completes and clears the cursor.
It is not the original helper's ignored return value and does not mean message
playback has finished. The older void `continueMessage` service acknowledges
synchronous invocation. With neither service bound, phase 2 remains pending.
Other completed branches also clear the cursor.

The convenience `packedSetBattleAttackEvent` wrapper uses a temporary cursor.
It is suitable only for services that complete synchronously. Retrying that
wrapper after an unavailable service loses retained state and can replay
already published type-11 writes/counts. Callers that may suspend must retain
a cursor and call the resume API. The existing trio weapon projection uses
ordinary selected-weapon modes; it is not a demonstrated audience dispatcher.

## Native admission and ownership limits

`HarborTutorialPacked::selectAttackEvent` supplies the persistent
`attackSelectionCursor` and invokes the original selector without its former
unconditional `weaponDecided` gate. This permits real type-11/type-12 selection
without fabricating a weapon decision. Its ordinary cursor service remains the
packed command cursor service. Successful selection clears `weaponDecided`;
a pending selection preserves its retained operation. `buildClassMenu` and
`buildWeaponMenu` reject admission while that cursor is active, before changing
command flags or menu state. This preserves ownership across retries and prevents
an unrelated menu build from completing an old continuation as a new command.
Memory pointer identity alone does not establish a new command generation.

The native adapter has no real message-window owner for `B+0x19094` yet. A
flagged type-11 selection therefore holds after the original writes/count,
without a successful no-op continuation or a guessed mapping to arbitrary
message playback. The original
[message lifecycle helper](../message/MESSAGE_WINDOW_LIFECYCLE.md) at
`0x8008156c`, size `0x50`, sets window state 1, ORs flags with 2 and refreshes
its last timestamp while preserving playback/parser state. A generic stopped
`MessagePlayback::resume` call does not prove ownership of this source handle.

This admission does not run AudienceAttack, AudienceMove, Get or Damage event
roots, validate their receipt owners, provide an original message pool, or
complete the outer command/Main scheduler. The legacy confirmed-weapon attack
harness remains distinct from original audience actor-phase choreography.
Physical X target interruption remains available through the linked contract;
optional lower-screen audience taps, live telegraphs, complete battle flow and
hardware performance remain unfinished. No HUD/art/dialogue asset is changed
by this selector adaptation.

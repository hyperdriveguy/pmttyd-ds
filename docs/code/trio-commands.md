# Trio command selection and retained action manager

[TrioBattleCommands](../../include/battle/trio_battle_commands.hpp) is a native
menu around existing source candidate samplers. Its [implementation](../../source/battle/trio_battle_commands.cpp)
produces an explicit pending attack request; it does not execute an event.
See [candidate/selection helpers](typed-battle-support.md),
[weapon menus](battle-weapon-menus.md) and [Trio provider composition](trio-providers.md).

## Records and actor domains

`PMTCMD2` contains exactly three little-endian records: Mario Jump, Mario Hammer
and Goombella's default command. Each holds original weapon address, admission/
attribute words, 16-bit icon domain and a terminated 64-byte original message
key. The loader requires all original global labels and exact EOF before
publishing new command/label state. It does not author or paraphrase dialogue.

Actor IDs 1/2 are Mario/Goombella, kinds 222/224. Slot 0 is unused. Spec rows map
to registry IDs 3–6, while story visual indices are Mario 0, Goombella 1,
Frankly 2, Goomba 3, Spiky Goomba 4 and Paragoomba 5. The registry-to-visual map
is `{0,0,1,3,5,4,2}`. Runtime kind determines target labels, including wing-loss
changes; the initial slot alone cannot choose the current name.

Mario has two menu rows; Goombella's one row maps catalog index 2. Mario command
icons query actual pouch upgrade bytes through existing source icon helpers;
Goombella uses the original fixed command icon. Missing pouch remains unavailable.

Source candidate storage differs from the native metadata format:

| Buffer offset | Meaning |
| --- | --- |
| `+0xa6c` | Signed byte candidate count |
| `+0xa6d` | Permutation bytes selecting physical records |
| `+4`, stride 36 | BE signed16 unit/part IDs at record+0/+2 |
| `+0xab7` | Selected permutation index |

`target` rejects absent/out-of-range indices and returns `{-1,-1}`. It uses the
stored permutation rather than physical record order. `refreshTargets` preserves
the selected `(unit,part)` identity if present after source refresh; otherwise it
keeps the source sampler's resulting selection. Synchronize actual unit/alive/
part state before sampling; `begin` only creates the scoped initial view.

Sampling position/part/unit views are rebound to this object's arrays for each
service construction. A copied Game/fixture must not retain pointers into the
old owner. The native view admits only body part 1, checks actual presence on
unit lookup and delegates status 27/world position to existing helpers.

## UI state transitions

`Closed`, `Command`, `Target` and `AttackReady` are native states, not source
sequence-bank numbers. Repeat masks `0x80000/0x40000` increment/decrement with
wrap. Both bits run in order if both occur. Trigger `0x100` confirms; `0x200`
cancels. Command cancellation wins over simultaneous confirmation. Target input
refreshes candidates and applies movement before cancellation/confirmation.

Confirmation with actual targets publishes the original weapon token and
selected registry unit/part into `attack`, then enters `AttackReady`. Ownership
remains pending until an event consumer accepts dispatch. Its B/cancel path is
an explicit development return to target selection, not original battle-ending
or story behavior.

## Retained action-manager invocation

[BattleActionManagerCursor](../../include/battle/battle_action_manager_continuation.hpp)
wraps [the complete local manager](../../source/battle/battle_action_manager_continuation.cpp).
The immediate helper can return unavailable after pad/Main effects; restarting
it is not automatically safe. This cursor retains one invocation's Battle token,
exact service identity, phase and pending child arguments. Each child still owns
its own unfinished effects. Reset drops metadata without rolling back guest work
and refuses reentry; execution also rejects recursive invocation or provider swaps.

| Native phase | Retained operation |
| --- | --- |
| 0 | Private pad `B+0x1d1c`, channel 0 |
| 1 | Capture Main from `B+0x1ca0`; null Main completes without Display |
| 2 | Invoke retained Main; absent optional holds, engaged zero clears Main |
| 3 | Fresh Display at `B+0x1ca8`, then float-priority bits at `0x80422d68` |
| 4 | Queue retained Display on camera 8, class 1, argument B |

Pad is never replayed after acknowledgement. Main pointer does not change while
its child is held. Display is read after Main effects, including Main clearing
itself on result zero, and can still register that visit. Priority and Display
pointer freeze across a registration hold. Completion destroys retained state;
the next invocation starts at pad admission again.

`pending()` reports native diagnostic tokens: manager `0x800fa5b0`, actual Main
callback or display entry `0x80010bc4`. A diagnostic zero is not independently
proof of inactive state. Source callback slots are Main `0x1ca0`, Result `0x1ca4`,
Display `0x1ca8` and Delete `0x1cac`; the earlier Harbor comments naming Init here
were incorrect and are corrected with this pass.

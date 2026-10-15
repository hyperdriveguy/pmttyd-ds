# Retained command-input invocation

[CommandInput](../../source/battle/battle_command_input_continuation.cpp) retains
all thirty original states and same-invocation branches. `nullopt` holds the exact
missing child; engaged 0 completes one source simulation invocation. It does not
wait for eventual player confirmation. Actual child owners retain partial work,
and callers retain allocations/context while provider identities stay fixed.
Reentry/reset during a callback is refused. Reset does not undo source effects.

Entry captures B, acting U and wrapped M=B+0x171c. Wrapped M=0 returns 1 after
unit lookup. GetCursorPtr(M,14) owns this invocation's native optional SP08,
initially unknown rather than zero. Only cursor requests expose its address.
A successful unsupported query may leave it unknown; an actual zero write is
known data. Consumption holds at its first dereference without repeating an
acknowledged lookup or preceding source mutation. Sibling menu/updater outputs
are separate cells.

M+0 is STATE, while M+0x540 is TYPE. The original PC table dispatches unsigned
states 0..29; other values reach the common tail. The
[complete state map](../behavior/battle/COMMAND_INPUT_AUXILIARY_STATES_AND_ATTACK_DISPATCH.md)
describes all entries. Important groups are:

| States | Local ownership |
| --- | --- |
| 0..2 | Class setup/Main and actor-mode bypass |
| 3/4, 8/9, 17/18, 22/23 | Reason event creation/wait, then destinations 1/6/15/20 |
| 5/6, 7 | Weapon setup with distinct flags, then weapon Main |
| 10, 11 | Selected-weapon help setup, then target remap/input |
| 12, 13 | Multi-item cancel setup and separate Main visit |
| 14/15, 16 | Multi-item setup and immediate Main |
| 19/20, 21 | Operation setup and immediate Main |
| 24/25, 26 | Party selection setup and immediate Main |
| 27, 28 | Protect-partner setup and controls |
| 29 | Attack selector, fresh flag clears, OffLight and direct return 1 |

Reason creation stores its wait state before evtEntry(0x80416dc4,0,32), reads
entry+0x15c and stores ID at M+0x568. It immediately falls into the wait code,
which queries the current stored ID. There is no local null-entry guard. Setup
pairs retain the entry state for flag 1/0 even if a callback changes M+0. Do not
add a one-state-per-render-frame rule.

State 11 captures the old target ID/part before sampling refresh. Target bank
B+0x428 uses signed selected byte +0xab7, signed permutation +0xa6d, 36-byte
pair records at +4, and signed halfword ID/part. After refresh, signed count
+0xa6c selects the first matching pair or ordinal 0. The remap store survives a
later unavailable cursor or child.

Weapon flag 0x01000000 permits independent forward/reverse repeat queries
0x80000 then 0x40000, with byte wrapping, signed fresh counts and source sounds.
Cancel trigger 0x200 precedes confirm 0x100 and skips confirm/audience if accepted.
Confirm stores state 29 before target End, retains entry type for later cost
branches, reads fresh actor mode/operation and still visits the physical-X
audience helper. Protect state 28 has its own smaller cancel/confirm sequence;
it does not borrow state 11's help/sound policy.

The common pending-byte path reenters dispatch without clearing progress. Ordinary
progress reentry clears progress first. Otherwise return is engaged 0. State 29
bypasses that tail: select attack with retained U/M, freshly clear M+4 bit
0x01000000 and B+0xef8 bit 1, call OffLight(0,0), return 1. These are local source
operations, not proof of executed attack damage, turn consumption or event return.
See [retained input ownership](../behavior/battle/RETAINED_COMMAND_INPUT.md).

# Battle commands and action-command callbacks

This guide covers the committed command-input, action-command manager/help/
feedback, attack-selection/registration, command-pose and weapon-power helpers.
See [weapon menus](battle-weapon-menus.md) for row producers and target selection,
[battle controllers](battle-control.md) for the outer sequence and
[battle core](battle-core.md) for damage and typed action-command mechanics.
Addresses and `+0x...` offsets identify guest storage, never native pointers.
These comments explain the implementation; recovered evidence belongs in the
[behavior contracts](../behavior/README.md). Provider admission does not establish
complete battle integration or original SDK/Gekko arithmetic parity.

## Result domains and ownership

| Value | Meaning in its own API |
| --- | --- |
| Action result mask `1` | Result still pending |
| Action result mask `2` | Success bit, also used to choose weapon power |
| EVT result `0` / `2` | Retry / completed instruction |
| Help mode `0` / `2` | Hidden / requested display |
| Action main state `100` | Start publication, even when resolved `on` is zero |
| `std::nullopt` | Required native provider unavailable |
| Engaged optional `0` | A real scalar result; its caller decides its meaning |

Typed views borrow fields; packed routines use callbacks over guest memory.
Callbacks can change owners or the bytes subsequently read. Do not hoist fresh
reads or combine read/modify/write operations just because the same offset occurs
twice. Immediate parents have no rollback or saved suffix. A failed provider can
follow sound, state, counter or event effects; callers need proper admission.
Retained attack-selection and audience cursors are the exceptions described below.

`SetACParam` resolves eight operands and preserves an existing lane only when the
resolved value is `-250000000`. `SetACFlag` replaces its raw setup mask. Timing
options resolve their on/off operand but OR/clear a raw mask. Output operands
remain raw destinations. Stop calls the optional removal provider before hiding
help; it does not independently clear result, count or callback storage.

The packed manager always updates private pad `B+0x1d1c`, channel zero. A null
main callback at `+0x1ca0` skips display work entirely. An actual main callback
returning zero clears that slot, then a fresh `+0x1ca8` display callback can still
register on camera 8, class 1 with argument B and priority from `0x80422d68`.
The result-event wrapper ignores evaluation's scalar: it freshly tests pending
bit 1 at `+0x1cb8`, hiding help only when ready. The callback-free route retains
the earlier discarded success-bit read. Help mode publication and its subsequent
audience-window byte write independently resolve the current battle owner.

## Input-state dispatch

`battle_command_input.cpp` dispatches through menu `B+0x171c`. Its service enum
names the original child operations; their scalar domains differ. It restores
presentation before its loop and resolves a private cursor using type 14.

| Menu state | Local work |
| --- | --- |
| 0, 1, 2 | Initial command pose/class setup, then class main |
| 3/4, 8/9, 17/18, 22/23 | Create/wait for original root `0x80416dc4`, then return to class/weapon/multi/operation setup |
| 5, 6, 7 | Weapon setup/main; forced Jump/Hammer flags can directly decide target |
| 10, 11 | Publish weapon help, then target selection |
| 12, 13 | Multi-cancel setup/main |
| 14, 15, 16 | Multi-action setup/main |
| 19, 20, 21 | Operation setup/main |
| 24, 25, 26 | Party setup/main |
| 27, 28 | Protect setup, cancel `0x200` or confirm `0x100` |
| 29 | Select attack, clear command flags, turn lights off, return 1 |

Setup states deliberately fall through into the corresponding main child where
written. A progress result restarts dispatch in the same visit. A nonzero battle
marker `+0x1c74` restores presentation and loops without resetting progress; do not
replace this with one state per render frame. A stable visit without progress
returns zero. This loop has no retained native continuation.

## Attack selection and root registration

Selection publishes unit event `+0x2a0` and category `+0x2ac`; registration later
creates the root. Ordinary types 0/1/2/4/6 take a 28-byte weapon row and script
`weapon+0xb0`; a null ordinary script falls back to Wait `0x80416b40`.
Override byte `menu+0x551` resets unit state to zero and selects Wait/category 1.
Operation type 5 has its own fixed scripts and counters: data-key-2 subcase 8
retains category 3 even with a null script. Type 9 publishes state 1/category 7.
Types 11/12 rewind state `6→5`, `7→6`, `9→8`, otherwise 4. Type 11 resolves
unit data key 6 (fallback `0x80373d00`) and may suspend for message `B+0x19094`.
Its retained phase 2 resumes only acknowledgement, preserving prior state/counters.
The synchronous wrapper discards that cursor and is unsuitable for replaying a
held message prefix. Action record bytes increment only below 100; larger values
are preserved. These counters and source cases are not invented gameplay limits.

State-6 attack registration performs a fresh actor lookup after possible window
closure. Forced flags choose `0x8034f234` before status gates. Category mask
`0x10` guards null scripts; category mask 2 can request creation with script zero.
Statuses 1/2/9 block and 27 blocks unless attribute `0x10000` is set. Status 6
still consumes `irand(100)` with a strict `<0` test: ordinary nonnegative RNG
cannot take that branch, but removing the call changes RNG history. Status 5
uses `<50` or unit flag `0x10` to choose the alternate event at `+0x2a4`.
Only a created entry gets its ID/owner and removes the wait event. Channel 6
advances even on blocked or null-entry routes.

## Feedback, pose and power

Audience work is inline at `B+0x2858`; 200 actor rows start at `+0x1bc`, stride
`0x134`. Existence requires flag 1 and excludes `0x100/2`; system control also
excludes `0x10`. Waiting additionally needs halfword `+0x1c == 0xffff` and unsigned
state byte `+0x19 <= 3`. Nested checks recapture current owners, while the eventual
flag write uses the earlier captured audience owner. Good/bad counters at
`B+0x16f58/59` increment only below 200. Result `-1` alone selects bad feedback;
good feedback publishes weapon/multiplier/lane bytes, bad preserves old values.
Requests precede audience good/bad flags `0x10/0x20`. The `(180,120)` request
arguments are source parameters, not measured timing. Acrobat-no-try performs
only the original global lookup.

Command pose scans for body-part attribute 1 and returns a definition ID. Status
attribute `0x20000` selects pose 27; the signed table at `0x80416ab0` terminates
at -1 and defaults to 28. Only pose 28 chooses animation type 67. Name resolution
and copying precede flags; separate flag accesses preserve callback ordering.

Weapon power uses success bit 2 and signed low-byte level. Its wrapped index is
`(level-1)*2 + success`, upper-clamped to 5 with no lower clamp. Level zero can
therefore request a negative provider index. The bonus callback runs before the
power-word read even with bonus zero. Ordinary Jump/Hammer factory arrays encode
three failure/success pairs; their masks `0x35807`/`0x30887` are original damage
modifier masks, not menu flags. Factories are minimal records and require bound
callback data; they do not infer live pouch/action-command ownership.

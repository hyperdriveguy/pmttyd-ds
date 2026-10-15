# Harbor packed state and command services

[HarborTutorialPacked](../../include/battle/harbor_tutorial_packed.hpp) borrows an
actual event VM and supplies bounded native storage/providers around recovered
battle routines. Its [packed implementation](../../source/battle/harbor_tutorial_packed.cpp)
keeps original field offsets even where allocation is native policy. See
[composition](harbor-composition.md), [service adapters](harbor-service-adapters.md),
[weapon menus](battle-weapon-menus.md), [commands](battle-commands.md) and
[packed motion](packed-battle-motion.md) for linked behavior.

## Native arenas and original records

| Guest address | Native storage policy |
| --- | --- |
| `0x84000000` | Battle work, `0x19400` bytes |
| `0x84100000` / `0x84200000` | Unit/part arenas, `0x4000` bytes each, slot stride `0x1000` |
| `0x84400000` | Fresh pouch, original size `0x5d4` |
| `0x84500000` / `0x84501000` | Scoped field/NPC work, `0x580` / `0x340` bytes |
| `0x84600000` | Private menu arena (`MenuRam`), `0x4000` bytes |

These are emulated addresses. Host pointers must never be cast into them.
Original globals `0x8041eb90` and `0x8041eb00` hold BattleWork/pouch pointers.
VM byte/half/word access retains big-endian transport. Required-access failure
aborts; silently returning zero could acknowledge a source operation that never
ran. Float words preserve bits through `memcpy` rather than numeric conversion.
Raw event operands remain distinct from decoded values/output destinations.

Only four registry IDs are provisioned, although the source registry retains
64 entries. Kind part count must be 1–3. Definitions use stride `0x4c`; native
part work uses `0x500` within the unit's part slot. Each part links the next and
its owning unit at `+0x4ec`. Source kind attributes, defenses and pose tables
are copied into their original part fields. Definition flag 1 chooses the body
ID. This is not full original allocation, equipment or display initialization.
The original Mario kind pointer is loaded from `0x80416a50`.

Installed script pointers and running event handles differ. Unit fields
`0x28c/0x298/0x2a0/0x2a4/0x2b0/0x2b8` hold wait/phase/attack/confusion/damage/
entry scripts, while `0x288/0x29c/0x2a8` record init/phase/attack event handles.
Starting an attack deletes the wait-child handle at `0x290`. Hit activity
observes actual active events at handle `0x2b4`; merely finding no hit event does
not establish completed phase/story work.

`publish` writes selected scene fields, resolving registry IDs independently of
visual slots. `capture` reads back selected fields and moves queued effects,
sounds, rumble and audience notices into scene vectors, clearing each local
queue once. Neither copies all guest state. A pending child that owns captured
work needs its own scheduler protection against a fresh projection.

## Menu scratch and table projection

| `MenuRam` offset | Use |
| --- | --- |
| `+0/+4/+8` | Builder cursor, count and eligibility cursor outputs |
| `+0xc/+0x10` | Weapon setup/input cursor outputs |
| `+0x14/+0x18/+0x1c` | Audience item, command Input and act-menu cursor outputs |
| `+0x20` | Retained present-item XYZ |
| `+0x40` | Separate candidate buffer for target-count queries |
| `+0xc00` | Fixed legacy weapon UI projection |
| `+0x1000..+0x3fff` | Stable cached original message text |

[Menu construction](../../source/battle/harbor_tutorial_menu.cpp) caches original
label bytes by guest key, including NUL and rounding allocation to four bytes.
It does not evict because published rows retain guest pointers. Arena exhaustion
aborts. Cost eligibility uses actual Mario FP/AP and equipped badge queries;
invalid nonfinite/out-of-range power conversion returns unavailable rather than
turning an attack into a free action. Enemy allegiance swaps signed 0/1 while
preserving other byte values.

Source menu starts at `B+0x171c`. Class rows have 20-byte stride but icon/reason
fields at `+0x14/+0x18`, overlapping the next row's prefix as in the recovered
layout. Do not replace this with `sizeof` a native row struct. Class cursor is
`menu+0x498`, count at cursor+8 and at most seven rows. Weapon rows start at
`menu+0x80`, stride 28 with weapon/enabled/name/icon/ordinal/item/reason fields;
the projected visible count is bounded to 64. Scene strings/rows are copies,
not ownership of the source guest records.

Weapon decisions project signed candidate count at buffer `+0xa6c` and signed
permutation bytes at `+0xa6d`, then read signed unit/part IDs from 36-byte records.
Preserve permutation order. An engaged decision, including source result zero,
admits the projection. Failed admission leaves previous scene targets intact.
Retained attack selection blocks table rebuild and consumes the native decision
latch only after whole completion; it does not create an attack root.

## UI and command ownership

[Act-menu services](../../source/battle/harbor_tutorial_act_menu.cpp) allocate
actual class UI through SDK heap 0 and track both address and fresh lifetime
token. Release rejects unknown addresses; the fixed legacy weapon projection
is an explicit separate exception. Scene open/closed state changes after actual
Init/End acknowledgement. Row capture can reflect partial source work even when
a visit is unavailable, so populated rows do not prove completed setup.

[Command Input](../../source/battle/harbor_tutorial_command.cpp) uses synchronous
stack contexts, with optional zero representing completed source work. A missing
child records its function token and the immediate Input path latches that
unavailability. Subsequent calls do not replay partially completed work. Weapon
table rebuild clears the latch only after attack-selection ownership allows
entry; fresh-pouch/type checks can still reject the rebuild afterward.

Restore follows original `B+0x1c70/0x1c74/0x171c` save/gate/live-state rules and
reloads the actual global BattleWork pointer. Trigger/repeat/current-held words
are separate. Target-payment service `0x80126840` remains unavailable here;
a zero-looking cost is not proof that payment ran. The fallback diagnostic
`0x801229d8` identifies Input itself when no child diagnostic exists. Result 1
latches command finalization without scheduling a root or synthesizing HP.

Command pose copying requires a nonempty NUL-terminated original name fitting
64 bytes in part `+0x1c4`, with owner resolved through `+0x4ec`. Repeated identical
pose publication does not restart the native animation clock. Original `blr`
light/display leaves are explicitly admitted; that does not license substituting
empty callbacks for other missing services.

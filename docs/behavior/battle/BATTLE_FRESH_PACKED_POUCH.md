# Original fresh packed pouch

`pouchInit800d59c8` allocates heap0 size5d4, publishes pointer at8041eb00 and
clears the full block. It then explicitly clears 121 key-item halfwords at+a0,
20 item halfwords192, 32 storage halfwords1ba, 200 badge halfwords1fa and
200 equipment halfwords38a. These are source facts; current native facade items
and legacy save layouts do not establish any original equipment state.

Nonzero fresh scalar fields are source HP/maxHP10 at70/72, baseHP10 at8e,
FP/maxFP5 at74/76, baseFP5 at90, BP3 at92/94 and level1 at8a. Source HP and FP
copies follow the maximum-field initialization. Boots/hammer bytes98/99 are0.
Source float84 is0, read-only constant8042187c. Partner records have stride14;
record0 remains zero and records1..7 initialize their halfwords2/4/6 to10, with
0/A/C set0. This does not merge the neutral harbor NPC helper with a party slot.

The final source calls msgSearch on original key `name_party3` at802cc728 and
strcpy to5c0; this is the localized default Yoshi name, not a language code.
`initializePackedPouch` requires explicit allocation, big-endian write services
and a message provider for that exact source key. The allocator owns mapping the
1492-byte block and8041eb00 pointer slot. It returns the new original pouch
address. The native boundary asserts the provided name fits the original20-byte
storage; valid original US text follows the original copy including trailing NUL.
No message text or source bytes are committed.

`packedPouchEquippedBadgeCount` recovers pouchCheckEquipBadge800d3300: it scans
all200 signed halfwords at38a and counts exact signed-ID matches, without treating
zero as a sentinel. Consequently a fresh query for327/328 returns0 while ID0
returns200. This is the original provider consumed by Mario's emblem initializer.
The source pouchGetPtr800d5eb0 simply reads the published pointer8041eb00.

These helpers provide genuine fresh state. They are not automatically run at each
battle, do not borrow native Progress inventory and do not invent later field
item/equipment mutations. The owner must run new-game initialization at its
source lifecycle point, preserve its state, and pass actual entry state. The
explicit harbor beginFresh entry now connects this verified new-game state and
runs original Mario init80372ec4; parent field/battle persistence and full player
command/reaction routing remain open. See [Mario init](../story/NATIVE_HARBOR_MARIO_INIT.md).
Parent's explicit original harbor story provider is4; source story201..212 branch
therefore remains false. It must not use provisional facade inventory as pouch.

Focused tests verify source allocation/fields, partner0 distinction, full zero
inventory/badge state, exact200-slot equipment scan, last-slot inclusion and
signed IDs. `verify_harbor_pouch_services.py` pins initializer, count/get-pointer
bodies and zero constant. This is binary inspection and portable testing, not a
Dolphin measurement or completed native battle.

See [counter/hit ownership](BATTLE_PACKED_COUNTER_AND_HIT_EVENT.md) and
[harbor physical defense](../story/NATIVE_HARBOR_PHYSICAL_DEFENSE.md).
